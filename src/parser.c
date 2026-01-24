/*Copyright (c) 2022-2025 MinervaWare LLC*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "asmout.h"
#include "werror.h"

struct parserData *gPData;

/*Private utility protos*/
int checkImportantType(char *line);
enum wTypes getScopeType(char *line);
char *getWT(enum wTypes t);
char *getScopeName(char *line);
Scope getScopeAt(int lineNum, struct parserData *parser);
Scope getNextScope(Scope scope, struct parserData *parser);
Variable *getVarFrom(struct parserData *parser, char *name);
Variable *queryLocalVariable(struct parserData *parser, 
		int lineNum, char *var);
char *querySubFuncScopeName(Function *func, int lineNum);

/*
 * Checks if line contains important type:
 * EX: ~constants or ~type:function 
 * */
int checkImportantType(char *line) {
	int i;
	if(line==NULL||*line=='\0') return 0;
	for(i=0;i<strlen(line)&&line[i]!='\0';i++) {
		/* 
		 * ~ is the special sensor for types or instructions
		 * ~... is a spectial type,
		 * ...~ is a special instruction
		 * */
		if(line[i]=='~'&&(line[i+1]!='\0'&&line[i+1]!=' '))
			return 1;
	}
	return 0;
}


/* * * * *
 * Scope related functions!
 * * * * */


enum wTypes getScopeType(char *line) {
	int i, col=0, curl=0;
	if(strstr(line, "~constants")) return CONSTANTS;
	if(strstr(line, "~extern")) return EXTERN;
	if(strstr(line, "~include")) return INCLUDE;
	for(i=0;line[i]!='\0';i++) {
		if(line[i]==':') col=1;
		if(line[i]=='{') curl=1;
	}
	if(curl&&!col) {
		if(strstr(line, "if")) return IFSTATE;
		if(strstr(line, "while")||
				strstr(line, "for")) return LOOP;
	}

	if(col&&curl) return FUNCTION;
	if(col&&!curl) return VARIABLE;

	return NONE;
}

char *getWT(enum wTypes t) {
	switch(t) {
		case NONE: return "NONE";
		case INCLUDE: return "INCLUDE";
		case FUNCTION: return "FUNCTION";
		case IFSTATE: return "IFSTATE";
		case LOOP: return "LOOP";
		case VARIABLE: return "VARIABLE";
		case CONSTANTS: return "CONSTANTS";
		case EXTERN: return "EXTERN";
	}
	return "";
}

char *getScopeName(char *line) {
	char *tmp = calloc(strlen(line)+1, sizeof(char));
	char *aftrcurl;
	strcpy(tmp, line);
	aftrcurl = strstr(tmp, "~");
	if(aftrcurl!=NULL) {
		char *name;
		aftrcurl++;
		if((name = strstr(aftrcurl, ":"))&&name!=NULL) {
			name++;
			name = strtok(name, " ");
			if(name!=NULL) return name;
		} else if((name = strtok(aftrcurl, " {"))&&name!=NULL) return name;
	}

	return NULL;
}

void dumpScopes(struct parserData *parser) {
	int i;
	for(i=0;i<MAXSCOPES;i++) {
		char buffer[100];
		if(parser->scopes[i].scopeName==NULL) break;
		snprintf(buffer, sizeof(buffer), 
				"%s : %s : %s",
				getWT(parser->scopes[i].scopeType), 
				parser->scopes[i].scopeName,
				parser->fileBuffer[parser->scopes[i].lineNum]);
		WLOG(INFO, buffer);
	}
}

void getScopes(struct parserData *parser) {
	int i, j=0;

	for(i=0;i<parser->bufferSize;i++) {
		char *currentLine = parser->fileBuffer[i];
		if(currentLine!=NULL&&
				checkImportantType(currentLine)) {
			char *name = getScopeName(currentLine);
			Scope s; 
			s.scopeType = getScopeType(currentLine), 
			s.scopeName = NULL,
			s.lineNum = i;
			s.scopeName = calloc(strlen(name)+1, sizeof(char));
			strcpy(s.scopeName, name);
			if(s.scopeType == NONE) continue;
			parser->scopes[j] = s;
			j++;
		}
	}
}

Scope getScopeAt(int lineNum, struct parserData *parser) {
	int i;
	Scope r = {0};
	for(i=0;i<MAXSCOPES&&parser->scopes[i].scopeName!=NULL;i++) {
		if(lineNum==parser->scopes[i].lineNum) return parser->scopes[i];
	}
	return r;
}

Scope getNextScope(Scope scope, struct parserData *parser) {
	int i;
	Scope r = {0};
	for(i=0;i<MAXSCOPES&&parser->scopes[i].scopeName!=NULL;i++) {
		if(scope.scopeName==parser->scopes[i].scopeName&&
				scope.lineNum==parser->scopes[i].lineNum) break;
	}
	for(i=i+1;i<MAXSCOPES&&parser->scopes[i].scopeName!=NULL;i++) {
		if(parser->scopes[i].scopeType!=IFSTATE&&
				parser->scopes[i].scopeType!=LOOP) return parser->scopes[i];
	}
	return r;
}


