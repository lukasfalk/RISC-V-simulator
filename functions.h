#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "stdint.h"

#define RAM 2^20

struct arch{

    uint32_t PC = 0;

    uint32_t reg[30];

    int32_t ra = 0;

    int32_t sp = 0;

    int32_t stackr[64] = { 0 };

    uint32_t instrMem[256];

    uint8_t mem[RAM];

};

struct instruction{

    uint32_t funct7, rs2, rs1, funct3, rd, opcode;

};

void decode(struct instruction *instPtr, int a);

void execute(struct instruction *instPtr, struct arch *CPUptr);