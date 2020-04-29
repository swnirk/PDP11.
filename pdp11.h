//#ifdef _PDP11_
//#define _PDP11_

typedef unsigned char byte; 	 // 8 bit
typedef unsigned short int word; // 16 bit
typedef word Adress;			 // 16 bit


#define MEMSIZE (64*1024)
#define pc reg[7]
#define sp reg[6]

#define HASNT_PARAM 0
#define HAS_XX 1
#define HAS_SS (1<<1)
#define HAS_DD (1<<2)
#define HAS_NN (1<<3)
#define HAS_R1 (1<<4)
#define HAS_R2 (1<<5)

#define ostat 0177564			// регистр данных ввода
#define odata 0177566			// регистр данных дисплея



struct SSDD {
	
	word val;		//значение аргумента
	word adr;		//адрес аргумента
	word res;		//адрес
	
};

struct Command {
	
	word mask;
	word opcode;
	char * name;
	void (*do_func)();
	byte param;
	
};


void b_write (Adress adr, byte b);
byte b_read (Adress adr);
void w_write (Adress adr, word w);
word w_read (Adress adr);


void load_file();
void run();
void NZVC(word w);
void trace(const char * fmt, ...);
void print_reg();
void mem_dump(Adress A, word N);



void do_mov();
void do_bmov();
void do_add();
void do_halt();
void do_sob();
void do_clr();
void do_br();
void do_beq();
void do_bpl();
void do_tstb();
void do_tst();
void do_jsr();
void do_rts();


extern int N, Z, C;
extern int bit, n, XX;
extern int r1, r2;
extern word X;
extern byte mem[MEMSIZE];
extern word reg[8];

extern struct Command commd[];
extern struct SSDD ss, dd, NN;

//#endif
