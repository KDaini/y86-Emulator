#ifndef emulator_h
#define emulator_h
void printHelp();
void get(char *lineIn);
void getBin(int num, char *str);
void decode(char * text);
char *enter(char *line1);


#define EAX         0
#define ECX         1
#define EDX         2
#define EBX         3
#define ESP         4
#define EBP         5
#define ESI         6
#define EDI         7

#define overflow    "\nOF\n"
#define zero        "\nZF\n"
#define negative    "\nSF\n"

#define OKAY         "Everything is ok.\n"
#define HALT         "Halt instruction encountered, stopped executing\n"
#define BADR         "Bad Address.\n"
#define BINS         "Bad Instruction.\n"

typedef struct Node{ // 64 bit register
    struct Node **ptr;
    char data[1024];
    int location;
}Node;

typedef struct regs {
    Node regList[8]; //register codes in order. each points to a reg struct
}regs;

#endif /* emulator_h */
