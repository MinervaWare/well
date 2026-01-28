/*Copyright (c) 2022-2025 MinervaWare LLC*/
#ifndef AMD_X86_64_H
#define AMD_X86_64_H
#include "util.h"
#include "asmout.h"
#include "parser.h"
#include "cpu.h"

enum registersAMD_X86_64 {
#if defined _WIN32 || defined _WIN64 || defined __WIN32__ || __CYGWIN__
  	RCX=0,
	RDX,
  	R8_,
  	R9_,
  	RDI,
  	RSI,
  	RAX,
  	R10_,
  	AMD64SP,
	XMM0,
	XMM1,
	XMM2,
	XMM3,
	XMM4,
	XMM5,
	XMM6
#else
    RDI=0,
    RSI,
    RDX,
    RCX,
    R8_,
    R9_,
    RAX, /*Return value*/
    R10_,
    AMD64SP,
	XMM0,
	XMM1,
	XMM2,
	XMM3,
	XMM4,
	XMM5,
	XMM6
#endif
};

/*The registers here are just the ones I currently map to, not all the registers*/
#define ISREG(a_) \
	(!strcmp(a_,"rdi")||!strcmp(a_,"rsi")|| \
	 !strcmp(a_,"rdx")||!strcmp(a_,"rcx")|| \
	 !strcmp(a_,"r8")||!strcmp(a_,"r9")|| \
	 !strcmp(a_,"rax")||!strcmp(a_,"r10")|| \
	 !strcmp(a_,"sp"))||!strcmp(a_,"xmm0")|| \
	 !strcmp(a_,"xmm1")||!strcmp(a_,"xmm2")|| \
	 !strcmp(a_,"xmm3")||!strcmp(a_,"xmm4")|| \
	 !strcmp(a_,"xmm5")||!strcmp(a_,"xmm6")

char *stackAllocateAMD_X86_64(enum cpuType cpu);
char *stackDeallocateAMD_X86_64();
void AMD_X86_64GetLVAlloc(char *buf, size_t bSize, Variable *var);
char *convertInstructionAMD_X86_64(AsmOut *out, Instruction ins);

#endif
