//#ifdef _PDP11_
//#define _PDP11_

typedef unsigned char byte; 	 // 8 bit
typedef unsigned short int word; // 16 bit
typedef word Adress;			 // 16 bit


#define MEMSIZE (64*1024)
#define pc reg[7]
#define FULL_DEBAG 2
#define HAS_SS (1<<1)
#define HAS_DD (1<<2)
#define HASNT_PARAM 0

struct SSDD {
	
	word val;		//значение аргумента
	word adr;		//адрес аргумента 
	
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
word bw_read (Adress a, int nb, int r);

void trace(const char * fmt, ...);
void load_file();
void run();


void do_mov();
void do_bmov();
void do_add();
void do_halt();
void do_clr();
void do_br();

void NZVC(word w);

extern int b_or_w;
extern int N, Z, C;
extern int NN, n, XX;
extern byte mem[MEMSIZE];
extern word reg[8];

extern struct Command commd[];
extern struct SSDD ss,dd;

//#endif
