/*Copyright (c) 2022-2025 MinervaWare LLC*/

#include "asmout.h"
#include "cpu.h"

#include "ALPHA.h"
#include "PPC.h"
#include "ARM_MAC.h"
#include "AMDX8664.h"

/*
 * Register conversion
 * */

int checkRegister(char *reg) {
	WTRIM(reg);
	if(reg==NULL||reg[0]=='\0') return 0;
	if(reg[0]=='r'||reg[0]=='f') {
		int regNum;
		if(strlen(reg)>2) return 0;
		regNum = (int)reg[1]-48;
		if(regNum>0&&regNum<9) return 1;
	}
	return 0;
}

int regToEnum(char *reg) {
	if(!strcmp(reg, "r1")) return R1;
	if(!strcmp(reg, "r2")) return R2;
	if(!strcmp(reg, "r3")) return R3;
	if(!strcmp(reg, "r4")) return R4;
	if(!strcmp(reg, "r5")) return R5;
	if(!strcmp(reg, "r6")) return R6;
	if(!strcmp(reg, "r7")) return R7;
	if(!strcmp(reg, "r8")) return R8;
	if(!strcmp(reg, "sp")) return SP;
	if(!strcmp(reg, "f1")) return F1;
	if(!strcmp(reg, "f2")) return F2;
	if(!strcmp(reg, "f3")) return F3;
	if(!strcmp(reg, "f4")) return F4;
	if(!strcmp(reg, "f5")) return F5;
	if(!strcmp(reg, "f6")) return F6;
	if(!strcmp(reg, "f7")) return F7;
	return R1;
}

/*Instruction output*/

char *dumpInlineASM(Instruction *ins) {
	char *out;
	int i;
	if(ins->argLen==0) return NULL;
	out = calloc(strlen(ins->arguments[0])*ins->argLen+1024, sizeof(char));
	strcpy(out, ins->arguments[0]);
	for(i=1;i<ins->argLen;i++) {
		char buf[1024];
		snprintf(buf, sizeof(buf), ", %s", ins->arguments[i]);
		strcat(out, buf);
	}
	return out;
}

/* 
 * Funtion output
* */

char *getCPUMain() {
	char *ret = (char *)malloc(sizeof(char)*10);
	/*I know this is redundant,
	 * I still need to go through different compilers and sort the main per compiler, not CPU.*/
	switch(CPU) {
		case ARMv8: strcpy(ret, "main"); break;
		case ARM_MAC: strcpy(ret, "_main"); break;
		case AMD_X86_64: strcpy(ret, "main"); break;
		case I386: strcpy(ret, "main"); break; 
		case ALPHA: strcpy(ret, "main"); break; 
		case ITANIUM_64: strcpy(ret, "main"); break; 
		case ARMv7: strcpy(ret, "main"); break;
		case POWERPC: strcpy(ret, "main"); break;
		case RS6000: strcpy(ret, "main"); break;
		case SZ_IBM: strcpy(ret, "main"); break;
		case SPARC: strcpy(ret, "main"); break;
		case MIPS: strcpy(ret, "main"); break;
		case HPPA: strcpy(ret, "main"); break;
	}
	return ret;
}

int isEntryPoint(char *func) {
	return !(strcmp(func, getCPUMain()));
}

char *createFunctionHeader(char *name) {
	int bSize;
	char *head;
	if(!strcmp(name, "main")) name = getCPUMain();
	bSize = strlen(name)+1024;
	head = calloc(bSize, sizeof(char));
	switch(CPU) {
		case ALPHA: /*same as AMD_X86_64*/
		case AMD_X86_64: snprintf(head, bSize,
				  "\t.text\n\t.global %s\n%s:\n", name, name);
                                  break;
		case I386: snprintf(head, bSize,
                                           "\t.text\n\t.global %s\n%s:\n", name, name);
						   break;				
		case ITANIUM_64: break; /*TODO*/
		case ARMv8:
		case ARM_MAC: snprintf(head, bSize,
                                           "\t.global %s\n\t.p2align 2\n%s:\n",
                                           name, name);
			      break;
		case ARMv7: break; /*TODO*/
		case POWERPC: snprintf(head, bSize,
					      	"\t.section\t\".opd\",\"aw\"\n"
						"\t.global %s\n\t.align 3\n%s:"
						"\n\t.quad .wl_%s,.TOC.@tocbase, 0\n"
						"\t.section\t\".text\"\n.wl_%s:\n",
						name, name, name, name); 
			      break;
		case RS6000: break; /*TODO*/
		case SZ_IBM: break; /*TODO*/
		case SPARC: break; /*TODO*/
		case MIPS: break; /*TODO*/
		case HPPA: break; /*TODO*/
	};
	if(isEntryPoint(name)) {
		if(CPU==ALPHA) strcat(head, "\tldgp $29, 0($27)\n");
	}
	return head;
}

