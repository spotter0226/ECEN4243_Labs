/***************************************************************/
/*                                                             */
/*   ARMv4-32 Instruction Level Simulator                      */
/*                                                             */
/*   ECEN 4243                                                 */
/*   Oklahoma State University                                 */
/*                                                             */
/***************************************************************/

#ifndef _SIM_ISA_H_
#define _SIM_ISA_H_
#define N_CUR ( (CURRENT_STATE.CPSR>>31) & 0x00000001 )
#define Z_CUR ( (CURRENT_STATE.CPSR>>30) & 0x00000001 )
#define C_CUR ( (CURRENT_STATE.CPSR>>29) & 0x00000001 )
#define V_CUR ( (CURRENT_STATE.CPSR>>28) & 0x00000001 )
#define N_NXT ( (NEXT_STATE.CPSR>>31) & 0x00000001 )
#define Z_NXT ( (NEXT_STATE.CPSR>>30) & 0x00000001 )
#define C_NXT ( (NEXT_STATE.CPSR>>29) & 0x00000001 )
#define V_NXT ( (NEXT_STATE.CPSR>>28) & 0x00000001 )

#define N_N 0x80000000 
#define Z_N 0x40000000
#define C_N 0x20000000
#define V_N 0x10000000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"


//ADD
int ADD(int Rd, int Rn, int Operand2, int I, int S, int CC) {

	int cur = 0;
	if (I == 0) {
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] << shamt5);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] +
				((CURRENT_STATE.REGS[Rm] >> shamt5) |
					(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		else
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] +
				((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
					(CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
				break;
			}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = CURRENT_STATE.REGS[Rn] + (Imm >> 2 * rotate | (Imm << (32 - 2 * rotate)));
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;

}


//AND
int AND(int Rd, int Rn, int Operand2, int I, int S, int CC) {

	int cur = 0;
	if (I == 0) {
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] &
				(CURRENT_STATE.REGS[Rm] << shamt5);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] &
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] &
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] &
				((CURRENT_STATE.REGS[Rm] >> shamt5) |
					(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		else
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] &
				(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] &
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] &
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] &
				((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
					(CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
				break;
			}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = CURRENT_STATE.REGS[Rn] & (Imm >> 2 * rotate | (Imm << (32 - 2 * rotate)));
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;
}


//EOR
int EOR(int Rd, int Rn, int Operand2, int I, int S, int CC) {

	int cur = 0;
	if (I == 0) {
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] ^
				(CURRENT_STATE.REGS[Rm] << shamt5);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] ^
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] ^
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] ^
				((CURRENT_STATE.REGS[Rm] >> shamt5) |
					(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		else
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] ^
				(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] ^
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] ^
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] ^
				((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
					(CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
				break;
			}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = CURRENT_STATE.REGS[Rn] ^ (Imm >> 2 * rotate | (Imm << (32 - 2 * rotate)));
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;

}


//SUB
int SUB(int Rd, int Rn, int Operand2, int I, int S, int CC) {

	int cur = 0;
	if (I == 0) {
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] << shamt5);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] -
				((CURRENT_STATE.REGS[Rm] >> shamt5) |
					(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		else
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] -
				((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
					(CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
				break;
			}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = CURRENT_STATE.REGS[Rn] - (Imm >> 2 * rotate | (Imm << (32 - 2 * rotate)));
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;

}


//ADC
int ADC(int Rd, int Rn, int Operand2, int I, int S, int CC) {

	int cur = 0;
	if (I == 0) {
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] << shamt5);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] +
				((CURRENT_STATE.REGS[Rm] >> shamt5) |
					(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		else
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] +
				((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
					(CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
				break;
			}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = CURRENT_STATE.REGS[Rn] + (Imm >> 2 * rotate | (Imm << (32 - 2 * rotate)));
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;

}


//SBC
int SBC(int Rd, int Rn, int Operand2, int I, int S, int CC) 
{
	int cur = 0;
	if (I == 0) {
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] << shamt5);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] -
				((CURRENT_STATE.REGS[Rm] >> shamt5) |
					(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		else
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] -
				((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
					(CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
				break;
			}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = CURRENT_STATE.REGS[Rn] - (Imm >> 2 * rotate | (Imm << (32 - 2 * rotate)));
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;

}


//TST -- NEEDS WORK -- Might be fixed??
int TST(int Rd, int Rn, int Operand2, int I, int S, int CC) {

	int cur = 0;
	if (I == 0) {
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] &
				(CURRENT_STATE.REGS[Rm] << shamt5);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] &
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] &
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] &
				((CURRENT_STATE.REGS[Rm] >> shamt5) |
					(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		else
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] &
				(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] &
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] &
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] &
				((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
					(CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
				break;
			}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = CURRENT_STATE.REGS[Rn] & (Imm >> 2 * rotate | (Imm << (32 - 2 * rotate)));
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;

}


//TEQ -- NEEDS WORK -- Might be fixed??
int TEQ(int Rd, int Rn, int Operand2, int I, int S, int CC) {

	int cur = 0;
	if (I == 0) {
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] ^
				(CURRENT_STATE.REGS[Rm] << shamt5);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] ^
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] ^
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] ^
				((CURRENT_STATE.REGS[Rm] >> shamt5) |
					(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		else
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] ^
				(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] ^
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] ^
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] ^
				((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
					(CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
				break;
			}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = CURRENT_STATE.REGS[Rn] ^ (Imm >> 2 * rotate | (Imm << (32 - 2 * rotate)));
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;

}


//CMP -- NEEDS WORK
int CMP(int SBZ, int Rn, int Operand2, int I, int S, int CC) {

	int cur = 0;
	if (I == 0) {
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] << shamt5);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] -
				((CURRENT_STATE.REGS[Rm] >> shamt5) |
					(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		else
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] -
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] -
				((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
					(CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
				break;
			}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = CURRENT_STATE.REGS[Rn] - (Imm >> 2 * rotate | (Imm << (32 - 2 * rotate)));
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;

}


//CMN-  NEEDS WORK
int CMN(int SBZ, int Rn, int Operand2, int I, int S, int CC) {

	int cur = 0;
	if (I == 0) {
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] << shamt5);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] +
				((CURRENT_STATE.REGS[Rm] >> shamt5) |
					(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		else
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] +
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] +
				((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
					(CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
				break;
			}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = CURRENT_STATE.REGS[Rn] + (Imm >> 2 * rotate | (Imm << (32 - 2 * rotate)));
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;

}


//ORR
int ORR(int Rd, int Rn, int Operand2, int I, int S, int CC) {

	int cur = 0;
	if (I == 0) {
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] |
				(CURRENT_STATE.REGS[Rm] << shamt5);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] |
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] |
				(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] |
				((CURRENT_STATE.REGS[Rm] >> shamt5) |
					(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		else
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] |
				(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] |
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] |
				(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] |
				((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
					(CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
				break;
			}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = CURRENT_STATE.REGS[Rn] | (Imm >> 2 * rotate | (Imm << (32 - 2 * rotate)));
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;

}

// NEEDS MORE WORK -- Might be correct now??
int MOV(int Rd, int Rn, int Operand2, int I, int S, int CC) {

	int cur = 0;
	if (I == 0) {
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rm] << shamt5;
				break;
			case 1: cur = CURRENT_STATE.REGS[Rm] >> shamt5;
				break;
			case 2: cur = CURRENT_STATE.REGS[Rm] >> shamt5;
				break;
			case 3: cur = ((CURRENT_STATE.REGS[Rm] >> shamt5) |
				(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		else
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs];
				break;
			case 1: cur = CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs];
				break;
			case 2: cur = CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs];
				break;
			case 3: cur = (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
				(CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs]));
				break;
			}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = (Imm >> 2 * rotate | (Imm << (32 - 2 * rotate)));
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;
}

