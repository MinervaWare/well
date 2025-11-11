/*Copyright (c) 2024 Tristan Wellman*/
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "AMD_X86_64.h"

char *stackAllocateAMD_X86_64() {
	/*auto to 16*/
	if(CPU==AMD_X86_64) {
		char *ret = calloc(1024, sizeof(char));
		sprintf(ret, "\tpushq %%rbp\n\tmovq %%rsp, %%rbp\n\tsubq $32, %%rsp\n");
		return ret;
	} else if(CPU==I386) {
		char *ret = calloc(1024, sizeof(char));
		sprintf(ret, "\tpushl %%ebp\n\tmovl %%esp, %%ebp\n\tsubl $32, %%esp\n");
		return ret;
	}
	return NULL;
}

char *stackDeallocateAMD_X86_64() {
	/*auto to 16*/
	if(CPU==AMD_X86_64) {
		char *ret = calloc(1024, sizeof(char));
		sprintf(ret, "\taddq $32, %%rsp\n\tpopq %%rbp\n");
		return ret;
	} else if(CPU==I386) {
		char *ret = calloc(1024, sizeof(char));
		sprintf(ret, "\taddl $32, %%esp\n\tpopl %%ebp\n");
		return ret;
	}
	return NULL;
}

char *initLocalVariables(Function *func) {
	if(func==NULL) return NULL;
	char *res = NULL;

	return res;
}

char *mapRegisterAMD_X86_64(char *reg) {
    int regNum = regToEnum(reg);
	if(CPU==AMD_X86_64) {
    	switch(regNum) {
        	case RDI: return "rdi";
        	case RSI: return "rsi";
        	case RDX: return "rdx";
        	case RCX: return "rcx";
        	case R8_: return "r8";
        	case R9_: return "r9";
        	case RAX: return "rax";
        	case XMM0: return "xmm0";
        	case AMD64SP: return "sp";
    	};
	} else if(CPU==I386) {
    	switch(regNum) {
        	case RDI: return "edi";
        	case RSI: return "esi";
        	case RDX: return "edx";
        	case RCX: return "ecx";
        	case R8_: return "r8";
        	case R9_: return "r9";
        	case RAX: return "eax";
        	case XMM0: return "xmm0";
        	case AMD64SP: return "sp";
    	};
	}
    return NULL;
}

char *getCurrentVar(struct parserData *parser, Instruction *ins, int argSpot) {
	if(parser==NULL||ins==NULL) return NULL;
	char *res = NULL;
	Variable *v = NULL;
	v = getVarFrom(parser, ins->arguments[argSpot]);
	char asmVName[1024];
	if(v!=NULL) {
		switch(v->type) {
			case STRING: snprintf(asmVName, sizeof(asmVName),
								 "wl_str_%s", ins->arguments[argSpot]);break;
			case CHAR: snprintf(asmVName, sizeof(asmVName),
							   "wl_ch_%s", ins->arguments[argSpot]);break;
			case INT: snprintf(asmVName, sizeof(asmVName),
							  "wl_int_%s", ins->arguments[argSpot]);break;
			case FLOAT: snprintf(asmVName, sizeof(asmVName),
								"wl_fl_%s", ins->arguments[argSpot]);break;
			case VOID: /*TODO*/break;
			case ZERO: snprintf(asmVName, sizeof(asmVName), 
							   "wl_z_%s", ins->arguments[argSpot]);break;
		};
		if(CPU==AMD_X86_64) strcat(asmVName, "(%rip)");
	} else {
		v = NULL;
		v = queryLocalVariable(parser, ins->lineNum, ins->arguments[argSpot]);
		if(v!=NULL) snprintf(asmVName, sizeof(asmVName), "-%d(%%rbp)", v->offset);
		else {
			snprintf(asmVName, sizeof(asmVName), "_%s", ins->arguments[argSpot]);
			if(CPU==AMD_X86_64) strcat(asmVName, "(%rip)");
		}
	}
	res = calloc(strlen(asmVName)+1, sizeof(char));
	strcpy(res, asmVName);
	return res;
}

/*
 * Tri-Argument Instruction with destination on right.
 * */
