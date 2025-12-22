/*Copyright (c) 2022-2025 MinervaWare LLC*/

#include "ALPHA.h"

char *stackAllocateALPHA(enum cpuType cpu) {
	CPU = cpu;
	char *ret = calloc(1024, sizeof(char));
	sprintf(ret, "\tlda $30, -16($30)\n"
				 "\tstq $26, 0($30)\n"
				 "\tstq $15, 8($30)\n"
				 "\tbis $31, $30, $15\n");
	return ret;
}

char *stackDeallocateALPHA() {
	char *ret = calloc(1024, sizeof(char));
	sprintf(ret, "\tmov $15, $30\n"
				 "\tldq $26, 0($30)\n"
				 "\tldq $15, 8($30)\n"
				 "\tlda $30, 16($30)\n"
				 "\tret $31, ($26), 1\n");
	return ret;
}

void ALPHAGetLVTAlloc(char *buf, size_t bSize, Variable *var) {
	char *vName = var->varName;
	char *value = var->value;
	int offset = var->offset;
	switch(var->type) {
		case INT: snprintf(buf, bSize,
						  "\tlda $22, %s($31)\n"
						  "\tstq $22, %d($23)\n",
						  value, offset);
				  break;
		case CHAR: snprintf(buf, bSize,
						   "\tlda $22, %d($31)\n"
						   "\tstq $22, %d($23)\n",
						   (int)value[0], offset);
				   break;
		case STRING: snprintf(buf, bSize,
							 "\tlda $22, wl_str_%s\n"
							 "\tstq $22, %d($23)\n",
							 vName, offset);
					 break;
		case FLOAT: break;
		case VOID: break;
		case ZERO: break;
	};
}

char *mapRegisterALPHA(char *reg) {
	int regNum = regToEnum(reg);
	switch(regNum) {
		case a0: return "$16";
		case a1: return "$17";
		case a2: return "$18";
		case a3: return "$19";
		case a4: return "$20";
		case a5: return "$21";
		case v0: return "$0";
		case t0: return "$1";
		case sp: return "$30";
	};
	return NULL;
}

char *getCurrentVarALPHA(struct parserData *parser, Instruction *ins, int argSpot) {
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
	} else {
		v = queryLocalVariable(parser, ins->lineNum, ins->arguments[argSpot]);
		if(v!=NULL) snprintf(asmVName, sizeof(asmVName), "%d($23)", v->offset);
		else snprintf(asmVName, sizeof(asmVName), "%s", v->value);
	}
	res = calloc(strlen(asmVName)+1, sizeof(char));
	strcpy(res, asmVName);
	return res;
}

/*
 * Tri-Argument Instruction with destination on right.
 * */
char *triArgInsRDestALPHA(char *ins, Instruction *wins) {
	int bsize = wins->argLen*128;
	char *buf = calloc(bsize, sizeof(char));
	char *dest = mapRegisterALPHA(wins->arguments[0]);
	char *s2 = mapRegisterALPHA(wins->arguments[1]);
	snprintf(buf, bsize*sizeof(char), "\t%s %s, %s, %s\n", ins, dest, s2, dest);
	return buf;
}

