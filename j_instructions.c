//
//  j_instructions.c
//  
//
//  Created by Samantha Garcia on 12/6/19.
//

#include <stdio.h>
#include "j_instructions.h"
#include "cpu.h"

//6 bits opcode, 26 bits address field

//bits 26 to 31, mask = 0x1C000000
unsigned int getOpcode(unsigned int instruction)
{
    unsigned int opmask = 0x1C000000;
    unsigned int op = instruction & opmask;
    op = op >> 26;
    return op;
}

//bits 0 to 25, mask = 0x03FFFFFF
unsigned int getAddress(unsigned int instruction)
{
    unsigned int amask = 0x03FFFFFF;
    unsigned int address = instruction & amask;
    return address;
}

////////////************MACROS************////////////
#define OP(x) (getOpcode(x))
#define A(x) (getAddress(x))

void j(unsigned int instruction)
{
    unsigned int address = A(instruction);
    unsigned int res = ((pc + 4) & 0xF0000000);
    res = res | (address << 2);
    npc = res;
}

void jal(unsigned int instruction)
{
    unsigned int address = A(instruction);
    registers[31] = (pc + 4);
    unsigned int res = ((pc + 4) & 0xF0000000);
    res = res | (address << 2);
    npc = res;
}

