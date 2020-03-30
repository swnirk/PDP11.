#include <stdio.h>
#include <stdlib.h>
#include "pdp11.h"

struct SSDD ss, dd;
struct SSDD get_mode_reg(word w);
byte mem[MEMSIZE];
word reg[8];

void b_write (Adress adr, byte b);
byte b_read (Adress adr);
void w_write (Adress adr, word w);
word w_read (Adress adr);

struct Command commd[] = {
	
	{0170000, 0010000, "mov", do_mov},
	{0170000, 0060000, "add", do_add},
	{0170000, 0000000, "halt", do_halt},
};

struct SSDD get_mode_reg(word w) {
	
	struct SSDD res;
	int r = w & 7;
	int mode = (w >> 3) & 7;
	
	switch(mode) {
		
		case 0:
			res.adr = r;
			res.val = reg[r];
			printf ("R%o ", r);
			break;
		
		case 1:
			res.adr = reg[r];
			res.val = w_read(res.adr);
			printf ("(R%o) ", r);
			break;
		
		case 2:
			res.adr = reg[r];
			res.val = w_read(res.adr);
			reg[r] += 2;
			
			if (r == 7) 
				printf ("#%o ", res.val);
			else 
				printf ("(R%o)+ ", r);
			break;
			
		case 3:
			res.adr = reg[r];
			res.adr = w_read(res.adr);
			reg[r] += 2;
			res.val = w_read(res.adr);
			
			if (r == 7) 
				printf ("#%o ", res.val);
			else 
				printf ("@(R%o)+ ", r);
			break;
			
		case 4:
			reg[r] -= 2;
			res.adr = reg[r];
			res.val = w_read(res.adr);
			printf ("-(R%o) ", r);
			break;
		
		case 5:
			reg[r] -= 2;
			res.adr = reg[r];
			res.adr = w_read(res.adr);
			res.val = w_read(res.adr);
			
			if (r == 7) 
				printf ("#%o ", res.val);
			else 
				printf ("@-(R%o)+ ", r);
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
		printf ("%06o %06o : ", pc, w);
		pc += 2;
		int i = 0;
		struct Command cmmd;
		
		for (i = 0; ;i++) {
			
			cmmd = commd[i];
			
			if ((w & cmmd.mask) == cmmd.opcode) {
				
				printf ("%s ", cmmd.name);
				ss = get_mode_reg(w >> 6);
				dd = get_mode_reg(w);
				cmmd.do_func();
			}
		}
	}
	
	printf ("unknown");
}
	
			
			
			
