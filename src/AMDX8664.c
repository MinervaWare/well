/*Copyright (c) 2024-2026 MinervaWare LLC*/
/*Copyright (c) 2022-2024 Tristan Wellman*/

#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "AMDX8664.h"

char *stackAllocateAMD_X86_64(enum cpuType cpu) {
	CPU = cpu;
	/*auto to 16*/
	if(CPU==AMD_X86_64) {
		wString ret = wInitString(1024);
		wAssign(&ret, "\tpushq %rbp\n\tmovq %rsp, %rbp\n\tsubq $32, %rsp\n");
		return ret.data;
	} else if(CPU==I386) {
		wString ret = wInitString(1024);
		wAssign(&ret, "\tpushl %ebp\n\tmovl %esp, %ebp\n\tsubl $32, %esp\n");
		return ret.data;
	}
	return NULL;
}

char *stackDeallocateAMD_X86_64() {
	/*auto to 16*/
	if(CPU==AMD_X86_64) {
		wString ret = wInitString(1024);
		wAssign(&ret, "\taddq $32, %rsp\n\tpopq %rbp\n");
		return ret.data;
	} else if(CPU==I386) {
		wString ret = wInitString(1024);
		wAssign(&ret, "\taddl $32, %esp\n\tpopl %ebp\n");
		return ret.data;
	}
	return NULL;
}

void AMD_X86_64GetLVAlloc(char *buf, size_t bSize, Variable *var) {
	char *vName = var->varName;
	char *value = var->value;
	int offset = var->offset;
	if(CPU==AMD_X86_64) {
		switch(var->type) {
			case INT: sprintf(buf,
							  "\tmovq $%s, -%d(%%rbp)\n",
							  value, offset);
					  break;
			case CHAR: sprintf(buf,
							   "\tmovq $%d, -%d(%%rbp)\n",
							   (int)value[0], offset);
					   break;
			case STRING: sprintf(buf,
								 "\tmovq wl_str_%s(%%rip), %%r10\n"
								 "\tmovq %%r10, -%d(%%rbp)\n",
								 vName, offset); 
						break;
			case FLOAT: sprintf(buf,
							  "\tmovss wl_fl_%s(%%rip), %%xmm7\n"
							  "\tmovss %%xmm7, -%d(%%rbp)\n",
							  vName, offset);
						break;
			case VOID: break;
			case PTR: break;
			case ANY: break;
		};
	} else if(CPU==I386) {
		switch(var->type) {
			case INT: sprintf(buf,
							  "\tmovl $%s, -%d(%%ebp)\n",
							  value, offset);
					  break;
			case CHAR: sprintf(buf,
							   "\tmovl $%d, -%d(%%ebp)\n",
							   (int)value[0], offset);
					   break;
			case STRING: sprintf(buf,
								 "\tmovl wl_str_%s, -%d(%%ebp)\n",
								 vName, offset); 
						break;
			case FLOAT: sprintf(buf,
							  "\tflds wl_fl_%s\n\tfstps -%d(%%esp)\n",
							  vName, offset);
						break;
			case VOID: break;
			case PTR: break;
			case ANY: break;
		};
	}
}

char *initLocalVariables(Function *func) {
	char *res = NULL;
	if(func==NULL) return NULL;

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
        	case AMD64SP: return "sp";
        	case XMM0: return "xmm0";
        	case XMM1: return "xmm1";
        	case XMM2: return "xmm2";
        	case XMM3: return "xmm3";
        	case XMM4: return "xmm4";
        	case XMM5: return "xmm5";
        	case XMM6: return "xmm6";
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
        	case AMD64SP: return "sp";
        	case XMM0: return "xmm0";
        	case XMM1: return "xmm1";
        	case XMM2: return "xmm2";
        	case XMM3: return "xmm3";
        	case XMM4: return "xmm4";
        	case XMM5: return "xmm5";
        	case XMM6: return "xmm6";
    	};
	}
    return NULL;
}

