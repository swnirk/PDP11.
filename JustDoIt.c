#include <stdio.h>
#include <stdlib.h>
#include "pdp11.h"

int n, XX;
word reg[8], w;
int r1, r2;

struct Operand oper;


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
		
	NZVC(dd.res);
}

void do_bmov() {

	dd.res = ss.val;
	b_write(dd.adr, (byte)dd.res);


	fprintf(stderr, "Wanna print to dd.adr=%o?\n", dd.adr);
	if (dd.adr == odata)
		fprintf(stderr, "%c %d\n", ss.val, ss.val);
	NZVC(dd.res);
}

	
void do_add() {
	
	dd.res = dd.val + ss.val;
	w_write(dd.adr, dd.res);
	NZVC(dd.res);
	
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
	NZVC(dd.res);

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
	NZVC(dd.res);
}

void do_tst() {
	
	dd.res = dd.val;
	NZVC(dd.res);
}

void do_jsr() {
	
	sp -= 2;
	w_write(sp, reg[r1]);
	//trace ("r1 = %o\n", r1);
	reg[r1] = pc;
	pc = dd.adr;
	trace ("R%o \n", r1);
	//trace ("R0 = %o", reg[0]);
}

void do_rts() {
	
	pc = reg[r2];
	//trace ("r2 = %o\n", r2);
	reg[r2] = w_read(sp);
	sp += 2;
	trace ("R%o \n", r1);
	//trace ("R0 = %o", reg[0]);
}
	

