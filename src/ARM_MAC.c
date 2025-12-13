/*Copyright (c) 2024 Tristan Wellman*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
/*Copyright (c) 2022-2025 MinervaWare LLC*/

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
	sprintf(ret, "\tsub sp, sp, #32\n\tstp x29, x30, [sp, #32]\n\tadd x29, sp, #32\n");
	return ret; 
}

char *stackDeallocateARM_MAC() {
	/*auto to 16*/
	char *ret = calloc(1024, sizeof(char));
	sprintf(ret, "\tldp x29, x30, [sp, #32]\n\tadd sp, sp, #32\n");
	return ret; 
}

void ARM_MACGetLVAlloc(char *buf, int bSize, Variable *var) {
	char *vName = var->varName;
	char *value = var->value;
	int offset = var->offset;
	switch(var->type) {
		case INT: snprintf(buf, bSize,
						  "\tmov x28, %s\n"
						  "\tstr x28, [sp, #%d]\n",
						  value, offset);
				  break;
		case CHAR: snprintf(buf, bSize,
						  "\tmov x28, #%d\n"
						  "\tstr x28, [sp, #%d]\n",
						  (int)value[0], offset);
				   break;
		case STRING: snprintf(buf, bSize,
							 "\tadrp x28, wl_str_%s@PAGE\n"
							 "\tadd x28, x28, wl_str_%s@PAGEOFF\n"
							 "\tstr x28, [sp, %d]\n",
							 vName, vName, offset);
					 break;
		case FLOAT: break;
		case VOID: break;
		case ZERO: break;
	};
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

char *ARM_MACgetCurrentVar(struct parserData *parser, Instruction *ins, int argSpot) {
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
		v = NULL;
		v = queryLocalVariable(parser, ins->lineNum, ins->arguments[argSpot]);
		if(v!=NULL) snprintf(asmVName, sizeof(asmVName), "[sp, %d]", v->offset);
		else {
			if(!atoi(ins->arguments[argSpot])&&
					strcmp(ins->arguments[argSpot], "0")) {
				snprintf(asmVName, sizeof(asmVName), "_%s", ins->arguments[argSpot]);
			} else {
				snprintf(asmVName, sizeof(asmVName), "#%s", ins->arguments[argSpot]);
			}
		}
	}
	res = calloc(strlen(asmVName)+1, sizeof(char));
	strcpy(res, asmVName);
	return res;
}