/*
 * Variable related functions
 * */

void dumpVariables(struct parserData *parser) {
	int i;
	for(i=0;i<parser->totalVariables;i++) {
		printf("%s:%s\n", parser->variables[i].varName,
				parser->variables[i].value);
	}
}

void setVariableType(Variable *var, char *type,
		int lineNum, char *file) {
	if(!strcmp(type, "string")) {
		var->type = STRING; return;
	} else if(!strcmp(type, "char")) {
		var->type = CHAR; return;
	} else if(!strcmp(type, "int")) {
		var->type = INT; return;
	} else if(!strcmp(type, "float")) {
		var->type = FLOAT; return;
	} else if(!strcmp(type, "void")) {
		var->type = VOID; return;
	} else if(!strcmp(type, "zero")) {
		var->type = ZERO; return;
	}
	WLOG_WERROR(WERROR_UNDEFINED_TYPE,
			file, lineNum, "constants", "");
}

Variable *getVarFrom(struct parserData *parser, char *name) {
	int i;
	for(i=0;i<parser->totalVariables&&parser->variables[i].varName!=NULL;i++) {
		if(!strcmp(parser->variables[i].varName, name))
			return &parser->variables[i];
	}
	return NULL;
}

/*This is expected to be ran AFTER scopes have been initialized*/
void getVariables(struct parserData *parser) {
	int i, j=0;
	static unsigned int inScope = 0;
	parser->variables = calloc(1, sizeof(Variable));
	parser->totalVariables = 0;

	for(i=0;i<MAXSCOPES;i++) {
		if(parser->scopes[i].scopeName==NULL) break;
		if(parser->scopes[i].scopeType == CONSTANTS) inScope=1;
		if(parser->scopes[i].scopeType < VARIABLE) inScope = 0;
		if(parser->scopes[i].scopeType == VARIABLE && inScope) {
			char *line, *data;	
			parser->variables = (Variable *)realloc(parser->variables, 
					sizeof(Variable)*(parser->totalVariables+1));
			line = parser->fileBuffer[parser->scopes[i].lineNum];
			if(line==NULL) continue;
			if(strstr(line, "}")) break;
			data = strstr(line, "=");
			if(data==NULL) {
 				WLOG_WERROR(WERROR_UNINITIALIZED_VARIABLE,
						parser->fData->fileName, 
						parser->scopes[i].lineNum, "constants", "");	
			} else {
				char *tmp, *type;
				data++;
				if(data[strlen(data)-1]=='\n') data[strlen(data)-1] = '\0';
				while(data[0]==' ') data++;
				parser->variables[j].value = calloc(strlen(data)+1, sizeof(char));
				strcpy(parser->variables[j].value, data);
				/*save data*/
				tmp  = calloc(strlen(line)+1, sizeof(char));
				strcpy(tmp, line);
				
				/*type*/
				type = strstr(line, "~");
				type++;
				type = strtok(type, ":");
				setVariableType(&parser->variables[j], type,
						parser->scopes[i].lineNum, parser->fData->fileName);

				if(parser->variables[j].type!=ZERO) {
					char *name = strstr(tmp, ":");
					name++;	
					name = strtok(name, "=");
					while(name[strlen(name)-1]==' ') name[strlen(name)-1] = '\0';
					parser->variables[j].varName = calloc(strlen(name)+1, sizeof(char));
					strcpy(parser->variables[j].varName, name);
					parser->totalVariables++;
					j = parser->totalVariables;
				} else {
					/*Different syntax for zero initialized areas*/
					char *bytes = strstr(tmp, ":");
					bytes++;
					bytes = strtok(bytes, ":");
					while(bytes[strlen(bytes)-1]==' ') bytes[strlen(bytes)-1] = '\0';
					
				}
				free(tmp);tmp=NULL;
			}
		}
	}
}

Variable *queryLocalVariable(struct parserData *parser, 
		int lineNum, char *var) {
	int i;
	Variable *res = NULL;
	Function *func = NULL;
	if(var==NULL||parser==NULL) return res;
	if(parser->totalFunctions>2) {
		for(i=2;i<parser->totalFunctions;i++) {
			Function *func3 = &parser->functions[i-2];
			Function *func2 = &parser->functions[i];
			func = &parser->functions[i-1];
			if(lineNum>func3->scope.lineNum&&
					lineNum<func2->scope.lineNum) break;
		}
		if(parser->totalFunctions%2==0&&i==parser->totalFunctions)
			func = &parser->functions[i-1];
	} else if(parser->totalFunctions==2) {
		if(lineNum>parser->functions[1].scope.lineNum)
			func = &parser->functions[1];
		else func = &parser->functions[0];
	} else func = &parser->functions[0];
	if(func==NULL) return res;
	for(i=0;i<func->lvt->totalVariables;i++) {
		if(!strcmp(var,func->lvt->variables[i].varName))
			res = &func->lvt->variables[i];
	}
	return res;
}