/*Init any local variables with provided data values.*/
char *getLVTAllocation(Function *func) {
	char *res = NULL;
	int i;
	if(func==NULL) return res;
	for(i=0;i<func->lvt->totalVariables;i++) {
		if(func->lvt->variables[i].varName!=NULL) {
			char *vName = func->lvt->variables[i].varName;
			char *value = func->lvt->variables[i].value;
			int offset = func->lvt->variables[i].offset;
			char *buf;
			int bSize;
			if(value==NULL||vName==NULL) continue;
			bSize = strlen(vName)+strlen(value)+1024;
			buf = calloc(bSize, sizeof(char));
			switch(CPU) {
				case I386:
				case AMD_X86_64: AMD_X86_64GetLVAlloc(buf, sizeof(char)*bSize, 
										 &func->lvt->variables[i]);
								 break;
				case ARMv8:
				case ARM_MAC: ARM_MACGetLVAlloc(buf, sizeof(char)*bSize,
									  &func->lvt->variables[i]);
							  break;
				case ALPHA: ALPHAGetLVTAlloc(buf, sizeof(char)*bSize,
									&func->lvt->variables[i]);
							break; 
				case ITANIUM_64: break; /*TODO*/
				case ARMv7: break; /*TODO*/
				case POWERPC: break; /*TODO*/
				case RS6000: break; /*TODO*/
				case SZ_IBM: break; /*TODO*/
				case SPARC: break; /*TODO*/
				case MIPS: break; /*TODO*/
				case HPPA: break; /*TODO*/

			};
			if(res==NULL) {
				res = calloc(strlen(buf)+1, sizeof(char));
				strcpy(res, buf);
			} else {
				res = (char *)realloc(res,
						(strlen(res)+strlen(buf)+1)*sizeof(char));
				strcat(res, buf);
			}
			free(buf);
		}
	}
	return res;
}

char *getSubScopeHeader(FuncSubScopeData *subScope) {
	int len = strlen(subScope->scope.scopeName)+1024;
	char *res = calloc(len, sizeof(char));
	snprintf(res, len*sizeof(char), ".%s:\n", subScope->scope.scopeName);	
	return res;
}

/*TODO This needs changed to check what type of scope we are under whenever the loops and other scopes are implemented*/
char *convertFunctionSubScopes(AsmOut *out, Function *func) {
	int bufferSize = 1;
	char *res = calloc(bufferSize, sizeof(char));
	int i,j;
	memset(res, 0, bufferSize);
	for(i=0;i<func->totalScopes;i++) {
		char *header = getSubScopeHeader(&func->subScopes[i]);
		char *ret;
		if(header!=NULL) {
			bufferSize += strlen(header)+1;
			res = (char *)realloc(res, sizeof(char)*bufferSize);
			strcat(res, header);
			free(header); header = NULL;
		}
		for(j=0;j<func->subScopes[i].totalData;j++) {
			Instruction *curIns = &func->subScopes[i].instructions[j];
			char *asmInstruction = NULL;
			switch(CPU) {
				case ALPHA: asmInstruction = 
								convertInstructionALPHA(out, *curIns);
							break;
				/*The x86_64 code also conducts 32-bit*/
				case I386:
				case AMD_X86_64:							
							asmInstruction = 
								convertInstructionAMD_X86_64(out, *curIns);
							break;
				case ITANIUM_64: break; /*TODO*/
				case ARMv8:
				case ARM_MAC: 
							asmInstruction = 
								convertInstructionARM_MAC(out, *curIns);
							break;
				case ARMv7: break; /*TODO*/
				case POWERPC: break; /*TODO*/
				case RS6000: break; /*TODO*/
				case SZ_IBM: break; /*TODO*/
				case SPARC: break; /*TODO*/
				case MIPS: break; /*TODO*/
				case HPPA: break; /*TODO*/
			};
			if(asmInstruction!=NULL) {
				bufferSize += strlen(asmInstruction)+1;	
				res = (char *)realloc(res, sizeof(char)*bufferSize);
				strcat(res, asmInstruction);
				free(asmInstruction);
			}
		}
		ret = calloc(strlen(func->subScopes[i].scope.scopeName)+128, sizeof(char));
		bufferSize += strlen(ret)+128;
		switch(CPU) {
			case AMD_X86_64: snprintf(ret, (strlen(ret)+128)*sizeof(char), 
									 "\tjmp .%s_cont\n",
									 func->subScopes[i].scope.scopeName);
							 break;
			case ARM_MAC: snprintf(ret, (strlen(ret)+128)*sizeof(char), 
									 "\tb .%s_cont\n",
									 func->subScopes[i].scope.scopeName);
						  break;
			default: break;
		};
		res = (char *)realloc(res, sizeof(char)*bufferSize);
		strcat(res, ret);
		free(ret); ret = NULL;
	}
	return res;
}

