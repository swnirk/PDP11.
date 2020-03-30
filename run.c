#include "pdp11.h"
#include <stdio.h>
#include <stdlib.h>

#define pc reg[7]

struct SSDD ss, dd;
struct SSDD get_mode_reg(word w);
int NN, n;
byte mem[MEMSIZE]
word reg[8];

//void NZVC(w) {}
	

struct Command commd[] = {
	
	{0170000, 0010000, "mov", do_mov},
	{0170000, 0060000, "add", do_add},
	{0170000, 0000000, "halt", do_halt},
};

struct SSDD get_mode_reg(word w) {
	
	struct SSDD res;
	int r = w & 7;							// номер регистра
	int mode = (w >> 3) & 7 				// номер моды	
	
	switch(mode) {
		
		case 0: 							// mode 0  ожидаем R3
			res.adr = r;
			res.val = reg[r];
			printf ("R%o ", r);
			break;
			
		case 1: 							// mode 1  ожидаем (R3)
			res.adr = reg[r];
			res.val = w_read(res.adr);		// или b_read 
			printf ("(R%o) ", r);
			break;
			
		case 2: 							// mode 2  ожидаем (R3)+ или #3
			res.adr = reg[r];
			res.val = w_read(res.adr);		// или b_read 
			reg[r] += 2;					// или +1
			
			if (r == 7) 
				printf ("#%o ", res.val);
			else 
				printf ("(R%o)+ ", r);
			break;
			
		case 3:								// mode 3 ожидаем @(R3)+
			res.adr = reg[r];
			res.adr = w_read(res.adr);		// или b_read
			reg[r] += 2;
			res.val = w_read(res.adr);		// или b_read
			
			if (r == 7)
				printf("#%o ", res.val);
			else 
				printf("@(R%o)+ ", r);
			break;
			
		case 4:								// mode 4 ожидаем -(R3) 
			reg[r] -= 2;
			res.adr = reg[r];
			res.val = w_read(res.adr);
			printf("-(R%o) ", r);
			break;
			
		case 5:								//mode 5 ожидаем @-(R3)
			reg[r] -= 2;
			res.adr = reg[r];
			res.adr = w_read(res.adr);		// или b_read
			res.val = w_read(res.adr);		// или b_read
			
			if (r == 7)
				printf("#%o ", res.val);
			else 
				printf("@-(R%o)+ ", r);
			break;
			
			
		default:
			fprintf(stderr, 
			
				"Mode %o NO IMPLEMENTED YET\n", mode);
				
			exit(1);	
		}
	return res;
}
	

void run() {
	
	pc = 01000;
	
	while(1) {
		
		word w = w_read(pc);
		printf("%06o %06o: ", pc, w);
		pc += 2;
		int i = 0;
		struct Command cmmd;
		
		for (i = 0 ; ; i++) {
			
			cmmd = commd[i];
		
			if ((w & cmmd.mask) == cmmd.opcode) {
			
				printf ("%s ", cmmd.name);
				ss = get_mode_reg;
				dd = get_mode_reg(w);
				cmmd.do_func();
			}
		}
	}
		
		printf ("unknown");
}


int main () {
	
	test_mem();
	run();
	return 0;
}