char *LVTGetType(char *line) {
	char *lcpy, *type;
	if(line==NULL) return NULL;
	lcpy = calloc(strlen(line)+1, sizeof(char));
	strcpy(lcpy, line);
	type = NULL;
	type = strstr(lcpy, "~");
	type++;
	type = strtok(type, ":");
	while(type[0]==' ') type++;
	while(type[strlen(type)-1]==' ') type[strlen(type)-1] = '\0';
	return type;
}

char *LVTGetValue(char *line) {
	char *lcpy, *value;
	if(line==NULL) return NULL;
	lcpy = calloc(strlen(line)+1, sizeof(char));
	strcpy(lcpy, line);
	value = NULL;
	value = strstr(lcpy, "=");
	value++;
	while(value[0]==' ') value++;
	while(value[strlen(value)-1]==' '||
			value[strlen(value)-1]=='\n') value[strlen(value)-1] = '\0';
	return value;
}

char *LVTGetVName(char *line) {
	char *lcpy, *name;
	if(line==NULL) return NULL;
	lcpy = calloc(strlen(line)+1, sizeof(char));
	strcpy(lcpy, line);
	name = NULL;
	name = strstr(lcpy, ":");
	name++;
	name = strtok(name, "=");
	while(name[0]==' ') name++;
	while(name[strlen(name)-1]==' ') name[strlen(name)-1] = '\0';
	return name;
}

/*This will be changed once bitfields are added*/
int LVTGetOffsetSize(enum varTypes type) {
	switch(type) {
		case INT: return sizeof(int64_t);
		case CHAR: return sizeof(char);
		case STRING: return sizeof(char *) /*8*/;
		case FLOAT: return sizeof(float);
		case VOID: return 4;
		case ZERO: return 4;
	};	
	return 0;
}

void buildFunctionLVT(Function *func) {
	int curOffset = 0;
	int i, *j;
	if(func==NULL) return;
	if(func->lvt==NULL) {
		func->lvt = calloc(1, sizeof(LVT));
		func->lvt->variables = NULL;
		func->lvt->totalVariables = 0;
		func->lvt->cap = DEFALLOCSTEP;
	}
	if(func->lvt->variables==NULL) {
		func->lvt->variables = calloc(DEFALLOCSTEP, sizeof(Variable));	
	}
	j = &func->lvt->totalVariables;
	for(i=0;i<func->dataLength;i++) {
		if(func->data[i]==NULL) continue;
		if(checkImportantType(func->data[i])) {
			if(getScopeType(func->data[i])==VARIABLE) {
				char *type, *name, *value;
				if(*j>func->lvt->cap) {
					func->lvt->cap += DEFALLOCSTEP;
					func->lvt->variables = (Variable *)realloc(func->lvt->variables,
							sizeof(Variable)*func->lvt->cap);
				}
				type = LVTGetType(func->data[i]);
				name = LVTGetVName(func->data[i]);
				value = LVTGetValue(func->data[i]);
				if(value==NULL) func->lvt->variables[*j].value = NULL;
				if(type==NULL) {
					WLOG_WERROR(WERROR_UNDEFINED_TYPE,
							gPData->fData->fileName, 
							func->scope.lineNum+i, 
							func->funName, "Fix your locals buster");
				}	
				if(name==NULL) {
					WLOG_WERROR(WERROR_INVALID_SYNTAX,
							gPData->fData->fileName, 
							func->scope.lineNum+i, 
							func->funName, "Give your local variable a name!");
				}
				setVariableType(&func->lvt->variables[*j], type,
						func->scope.lineNum+i, gPData->fData->fileName);
				func->lvt->variables[*j].varName = calloc(strlen(name)+1, sizeof(char));
				strcpy(func->lvt->variables[*j].varName, name);
				func->lvt->variables[*j].value = calloc(strlen(value)+1, sizeof(char));
				strcpy(func->lvt->variables[*j].value, value);

				curOffset += LVTGetOffsetSize(
						func->lvt->variables[*j].type);
				func->lvt->variables[*j].offset = curOffset;
				*j = *j+1;
			}
		}	
	}
}


/* * * * *
 * Instruction related functions.
 * * * * */
void appendInsArgs(Instruction *ins, char *arg) {
	while(arg[0]==' ') arg++;
	while(arg[strlen(arg)-1]=='\n'||
               arg[strlen(arg)-1]=='\r') arg[strlen(arg)-1] = '\0';
	if(ins->argLen>=ins->capacity) {
		size_t newCapacity = ins->capacity==0 ? DEFAULTINSARGSIZE : ins->capacity*2;
		char **newArgs = (char **)realloc(ins->arguments, sizeof(char *)*newCapacity);
		ins->arguments = newArgs;
		ins->capacity = newCapacity;
	}
	ins->arguments[ins->argLen] = calloc(strlen(arg)+1, sizeof(char));
	strcpy(ins->arguments[ins->argLen], arg);
	ins->argLen++;

}