char *getStackAllocation() {
	switch(CPU) {
		case ALPHA: return stackAllocateALPHA(CPU);
		case I386:
		case AMD_X86_64: return stackAllocateAMD_X86_64(CPU);
		case ITANIUM_64: break; /*TODO*/
		case ARMv8:
		case ARM_MAC: return stackAllocateARM_MAC(CPU);
		case ARMv7: break; /*TODO*/
		case POWERPC: return stackAllocatePPC(CPU);
		case RS6000: break; /*TODO*/
		case SZ_IBM: break; /*TODO*/
		case SPARC: break; /*TODO*/
		case MIPS: break; /*TODO*/
		case HPPA: break; /*TODO*/
	};
	return NULL;
}

void convertFunctions(AsmOut *out) {
	int bufferSize = 1;
	int i,j;
	out->buffers.functions = calloc(1, sizeof(char));
	memset(out->buffers.functions, 0, bufferSize);

	for(i=0;i<out->parser->totalFunctions;i++) {
		static int setAllocation = 0;
		char *header = createFunctionHeader(out->parser->functions[i].funName);
		char *LVTAlloc = getLVTAllocation(&out->parser->functions[i]);
		char *subScopeOut;
		bufferSize+=strlen(header)+1;

		out->buffers.functions =
			realloc(out->buffers.functions, bufferSize);
		strcat(out->buffers.functions, header);
		free(header);header = NULL;
		/*instructions*/
		for(j=0;j<out->parser->functions[i].dataLength;j++) {
			Instruction *curIns = &out->parser->functions[i].instructions[j];
			char *asmInstruction = NULL;
			char *stackAllocation = getStackAllocation();
			if(curIns->instruction==NULL) continue;

			switch(CPU) {
				case ALPHA: asmInstruction = convertInstructionALPHA(out, *curIns);
							break;
				/*The x86_64 code also conducts 32-bit*/
				case I386:
				case AMD_X86_64:							
							asmInstruction = convertInstructionAMD_X86_64(out, *curIns);
							break;
				case ITANIUM_64: break; /*TODO*/
				case ARMv8:
				case ARM_MAC: 
							asmInstruction = convertInstructionARM_MAC(out, *curIns);
							break;
				case ARMv7: break; /*TODO*/
				case POWERPC: 
					    		asmInstruction = convertInstructionPPC(out, *curIns);
							break;
				case RS6000: break; /*TODO*/
				case SZ_IBM: break; /*TODO*/
				case SPARC: break; /*TODO*/
				case MIPS: break; /*TODO*/
				case HPPA: break; /*TODO*/
			};
			if(asmInstruction!=NULL&&strcmp(asmInstruction, "")) {
				bufferSize+=strlen(asmInstruction)+
							strlen(stackAllocation)+2;
				out->buffers.functions =
					(char *)realloc(out->buffers.functions, bufferSize);
				if(setAllocation==0) {
					strcat(out->buffers.functions, stackAllocation);
					if(LVTAlloc!=NULL) {
						bufferSize += strlen(LVTAlloc)+1;
						out->buffers.functions =
							(char *)realloc(out->buffers.functions, bufferSize);
						strcat(out->buffers.functions, LVTAlloc);
					}
					setAllocation=1;
				}
				strcat(out->buffers.functions, asmInstruction);
				free(asmInstruction); asmInstruction = NULL;
			}
			free(stackAllocation);stackAllocation=NULL;
		}
		free(LVTAlloc);
		LVTAlloc = NULL;
		setAllocation = 0;

		/*Check for void function type so we can return correctly*/
		if(out->parser->functions[i].type==VOID) {
			char *deallocateStack = NULL;
			switch(CPU) {
				case ALPHA: deallocateStack = stackDeallocateALPHA();break;
				case AMD_X86_64: 
				case I386: deallocateStack = stackDeallocateAMD_X86_64();break;
				case ITANIUM_64: break; /*TODO*/
				case ARMv8:
				case ARM_MAC: deallocateStack = stackDeallocateARM_MAC();break;
				case ARMv7: break; /*TODO*/
				case POWERPC: deallocateStack = stackDeallocatePPC();break;
				case RS6000: break; /*TODO*/
				case SZ_IBM: break; /*TODO*/
				case SPARC: break; /*TODO*/
				case MIPS: break; /*TODO*/
				case HPPA: break; /*TODO*/
			};
			if(CPU!=POWERPC) {
				if(deallocateStack!=NULL) strcat(deallocateStack, "\tret\n");
				else deallocateStack = "\tret\n";
			}
			bufferSize+=strlen(deallocateStack)+2;
			out->buffers.functions =
				(char *)realloc(out->buffers.functions, bufferSize);
			strcat(out->buffers.functions, deallocateStack);
			if(deallocateStack!=NULL) {free(deallocateStack);deallocateStack = NULL;}
		}
		/*Function sub-scopes*/
		subScopeOut = 
			convertFunctionSubScopes(out, &out->parser->functions[i]);
		if(subScopeOut!=NULL) {
			bufferSize+=strlen(subScopeOut);
			out->buffers.functions =
				(char *)realloc(out->buffers.functions, bufferSize);
			strcat(out->buffers.functions, subScopeOut);
			free(subScopeOut);subScopeOut = NULL;
		}
	}
}

