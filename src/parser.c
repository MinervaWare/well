/*Copyright (c) 2024 Tristan Wellman*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "asmout.h"
#include "werror.h"

struct parserData *gPData;

/*
 * Checks if line contains important type:
 * EX: ~constants or ~type:function 
 * */
int checkImportantType(char *line) {
    if(line==NULL||*line=='\0') return 0;
	int i;
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
	if(strstr(line, "~constants")) return CONSTANTS;
	if(strstr(line, "~extern")) return EXTERN;
	if(strstr(line, "~include")) return INCLUDE;
	int i, col=0, curl=0;
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
	strcpy(tmp, line);
	char *aftrcurl = strstr(tmp, "~");
	if(aftrcurl!=NULL) {
		aftrcurl++;
		char *name;
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
		if(parser->scopes[i].scopeName==NULL) break;
		char buffer[100];
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
			Scope s = (Scope){ getScopeType(currentLine), NULL, i};
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
	for(i=0;i<MAXSCOPES&&parser->scopes[i].scopeName!=NULL;i++) {
		if(lineNum==parser->scopes[i].lineNum) return parser->scopes[i];
	}
	return (Scope){};
}

Scope getNextScope(Scope scope, struct parserData *parser) {
	int i;
	for(i=0;i<MAXSCOPES&&parser->scopes[i].scopeName!=NULL;i++) {
		if(scope.scopeName==parser->scopes[i].scopeName&&
				scope.lineNum==parser->scopes[i].lineNum) break;
	}
	for(i=i+1;i<MAXSCOPES&&parser->scopes[i].scopeName!=NULL;i++) {
		if(parser->scopes[i].scopeType!=IFSTATE&&
				parser->scopes[i].scopeType!=LOOP) return parser->scopes[i];
	}
	return (Scope){};
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

Variable getVarFrom(struct parserData *parser, char *name) {
	int i;
	for(i=0;i<parser->totalVariables&&parser->variables[i].varName!=NULL;i++) {
		if(!strcmp(parser->variables[i].varName, name)) return parser->variables[i];
	}
	return (Variable){.varName=NULL};
}

/*This is expected to be ran AFTER scopes have been initialized*/
void getVariables(struct parserData *parser) {

	parser->variables = calloc(1, sizeof(Variable));
	parser->totalVariables = 0;

	int i, j=0;
	for(i=0;i<MAXSCOPES;i++) {
		if(parser->scopes[i].scopeName==NULL) break;
		
		if(parser->scopes[i].scopeType == VARIABLE) {
			
			parser->variables = (Variable *)realloc(parser->variables, 
					sizeof(Variable)*(parser->totalVariables+1));
			char *line = parser->fileBuffer[parser->scopes[i].lineNum];
			char *data = strstr(line, "=");
			if(data==NULL) {
 				WLOG_WERROR(WERROR_UNINITIALIZED_VARIABLE,
						parser->fData->fileName, 
						parser->scopes[i].lineNum, "constants", "");	
			} else {
				data++;
				if(data[strlen(data)-1]=='\n') data[strlen(data)-1] = '\0';
				while(data[0]==' ') data++;
				parser->variables[j].value = calloc(strlen(data)+1, sizeof(char));
				strcpy(parser->variables[j].value, data);
				/*save data*/
				char *tmp  = calloc(strlen(line)+1, sizeof(char));
				strcpy(tmp, line);
				
				/*type*/
				char *type = strstr(line, "~");
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
				free(tmp);
			}
		}
	}
}

/*
 *   ~int:abc = 123
 * */
char *LVTGetType(char *line) {
	if(line==NULL) return NULL;
	char *lcpy = calloc(strlen(line)+1, sizeof(char));
	strcpy(lcpy, line);
	char *type = NULL;
	type = strstr(lcpy, "~");
	type++;
	type = strtok(type, ":");
	while(type[0]==' ') type++;
	while(type[strlen(type)-1]==' ') type[strlen(type)-1] = '\0';
	return type;
}

char *LVTGetValue(char *line) {
	if(line==NULL) return NULL;
	char *lcpy = calloc(strlen(line)+1, sizeof(char));
	strcpy(lcpy, line);
	char *value = NULL;
	value = strstr(lcpy, "=");
	value++;
	while(value[0]==' ') value++;
	while(value[strlen(value)-1]==' '||
			value[strlen(value)-1]=='\n') value[strlen(value)-1] = '\0';
	return value;
}

char *LVTGetVName(char *line) {
	if(line==NULL) return NULL;
	char *lcpy = calloc(strlen(line)+1, sizeof(char));
	strcpy(lcpy, line);
	char *name = NULL;
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
		case INT: return sizeof(int32_t);
		case CHAR: return sizeof(char);
		case STRING: return sizeof(char *) /*8*/;
		case FLOAT: return sizeof(float);
		case VOID: return sizeof(void);
		case ZERO: return sizeof(void);
	};	
	return 0;
}

void buildFunctionLVT(Function *func) {
	static int curOffset = 0;
	if(func==NULL) return;
	if(func->lvt==NULL) {
		func->lvt = calloc(1, sizeof(LVT));
		func->lvt->offsets = NULL;
		func->lvt->variables = NULL;
		func->lvt->totalVariables = 0;
		func->lvt->cap = DEFALLOCSTEP;
	}
	if(func->lvt->variables==NULL&&
			func->lvt->offsets==NULL) {
		func->lvt->variables = calloc(DEFALLOCSTEP, sizeof(Variable));	
		func->lvt->offsets = calloc(DEFALLOCSTEP, sizeof(int));
	}
	int i, *j = &func->lvt->totalVariables;
	for(i=0;i<func->dataLength;i++) {
		if(func->data[i]==NULL) continue;
		if(checkImportantType(func->data[i])) {
			if(getScopeType(func->data[i])==VARIABLE) {
				if(*j>func->lvt->cap) {
					func->lvt->cap += DEFALLOCSTEP;
					func->lvt->variables = (Variable *)realloc(func->lvt->variables,
							sizeof(Variable)*func->lvt->cap);
					func->lvt->offsets = (int *)realloc(func->lvt->offsets,
							sizeof(int)*func->lvt->cap);
				}
				char *type = LVTGetType(func->data[i]);
				char *name = LVTGetVName(func->data[i]);
				char *value = LVTGetValue(func->data[i]);
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
				func->lvt->offsets[*j] = LVTGetOffsetSize(
						func->lvt->variables[*j].type);
				*j = *j+1;
			}
		}	
	}
}


/* * * * *
 * Instruction related functions.
 * * * * */
Instruction instructionDup(const Instruction *src) {
	Instruction ret;
	memset(&ret, 0, sizeof(Instruction));

	ret.argLen = src->argLen;
	ret.capacity = src->capacity;
	if(src->line!=NULL) {ret.line = calloc(strlen(src->line)+1,sizeof(char));
		strcpy(ret.line,src->line);}
	if(src->instruction!=NULL) {ret.instruction = calloc(strlen(src->instruction)+1,sizeof(char));
		strcpy(ret.instruction, src->instruction);}

	if(src->argLen>0&&src->arguments!=NULL) {
		ret.arguments = calloc(src->capacity>0 ? src->capacity : DEFAULTINSARGSIZE, 
				sizeof(char *));
		int i;
		for(i=0;i<src->argLen;i++) {
			if(src->arguments[i]!=NULL) 
				ret.arguments[i] = calloc(strlen(src->arguments[i])+1,sizeof(char));
				strcpy(ret.arguments[i], src->arguments[i]);
		}
	}
	return ret;
}

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
		ins->argLen = 0;
		ins->capacity = DEFAULTINSARGSIZE;
		ins->arguments = calloc(ins->capacity, sizeof(char *));
        char *backup = calloc(strlen(ins->line)+1, sizeof(char));
		strcpy(backup, ins->line);
		char *ignoreIns = strstr(backup, "~");
		if(ignoreIns!=NULL) {
			ignoreIns++;
			char *arg = strtok(ignoreIns, ",");
			while(arg!=NULL) {
				int i;
				for(i=0;i<strlen(arg);i++) {
					if(arg[i]=='\n') arg[i] = '\0';
				}
				appendInsArgs(ins, arg);
				arg = strtok(NULL, ",");
			}
		}
		free(backup);
	}
	if(ins->argLen==0) ins->arguments = NULL;
}

