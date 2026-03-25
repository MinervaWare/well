/*Copyright (c) 2024-2026 MinervaWare LLC*/
/*Copyright (c) 2022-2024 Tristan Wellman*/

/*PPC will handle PPC & PPC64*/

/* 1/29/2026
 * All testing and development is done on a PS3 Cell CPU (Power4).
 * At some point I'd also like to add SPU support.
 * */

/*
 * https://zenith.nsmbu.net/wiki/Custom_Code/PowerPC_Assembly_Cheatsheet
 * */

#include "PPC.h"

char *stackAllocatePPC(enum cpuType cpu) {
	char *ret;
	CPU = cpu;
	ret = calloc(1024, sizeof(char));
	snprintf(ret, 1024*sizeof(char),
			"\tmflr 0\n\tstd 0,16(1)\n"
			"\tstd 31,-8(1)\n\tstdu 1,-128(1)\n"
			"\tmr 31,1\n");
	return ret;
}                                                                           

char *stackDeallocatePPC() {
	char *ret = calloc(1024, sizeof(char));
	snprintf(ret, 1024*sizeof(char),
			"\taddi 1,31,128\n\tld 0,16(1)\n"
			"\tmtlr 0\n\tld 31,-8(1)\n\tblr\n");
	return ret;
}                                                                                        

void PPCGetLVTAlloc(char *buf, size_t bSize, Variable *var) {
	char *vName = var->varName;
	char *value = var->value;
	int offset = var->offset;
	switch(var->type) {
		case CHAR:
		case INT: snprintf(buf, bSize, 
					  "\tli 11,%s\n\tstd 11,%d(31)\n",
					  value, offset+PPC64RESERVEDGPOFFSET);
			  break;
		case STRING: snprintf(buf, bSize,
					     "\taddis 11,2,wl_str_%s@toc@ha\n"
					     "\taddi 11,11,wl_str_%s@toc@l\n"
					     "\tstd 11,%d(31)\n",
					     vName, vName, offset+PPC64RESERVEDGPOFFSET);
			     break;
		case FLOAT: break;
		case VOID: break;
		case ZERO: break;
	};
}

char *mapRegisterPPC(char *reg) {
	int regNum = regToEnum(reg);
	switch(regNum) {
		case r4: return "4";
		case r5: return "5";
		case r6: return "6";
		case r7: return "7";
		case r8: return "8";
		case r9: return "9";
		case r10: return "10";
		case r3: return "3";
		case r1: return "1";
	};
	return NULL;
}

char *getCurrentVarPPC(struct parserData *parser, Instruction *ins, int argSpot) {
	char asmVName[1024];
	char *res = NULL;
	Variable *v = NULL;
	if(parser==NULL||ins==NULL) return NULL;
	v = getVarFrom(parser, ins->arguments[argSpot]);
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
		v = NULL;
		v = queryLocalVariable(parser, ins->lineNum, ins->arguments[argSpot]);
		/*local var or immediate val*/
		if(v!=NULL) snprintf(asmVName, sizeof(asmVName), "%d(31)", v->offset+PPC64RESERVEDGPOFFSET);
		else snprintf(asmVName, sizeof(asmVName), "%s", ins->arguments[argSpot]);
	}
	res = calloc(strlen(asmVName)+1, sizeof(char));
	strcpy(res, asmVName);
	return res;
}

