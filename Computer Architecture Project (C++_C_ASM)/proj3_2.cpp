// UMBC Spring 2026 Project 3 (Part 2)
//


#include <stdio.h>
#include <stdlib.h>

// simulated computer memory 200 (32 bit) words
int mem[701] = { 0 };
// simulated register file -- 32 registers
// R0 always = 0;
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


const int LD = 0x1a;

const int ST = 0x1d;


int load_mem()
{
	int i = 0;
	for (i = 0; i < 700; i++)
		mem[i] = i+100;
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
	// printf("mode = %i \t opcode = 0x%x \n", mode, op);

	return 0;
}

int reg_decode(int word, int& rd, int& rs, int& rt)
{
	rd = (word & 0x03E00000) >> 21;
	rs = (word & 0x001F0000) >> 16;
	rt = (word & 0x0000F800) >> 11;
	//printf(" ... reg_decode() -> rd = %i \t  rs = %i \t  rt = %i \n", rd, rs, rt);
	return 0;
}

int int_decode(int word, int& rd, int& rs, int& imm16)
{
	rd = (word & 0x03E00000) >> 21;
	rs = (word & 0x001F0000) >> 16;
	imm16 = word & 0xFFFF; // assume positive two's complement number
	if (imm16 & 0x8000)    // if negative number, sign extend to 32 bits
		imm16 = imm16 | 0xFFFF0000;
	//printf(" ... int_decode() -> rd = %i \t  rs = %i \t  imm16 = %i \n", rd, rs, imm16);
	return 0;
}

