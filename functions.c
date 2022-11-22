#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"



void decode(struct instruction *instPtr, int a){

    instPtr->funct7 =   (a & 0b11111110000000000000000000000000) >> 25;

    instPtr->rs2 =      (a & 0b00000001111100000000000000000000) >> 20;

    instPtr->rs1 =      (a & 0b00000000000011111000000000000000) >> 15;

    instPtr->funct3 =   (a & 0b00000000000000000111000000000000) >> 12;

    instPtr->rd =       (a & 0b00000000000000000000111110000000) >> 7;

    instPtr->opcode =   (a & 0b00000000000000000000000001111111);

}

void execute(struct instruction *instPtr, struct arch *CPUptr){

    //Remember to load every instruction opcode into CPU memory



    switch(instPtr->opcode) { // General instruction

        // "Load Upper Imm" and "Add Upper Imm to PC" :

        case 0b0110111:{

        }

        case 0b0010111:{

        }
        
        // I-type instructions / Immediate value instructions:
        case 0b0010011:{

            int imm = (instPtr->funct7 << 5) | instPtr->rs2;

            switch(instPtr->funct3){

                // ADDI
                case 0b000:{
                    CPUptr->reg[instPtr->rd] = CPUptr->reg[instPtr->rs1] + imm;
                    break;
                }

                //SLTI
                case 0b010:{
                    ( CPUptr->reg[instPtr->rs1] < imm ) ? (CPUptr->reg[instPtr->rd] = 1) : (CPUptr->reg[instPtr->rd] = 0);
                    break;
                }

                //SLTUI
                case 0b011:{
                    ( (unsigned)CPUptr->reg[instPtr->rs1] < (unsigned)imm ) ? (CPUptr->reg[instPtr->rd] = 1) : (CPUptr->reg[instPtr->rd] = 0);
                    break;
                }

                //XORI
                case 0b100:{
                    CPUptr->reg[instPtr->rd] = CPUptr->reg[instPtr->rs1] ^ imm;
                    break;
                }

                //ORI
                case 0b110:{
                    CPUptr->reg[instPtr->rd] = CPUptr->reg[instPtr->rs1] | imm;
                    break;
                }

                //ANDI
                case 0b111:{
                    CPUptr->reg[instPtr->rd] = CPUptr->reg[instPtr->rs1] & imm;
                    break;
                }

                //SLLI
                case 0b001:{
                    CPUptr->reg[instPtr->rd] = CPUptr->reg[instPtr->rs1] << instPtr->rs2;
                    break; // Five last bits of imm as rs2. There a better way?
                }

                //SRLI
                case 0b101:{
                    CPUptr->reg[instPtr->rd] = CPUptr->reg[instPtr->rs1] >> instPtr->rs2;
                    break; // Five last bits of imm as rs2. There a better way?
                }

                //SRAI ?????????
            }
            break;
        }


        // R-type instructions
        case 0b0110011:{

            int dec = (instPtr->funct7 << 3) | instPtr->funct3;

            switch (dec) { // Funct7 + funct3 / More specific instruction

                //ADD
                case 0b0000000000:{
                    CPUptr->reg[instPtr->rd] = CPUptr->reg[instPtr->rs1] + CPUptr->reg[instPtr->rs2];
                    break;
                }

                //SUB
                case 0b0100000000:{
                    CPUptr->reg[instPtr->rd] = CPUptr->reg[instPtr->rs1] - CPUptr->reg[instPtr->rs2];
                    break;
                }

                //SLL
                case 0b0000000001:{
                    CPUptr->reg[instPtr->rd] = CPUptr->reg[instPtr->rs1] << CPUptr->reg[instPtr->rs2];
                    break;
                }

                //SLT
                case 0b0000000010:{
                    (CPUptr->reg[instPtr->rs1] < CPUptr->reg[instPtr->rs2]) ? (CPUptr->reg[instPtr->rd] = 1) : (CPUptr->reg[instPtr->rd] = 0);
                    break;
                }

                //SLTU
                case 0b0000000011:{
                    ((unsigned)CPUptr->reg[instPtr->rs1] < (unsigned)CPUptr->reg[instPtr->rs2]) ? (CPUptr->reg[instPtr->rd] = 1) : (CPUptr->reg[instPtr->rd] = 0);
                    break;
                }

                //XOR
                case 0b0000000100:{
                    CPUptr->reg[instPtr->rd] = CPUptr->reg[instPtr->rs1] ^ CPUptr->reg[instPtr->rs2];
                    break;
                }

                //SRL
                case 0b0000000101:{
                    CPUptr->reg[instPtr->rd] = CPUptr->reg[instPtr->rs1] >> CPUptr->reg[instPtr->rs2];
                    break;
                }

                //SRA


                //OR
                case 0b0000000110:{
                    CPUptr->reg[instPtr->rd] = CPUptr->reg[instPtr->rs1] | CPUptr->reg[instPtr->rs2];
                    break;
                }

                //AND
                case 0b0000000111:{
                    CPUptr->reg[instPtr->rd] = CPUptr->reg[instPtr->rs1] & CPUptr->reg[instPtr->rs2];
                    break;
                }

            }
            break;
        }

       //Loading instructions
        case 0b0000011:{

            int offset = (instPtr->funct7 << 5) | instPtr->rs2;
            int byteIndex = offset & 0b11;
            int halfWordIndex = offset & 0b1;

            switch(instPtr->funct3){

                //LB
                case 0b000:{
                    instPtr->rd = CPUptr->mem[offset + instPtr->rs1] & (0b11111111 << ( 8 * byteIndex));
                    instPtr->rd >>= (8 * byteIndex);
                    break;
                }

                //LH
                case 0b001:{
                    instPtr->rd = CPUptr->mem[offset + instPtr->rs1] & (0b1111111111111111 << (16 * halfWordIndex));
                    instPtr->rd >>= ( 16 * halfWordIndex);
                    break;
                }

                //LW
                case 0b010:{
                    instPtr->rd = CPUptr->mem[offset + instPtr->rs1];
                    break;
                }

                //LBU
                case 0b100:{
                    instPtr->rd = (unsigned) CPUptr->mem[offset + instPtr->rs1] & (0b11111111 << (8 * byteIndex));
                    instPtr->rd >>= ( 8 * byteIndex);
                    break;
                }

                //LHU
                case 0b101:{
                    instPtr->rd = (unsigned) CPUptr->mem[offset + instPtr->rs1] & (0b1111111111111111 << ( 16 * halfWordIndex));
                    instPtr->rd >>= (16 * halfWordIndex);
                    break;
                }
            }
            break;
        }

        //Storing instructions
        case 0b0100011:{

            int offset = (instPtr->funct7 << 5) | instPtr->rd;
            int byteIndex = offset & 0b11;
            int halfWordIndex = offset & 0b1;

            switch(instPtr->funct3){

                //SB
                case 0b000:{
                    CPUptr->mem[offset + instPtr->rs1] = instPtr->rs2 & (0b11111111 << ( 8 * byteIndex));break;
                }

                //SH
                case 0b001:{
                    CPUptr->mem[offset + instPtr->rs1] = instPtr->rs2 & (0b11111111 << ( 16 * byteIndex));break;
                }

                //SW
                case 0b010:{
                    CPUptr->mem[offset + instPtr->rs1] = instPtr->rs2;break;
                }
            }
            break;
        }
    }
}