//NEED TO WRITE THESE
int ASR(int Rd, int Rn, int Operand2, int I, int S, int CC)
{
	int cur = 0;
	if (I == 0)
	{
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;

		//Make it 32 bits
		CURRENT_STATE.REGS[Rm] = CURRENT_STATE.REGS[Rm] << 8;
		CURRENT_STATE.REGS[Rm] = CURRENT_STATE.REGS[Rm] & 0xFFFFFFFF;
		CURRENT_STATE.REGS[Rm] = (signed int)CURRENT_STATE.REGS[Rm] >> 8; // now this should be 32-bits

		if (bit4 == 0)
			switch (sh) {
			case 0: cur = (signed int)CURRENT_STATE.REGS[Rm] << shamt5;
				break;
			case 1: cur = (signed int)CURRENT_STATE.REGS[Rm] >> shamt5;
				break;
			case 2: cur = (signed int)CURRENT_STATE.REGS[Rm] >> shamt5;
				break;
			case 3: cur = ((signed int)(CURRENT_STATE.REGS[Rm] >> shamt5) |
				(signed int)(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		else
			switch (sh) {
			case 0: cur = (signed int)CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs];
				break;
			case 1: cur = (signed int)CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs];
				break;
			case 2: cur = (signed int)CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs];
				break;
			case 3: cur = ((signed int)CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
				((signed int)CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs]));
				break;
			}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = (signed int)Imm >> 2 * rotate | ((signed int)Imm << (32 - 2 * rotate));
	}

	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;
}

