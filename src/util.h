/*Copyright (c) 2024 Tristan Wellman*/

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>

#define WLOG(logLevel, ...) \
	fprintf(stdout, "%s "#logLevel"  (%s:%d): %s\n", \
		__TIME__,__FILE__,__LINE__,__VA_ARGS__);

#define WASSERT(err, ...) \
	if(!(err)) {fprintf(stderr,__VA_ARGS__);exit(1);}	

#define RESETFCURSOR(file) \
	fseek(file, 0, SEEK_SET);

#define ARRLEN(x) \
		(sizeof(x)/sizeof(x[0]))

#define STARTAPPCHAR(str_, c_) \
	do{char *STRET_ = calloc((strlen(str_)+2),sizeof(char)); \
	STRET_[0] = c_; \
	memcpy(STRET_+1, str_, strlen(str_)); \
	STRET_[strlen(str_)+1] = '\0'; \
	str_ = STRET_;}while(0)	\

#define WTRIM(s_) \
	do{while(*(s_)&&isspace((unsigned char)*(s_)))(s_)++; \
	if(*(s_)){char *endp = (s_)+strlen(s_)-1; \
	while(endp>(s_)&&isspace((unsigned char)*endp))*endp--='\0';}}while(0);

#define EATTABS(line) \
	do{int i,j=0; \
	for(i=0;line[i];i++) { \
		if(line[j+1]=='\0') break; \
		if(line[i]!='\t') line[j++] = line[i];} \
	line[j]='\0';}while(0)

#define _W_NONE
#if defined(__GNUC__) || defined(__clang__)
#	define _W_PRIVATE __attribute__((unused)) static
#	define _W_NTHROW __attribute__((nothrow))
#	define _W_HOT __attribute__((hot))
#	define _W_COLD __attribute__((cold))
#	define _W_PURE __attribute__((pure))
#else
#	define _W_PRIVATE static
#	define _W_NTHROW _W_NONE
#	define _W_HOT _W_NONE
#	define _W_COLD _W_NONE
#	define _W_PURE _W_NONE
#endif

/*Private util implementations*/

_W_PRIVATE _W_HOT char *intToHex(char *value) {
	int input = atoi(value);
	char *res = calloc(strlen(value)+1024, sizeof(int));
	snprintf(res, sizeof(res), "0x%x", input);
	return res;
}

_W_PRIVATE _W_HOT char *floatToHex(char *value) {
	float input = atof(value);
	uint32_t convVal = 0;
	memcpy(&convVal, &input, sizeof(float));
	char *res = calloc(strlen(value)+1024, sizeof(int));
	snprintf(res, sizeof(res), "0x%x", convVal);
	return res;
}

#endif
