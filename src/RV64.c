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
	wAssign(&ret, "\taddi sp,sp,-48\n\tsd ra,40(sp)\n"
				  "\tsd s0,32(sp)\n\taddi s0,sp,48\n");
	return ret.data;
}

char *stackDeallocateRV64() {
	wString ret = wInitString(1024);
	wAssign(&ret, "\tld ra,40(sp)\n\tld s0,32(sp)\n"
				  "\taddi sp,sp,48\n\tjr ra\n");
	return ret.data;
}

char *RV64GetLVAlloc(Variable *var) {
	wString res;
	char *vName = var->varName;
	char *value = var->value;
	int offset = var->offset+RESERVEDRVOFFSET;
	
	switch(var->type) {
		case INT: res = wCFmt("\tli t0,%s\n\tsw t0,-%d(s0)\n",
								value, offset);
				  break;
		case CHAR: res = wCFmt("\tli t0,%s\n\tsb t0,-%d(s0)\n",
						 		value, offset);
				   break;
		case STRING: res = wCFmt("\tlla t0,wl_str_%s\n\tsd t0,-%d(s0)\n",
						   		vName, offset);
					 break;
		case FLOAT: break;
		case VOID: break;
		case PTR: break;
		case ANY: break;
	};

	return res.data;
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
		var = queryLocalVariable(parser, ins->lineNum, ins->arguments[0]);
		if(var) {
			if(var->type == INT) {
				res = wCFmt("\taddi %s,s0,-%d\n", val2, 
				 	  		var->offset+RESERVEDRVOFFSET);
			} else {
				res = wCFmt("\tld %s,-%d(s0)\n", val2,
							var->offset+RESERVEDRVOFFSET);
			}
		} else {
			if(checkRegister(ins->arguments[0])&&
					!checkRegister(ins->arguments[1])) {
				var = queryLocalVariable(parser, ins->lineNum, 
						ins->arguments[1]);
				if(var) res = wCFmt("\tsw %s,-%d(s0)\n", val1, 
								var->offset+RESERVEDRVOFFSET);
			} else if(checkRegister(ins->arguments[0])&&
					checkRegister(ins->arguments[1])) {
				res = wCFmt("\tmv %s,%s\n", val2, val1);
			} else {
				res = wCFmt("\t\n");
			}
		}
	}
	return res.data;
}

char *getIfStateCmpRV64(char *arg) {
	if(!strcmp(arg, "je")) return "bne";
	else if(!strcmp(arg, "jne")) return "beq";
	else if(!strcmp(arg, "jle")) return "bge";
	else if(!strcmp(arg, "jge")) return "ble";
	return NULL;
}

char *ifStateConvertRV64(AsmOut *out, Instruction *ins) {
	wString ret;
	char *val1 = NULL;
	char *val2 = NULL;
	char *scopeName = ins->arguments[0];
	char *op = getIfStateCmpRV64(ins->instruction);
	if(checkRegister(ins->arguments[1])) {
		val1 = mapRegisterRV64(ins->arguments[1]);
	} else {
		val1 = RV64GetCurrentVar(out->parser, ins, 1);
	}
	if(checkRegister(ins->arguments[2])) {
		val2 = mapRegisterRV64(ins->arguments[2]);
	} else {
		val2 = RV64GetCurrentVar(out->parser, ins, 2);
	}
	if(val1[0]=='-') {
		ret = wCFmt("\tlw t0,%s\n\tli t1,%s\n"
					"\t%s t0,t1,.%s_cont\n\tjal .%s\n.%s_cont:\n",
					val1, val2, op, scopeName, scopeName, scopeName);
	} else if(val2[0]=='-') {
		ret = wCFmt("\tlw t0,%s\n\tli t1,%s\n"
					"\t%s t1,t0,.%s_cont\n\tjal .%s\n.%s_cont:\n",
					val2, val1, op, scopeName, scopeName, scopeName);
	} else {
		ret = wCFmt("\t%s %s,%s,.%s_cont\n\tjal .%s\n.%s_cont:\n",
					op, val1, val2, scopeName, scopeName, scopeName);                                           
	}
	if(val1) {free(val1);val1=NULL;}
	if(val2) {free(val2);val2=NULL;}
	return ret.data;
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

	
	/*
	 * 3 argument instructions
	 * */
	} else if(args==3) {

		/*
		 * Compiler generated instructions, I.E. if statements
		 * */
		if(getIfStateCmpRV64(ins.instruction)!=NULL) {
			wCAppend(&outBuf, ifStateConvertRV64(out, &ins));
		}
	}

	return outBuf.data;
}