void getInstructionArguments(Instruction *ins) {
	if(ins->line!=NULL) {
		char *backup, *ignoreIns;
		ins->argLen = 0;
		ins->capacity = DEFAULTINSARGSIZE;
		ins->arguments = calloc(ins->capacity, sizeof(char *));
		backup = calloc(strlen(ins->line)+1, sizeof(char));
		strcpy(backup, ins->line);
		ignoreIns = strstr(backup, "~");
		if(ignoreIns!=NULL) {
			char *arg;
			ignoreIns++;
			arg = strtok(ignoreIns, ",");
			while(arg!=NULL) {
				int i;
				for(i=0;i<strlen(arg);i++) {
					if(arg[i]=='\n') arg[i] = '\0';
				}
				appendInsArgs(ins, arg);
				arg = strtok(NULL, ",");
			}
		}
		free(backup);backup=NULL;
	}
	if(ins->argLen==0) ins->arguments = NULL;
}

void disectInstructionName(Instruction *ins) {
	if(ins->line!=NULL) {
		char *backup = calloc(strlen(ins->line)+1, sizeof(char));
		char *name;
		strcpy(backup, ins->line);
		EATTABS(backup);
		/*move~ text, r1*/

		name = strtok(backup, "~");
		if(name!=NULL) {
			ins->instruction = calloc(strlen(name)+1, sizeof(char));
			strcpy(ins->instruction, name);
		}

		free(backup);backup=NULL;
	}
}

/*func goes unused, it's there to follow my parse standard for a WPFUNC*/
void parseInstruction(Function *func, char *line, Instruction *ins) {
	if(!line||!ins) return;
	disectInstructionName(ins);
	getInstructionArguments(ins);
}

/* * * * *
 * IFSTATE & LOOP related functions
 * * * * */

int isChIfStateOp(char c) {
	/*TODO add ops like |, &, ^, etc.*/
	if(c=='='||c=='!'||
			c=='>'||c=='<') return 1;
	return 0;
}

/*This is just converting stuff like "==" into "je"*/
char *convertIfStateOp(char *op) {
	if(!strcmp(op, "==")) return "je";
	else if(!strcmp(op, "!=")) return "jne";
	else if(!strcmp(op, "<=")) return "jle";
	else if(!strcmp(op, ">=")) return "jge";
	else return NULL;
}

void disectIfStateInstructionArgs(Function *func, char *line, Instruction *ins) {
	char *cpy = calloc(strlen(line)+1, sizeof(char));
	char *orig = cpy;
	char tmp[1024];
	int i, j=0;
	if(!ins->arguments) {
		ins->capacity = 3; 
		ins->argLen = 3;
		ins->arguments = calloc(ins->capacity, sizeof(char *));
	}

	ins->arguments[0] = querySubFuncScopeName(func, ins->lineNum);

	strcpy(cpy, line);
	cpy = strstr(cpy, "(");
	cpy++;
	for(i=0;strlen(cpy)>0;i++,cpy++) {
		if(cpy[0]==')') {
			ins->arguments[2] = calloc(strlen(tmp)+1, sizeof(char));
			strcpy(ins->arguments[2], tmp);
			ins->arguments[2][i-j] = '\0';
			WTRIM(ins->arguments[2]);
			break;
		}
		if(strlen(cpy)>=2&&
				(isChIfStateOp(cpy[0])&&isChIfStateOp(cpy[1]))) {
			ins->arguments[1] = calloc(strlen(tmp)+1, sizeof(char));
			strcpy(ins->arguments[1], tmp);
			ins->arguments[1][i-j] = '\0';
			WTRIM(ins->arguments[0]);
			memset(tmp, 0, sizeof(tmp));
			i+=2; cpy+=2;
			j += i;
		}
		tmp[i-j] = cpy[0];
	}
	free(orig); orig = NULL;
}

void getIfStateOp(char *line, Instruction *ins) {
	char *cpy = calloc(strlen(line)+1, sizeof(char));
	char *tmp = calloc(IFOPALLOCS, sizeof(char)); /*2 is sizeof op I.E. "=="*/
	int i;
	strcpy(cpy, line);
	for(i=0;strlen(cpy)>=2;cpy++,i++) {
		if(isChIfStateOp(cpy[0])&&
				isChIfStateOp(cpy[1])) {
			snprintf(tmp, IFOPALLOCS*sizeof(char), "%c%c", 
					cpy[0], cpy[1]);
			break;
		}
	}
	cpy -= i;
	free(cpy); cpy = NULL;
	ins->instruction = convertIfStateOp(tmp);
	if(ins->instruction==NULL) {
		/*invalid op*/
		WLOG_WERROR(WERROR_INVALID_OPERATOR, ins->errData.fileName, 
				ins->errData.lineNum, ins->errData.function, "");	
	}
	free(tmp); tmp = NULL;
}

