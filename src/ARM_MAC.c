/*Copyright (c) 2024-2026 MinervaWare LLC*/
/*Copyright (c) 2022-2024 Tristan Wellman*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

#include "asmout.h"
#include "cpu.h"
#include "ARM_MAC.h"

#define RESERVEDARGOFFSET 31

MacRegData *macRegData = NULL;

/*
 * Stack initialization is going to be large and unoptimized to local variables/structs.
 * This will be fixed later depending on what we need.
 * */
char *stackAllocateARM_MAC(enum cpuType cpu) {
	char *ret = calloc(1024, sizeof(char));
	CPU = cpu;
	/*auto to 16*/
	sprintf(ret, "\tsub sp, sp, #80\n\tstp x29, x30, [sp, #64]\n\tadd x29, sp, #64\n");
	return ret; 
}

char *stackDeallocateARM_MAC() {
	/*auto to 16*/
	char *ret = calloc(1024, sizeof(char));
	sprintf(ret, "\tldp x29, x30, [sp, #64]\n\tadd sp, sp, #80\n");
	return ret; 
}

/*
 * Choose between standard elf and mach value definitions.
 * */
char *getTargetValVariationARM_MAC(char *value, int operand) {
	int len = strlen(value)+128;
	char *res = calloc(len, sizeof(char));
	if(CPU!=ARM_MAC) { 
		if(operand==2) snprintf(res, sizeof(char)*len, ":lo12:%s", value);
		else snprintf(res, sizeof(char)*len, "%s", value);
	} else {
		if(operand==1) snprintf(res, sizeof(char)*len, "%s@PAGE", value); 
		else if(operand==2) snprintf(res, sizeof(char)*len, "%s@PAGEOFF", value);
		else snprintf(res, sizeof(char)*len, "%s", value);
	}
	return res;
}

