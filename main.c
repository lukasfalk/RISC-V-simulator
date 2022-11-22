#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"

struct program{
    int prog1, prog2, prog3, prog4, prog5;
};

int main() {

    //Initialization

    struct program prog;
    prog.prog1 = 0x00700093;  // li x1, 5
    prog.prog2 = 0x02b00113;  // li x2, 19
    prog.prog3 = 0x001101b3;  // add x3, x2, x1
    prog.prog4 = 0x40208533;  // sub a0, x1, x2
    prog.prog5 = 0x0020d713;  // srli x14, x1, 2


    // Initializing the sim

    struct arch *CPUptr, CPU;
    struct instruction *instPtr, inst;

    CPUptr = &CPU;
    instPtr = &inst;

    static int x0 = 0; // x0 is always 0
    CPU.reg[0] = x0;

    for (int i = 1; i < 32; i++){
        CPU.reg[i] = 0;
    }

    CPU.PC = 0;


    // Writing binary instruction file

    FILE *fptr;
    fptr = fopen("RISCV test program.bin", "wb");
    fwrite(&prog, sizeof(struct program), 1, fptr);
    fclose(fptr);
    fptr = fopen("RISCV test program.bin", "rb");

            // Read the entire program and store individual instructions in the memory
    int t = 0;
    do{
        fseek(fptr, t * sizeof(int), SEEK_SET);
        fread(&CPU.mem[t], sizeof(int), 1, fptr);
        t++;
    }while(CPU.mem[t - 1] != 0);



    // Running the sim

    int run = 1;

    while(run == 1) {

        decode(instPtr, CPU.mem[CPU.PC]);

        execute(instPtr, CPUptr);

        CPU.PC++;

        (CPU.PC >= 10) ? (run = 0) : (run = 1);

    }

    //Dumping register contents
    for (int i = 0; i < 32; i++){

        if (i % 4 == 0 && i != 0){printf("\n");}

        printf("%s%02d%s%-8d", "x", i, ": ", CPU.reg[i]);
    }

    return 0;
}