void parseIfStateInstruction(Function *func, char *line, Instruction *ins) {
		if(!line||!ins||!func) return;
		getIfStateOp(line, ins);
		disectIfStateInstructionArgs(func, line, ins);
}

/* * * * *
 * Function related functions
 * * * * */


void appendFuncArr(Function func, struct parserData *parser) {
	int i;
	for(i=0;i<MAXFUNCTIONS&&parser->functions[i].funName!=NULL;i++);
	parser->functions[i] = func;
	parser->totalFunctions++;
}

int checkFuncForExternScope(int lineNum, struct parserData *parser) {
	int i;
	for(i=0;i<MAXSCOPES;i++) {
		if(parser->scopes[i].scopeName==NULL) break;
		if(parser->scopes[i].lineNum==lineNum&&
				(parser->scopes[i].scopeType==IFSTATE ||
				parser->scopes[i].scopeType==LOOP)) return 1;	
	}
	return 0;
}

int checkBrackFrom(int lineFrom, int lineTo, struct parserData *parser) {
	int i = lineFrom;
	for(;i<=lineTo;i++) {
		if(checkImportantType(parser->fileBuffer[i])) return 0;
		if(parser->fileBuffer[i][0]!='\n'&&
				strlen(parser->fileBuffer[i])!=1) return 1;
	}
	return 0;
}

char *querySubFuncScopeName(Function *func, int lineNum) {
	int i;
	if(!func) return NULL;
	for(i=0;i<func->totalScopes;i++) {
		if(func->subScopes[i].scope.lineNum==lineNum) 
			return func->subScopes[i].scope.scopeName;
	}
	return NULL;
}

void addFunctionSubScopeData(Function *func, char *data) {
	int *sspos;
	FuncSubScopeData *scope;
	if(!func||!data) return;
	sspos = &func->totalScopes;
	scope = &func->subScopes[*sspos];
	if(scope->totalData>=scope->dataCap) {
		scope->dataCap += SUBSCOPESTEP;
		scope->data = (char **)realloc(scope->data, 
				scope->dataCap*sizeof(char *));
		scope->instructions = (Instruction *)realloc(scope->instructions, 
				scope->dataCap*sizeof(Instruction));
	}
	scope->data[scope->totalData] = calloc(strlen(data)+1, sizeof(char));
	strcpy(scope->data[scope->totalData], data);
	scope->totalData++;
}

void startFunctionSubScope(Function *func, char *data, int lineNum) {
	char buf[1024];
	if(!func||!data) return;
	if(func->totalScopes>=func->scopeCap) {
		func->scopeCap += SUBSCOPESTEP;
		func->subScopes = (FuncSubScopeData *)realloc(func->subScopes,
				func->scopeCap*sizeof(FuncSubScopeData));
	}
	func->subScopes[func->totalScopes].scope.scopeType = getScopeType(data);
	func->subScopes[func->totalScopes].scope.lineNum = lineNum;
	switch(func->subScopes[func->totalScopes].scope.scopeType) {
		case IFSTATE: snprintf(buf, sizeof(buf), "wl_%s_is_%d", 
							  func->funName, func->totalScopes); 
					  break;
		case LOOP: snprintf(buf, sizeof(buf), "wl_%s_lop_%d", 
						   func->funName, func->totalScopes);
				   break;
		default: break;
	};
	func->subScopes[func->totalScopes].scope.scopeName =
		calloc(strlen(buf), sizeof(char));
	strcpy(func->subScopes[func->totalScopes].scope.scopeName, buf);
	func->subScopes[func->totalScopes].totalData = 0;
	func->subScopes[func->totalScopes].dataCap = SUBSCOPESTEP;
	func->subScopes[func->totalScopes].data = calloc(SUBSCOPESTEP, sizeof(char *));
	func->subScopes[func->totalScopes].instructions = 
		calloc(SUBSCOPESTEP, sizeof(Instruction));
}

void endFunctionSubScope(Function *func) {
	func->totalScopes++;
}

/* Get all lines from a function scope into function buffer */
void buffToFunc(Function *func, struct parserData *parser) {
	int i = func->scope.lineNum+1, j=0;
	int scopeCarry=0;
	func->dataLength = 0;
	for(;i<parser->bufferSize;i++) {
		int pos = (i-(func->scope.lineNum+1))-j;
		if(parser->fileBuffer[i]==NULL||parser->fileBuffer[i][0]=='\n') {
			j++;
			continue;
		}
		if(pos>=func->capacity) {
			func->capacity+=DEFAULTINSARGSIZE/2;
			func->data = (char **)realloc(func->data, sizeof(char *)*func->capacity);
		}
		if(checkFuncForExternScope(i, parser)) {
			startFunctionSubScope(func, parser->fileBuffer[i], i);
			scopeCarry++;
			/*throw the if statement into the function data*/
			func->data[pos] = calloc(strlen(parser->fileBuffer[i])+1, sizeof(char));
			strcpy(func->data[pos], parser->fileBuffer[i]);
			func->dataLength++;
			continue;
		}
		if(strstr(parser->fileBuffer[i], "}")) {
			if(scopeCarry>0) endFunctionSubScope(func);
			scopeCarry--;
			if(scopeCarry<0) break;
			/*-1 would be main*/
			if(scopeCarry==0) {j++;continue;} 
		}
		if(scopeCarry>0) {
			addFunctionSubScopeData(func, parser->fileBuffer[i]);
			j++;
		} else {
			func->data[pos] = calloc(strlen(parser->fileBuffer[i])+1, sizeof(char));
			strcpy(func->data[pos], parser->fileBuffer[i]);
			func->dataLength++;
		}
	}
	if(scopeCarry>0) {
		WLOG_WERROR(WERROR_MISSING_BRACKET, 
				parser->fData->fileName, 
				func->scope.lineNum,
				func->funName, "");
	}
}



