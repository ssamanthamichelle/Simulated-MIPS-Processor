//
//  cpu.c
//  
//
//  Created by Samantha Garcia on 12/12/19.
//

#include "cpu.h"
#include "r_instructions.h"
//#include "r_instructions.c"
#include "i_instructions.h"
//#include "i_instructions.c"
#include "j_instructions.h"
//#include "j_instructions.c"
#include <stdio.h>

unsigned int registers[NUM_REGISTERS];
unsigned int pc;
unsigned int npc;
unsigned int HI;
unsigned int LO;
unsigned char memory[MEMORY_SIZE_IN_BYTES];

unsigned int loop = 1;

//opcode = bits 26-31, mask = 0xFC000000
unsigned int getOpcode(unsigned int instruction)
{
    unsigned int opmask = 0xFC000000;
    unsigned int opcode = instruction & opmask;
    opcode = opcode >> 26;
    return opcode;
}

//funct field = bits 0-5, mask = 0x0000003F
unsigned int getFunct(unsigned int instruction)
{
    unsigned int fmask = 0x0000003F;
    unsigned int funct = instruction & fmask;
    return funct;
}

///////*******MACROS*******///////
#define OP(x) (getOpcode(x))
#define FUNCT(x) (getFunct(x))


void cpu_initialize()
{
    pc = CODE_STARTING_ADDRESS;
    npc = CODE_STARTING_ADDRESS;
    
    registers[29] = STACK_STARTING_ADDRESS;
    registers[0] = 0;
}

void handle_r(unsigned int instruction)
{
    unsigned int f = FUNCT(instruction);
    
    switch (f)
    {
            //add
        case 0x20: add(instruction);
            break;

            //addu
        case 0x21: addu(instruction);
            break;

            //sub
        case 0x22: sub(instruction);
            break;

            //subu
        case 0x23: subu(instruction);
            break;
        
            //mult
        case 0x18: mult(instruction);
            break;
        
            //multu
        case 0x19: multu(instruction);
            break;
        
            //div
        case 0x1a: div(instruction);
            break;
        
            //divu
        case 0x1b: divu(instruction);
            break;
        
            //mfhi
        case 0x10: mfhi(instruction);
            break;
     
            //mflo
        case 0x12: mflo(instruction);
            break;

            //and
        case 0x24: and(instruction);
            break;
        
            //or
        case 0x25: or(instruction);
            break;
        
            //xor
        case 0x26: xor(instruction);
            break;
        
            //nor
        case 0x27: nor(instruction);
            break;
        
            //slt
        case 0x2a: slt(instruction);
            break;
        
            //sltu
        case 0x2b: sltu(instruction);
            break;
        
            //sll
        case 0x00: sll(instruction);
            break;
        
            //sslv
        case 0x04: sllv(instruction);
            break;
        
            //srl
        case 0x02: srl(instruction);
            break;
        
            //srlv
        case 0x06: srlv(instruction);
            break;
        
            //sra
        case 0x03: sra(instruction);
            break;

            //srav
        case 0x07: srav(instruction);
            break;

            //jr
        case 0x08: jr(instruction);
            break;
        
            //jalr
        case 0x09: jalr(instruction);
            break;
        
            //syscall
        case 0x0c: syscall(instruction);

        
        
    }

}





void cpu_execute()
{
    
    while (loop)
    {
    
        pc = npc;
        npc += 4;
        
        unsigned int curr_instruction = memory[pc];
    
        unsigned int opcode = OP(curr_instruction);
    
        switch (opcode)
        {
                //r-instruction
            case 0x00: handle_r(curr_instruction);
                break;
                
                //I-instructions
            
                //addi
            case 0x08: addi(curr_instruction);
                break;
            
                //addiu
            case 0x09: addiu(curr_instruction);
                break;
        
                //lw
            case 0x23: lw(curr_instruction);
                break;
        
                //lh
            case 0x21: lh(curr_instruction);
                break;
            
                //lhu
            case 0x25: lhu(curr_instruction);
                break;
            
                //lb
            case 0x20: lb(curr_instruction);
                break;
            
                //lbu
            case 0x24: lbu(curr_instruction);
                break;
            
                //sw
            case 0x2b: sw(curr_instruction);
                break;
        
                //sh
            case 0x29: sh(curr_instruction);
                break;
            
                //sb
            case 0x28: sb(curr_instruction);
                break;
        
                //lui
            case 0x0f: lui(curr_instruction);
                break;
            
                //ori
            case 0x0d: ori(curr_instruction);
                break;
           
                //andi
            case 0x0c: andi(curr_instruction);
                break;
            
                //xori
            case 0x0e: xori(curr_instruction);
                break;
           
                //slti
            case 0x0a: slti(curr_instruction);
                break;
        
                //sltiu
            case 0x0b: sltiu(curr_instruction);
                break;
            
                //beq
            case 0x04: beq(curr_instruction);
                break;
            
                //bne
            case 0x05: bne(curr_instruction);
                break;
            
                //J-instructions
            case 0x02: j(curr_instruction);
                break;
            
                //jal
            case 0x03: jal(curr_instruction);
                break;
            
        }
    }
      
}

void cpu_exit(int errorcode)
{
    loop = 0;
    
    if (errorcode == 0)
    {
        printf("Program terminated normally");
    }
    else
    {
        printf("Program Terminated, Error Code: %d", errorcode);
        //end loop in cpu_execute...
    }
}