char* triArgInsRDest(char *ins, Instruction *wins) {
	int bsize = wins->argLen*1024;
	char *buf = calloc(bsize, sizeof(char));
	char *dest = mapRegisterAMD_X86_64(wins->arguments[0]);
	char *s1 = mapRegisterAMD_X86_64(wins->arguments[0]);
	char *s2 = mapRegisterAMD_X86_64(wins->arguments[1]);
	if((!strcmp(dest,s1)&&!strcmp(dest,s2))||!strcmp(s1,s2)) {
		if(CPU==AMD_X86_64) snprintf(buf, bsize*sizeof(char), "\t%sq %%%s, %%%s\n", ins, dest, dest);
		else if(CPU==I386) snprintf(buf, bsize*sizeof(char), "\t%sl %%%s, %%%s\n", ins, dest, dest);
	} else {
		if(CPU==AMD_X86_64) {
			snprintf(buf, bsize*sizeof(char), 
					"\tmovq %%%s, %%%s\n\t%sq %%%s, %%%s\n", s2, dest, ins, s1, dest);
		} else if(CPU==I386) {
				snprintf(buf, bsize*sizeof(char), 
					"\tmovl %%%s, %%%s\n\t%sl %%%s, %%%s\n", s2, dest, ins, s1, dest);
		}
	}
	return buf;
}

/*
 * Instruction Conversion
 * This x86_64 version checks for 32-bit and will change registers accordingly.
 *
 * TODO: Use lower-bit registers when not needing 64-bit space.
 * */