void parseFunctionInstructions(Function *func) {
	int i,j=0,k=0;
	for(i=0;i<func->dataLength;i++) {
		char *cpy;
		WPFUNC parseFunc;
		if(func->data[i]==NULL) continue;
		if(!strcmp(func->data[i], "\t")||
				!strcmp(func->data[i], "\n")||
				!strcmp(func->data[i], "\0")) continue;
		parseFunc = (WPFUNC)parseInstruction;
		if(checkImportantType(func->data[i])) {
			enum wTypes st = getScopeType(func->data[i]);
			if(st==IFSTATE) parseFunc = (WPFUNC)parseIfStateInstruction;
			else if(st==LOOP) continue; /*TODO*/
			else continue;
		}
		cpy = calloc(strlen(func->data[i])+1, sizeof(char));
		strcpy(cpy, func->data[i]);
		func->instructions[j].line = calloc(strlen(func->data[i])+1, sizeof(char));
		strcpy(func->instructions[j].line, func->data[i]);
		func->instructions[j].instruction = NULL;
		func->instructions[j].arguments = NULL;
		func->instructions[j].errData.lineNum = func->scope.lineNum+i+1; /*+1 since we skip header*/
		func->instructions[j].lineNum = func->scope.lineNum+i+1;
		func->instructions[j].errData.function = calloc(strlen(func->funName)+1, 
				sizeof(char));
		strcpy(func->instructions[j].errData.function, func->funName);
		parseFunc(func, cpy, &func->instructions[j]);
		free(cpy);cpy=NULL;
		func->instructions[j].lineNum = func->scope.lineNum+i;
		j++;
	}
	/*Parse subscope instructions*/
	for(i=0;i<func->totalScopes;i++) {
		FuncSubScopeData *scope = &func->subScopes[i];
		if(!scope||!scope->data) continue;
		for(j=0;j<scope->totalData;j++) {
			char *cpy;
			if(checkImportantType(scope->data[j])) continue; /*TODO add the recursive sub sub scopes*/
			cpy = calloc(strlen(scope->data[j])+1, sizeof(char));
			strcpy(cpy, scope->data[j]);
			scope->instructions[k].lineNum = scope->scope.lineNum+k;
			scope->instructions[k].line = calloc(strlen(scope->data[j])+1,
					sizeof(char));
			strcpy(scope->instructions[k].line, scope->data[j]);
			scope->instructions[k].instruction = NULL;
			scope->instructions[k].arguments = NULL;
			scope->instructions[k].errData.lineNum = scope->scope.lineNum+k;
			scope->instructions[k].errData.function = 
				calloc(strlen(func->funName)+1, sizeof(char));
			strcpy(scope->instructions[k].errData.function, func->funName);
			parseInstruction(func, cpy, &scope->instructions[k]);
			free(cpy);cpy=NULL;
			k++;
		}
	}
}

void getFunctionType(struct parserData *parser, Function *func) {
	char *type;
	if(func->scope.lineNum>parser->bufferSize) return;
	if(parser->fileBuffer[func->scope.lineNum]==NULL) return;
	type = strstr(parser->fileBuffer[func->scope.lineNum], "~");
	if(type!=NULL) {
		Variable grabtype;
		type++;
		type = strtok(type, ":");
		setVariableType(&grabtype, type,
				func->scope.lineNum, parser->fData->fileName);
		func->type = grabtype.type;
	}
}

int verifyFunctionReturn(struct parserData *parser, Function *func) {
	int i;
	if(parser==NULL||func==NULL) return 0;
	if(func->type==VOID) return 1; /*you don't need to return in void*/
	for(i=0;i<func->dataLength&&
			func->instructions[i].instruction!=NULL;i++) {
		if(!strcmp(func->instructions[i].instruction, "return")) {
			return 1;
		}
	}

	WLOG_WERROR(WERROR_NO_RETURN, 
			parser->fData->fileName, func->scope.lineNum, 
			func->funName, "");
	return 0;
}