char *PPCGetMoveInstructions(struct parserData *parser, Instruction *ins, 
	char *val1, char *val2) {
	char *res = NULL;
	char outBuf[1024];
	Variable *var = getVarFrom(parser, ins->arguments[0]);
	if(var) {
		if(var->varName!=NULL) {
			switch(var->type) {
				case INT: snprintf(outBuf, sizeof(outBuf), 
					  	"\taddis %s,2,%s@toc@ha\n\taddi %s,%s,%s@toc@l\n",
					  	val2, val1, val2, val2, val1);
					  break;
				case CHAR: snprintf(outBuf, sizeof(outBuf),
					   	"\taddis %s,2,%s@toc@ha\n\taddi %s,%s,%s@toc@l\n",
						val2, val1, val2, val2, val1);
					   break;
				case STRING: snprintf(outBuf, sizeof(outBuf),
					     	"\taddis %s,2,%s@toc@ha\n\taddi %s,%s,%s@toc@l\n",
						val2, val1, val2, val2, val1);
				case FLOAT: break;
				default: snprintf(outBuf, sizeof(outBuf),
						"\taddis %s,2,%s@toc@ha\n\taddi %s,%s,%s@toc@l\n",
						val2, val1, val2, val2, val1);
					 break;
			};
		}
	} else {
		var = queryLocalVariable(parser, ins->lineNum, ins->arguments[0]);
		if(var!=NULL) {
			snprintf(outBuf, sizeof(outBuf),
					"\tld %s,%d(31)\n", 
					val2, var->offset+PPC64RESERVEDGPOFFSET);
		} else {
			if(checkRegister(ins->arguments[0])&&
					!checkRegister(ins->arguments[1])) {
				var = queryLocalVariable(parser, ins->lineNum, ins->arguments[1]);
				if(var!=NULL) {
					snprintf(outBuf, sizeof(outBuf), 
							"\tld %s,%d(31)\n", 
							val1, var->offset+PPC64RESERVEDGPOFFSET);
				}
			} else if(checkRegister(ins->arguments[0])&&
					checkRegister(ins->arguments[1])) {
				snprintf(outBuf, sizeof(outBuf),
						"\tmr %s,%s\n", val2, val1);
			} else {
				snprintf(outBuf, sizeof(outBuf),
						"\tli %s,%s\n", val2, val1);
			}
		}
	}
	res = calloc(strlen(outBuf)+1, sizeof(char));
	strcpy(res, outBuf);
	return res;
}

char *getIfStateCmpPPC(char *arg) {
	if(!strcmp(arg, "je")) return "beq";
	else if(!strcmp(arg, "jne")) return "bne";
	else if(!strcmp(arg, "jle")) return "blt";
	else if(!strcmp(arg, "jge")) return "bgt";
	return NULL;
}

char *ifStateConvertPPC(AsmOut *out, Instruction *ins) {
	char *ret = NULL;
	char outBuf[DEFMAXFSIZE];
	char *val1 = NULL;
	char *val2 = NULL;
	Variable *local0 = NULL;
	Variable *local1 = NULL;
	char *scopeName = ins->arguments[0];
	char *op = getIfStateCmpPPC(ins->instruction);
	char *cmp = "cmpwi"; /*update this for immediate / logic immediates*/
	if(checkRegister(ins->arguments[1])) {
		char *PPCReg = mapRegisterPPC(ins->arguments[1]);
		val1 = calloc(strlen(PPCReg)+1, sizeof(char));
		strcpy(val1, PPCReg);
	} else {
		char *var = getCurrentVarPPC(out->parser, ins, 1);
		val1 = calloc(strlen(var)+1, sizeof(char));
		strcpy(val1, var);
	}
	if(checkRegister(ins->arguments[2])) {
		char *PPCReg = mapRegisterPPC(ins->arguments[1]);
		val2 = calloc(strlen(PPCReg)+1, sizeof(char));
		strcpy(val2, PPCReg);
	} else {
		char *var = getCurrentVarPPC(out->parser, ins, 2);
		val2 = calloc(strlen(var)+1, sizeof(char));
		strcpy(val2, var);
	}
	local0 = queryLocalVariable(out->parser, ins->lineNum, ins->arguments[1]);
	local1 = queryLocalVariable(out->parser, ins->lineNum, ins->arguments[2]);
	if(local0!=NULL&&local1==NULL) {
		snprintf(outBuf, sizeof(outBuf),
				"\tld 11,%d(31)\n\t%s 0,%s,%s\n"
				"\t%s 0,.%s\n"
				".%s_cont:\n",
				local0->offset+PPC64RESERVEDGPOFFSET,
				cmp, val2, "11", op, scopeName, scopeName);
	} else if(local1!=NULL&&local0==NULL) {
		snprintf(outBuf, sizeof(outBuf),
				"\tld 11,%d(31)\n\t%s 0,%s,%s\n"
				"\t%s 0,.%s\n"
				".%s_cont:\n",
				local1->offset+PPC64RESERVEDGPOFFSET,
				cmp, "11", val1, op, scopeName, scopeName);
	} else {
		snprintf(outBuf, sizeof(outBuf), 
				"\t%s 0,%s,%s\n"
				"\t%s 0,.%s\n"
				".%s_cont:\n",
				cmp, val2, val1, op, scopeName, scopeName);
	}
	if(val1!=NULL) {free(val1);val1=NULL;}
	if(val2!=NULL) {free(val2);val2=NULL;}
	ret = calloc(strlen(outBuf)+1, sizeof(char));
	strcpy(ret, outBuf);
	return ret;
}