void ARM_MACGetLVAlloc(char *buf, int bSize, Variable *var) {
	char *vName = var->varName;
	char *value = var->value;
	int offset = var->offset;
	switch(var->type) {
		case INT: snprintf(buf, bSize,
						  "\tmov x15, %s\n"
						  "\tstr x15, [sp, #%d]\n",
						  value, offset+RESERVEDARGOFFSET);
				  break;
		case CHAR: snprintf(buf, bSize,
						  "\tmov x15, #%d\n"
						  "\tstr x15, [sp, #%d]\n",
						  (int)value[0], offset+RESERVEDARGOFFSET);
				   break;
		case STRING: {
					if(CPU!=ARM_MAC) {
						snprintf(buf, bSize,
								"\tadrp x15, wl_str_%s\n"
								"\tadd x15, x15, :lo12:wl_str_%s\n"
								"\tstr x15, [sp, %d]\n",
								vName, vName, offset+RESERVEDARGOFFSET);
					} else {
						snprintf(buf, bSize,
								"\tadrp x15, wl_str_%s@PAGE\n"
								"\tadd x15, x15, wl_str_%s@PAGEOFF\n"
								"\tstr x15, [sp, %d]\n",
								vName, vName, offset+RESERVEDARGOFFSET);
					}
					break;
		}
		case FLOAT: break;
		case VOID: break;
		case PTR: {
					if(CPU!=ARM_MAC) {
						snprintf(buf, bSize,
								"\tadrp x15, wl_z_%s\n"
								"\tadd x15, x15, :lo12:wl_z_%s\n"
								"\tstr x15, [sp, %d]\n",
								vName, vName, offset+RESERVEDARGOFFSET);
					} else {
						snprintf(buf, bSize,
								"\tadrp x15, wl_z_%s@PAGE\n"
								"\tadd x15, x15, wl_z_%s@PAGEOFF\n"
								"\tstr x15, [sp, %d]\n",
								vName, vName, offset+RESERVEDARGOFFSET);
					}
					break;
		}
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
	char rt = 'x';
	int regNum = regToEnum(reg);	
	ret = calloc(3, sizeof(char));
	switch(type) {
		case CHAR:
		case INT: rt = 'w'; break;
		case FLOAT: rt = 's'; break;
		case STRING: 
		default: rt = 'x'; break;
	};
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
	char *res = NULL;
	Variable *v = NULL;
	char asmVName[1024];
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
			case PTR: snprintf(asmVName, sizeof(asmVName), 
							   "wl_z_%s", ins->arguments[argSpot]);break;
		};
	} else {
		v = NULL;
		v = queryLocalVariable(parser, ins->lineNum, ins->arguments[argSpot]);
		if(v!=NULL) snprintf(asmVName, sizeof(asmVName), "[sp, #%d]", v->offset+RESERVEDARGOFFSET);
		else {
			if(!atoi(ins->arguments[argSpot])&&
					strcmp(ins->arguments[argSpot], "0")) {
				if(CPU!=ARM_MAC) snprintf(asmVName, sizeof(asmVName), "%s", ins->arguments[argSpot]);
				else snprintf(asmVName, sizeof(asmVName), "_%s", ins->arguments[argSpot]);
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
								  "\tadrp %s,%s\n\tldr w%s, [%s, %s]\n",
								  val2, getTargetValVariationARM_MAC(val1, 1), 
								  val2+1, val2, getTargetValVariationARM_MAC(val1, 2));
						  break;
				case CHAR: snprintf(outBuf, sizeof(outBuf), 
								  "\tadrp %s,%s\n\tldrsb w%s, [%s, %s]\n",
								  val2, getTargetValVariationARM_MAC(val1, 1), 
								  val2+1, val2, getTargetValVariationARM_MAC(val1, 2));
						   break;
				case PTR:
				case STRING: snprintf(outBuf, sizeof(outBuf), 
								  "\tadrp %s,%s\n\tadd %s, %s, %s\n",
								  val2, getTargetValVariationARM_MAC(val1, 1), 
								  val2, val2, getTargetValVariationARM_MAC(val1, 2));
							 break;
				case FLOAT: snprintf(outBuf, sizeof(outBuf), 
								  "\tadrp %s,%s\n\tldr s%s, [%s, %s]\n",
								  val2, getTargetValVariationARM_MAC(val1, 1), 
								  val2+1, val2, getTargetValVariationARM_MAC(val1, 2));
							break;
				default: snprintf(outBuf, sizeof(outBuf), 
								 "\tadrp %s,%s\n\tadd %s, %s, %s\n",
								 val2, getTargetValVariationARM_MAC(val1, 1), 
								 val2, val2, getTargetValVariationARM_MAC(val1, 2));
						 break;
			};
		}
	} else {
		var = queryLocalVariable(parser, ins->lineNum, ins->arguments[0]);
		if(var) {
			snprintf(outBuf, sizeof(outBuf),
					"\tldr %s, [sp, #%d]\n", val2, var->offset+RESERVEDARGOFFSET);
		} else {
			if(checkRegister(ins->arguments[0])&&
					!checkRegister(ins->arguments[1])) {
				var = queryLocalVariable(parser, ins->lineNum, ins->arguments[1]);
				if(var) {
					snprintf(outBuf, sizeof(outBuf), 
							"\tstr %s, [sp, #%d]\n", val1, var->offset+RESERVEDARGOFFSET);
				}
			} else if(checkRegister(ins->arguments[0])&&
					checkRegister(ins->arguments[1])) {
				snprintf(outBuf, sizeof(outBuf), 
						"\tmov %s, %s\n", val2, val1);
			} else {
				snprintf(outBuf, sizeof(outBuf), 
						"\tadrp %s,%s\n\tadd %s, %s, %s\n",
						val2, getTargetValVariationARM_MAC(val1, 1), 
						val2, val2, getTargetValVariationARM_MAC(val1, 2));
			}
		}
	}
	res = calloc(strlen(outBuf)+1, sizeof(char));
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
		snprintf(outBuf, sizeof(outBuf), "\tldr x15, %s\n"
										 "\tcmp x15, %s\n"
										 "\t%s .%s\n.%s_cont:\n", 
										 val1, val2, op, scopeName, scopeName);
	} else if(val2[0]=='[') {
		snprintf(outBuf, sizeof(outBuf), "\tldr x15, %s\n"
										 "\tcmp x15, %s\n"
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

char *ARM_MACInitializeExternStackData(ExternData *data) {
	char *ret;
	char outBuf[DEFMAXFSIZE] = "";	
	unsigned int argSize;
	int i = 0;
	int j = 0;
	int offset = 0;
	if(!data) return "";
	argSize = data->argSize;
	for(;i<argSize;i++) {
		if(data->argTypes[i]==ANY) {
			for(j=i;j<7;j++) { /*every register after the variadic is included*/
				char buf[1024];
				snprintf(buf, sizeof(buf), "\tstr x%d, [sp, #%d]\n", j, offset);
				strcat(outBuf, buf);
				offset += 8;
			}
		}
	}
	ret = calloc(strlen(outBuf)+1, sizeof(char));
	strcpy(ret, outBuf);
	return ret;
}

void insertDefaultMappedArm3Instruction(char *outBuf, int outlen, char *instruction, Instruction *ins) {
	if(!ins||!outBuf) return;
	if(ins->arguments[0]!=NULL&&ins->arguments[1]!=NULL&&
			ins->arguments[2]!=NULL) {
		char *dest = mapRegister(ins->arguments[0]);
		char *s1 = mapRegister(ins->arguments[1]);
		char *s2 = mapRegister(ins->arguments[2]);
		snprintf(outBuf, sizeof(char)*outlen, "\t%s %s, %s, %s\n",
				instruction, dest, s1, s2, dest, dest);
	}
}

void insertDefaultMappedArm3InstructionWithIdempotency(char *outBuf, int outlen,
		char *instruction, short idempotency, Instruction *ins) {
	if(!ins||!outBuf) return;
	if(ins->arguments[0]!=NULL&&ins->arguments[1]!=NULL&&
			ins->arguments[2]!=NULL) {
		char *dest = mapRegister(ins->arguments[0]);
		char *s1 = mapRegister(ins->arguments[1]);
		char *s2 = mapRegister(ins->arguments[2]);
		if((!strcmp(dest,s1)&&!strcmp(dest,s2))||!strcmp(s1,s2)) {
			snprintf(outBuf, sizeof(char)*outlen, "\tmov %s, #%d\n", dest, idempotency);
		} else {
			snprintf(outBuf, sizeof(char)*outlen, "\t%s %s, %s, %s\n",
					instruction, dest, s1, s2);
		}
	}
}

/*x registers are 64-bit w registers are 32*/
char *convertInstructionARM_MAC(AsmOut *out, Instruction ins) {
	int args = ins.argLen;
	int outlen = args*DEFMAXFSIZE;
	char *outBuf = calloc(outlen, sizeof(char));
	int i;
	char *ret;
	if(macRegData==NULL) {
		macRegData = calloc(1, sizeof(MacRegData));
		macRegData->prevRegType = 'x';
	}
	CPU = out->parser->fData->cpu;

	for(i=0;i<args;i++) WTRIM(ins.arguments[i]);

	/*Special instructions*/
	/*Inline - Drops direct asm instructions into the output*/
	if(!strcmp(ins.instruction, "inline")) { 
		snprintf(outBuf, sizeof(char)*outlen, "\t%s\n", dumpInlineASM(&ins));
	

	/*
	 * 0 argument instructions
	 * */
	} else if(args==0) {
		/*
		 * Jump - Jump to or from function 
		 * NOTE: This is "temporary" until I get if statements and loops going
		 * */
		if(!strcmp(ins.instruction, "jump")) {
			snprintf(outBuf, sizeof(char)*outlen, "\tb _%s\n", ins.arguments[0]);
		}

	/*
	 * 1 argument instructions
	 * */
	} else if(args==1) {
		/*
		 * Call
		 * */
		if(!strcmp(ins.instruction, "call")) {
			if(ins.argLen>0 && ins.arguments[0]!=NULL) {
				if(CPU!=ARM_MAC) snprintf(outBuf, sizeof(char)*outlen, "\tbl %s\n", ins.arguments[0]);
				else if(!doesFunctionExistInternal(out->parser, ins.arguments[0])) {
					char *init = ARM_MACInitializeExternStackData(
							getExternalData(out->parser, ins.arguments[0]));
					snprintf(outBuf, sizeof(char)*outlen, "%s\tbl _%s\n", init, ins.arguments[0]);
				} else snprintf(outBuf, sizeof(char)*outlen, "\tbl %s\n", ins.arguments[0]);
			}
		/*
		 * Return
		 * */
		} else if(!strcmp(ins.instruction, "return")) {
			if(ins.arguments[0]!=NULL) {
				char *reg = NULL;
				char *dealloc = stackDeallocateARM_MAC();
				if(strlen(ins.arguments[0])==0) ins.arguments[0] = "0";
				else reg = mapRegister(ins.arguments[0]);
				if(reg!=NULL) {
					snprintf(outBuf, sizeof(char)*outlen, "\tmov x0, %s\n%s\tret\n",
							reg, dealloc);
				} else {
					snprintf(outBuf, sizeof(char)*outlen, "\tmov x0, #%s\n%s\tret\n",
							ins.arguments[0], dealloc);
				}
			}

		 /*
		  * SStruct - save structure from return.
		  * NOTE: this is sort of patch work so I can manage C lib structs easier
		  * */
		} else if(!strcmp(ins.instruction, "sstruct")) {
			char *reg = mapRegister(ins.arguments[0]);
			snprintf(outBuf, sizeof(char)*outlen, 
					"\tstr %s, [sp, #8]\n", reg);
		
		/*
		 * LStruct - Load returned C struct into register
		 * NOTE: this is sort of patch work so I can manage C lib structs easier
		 * */
		} else if(!strcmp(ins.instruction, "lstruct")) {
			char *reg = mapRegister(ins.arguments[0]);
			snprintf(outBuf, sizeof(char)*outlen, "\tldr %s, [sp, #8]\n", reg);
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

			mov = ARM_MACgetMoveInstructions(out->parser, &ins, val1, val2);
			if(mov==NULL) mov = "";
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
						snprintf(outBuf, sizeof(char)*outlen, 
								"\tmvn %c%s, %c%s\n\tsxtw x%s, w%s\n\tstr %s, [sp]\n", 
								t, src+1, t, src+1, src+1, src+1, src);
						macRegData->prevRegType = 'x';
					} else {
						snprintf(outBuf, sizeof(char)*outlen, "\tmvn %c%s, %c%s\n", 
								t, src+1, t, src+1);
					}
				} else {
					if(t=='w') {
						snprintf(outBuf, sizeof(char)*outlen, 
								"\tmvn %c%s, %c%s\n\tsxtw x%s, w%s\n\tstr %s, [sp]\n", 
								t, dest+1, t, src+1, dest+1, dest+1, dest);
						macRegData->prevRegType = 'x';
					} else {
						snprintf(outBuf, sizeof(char)*outlen, "\tmvn %c%s, %c%s\n", 
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
			snprintf(outBuf, sizeof(char)*outlen, "%s", conv);
			free(conv); conv = NULL;
		
		/* AND: and~ r1, r2, r3
		 * r1 = r2&r3
		 * */
		} else if(!strcmp(ins.instruction, "and")) {
			insertDefaultMappedArm3InstructionWithIdempotency(outBuf, sizeof(char)*outlen,
					"and", 1, &ins);
		
		/* OR: or~ r1, r2, r3
		 * r1 = r2|r3
		 * */
		} else if(!strcmp(ins.instruction, "or")) {
			insertDefaultMappedArm3InstructionWithIdempotency(outBuf, sizeof(char)*outlen,
					"orr", 1, &ins);

		/* NOR: nor~ r1, r2, r3
		 * r1 = !(r2|r3)
		 * */
		} else if(!strcmp(ins.instruction, "nor")) {
			if(ins.arguments[0]!=NULL&&ins.arguments[1]!=NULL&&
					ins.arguments[2]!=NULL) {
				char *dest = mapRegister(ins.arguments[0]);
				char *s1 = mapRegister(ins.arguments[1]);
				char *s2 = mapRegister(ins.arguments[2]);
				if((!strcmp(dest,s1)&&!strcmp(dest,s2))||!strcmp(s1,s2)) {
					snprintf(outBuf, sizeof(char)*outlen, "\tmov %s, #0\n", dest);
				} else {
					snprintf(outBuf, sizeof(char)*outlen, "\torr %s, %s, %s\n\tmvn %s, %s\n",
							dest, s1, s2, dest, dest);
				}
			}

		/* NAND: nand~ r1, r2, r3
		 * r1 = ~(r2&r3)
		 * */
		} else if(!strcmp(ins.instruction, "nand")) {
			if(ins.arguments[0]!=NULL&&ins.arguments[1]!=NULL&&
					ins.arguments[2]!=NULL) {
				char *dest = mapRegister(ins.arguments[0]);
				char *s1 = mapRegister(ins.arguments[1]);
				char *s2 = mapRegister(ins.arguments[2]);
				if((!strcmp(dest,s1)&&!strcmp(dest,s2))||!strcmp(s1,s2)) {
					snprintf(outBuf, sizeof(char)*outlen, "\tmov %s, #0\n", dest);
				} else {
					snprintf(outBuf, sizeof(char)*outlen, "\tand %s, %s, %s\n\tmvn %s, %s\n",
							dest, s1, s2, dest, dest);
				}
			}
		
		/* XOR: xor~ r1, r2, r3
		 * r1 = r2^r3
		 * */
		} else if(!strcmp(ins.instruction, "xor")) {
			insertDefaultMappedArm3Instruction(outBuf, sizeof(char)*outlen, "eor", &ins);

		/*
		 * Mathematical Instructions
		 * */
		/* ADD: add~ r1, r2, r3
		 * r1 = r2+r3
		 * */
		} else if(!strcmp(ins.instruction, "add")) {
			insertDefaultMappedArm3Instruction(outBuf, sizeof(char)*outlen, "add", &ins);
			
		/* SUB: sub~ r1, r2, r3
		 * r1 = r2-r3
		 * */
		} else if(!strcmp(ins.instruction, "sub")) {
			insertDefaultMappedArm3Instruction(outBuf, sizeof(char)*outlen, "subs", &ins);
		
		/* MUL: mul~ r1, r2, r3
		 * r1 = r2*r3
		 * */
		} else if(!strcmp(ins.instruction, "mul")) {
			insertDefaultMappedArm3Instruction(outBuf, sizeof(char)*outlen, "mul", &ins);

		/* DIV: div~ r1, r2, r3
		 * r1 = r2/r3
		 * */
		} else if(!strcmp(ins.instruction, "div")) {
			insertDefaultMappedArm3Instruction(outBuf, sizeof(char)*outlen, "sdiv", &ins);

		/* MOD: mod~ r1, r2, r3
		 * r1 = r2%r3
		 * */
		} else if(!strcmp(ins.instruction, "mod")) {
			if(ins.arguments[0]!=NULL&&ins.arguments[1]!=NULL&&
					ins.arguments[2]!=NULL) {
				char *dest = mapRegister(ins.arguments[0]);
				char *s1 = mapRegister(ins.arguments[1]);
				char *s2 = mapRegister(ins.arguments[2]);
				snprintf(outBuf, sizeof(char)*outlen, 
						"\tsdiv %s, %s, %s\n\tmul %s, %s, %s\n\tsub %s, %s, %s\n",
						dest, s1, s2, dest, dest, dest, dest, s2, dest, s1, dest);
			}
		}

	}

	ret = calloc(strlen(outBuf)+1, sizeof(char));
	strcpy(ret, outBuf);
	free(outBuf);
	return ret;
}