char *getCurrentVar(struct parserData *parser, Instruction *ins, int argSpot) {
	wString res;
	Variable *v = NULL;
	if(parser==NULL||ins==NULL) return NULL;
	v = getVarFrom(parser, ins->arguments[argSpot]);
	if(v!=NULL) {
		switch(v->type) {
			case STRING: res = wCFmt("wl_str_%s", ins->arguments[argSpot]);break;
			case CHAR: res = wCFmt("wl_ch_%s", ins->arguments[argSpot]);break;
			case INT: res = wCFmt("wl_int_%s", ins->arguments[argSpot]);break;
			case FLOAT: res = wCFmt("wl_fl_%s", ins->arguments[argSpot]);break;
			case VOID: /*TODO*/break;
			case PTR: res = wCFmt("wl_z_%s", ins->arguments[argSpot]);break;
			case ANY: break;
		};
		if(CPU==AMD_X86_64) wCAppend(&res, "(%rip)");
	} else {
		char *bp = "rbp";
		if(CPU==I386) bp = "ebp";
		v = NULL;
		v = queryLocalVariable(parser, ins->lineNum, ins->arguments[argSpot]);
		if(v!=NULL) res = wCFmt("-%d(%%s)", v->offset, bp);
		else {
			if(!atoi(ins->arguments[argSpot])&&
					strcmp(ins->arguments[argSpot], "0")) {
				res = wCFmt("%s", ins->arguments[argSpot]);
				if(CPU==AMD_X86_64) wCAppend(&res, "(%rip)");
			} else {
				res = wCFmt("$%s", ins->arguments[argSpot]);
			}
		}
	}

	return res.data;
}

/*
 * Tri-Argument Instruction with destination on right.
 * */
char* triArgInsRDest(char *ins, Instruction *wins) {
	int bsize = wins->argLen*1024;
	wString buf;
	char *dest = mapRegisterAMD_X86_64(wins->arguments[0]);
	char *s1 = mapRegisterAMD_X86_64(wins->arguments[0]);
	char *s2 = mapRegisterAMD_X86_64(wins->arguments[1]);
	if((!strcmp(dest,s1)&&!strcmp(dest,s2))||!strcmp(s1,s2)) {
		if(CPU==AMD_X86_64) buf = wCFmt("\t%sq %%s, %%s\n", ins, dest, dest);
		else if(CPU==I386) buf = wCFmt("\t%sl %%s, %%s\n", ins, dest, dest);
	} else {
		if(CPU==AMD_X86_64) {
			buf = wCFmt("\tmovq %%s, %%s\n\t%sq %%s, %%s\n", 
					s2, dest, ins, s1, dest);
		} else if(CPU==I386) {
			buf = wCFmt("\tmovl %%s, %%s\n\t%sl %%s, %%s\n", 
					s2, dest, ins, s1, dest);
		}
	}
	return buf.data;
}

/*This looks redundant on x86_64 because the instructions are pretty much the same for jumps*/
char *getIfStateCmp(char *arg) {
	if(!strcmp(arg, "je")) return "je";
	else if(!strcmp(arg, "jne")) return "jne";
	else if(!strcmp(arg, "jle")) return "jle";
	else if(!strcmp(arg, "jge")) return "jge";
	return NULL;
}

