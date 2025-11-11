/*Copyright (c) 2024 Tristan Wellman*/
#ifndef WERROR_H
#define WERROR_H
#include "util.h"

struct wErrors {
	int errorCount;
	int warnCount;
};
_W_PRIVATE struct wErrors errors;

typedef struct {
	int lineNum;
	char *fileName;
	char *function;
} WerrorData;

#define WERROR_MISSING_BRACKET 1
#define WERROR_MAIN 2
#define WERROR_UNDEFINED_INSTRUCTION 3
#define WERROR_UNINITIALIZED_VARIABLE 4
#define WERROR_UNDEFINED_TYPE 5
#define WERROR_NO_RETURN 6
#define WERROR_INVALID_SYNTAX 7
#define WERROR_EXTERN_NOVALUE 8
#define WERROR_MISSING_INS_ARG 9

#define WLOG_WERROR(werror, file, line, function, ...) 		\
	fprintf(stderr, "%s \033[31m"#werror"\033[0m in %s:%d - %s\n%s",		\
			__TIME__, file, line, function, __VA_ARGS__); 	\
	errors.errorCount++;

#define WLOG_WARN(wwarn, file, line, function, ...) \
	fprntf(); \
	errors.warnCount++;

#define WERROR_EXIT() \
	fprintf(stdout, "%d errors generated.\n", errors.errorCount); \
	if(errors.errorCount>0) exit(EXIT_FAILURE);

#endif
