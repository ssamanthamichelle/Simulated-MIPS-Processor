//
//  r_instructions.c
//  
//
//  Created by Samantha Garcia on 11/26/19.

#include <stdio.h>
#include "r_instructions.h"
#include "cpu.h"

//r-instructions
//opcode 6 bits, RS fields 5 bits, RT field 5 bits, RD field 5 bits, shamt field 5 bits, funct field 6 bits.

//bits 25 to 21, mask = 0x 03E00000
unsigned int getRS(unsigned int instruction)
{
    unsigned int maskrs = 0x03E00000;
    unsigned int rs = instruction & maskrs;
    rs = rs >> 21;
    return rs;
}

//bits 16 to 20, mask = 0x 001F0000
unsigned int getRT(unsigned int instruction)
{
    unsigned int maskrt = 0x001F0000;
    unsigned int rt = instruction & maskrt;
    rt = rt >> 16;
    return rt;
}

//bits 11 to 15, mask = 0x 0000F800
unsigned int getRD(unsigned int instruction)
{
    unsigned int maskrd = 0x0000F800;
    unsigned int rd = instruction & maskrd;
    rd = rd >> 11;
    return rd;
}

//bits 6 to 10, mask = 0x 000007C0
unsigned int getSHAMT(unsigned int instruction)
{
    unsigned int maskshamt = 0x000007C0;
    unsigned int shamt = instruction & maskshamt;
    shamt = shamt >> 6;
    return shamt;
}

/*
//bits 0 to 5, mask = 0x0000003F
unsigned int getFunction(unsigned int instruction)
{
    unsigned int maskf = 0x0000003F;
    unsigned int f = instruction & maskf;
    //don't need to shift
    return f;
    
} */ //not necessary0

////////////////////////////////*******MACROS*******///////////////////////////////////
#define RS(x) (getRS(x))
#define RT(x) (getRT(x))
#define RD(x) (getRD(x))
#define SHAMT(x) (getSHAMT(x))

void add(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int rd = RD(instruction);
    
    //cast numbers in registers to signed int
    int intrs = (int) registers[rs];
    int intrt = (int) registers[rt];
    
    int sum = intrs + intrt;
    
    unsigned int usum = (unsigned int) sum;
    registers[rd] = usum;
}

void addu(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int rd = RD(instruction);
    
    int intrs = (int) registers[rs];
    int intrt = (int) registers[rt];
    
    int sum = intrs + intrt;
    
    unsigned int usum = (unsigned int) sum;
    registers[rd] = usum;
}

void sub(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int rd = RD(instruction);
    
    int intrs = (int) registers[rs];
    int intrt = (int) registers[rt];

    int res = intrs - intrt;
    registers[rd] = res;
    
}

void subu(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int rd = RD(instruction);
    
    int intrs = (int) registers[rs];
    int intrt = (int) registers[rt];

    int sub = intrs - intrt;
    
    unsigned int usub = (unsigned int) sub;
    registers[rd] = usub;
    
}

void mult(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int rd = RD(instruction);
    
    long res = (long) rs * (long) rt;
    
    HI = res >> 32;
    LO = (res & 0x00000000FFFFFFFF);
}

void multu(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int rd = RD(instruction);
    
    unsigned long res = (unsigned long) rs * (unsigned long) rt;
    
    HI = res >> 32;
    LO = (res & 0x00000000FFFFFFFF);
}

void div(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int rd = RD(instruction);
    
    long lo_res = (long) registers[rs] / (long) registers[rt];
    LO = lo_res;
    
    long hi_res = (long) registers[rs] % (long) registers[rt];
    HI = hi_res;
}

void divu(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int rd = RD(instruction);
    
    unsigned long lo_res = (unsigned long) registers[rs] / registers[rt];
    LO = lo_res;
    
    unsigned long hi_res = (unsigned long) registers[rs] % (unsigned long) registers[rt];
    HI = hi_res;
}

void and(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int rd = RD(instruction);
    
    unsigned int res = registers[rs] & registers[rt];
    
    registers[rd] = res;
}

void nor(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int rd = RD(instruction);
    
    unsigned int res = ~(registers[rs] | registers[rt]);
    
    registers[rd] = res;
}

void or(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int rd = RD(instruction);
    
    unsigned int res = registers[rs] | registers[rt];
    
    registers[rd] = res;
}

void xor(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    unsigned int rd = RD(instruction);
    
    unsigned int res = registers[rs] ^ registers[rd];
    
    registers[rd] = res;
}

void sll(unsigned int instruction)
{
    unsigned int rd = RD(instruction);
    unsigned int rt = RT(instruction);
    unsigned int shamt = SHAMT(instruction);
    
    unsigned int res = registers[rt] << shamt;
    registers[rd] = res;
}

void sllv(unsigned int instruction)
{
    unsigned int rd = RD(instruction);
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    
    unsigned int res = registers[rs] << registers[rt];
    
    registers[rd] = res;
}

void srl(unsigned int instruction)
{
    unsigned int rd = RD(instruction);
    unsigned int rt = RT(instruction);
    unsigned int shamt = SHAMT(instruction);
    
    unsigned int res = registers[rt] >> shamt;
    registers[rd] = res;
    
}

void sra(unsigned int instruction)
{
    unsigned int rd = RD(instruction);
    unsigned int rt = RT(instruction);
    unsigned int shamt = SHAMT(instruction);
    
    unsigned int left = 0x80000000;
    
    unsigned int r_rt = registers[rt];
    unsigned int ones = ((1 << shamt) - 1);
    ones = ones << (32 - shamt);
    
    if (r_rt & left)
    {
        r_rt = r_rt >> shamt;
        r_rt = r_rt & ones;
    }
    
    registers[rd] = r_rt;
}

void srlv(unsigned int instruction)
{
    unsigned int rd = RD(instruction);
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    
    unsigned int res = registers[rs] >> registers[rt];
    registers[rd] = res;
}

void srav(unsigned int instruction)
{
    unsigned int rd = RD(instruction);
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    
    unsigned int left = 0x80000000;
       
    unsigned int r_rs = registers[rs];
    unsigned int r_rt = registers[rt];
    unsigned int ones = ((1 << r_rt) - 1);
    ones = ones << (32 - r_rt);
       
    if (r_rs & left)
    {
        r_rs = r_rs >> r_rt;
        r_rs = r_rs & ones;
        
    }
    
    registers[rd] = r_rs;
}

void slt(unsigned int instruction)
{
    unsigned int rd = RD(instruction);
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    
    int intrs = (int) registers[rs];
    int intrt = (int) registers[rt];
    
    if (intrs < intrt)
    {
        registers[rd] = 1;
    }
    else
    {
        registers[rd] = 0;
    }
}

void sltu(unsigned int instruction)
{
    unsigned int rd = RD(instruction);
    unsigned int rs = RS(instruction);
    unsigned int rt = RT(instruction);
    
    if (registers[rs] < registers[rt])
    {
        registers[rd] = 1;
    }
    else
    {
        registers[rd] = 0;
    }
}

void jr(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    npc = registers[rs];
}

void jalr(unsigned int instruction)
{
    unsigned int rs = RS(instruction);
    
    registers[31] = pc + 4;
    npc = registers[rs];
}

void mfhi(unsigned int instruction)
{
    unsigned int rd = RD(instruction);
    registers[rd] = HI;
}

void mflo(unsigned int instruction)
{
    unsigned int rd = RD(instruction);
    registers[rd] = LO;
}



