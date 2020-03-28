#include <stdio.h>
#include <stdlib.h>
#include "pdp11.h"

void do_halt() {
	
	printf("THE END!!!\n");
	exit(0);
}

void do_mov() {
	
	w_write(dd.adr, ss.val);
}

void do_bmov() {
	
	b_write(dd.adr, ss.val);
}
	
void do_add() {
	
	w_write(dd.adr, (ss.val + dd.val)&0xFF);
	
}

void do_sob() {
	
	reg[n]--;
	if (reg[n] != 0) 
		pc = pc - 2*NN;
		
	
void do_clr() {
	
	w_write(dd.adr, 0);
}
