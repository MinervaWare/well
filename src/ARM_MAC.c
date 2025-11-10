/*Copyright (c) 2024 Tristan Wellman*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "asmout.h"
#include "cpu.h"
#include "ARM_MAC.h"

MacRegData *macRegData = NULL;

/*
 * Stack initialization is going to be large and unoptimized to local variables/structs.
 * This is because
 * */
char *stackAllocateARM_MAC() {
	/*auto to 16*/
	char *ret = calloc(1024, sizeof(char));
	sprintf(ret, "\tsub sp, sp, #32\n\tstp x29, x30, [sp, #16]\n\tadd x29, sp, #16\n");
	return ret; 
}

char *stackDeallocateARM_MAC() {
	/*auto to 16*/
	char *ret = calloc(1024, sizeof(char));
	sprintf(ret, "\tldp x29, x30, [sp, #16]\n\tadd sp, sp, #32\n");
	return ret; 
}

char *mapRegister(char *reg) {
	int regNum = regToEnum(reg);	
	switch(regNum) {
		case X0: return "x0";
		case X1: return "x1";
		case X2: return "x2";
		case X3: return "x3";
		case X4: return "x4";
		case X5: return "x5";
		case X6: return "x6";
		case X7: return "x7";
		case MACARMSP: return "sp";
	};
	return "x0";
}

char *mapVarRegister(char *reg, enum varTypes type) {
	/*Magic 3 is just register string size + 1*/
	char *ret = NULL;
	ret = calloc(3, sizeof(char));
	char rt = 'x';
	switch(type) {
		case CHAR:
		case INT: rt = 'w'; break;
		case FLOAT: rt = 's'; break;
		case STRING: 
		default: rt = 'x'; break;
	};
	int regNum = regToEnum(reg);	
	switch(regNum) {
		case X0: snprintf(ret, sizeof(ret), "%c0", rt);break;
		case X1: snprintf(ret, sizeof(ret), "%c1", rt);break;
		case X2: snprintf(ret, sizeof(ret), "%c2", rt);break;
		case X3: snprintf(ret, sizeof(ret), "%c3", rt);break;
		case X4: snprintf(ret, sizeof(ret), "%c4", rt);break;
		case X5: snprintf(ret, sizeof(ret), "%c5", rt);break;
		case X6: snprintf(ret, sizeof(ret), "%c6", rt);break;
		case X7: snprintf(ret, sizeof(ret), "%c7", rt);break;
		case MACARMSP: return "sp";
	};
	return ret;
}

char getVarRegType(enum varTypes type) {
	switch(type) {
		case CHAR:
		case INT: return 'w';
		case FLOAT: return 's';
		case STRING: 
		default: return 'x';
	};
}

