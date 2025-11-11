/*Copyright (c) 2024 Tristan Wellman*/
#ifndef AMD_X86_64_H
#define AMD_X86_64_H
#include "util.h"
#include "asmout.h"
#include "parser.h"

enum registersAMD_X86_64 {
#if defined _WIN32 || defined _WIN64 || defined __WIN32__ || __CYGWIN__
  	RCX=0,
	RDX,
  	R8_,
  	R9_,
  	RDI,
  	RSI,
  	RAX,
  	XMM0,
  	AMD64SP
#else
    RDI=0,
    RSI,
    RDX,
    RCX,
    R8_,
    R9_,
    RAX, /*Return value*/
    XMM0,
    AMD64SP
#endif
};

/*The registers here are just the ones I currently map to, not all the registers*/
#define ISREG(a_) \
	(!strcmp(a_,"rdi")||!strcmp(a_,"rsi")|| \
	 !strcmp(a_,"rdx")||!strcmp(a_,"rcx")|| \
	 !strcmp(a_,"r8")||!strcmp(a_,"r9")|| \
	 !strcmp(a_,"rax")||!strcmp(a_,"xmm0")|| \
	 !strcmp(a_,"sp"))

char *stackAllocateAMD_X86_64();
char *stackDeallocateAMD_X86_64();
char *convertInstructionAMD_X86_64(AsmOut *out, Instruction ins);

#endif