char *ifStateConvert(AsmOut *out, Instruction *ins) {
	wString res;
	char *val1 = NULL;
	char *val2 = NULL;
	char *scopeName = ins->arguments[0];
	char *op = getIfStateCmp(ins->instruction);
	char *cmp = "cmpq";
	if(checkRegister(ins->arguments[1])) {
		char *AMDReg = mapRegisterAMD_X86_64(ins->arguments[1]);
		val1 = calloc(strlen(AMDReg)+128, sizeof(char));
		strcpy(val1, AMDReg);
		STARTAPPCHAR(val1, '%');
	} else {
		char *var = getCurrentVar(out->parser, ins, 1);
		val1 = calloc(strlen(var)+1, sizeof(char));
		strcpy(val1, var);
		free(var); var = NULL;
	}
	if(checkRegister(ins->arguments[2])) {
		char *AMDReg = mapRegisterAMD_X86_64(ins->arguments[2]);
		val2 = calloc(strlen(AMDReg)+128, sizeof(char));
		strcpy(val2, AMDReg);
		STARTAPPCHAR(val2, '%');
	} else {
		char *var = getCurrentVar(out->parser, ins, 2);
		val2 = calloc(strlen(var)+1, sizeof(char));
		strcpy(val2, var);
		free(var); var = NULL;
	}
	if(CPU==I386) cmp = "cmpl";
	res = wCFmt("\t%s %s, %s\n"
				"\t%s .%s\n"
				".%s_cont:\n",
				cmp, val2, val1, op, scopeName, scopeName);
	if(val1) {free(val1);val1=NULL;}
	if(val2) {free(val2);val2=NULL;}

	return res.data;
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
	char *outBuf = calloc(outlen, sizeof(char));
	char *ret;
	int i;
	if(ins.instruction==NULL) return NULL;
	if(!strcmp(ins.instruction, "\n")) return NULL;
	CPU = out->parser->fData->cpu;
	memset(outBuf, 0, outlen*sizeof(char));

	for(i=0;i<args;i++) WTRIM(ins.arguments[i]);

	/*Special instructions*/
	/*Inline - Drops direct asm instructions into the output*/
	if(!strcmp(ins.instruction, "inline")) { 
		sprintf(outBuf, "\t%s\n", dumpInlineASM(&ins));

	/*
	 * 1 argument instructions
	 * */
	} else if(args==1) {
		/*
		 * Call: call~ printf
		 * */
		if(!strcmp(ins.instruction, "call")) {
    	    if(ins.arguments[0]!=NULL)
    	        sprintf(outBuf, "\tcall %s\n", ins.arguments[0]);
		/*
		 * Return: return~ 0
		 * */
		} else if(!strcmp(ins.instruction, "return")) {
			if(ins.arguments[0]!=NULL) {
				char *reg = NULL;
				char *dealloc = stackDeallocateAMD_X86_64();
				if(strlen(ins.arguments[0])==0) ins.arguments[0] = "0";
				else reg = mapRegisterAMD_X86_64(ins.arguments[0]);
				
				if(reg!=NULL) {
					if(CPU==AMD_X86_64) {
						sprintf(outBuf, "\tmovq %%%s, %%rax\n%s\tret\n",
								reg, dealloc); 
					} else {
						sprintf(outBuf, "\tmovl %%%s, %%eax\n%s\tret\n",
								reg, dealloc);
					}
				} else {
					sprintf(outBuf, "\tmovl $%s, %%eax\n%s\tret\n",
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
			char *mov = "movq";
			if(checkRegister(ins.arguments[0])) {
				char *AMDReg = mapRegisterAMD_X86_64(ins.arguments[0]);
				val1 = calloc(strlen(AMDReg)+128, sizeof(char));
				strcpy(val1, AMDReg);
				STARTAPPCHAR(val1, '%');
			} else {
				char *var = getCurrentVar(out->parser, &ins, 0);
				val1 = calloc(strlen(var)+1, sizeof(char));
				strcpy(val1, var);
				free(var); var = NULL;
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
				free(var); var = NULL;
			}
			if(CPU==AMD_X86_64) {
				sprintf(outBuf,
						"\t%s %s,%s\n", mov, val1, val2);
			} else if(CPU==I386) {
				sprintf(outBuf,
						"\tmovl %s,%s\n", val1, val2);
			}
			if(val1!=NULL) {free(val1);val1=NULL;}
			if(val2!=NULL) {free(val2);val2=NULL;}
    
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
						sprintf(outBuf, "\tnotq %%%s\n", dest);
					else if(CPU==I386) 
						sprintf(outBuf, "\tnotl %%%s\n", dest);
				} else {
					if(CPU==AMD_X86_64) {
						sprintf(outBuf, "\tmovq %%%s, %%%s\n\tnotq %%%s\n",
								src, dest, dest);
					} else if(CPU==I386) {
						sprintf(outBuf, "\tmovl %%%s, %%%s\n\tnotl %%%s\n",
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
		 * Compiler statement related instructions, I.E. If state or loops.
		 * These statements are generated by the parser.
		 *
		 * Users can also use these if they decide to opt out of using ~if or ~loop.
		 * This would look like:
		 * jne~ function, arg1, arg2
		 * */
		if(getIfStateCmp(ins.instruction)!=NULL) {
			char *conv = ifStateConvert(out, &ins);
			sprintf(outBuf, "%s", conv);
			free(conv); conv = NULL;

		/*
		 * Bitwise Instructions (Logical modifications are made with symbol operators)
		 * */


		/* AND: and~ r1, r2, r3
		 * r1 = r2&r3
		 * */
		} else if(!strcmp(ins.instruction, "and")) {
			if(ins.arguments[0]!=NULL&&ins.arguments[1]!=NULL&&
					ins.arguments[2]!=NULL) {
				char *dest = mapRegisterAMD_X86_64(ins.arguments[0]);
				char *s1 = mapRegisterAMD_X86_64(ins.arguments[1]);
				char *s2 = mapRegisterAMD_X86_64(ins.arguments[2]);
				if((!strcmp(dest,s1)&&!strcmp(dest,s2))||!strcmp(s1,s2)) {
					/*If the are the same it's always 1*/
					sprintf(outBuf, "\tmovl $1, %%%s\n", dest);
				} else {
					if(CPU==AMD_X86_64) {
						sprintf(outBuf, "\tmovq %%%s, %%%s\n\tandq %%%s, %%%s\n",
								s1, dest, s2, dest);
					} else if(CPU==I386) {
						sprintf(outBuf, "\tmovl %%%s, %%%s\n\tandl %%%s, %%%s\n",
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
					if(CPU==AMD_X86_64) sprintf(outBuf, "\tmovq $1, %%%s\n", dest);
					else if(CPU==I386) sprintf(outBuf, "\tmovl $1, %%%s\n", dest);
				} else {
					if(CPU==AMD_X86_64) {
						sprintf(outBuf, "\tmovq %%%s, %%%s\n\torq %%%s, %%%s\n",
								s1, dest, s2, dest);
					} else if(CPU==I386) {
						sprintf(outBuf, "\tmovl %%%s, %%%s\n\torl %%%s, %%%s\n",
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
					if(CPU==AMD_X86_64) sprintf(outBuf, "\tmovq $0, %%%s\n", dest);
					else if(CPU==I386) sprintf(outBuf, "\tmovl $0, %%%s\n", dest);
				} else {
					if(CPU==AMD_X86_64) {
						sprintf(outBuf, 
								"\tmovq %%%s, %%%s\n\torq %%%s, %%%s\n\tnotq %%%s\n",
								s1, dest, s2, dest, dest);
					} else if(CPU==I386) {
						sprintf(outBuf, 
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
					if(CPU==AMD_X86_64) sprintf(outBuf, "\tmovq $0, %%%s\n", dest);
					else if(CPU==I386) sprintf(outBuf, "\tmovl $0, %%%s\n", dest);
				} else {
					if(CPU==AMD_X86_64) {
						sprintf(outBuf, 
								"\tmovq %%%s, %%%s\n\tandq %%%s, %%%s\n\tnotq %%%s\n",
								s1, dest, s2, dest, dest);
					} else if(CPU==I386) {
						sprintf(outBuf, 
								"\tmovl %%%s, %%%s\n\tandl %%%s, %%%s\n\tnotl %%%s\n",
								s1, dest, s2, dest, dest);

					}
				} 
			}
	
	
		/* XOR: xor~ r1, r2, r3
		 * r1 = r2^r3
		 * */
		} else if(!strcmp(ins.instruction, "xor")) {
			char *out = triArgInsRDest("xor", &ins);
			sprintf(outBuf, "%s", out);
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
			sprintf(outBuf, "%s", out);
			free(out);
		}

		/* SUB: sub~ r1, r2, r3
		 * r1 = r2-r3
		 * */
		else if(!strcmp(ins.instruction, "sub")) {
			char *out = triArgInsRDest("sub", &ins);
			sprintf(outBuf, "%s", out);
			free(out);
		} 

		/* MUL: mul~ r1, r2, r3
		 * r1 = r2*r3
		 * */
		else if(!strcmp(ins.instruction, "mul")) {
			char *out = triArgInsRDest("imul", &ins);
			sprintf(outBuf, "%s", out);
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
					if(CPU==AMD_X86_64) sprintf(outBuf, "\tmovq $1, %%%s\n", dest);
					else if(CPU==I386) sprintf(outBuf, "\tmovl $1, %%%s\n", dest);
				} else {
					if(CPU==AMD_X86_64) {
						sprintf(outBuf, 
								"\tmovq %%%s, %%rax\n\tcqto\n\tidivq %%%s\n\tmovq %%rax, %%%s\n",
								s1, s2, dest);
					} else if(CPU==I386) {
						sprintf(outBuf, 
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
					if(CPU==AMD_X86_64) sprintf(outBuf, "\tmovq $0, %%%s\n", dest);
					else if(CPU==I386) sprintf(outBuf, "\tmovl $0, %%%s\n", dest);
				} else {
					if(CPU==AMD_X86_64) {
						sprintf(outBuf, 
								"\tmovq %%%s, %%rax\n\tcqto\n\tidivq %%%s\n\tmovq %%rdx, %%%s\n",
								s1, s2, dest);
					} else if(CPU==I386) {
						sprintf(outBuf, 
								"\tmovl %%%s, %%eax\n\tcltd\n\tidivl %%%s\n\tmovl %%edx, %%%s\n",
								s1, s2, dest);

					}
				}
			} 
		}
	}

	ret = calloc(strlen(outBuf)+1, sizeof(char));
	strcpy(ret, outBuf);
	free(outBuf);
	return ret;
}
