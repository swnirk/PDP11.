#include <stdio.h>
#include <assert.h>
#include "pdp11.h"
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
	printf ("//%04hx//", w);
	return w;
}

void w_write (Adress adr, word w) {
	
	mem[adr] = (byte)(w);
	mem[adr+1] = (byte)(w >> 8);
}