/* This should be pretty standard along all CPUs
 * Variable conversion
 * */

char *getAsmString(char *name, char *value) {
	int bLen = strlen(name)+strlen(value)+1024;
	char *buf = calloc(bLen, sizeof(char));
	char *ret;
	switch(CPU) {
		case AMD_X86_64: snprintf(buf, sizeof(char)*bLen,
                           "\t.text\n\t.global wl_str_%s\n.rawwl_str%s:\n\t.asciz %s\n\t"
                           ".data\n\t.align 8\nwl_str_%s:\n\t.quad .rawwl_str%s\n",
                           name, name, value, name, name);break;
		case I386: snprintf(buf, sizeof(char)*bLen,
                           "\t.text\n\t.global wl_str_%s\n.rawwl_str%s:\n\t.asciz %s\n\t"
                           ".data\n\t.align 8\nwl_str_%s:\n\t.long .rawwl_str%s\n",
                           name, name, value, name, name);break;
		case ITANIUM_64: break; /*TODO*/
		case ARMv8: /*Same as ARM_MAC*/
		case ALPHA: /*Same as ARM_MAC*/
		case POWERPC: /*Same as ARM_MAC*/
		case ARM_MAC: snprintf(buf, sizeof(char)*bLen, "wl_str_%s:\n\t.asciz %s\n",
                           name, value);break;
		case ARMv7: break; /*TODO*/
		case RS6000: break; /*TODO*/
		case SZ_IBM: break; /*TODO*/
		case SPARC: break; /*TODO*/
		case MIPS: break; /*TODO*/
		case HPPA: break; /*TODO*/
	};
	ret = calloc(strlen(buf)+1, sizeof(char));
	strcpy(ret, buf);
	free(buf);
	return ret;
}

