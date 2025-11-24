/*Copyright (c) 2022-2025 MinervaWare LLC*/

#include "ALPHA.h"

char *stackAllocateALPHA() {
	char *ret = calloc(1024, sizeof(char));
	sprintf(ret, "\tldgp $29, 0($27)\n"
				 "\tlda $30, -16($30)\n"
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
	}
	res = calloc(strlen(asmVName)+1, sizeof(char));
	strcpy(res, asmVName);
	return res;
}

char *convertInstructionALPHA(AsmOut *out, Instruction ins) {
	int args = ins.argLen;
	int outlen = args*DEFMAXFSIZE;
	char outBuf[outlen];
	outBuf[0] = '\0';
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
							"\tldgp $29, 0($26)\n\tbis $31, %s, %1\n\tmov $1, $0\n%s",
							reg, dealloc);
				} else {
					snprintf(outBuf, sizeof(outBuf),
							"\tldgp $29, 0($26)\n\tbis $31, %s, $1\n\tmov $1, $0\n%s",
							ins.arguments[0], dealloc);
				}
				free(dealloc);
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
			}
			if(checkRegister(ins.arguments[1])) {
				char *ALPHAReg = mapRegisterALPHA(ins.arguments[1]);
				val2 = calloc(strlen(ALPHAReg)+128, sizeof(char));
				strcpy(val2, ALPHAReg);
			} else {
				char *var = getCurrentVarALPHA(out->parser, &ins, 1);
				val2 = calloc(strlen(var)+1, sizeof(char));
				strcpy(val2, var);
			}
			snprintf(outBuf, sizeof(outBuf),
					"\tlda %s, %s\n", val2, val1);
			if(val1!=NULL) free(val1);
			if(val2!=NULL) free(val2);
		}	
	}

	char *ret = calloc(strlen(outBuf)+1, sizeof(char));
    strcpy(ret, outBuf);
	return ret;
}