/*x registers are 64-bit w registers are 32*/
char *convertInstructionARM_MAC(AsmOut *out, Instruction ins) {
	if(macRegData==NULL) {
		macRegData = calloc(1, sizeof(MacRegData));
		macRegData->prevRegType = 'x';
	}
	int args = ins.argLen;
	int outlen = args*DEFMAXFSIZE;
	char outBuf[outlen];
	outBuf[0] = '\0';
	int i;

	for(i=0;i<args;i++) WTRIM(ins.arguments[i]);

	/*Special instructions*/
	/*Inline - Drops direct asm instructions into the output*/
	if(!strcmp(ins.instruction, "inline")) 
		snprintf(outBuf, sizeof(outBuf), "\t%s\n", dumpInlineASM(&ins));
	

	/*
	 * 0 argument instructions
	 * */
	if(args==0) {
		/*
		 * Jump - Jump to or from function 
		 * NOTE: This is "temporary" until I get if statements and loops going
		 * */
		if(!strcmp(ins.instruction, "jump")) {
			snprintf(outBuf, sizeof(outBuf), "\tb _%s\n", ins.arguments[0]);
		}

	/*
	 * 1 argument instructions
	 * */
	} else if(args==1) {
		/*
		 * Call
		 * */
		if(!strcmp(ins.instruction, "call")) {
			if(ins.argLen>0 && ins.arguments[0]!=NULL)
				snprintf(outBuf, sizeof(outBuf), "\tbl _%s\n", ins.arguments[0]);
		/*
		 * Return
		 * */
		} else if(!strcmp(ins.instruction, "return")) {
			if(ins.arguments[0]!=NULL) {
				if(strlen(ins.arguments[0])==0) ins.arguments[0] = "0";
				sprintf(outBuf, "\tmov x0, #%s\n%s\tret\n",
						ins.arguments[0], stackDeallocateARM_MAC());
			}

		 /*
		  * SStruct - save structure from return.
		  * NOTE: this is sort of patch work so I can manage C lib structs easier
		  * */
		} else if(!strcmp(ins.instruction, "sstruct")) {
			char *reg = mapRegister(ins.arguments[0]);
			snprintf(outBuf, sizeof(outBuf), 
					"\tstr %s, [sp, #8]\n", reg);
		
		/*
		 * LStruct - Load returned C struct into register
		 * NOTE: this is sort of patch work so I can manage C lib structs easier
		 * */
		} else if(!strcmp(ins.instruction, "lstruct")) {
			char *reg = mapRegister(ins.arguments[0]);
			snprintf(outBuf, sizeof(outBuf), "\tldr %s, [sp, #8]\n", reg);
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
			if(checkRegister(ins.arguments[0])) {
				char *ARMReg = mapRegister(ins.arguments[0]);
				val1 = calloc(strlen(ARMReg)+1, sizeof(char));
				strcpy(val1, ARMReg);
			} else {
				Variable v = getVarFrom(out->parser, ins.arguments[0]);
				/*If we are moving a var we need to set the type for the scope's future*/
				macRegData->prevRegType = getVarRegType(v.type);

				char asmVName[1024];
				if(v.varName!=NULL) {
					switch(v.type) {
						case STRING: sprintf(asmVName, "wl_str.%s", ins.arguments[0]);break;
						case CHAR: sprintf(asmVName, "wl_ch_%s", ins.arguments[0]);break;
						case INT: sprintf(asmVName, "wl_int_%s", ins.arguments[0]);break;
						case FLOAT: sprintf(asmVName, "wl_fl_%s", ins.arguments[0]);break;
						case VOID: /*TODO*/break;
						case ZERO: sprintf(asmVName, "wl_z_%s", ins.arguments[0]);break;
					};
				} else snprintf(asmVName, sizeof(asmVName), "_%s", ins.arguments[0]);
				val1 = calloc(strlen(asmVName)+1, sizeof(char));
				strcpy(val1, asmVName);
			}
			if(checkRegister(ins.arguments[1])) {
				char *ARMReg = mapRegister(ins.arguments[1]);
				val2 = calloc(strlen(ARMReg)+1, sizeof(char)); 
				strcpy(val2, ARMReg);
			} else {
				Variable v = getVarFrom(out->parser, ins.arguments[1]);
				char asmVName[1024];
				if(v.varName!=NULL) {
					switch(v.type) {
						case STRING: sprintf(asmVName, "wl_str.%s", ins.arguments[1]);break;
						case CHAR: sprintf(asmVName, "wl_ch_%s", ins.arguments[1]);break;
						case INT: sprintf(asmVName, "wl_int_%s", ins.arguments[1]);break;
						case FLOAT: sprintf(asmVName, "wl_fl_%s", ins.arguments[1]);break;
						case VOID: /*TODO*/break;
						case ZERO: sprintf(asmVName, "wl_z_%s", ins.arguments[0]);break;
					};
				} else snprintf(asmVName, sizeof(asmVName), "_%s", ins.arguments[1]);
				val2 = calloc(strlen(asmVName)+1, sizeof(char));
				strcpy(val2, asmVName);
			}

			Variable var = getVarFrom(out->parser, ins.arguments[0]);
			if(var.varName!=NULL) {
				snprintf(outBuf, sizeof(outBuf), 
						"\tadrp %s,%s@PAGE\n\tadd %s, %s, %s@PAGEOFF\n",
						val2, val1, val2, val2, val1);
				switch(var.type) {
					case INT: snprintf(outBuf, sizeof(outBuf), 
									  "\tadrp %s,%s@PAGE\n\tldr w%s, [%s, %s@PAGEOFF]\n",
									  val2, val1, val2+1, val2, val1);
							  break;
					case CHAR: snprintf(outBuf, sizeof(outBuf), 
									  "\tadrp %s,%s@PAGE\n\tldrsb w%s, [%s, %s@PAGEOFF]\n",
									  val2, val1, val2+1, val2, val1);
							   break;
					case STRING: snprintf(outBuf, sizeof(outBuf), 
									  "\tadrp %s,%s@PAGE\n\tadd %s, %s, %s@PAGEOFF\n",
									  val2, val1, val2, val2, val1);
								 break;

					case FLOAT: snprintf(outBuf, sizeof(outBuf), 
									  "\tadrp %s,%s@PAGE\n\tldr s%s, [%s, %s@PAGEOFF]\n",
									  val2, val1, val2+1, val2, val1);
								break;
					default: snprintf(outBuf, sizeof(outBuf), 
									 "\tadrp %s,%s@PAGE\n\tadd %s, %s, %s@PAGEOFF\n",
									 val2, val1, val2, val2, val1);
							 break;
				};
			} else {
				snprintf(outBuf, sizeof(outBuf), 
						"\tadrp %s,%s@PAGE\n\tadd %s, %s, %s@PAGEOFF\n",
						val2, val1, val2, val2, val1);
			}
		
		/*
		 * Bitwise Instructions (Logical modifications are made with symbol operators)
		 * */

		/* NOT: not~ r1, r2
		 * r1 = ~r2
		 * */
		} else if(!strcmp(ins.instruction, "not")) {
			if(ins.arguments[0]!=NULL&&ins.arguments[1]!=NULL) {
				char *dest = mapRegister(ins.arguments[0]);
				char *src = mapRegister(ins.arguments[1]);
				char t = macRegData->prevRegType;
				/*if it is 32-bit(w) we need to extend it back afterwards.*/
				if(!strcmp(src,dest)) {
					if(t=='w') {
						snprintf(outBuf, sizeof(outBuf), 
								"\tmvn %c%s, %c%s\n\tsxtw x%s, w%s\n\tstr %s, [sp]\n", 
								t, src+1, t, src+1, src+1, src+1, src);
						macRegData->prevRegType = 'x';
					} else {
						snprintf(outBuf, sizeof(outBuf), "\tmvn %c%s, %c%s\n", 
								t, src+1, t, src+1);
					}
				} else {
					if(t=='w') {
						snprintf(outBuf, sizeof(outBuf), 
								"\tmvn %c%s, %c%s\n\tsxtw x%s, w%s\n\tstr %s, [sp]\n", 
								t, dest+1, t, src+1, dest+1, dest+1, dest);
						macRegData->prevRegType = 'x';
					} else {
						snprintf(outBuf, sizeof(outBuf), "\tmvn %c%s, %c%s\n", 
								t, dest+1, t, src+1);
					}
				}
			}
		}
	
	
	/*
	 * 3 argument instructions
	 * */
	} else if(args==3) {
		/*
		 * Compare - comp two items and branch if zero
		 * NOTE: same as Jump
		 * */
		if(!strcmp(ins.instruction, "cmpz")) {
			char *arg1 = calloc(strlen(ins.arguments[0])+1, sizeof(char));
			strcpy(arg1, ins.arguments[0]);
			char *arg2 = calloc(strlen(ins.arguments[1])+1, sizeof(char));
			strcpy(arg2, ins.arguments[1]);
			char *arg3 = calloc(strlen(ins.arguments[2])+1, sizeof(char));
			strcpy(arg3, ins.arguments[2]);
			if(checkRegister(arg1)) arg1 = mapRegister(arg1);
			if(checkRegister(arg2)) arg2 = mapRegister(arg2);
			snprintf(outBuf, sizeof(outBuf), "\ttbz %s, %s, _%s\n", arg1, arg2, arg3);
		}


	}




	char *ret = calloc(strlen(outBuf)+1, sizeof(char));
	strcpy(ret, outBuf);
	return ret;
}