char *getAsmChar(char *name, char *value) {
	char *nameBuf;
	char *buf;
	char *ret;
	int nbLen, bLen;
	while(value[0]=='\''||value[0]==' ') value++;
	while(value[strlen(value)-1]=='\'') value[strlen(value)-1] = '\0';
	nbLen = strlen(name)+100;
	nameBuf = calloc(nbLen, sizeof(char));
	snprintf(nameBuf, sizeof(char)*nbLen, "wl_ch_%s", name);
	bLen = strlen(name)+strlen(value)+100;
	buf = calloc(bLen, sizeof(char));
	snprintf(buf, sizeof(char)*bLen,
			"\n\t.global %s\n%s:\n\t.byte %d\n",
			nameBuf, nameBuf, value[0]);
	ret = calloc(strlen(buf)+1, sizeof(char));
	strcpy(ret, buf);
	free(nameBuf);
	free(buf);
	return ret;
}

char *getAsmInt(char *name, char *value) {
	char *hexValue = intToHex(value);
	int nbLen = strlen(name)+100;
	int bLen;
	char *ret;
	char *buf;
	char *nameBuf = calloc(nbLen, sizeof(char));
	snprintf(nameBuf, sizeof(char)*nbLen, "wl_int_%s", name);
	bLen = strlen(nameBuf)+strlen(hexValue)+100;
	buf = calloc(bLen, sizeof(char));
	snprintf(buf, sizeof(char)*bLen, 
			"\n\t.global %s\n\t.p2align 2,0x0\n%s:\n\t.long %s\n",
			nameBuf, nameBuf, hexValue);	
	ret = calloc(strlen(buf)+1, sizeof(char));
	strcpy(ret, buf);
	free(nameBuf);
	free(buf);
	return ret;
}

char *getAsmFloat(char *name, char *value) {
	char *hexValue = floatToHex(value);
	int nbLen = strlen(name)+100;
	int bLen;
	char *nameBuf = calloc(nbLen, sizeof(char));
	char *buf;
	char *ret;
	snprintf(nameBuf, sizeof(char)*nbLen, "wl_fl_%s", name);
	bLen = strlen(nameBuf)+strlen(hexValue)+100;
	buf = calloc(bLen, sizeof(char));
	snprintf(buf, sizeof(char)*bLen, 
			"\n\t.global %s\n\t.p2align 2,0x0\n%s:\n\t.long %s\n",
			nameBuf, nameBuf, hexValue);	
	ret = calloc(strlen(buf)+1, sizeof(char));
	strcpy(ret, buf);
	free(nameBuf);
	free(buf);
	return ret;
}

char *getAsmZero(char *name, char *value) {
	int len = strlen(name)+strlen(value)+128;
	char *ret = calloc(len, sizeof(char));
	snprintf(ret, sizeof(char)*len, "\n.zerofill __DATA,__bss,wl_z_%s,%s,3", name, value);
	return ret;
}

void convertVariables(AsmOut *out) {
	int totalSize = 1;
	int i,j;
	struct parserData *parser = out->parser;
	out->buffers.variables = calloc(1, sizeof(char));
	for(i=0;i<out->parser->totalVariables;i++) {	
		char *curName, *curValue, *asmVar = NULL;
		curName = calloc(strlen(out->parser->variables[i].varName)+1, sizeof(char));
		strcpy(curName, out->parser->variables[i].varName);
		curValue = calloc(strlen(out->parser->variables[i].value)+1, sizeof(char));
		strcpy(curValue, out->parser->variables[i].value);
		switch(out->parser->variables[i].type) {
			case INT: asmVar = getAsmInt(curName, curValue);break;
			case FLOAT: asmVar = getAsmFloat(curName, curValue);break;
			case STRING: asmVar = getAsmString(curName, curValue);break;
			case CHAR: asmVar = getAsmChar(curName, curValue);break;
			case VOID: /*TODO*/break;
			case ZERO: asmVar = getAsmZero(curName, curValue);
		};
		if(asmVar!=NULL) {
			totalSize += strlen(asmVar);
			out->buffers.variables = (char *)realloc(
					out->buffers.variables, sizeof(char)*(totalSize+2));
			strcat(out->buffers.variables, asmVar);	
		}	
		free(curName);
		free(curValue);
	}
	/*Local string variables*/
	for(i=0;i<parser->totalFunctions;i++) {
		Function *f = &parser->functions[i];
		for(j=0;j<f->lvt->totalVariables;j++) {
			char *asmVar = NULL;
			if(f->lvt->variables[j].type==STRING) {
				asmVar = getAsmString(f->lvt->variables[j].varName,
						f->lvt->variables[j].value);
				if(asmVar!=NULL) {
					totalSize += strlen(asmVar);
					out->buffers.variables = (char *)realloc(
							out->buffers.variables, sizeof(char)*(totalSize+2));
					strcat(out->buffers.variables, asmVar);	
				}
			} else if(f->lvt->variables[j].type==FLOAT) {
				asmVar = getAsmFloat(f->lvt->variables[j].varName,
						f->lvt->variables[j].value);
				if(asmVar!=NULL) {
					totalSize += strlen(asmVar);
					out->buffers.variables = (char *)realloc(
							out->buffers.variables, sizeof(char)*(totalSize+2));
					strcat(out->buffers.variables, asmVar);	
				}
			}
		}
	}
}