char *ARM_MACgetMoveInstructions(struct parserData *parser, Instruction *ins, 
		char *val1, char *val2) {
	char *res = NULL;
	char outBuf[1024];
	Variable *var = getVarFrom(parser, ins->arguments[0]);
	if(var) {
		if(var->varName!=NULL) {
			switch(var->type) {
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
		}
	} else {
		var = queryLocalVariable(parser, ins->lineNum, ins->arguments[0]);
		if(var) {
			snprintf(outBuf, sizeof(outBuf),
					"\tldr %s, [sp, #%d]\n", val2, var->offset);
		} else {
			if(checkRegister(ins->arguments[0])&&
					!checkRegister(ins->arguments[1])) {
				var = queryLocalVariable(parser, ins->lineNum, ins->arguments[1]);
				if(var) {
					snprintf(outBuf, sizeof(outBuf), 
							"\tstr %s, [sp, #%d]\n", val1, var->offset);
					printf("%s\n", outBuf);
				}
			} else if(checkRegister(ins->arguments[0])&&
					checkRegister(ins->arguments[1])) {
				snprintf(outBuf, sizeof(outBuf), 
						"\tmov %s, %s\n", val2, val1);
			} else {
				snprintf(outBuf, sizeof(outBuf), 
						"\tadrp %s,%s@PAGE\n\tadd %s, %s, %s@PAGEOFF\n",
						val2, val1, val2, val2, val1);
			}
		}
	}
	res = calloc(strlen(outBuf), sizeof(char));
	strcpy(res, outBuf);
	return res;
}

char *getIfStateCmpARM_MAC(char *arg) {
	if(!strcmp(arg, "je")) return "beq";
	else if(!strcmp(arg, "jne")) return "bne";
	else if(!strcmp(arg, "jle")) return "ble";
	else if(!strcmp(arg, "jge")) return "bge";
	return NULL;
}

char *ifStateConvertARM_MAC(AsmOut *out, Instruction *ins) {
	char *ret = NULL;
	char outBuf[DEFMAXFSIZE];

	char *val1 = NULL;
	char *val2 = NULL;
	char *scopeName = ins->arguments[0];
	char *op = getIfStateCmpARM_MAC(ins->instruction);
	if(checkRegister(ins->arguments[1])) {
		char *ARMReg = mapRegister(ins->arguments[1]);
		val1 = calloc(strlen(ARMReg)+128, sizeof(char));
		strcpy(val1, ARMReg);
	} else {
		char *var = ARM_MACgetCurrentVar(out->parser, ins, 1);
		val1 = calloc(strlen(var)+1, sizeof(char));
		strcpy(val1, var);
		free(var); var = NULL;
	}
	if(checkRegister(ins->arguments[2])) {
		char *ARMReg = mapRegister(ins->arguments[2]);
		val2 = calloc(strlen(ARMReg)+128, sizeof(char));
		strcpy(val2, ARMReg);
	} else {
		char *var = ARM_MACgetCurrentVar(out->parser, ins, 2);
		val2 = calloc(strlen(var)+1, sizeof(char));
		strcpy(val2, var);
		free(var); var = NULL;
	}
	if(val1[0]=='[') { /*Local var*/
		snprintf(outBuf, sizeof(outBuf), "\tldr x28, %s\n"
										 "\tcmp x28, %s\n"
										 "\t%s .%s\n.%s_cont:\n", 
										 val1, val2, op, scopeName, scopeName);
	} else if(val2[0]=='[') {
		snprintf(outBuf, sizeof(outBuf), "\tldr x28, %s\n"
										 "\tcmp x28, %s\n"
										 "\t%s .%s\n.%s_cont:\n", 
										 val2, val1, op, scopeName, scopeName);
	} else {
		snprintf(outBuf, sizeof(outBuf), "\tcmp %s, %s\n"
										 "\t%s .%s\n"
										 ".%s_cont:\n",
										 val1, val2, op, scopeName, scopeName);
	}
	if(val1) {free(val1);val1=NULL;}
	if(val2) {free(val2);val2=NULL;}
	ret = calloc(strlen(outBuf)+1, sizeof(char));
	strcpy(ret, outBuf);
	return ret;
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
	if(!strcmp(ins.instruction, "inline")) { 
		snprintf(outBuf, sizeof(outBuf), "\t%s\n", dumpInlineASM(&ins));
	

	/*
	 * 0 argument instructions
	 * */
	} else if(args==0) {
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
				char *var = ARM_MACgetCurrentVar(out->parser, &ins, 0);
				val1 = calloc(strlen(var)+1, sizeof(char));
				strcpy(val1, var);			}
			if(checkRegister(ins.arguments[1])) {
				char *ARMReg = mapRegister(ins.arguments[1]);
				val2 = calloc(strlen(ARMReg)+1, sizeof(char)); 
				strcpy(val2, ARMReg);
			} else {
				char *var = ARM_MACgetCurrentVar(out->parser, &ins, 1);
				val2 = calloc(strlen(var)+1, sizeof(char));
				strcpy(val2, var);
			}

			char *mov = ARM_MACgetMoveInstructions(out->parser, &ins, val1, val2);
			strcpy(outBuf, mov);
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
		 * Compiler statement related instructions, I.E. If state or loops.
		 * These statements are generated by the parser.
		 *
		 * Users can also use these if they decide to opt out of using ~if or ~loop.
		 * This would look like:
		 * jne~ function, arg1, arg2
		 * */
		if(getIfStateCmpARM_MAC(ins.instruction)!=NULL) {
			char *conv = ifStateConvertARM_MAC(out, &ins);
			snprintf(outBuf, sizeof(outBuf), "%s", conv);
			free(conv); conv = NULL;
		}
	}




	char *ret = calloc(strlen(outBuf)+1, sizeof(char));
	strcpy(ret, outBuf);
	return ret;
}

