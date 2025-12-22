/*Copyright (c) 2022-2025 MinervaWare LLC*/
#ifndef ARM_MAC_H
#define ARM_MAC_H
#include "util.h"
#include "asmout.h"
#include "parser.h"
#include "cpu.h"

enum registersARM_MAC {
	X0=0,
	X1,
	X2,
	X3,
	X4,
	X5,
	X6,
	X7, /*For now we are only gonna deal with the first 7*/
	MACARMSP
};

typedef struct {
	char prevRegType;
} MacRegData;

char *stackAllocateARM_MAC(enum cpuType cpu);
char *stackDeallocateARM_MAC();
void ARM_MACGetLVAlloc(char *buf, int bSize, Variable *var);
char *convertInstructionARM_MAC(AsmOut *out, Instruction ins);

#endif
