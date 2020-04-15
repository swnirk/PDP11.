#include <stdio.h>
#include <stdlib.h>
#include "pdp11.h"

int n, XX;
word reg[8];

struct Operand psw;


struct SSDD ss;
struct SSDD dd;

void do_halt() {
	
	printf("\n\n-------------THE END!!!-------------\n\n");
	print_reg();
	exit(0);
}

void do_mov() {
	
	dd.res = ss.val;
	w_write(dd.adr, dd.res);
		
	NZVC(psw);
}

void do_bmov() {

	dd.res = ss.val;
	b_write(dd.adr, (byte)dd.res);
	
	if (dd.adr == odata)
		printf("%c", ss.val);
	NZVC(psw);

}

	
void do_add() {
	
	dd.res = dd.val + ss.val;
	w_write(dd.adr, dd.res);
	NZVC(psw);
	
}

void do_sob() {
	
	reg[NN.adr]--;
	
	if (reg[NN.adr] != 0) 
		pc = pc - 2*NN.val;
}
		
void do_clr() {
	
	dd.res = dd.val = 0;
	w_write(dd.adr, dd.res);
	N = 0;
	Z = 1;
	NZVC(psw);

}

void do_br() {
	
	pc = pc + XX*2;
}

void do_beq() {
	
	if (Z == 1)
		do_br();
}

void do_bpl() {
	
	if (N == 0)
		do_br();
}

void do_tstb() {
	
	dd.res = dd.val;
	NZVC(psw);
}

void do_tst() {
	
	dd.res = dd.val;
	NZVC(psw);
}
