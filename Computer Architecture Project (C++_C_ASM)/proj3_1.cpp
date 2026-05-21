// UMBC Fall 2025 - Project 3(Part 1)
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// simulated computer memory 200 (32 bit) words
int mem[201] = { 0 };

int  reg_file[32] = { 0 };

int PCR = 0; // program counter register

// register-integer class
const int ADD = 0x0;
const int SUB = 0x1;
const int AND = 0x2;
const int OR = 0x3;
const int XOR = 0x4;
const int SR = 0x5;
const int SL = 0x6;
const int ASR = 0x7;
const int RR = 0x8;
const int RL = 0x9;
const int NOT = 0xA;

const int BEQ = 0x10;
const int BNE = 0x11;
const int BGT = 0x12;
const int BGE = 0x13;
const int BLT = 0x14;
const int BLE = 0x15;

const int EXT = 0x0F;  // exit simulated code

int load_mem()
{
	int i = 100;
	for (i = 100; i < 200; i++)
		mem[i] = i;
	return 0;
}

int print_mem()
{
	int i = 0;
	for (i = 0; i < 200; i++)
		printf(" [Address = %i] = %i \n", i, mem[i]);
	return 0;
}

int op_decode(int word, int& op, int& mode)
{
	mode = word & 0x80000000;
	if (mode)
		mode = 1;  // integer instruction
	else
		mode = 0;  // register-to-register		

	op = word & 0x7C000000;
	op = (op >> 26); // shift operation code to lower 5 bits.

	return 0;
}

int reg_decode(int word, int& rd, int& rs, int& rt)
{
	rd = (word & 0x03E00000) >> 21;
	rs = (word & 0x001F0000) >> 16;
	rt = (word & 0x0000F800) >> 11;
	printf(" ... reg_decode() -> rd = %i \t  rs = %i \t  rt = %i \n", rd, rs, rt);
	return 0;
}

int int_decode(int word, int& rd, int& rs, int& imm16)
{
	rd = (word & 0x03E00000) >> 21;
	rs = (word & 0x001F0000) >> 16;
	imm16 = word & 0xFFFF; // assume positive two's complement number
	if (imm16 & 0x8000)    // if negative number, sign extend to 32 bits
		imm16 = imm16 | 0xFFFF0000;
	printf(" ... int_decode() -> rd = %i \t  rs = %i \t  imm16 = %i \n", rd, rs, imm16);
	return 0;
}

int ri_decode(int word, int& rd, int& rs, int& rt, int& imm16)
{
	rd = (word & 0x03E00000) >> 21;
	rs = (word & 0x001F0000) >> 16;
	rt = (word & 0x0000F800) >> 11;

	imm16 = word & 0xFFFF; // assume positive two's complement number
	if (imm16 & 0x8000)    // if negative number, sign extend to 32 bits
		imm16 = imm16 | 0xFFFF0000;

	return 0;
}

