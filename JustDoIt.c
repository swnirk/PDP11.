#include <stdio.h>
#include <stdlib.h>
#include "pdp11.h"

#define pc reg[7]

int NN, n, XX;
word reg[8];

void b_write (Adress adr, byte b);
byte b_read (Adress adr);
void w_write (Adress adr, word w);
word w_read (Adress adr);


struct SSDD ss;
struct SSDD dd;

void do_halt() {
	
	printf("THE END!!!\n");
	print_reg();
	exit(0);
}

void do_mov() {
	
	w_write(dd.adr, ss.val);
	
	//NZVC(ss.val);
}

void do_bmov() {
	
	b_write(dd.adr, ss.val);
	
	//NZVC(ss.val);
}
	
void do_add() {
	
	w_write(dd.adr, (ss.val + dd.val)&0xFF);
	
	//NZVC(ss.val + dd.val);
	
}

void do_sob() {
	
	reg[n]--;
	if (reg[n] != 0) 
		pc = pc - 2*NN;
	}
		
	
void do_clr() {
	
	w_write(dd.adr, 0);
	
	//NZVC(0);
}

/*void do_br() {
	
	pc = pc + XX*2;
}

void do_beq() {
	
	if (Z == 1)
		do_br();
}

void do_bpl() {
	
	if (N == 0)
		do_br();
}*/

/*void do_tstb() {
	
	NZVC(dd.val);
	C = 0;
}*/

