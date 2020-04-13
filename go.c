#include <stdio.h>
#include <stdlib.h>
#include "pdp11.h"
#include <stdarg.h>

int b_or_w;
int N, Z, C;

struct SSDD ss, dd, NN;
struct SSDD get_mode_reg(word w, int b);
byte mem[MEMSIZE];
word reg[8];

void trace (const char * fmt, ...) {
	
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}

void load_file() {
	
	FILE * test = NULL;
	Adress a, A;
	unsigned int bt;
	word n, N;
	test = fopen("/home/ira/информатика/workpdp/01_sum.pdp.o", "r");
	
	if (test == NULL) {
		
		perror("01_sum.pdp.o");
		exit(1);
	}
	
	fscanf(test, "%hx%hx", &a, &N);
	
	
	for(int i = 0; i < N; i++) {
		
		fscanf(test, "%x", &bt);
		b_write(a+i, bt);
	}
	
	fscanf(test, "%hx%hx", &A, &n);
	
	
	for(int i = 0; i < n; i++) {
		
		fscanf(test, "%x", &bt);
		b_write(A+i, bt);
	}
	
	fclose(test);
}

struct Command commd[] = {
	
	{0170000, 0010000, "mov", do_mov, HAS_DD | HAS_SS},
	{0170000, 0060000, "add", do_add, HAS_DD | HAS_SS},
	{0170000, 0110000, "movb", do_bmov, HAS_DD | HAS_SS},
	{0177777, 0000000, "halt", do_halt, HASNT_PARAM},
	{0177000, 0077000, "sob",  do_sob,  HAS_NN},
	{0017700, 0005000, "clr",  do_clr,  HAS_DD},
	{0177400, 0000400, "br",   do_br,   HAS_XX},
	{0177400, 0001400, "beq",  do_beq,  HAS_XX},
	{0177400,  0100000, "bpl",  do_bpl,  HAS_XX},
};


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


word byte_or_word(byte b) {

    word w;
    if (SIGN(b, 1) == 0) {
        w = 0;
        w |= b;
    }
    else {
        w = ~0xFF;
        w |= b;
    }
    return w;
}


struct Operand create(word w) {
    struct Operand res;

    res.Byte = (w >> 15);
    res.r1 = (w >> 6) & 7;
    res.r2 = w & 7;
    return res;
}

void NZVC (struct Operand psw) {
	
	
	if (psw.Byte) {
		
		N = (dd.res >> 7) & 1;
		C = (dd.res >> 8) & 1;
	}
	
	else {
		
		N = (dd.res >> 15) & 1;
		C = (dd.res >> 16) & 1;
	}
	
	Z = (dd.res == 0);
}

struct SSDD get_NN (word w) {
	
	struct SSDD res;

	res.adr = (w >> 6) & 07;
	res.val = w & 077;

	printf("R%d, %o", reg[NN.adr], pc - 2 * NN.val);

	return res;
}

	

struct SSDD get_mode_reg(word w, int b) {
	
	struct SSDD res;
	int r = w & 7;					// номер регистра
	int mode = (w >> 3) & 7;		// номер моды
	
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
			
			if (r == 7) {
				
				res.adr = reg[r];
				res.val = b ? b_read(res.adr) : w_read(res.adr);
				trace ("#%o ", res.val);
			}
			
			else {
				
				res.adr = reg[r];
				res.val = b ? b_read(res.adr) : w_read(res.adr);
				trace ("(R%o)+ ", r);
			}
				
			if (r == 7 || r == 6 || b == 0)
				reg[r] += 2;
			else 
				reg[r]++;
				
			break;
			
		case 3:
			res.adr = reg[r];
			if (r == 7 || r == 6 || b == 0) {
				res.adr = w_read ((Adress) reg[r]);
				res.val = w_read ((Adress) w_read ((Adress) (reg[r])));
				printf ("@#%o", w_read((Adress) (reg[r])));
				reg[r] += 2;
			}
            else {
				res.adr = w_read ((Adress) reg[r]);
				res.val = b_read ((Adress) w_read ((Adress) (reg[r])));
				reg[r] += 2;       // reg[r] ++;
				printf ("@(R%o)+", r);
			}
            break;
			
		case 4:
			if (r == 7 || r == 6 || b == 0) {
				reg[r] -= 2;
				res.adr = reg[r];
				res.val = w_read (res.adr);
			}
			else {
				reg[r] --;
				res.adr = reg[r];
				res.val = b_read (res.adr);
			}
			
			trace ("-(R%d) ", r);
            
			break;
		
		case 5:
			trace ("@-(R%o)", r);
			reg[r] -= 2;
			res.adr = reg[r];
			res.adr = w_read (res.adr);
			res.val = w_read (res.adr);
			
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
	
	while (1) {
		
		word w = w_read(pc);
		//trace ("%06o %06o : ", pc, w);			//отладочная печать
		trace ("%06o : ", pc);					//обычная печать
		pc += 2;
		struct Operand OP = create(w);
		int i;
		int size = sizeof(commd)/sizeof(struct Command);
		struct Command cmmd;
		
		for (i = 0; i < size; i++) {
			
			cmmd = commd[i];
			
			if ((cmmd.mask & w) == cmmd.opcode) {
				
				trace ("%s    ", cmmd.name);
				
				if (cmmd.param & HAS_SS) {
					ss = get_mode_reg (w >> 6, OP.Byte);
					//trace ("\n ss = %o, %o\n", ss.val, ss.adr);
				}
					

				if (cmmd.param & HAS_DD) {
					dd = get_mode_reg(w, OP.Byte);
					//trace ("\n dd = %o, %o\n", dd.val, dd.adr);
				}
				
				if (cmmd.param & HAS_NN) {
					
					NN = get_NN(w);
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