int exe_code(int& PCR)
{
	int op = 0;
	int mode = 0;
	int rd = 0, rs = 0, rt = 0;
	int imm16 = 0;
	int sign_bit = 0;
	int bit_mask = 0;
	int temp = 0;
	int word = 0;

	int x = 0;
	int r = 0;
	int rotate_right = 0;

	word = mem[PCR];

	op_decode(word, op, mode);

	ri_decode(word, rd, rs, rt, imm16);
	if (mode)
		temp = imm16;
	else
		temp = reg_file[rt];

	switch (op)
	{
	case ADD:

		// register-integer class
		// reg_file[rd] = reg_file[rs] + [reg_file[rt] | immediate_16_bit_constant ]
		reg_file[rd] = reg_file[rs] + temp;

		if (!mode) printf(" [Mode = %i | OPCode = %i] = [ADD-R]  => R%i{%i} = R%i{%i} + R%i{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		else       printf(" [Mode = %i | OPCode = %i] = [ADD-I]  => R%i{%i} = R%i{%i} + imm{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], imm16);

		break;

	case SUB:

		reg_file[rd] = reg_file[rs] - temp;

		if (!mode) printf(" [Mode = %i | OPCode = %i] = [SUB-R]  => R%i{%i} = R%i{%i} - R%i{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		else       printf(" [Mode = %i | OPCode = %i] = [SUB-I]  => R%i{%i} = R%i{%i} - imm{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], imm16);

		break;

	case AND:

		reg_file[rd] = reg_file[rs] & temp;

		if (!mode) printf(" [Mode = %i | OPCode = %i] = [AND-R]  => R%i{%i} = R%i{%i} & R%i{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		else       printf(" [Mode = %i | OPCode = %i] = [AND-I]  => R%i{%i} = R%i{%i} & imm{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], imm16);

		break;

	case OR:

		reg_file[rd] = reg_file[rs] | temp;

		if (!mode) printf(" [Mode = %i | OPCode = %i] = [OR-R ]  => R%i{%i} = R%i{%i} | R%i{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		else       printf(" [Mode = %i | OPCode = %i] = [OR-I ]  => R%i{%i} = R%i{%i} | imm{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], imm16);

		break;

	case XOR:

		reg_file[rd] = reg_file[rs] ^ temp;

		if (!mode) printf(" [Mode = %i | OPCode = %i] = [XOR-R]  => R%i{%i} = R%i{%i} ^ R%i{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		else       printf(" [Mode = %i | OPCode = %i] = [XOR-I]  => R%i{%i} = R%i{%i} ^ imm{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], imm16);

		break;

	case SR:

		temp = temp & 31; // restrict shift range to 0..31
		reg_file[rd] = (int)((uint32_t)reg_file[rs] >> temp);

		if (!mode) printf(" [Mode = %i | OPCode = %i] = [SR-R ]  => R%i{%i} = R%i{%i} >> R%i{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		else       printf(" [Mode = %i | OPCode = %i] = [SR-I ]  => R%i{%i} = R%i{%i} >> imm{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], imm16);

		break;

	case SL:

		temp = temp & 31; // restrict shift range to 0..31
		reg_file[rd] = reg_file[rs] << temp;

		if (!mode) printf(" [Mode = %i | OPCode = %i] = [SL-R ]  => R%i{%i} = R%i{%i} << R%i{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		else       printf(" [Mode = %i | OPCode = %i] = [SL-I ]  => R%i{%i} = R%i{%i} << imm{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], imm16);

		break;

	case ASR:  // code for ASR is provided

		temp = temp & 31; // restrict bit shift range to 0..31

		sign_bit = (reg_file[rs] & 0x80000000);

		reg_file[rd] = reg_file[rs] >> temp;

		if (sign_bit && temp != 0) // number is negative
		{
			bit_mask = 0xFFFFFFFF << (32 - temp);
			reg_file[rd] = reg_file[rd] | bit_mask;
		}

		if (!mode) printf(" [Mode = %i | OPCode = %i] = [ASR-R]  => R%i{%i} = R%i{%i} ASR R%i{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		else       printf(" [Mode = %i | OPCode = %i] = [ASR-I]  => R%i{%i} = R%i{%i} ASR imm{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], imm16);

		break;

	case RR:

		x = reg_file[rs];
		r = temp & 31;

		if (r == 0)
			rotate_right = x;
		else
			rotate_right = (int)(((uint32_t)x >> r) | ((uint32_t)x << (32 - r)));

		reg_file[rd] = rotate_right;

		if (!mode) printf(" [Mode = %i | OPCode = %i] = [RR-R ]  => R%i{%i} = R%i{%i} ROR R%i{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		else       printf(" [Mode = %i | OPCode = %i] = [RR-I ]  => R%i{%i} = R%i{%i} ROR imm{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], imm16);

		break;

	case RL:

		x = reg_file[rs];
		r = temp & 31;

		if (r == 0)
			reg_file[rd] = x;
		else
			reg_file[rd] = (int)(((uint32_t)x << r) | ((uint32_t)x >> (32 - r)));

		if (!mode) printf(" [Mode = %i | OPCode = %i] = [RL-R ]  => R%i{%i} = R%i{%i} ROL R%i{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		else       printf(" [Mode = %i | OPCode = %i] = [RL-I ]  => R%i{%i} = R%i{%i} ROL imm{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs], imm16);

		break;

	case NOT:

		reg_file[rd] = ~reg_file[rs];

		if (!mode) printf(" [Mode = %i | OPCode = %i] = [NOT-R]  => R%i{%i} = NOT R%i{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs]);
		else       printf(" [Mode = %i | OPCode = %i] = [NOT-I]  => R%i{%i} = NOT R%i{%i}  \n", mode, op, rd, reg_file[rd], rs, reg_file[rs]);

		break;

	case EXT:
		printf("opcode = EXIT = 0x%x \n", op);
		break;

	default:
		printf("bad opcode \n");
		break;

	}
	PCR = PCR + 1;

	// make sure R0 = 0
	reg_file[0] = 0;

	return 0;
}

int init_reg_file()
{
	int i = 0;
	for (i = 0; i < 32; i++)
		reg_file[i] = 0;
	reg_file[1] = 1;
	return 0;
}

int print_reg_file()
{
	int i = 0;
	for (i = 0; i < 32; i++)
		printf(" reg_file[%i] = %i \n", i, reg_file[i]);
	return 0;
}

int main(void)
{
	int last = 0;
	int i = 0;

	init_reg_file();

	// SORA machine codes for opcodes ADD ... NOT
	mem[0] = 0x00432000;  // ADD-Register mode
	mem[1] = 0x80640065;  // ADD-Integer mode
	mem[2] = 0x04432000;
	mem[3] = 0x84640067;
	mem[4] = 0x08432000;
	mem[5] = 0x88640069;
	mem[6] = 0x0c432000;
	mem[7] = 0x8c64006b;
	mem[8] = 0x10432000;
	mem[9] = 0x9064006d;
	mem[10] = 0x14432000;
	mem[11] = 0x9464006f;
	mem[12] = 0x18432000;
	mem[13] = 0x98640071;
	mem[14] = 0x1c432000;
	mem[15] = 0x9c640073;
	mem[16] = 0x20432000;
	mem[17] = 0xa0640075;
	mem[18] = 0x24432000;
	mem[19] = 0xa4640077;
	mem[20] = 0x28432000;
	mem[21] = 0xa8640079;

	last = 21;

	int PCR = 0;

	printf("SORA Machine Code Simulation (Project 3) \n\n");

	for (i = 0; i <= last; i++)
	{
		PCR = i;
		reg_file[0] = 0;
		reg_file[1] = 1;
		reg_file[2] = 5;
		reg_file[3] = 7;
		reg_file[4] = 9;

		exe_code(PCR);
	}
}
