/*Copyright (c) 2024-2026 MinervaWare LLC*/
/*Copyright (c) 2022-2024 Tristan Wellman*/
#ifndef PARSER_H
#define PARSER_H

#include <chash.h>
#include "util.h"
#include "wdata.h"
#include "werror.h"

struct StringArray {
    int length;
    int capacity;
    char **contents;
};

enum wTypes {
	NONE=0,
	INCLUDE,
	EXTERN,
	FUNCTION,
	IFSTATE,
	LOOP,
	VARIABLE,
	CONSTANTS /*This one is specific to wellang design*/
};


enum varTypes {
	INT,
	CHAR,
	STRING,
	FLOAT,
	VOID,
	PTR, /*This can be any random memory location to any type*/
	ANY /*variadic arguments*/
};

/*
 * If you need more than that many scopes in a file,
 * you should re-think what you are doing
 * */
#define MAXSCOPES 100000
#define SUBSCOPESTEP 5

#define MAXFUNCTIONS 10000
#define DEFMAXFSIZE 1024 /*default max amount of lines for a function*/

#define DEFAULTINSARGSIZE 2

#define DEFALLOCSTEP 5

#define IFOPALLOCS 3

typedef void (*WPFUNC)();

/*Declarations for dynamic scope arrays*/
typedef struct {
	enum wTypes scopeType;
	char *scopeName;
	int lineNum;
} Scope;

typedef struct {
	char *funName; /*parent*/
	char *line; /* move~ 1, r */
	char *instruction; /* move */
	char **arguments; /* ["1", "r1"] */
	int argLen;
	int capacity;
	WerrorData errData;
	int lineNum;
} Instruction;

typedef struct {
	enum varTypes type;
	char *varName;
	char *value;
	int offset;
} Variable;

typedef struct {
	Variable *variables;
	int totalVariables, cap;
} LVT; /*Local Variable Table*/

typedef struct {
	/*Name/ID will be stored in scope*/
	Scope scope;
	Instruction *instructions;
	char **data;
	int totalData, dataCap;
} FuncSubScopeData;

typedef struct {
	enum varTypes type;
	char *funName;
	char **data; 
	int dataLength;
	int capacity;
	Scope scope;
	FuncSubScopeData *subScopes;
	int totalScopes, scopeCap;
	Instruction *instructions;
	LVT *lvt;
} Function;

struct Directives {
	int NOMAIN;
	/*TODO add more comp directives*/
};

typedef struct {
	char *name;
	enum varTypes *argTypes;
	unsigned int argSize, argCap;
} ExternData;

struct Externs {
	ExternData *externs;
	int size;
	int capacity;
};

struct Inclusion {
	char **includedFiles;
	int includeSize;
	int capacity;
};

/*Struct to hold arrays of parser handy data*/
struct parserData {
	wData *fData;
	
	char **fileBuffer;
	int bufferSize;

	struct Directives compDirectives;
	struct Externs externals;
	struct Inclusion includes;

	Scope scopes[MAXSCOPES];
	Function functions[MAXFUNCTIONS];
	int totalFunctions;
	Variable *variables;
	int totalVariables;
};

/**
 * Function Prototypes
 * */
Variable *getVarFrom(struct parserData *parser, char *name);
Variable *queryLocalVariable(struct parserData *parser, 
		int lineNum, char *var);

int doesFunctionExistInternal(struct parserData *parser, char *name);

ExternData *getExternalData(struct parserData *parser, char *name);

void parseProgram(struct parserData *parser);
struct parserData *initParser(wData *data);
void freeParser(struct parserData *parser);

#endif
