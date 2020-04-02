#include <stdio.h>
#include <stdlib.h>
#include "pdp11.h"
#include <stdarg.h>

int b_or_w;
int N, Z, C;

void trace (const char * fmt, ...) {
	
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}

struct SSDD ss, dd;
struct SSDD get_mode_reg(word w);
byte mem[MEMSIZE];
word reg[8];

void b_write (Adress adr, byte b);
byte b_read (Adress adr);
void w_write (Adress adr, word w);
word w_read (Adress adr);

void load_file() {
	
	FILE * test = NULL;
	Adress a;
	unsigned int b;
	word N;
	test = fopen("/home/ira/информатика/PDP11/01_sum.pdp.o", "r");
	
	if (test == NULL) {
		
		perror("01_sum.pdp.o");
		exit(1);
	}
	
	fscanf(test, "%hx%hx", &a, &N);
	
	
	for(int i = 0; i < N; i++) {
		
		fscanf(test, "%x", &b);
		b_write(a+i, b);
	}
		
	
	fclose(test);
}

struct Command commd[] = {
	
	{0170000, 0010000, "mov", do_mov, HAS_DD | HAS_SS},
	{0170000, 0060000, "add", do_add, HAS_DD | HAS_SS},
	{0170000, 0110000, "movb", do_bmov, HAS_DD | HAS_SS},
	{0170000, 0000000, "halt", do_halt, HASNT_PARAM},
};

//void NZVC

struct SSDD get_mode_reg(word w) {
	
	struct SSDD res;
	int r = w & 7;
	int mode = (w >> 3) & 7;
	int b1, b2;
	b2 = b_or_w;
	b1 = (((b_or_w)||(r==6)||(r==7)) ? 2 : 1);
	
	switch(mode) {
		
		case 0:
			res.adr = r;
			res.val = reg[r];
			trace ("R%o ", r);
			break;
		
		case 1:
			res.adr = reg[r];
			res.val = bw_read(res.adr, b2, r);
			trace ("(R%o) ", r);
			break;
		
		case 2:
			res.adr = reg[r];
			res.val = bw_read(res.adr, b2, r);
			reg[r] += b1;
			
			if (r == 7) 
				trace ("#%o ", res.val);
			else 
				trace ("(R%o)+ ", r);
			break;
			
		case 3:
			res.adr = reg[r];
			res.adr = bw_read(res.adr, b2, r);
			reg[r] += b1;
			res.val = bw_read(res.adr, b2, r);
			
			if (r == 7) 
				trace ("#%o ", res.val);
			else 
				trace ("@(R%o)+ ", r);
			break;
			
		case 4:
			reg[r] -= b1;
			res.adr = reg[r];
			res.val = bw_read(res.adr, b2, r);
			trace("-(R%o) ", r);
			break;
		
		case 5:
			reg[r] -= b1;
			res.adr = reg[r];
			res.adr = bw_read(res.adr, b2, r);
			res.val = bw_read(res.adr, b2, r);
			
			if (r == 7) 
				trace ("#%o ", res.val);
			else 
				trace ("@-(R%o)+ ", r);
			break;
			
		default:
				fprintf (stderr, "Mode %o NO IMPLEMENTED YET\n", mode);
				exit(1);
		}
		
	return res;
}

void run() {
	
	pc = 01000;
	
	while (1) {
		
		word w = w_read(pc);
		trace ("%06o %06o : ", pc, w);
		pc += 2;
		int i = 0;
		struct Command cmmd;
		
		for (i = 0; ;i++) {
			
			cmmd = commd[i];
			
			if ((w & cmmd.mask) == cmmd.opcode) {
				
				trace ("%s ", cmmd.name);
				cmmd.do_func();
				if (cmmd.param & HAS_SS)
					ss = get_mode_reg (w>>6);

                if (cmmd.param & HAS_DD)
					dd = get_mode_reg(w);
					
                }
		}
	}
}
