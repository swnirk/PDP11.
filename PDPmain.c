#include <stdio.h>
#include <stdlib.h>
#include "pdp11.h"
#include <assert.h>

void test_wr() {
	
	// пишем байт, читаем байт
	byte b0 = 0x0a;
	b_write(2,b0); 
	byte bres = b_read(2);
	printf ("%02hhx = %02hhx\n", b0, bres);
	assert(b0 == bres);
	
	//пишем 2 байта, читаем слово
	
	Adress a = 4;
	//byte b1 = 0xcb;
	word w = 0xcb0a;
	//b_write(a,b0);
	//b_write(a+1, b1);
	w_write(a, w);
	word wres = w_read(a);
	printf ("%04hx = %04hx\n", wres, w);
	assert(w == wres);
}

/*void test_do() {
	
	struct SSDD ss, dd;
	
	ss.adr = 6;
	ss.val = 0x0c;
	dd.adr = 9;
	dd.val = 0x0f;
	do_mov();
	do_add();
}*/
	
int main () {
	
	test_wr();
	//test_do();
	return 0;
}