char *convertInstructionALPHA(AsmOut *out, Instruction ins) {
	if(ins.instruction==NULL) return NULL;
	if(!strcmp(ins.instruction, "\n")) return NULL;
	CPU = out->parser->fData->cpu;
	int args = ins.argLen;
	int outlen = args*DEFMAXFSIZE;
	char outBuf[outlen];
	memset(outBuf, 0, outlen*sizeof(char));
	int i;

	for(i=0;i<args;i++) WTRIM(ins.arguments[i]);
	
	/*Special instructions*/
	/*Inline - Drops direct asm instructions into the output*/
	if(!strcmp(ins.instruction, "inline")) { 
		snprintf(outBuf, sizeof(outBuf), "\t%s\n", dumpInlineASM(&ins));

	/*
	 * 1 argument instructions
	 */
	} else if(args==1) {
		/*
		 * Call: call~ printf
		 * */
		if(!strcmp(ins.instruction, "call")) {
    	    if(ins.argLen>0 && ins.arguments[0]!=NULL)
    	        snprintf(outBuf, sizeof(outBuf), "\tjsr $26, %s\n", ins.arguments[0]);
		/*
		 * Return: return~ 0
		 */
		} else if(!strcmp(ins.instruction, "return")) {
			if(ins.arguments[0]!=NULL) {
				char *reg = NULL;
				if(strlen(ins.arguments[0])==0) ins.arguments[0] = "0";
				else reg = mapRegisterALPHA(ins.arguments[0]);
				char *dealloc = stackDeallocateALPHA();
				if(checkRegister(ins.arguments[0])) {
					snprintf(outBuf, sizeof(outBuf), 
							"\tldgp $29, 0($26)\n\tbis $31, %s, $1\n\tmov $1, $0\n%s",
							reg, dealloc);
				} else {
					snprintf(outBuf, sizeof(outBuf),
							"\tldgp $29, 0($26)\n\tbis $31, %s, $1\n\tmov $1, $0\n%s",
							ins.arguments[0], dealloc);
				}
				free(dealloc);
				dealloc = NULL;
			}
		}
	} else if(args==2) {
		/*
		 * Move: move~ r1, r2
		 * */
		if(!strcmp(ins.instruction, "move")) {
			char *val1 = NULL;
			char *val2 = NULL;
			if(checkRegister(ins.arguments[0])) {
				char *ALPHAReg = mapRegisterALPHA(ins.arguments[0]);
				val1 = calloc(strlen(ALPHAReg)+128, sizeof(char));
				strcpy(val1, ALPHAReg);
			} else {
				char *var = getCurrentVarALPHA(out->parser, &ins, 0);
				val1 = calloc(strlen(var)+1, sizeof(char));
				strcpy(val1, var);
				free(var); var = NULL;
			}
			if(checkRegister(ins.arguments[1])) {
				char *ALPHAReg = mapRegisterALPHA(ins.arguments[1]);
				val2 = calloc(strlen(ALPHAReg)+128, sizeof(char));
				strcpy(val2, ALPHAReg);
			} else {
				char *var = getCurrentVarALPHA(out->parser, &ins, 1);
				val2 = calloc(strlen(var)+1, sizeof(char));
				strcpy(val2, var);
				free(var); var = NULL;
			}
			if(val1[strlen(val1)-1]==')') {
				snprintf(outBuf, sizeof(outBuf),
						"\tldq $22, %s\n"
						"\tmov $22, %s\n", val1, val2);
			} else if(val2[strlen(val2)-1]==')') {
				snprintf(outBuf, sizeof(outBuf),
						"\tmov $22, %s\n"
						"\tstq $22, %s\n", val1, val2);	
			} else if(val1[0]=='$'&&val2[0]=='$') {
				snprintf(outBuf, sizeof(outBuf),
						"\tmov %s, %s\n", val2, val1);
			} else {
				snprintf(outBuf, sizeof(outBuf),
						"\tlda %s, %s\n", val2, val1);
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
				char *dest = mapRegisterALPHA(ins.arguments[0]);
				char *src = mapRegisterALPHA(ins.arguments[1]);
				snprintf(outBuf, sizeof(outBuf), "\tnot %s, %s\n", dest, src);
			}
		}	
	
	/*
	 * 3 argument instructions
	 */
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
				char *dest = mapRegisterALPHA(ins.arguments[0]);
				char *s1 = mapRegisterALPHA(ins.arguments[1]);
				char *s2 = mapRegisterALPHA(ins.arguments[2]);
				if((!strcmp(dest,s1)&&!strcmp(dest,s2))||!strcmp(s1,s2)) {
					/*If the are the same it's always 1*/
					/*TODO: I do not believe there is a bitwise or instruction
					 * for ALPHA: https://www.cs.cmu.edu/afs/cs/academic/class/15213-f98/doc/alpha-guide.pdf*/
					snprintf(outBuf, sizeof(outBuf), "\tor $31, $1, %s\n", dest);
				} else {
					snprintf(outBuf, sizeof(outBuf), "\tand %s, %s, %s\n",
							s1, s2, dest);
				} 
			}

		/*
		 * Mathematical Instructions
		 * */
		/* ADD: add~ r1, r2, r3
		 * r1 = r2+r3
		 * */
		} else if(!strcmp(ins.instruction, "add")) {
			char *out = triArgInsRDestALPHA("addq", &ins);
			snprintf(outBuf, sizeof(outBuf), "%s", out);
			free(out);
			out = NULL;
		}
	}
	char *ret = calloc(strlen(outBuf)+1, sizeof(char));
    strcpy(ret, outBuf);
	return ret;
}


