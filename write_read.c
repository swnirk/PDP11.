#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include "pdp11.h"

byte mem [MEMSIZE];


void b_write (Adress adr, byte b) {
	
	if (adr == odata)
		fprintf(stderr, "%c ", b);
	
	if (adr < 8)
		reg[adr] = ((b >> 7) ? (b | 0xFF00) : b);
		
	else
		mem[adr] = b;

}

byte b_read (Adress adr) {
	
	return mem[adr];
}

word w_read (Adress adr) {
	
	word w = ((b_read(adr + 1) << 8) | (b_read(adr) & 0xFF));
	return w;
}

void w_write (Adress adr, word w) {
	
	if (adr == odata) {
		
		fprintf(stderr, "%c ", w);
		return;
	}
	
	else if (adr < 15)
		reg[adr] = w;
		
	else {
		mem[adr] = (byte)(w & 0xFF);
		mem[adr+1] = (byte)((w >> 8) & 0xFF);
	}
	
}
