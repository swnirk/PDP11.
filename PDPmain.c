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
	
	//пишем слово, читаем слово
	
	Adress adr = 4;
	//byte b1 = 0xcb;
	word wr = 0xcb0a;
	//b_write(a,b0);
	//b_write(a+1, b1);
	w_write(adr, wr);
	word wres = w_read(adr);
	printf ("%04hx = %04hx\n", wres, wr);
	assert(wr == wres);
	
	//пишем слово, читаем 2 байта
	
	Adress a = 4;
	byte b1 = 0xcb;
	byte b2 = 0x0a;
	word w = 0xcb0a;
	word res1 = b_read(a+1);
	word res2 = b_read(a);
	printf("%04hx = %02hhx%02hhx\n", w, res1, res2);
	assert (b1 == res1);
	assert (b2 == res2);
	
	//пишем 2 байта, читаем слово
	
	Adress A = 4;
	byte b3 = 0xfd;
	byte b4 = 0xcd;
	word wor = 0xfdcd;
	b_write(A+1, b3);
	b_write(A, b4);
	word res = w_read(A);
	printf("%02hhx%02hhx = %04hx\n", b3, b4, res);
	assert(res == wor);

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
	
int main (int argc, char * argv[]) {
	
	mem[ostat] = -1;
	
	if (argc == 1) {							// проверка существования argv[1] 
		printf ("USAGE: %s sum\n", argv[0]);
		exit(1);
	}
	
	char str[] = "-t";
	
	for (int i = 0; i < 1; i ++) {
		
		if (strcmp(str, argv[1]) == 0)
			trc = 1;
	}
	
	load_file(argv[2]);
	run();
	
	
	return 0;
}