void getFunctionData(struct parserData *parser) {
	int i;
	/*scope ptr*/
	Scope *s = calloc(1, sizeof(Scope));
	for(i=0;i<MAXSCOPES&&parser->scopes[i].scopeName!=NULL&&
			(s=&parser->scopes[i]);i++) {
		/*Make sure it's a function scope*/
		if(s->scopeType==FUNCTION) {
			Function f;
			memset(&f, 0, sizeof(Function));
			f.data = calloc(DEFMAXFSIZE+2, sizeof(char *));
			f.capacity = DEFMAXFSIZE;
			f.dataLength = 0;
			f.funName = calloc(strlen(s->scopeName)+1, sizeof(char));
			strcpy(f.funName, s->scopeName);
			f.scope = *s;
			f.subScopes = calloc(SUBSCOPESTEP, sizeof(FuncSubScopeData));
			f.totalScopes = 0;
			f.scopeCap = SUBSCOPESTEP;

			f.lvt = calloc(1, sizeof(LVT));
			f.lvt->variables = NULL;
			f.lvt->totalVariables = 0;

			buffToFunc(&f, parser);
			getFunctionType(parser, &f);

			f.instructions = calloc(f.dataLength+10, sizeof(Instruction));
			buildFunctionLVT(&f);
			parseFunctionInstructions(&f);

			if(verifyFunctionReturn(parser, &f)) appendFuncArr(f, parser);
		}
	}
}

void dumpFunctionData(struct parserData *parser) {
	int i,j;
	for(i=0;i<MAXFUNCTIONS&&parser->functions[i].funName!=NULL;i++) {
		WLOG(INFO, parser->functions[i].funName);
		for(j=0;j<parser->functions[i].dataLength;j++) {
			printf("%s", parser->functions[i].data[j]);
			printf("%s\n", parser->functions[i].instructions[j].arguments[0]);
		}
	}
}

int verifyMainFunction(struct parserData *parser) {
	int i;
	for(i=0;i<parser->totalFunctions&&parser->functions[i].funName!=NULL;i++) {
		if(!strcmp(parser->functions[i].funName, "main")) return 1;
	}
	WLOG_WERROR(WERROR_MAIN, 
				parser->fData->fileName, 
				0, "Global", "");
	return 0;
}


/*
 * Compiler directive handling
 * */
void getCompTimeDirectives(struct parserData *parser) {
	int i;
	parser->compDirectives.NOMAIN = 0;
	for(i=0;i<parser->bufferSize;i++) {
		char *directive;
		if(parser->fileBuffer[i]==NULL) continue;
		directive = strchr(parser->fileBuffer[i], '#');
		if(directive!=NULL) {
			directive++;
			while(directive[strlen(directive)-1]=='\n') directive[strlen(directive)-1] = '\0';
			directive = strtok(directive, " ");
			/*See what the directive is*/
			if(!strcmp(directive, "nomain")||
					!strcmp(directive, "NOMAIN")) parser->compDirectives.NOMAIN=1;
		}
	}
}


/*
 * Extern handling
 * */

void getExternals(struct parserData *parser) {
	static int defaultArrSize = 100;
	int i;
	parser->externals.externs = calloc(defaultArrSize+2, sizeof(char *));
	parser->externals.capacity = defaultArrSize;
	parser->externals.externSize = 0;
	for(i=0;i<MAXSCOPES&&parser->scopes[i].scopeName!=NULL;i++) {
		if(parser->scopes[i].scopeType==EXTERN) {
			char *external;
			char *line = calloc(strlen(parser->fileBuffer[parser->scopes[i].lineNum])+1, sizeof(char));
			strcpy(line, parser->fileBuffer[parser->scopes[i].lineNum]);
			external = strstr(line, "~extern");
			if(external!=NULL) {
				external+=strlen("~extern");
				while(external[0]==' ') external++;
				while(external[strlen(external)-1]=='\n') external[strlen(external)-1] = '\0';
				if(parser->externals.externSize>=parser->externals.capacity) {
					parser->externals.capacity*=2;
					parser->externals.externs = 
						(char **)realloc(parser->externals.externs, 
								sizeof(char *)*parser->externals.capacity);
				}
				parser->externals.externs[parser->externals.externSize] = calloc(strlen(external)+1, sizeof(char));
				strcpy(parser->externals.externs[parser->externals.externSize], external);
				parser->externals.externSize++;
			} else {
				WLOG_WERROR(WERROR_EXTERN_NOVALUE,
						parser->fData->fileName, 
						parser->scopes[i].lineNum, "Global", "");	
			}
		}
	}
}

/* 
 * Inclusion handling
 * */

