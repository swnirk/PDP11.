#include <stdio.h>
#include <assert.h>
#include "pdp11.h"
#include <stdarg.h>

#define MEMSIZE (64*1024)


byte mem [MEMSIZE];


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
	trace ("//%04hx//\n", w);
	return w;
}

void w_write (Adress adr, word w) {
	
	mem[adr] = (byte)(w);
	mem[adr+1] = (byte)(w >> 8);
	
}

word bw_read (Adress a, int nb, int r) {
	
	return (((nb)|(r == 6)|(r == 7)) ? w_read(a) : b_read(a));
}

