/*Copyright (c) 2026 MinervaWare LLC*/
#ifndef RV64_H
#define RV64_H

#include "util.h"
#include "asmout.h"
#include "parser.h"
#include "cpu.h"

enum registersRV64 {
	RVa0=0,
	RVa1,
	RVa2,
	RVa3,
	RVa4,
	RVa5,
	RVa6,
	RVa7,
	RV64SP,
	RVfa0,
	RVfa1,
	RVfa2,
	RVfa3,
	RVfa4,
	RVfa5,
	RVfa6
};

char *stackAllocateRV64(enum cpuType cpu);
char *stackDeallocateRV64();
char *RV64GetLVAlloc(char *buf, size_t bSize, Variable *var);
char *convertInstructionRV64(AsmOut *out, Instruction ins);

#endif
