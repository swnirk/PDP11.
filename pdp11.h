//#ifdef _PDP11_
//#define _PDP11_

typedef unsigned char byte; 	 // 8 bit
typedef unsigned short int word; // 16 bit
typedef word Adress;			 // 16 bit


#define MEMSIZE (64*1024)
#define pc reg[7]
#define FULL_DEBAG 2
#define HAS_SS 1
#define HAS_DD 2
#define HAS_NN 4
#define HAS_XX 8
#define HASNT_PARAM 0
#define ostat 0177564			// регистр данных ввода
#define odata 0177566			// регистр данных дисплея
#define SIGN(w, is_byte) (is_byte ? ((w)>>7)&1 : ((w)>>15)&1 )
#define pc reg[7]


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

struct Operand {
    int Byte;        // Byte
    word r1;      	// 1 operand
    word r2;       // 2 operand
};

void b_write (Adress adr, byte b);
byte b_read (Adress adr);
void w_write (Adress adr, word w);
word w_read (Adress adr);

word byte_to_word(byte b);

void trace(const char * fmt, ...);
void load_file();
void run();
void print_reg();
void NZVC(struct Operand psw);

struct SSDD get_NN (word w);


void do_mov();
void do_bmov();
void do_add();
void do_halt();
void do_sob();
void do_clr();
void do_br();
void do_beq();
void do_bpl();
void d0_tstb();



extern int b_or_w;
extern int N, Z, C;
extern int n, XX;
extern byte mem[MEMSIZE];
extern word reg[8];

extern struct Command commd[];
extern struct SSDD ss,dd, NN;

//#endif
