#include <stdio.h>
#include <stdlib.h>
#include "pdp11.h"

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
	
	dd.res = dd.val;
	
	if (dd.where == ZERO) 
		reg[dd.adr] = dd.res;
	else
		w_write(dd.adr, dd.res);
		
	//NZVC(ss.val);
}

void do_bmov() {

	dd.res = ss.val;
	if (dd.where == ZERO)
		reg[dd.adr] = byte_to_word(dd.res);
	else
		b_write(dd.adr, (byte)dd.res);

}

	
void do_add() {
	
	dd.res = dd.val + ss.val;
	
	if (dd.where == ZERO)
		reg[dd.adr] = dd.res;
	else
		w_write(dd.adr, dd.res);
	
}

void do_sob() {
	
	reg[n]--;
	
	if (reg[n] != 0) 
		pc = pc - 2*NN;
}
		
void do_clr() {
	
	dd.res = dd.val = 0;
	
	if(dd.where == ZERO)
		reg[dd.adr] = dd.res;
	else
		w_write(dd.adr, dd.res);

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

