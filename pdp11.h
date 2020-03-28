typedef unsigned char byte; 	 // 8 bit
typedef unsigned short int word; // 16 bit
typedef word Adress;			 // 16 bit
#define MEMSIZE (64*1024)

byte mem [MEMSIZE];
word reg[8];
#define pc reg[7]

typedef struct {
	
	word val;		//значение аргумента
	word adr;		//адрес аргумента 
	
}Arg;


void b_write (Adress adr, byte b);
byte b_read (Adress adr);
void w_write (Adress adr, word w);
word w_read (Adress adr);

void do_mov();
void do_bmov();
void do_add();
void do_halt();
void do_clr();

void NZVC(word w);

extern int NN, n;
