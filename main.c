#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"

int main() {

    //Initialization

    //          f7      rs2  rs1 f3  rd  opcode
    //          !-----!!---!!---!!-!!---!!-----!   Hard-coding instruction
    int bin = 0b00000000111100000000000110010011;


    // Initializing the sim
    struct arch *CPUptr, CPU;
    CPUptr = &CPU;

    struct instruction *instPtr, inst;
    instPtr = &inst;

    static int x0 = 0;

    CPU.reg[0] = x0;

    for (int i = 1; i < 32; i++){
        CPU.reg[i] = 0;
    }

    FILE *fptr;

    // Writing binary instruction file
    fptr = fopen("RISCV instruction test.bin", "wb");
    fwrite(&bin, 4, 1, fptr);
    fclose(fptr);

    fptr = fopen("RISCV instruction test.bin", "rb");

    // Running the sim
    int run = 1;

    int buffer;

    while(run == 1) {

        //fseek(fptr, CPU.PC * 4, SEEK_SET);

        fread(&buffer, sizeof(int), 1, fptr);

        if(buffer == 0b0){
            run = 0;
        }

        decode(instPtr, buffer);

        execute(instPtr, CPUptr);

        run = 0;
    }

    //Dumping register contents
    for (int i = 0; i < 32; i++){

        if (i % 4 == 0 && i != 0){printf("\n");}

        printf("%s%d%s%d%s", "x", i, ": ", CPU.reg[i], "           ");
    }
    return 0;
}