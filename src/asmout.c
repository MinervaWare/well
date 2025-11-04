/*Copyright (c) 2024 Tristan Wellman*/

#include "asmout.h"
#include "cpu.h"

#include "ARM_MAC.h"
#include "AMD_X86_64.h"

/*
 * Register conversion
 * */

int checkRegister(char *reg) {
	WTRIM(reg);
	if(reg==NULL||reg[0]=='\0') return 0;
	char *num = strstr(reg, "r");
	if(num!=NULL) {
		num++;
		if(strlen(num)>1) return 0;
		int regNum = atoi(num);
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
	return R1;
}


/* 
 * Funtion output
* */

char *getCPUMain() {
	char *ret = (char *)malloc(sizeof(char)*10);
	/*I know this is redundant,
	 * I still need to go through different compilers and sort the main per compiler, not CPU.*/
	switch(CPU) {
		case ARM_MAC: strcpy(ret, "main"); break;
		case AMD_X86_64: strcpy(ret, "main"); break;
		case I386: strcpy(ret, "main"); break; 
		case ALPHA: strcpy(ret, "main"); break; 
		case ITANIUM_64: strcpy(ret, "main"); break; 
		case ARMv7: strcpy(ret, "main"); break;
		case ARMv8: strcpy(ret, "main"); break;
		case POWERPC: strcpy(ret, "main"); break;
		case RS6000: strcpy(ret, "main"); break;
		case SZ_IBM: strcpy(ret, "main"); break;
		case SPARC: strcpy(ret, "main"); break;
	}
	return ret;
}

char *createFunctionHeader(char *name) {
	if(!strcmp(name, "main")) name = getCPUMain();
	int bSize = strlen(name)+1024;
	char *head = calloc(bSize, sizeof(char));
    switch(CPU) {
				case ALPHA: break; /*TODO*/
				case AMD_X86_64: snprintf(head, bSize,
                                           "\t.text\n\t.global %s\n%s:\n", name, name);
                                  break;
				case I386: snprintf(head, bSize,
                                           "\t.text\n\t.global %s\n%s:\n", name, name);
						   break;				
				case ITANIUM_64: break; /*TODO*/
				case ARM_MAC: snprintf(head,bSize,
                                           "\t.section __TEXT,__text\n\t.global _%s\n\t.p2align 2\n_%s:\n",
                                           name, name);
                              break;
				case ARMv7: break; /*TODO*/
				case ARMv8: break; /*TODO*/
				case POWERPC: break; /*TODO*/
				case RS6000: break; /*TODO*/
				case SZ_IBM: break; /*TODO*/
				case SPARC: break; /*TODO*/
	};
	return head;
}

void convertFunctions(AsmOut *out) {
	out->buffers.functions = calloc(1, sizeof(char));

	size_t bufferSize=1;
	int i,j;
	for(i=0;i<out->parser->totalFunctions;i++) {
		char *header = createFunctionHeader(out->parser->functions[i].funName);
		bufferSize+=strlen(header)+1;
		out->buffers.functions =
			realloc(out->buffers.functions, bufferSize);
		strcat(out->buffers.functions, header);
		free(header);

		static int setAllocation = 0;
		/*instructions*/
		for(j=0;j<out->parser->functions[i].dataLength;j++) {
			Instruction *curIns = &out->parser->functions[i].instructions[j];
			if(curIns->instruction==NULL) continue;
			char *asmInstruction = NULL;
			char *stackAllocation = NULL;

			switch(CPU) {
				/*alpha*/
				case ALPHA: break; /*TODO*/
				/*intel*/
				case AMD_X86_64:							
							asmInstruction = convertInstructionAMD_X86_64(out, *curIns);
                            stackAllocation = stackAllocateAMD_X86_64();
							break;
				/*The x86_64 code checks for 32-bit and switches.*/
				case I386: 	asmInstruction = convertInstructionAMD_X86_64(out, *curIns);
                            stackAllocation = stackAllocateAMD_X86_64();
							break;
				case ITANIUM_64: break; /*TODO*/
				/*ARM*/
				case ARM_MAC: 
							asmInstruction = convertInstructionARM_MAC(out, *curIns);
							stackAllocation = stackAllocateARM_MAC();
							break;
				case ARMv7: break; /*TODO*/
				case ARMv8: break; /*TODO*/
				/*IBM*/
				case POWERPC: break; /*TODO*/
				case RS6000: break; /*TODO*/
				case SZ_IBM: break; /*TODO*/
				/*SUN*/
				case SPARC: break; /*TODO*/
			};
			if(asmInstruction!=NULL&&strcmp(asmInstruction, "")) {
				bufferSize+=strlen(asmInstruction)+
							strlen(stackAllocation)+2;
				out->buffers.functions =
					(char *)realloc(out->buffers.functions, bufferSize);
				if(setAllocation==0) {
					strcat(out->buffers.functions, stackAllocation);
					setAllocation=1;
				}
				strcat(out->buffers.functions, asmInstruction);
				free(asmInstruction);
				asmInstruction = NULL;
            }
			free(stackAllocation);
			if(asmInstruction!=NULL) free(asmInstruction);
		}
		setAllocation = 0;

		/*Check for void function type so we can return correctly*/
		if(out->parser->functions[i].type==VOID) {
			char *deallocateStack = NULL;
			switch(CPU) {
				case ALPHA: break; /*TODO*/
				case AMD_X86_64: deallocateStack = stackDeallocateAMD_X86_64();break;
				case I386: deallocateStack = stackDeallocateAMD_X86_64();break;
				case ITANIUM_64: break; /*TODO*/
				case ARM_MAC: deallocateStack = stackDeallocateARM_MAC();break;
				case ARMv7: break; /*TODO*/
				case ARMv8: break; /*TODO*/
				case POWERPC: break; /*TODO*/
				case RS6000: break; /*TODO*/
				case SZ_IBM: break; /*TODO*/
				case SPARC: break; /*TODO*/
			};
			if(deallocateStack!=NULL) strcat(deallocateStack, "\tret\n");
			else deallocateStack = "\tret\n";
			bufferSize+=strlen(deallocateStack)+2;
			out->buffers.functions =
				(char *)realloc(out->buffers.functions, bufferSize);
			strcat(out->buffers.functions, deallocateStack);
		}

	}
}

/* This should be pretty standard along all CPUs
 * Variable conversion
 * */

char *getAsmString(char *name, char *value) {
	char buf[strlen(name)+strlen(value)+1024];
    switch(CPU) {
		case ALPHA: break; /*TODO*/
		case AMD_X86_64: snprintf(buf, sizeof(buf),
                           "\t.text\n\t.global wl_str_%s\n.rawwl_str%s:\n\t.asciz %s\n\t"
                           ".data\n\t.align 8\nwl_str_%s:\n\t.quad .rawwl_str%s\n",
                           name, name, value, name, name);break;
		case I386: break; snprintf(buf, sizeof(buf),
                           "\t.text\n\t.global wl_str_%s\n.rawwl_str%s:\n\t.asciz %s\n\t"
                           ".data\n\t.align 8\nwl_str_%s:\n\t.long .rawwl_str%s\n",
                           name, name, value, name, name);break;
		case ITANIUM_64: break; /*TODO*/
		case ARM_MAC: snprintf(buf, sizeof(buf), "wl_str.%s:\n\t.asciz %s\n",
                           name, value);break;
        case ARMv7: break; /*TODO*/
		case ARMv8: break; /*TODO*/
		case POWERPC: break; /*TODO*/
		case RS6000: break; /*TODO*/
		case SZ_IBM: break; /*TODO*/
		case SPARC: break; /*TODO*/
	};
	char *ret = calloc(strlen(buf)+1, sizeof(char));
	strcpy(ret, buf);
	return ret;
}

char *getAsmChar(char *name, char *value) {
	while(value[0]=='\''||value[0]==' ') value++;
	while(value[strlen(value)-1]=='\'') value[strlen(value)-1] = '\0';
	char nameBuf[strlen(name)+100];
	snprintf(nameBuf, sizeof(nameBuf), "wl_ch_%s", name);
	char buf[strlen(name)+strlen(value)+100];
	snprintf(buf, sizeof(buf),
			"\n\t.global %s\n%s:\n\t.byte %d\n",
			nameBuf, nameBuf, value[0]);
	char *ret = calloc(strlen(buf)+1, sizeof(char));
	strcpy(ret, buf);
	return ret;
}

char *getAsmInt(char *name, char *value) {
	char *hexValue = intToHex(value);
	char nameBuf[strlen(name)+100];
	snprintf(nameBuf, sizeof(nameBuf), "wl_int_%s", name);
	char buf[strlen(nameBuf)+strlen(hexValue)+100];
	snprintf(buf, sizeof(buf), 
			"\n\t.global %s\n\t.p2align 2,0x0\n%s:\n\t.long %s\n",
			nameBuf, nameBuf, hexValue);	
	char *ret = calloc(strlen(buf)+1, sizeof(char));
	strcpy(ret, buf);
	return ret;
}

char *getAsmFloat(char *name, char *value) {
	char *hexValue = floatToHex(value);
	char nameBuf[strlen(name)+100];
	snprintf(nameBuf, sizeof(nameBuf), "wl_int_%s", name);
	char buf[strlen(nameBuf)+strlen(hexValue)+100];
	snprintf(buf, sizeof(buf), 
			"\n\t.global %s\n\t.p2align 2,0x0\n%s:\n\t.long %s\n",
			nameBuf, nameBuf, hexValue);	
	char *ret = calloc(strlen(buf)+1, sizeof(char));
	strcpy(ret, buf);
	return ret;
}

void convertVariables(AsmOut *out) {
	out->buffers.variables = calloc(1, sizeof(char));
	int totalSize = 1;
	int i;
	for(i=0;i<out->parser->totalVariables;i++) {	

		char *curName = calloc(strlen(out->parser->variables[i].varName)+1, sizeof(char));
		strcpy(curName, out->parser->variables[i].varName);
		char *curValue = calloc(strlen(out->parser->variables[i].value)+1, sizeof(char));
		strcpy(curValue, out->parser->variables[i].value);
		char *asmVar = NULL;	
		switch(out->parser->variables[i].type) {
			case INT: asmVar = getAsmInt(curName, curValue);break;
			case FLOAT: asmVar = getAsmFloat(curName, curValue);break;
			case STRING: asmVar = getAsmString(curName, curValue);break;
			case CHAR: asmVar = getAsmChar(curName, curValue);break;
			case VOID: /*TODO*/break;
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
}

/*
 * Externals and inclusiom
 * */

void convertExternals_Includes(AsmOut *out) {
	out->buffers.externals = calloc(1, sizeof(char));
	int totalSize=1;
	int i;
	/*Externals*/
	for(i=0;i<out->parser->externals.externSize;i++) {
		char *curEx = calloc(strlen(out->parser->externals.externs[i])+1,sizeof(char));
		strcpy(curEx, out->parser->externals.externs[i]);
		char buf[strlen(curEx)+100];
		snprintf(buf, sizeof(buf), "\t.extern %s\n", curEx);
		if(curEx!=NULL) {
			totalSize+=strlen(buf);
			out->buffers.externals = 
				(char *)realloc(out->buffers.externals, sizeof(char)*(totalSize+2));
			strcat(out->buffers.externals, buf);
		}
	}
}

/*
 * Run
 * */

void completeBuffer(AsmOut *out) {
	if(out->buffers.functions==NULL||
			out->buffers.variables==NULL) return;
	int totalSize = 
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
	if(output==NULL) output = calloc(1, sizeof(AsmOut));
	output->parser = parser;
	output->functions = calloc(parser->totalFunctions, sizeof(Function));

	char *fileName = strtok(parser->fData->fileName, ".");
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