char *convertInstructionAMD_X86_64(AsmOut *out, Instruction ins) {
	int args = ins.argLen;
	int outlen = args*DEFMAXFSIZE;
	char outBuf[outlen];
	outBuf[0] = '\0';
	int i;

	for(i=0;i<args;i++) WTRIM(ins.arguments[i]);

	/*Special instructions*/
	/*Inline - Drops direct asm instructions into the output*/
	if(!strcmp(ins.instruction, "inline")) 
		snprintf(outBuf, sizeof(outBuf), "%s", dumpInlineASM(&ins));

	/*
	 * 1 argument instructions
	 * */
	if(args==1) {
		/*
		 * Call: call~ printf
		 * */
		if(!strcmp(ins.instruction, "call")) {
    	    if(ins.argLen>0 && ins.arguments[0]!=NULL)
    	        snprintf(outBuf, sizeof(outBuf), "\tcall %s\n", ins.arguments[0]);
		/*
		 * Return: return~ 0
		 * */
		} else if(!strcmp(ins.instruction, "return")) {
			if(ins.arguments[0]!=NULL) {
				char *reg = NULL;
				if(strlen(ins.arguments[0])==0) ins.arguments[0] = "0";
				else reg = mapRegisterAMD_X86_64(ins.arguments[0]);
				char *dealloc = stackDeallocateAMD_X86_64();
				if(reg!=NULL) {
					snprintf(outBuf, sizeof(outBuf), "\tmovq %%%s, %%rax\n%s\tret\n",
							reg, dealloc);
				} else {
					snprintf(outBuf, sizeof(outBuf), "\tmovl $%s, %%eax\n%s\tret\n",
							ins.arguments[0], dealloc);
				}
				free(dealloc);
			}
		}
	/*
	 * 2 argument instructions
	 * */
	} else if(args==2) {
		/*
		 * Move: move~ r1, r2
		 * */
		if(!strcmp(ins.instruction, "move")) {
			char *val1 = NULL;
			char *val2 = NULL;
			if(checkRegister(ins.arguments[0])) {
				char *AMDReg = mapRegisterAMD_X86_64(ins.arguments[0]);
				val1 = calloc(strlen(AMDReg)+128, sizeof(char));
				strcpy(val1, AMDReg);
				STARTAPPCHAR(val1, '%');
			} else {
				char *var = getCurrentVar(out->parser, &ins, 0);
				val1 = calloc(strlen(var)+1, sizeof(char));
				strcpy(val1, var);
			}
			if(checkRegister(ins.arguments[1])) {
				char *AMDReg = mapRegisterAMD_X86_64(ins.arguments[1]);
				val2 = calloc(strlen(AMDReg)+128, sizeof(char));
				strcpy(val2, AMDReg);
				STARTAPPCHAR(val2, '%');
			} else {
				char *var = getCurrentVar(out->parser, &ins, 1);
				val2 = calloc(strlen(var)+1, sizeof(char));
				strcpy(val2, var);
			}
			char *mov = "movq";
			if(CPU==AMD_X86_64) {
				snprintf(outBuf, sizeof(outBuf),
						"\t%s %s,%s\n", mov, val1, val2);
			} else if(CPU==I386) {
				snprintf(outBuf, sizeof(outBuf),
						"\tmovl %s,%s\n", val1, val2);
			}
			if(val1!=NULL) free(val1);
			if(val2!=NULL) free(val2);
    
		/*
		 * Bitwise Instructions (Logical modifications are made with symbol operators)
		 * */

		/* NOT: not~ r1, r2
		 * r1 = ~r2
		 * */
		} else if(!strcmp(ins.instruction, "not")) {
			if(ins.arguments[0]!=NULL&&ins.arguments[1]!=NULL) {
				char *dest = mapRegisterAMD_X86_64(ins.arguments[0]);
				char *src = mapRegisterAMD_X86_64(ins.arguments[1]);
				if(!strcmp(src,dest)) {
					if(CPU==AMD_X86_64) 
						snprintf(outBuf, sizeof(outBuf), "\tnotq %%%s\n", dest);
					else if(CPU==I386) 
						snprintf(outBuf, sizeof(outBuf), "\tnotl %%%s\n", dest);
				} else {
					if(CPU==AMD_X86_64) {
						snprintf(outBuf, sizeof(outBuf), "\tmovq %%%s, %%%s\n\tnotq %%%s\n",
								src, dest, dest);
					} else if(CPU==I386) {
						snprintf(outBuf, sizeof(outBuf), "\tmovl %%%s, %%%s\n\tnotl %%%s\n",
								src, dest, dest);
					}
				}
			}
		}
	/*
	 * 3 argument instructions
	 * */
	} else if(args==3) {
		/*
		 * Bitwise Instructions (Logical modifications are made with symbol operators)
		 * */


		/* AND: and~ r1, r2, r3
		 * r1 = r2&r3
		 * */
		if(!strcmp(ins.instruction, "and")) {
			if(ins.arguments[0]!=NULL&&ins.arguments[1]!=NULL&&
					ins.arguments[2]!=NULL) {
				char *dest = mapRegisterAMD_X86_64(ins.arguments[0]);
				char *s1 = mapRegisterAMD_X86_64(ins.arguments[1]);
				char *s2 = mapRegisterAMD_X86_64(ins.arguments[2]);
				if((!strcmp(dest,s1)&&!strcmp(dest,s2))||!strcmp(s1,s2)) {
					/*If the are the same it's always 1*/
					snprintf(outBuf, sizeof(outBuf), "\tmovl $1, %%%s\n", dest);
				} else {
					if(CPU==AMD_X86_64) {
						snprintf(outBuf, sizeof(outBuf), "\tmovq %%%s, %%%s\n\tandq %%%s, %%%s\n",
								s1, dest, s2, dest);
					} else if(CPU==I386) {
						snprintf(outBuf, sizeof(outBuf), "\tmovl %%%s, %%%s\n\tandl %%%s, %%%s\n",
								s1, dest, s2, dest);

					}
				} 
			}
		
		/* OR: or~ r1, r2, r3
		 * r1 = r2|r3
		 * */
		} else if(!strcmp(ins.instruction, "or")) {
			if(ins.arguments[0]!=NULL&&ins.arguments[1]!=NULL&&
					ins.arguments[2]!=NULL) {
				char *dest = mapRegisterAMD_X86_64(ins.arguments[0]);
				char *s1 = mapRegisterAMD_X86_64(ins.arguments[1]);
				char *s2 = mapRegisterAMD_X86_64(ins.arguments[2]);
				if((!strcmp(dest,s1)&&!strcmp(dest,s2))||!strcmp(s1,s2)) {
					/*If the are the same it's always 1*/
					if(CPU==AMD_X86_64) snprintf(outBuf, sizeof(outBuf), "\tmovq $1, %%%s\n", dest);
					else if(CPU==I386) snprintf(outBuf, sizeof(outBuf), "\tmovl $1, %%%s\n", dest);
				} else {
					if(CPU==AMD_X86_64) {
						snprintf(outBuf, sizeof(outBuf), "\tmovq %%%s, %%%s\n\torq %%%s, %%%s\n",
								s1, dest, s2, dest);
					} else if(CPU==I386) {
						snprintf(outBuf, sizeof(outBuf), "\tmovl %%%s, %%%s\n\torl %%%s, %%%s\n",
								s1, dest, s2, dest);
					}
				} 
			}
		
		/* NOR: nor~ r1, r2, r3
		 * r1 = !(r2|r3)
		 * */
		} else if(!strcmp(ins.instruction, "nor")) {
			if(ins.arguments[0]!=NULL&&ins.arguments[1]!=NULL&&
					ins.arguments[2]!=NULL) {
				char *dest = mapRegisterAMD_X86_64(ins.arguments[0]);
				char *s1 = mapRegisterAMD_X86_64(ins.arguments[1]);
				char *s2 = mapRegisterAMD_X86_64(ins.arguments[2]);
				if((!strcmp(dest,s1)&&!strcmp(dest,s2))||!strcmp(s1,s2)) {
					/*If the are the same it's always 0*/
					if(CPU==AMD_X86_64) snprintf(outBuf, sizeof(outBuf), "\tmovq $0, %%%s\n", dest);
					else if(CPU==I386) snprintf(outBuf, sizeof(outBuf), "\tmovl $0, %%%s\n", dest);
				} else {
					if(CPU==AMD_X86_64) {
						snprintf(outBuf, sizeof(outBuf), 
								"\tmovq %%%s, %%%s\n\torq %%%s, %%%s\n\tnotq %%%s\n",
								s1, dest, s2, dest, dest);
					} else if(CPU==I386) {
						snprintf(outBuf, sizeof(outBuf), 
								"\tmovl %%%s, %%%s\n\torl %%%s, %%%s\n\tnotl %%%s\n",
								s1, dest, s2, dest, dest);
					}
				} 
			}


		/* NAND: nand~ r1, r2, r3
		 * r1 = ~(r2&r3)
		 * */
		} else if(!strcmp(ins.instruction, "nand")) {
			if(ins.arguments[0]!=NULL&&ins.arguments[1]!=NULL&&
					ins.arguments[2]!=NULL) {
				char *dest = mapRegisterAMD_X86_64(ins.arguments[0]);
				char *s1 = mapRegisterAMD_X86_64(ins.arguments[1]);
				char *s2 = mapRegisterAMD_X86_64(ins.arguments[2]);
				if((!strcmp(dest,s1)&&!strcmp(dest,s2))||!strcmp(s1,s2)) {
					/*If the are the same it's always 0*/
					if(CPU==AMD_X86_64) snprintf(outBuf, sizeof(outBuf), "\tmovq $0, %%%s\n", dest);
					else if(CPU==I386) snprintf(outBuf, sizeof(outBuf), "\tmovl $0, %%%s\n", dest);
				} else {
					if(CPU==AMD_X86_64) {
						snprintf(outBuf, sizeof(outBuf), 
								"\tmovq %%%s, %%%s\n\tandq %%%s, %%%s\n\tnotq %%%s\n",
								s1, dest, s2, dest, dest);
					} else if(CPU==I386) {
						snprintf(outBuf, sizeof(outBuf), 
								"\tmovl %%%s, %%%s\n\tandl %%%s, %%%s\n\tnotq %%%s\n",
								s1, dest, s2, dest, dest);

					}
				} 
			}
	
	
		/* XOR: xor~ r1, r2, r3
		 * r1 = r2^r3
		 * */
		} else if(!strcmp(ins.instruction, "xor")) {
			char *out = triArgInsRDest("xor", &ins);
			snprintf(outBuf, sizeof(outBuf), "%s", out);
			free(out);
		}

		/*
		 * Mathematical Instructions
		 * */
		/* ADD: add~ r1, r2, r3
		 * r1 = r2+r3
		 * */
		else if(!strcmp(ins.instruction, "add")) {
			char *out = triArgInsRDest("add", &ins);
			snprintf(outBuf, sizeof(outBuf), "%s", out);
			free(out);
		}

		/* SUB: sub~ r1, r2, r3
		 * r1 = r2-r3
		 * */
		else if(!strcmp(ins.instruction, "sub")) {
			char *out = triArgInsRDest("sub", &ins);
			snprintf(outBuf, sizeof(outBuf), "%s", out);
			free(out);
		} 

		/* MUL: mul~ r1, r2, r3
		 * r1 = r2*r3
		 * */
		else if(!strcmp(ins.instruction, "mul")) {
			char *out = triArgInsRDest("imul", &ins);
			snprintf(outBuf, sizeof(outBuf), "%s", out);
			free(out);
		}

		/*FOR DIV AND MOD: TODO: right now we take rax withought checking if the user is using it
		 * 						 in the context.*/
		/* DIV: div~ r1, r2, r3
		 * r1 = r2/r3
		 * */
		else if(!strcmp(ins.instruction, "div")) {
			if(ins.arguments[0]!=NULL&&ins.arguments[1]!=NULL&&
				ins.arguments[2]!=NULL) {
				char *dest = mapRegisterAMD_X86_64(ins.arguments[0]);
				char *s1 = mapRegisterAMD_X86_64(ins.arguments[1]);
				char *s2 = mapRegisterAMD_X86_64(ins.arguments[2]);
				if((!strcmp(dest,s1)&&!strcmp(dest,s2))||!strcmp(s1,s2)) {
					/*If the are the same it's always 1*/
					if(CPU==AMD_X86_64) snprintf(outBuf, sizeof(outBuf), "\tmovq $1, %%%s\n", dest);
					else if(CPU==I386) snprintf(outBuf, sizeof(outBuf), "\tmovl $1, %%%s\n", dest);
				} else {
					if(CPU==AMD_X86_64) {
						snprintf(outBuf, sizeof(outBuf), 
								"\tmovq %%%s, %%rax\n\tcqto\n\tidivq %%%s\n\tmovq %%rax, %%%s\n",
								s1, s2, dest);
					} else if(CPU==I386) {
						snprintf(outBuf, sizeof(outBuf), 
								"\tmovl %%%s, %%eax\n\tcltd\n\tidivl %%%s\n\tmovl %%eax, %%%s\n",
								s1, s2, dest);

					}
				} 
			}
		} 

		/* MOD: mod~ r1, r2, r3
		 * r1 = r2%r3
		 * */
		else if(!strcmp(ins.instruction, "mod")) {
			if(ins.arguments[0]!=NULL&&ins.arguments[1]!=NULL&&
				ins.arguments[2]!=NULL) {
				char *dest = mapRegisterAMD_X86_64(ins.arguments[0]);
				char *s1 = mapRegisterAMD_X86_64(ins.arguments[1]);
				char *s2 = mapRegisterAMD_X86_64(ins.arguments[2]);
				if((!strcmp(dest,s1)&&!strcmp(dest,s2))||!strcmp(s1,s2)) {
					/*If the are the same it's always 0*/
					if(CPU==AMD_X86_64) snprintf(outBuf, sizeof(outBuf), "\tmovq $0, %%%s\n", dest);
					else if(CPU==I386) snprintf(outBuf, sizeof(outBuf), "\tmovl $0, %%%s\n", dest);
				} else {
					if(CPU==AMD_X86_64) {
						snprintf(outBuf, sizeof(outBuf), 
								"\tmovq %%%s, %%rax\n\tcqto\n\tidivq %%%s\n\tmovq %%rdx, %%%s\n",
								s1, s2, dest);
					} else if(CPU==I386) {
						snprintf(outBuf, sizeof(outBuf), 
								"\tmovl %%%s, %%eax\n\tcltd\n\tidivl %%%s\n\tmovl %%edx, %%%s\n",
								s1, s2, dest);

					}
				}
			} 
		}
	}

	char *ret = calloc(strlen(outBuf)+1, sizeof(char));
    strcpy(ret, outBuf);
	return ret;
}
