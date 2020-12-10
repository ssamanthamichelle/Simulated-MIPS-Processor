//
//  i_instructions.c
//  
//
//  Created by Samantha Garcia on 12/2/19.
//

#include <stdio.h>
#include "i_instructions.h"
#include "cpu.h"

//i instructions: opcode 6 bits, RS 5 bits, RT 5 bits, immediate 16 bits

//bits 26 to 31, mask = 0xFC000000
unsigned int getOpcode(unsigned int instruction)
{
    unsigned int opmask = 0xFC000000;
    unsigned int op = instruction & opmask;
    op = op >> 26;
    return op;
}

//bits 21 to 25, mask = 0x03E00000
unsigned int getRS(unsigned int instruction)
{
    unsigned int rsmask = 0x03E00000;
    unsigned int rs = instruction & rsmask;
    rs = rs >> 21;
    return rs;
}

//bits 16 to 20, mask = 0x001F0000
unsigned int getRT(unsigned int instruction)
{
    unsigned int rtmask = 0x001F0000;
    unsigned int rt = instruction & rtmask;
    rt = rt >> 16;
    return rt;
}

//bits 0 to 15, mask = 0x0000FFFF;
int getIM(unsigned int instruction)
{
    unsigned int immask = 0x0000FFFF;
    unsigned int im = instruction & immask;
    return im;
}

//returns sign extended immediate value
long int sign_extend(int imm)
{
    //mask to get bit 15
    unsigned int signmask = 0x8000;
    
    unsigned int extendmask = 0xFFFF0000;
    
    //bit 15 is 1, extend 1
    if (signmask & imm)
    {
        imm = imm | extendmask;
    }
    return imm;
}

//zero extend immediate
long unsigned zero(int imm)
{
    long unsigned zeromask = 0x00000000;
    imm = imm | zeromask;
    return imm;
}

////////////***********************MACROS*******************////////////////
#define OP(x) (getOp(x))
#define RS(x) (getRS(x))
#define RT(x) (getRT(x))
#define IM(x) (getIM(x))
#define EXT(x) (sign_extend(x))
#define Z(x) (zero(x))


void addi(unsigned int instruction)
{
    unsigned int rt = RT(instruction);
    unsigned int rs = RS(instruction);
    int imm = IM(imm);
    imm = EXT(imm);
    
    registers[rt] = registers[rs] + imm;
}

void addiu(unsigned int instruction)
{
    unsigned int rt = RT(instruction);
    unsigned int rs = RS(instruction);
    unsigned int imm = IM(instruction);
    imm = EXT(imm);
    
    registers[rt] = registers[rs] + imm;
}

void andi(unsigned int instruction)
{
    unsigned int rt = RT(instruction);
    unsigned int rs = RS(instruction);
    unsigned int imm = IM(instruction);
    imm = Z(imm);
    
    registers[rt] = registers[rs] & imm;
    
}

void ori(unsigned int instruction)
{
    unsigned int rt = RT(instruction);
    unsigned int rs = RS(instruction);
    unsigned int imm = IM(instruction);
    imm = Z(imm);
    
    registers[rt] = registers[rs] | imm;
}

void xori(unsigned int instruction)
{
    unsigned int rt = RT(instruction);
    unsigned int rs = RS(instruction);
    unsigned int imm = IM(instruction);
    imm = Z(imm);
    
    registers[rt] = registers[rs] ^ imm;
}

void slti(unsigned int instruction)
{
    unsigned int rt = RT(instruction);
    unsigned int rs = RS(instruction);
    unsigned int imm = IM(instruction);
    imm = EXT(imm);
    
    if (registers[rs] < imm)
    {
        registers[rt] = 1;
    }
    else
    {
        registers[rt] = 0;
    }
}

void sltiu(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int imm = IM(instruction);
    imm = EXT(imm);
    
    if (registers[rs] < (unsigned) imm)
    {
        registers[rt] = 1;
    }
    else
    {
        registers[rt] = 0;
    }
}

void beq(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int imm = IM(instruction);
    imm = (int) EXT(imm);
    
    if (registers[rs] == registers[rt])
    {
        npc = pc + 4 + (imm << 2);
    }
}

void bne(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int imm = IM(instruction);
    imm = (int) EXT(imm);
    
    if (registers[rs] != registers[rt])
    {
        npc = pc + 4 + (imm << 2);
    }
}

void lb(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int imm = IM(instruction);
    imm = (int) EXT(imm);
    
    unsigned int address = registers[rs] + imm;
    long int m = EXT(memory[address]);
    registers[rt] = m;
}

void lbu(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int imm = IM(instruction);
    imm = Z(imm);
    
    unsigned int address = registers[rt] + imm;
    registers[rt] = memory[address];
}

void lh(unsigned int instruction)
{
    unsigned int rt = RT(instruction);
    unsigned int rs = RS(instruction);
    unsigned int imm = IM(instruction);
    
    int addr = registers[rs] + EXT(imm);
    unsigned short *p = memory[addr];
    int res = EXT(*p);
    
    registers[rt] = res;
}

void lhu(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int imm = IM(instruction);
    
    unsigned int addr = registers[rs] + EXT(imm);
    
    unsigned short *p = memory[addr];
    unsigned res = Z(*p);
    
    registers[rt] = res;
}

void lw(unsigned int instruction)
{
    unsigned int rt = RT(instruction);
    unsigned int rs = RS(instruction);
    unsigned int imm = IM(instruction);
    imm = EXT(imm);
    
    int address = (registers[rs] + imm);
    int res = EXT(memory[address]);
    
    registers[rt] = res;
}

void lui(unsigned int instruction)
{
    unsigned int rt = RT(instruction);
    unsigned int imm = IM(instruction);
    
    registers[rt] = (imm << 16);
}

void sb(unsigned int instruction)
{
    unsigned int rt = RT(instruction);
    unsigned int rs = RS(instruction);
    unsigned int imm = IM(imm);
    imm = (int) EXT(imm);
    
    unsigned int address = (registers[rs] + imm);
    memory[address] = registers[rt];
}

//the compiler will automatically truncate the unsigned int in register[rt] when writing
//to a half-word or byte. for the half-word case, you'll need to assign a pointer of type
// (unsigned short *) to point to the place in memory where you want to write the data to
void sh(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int imm = IM(instruction);
    imm = (int) EXT(imm);
    
    unsigned int address = (registers[rs] + imm);
    unsigned short *p = memory[address];
    
    *p = registers[rt];
}

void sw(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int imm = IM(instruction);
    imm = (int) EXT(imm);
    
    unsigned int address = (registers[rs] + imm);
    memory[address] = registers[rt];
}










