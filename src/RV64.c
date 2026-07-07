/*Copyright (c) 2026 MinervaWare LLC*/

/*
 * All testing was done with a SiFive JH7110
 * */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "RV64.h"

#define RESERVEDRVOFFSET 20


char *stackAllocateRV64(enum cpuType cpu) {
	wString ret = wInitString(1024);
	CPU = cpu;
	wAssign(&ret, "\taddi sp,sp,-32\n\tsd ra,24(sp)\n"
				  "\tsd s0,16(sp)\n\taddi s0,sp,32\n");
	return ret.data;
}

char *stackDeallocateRV64() {
	wString ret = wInitString(1024);
	wAssign(&ret, "\tld ra,24(sp)\n\tld s0,16(sp)\n"
				  "\taddi sp,sp,32\n\tjr ra\n");
	return ret.data;
}

char *RV64GetLVAlloc(char *buf, size_t bSize, Variable *var) {
	return NULL;
}

char *mapRegisterRV64(char *reg) {
	int regNum = regToEnum(reg);
	switch(regNum) {
		case RVa0: return "a0";
		case RVa1: return "a1";
		case RVa2: return "a2";
		case RVa3: return "a3";
		case RVa4: return "a4";
		case RVa5: return "a5";
		case RVa6: return "a6";
		case RVa7: return "a7";
		case RV64SP: return "sp";
		case RVfa0: return "fa0";
		case RVfa1: return "fa1";
		case RVfa2: return "fa2";
		case RVfa3: return "fa3";
		case RVfa4: return "fa4";
		case RVfa5: return "fa5";
		case RVfa6: return "fa6";
	};
	return "a0";
}

char *RV64GetCurrentVar(struct parserData *parser, Instruction *ins, 
		int argSpot) {
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
			case VOID: break;
			case PTR: res = wCFmt("wl_z_%s", ins->arguments[argSpot]);break;
			case ANY: break;
		};
	} else {
		v = NULL;
		v = queryLocalVariable(parser, ins->lineNum, ins->arguments[argSpot]);
		if(v!=NULL) res = wCFmt("-%d(s0)", v->offset+RESERVEDRVOFFSET);
		else {
			if(!atoi(ins->arguments[argSpot])&&
					strcmp(ins->arguments[argSpot], "0")) {
				res = wCFmt("%s", ins->arguments[argSpot]);
			} else res = wCFmt("%s", ins->arguments[argSpot]);
		}
	}
	return res.data;
}

char *RV64GetMoveInstruction(struct parserData *parser, Instruction *ins,
		char *val1, char *val2) {
	wString res;
	Variable *var = getVarFrom(parser, ins->arguments[0]);
	if(var) {
		if(var->varName!=NULL) {
			switch(var->type) {
				case INT: res = wCFmt("\tlla %s,%s\n\tlw %s,0(%s)\n", 
										val2, val1, val2, val2); 
						  break;
				case CHAR: res = wCFmt("\tlla %s,%s\n\tlbu %s,0(%s)",
								 		val2, val1, val2, val2); 
						   break;
				case PTR: res = wCFmt("\tlla %s,%s\n\tld %s,0(%s)\n",
										val2, val1, val2, val2);
						  break;
				case STRING:  res = wCFmt("\tlla %s,%s\n", val2, val1);
							  break;
				case FLOAT: res = wCFmt("\tlla %s,%s\n\tflw %s,0(%s)\n",
								  		val2, val1, val2, val2);
							break;
				default: res = wCFmt("\tlla %s,%s\n", val2, val1); break;
			};
		}	
	} else {
		
	}
	return res.data;
}

char *convertInstructionRV64(AsmOut *out, Instruction ins) {
	int args = ins.argLen;
	int outLen = args*DEFMAXFSIZE;
	wString outBuf = wInitString(outLen);
	int i;

	CPU = out->parser->fData->cpu;

	if(!strcmp(ins.instruction, "inline")) {
		wCAppend(&outBuf, wCFmt("\t%s\n", dumpInlineASM(&ins)).data);
	
	/*
	 * 0 argument instructions
	 * */
	} else if(args==0) {

	/*
	 * 1 argument instructions
	 * */
	} else if(args==1) {

		/*
		 * Call
		 * */
		if(!strcmp(ins.instruction, "call")) {
			if(ins.argLen>0&&ins.arguments[0]!=NULL) {
				if(doesFunctionExistInternal(out->parser, ins.arguments[0])) {
					wCAppend(&outBuf, wCFmt("\tcall %s\n\tmv a6,a0\n", 
							ins.arguments[0]).data);
				} else {
					wCAppend(&outBuf, wCFmt("\tcall %s@plt\n\tmv a6,a0\n", 
							ins.arguments[0]).data);
				}
			}	
		

		/*
		 * Return
		 * */
		} else if(!strcmp(ins.instruction, "return")) {
			if(ins.arguments[0]!=NULL) {
				char *reg = NULL;
				char *dealloc = stackDeallocateRV64(ins.funName);
				if(strlen(ins.arguments[0])==0) reg="0";
				else reg = mapRegisterRV64(ins.arguments[0]);
				wCAppend(&outBuf, wCFmt("\tmv a0,%s\n%s", reg, dealloc).data);
			}
		}

	
	/*
	 * 2 argument instructions
	 * */
	} else if(args==2) {
		
		/*
		 * Move
		 * */
		if(!strcmp(ins.instruction, "move")) {
			char *val1;
			char *val2;
			char *mov;
			if(checkRegister(ins.arguments[0])) {
				val1 = mapRegisterRV64(ins.arguments[0]);
			} else {
				val1 = RV64GetCurrentVar(out->parser, &ins, 0);
			}
			if(checkRegister(ins.arguments[1])) {
				val2 = mapRegisterRV64(ins.arguments[1]);
			} else {
				val2 = RV64GetCurrentVar(out->parser, &ins, 1);
			}
			mov = RV64GetMoveInstruction(out->parser, &ins, val1, val2);
			if(mov==NULL) mov = "";
			wCAppend(&outBuf, mov);
		}

	}

	return outBuf.data;
}
