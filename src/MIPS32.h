/*Copyright (c) 2024-2026 MinervaWare LLC*/
/*Copyright (c) 2022-2024 Tristan Wellman*/

/* Currently tested for use with Mips32r2el
 * THIS DOES NOT HANDLE BE YET */

#ifndef MIPS32_H
#define MIPS32_H

#include "util.h"
#include "asmout.h"
#include "parser.h"
#include "cpu.h"

enum registersMIPS32 {
	MIP32r4=0, /*a0 - a3 function arg registers*/
	MIP32r5,
	MIP32r6,
	MIP32r7,
	MIP32r16, /*s0-s2 (16,17,18) saved*/
	MIP32r17,
	MIP32r2, /*v0 return*/
	MIP32r18,
	MIP32SP /*29*/
};

char *stackAllocateMIPS32(enum cpuType cpu);
char *stackDeallocateMIPS32(char *funName);
void MIPS32GetLVAlloc(char *buf, int bsize, Variable *var);
char *convertInstructionMIPS32(AsmOut *out, Instruction ins);

#endif