void getIncludedFiles(struct parserData *parser) {
	static int defaultArrSize = 100;
	int i;
	parser->includes.includedFiles = calloc(defaultArrSize+2, sizeof(char *));
	parser->fData->includedFiles = calloc(defaultArrSize+2, sizeof(char *));
	parser->includes.capacity = defaultArrSize;
	parser->includes.includeSize = 0;
	parser->fData->includeSize = 0;
	for(i=0;i<MAXSCOPES&&parser->scopes[i].scopeName!=NULL;i++) {
		if(parser->scopes[i].scopeType==INCLUDE) {
			char *included;
			char *line = calloc(strlen(parser->fileBuffer[parser->scopes[i].lineNum])+1, sizeof(char));
			strcpy(line, parser->fileBuffer[parser->scopes[i].lineNum]);
			included = strstr(line, "~include");
			if(included!=NULL) {
				included+=strlen("~include");
				while(included[0]==' ') included++;
				while(included[strlen(included)-1]=='\n') included[strlen(included)-1] = '\0';
				if(included[0]=='<') {
					/*TODO standard*/
				} else if(included[0]=='\"'&&
						included[strlen(included)-1]=='\"') {
					char *tmp, *asmFile;
					char *erbuf;
					struct parserData *p;
					AsmOut *output;
					wData *data;
					included++;
					included[strlen(included)-1] = '\0';
					if(parser->includes.includeSize>=parser->includes.capacity) {
						parser->includes.capacity*=2;
						parser->includes.includedFiles = 
							(char **)realloc(parser->includes.includedFiles, 
									sizeof(char *)*parser->includes.capacity);
						parser->fData->includedFiles = 
							(char **)realloc(parser->fData->includedFiles,
									sizeof(char *)*parser->includes.capacity);

					}
					parser->includes.includedFiles[parser->includes.includeSize] = calloc(strlen(included)+1, sizeof(char));
					strcpy(parser->includes.includedFiles[parser->includes.includeSize], included);
					/*set the .s file so we can create objs*/
					tmp = calloc(strlen(included)+1, sizeof(char));
                    strcpy(tmp, included);
					asmFile = strtok(tmp, ".");
					strcat(asmFile, ".s");
					parser->fData->includedFiles[parser->fData->includeSize] = calloc(strlen(asmFile)+1, sizeof(char));
					strcpy(parser->fData->includedFiles[parser->fData->includeSize], asmFile);

					parser->includes.includeSize++;
					parser->fData->includeSize++;
					/*Run the parser on the included file*/
					data = calloc(1, sizeof(wData));
					data->main = fopen(included, "r");
					data->fileName = calloc(strlen(included)+1, sizeof(char));
					strcpy(data->fileName, included);
					erbuf = calloc(256, sizeof(char));
					snprintf(erbuf, sizeof(char)*256, 
							"FATAL:: Failed to open file: %s", included);
					WASSERT(data->main!=NULL, erbuf);
					free(erbuf);
					p = calloc(1, sizeof(struct parserData));
					p = initParser(data);
					parseProgram(p);
					output = calloc(1, sizeof(AsmOut));
					initAsmOut(p, output);
					convertToAsm(output);
					freeAsm(output);
					free(data);data=NULL;
					free(p);p=NULL;
					free(output);output=NULL;
				}
			} else {
				WLOG_WERROR(WERROR_EXTERN_NOVALUE,
					parser->fData->fileName, 
					parser->scopes[i].lineNum, "Global", "");	
			}
		}
	}
}

/* * * * *
 * Parser initialization and running functions
 * * * * */



void parseProgram(struct parserData *parser) {
	getCompTimeDirectives(parser);

	getScopes(parser);
	getIncludedFiles(parser);
	getExternals(parser);
	getFunctionData(parser);
	getVariables(parser);
	if(!parser->compDirectives.NOMAIN)
		verifyMainFunction(parser);
	/*dumpVariables(parser);*/
	/* Useful for debugging*/
	 /*dumpScopes(parser);*/
	 /*dumpFunctionData(parser);*/
	
}

struct parserData *initParser(wData *data) {
	char *line;
	int i, j=0, mul=1;
	if(!data) return NULL;
	gPData = calloc(1, sizeof(struct parserData));
	gPData->fData = data;
	CPU = gPData->fData->cpu;

	WASSERT(gPData->fData->main!=NULL,
			"FATAL:: Main file not found!");


	gPData->fileBuffer = calloc(2048, sizeof(char *));

	/*Get all the file data into the file buffer*/
	line = calloc(2048, sizeof(char));
	for(i=0;fgets(line, sizeof(char)*2048, gPData->fData->main)!=NULL;i++) { 
		char *rawptr;
		if(i-j>=2048*mul) {
			gPData->fileBuffer = (char **)realloc(gPData->fileBuffer,
					sizeof(char *)*(2048*mul));
			mul++;
		} 
		/*if(line[0]=='\n'||line[0]=='\0'||line[0]==';') {
			j++;
			continue;
		}*/
		rawptr = line;
		WTRIM(rawptr);
		gPData->fileBuffer[i-j] = calloc(strlen(rawptr)+1, sizeof(char));
		strcpy(gPData->fileBuffer[i-j], rawptr);

		free(line); line = NULL; rawptr = NULL;
		line = calloc(2048, sizeof(char));
	}
	gPData->bufferSize = i;

	fclose(gPData->fData->main);
	gPData->fData->main = NULL;

	gPData->totalFunctions = 0;

	return gPData;
}

