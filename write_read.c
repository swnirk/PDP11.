#include <stdio.h>
#include <assert.h>
#include "pdp11.h"
#include <stdarg.h>

#define MEMSIZE (64*1024)


byte mem [MEMSIZE];


void b_write (Adress adr, byte b) {
	
	if (adr < 8)
		reg[adr] = ((b >> 7) ? (b | 0xFF00) : b);
	else
		mem[adr] = b;
}

byte b_read (Adress adr) {
	return mem[adr];
}

word w_read (Adress a) {
	
	word w = ((b_read(a + 1) << 8) | (b_read(a) & 0xFF));
	trace ("//%04hx//\n", w);
	return w;
}

void w_write (Adress adr, word w) {
	
	if (adr < 15)
		reg[adr] = w;
		
	else {
		mem[adr] = (byte)(w & 0xFF);
		mem[adr+1] = (byte)((w >> 8) & 0xFF);
	}
	
}

word bw_read (Adress a, int nb, int r) {
	
	return (((nb)|(r == 6)|(r == 7)) ? w_read(a) : b_read(a));
}

