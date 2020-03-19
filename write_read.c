#include <stdio.h>
#include <assert.h>
#include "pdp11.h"
#define MEMSIZE (64*1024)

byte mem [MEMSIZE];

void b_write (Adress adr, byte b);
byte b_read (Adress adr);
void w_write (Adress adr, word w);
word w_read (Adress adr);

void test_mem() {
	
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
int main () {
	
	test_mem();
	return 0;
}

void b_write (Adress adr, byte b) {
	
	mem[adr] = b;
}

byte b_read (Adress adr) {
	return mem[adr];
}

word w_read (Adress a) {
	
	word w = ((word)mem[a+1]) << 8;
	//printf ("w = %x\n", w);
	w = w | mem[a];
	printf ("//%04hx//", w);
	return w;
}

void w_write (Adress adr, word w) {
	
	mem[adr] = (byte)(w);
	mem[adr+1] = (byte)(w >> 8);
}
	 
	



