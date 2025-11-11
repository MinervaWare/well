/*Copyright (c) 2024 Tristan Wellman*/

#ifndef ASMOUT_H
#define ASMOUT_H
#include "util.h"
#include "werror.h"
#include "parser.h"

#define DEFVARBSIZE 1024

/*
 * Registers:
 * R0 - R7: General purpose registers
 * SP: Stack Pointer.
 * TODO add more for compat
 * */
enum registers {
	R1=0,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
	R8,
	SP
};

struct bufferOut {
	char *asmOutBuffer; /*single super long string*/
	int AOBSize;
};

struct asmBuffers {

	/* This should be something like:
	 *  .section __TEXT,__text
	 *  .global _main
	 *  .p2align 2
	 * */
	char *heading; 
	char *variables;
	char *functions;

	char *externals;
	char *includes;

	struct bufferOut output;
};

/*Parser data in asm form*/
typedef struct {

	FILE *asmOut;
	struct asmBuffers buffers;
	
	struct parserData *parser;
	
	/*malloc to total function length in parser*/
	Function *functions;

} AsmOut;

int checkRegister(char *reg);
int regToEnum(char *reg);
char *dumpInlineASM(Instruction *ins);
void convertToAsm(AsmOut *out); 
void freeAsm(AsmOut *out);
void initAsmOut(struct parserData *parser, AsmOut *output); 

#endif
