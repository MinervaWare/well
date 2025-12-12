/*Copyright (c) 2022-2025 MinervaWare LLC*/

#ifndef ALPHA_H
#define ALPHA_H 
#include "util.h"
#include "asmout.h"
#include "parser.h"


/*refer to http://www.alphalinux.org/wiki/index.php/Alpha_Assembly*/
enum registersALPHA {
	a0, /*$16 - $21*/
	a1,
	a2,
	a3,
	a4,
	a5,
	v0, /*$0 - return value*/
	t0, /*$1 - tmp reg*/
	sp, /*$30*/
};

char *stackAllocateALPHA();
char *stackDeallocateALPHA();
void ALPHAGetLVTAlloc(char *buf, size_t bSize, Variable *var);
char *mapRegisterALPHA(char *reg);
char *convertInstructionALPHA(AsmOut *out, Instruction ins);

#endif
