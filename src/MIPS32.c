/*Copyright (c) 2024-2026 MinervaWare LLC*/
/*Copyright (c) 2022-2024 Tristan Wellman*/

#include "MIPS32.h"

char *stackAllocateMIPS32(enum cpuType cpu) {
	wString ret = wInitString(1024);
	CPU = cpu;
	wAssign(&ret, "\t.set noreorder\n\t.cpload $25\n");
	wCAppend(&ret, "\taddiu $sp,$sp,-32\n\tsw $31,28($sp)\n");
	wCAppend(&ret, "\tsw $fp,24($sp)\n\tmove $fp,$sp\n\t.cprestore 16\n");
	return ret.data;
}

char *stackDeallocateMIPS32(char *funName) {
	wString ret = wInitString(1024);
	wAssign(&ret, "\tmove $sp,$fp\n\tlw $31,28($sp)\n");
	wCAppend(&ret, "\tlw $fp,24($sp)\n\taddiu $sp,$sp,32\n");
	wCAppend(&ret, wCFmt("\tjr $31\n\tnop\n\t.end %s\n", funName).data);
	return ret.data;
}

void MIPS32GetLVAlloc(char *buf, int bsize, Variable *var) {

}

char *mapRegisterMIPS32(char *reg) {
	int regNum = regToEnum(reg);
	switch(regNum) {
		case MIP32r4: return "$4";
		case MIP32r5: return "$5";
		case MIP32r6: return "$6";
		case MIP32r7: return "$7";
		case MIP32r16: return "$16";
		case MIP32r17: return "$17";
		case MIP32r2: return "$2";
		case MIP32r18: return "$18";
		case MIP32SP: return "$sp";
	};
	return "$4";
}

char *MIPS32GetCurrentVar(struct parserData *parser, Instruction *ins, int argSpot) {
	wString res;
	Variable *v = NULL;
	res.data = NULL; res.count = 0;
	if(parser==NULL||ins==NULL) return NULL;
	v = getVarFrom(parser, ins->arguments[argSpot]);
	if(v!=NULL) {
		switch(v->type) {
			case STRING: res = wCFmt("wl_str_%s", ins->arguments[argSpot]);break;
			case CHAR: res = wCFmt("wl_ch_%s", ins->arguments[argSpot]);break;
			case INT: res = wCFmt("wl_int_%s", ins->arguments[argSpot]);break;
			case FLOAT: res = wCFmt("wl_fl_%s", ins->arguments[argSpot]);break;
			case VOID: /*@TODO*/break;
			case PTR: res = wCFmt("wl_z_%s", ins->arguments[argSpot]);break;
			case ANY: break;
		};
	} else {
		/*@TODO*/
	}
	return res.data;
}

wString MIPS32GetMoveInstructions(struct parserData *parser, Instruction *ins,
		char *val1, char *val2) {
	wString res;
	Variable *var = getVarFrom(parser, ins->arguments[0]);
	res.data = NULL; res.count = 0;
	if(var) {
		if(var->varName!=NULL) {
			switch(var->type) {
				case CHAR:
				case INT: res = wCFmt("\tlw %s,% got(%s)($28)\n\tlw %s,0(%s)\n",
							   val2, val1, val2, val2);break;
				case STRING: res = wCFmt("\tlw %s,% got(%s)($28)\n\taddiu %s,%s,% lo(%s)\n",
							     val2, val1, val2, val2, val1);
				case FLOAT: break;
				default: break;
			};
		}
	} else {
		/*@TODO locals*/
	}
	return res;
}

char *convertInstructionMIPS32(AsmOut *out, Instruction ins) {
	int args = ins.argLen;
	int outLen = args*DEFMAXFSIZE;
	wString outBuf = wInitString(outLen);
	int i;

	CPU = out->parser->fData->cpu;

	/*Special instructions*/
	/*Inline - Drops direct asm instructions into the output*/
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
					wCAppend(&outBuf, wCFmt("\tlw $25,%got(%s)($28)\n\n",
								ins.arguments[0]).data);
				} else {
					wCAppend(&outBuf, wCFmt("\tlw $25,% call16(%s)($28)\n\n", 
								ins.arguments[0]).data);			
				}
				wCAppend(&outBuf, wCFmt("\t.reloc 1f,R_MIPS_JALR,%s\n", 
						ins.arguments[0]).data);
				wCAppend(&outBuf, "1:\tjalr $25\n\tnop\n");
				wCAppend(&outBuf, "\tlw $28,16($fp)\n");
			}
		
		/*
		 * Return
		 * */
		} else if(!strcmp(ins.instruction, "return")) {
			if(ins.arguments[0]!=NULL) {
				char *reg = NULL;
				char *dealloc = stackDeallocateMIPS32(ins.funName);
				if(strlen(ins.arguments[0])==0) ins.arguments[0] = "$0";
				else reg = mapRegisterMIPS32(ins.arguments[0]);
				if(reg!=NULL) {
					wCAppend(&outBuf, wCFmt("\tmove $2,%s\n%s",
							       	reg, dealloc).data);
				} else {
					wCAppend(&outBuf, wCFmt("\tmove $2,%s\n%s", 
								ins.arguments[0], dealloc).data);
				}
			}	
		}
	
	/*
	 * 2 argument instructions
	 * */
	} else if(args==2) {
		if(!strcmp(ins.instruction, "move")) {
			char *val1 = NULL;
			char *val2 = NULL;
			wString mov;
			mov.data = NULL; mov.count = 0;
			if(checkRegister(ins.arguments[0])) {
				val1 = mapRegisterMIPS32(ins.arguments[0]);
			} else {
				val1 = MIPS32GetCurrentVar(out->parser, &ins, 0);
			}
			if(checkRegister(ins.arguments[1])) {
				val2 = mapRegisterMIPS32(ins.arguments[1]);
			} else {
				val1 = MIPS32GetCurrentVar(out->parser, &ins, 1);
			}
			mov = MIPS32GetMoveInstructions(out->parser, &ins, val1, val2);
			wAppend(&outBuf, &mov);
		}
	}



	return outBuf.data;
}
