#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


bool running = true;

//int ip = 0;
//int sp = -1;
#define sp registers[SP]
#define ip registers[IP]
int stack[256];
int val_popped = 69;
int nigtest = 0;
int memory[256];

typedef enum {
    PSHI,  //(int)for push immedite to stack
    ADD,   //()add 
    SUB,   //subtract
    PRNT,  //($r)
    SET,  //($r, int)
    HLT,
    POP,  //($r) takes top value from stack and puts it into register
    PSH,  //($r)push register to stack
    JEZ,
    JNZ,
    JMP,
    LD,  //($r, $r)set the second register to the value of the first register. 
    MOV,
    CALL, //Jumps to a certain program counter pops current one to the stack
    RET   //Jumps back to the first call by jumping to the top of the stack which should be the position set by call
} InstructionSet;

typedef enum {
    A, B, C, D, E, F, SP, IP,
    NUM_OF_REGISTERS
} Registers;

int registers[NUM_OF_REGISTERS];


const int program[] = {
    SET, C , 20, //change this for more numbers
    CALL, 28,
    PSH, A,
    PSH, B,
    ADD,
    LD, A, B,
    POP, A,
    PRNT, A,
    PSHI, 1,
    PSH, C,
    SUB,
    JEZ, 27,
    POP, C,
    JMP, 4,
    HLT,      //end of main
    SET, A, 0,
    SET, B, 1,
    RET
};

const char* names[] =
{
    "PSHI",
    "ADD",
    "SUB",
    "PRNT",
    "SET",
};

int fetch();
void eval(int instr);
void parse(const char* data, size_t size);


int main()
{
    FILE* srcFile = fopen("fib.txt", "r");
    fseek(srcFile, 0, SEEK_END);  // move to the end of the file (0 off from END)

    size_t srcSize = ftell(srcFile);  // "ftell" gets the current position

    rewind(srcFile);

    size_t dataSize = srcSize * sizeof(char);
    char* srcData = malloc(dataSize);

    memset(srcData, '\0', srcSize);

    size_t i = 0;
    int nNumber = 0;
    while (i < srcSize)
    {
        char character = fgetc(srcFile);
        
        if (character == EOF) { break; }
        //printf("%d", niggerNumber);
        srcData[i] = character;
        i = i + 1;
        nNumber = nNumber + 1;
    }

    fclose(srcFile);
    parse(srcData, dataSize);



    sp = -1;
    ip = 0;

    while (running)
    {
        eval(fetch());
        ip++; // increment the ip every iteration
    }
    
}



void parse(const char* data, size_t size)
{
    size_t one = 1;
    char* comma = ",nig";
    int programSize = 0;
    int realSize = (int)size; 
    int i = 0;
    while (i <= realSize)
    {
        char* compare = &data[i];
        if (strncmp(compare, comma, one) == 0)
        {
            programSize = programSize + 1;
            printf("%d", programSize);
        }
        i = i + 1;
    }

}

int fetch()
{
    return program[ip];
}

void eval(int instr) {
    switch (instr) {

    case HLT:
        running = false;
        break;
    case PSHI:
        sp++;
        stack[sp] = program[++ip];
        break;
    case PSH:
        sp++;
        stack[sp] = registers[program[++ip]];
        break;

    case PRNT:
    {
        ip++;
        int print = registers[program[ip]];
        printf("popped %d ", print);
        break;
    }
    case JMP:
    {
        ip++;
        int val = program[ip];
        ip = val;
        break;
    }
    case POP:
    {
        ip++;
        int popped = stack[sp];
        registers[program[ip]]= popped;
        sp--;
        break;
    }
    case LD:
    {
        ip++;
        int copy = registers[program[ip]];
        ip++;
        registers[program[ip]] = copy;
        break;
    }
    case JEZ:
    {
        if (stack[sp] == 0) {
            ip++;
            int val = program[ip];
            ip = val;
        }
        break;
    }
    case JNZ:
    {
        if (stack[sp] != 0) {
            ip++;
            int val = program[ip];
            ip = val;
        }
        break;
    }
    case MOV:
    {
        ip++;
        int val = registers[program[ip]];
        ip++;
        registers[program[ip]] = val;
        break;
    }
    case SET:
    {
        nigtest++;
        ip++;
        int reg = program[ip];
        ip++;
        registers[reg] = program[ip];
        break;
    }
    case ADD:
    {
        //take the top 2 off the stack
        int a = stack[sp--];
        int b = stack[sp];
        //return it to the stack
        int result = a + b;
        stack[sp] = result;
        break;
    }
    case SUB:
    {
        //take the top 2 off the stack
        int a = stack[sp--];
        int b = stack[sp];

        int result = a - b;
        stack[sp] = result;
        break;
    }
    case CALL:
    {
        ip++;
        int instructionPointer = registers[IP];
        sp++;
        stack[sp] = instructionPointer;
        
        int val = program[ip];
        ip = val;
        break;





    }
    case RET:
    {
        int instructionPointer = stack[sp];
        sp--;
        registers[IP] = instructionPointer;
        break;
    }
    }
}




/**   case SET:
       // Set, name of reg, Value to set reg to
       //get name of reg
       ip++;
       int val = program[ip++];
       ip--;
       registers[program[ip]] = val;
       ip++;
       break; */