void disectInstructionName(Instruction *ins) {
	if(ins->line!=NULL) {
        char *backup = calloc(strlen(ins->line)+1, sizeof(char));
		strcpy(backup, ins->line);
		EATTABS(backup);
		/*move~ text, r1*/

		char *name = strtok(backup, "~");
		if(name!=NULL) {
			ins->instruction = calloc(strlen(name)+1, sizeof(char));
            strcpy(ins->instruction, name);
        }

		free(backup);
	}
}

void parseInstruction(char *line, Instruction *ins) {
	disectInstructionName(ins);
	getInstructionArguments(ins);
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

/* Get all lines from a function scope into function buffer */
void buffToFunc(Function *func, struct parserData *parser) {
	/*NOTE: func data buffer must already be allocated, I aint checking it!*/
	int i = func->scope.lineNum+1, j=0;
	int scopeCarry=0; /*amount of scopes inside the function I.E. loops*/
	func->dataLength = 0;
	for(;i<parser->bufferSize;i++) {
        if(parser->fileBuffer[i]==NULL||parser->fileBuffer[i][0]=='\n') {
			j++;
			continue;
		}
		if(checkFuncForExternScope(i, parser)) scopeCarry++;
		if(strstr(parser->fileBuffer[i], "}")) {

			/*see if we are getting played*/
			if(scopeCarry==0) {
				if(!checkBrackFrom(i+1, 
					getNextScope(func->scope, parser).lineNum, parser)) {
					scopeCarry--;
					break;
				}
				scopeCarry--;
			}
			if(scopeCarry<0) break;
		}
		int pos = (i-(func->scope.lineNum+1))-j;
        if(pos>=func->capacity) {
          	func->capacity+=DEFAULTINSARGSIZE/2;
          	func->data = (char **)realloc(func->data, sizeof(char *)*func->capacity);
        }
        func->data[pos] = calloc(strlen(parser->fileBuffer[i])+1, sizeof(char));
		strcpy(func->data[pos], parser->fileBuffer[i]);
		func->dataLength++;
	}
	if(scopeCarry>0) {
		WLOG_WERROR(WERROR_MISSING_BRACKET, 
				parser->fData->fileName, 
				func->scope.lineNum,
				func->funName, "");
	}
}

void parseFunctionInstructions(Function *func) {
	int i,j=0;
	for(i=0;i<func->dataLength;i++) {
		if(func->data[i]==NULL) continue;
		if(checkImportantType(func->data[i])) {
			/*Get if or loop statement data and convert*/
			continue;
		} else {
			char *cpy = calloc(strlen(func->data[i])+1, sizeof(char));
			strcpy(cpy, func->data[i]);
			/*memset(func->instructions[i], 0, sizeof(Instruction));*/
			func->instructions[j].line = NULL;
			func->instructions[j].line = calloc(strlen(func->data[i])+1, sizeof(char));
			strcpy(func->instructions[j].line, func->data[i]);
			func->instructions[j].instruction = NULL;
			func->instructions[j].arguments = NULL;
			func->instructions[j].errData.lineNum = func->scope.lineNum+j;
			func->instructions[j].errData.function = calloc(strlen(func->funName)+1, sizeof(char));
			strcpy(func->instructions[j].errData.function, func->funName);
			parseInstruction(cpy,
					&func->instructions[j]);
			free(cpy);
			j++;
		}
	}
}

void getFunctionType(struct parserData *parser, Function *func) {
    if(func->scope.lineNum>parser->bufferSize) return;
    if(parser->fileBuffer[func->scope.lineNum]==NULL) return;
	char *type = strstr(parser->fileBuffer[func->scope.lineNum], "~");
	if(type!=NULL) {
		type++;
		type = strtok(type, ":");
		Variable grabtype;
		setVariableType(&grabtype, type,
				func->scope.lineNum, parser->fData->fileName);
		func->type = grabtype.type;
	}
}

int verifyFunctionReturn(struct parserData *parser, Function *func) {
	if(parser==NULL||func==NULL) return 0;
	int i;
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

			f.lvt = calloc(1, sizeof(LVT));
			f.lvt->offsets = NULL;
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
	parser->compDirectives.NOMAIN = 0;
	int i;
	for(i=0;i<parser->bufferSize;i++) {
		if(parser->fileBuffer[i]==NULL) continue;
		char *directive = strchr(parser->fileBuffer[i], '#');
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
	parser->externals.externs = calloc(defaultArrSize+2, sizeof(char *));
	parser->externals.capacity = defaultArrSize;
	parser->externals.externSize = 0;
	int i;
	for(i=0;i<MAXSCOPES&&parser->scopes[i].scopeName!=NULL;i++) {
		if(parser->scopes[i].scopeType==EXTERN) {
			char *line = calloc(strlen(parser->fileBuffer[parser->scopes[i].lineNum])+1, sizeof(char));
            strcpy(line, parser->fileBuffer[parser->scopes[i].lineNum]);
			char *external = strstr(line, "~extern");
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
	parser->includes.includedFiles = calloc(defaultArrSize+2, sizeof(char *));
	parser->fData->includedFiles = calloc(defaultArrSize+2, sizeof(char *));
	parser->includes.capacity = defaultArrSize;
	parser->includes.includeSize = 0;
	parser->fData->includeSize = 0;
	int i;
	for(i=0;i<MAXSCOPES&&parser->scopes[i].scopeName!=NULL;i++) {
		if(parser->scopes[i].scopeType==INCLUDE) {
			char *line = calloc(strlen(parser->fileBuffer[parser->scopes[i].lineNum])+1, sizeof(char));
            strcpy(line, parser->fileBuffer[parser->scopes[i].lineNum]);
			char *included = strstr(line, "~include");
			if(included!=NULL) {
				included+=strlen("~include");
				while(included[0]==' ') included++;
				while(included[strlen(included)-1]=='\n') included[strlen(included)-1] = '\0';
				if(included[0]=='<') {
					/*TODO standard*/
				} else if(included[0]=='\"'&&
						included[strlen(included)-1]=='\"') {
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
					char *tmp = calloc(strlen(included)+1, sizeof(char));
                    strcpy(tmp, included);
					char *asmFile = strtok(tmp, ".");
					strcat(asmFile, ".s");
					parser->fData->includedFiles[parser->fData->includeSize] = calloc(strlen(asmFile)+1, sizeof(char));
					strcpy(parser->fData->includedFiles[parser->fData->includeSize], asmFile);

					parser->includes.includeSize++;
					parser->fData->includeSize++;
					/*Run the parser on the included file*/
					wData *data = calloc(1, sizeof(wData));
					data->main = fopen(included, "r");
					data->fileName = calloc(strlen(included)+1, sizeof(char));
                    strcpy(data->fileName, included);
					WASSERT(data->main!=NULL,
						"FATAL:: Failed to open file: %s", included);
					struct parserData *p = calloc(1, sizeof(struct parserData));
					p = initParser(data);
					parseProgram(p);
					AsmOut *output = calloc(1, sizeof(AsmOut));
					initAsmOut(p, output);
					convertToAsm(output);
					freeAsm(output);
					free(data);
					free(p);
					free(output);
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
    gPData = calloc(1, sizeof(struct parserData));
	gPData->fData = data;

	WASSERT(gPData->fData->main!=NULL,
			"FATAL:: Main file not found!");


	gPData->fileBuffer = calloc(2048, sizeof(char *));

	/*Get all the file data into the file buffer*/
	char line[2048];
	int i, mul=1;
	for(i=0;fgets(line, sizeof(line), gPData->fData->main)!=NULL;i++) { 
		if(i>=2048*mul) {
			gPData->fileBuffer = (char **)realloc(gPData->fileBuffer,
					sizeof(char *)*(2048*mul));
			mul++;
		} 

		gPData->fileBuffer[i] = calloc(ARRLEN(line)+1, sizeof(char));
		strcpy(gPData->fileBuffer[i], line);
	}
	gPData->bufferSize = i;

	fclose(gPData->fData->main);
	gPData->fData->main = NULL;

	gPData->totalFunctions = 0;

	return gPData;
}

void freeParser(struct parserData *parser) {
	int i,j,k;
	if(parser == NULL) return;
    /*fileBuffer*/
    if(parser->fileBuffer) {
        for (i = 0; i < parser->bufferSize; i++) {
            free(parser->fileBuffer[i]);
        }
        free(parser->fileBuffer);
    }
    /*Functions*/
    for(i = 0;i<MAXFUNCTIONS&&parser->functions[i].funName != NULL;i++) {
        Function *func = &parser->functions[i];
        free(func->funName);
        if(func->data) {
            for(j = 0;j<func->dataLength;j++) free(func->data[j]);
            free(func->data);
        }
        /*instructions*/
        if(func->instructions) {
            for(j = 0;j<func->dataLength;j++) {
                Instruction *ins = &func->instructions[j];
                free(ins->line);
                free(ins->instruction);
                if(ins->arguments) {
                    for(k = 0;k<ins->argLen;k++) free(ins->arguments[k]);
                    free(ins->arguments);
                }
            }
            free(func->instructions);
        }
    }
    /*variables*/
    if(parser->variables) {
        for (i = 0;i<parser->totalVariables;i++) {
            free(parser->variables[i].varName);
            free(parser->variables[i].value);
        }
        free(parser->variables);
    }
    /*scopes*/
    for(i = 0;i<MAXSCOPES&&parser->scopes[i].scopeName != NULL;i++)
        free(parser->scopes[i].scopeName);

    free(parser);
}