int ri_decode(int word, int& rd, int& rs, int& rt, int& imm16)
{
	rd = (word & 0x03E00000) >> 21;
	rs = (word & 0x001F0000) >> 16;
	rt = (word & 0x0000F800) >> 11;
	// printf(" ... reg_decode() -> rd = %i \t  rs = %i \t  rt = %i \n", rd, rs, rt);

	imm16 = word & 0xFFFF; // assume positive two's complement number
	if (imm16 & 0x8000)    // if negative number, sign extend to 32 bits
		imm16 = imm16 | 0xFFFF0000;
	// printf("... int_decode() -> rd = %i \t  rs = %i \t  imm16 = %i \n", rd, rs, imm16);
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
	int rotate = 0, bit0 = 0, bit31 = 0;
	int branch_field_code = 0, destination_address = 0;
	bool test = 0;
	int upper_16bits = 0, lower_16bits = 0;
	int mem_address = 0;
	int memory = 0;
	int address = 0;

	int temp = 0;
	int word = 0;
	bool pcr_changed = false;

	word = mem[PCR];

	op_decode(word, op, mode);

	//printf("\nExe_code() -> Address = 0x%x ... \n", PCR);
	//printf(" ... mode = %i \t opcode = %i \n", mode, op);

	/*	if (mode) // if mode == 1, integer addressing mode
		{
			int_decode(word, rd, rs, imm16);
			temp = imm16;
		}

		else // mode == 0, register-to-register
		{
			reg_decode(word, rd, rs, rt);
			imm16 = 0;
			temp = reg_file[rt];
		}
	*/
	ri_decode(word, rd, rs, rt, imm16);
	if (mode)
	{
		temp = imm16;
		//printf("Mode = I, OPCode = 0x%x, reg_file[rd = %i] = %i \t reg_file[rs = %i] = %i \t imm16 = %i \n", op, rd, reg_file[rd], rs, reg_file[rs], imm16);
	}
	else
	{
		temp = reg_file[rt];
		//printf("Mode = R, OPCode = 0x%x, reg_file[rd = %i] = %i \t reg_file[rs = %i] = %i \t reg_file[rt = %i] = %i \n", op, rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
	}


	switch (op)
	{


	case BEQ:

		if (mode) printf("BEQ-I, reg_file[rd = %i] = %i \t reg_file[rs = %i] = %i \t imm16 = %i \n", rd, reg_file[rd], rs, reg_file[rs], imm16);
		else      printf("BEQ-R, reg_file[rd = %i] = %i \t reg_file[rs = %i] = %i \t reg_file[rt = %i] = %i \n", rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		printf(" -- PCR = (decimal) %i = (hexadecimal) 0x%x \n", PCR, PCR);
		if (reg_file[rd] == reg_file[rs])  // BEQ test case
		{
			destination_address = PCR + 1 + temp;
			PCR = destination_address;
			pcr_changed = true;
			if (mode) printf(" -- BEQ condition is TRUE.     Destination Address = PCR + 1 + imm16 = 0x%x \n\n", PCR);
			else      printf(" -- BEQ condition is TRUE.     Destination Address = PCR + 1 + Reg_File[RT] = 0x%x \n\n", PCR);
			
			
		}
		else // branch condition is false then goto next instruction
		{
			PCR = PCR + 1;
			pcr_changed = true;
			printf(" -- BEQ condition is FALSE.   Next Instruction Address = PCR + 1 = 0x%x \n\n", PCR);
		}
		break;

	case BNE:

		if (mode) printf("BNE-I, reg_file[rd = %i] = %i \t reg_file[rs = %i] = %i \t imm16 = %i \n", rd, reg_file[rd], rs, reg_file[rs], imm16);
		else      printf("BNE-R, reg_file[rd = %i] = %i \t reg_file[rs = %i] = %i \t reg_file[rt = %i] = %i \n", rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		printf(" -- PCR = (decimal) %i = (hexadecimal) 0x%x \n", PCR, PCR);

		// add code here  // BEQ test case
		if (reg_file[rd] != reg_file[rs])
		{
			destination_address = PCR + 1 + temp;
			PCR = destination_address;
			pcr_changed = true;
			if (mode) printf(" -- BNE condition is TRUE.     Destination Address = PCR + 1 + imm16 = 0x%x \n\n", PCR);
			else      printf(" -- BNE condition is TRUE.     Destination Address = PCR + 1 + Reg_File[RT] = 0x%x \n\n", PCR);
		}
		else // branch condition is false then goto next instruction
		{
			PCR = PCR + 1;
			pcr_changed = true;
			printf(" -- BNE condition is FALSE.   Next Instruction Address = PCR + 1 = 0x%x \n\n", PCR);
		}

		break;

	case BGT:

         // add code folling BEQ Above
		if (mode) printf("BGT-I, reg_file[rd = %i] = %i \t reg_file[rs = %i] = %i \t imm16 = %i \n", rd, reg_file[rd], rs, reg_file[rs], imm16);
		else      printf("BGT-R, reg_file[rd = %i] = %i \t reg_file[rs = %i] = %i \t reg_file[rt = %i] = %i \n", rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		printf(" -- PCR = (decimal) %i = (hexadecimal) 0x%x \n", PCR, PCR);

		if (reg_file[rd] > reg_file[rs])
		{
			destination_address = PCR + 1 + temp;
			PCR = destination_address;
			pcr_changed = true;
			if (mode) printf(" -- BGT condition is TRUE.     Destination Address = PCR + 1 + imm16 = 0x%x \n\n", PCR);
			else      printf(" -- BGT condition is TRUE.     Destination Address = PCR + 1 + Reg_File[RT] = 0x%x \n\n", PCR);
		}
		else
		{
			PCR = PCR + 1;
			pcr_changed = true;
			printf(" -- BGT condition is FALSE.   Next Instruction Address = PCR + 1 = 0x%x \n\n", PCR);
		}


		break;
	case BGE:

       // add code folling BEQ Above
		if (mode) printf("BGE-I, reg_file[rd = %i] = %i \t reg_file[rs = %i] = %i \t imm16 = %i \n", rd, reg_file[rd], rs, reg_file[rs], imm16);
		else      printf("BGE-R, reg_file[rd = %i] = %i \t reg_file[rs = %i] = %i \t reg_file[rt = %i] = %i \n", rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		printf(" -- PCR = (decimal) %i = (hexadecimal) 0x%x \n", PCR, PCR);

		if (reg_file[rd] >= reg_file[rs])
		{
			destination_address = PCR + 1 + temp;
			PCR = destination_address;
			pcr_changed = true;
			if (mode) printf(" -- BGE condition is TRUE.     Destination Address = PCR + 1 + imm16 = 0x%x \n\n", PCR);
			else      printf(" -- BGE condition is TRUE.     Destination Address = PCR + 1 + Reg_File[RT] = 0x%x \n\n", PCR);
		}
		else
		{
			PCR = PCR + 1;
			pcr_changed = true;
			printf(" -- BGE condition is FALSE.   Next Instruction Address = PCR + 1 = 0x%x \n\n", PCR);
		}

		break;
	case BLT:

        // add code folling BEQ Above 
		if (mode) printf("BLT-I, reg_file[rd = %i] = %i \t reg_file[rs = %i] = %i \t imm16 = %i \n", rd, reg_file[rd], rs, reg_file[rs], imm16);
		else      printf("BLT-R, reg_file[rd = %i] = %i \t reg_file[rs = %i] = %i \t reg_file[rt = %i] = %i \n", rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		printf(" -- PCR = (decimal) %i = (hexadecimal) 0x%x \n", PCR, PCR);

		if (reg_file[rd] < reg_file[rs])
		{
			destination_address = PCR + 1 + temp;
			PCR = destination_address;
			pcr_changed = true;
			if (mode) printf(" -- BLT condition is TRUE.     Destination Address = PCR + 1 + imm16 = 0x%x \n\n", PCR);
			else      printf(" -- BLT condition is TRUE.     Destination Address = PCR + 1 + Reg_File[RT] = 0x%x \n\n", PCR);
		}
		else
		{
			PCR = PCR + 1;
			pcr_changed = true;
			printf(" -- BLT condition is FALSE.   Next Instruction Address = PCR + 1 = 0x%x \n\n", PCR);
		}





		break;
	case BLE:
        // add code folling BEQ Above 
		if (mode) printf("BLE-I, reg_file[rd = %i] = %i \t reg_file[rs = %i] = %i \t imm16 = %i \n", rd, reg_file[rd], rs, reg_file[rs], imm16);
		else      printf("BLE-R, reg_file[rd = %i] = %i \t reg_file[rs = %i] = %i \t reg_file[rt = %i] = %i \n", rd, reg_file[rd], rs, reg_file[rs], rt, reg_file[rt]);
		printf(" -- PCR = (decimal) %i = (hexadecimal) 0x%x \n", PCR, PCR);

		if (reg_file[rd] <= reg_file[rs])
		{
			destination_address = PCR + 1 + temp;
			PCR = destination_address;
			pcr_changed = true;
			if (mode) printf(" -- BLE condition is TRUE.     Destination Address = PCR + 1 + imm16 = 0x%x \n\n", PCR);
			else      printf(" -- BLE condition is TRUE.     Destination Address = PCR + 1 + Reg_File[RT] = 0x%x \n\n", PCR);
		}
		else
		{
			PCR = PCR + 1;
			pcr_changed = true;
			printf(" -- BLE condition is FALSE.   Next Instruction Address = PCR + 1 = 0x%x \n\n", PCR);
		}

		break;

	case LD:

		// Rd = mem[Rs + Rt]
		printf(" ... Instructrion = LD MODE = %i {LD-R | LD-I} \n", mode);

		// add code here
		if (mode)
			mem_address = reg_file[rs] + imm16;
		else
			mem_address = reg_file[rs] + reg_file[rt];

		reg_file[rd] = mem[mem_address];
		printf("... mode = %i mem_address = %i, reg_file[%i] = %i \n", mode, mem_address, rd, reg_file[rd]);
	
		break;




	case ST:
		printf(" ... Instructrion = ST MODE = %i {ST-R | ST-I} \n", mode);

		// mem[Rs + Rt] = Rd

                // add code here
		if (mode)
			mem_address = reg_file[rs] + imm16;
		else
			mem_address = reg_file[rs] + reg_file[rt];

		mem[mem_address] = reg_file[rd];

		printf("... mode = %i mem_address = %i, reg_file[%i] = %i \n", mode, mem_address, rd, reg_file[rd]);
		printf("... mode = %i mem[mem_address] = %i, reg_file[%i] = %i \n\n", mode, mem[mem_address], rd, reg_file[rd]);
		break;



	default:
		printf("bad opcode \n");
		break;

	}

	if (!pcr_changed)
		PCR = PCR + 1;

	// make sure R0 = 0
	reg_file[0] = 0;
	// make sure R1 = 1
	reg_file[1] = 1;
	//printf(" ... PCR = %i \n", PCR);
	//printf("... End Exe_code() \n");
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
	int word = 0;
	int i = 0;

	load_mem();

	init_reg_file();

	mem[0] = 0x00432000;
	mem[1] = 0x80640065;
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



	mem[22] = 0x2c432000;
	mem[23] = 0xac64007b;
	mem[24] = 0x30432000;
	mem[25] = 0xb064007d;
	mem[26] = 0x34432000;
	mem[27] = 0xb464007f;
	mem[28] = 0x38432000;

	mem[29] = 0xb8640081;
	mem[30] = 0x3c432000;
	mem[31] = 0xbc640083;
	mem[32] = 0x40432000;
	mem[33] = 0xc0640085;

	mem[34] = 0x44432000;
	mem[35] = 0xc4640087;
	mem[36] = 0x48432000;
	mem[37] = 0xc8640089;
	mem[38] = 0x4c432000;
	mem[39] = 0xcc64008b;
	mem[40] = 0x50432000;
	mem[41] = 0xd064008d;
	mem[42] = 0x54432000;
	mem[43] = 0xd464008f;


	mem[44] = 0x68432000;
	mem[45] = 0xe8640099;

	mem[46] = 0x74432000;
	mem[47] = 0xf464009f;



	// print_reg_file();

	//read_mem(last);
	//load_mem();
	//print_mem();
	last = 47;

	// printf("last = %i \n", last);

	int PCR = 0;


	printf("---------------------------------------------------------\n\n");
	for (i = 32; i <= 47; i = i + 2)
	{
		PCR = i;  // R-mode

		reg_file[2] = 10;
		reg_file[3] = 10;
		reg_file[4] = 512;
		exe_code(PCR);

		PCR = i;    // R-mode
		reg_file[2] = 5;
		reg_file[3] = 10;
		reg_file[4] = 256;
		exe_code(PCR);


		PCR = i + 1; // I-mode

		reg_file[2] = 5;
		reg_file[3] = 132;
		reg_file[4] = 132;

		exe_code(PCR);

		PCR = i + 1; // I-mode

		reg_file[2] = 5;
		reg_file[3] = 133;
		reg_file[4] = 456;

		exe_code(PCR);
		printf("---------------------------------------------------------\n\n");

	}













	/*

	exe_code(PCR); // ADD
	exe_code(PCR);

	exe_code(PCR); // SUB
	exe_code(PCR);

	exe_code(PCR); // AND
	exe_code(PCR);

	exe_code(PCR); // OR
	exe_code(PCR);

	exe_code(PCR); // XOR
	exe_code(PCR);

	exe_code(PCR); // SR
	exe_code(PCR);

	exe_code(PCR); // SL
	exe_code(PCR);

	exe_code(PCR); // ASR
	exe_code(PCR);

	exe_code(PCR); // RR
	exe_code(PCR);

	exe_code(PCR); // RL
	exe_code(PCR);

	exe_code(PCR); // NOT
	exe_code(PCR);


	*/


	// print_reg_file();

}
