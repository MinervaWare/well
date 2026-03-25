/*Copyright (c) 2024-2026 MinervaWare LLC*/
/*Copyright (c) 2022-2024 Tristan Wellman*/

/*PPC will handle PPC & PPC64*/

#ifndef PPC_H
#define PPC_H
#include "util.h"
#include "asmout.h"
#include "parser.h"
#include "cpu.h"

#define PPC64RESERVEDGPOFFSET 48

enum registersPPC {
	r3,
	r4, /*r4-r10 are general purpose / function arguments*/
	r5,
	r6,
	r7,
	r8,
	r9,
	r10,
	r1 /*sp*/
};

char *stackAllocatePPC(enum cpuType cpu);
char *stackDeallocatePPC();
void PPCGetLVTAlloc(char *buf, size_t bSize, Variable *var);
char *mapRegiserPPC(char *reg);
char *convertInstructionPPC(AsmOut *out, Instruction ins);

#endif
