/*Copyright (c) 2024-2026 MinervaWare LLC*/
/*Copyright (c) 2022-2024 Tristan Wellman*/

#include "MIPS32.h"

char *stackAllocateMIPS32(enum cpuType cpu) {
	wString ret = wInitString(1024);
	CPU = CPU;
	wAssign(&ret, "\t.set noreorder\n\t.cpload $25\n");
	wCAppend(&ret, "\taddiu $sp,$sp,-32\n\tsw $31,28($sp)\n");
	wCAppend(&ret, "\tsw $fp,24($sp)\n\tmove $fp,$sp\n\t.cprestore 16");
	return ret.data;
}

char *stackDeallocateMIPS32() {
	wString ret = wInitString(1024);
	wCAppend(&ret, "\tmove $sp,$fp\n\tlw $31,28($sp)\n");
	wCAppend(&ret, "\tlw $fp,24($sp)\n\taddiu $sp,$sp,32\n");
	wCAppend(&ret, "\tjr $31\n\tnop\n");
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
					wCAppend(&outBuf, wCFmt("\tlw $25,%got(%s)($28)\n",
								ins.arguments[0]).data);
				} else {
					wCAppend(&outBuf, wCFmt("\tlw $25,%call16(%s)($28)\n", 
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
				char *dealloc = stackDeallocateMIPS32();
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
	}



	return outBuf.data;
}
