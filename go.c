#include <stdio.h>
#include <stdlib.h>
#include "pdp11.h"
#include <stdarg.h>

int b_or_w;
int N, Z, C;

void trace (const char * fmt, ...) {
	
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}

struct SSDD ss, dd;
struct SSDD get_mode_reg(word w, int b);
byte mem[MEMSIZE];
word reg[8];

void b_write (Adress adr, byte b);
byte b_read (Adress adr);
void w_write (Adress adr, word w);
word w_read (Adress adr);

void load_file() {
	
	FILE * test = NULL;
	Adress a;
	unsigned int bt;
	word N;
	test = fopen("/home/ira/информатика/workpdp/01_sum.pdp.o", "r");
	
	if (test == NULL) {
		
		perror("01_sum.pdp.o");
		exit(1);
	}
	
	fscanf(test, "%hx%hx", &a, &N);
	
	
	for(int i = 0; i < N; i++) {
		
		fscanf(test, "%x", &bt);
		b_write(a+i, bt);
	}
		
	
	fclose(test);
}

struct Command commd[] = {
	
	{0170000, 0010000, "mov", do_mov, HAS_DD | HAS_SS},
	{0170000, 0060000, "add", do_add, HAS_DD | HAS_SS},
	{0170000, 0110000, "movb", do_bmov, HAS_DD | HAS_SS},
	{0170000, 0000000, "halt", do_halt, HASNT_PARAM},
};

void NZVC (word w) {
	
	N = (b_or_w ? (w >> 15) : (w >> 7)) & 1;
	Z = (w == 0);
	C = (b_or_w ? (w >> 16) : (w >> 8)) & 1;
	
}

void print_reg()
{
	int i;
	for (i = 0; i < 8; i += 2)
		trace ("R%d : %06o  ", i, reg[i]);
	printf("\n");
	
	for (i = 1; i < 8; i += 2)
		printf("R%d : %06o  ", i, reg[i]);

}


word byte_to_word(byte b) {

    word w;
    if (SIGN(b, 1) == 0) {
        w = 0;
        w |= b;
    }
    else {
        w = ~0xFF;
        w |= b;
    }
    return w;
}

struct Operand {
    int B;        // Byte
    word r1;      // 1 operand
    word r2;      // 2 operand
};

struct Operand create(word w) {
    struct Operand res;

    res.B = (w >> 15);
    res.r1 = (w >> 6) & 7;
    res.r2 = w & 7;
    return res;
}

struct SSDD get_mode_reg(word w, int b) {
	
	struct SSDD res;
	int r = w & 7;					// номер регистра
	int mode = (w >> 3) & 7;		// номер моды
	/*int b1, b2;
	b2 = b_or_w;
	b1 = (((b_or_w)||(r == 6)||(r == 7)) ? 2 : 1);*/
	res.where = OK;
	
	switch(mode) {
		
		case 0:
			res.adr = r;
			res.val = reg[r];
			trace ("R%o ", r);
			res.where = ZERO;
			break;
		
		case 1:
			res.adr = reg[r];
			res.val = b ? b_read(res.adr) : w_read(res.adr); 
			trace ("(R%o) ", r);
			break;
		
		case 2:
			
			if (r == 7) {
				
				res.adr = reg[r];
				res.val = b ? b_read(res.adr) : w_read(res.adr);
				trace ("#%o ", res.val);
			}
			
			else {
				
				res.adr = reg[r];
				res.val = b ? b_read(res.adr) : w_read(res.adr);
				trace ("(R%o)+ ", r);
			}
				
			if (r == 7 || r == 6 || b == 0)
				reg[r] += 2;
			else 
				reg[r]++;
				
			break;
			
		case 3:
			res.adr = reg[r];
			if (r == 7 || r == 6 || b == 0) {
				res.adr = w_read ((Adress) reg[r]);
				res.val = w_read ((Adress) w_read ((Adress) (reg[r])));
				printf ("@#%o", w_read((Adress) (reg[r])));
				reg[r] += 2;
			}
            else {
				res.adr = w_read ((Adress) reg[r]);
				res.val = b_read ((Adress) w_read ((Adress) (reg[r])));
				reg[r] += 2;       // reg[r] ++;
				printf ("@(R%o)+", r);
			}
            break;
			
		case 4:
			if (r == 7 || r == 6 || b == 0) {
				reg[r] -= 2;
				res.adr = reg[r];
				res.val = w_read (res.adr);
			}
			else {
				reg[r] --;
				res.adr = reg[r];
				res.val = b_read (res.adr);
			}
			
			trace ("-(R%d) ", r);
            
			break;
		
		case 5:
			printf ("@-(R%o)", r);
			reg[r] -= 2;
			res.adr = reg[r];
			res.adr = w_read (res.adr);
			res.val = w_read (res.adr);
			
			break;
			
		default:
				fprintf (stderr, "Mode %o NO IMPLEMENTED YET\n", mode);
				exit(1);
		}
		
	return res;
}

void run() {
	
	pc = 01000;
	
	while (1) {
		
		word w = w_read(pc);
		trace ("%06o %06o : ", pc, w);
		pc += 2;
		struct Operand OP = create(w);
		int i;
		int size = sizeof(commd)/sizeof(struct Command);
		struct Command cmmd;
		
		for (i = 0; i < size; i++) {
			
			cmmd = commd[i];
			
			if ((w & cmmd.mask) == cmmd.opcode) {
				
				trace ("%s ", cmmd.name);
				cmmd.do_func();
				if (cmmd.param & HAS_SS) {
					ss = get_mode_reg (w >> 6, OP.B);
					dd = get_mode_reg(w, OP.B);
					trace ("\n ss = %o, %o\n", ss.val, ss.adr);
					trace ("\n dd = %o, %o\n", dd.val, dd.adr);
					print_reg();
				}
					

                if (cmmd.param & HAS_DD) {
					dd = get_mode_reg(w, OP.B);
					trace ("\n dd = %o, %o\n", dd.val, dd.adr);
				}
					
                }
		}
	}
}