int LSL(int Rd, int Rn, int Operand2, int I, int S, int CC) {
	int cur = 0;
	if (I == 0)
	{
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
		{
			//HELPITSNOTRIGHT
			cur = (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
		}
		else
		{
			//STILLNOTRIGHT
			cur = (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
		}
	}
	if (I == 1) {
		cur = (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;
}

int LSR(int Rd, int Rn, int Operand2, int I, int S, int CC) {
	if (I == 0)
	{
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
		{
			//HELPITSNOTRIGHT- maybe closer
			cur = (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
		}
		else
		{
			//STILLNOTRIGHT- maybe closer
			cur = (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
		}
	}
	if (I == 1) {
		cur = (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs])
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;
}

int ROR(int Rd, int SBZ, int Operand2, int I, int S, int CC) {
	int cur = 0;
	int temp = 0;
	if (I == 0)
	{
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;

		temp = CURRENT_STATE.REGS[Rm] & shamt5;

		if (bit4 == 0)
		{
			switch (sh) {
			case 0: cur = (CURRENT_STATE.REGS[Rm] << shamt5);
				break;
			case 1: cur = (CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 2: cur = (CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 3: cur = ((CURRENT_STATE.REGS[Rm] >> shamt5) |
				(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		}
		else
		{
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs];
				break;
			case 1: cur = CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs];
				break;
			case 2: cur = CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs];
				break;
			case 3: cur = (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
				(CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs]));
				break;
			}
		}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = Imm >> 2 * rotate | (Imm << (32 - 2 * rotate));
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;
}

//MVN
int MVN(int Rd, int Rn, int Operand2, int I, int S, int CC) {

	int cur = 0;
	if (I == 0) {
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
			switch (sh) {
			case 0: cur = ~(CURRENT_STATE.REGS[Rm] << shamt5);
				break;
			case 1: cur = ~(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 2: cur = ~(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 3: cur = ~((CURRENT_STATE.REGS[Rm] >> shamt5) |
				(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		else
			switch (sh) {
			case 0: cur = ~(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
				break;
			case 1: cur = ~(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 2: cur = ~(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 3: cur = ~((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
				(CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
				break;
			}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = ~(Imm >> 2 * rotate | (Imm << (32 - 2 * rotate)));
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;
}


//BIC
int BIC(int Rd, int Rn, int Operand2, int I, int S, int CC)
{
	int cur = 0;
	if (I == 0) {
		int sh = (Operand2 & 0x00000060) >> 5;
		int shamt5 = (Operand2 & 0x00000F80) >> 7;
		int bit4 = (Operand2 & 0x00000010) >> 4;
		int Rm = Operand2 & 0x0000000F;
		int Rs = (Operand2 & 0x00000F00) >> 8;
		if (bit4 == 0)
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] &
				~(CURRENT_STATE.REGS[Rm] << shamt5);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] &
				~(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] &
				~(CURRENT_STATE.REGS[Rm] >> shamt5);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] &
				~((CURRENT_STATE.REGS[Rm] >> shamt5) |
					~(CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
				break;
			}
		else
			switch (sh) {
			case 0: cur = CURRENT_STATE.REGS[Rn] &
				~(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
				break;
			case 1: cur = CURRENT_STATE.REGS[Rn] &
				~(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 2: cur = CURRENT_STATE.REGS[Rn] &
				~(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
				break;
			case 3: cur = CURRENT_STATE.REGS[Rn] &
				~((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
					~(CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
				break;
			}
	}
	if (I == 1) {
		int rotate = Operand2 >> 8;
		int Imm = Operand2 & 0x000000FF;
		cur = CURRENT_STATE.REGS[Rn] & ~(Imm >> 2 * rotate | (Imm << (32 - 2 * rotate)));
	}
	NEXT_STATE.REGS[Rd] = cur;
	if (S == 1) {
		if (cur < 0)
			NEXT_STATE.CPSR |= N_N;
		if (cur == 0)
			NEXT_STATE.CPSR |= Z_N;
		if ((signed int)cur > 0x7FFFFFFF || (signed int)cur < 0xFFFFFFFF)
			NEXT_STATE.CPSR |= V_N;
		if (cur > 0xFFFFFFFF)
			NEXT_STATE.CPSR |= C_N;
	}
	return 0;
}




//B
int B(int offset2, int CC)
{
	int off = offset2 & 0x00111111;
	off = off << 6;
	off = (signed int)off >> 6;
	off = off << 2;

	switch (CC) {
	case 0:
		// for if Branch is equal
		if (Z_CUR == 0) {
			CURRENT_STATE.REGS[15] = (CURRENT_STATE.REGS[15] + 8) + off;
		}
		else
			return 0;
		break;
	case 1:
		// for if Branch is not equal
		if (Z_CUR == 1) {
			CURRENT_STATE.REGS[15] = (CURRENT_STATE.REGS[15] + 8) + off;
		}
		else
			return 0;
		break;
	}
	return 0;
}

//BL
int BL(int offset2, int CC)
{
	int off = offset2 & 0x00111111;
	off = off << 6;
	off = (signed int)off >> 6;
	off = off << 2;

	switch (CC) {
	case 0:
		// Branch equal
		if (Z_CUR == 0) {
			CURRENT_STATE.REGS[14] = (CURRENT_STATE.REGS[15] + 8) - 4;
			CURRENT_STATE.REGS[15] = (CURRENT_STATE.REGS[15] + 8) + off;
		}
		else
			return 0;
		break;
	case 1:
		// Branch Not Equal
		if (Z_CUR == 1) {
			CURRENT_STATE.REGS[14] = (CURRENT_STATE.REGS[15] + 8) - 4;
			CURRENT_STATE.REGS[15] = (CURRENT_STATE.REGS[15] + 8) + off;
		}
		else
			return 0;
		break;
	}
	return 0;
}

//NOT DONE -- NEED HELP
/*int LDR(int Rd, int Rn, int I, int P, int U, int W, int src2, int CC) {
	//if(Rd == 15)
	int data = 0;
	int Rm = src2 & 0x00000000F;

	//Immediate values
	if (I == 0) {
		if (P == 1 & W == 0) {
			if (U == 1)
				data = CURRENT_STATE.REGS[Rn] + src2;
			else
				data = CURRENT_STATE.REGS[Rn] - src2;
			return 0;
		}
		else if (P == 1 & W == 1) {
			if (U == 1)
				data = CURRENT_STATE.REGS[Rn] + src2;
			else
				data = CURRENT_STATE.REGS[Rn] - src2;
			//NOT SURE WHAT TO DO
			return 0;
		}
		else if (P == 0 & W == 0) {
			if (Rn == Rd) {
				if (U == 1)
					data = CURRENT_STATE.REGS[Rn] + src2;
				else
					data = CURRENT_STATE.REGS[Rn] - src2;
			}
			return 0;
		}
	}
	//Non-shifted registered treated as shifted register with shift of 0
	else {
		int shamt_5 = (src2 >> 7) & 0x0000001F;
		int sh = (src2 >> 5) & 0x00000003;
		int Rm = src2 & 0x0000000F;
		int index = 0;
		int off = 0;

		if (P == 1 & W == 0) {
			switch (sh) {
			case 0:
				index = CURRENT_STATE.REGS[Rm] << shamt_5;
				break;
			case 1:
				index = CURRENT_STATE.REGS[Rm] >> shamt_5;
				break;
			case 2:
				off = CURRENT_STATE.REGS[Rm];
				off = (signed int)off >> shamt_5;
				index = off;
				break;
			case 3:
				index = ((CURRENT_STATE.REGS[Rm] >> shamt_5) |
					(CURRENT_STATE.REGS[Rm] << (32 - shamt_5)));
				break;
			}
			if (U == 1) data = CURRENT_STATE.REGS[Rn] + index;
			else data = CURRENT_STATE.REGS[Rn] - index;

			return 0;
		}
		else if (P == 1 & W == 1) {
			int shamt_5 = (src2 >> 7) & 0x0000001F;
			int sh = (src2 >> 5) & 0x00000003;
			int Rm = src2 & 0x0000000F;
			int index = 0;
			int off;

			switch (sh) {
			case 0:
				index = CURRENT_STATE.REGS[Rm] << shamt_5;
				break;
			case 1:
				index = CURRENT_STATE.REGS[Rm] >> shamt_5;
				break;
			case 2:
				off = CURRENT_STATE.REGS[Rm];
				off = (signed int)off >> shamt_5;
				index = off;
				break;
			case 3:
				index = ((CURRENT_STATE.REGS[Rm] >> shamt_5) |
					(CURRENT_STATE.REGS[Rm] << (32 - shamt_5)));
				break;
			}

			if (U == 1) data = CURRENT_STATE.REGS[Rn] + index;
			else data = CURRENT_STATE.REGS[Rn] - index;
			CURRENT_STATE.REGS[Rn] = data;

			return 0;
		}
		else if (P == 0 & W == 0) {
			int shamt_5 = (src2 >> 7) & 0x0000001F;
			int sh = (src2 >> 5) & 0x00000003;
			int Rm = src2 & 0x0000000F;
			int index = 0;
			int off = 0;


			switch (sh) {
			case 0:
				index = CURRENT_STATE.REGS[Rm] << shamt_5;
				break;
			case 1:
				index = CURRENT_STATE.REGS[Rm] >> shamt_5;
				break;
			case 2:
				off = CURRENT_STATE.REGS[Rm];
				off = (signed int)off >> shamt_5;
				index = off;
				break;
			case 3:
				index = ((CURRENT_STATE.REGS[Rm] >> shamt_5) |
					(CURRENT_STATE.REGS[Rm] << (32 - shamt_5)));
				break;
			}
			if (U == 1) data = CURRENT_STATE.REGS[Rn] + index;
			else data = CURRENT_STATE.REGS[Rn] - index;
			CURRENT_STATE.REGS[Rn] = data;

			return 0;
		}
	}
}*/
//Once figure out LDR, hopefully these will work out
/*int LDRB(int Rd, int Rn, int I, int P, int U, int W, int src2, int CC);
int STR(int Rd, int Rn, int I, int P, int U, int W, int src2, int CC));
int STRB(int Rd, int Rn, int I, int P, int U, int W, int src2, int CC);*/

//int MLA (char* i_); Don't worry about these right now
//int MUL (char* i_); Don't worry about these right now

int SWI(char* i_) { return 0; }

#endif