/*
 * Externals and inclusiom
 * */

void convertExternals_Includes(AsmOut *out) {
	int totalSize=1;
	int i;
	out->buffers.externals = calloc(1, sizeof(char));
	/*Externals*/
	for(i=0;i<out->parser->externals.externSize;i++) {
		char *curEx = calloc(strlen(out->parser->externals.externs[i])+1,sizeof(char));
		char *buf;
		int bLen;
		strcpy(curEx, out->parser->externals.externs[i]);
		bLen = strlen(curEx)+100;
		buf = calloc(bLen, sizeof(char));
		snprintf(buf, sizeof(char)*bLen, "\t.extern %s\n", curEx);
		if(curEx!=NULL) {
			totalSize+=strlen(buf);
			out->buffers.externals = 
				(char *)realloc(out->buffers.externals, sizeof(char)*(totalSize+2));
			strcat(out->buffers.externals, buf);
		}
		free(buf);
	}
}

/*
 * Run
 * */

void completeBuffer(AsmOut *out) {
	int totalSize;
	if(out->buffers.functions==NULL||
			out->buffers.variables==NULL) return;
	totalSize = 
		strlen(out->buffers.functions)+
		strlen(out->buffers.variables)+
		strlen(out->buffers.externals);
	out->buffers.output.asmOutBuffer = calloc(totalSize*100, sizeof(char));
	strcpy(out->buffers.output.asmOutBuffer, out->buffers.externals);
	strcat(out->buffers.output.asmOutBuffer, out->buffers.functions);
	strcat(out->buffers.output.asmOutBuffer, out->buffers.variables);
}

void convertToAsm(AsmOut *out) {
	convertExternals_Includes(out);
	convertFunctions(out);
	convertVariables(out);

	completeBuffer(out);
	fprintf(out->asmOut, "%s", out->buffers.output.asmOutBuffer);
	free(out->buffers.output.asmOutBuffer);
	free(out->buffers.externals);
	free(out->buffers.functions);
	free(out->buffers.variables);
}


/*
 * Initialization & exiting
 * */

int getRoughFileSize(AsmOut *out) {
	return out->parser->bufferSize*2;
}

void freeAsm(AsmOut *out) {
	fclose(out->asmOut);
	free(out->functions);
}

void initAsmOut(struct parserData *parser, AsmOut *output) {
	char *fileName;
	if(output==NULL) output = calloc(1, sizeof(AsmOut));
	output->parser = parser;
	output->functions = calloc(parser->totalFunctions, sizeof(Function));
	CPU = parser->fData->cpu;

	fileName = strtok(parser->fData->fileName, ".");
	strcat(fileName, ".s");

	output->asmOut = fopen(fileName, "r");
	/*File already exists*/
	if(output->asmOut!=NULL) {
		char rm[100];
		snprintf(rm, sizeof(rm), "rm %s", fileName);
		system(rm);
		fclose(output->asmOut);
	}
	
	/*reopen with write mode/create file*/
	output->asmOut = fopen(fileName, "wr");

	output->buffers.output.AOBSize = 0;

	/*Don't forget output buffer is in parser->output.asmOutBuffer*/
}