char *convertInstructionPPC(AsmOut *out, Instruction ins) {
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
		snprintf(outBuf, sizeof(char)*outlen, "\t%s\n", dumpInlineASM(&ins));

	/*
	 * 1 argument instructions
	 * */
	} else if(args==1) {
		/*
		 * Call: call~ printf
		 * */
		if(!strcmp(ins.instruction, "call")) {
			if(ins.argLen>0 && ins.arguments[0]!=NULL) {
				/*We have to move r3 to r9 for wl standard on external functions.*/
				if(!doesFunctionExistInternal(out->parser, ins.arguments[0])) {
					snprintf(outBuf, sizeof(char)*outlen,
							"\tbl %s\n\tnop\n\tmr 9,3\n", ins.arguments[0]);
				} else {
					snprintf(outBuf, sizeof(char)*outlen, "\tbl %s\n\tnop\n",
							ins.arguments[0]);
				}
			}
		/*
		 * Return: return~ 0
		 * */
		} else if(!strcmp(ins.instruction, "return")) {
			if(ins.arguments[0]!=NULL) {
				char *reg = NULL;
				char *dealloc = stackDeallocatePPC();
				if(strlen(ins.arguments[0])==0) ins.arguments[0] = "0";
				else reg = mapRegisterPPC(ins.arguments[0]);
				if(checkRegister(ins.arguments[0])) {
					snprintf(outBuf, sizeof(char)*outlen,
							"\tmr 3,%s\n%s", reg, dealloc);
				} else {
					snprintf(outBuf, sizeof(char)*outlen,
							"\tli 3,%s\n%s", ins.arguments[0], dealloc);
				}
				free(dealloc);
				dealloc = NULL;
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
			char *mov = NULL;
			if(checkRegister(ins.arguments[0])) {
				char *PPCReg = mapRegisterPPC(ins.arguments[0]);
				val1 = calloc(strlen(PPCReg)+1, sizeof(char));
				strcpy(val1, PPCReg);
			} else {
				char *var = getCurrentVarPPC(out->parser, &ins, 0);
				val1 = calloc(strlen(var)+1, sizeof(char));
				strcpy(val1, var);
			}
			if(checkRegister(ins.arguments[1])) {
				char *PPCReg = mapRegisterPPC(ins.arguments[1]);
				val2 = calloc(strlen(PPCReg)+1, sizeof(char));
				strcpy(val2, PPCReg);
			} else {
				char *var = getCurrentVarPPC(out->parser, &ins, 1);
				val2 = calloc(strlen(var)+1, sizeof(char));
				strcpy(val2, var);
			}
			mov = PPCGetMoveInstructions(out->parser, &ins, val1, val2);
			if(mov==NULL) mov = "";
			strcpy(outBuf, mov);
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
		if(getIfStateCmpPPC(ins.instruction)!=NULL) {
			char *conv = ifStateConvertPPC(out, &ins);
			snprintf(outBuf, sizeof(char)*outlen, "%s", conv);
			free(conv); conv = NULL;
		}
	}

	ret = calloc(strlen(outBuf)+1, sizeof(char));
	strcpy(ret, outBuf);
	free(outBuf);
	return ret;
} 
