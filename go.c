#include <stdio.h>
#include <stdlib.h>
#include "pdp11.h"
#include <stdarg.h>

int N, Z, C;
int r1, r2;
int XX;

struct SSDD ss, dd, NN;
struct SSDD get_mode_reg(word w, int b);
byte mem[MEMSIZE];
word reg[8];
int bit;

void trace (const char * fmt, ...) {
	
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}

void load_file(const char * sum) {
	
	FILE * test = fopen(sum, "r");
	Adress a;
	unsigned int bt;
	word N;
	
	if (test == NULL) {
		
		perror("sum");
		exit(1);
	}
	
	
	while (fscanf(test, "%hx%hx", &a, &N) == 2) {
		
		for(int i = 0; i < N; i++) {
		
			fscanf(test, "%x", &bt);
			b_write(a+i, bt);
		}
	}
	
	mem_dump(01000, N);
	
	fclose(test);
}

void mem_dump(Adress A, word N) {

	int i = 0;

	trace ("---------------mem_dump----------------\n");
	
    for (i = 0; i <= N; i += 2)
		trace("%06o : %06o\n", A + i , w_read(A + i));


}

void print_reg()
{
	trace("\n");
	
	int i;
	for (i = 0; i < 8; i += 2)
		trace ("R%d = %06o   ", i, reg[i]);
		
	printf("\n");
	
	for (i = 1; i < 8; i += 2)
		trace("R%d = %06o    ", i, reg[i]);
		
	trace ("\n\n");

}

struct Command commd[] = {
	
	{0170000, 0010000, "mov",  do_mov,  HAS_DD | HAS_SS},
	{0170000, 0060000, "add",  do_add,  HAS_DD | HAS_SS},
	{0170000, 0110000, "movb", do_bmov, HAS_DD | HAS_SS},
	{0177777, 0000000, "halt", do_halt, HASNT_PARAM},
	{0177000, 0077000, "sob",  do_sob,  HAS_NN},
	{0017700, 0005000, "clr",  do_clr,  HAS_DD},
	{0177400, 0000400, "br",   do_br,   HAS_XX},
	{0177400, 0001400, "beq",  do_beq,  HAS_XX},
	{0177400, 0100000, "bpl",  do_bpl,  HAS_XX},
	{0177700, 0105700, "tstb", do_tstb, HAS_DD},
	{0177700, 0005700, "tst",  do_tst,  HAS_DD},
	{0177000, 0004000, "jsr",  do_jsr,  HAS_DD | HAS_R1},
	{0177770, 0000200, "rts",  do_rts,  HAS_R2},
};



void NZVC (word w) {
	
	N = (bit ? (w >> 15) : (w >> 7)) & 1;
	Z = (w == 0);
	C = (bit ? (w >> 16) : (w >> 8)) & 1;
    
	if (N == 1) 
		trace("N");
	if (N != 1) 
		trace("-");
	if (C == 1)
		trace("C");
	if (C != 1)
		trace("-");
	if (Z == 1)
		trace("Z");
	if (Z != 1)
		trace("-");
}

struct SSDD get_NN (word w) {
	
	struct SSDD res;

	res.adr = (w >> 6) & 07;
	res.val = w & 077;

	trace ("R%d, %o", reg[NN.adr], pc - 2 * NN.val);

	return res;
}

struct SSDD get_mode_reg(word w, int b) {
	
	struct SSDD res;
	int r = w & 7;					// номер регистра
	int mode = (w >> 3) & 7;		// номер моды
	word X;
	
	switch(mode) {
		
		case 0:
		
			res.adr = r;
			res.val = reg[r];
			trace ("R%o ", r);
			break;
		
		case 1:
		
			res.adr = reg[r];
			res.val = b ? b_read(res.adr) : w_read(res.adr); 
			trace ("(R%o) ", r);
			break;
		
		case 2:
		
			res.adr = reg[r];
			res.val = b ? b_read(res.adr) : w_read(res.adr);

			if (r == 7) 
				trace ("#%o ", res.val);
			
			else 
				trace ("(R%o)+ ", r);
				
			if (r == 7 || r == 6 || b == 0)
				reg[r] += 2;
				
			else 
				reg[r]++;
				
			break;
			
		case 3:
		
			res.adr = reg[r];
			res.adr = w_read (res.adr);
			
			if (r == 7 || r == 6 || b == 0) {
				
				res.val = w_read (res.adr);
				trace ("@#%o", res.val);
				reg[r] += 2;
			}
			
			else {
				res.val = b_read (res.adr);
				reg[r] += 2;       // reg[r] ++;
				trace ("@(R%o)+", r);
			}
			
			break;
			
		case 4:
		
			if (r == 7 || r == 6 || b == 0) {
				
				reg[r] -= 2;
				res.adr = reg[r];
				res.val = w_read (res.adr);
			}
			else {
				reg[r]--;
				res.adr = reg[r];
				res.val = b_read (res.adr);
			}
			
			trace ("-(R%d) ", r);
            
			break;
		
		case 5:
		
			trace ("@-(R%o) ", r);
			reg[r] -= 2;
			res.adr = reg[r];
			res.adr = w_read (res.adr);
			res.val = w_read (res.adr);
			
			break;
		
		case 6:
				
			X = w_read(pc);
			pc += 2;
			res.adr = (reg[r] + X) & 0xFFFF;
			res.val = w_read(res.adr);
			
			if (r == 7) 
				trace ("%o ", res.adr);
				
			else 
				trace ("%o(R%d) ", X, r);
				
			break;
			
		default:
				fprintf (stderr, "Mode %o NO IMPLEMENTED YET\n", mode);
				exit(1);
		}
		
	return res;
}

void run() {
	
	trace ("\n-------------running-------------\n");
	
	pc = 01000;
	w_write(ostat, 0xFF);
	while (1) {
		
		word w = w_read(pc);
		trace ("%06o : %06o \n", pc, w);			//отладочная печать
		//trace ("%06o : ", pc);						//обычная печать
		pc += 2;
		int i;
		int size = sizeof(commd)/sizeof(struct Command);
		struct Command cmmd;
		
		for (i = 0; i < size; i++) {
			
			cmmd = commd[i];
			
			if ((cmmd.mask & w) == cmmd.opcode) {
				
				trace ("%s    ", cmmd.name);
				
				if (cmmd.param & HAS_R1) {
					
					r1 = (w >> 6) & 7;
				}
				
				if (cmmd.param & HAS_R2) {
					
					r2 = w & 7;
				}
				
				if (cmmd.param & HAS_SS) {
					
					ss = get_mode_reg (w >> 6, w >> 15);
					trace ("\n ss = %o, %o\n", ss.val, ss.adr);
				}
					

				if (cmmd.param & HAS_DD) {
					
					dd = get_mode_reg(w, w >> 15);
					trace ("\n dd = %o, %o\n", dd.val, dd.adr);
				}
				
				if (cmmd.param & HAS_NN) {
					
					NN = get_NN (w);
				}
				
				if (cmmd.param & HAS_XX) {
					
					XX = (char)(w & 255);
				}
			
				cmmd.do_func();
					
			}
		}
						
		trace ("\n");
		print_reg();
	}
}
