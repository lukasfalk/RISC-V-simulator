#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

struct arch{

    int PC;

    int reg[32];

    int mem[512];

};

struct instruction{

    int funct7, rs2, rs1, funct3, rd, opcode;

};

void decode(struct instruction *instPtr, int a);

void execute(struct instruction *instPtr, struct arch *CPUptr);