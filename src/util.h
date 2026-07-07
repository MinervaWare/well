/*Copyright (c) 2024-2026 MinervaWare LLC*/
/*Copyright (c) 2022-2024 Tristan Wellman*/

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdarg.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <chash.h>
#include "osname.h"

#if defined(__clang__)
#	ifndef __INT32_TYPE__
typedef int int32_t;
#	endif
#	ifndef __UINT32_TYPE__
typedef unsigned int uint32_t;
#	endif
#	ifndef __INT64_TYPE__
typedef long int int64_t;
#	endif
#	ifndef __UINT64_TYPE__
typedef unsigned long int uint64_t;
#	endif
#else
#	ifndef __S32_TYPE
typedef int int32_t;
#	endif
#	ifndef __U32_TYPE
typedef unsigned int uint32_t;
#	endif
#	ifndef __S64_TYPE
typedef long int int64_t;
#	endif
#	ifndef __U64_TYPE
typedef unsigned long int uint64_t;
#	endif
#endif

#define WLOG(logLevel, _str) \
	fprintf(stdout, "%s "#logLevel"  (%s:%d): %s\n", \
		__TIME__,__FILE__,__LINE__,(_str));

#define WASSERT(err, _str) \
	if(!(err)) {fprintf(stderr,(_str));exit(1);}	

#define RESETFCURSOR(file) \
	fseek(file, 0, SEEK_SET);

#define ARRLEN(x) \
		(sizeof(x)/sizeof(x[0]))

#define _wGetCStrSize(_data) (strlen(_data)+1)

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

/*allocation shit*/

typedef enum {
	ALLOC,
	RESIZE,
	FREE
} wAllocMode;

typedef void *(*WALLOCPROC)(int size, wAllocMode mode, void *oldMem);

typedef struct {
	WALLOCPROC allocProc;
} wAllocator;

_W_COLD void wInitDefaultAllocator();
_W_COLD void wSetAllocator(WALLOCPROC proc);
_W_HOT void *wAlloc(int _size);
_W_HOT void *wResize(void *_oldMem, int _size);
_W_HOT void wFree(void *_oldMem);

/*Private util implementations*/

#define WCSTR_TABLE_HEAP 1
#define WCSTR_TABLE_BUCKET wCStrBucket
#define WCSTR_TABLE_FREE_KEY(key)
#define WCSTR_TABLE_HASH(key, hash)        chash_string_hash(key, hash)
#define WCSTR_TABLE_FREE_VALUE(value)      NULL
#define WCSTR_TABLE_COMPARE(cmp_a, cmp_b)  chash_string_compare(cmp_a, cmp_b)
#define WCSTR_TABLE_INIT(bucket, _key, _value)   strcpy((bucket).key, _key); \
                                                  (bucket).value = _value;

typedef struct {
	char key[1024];
	int value;
	int state;
} wCStrBucket;

#define WTABLE(_bucket) 					\
	struct { 							\
		int length; 					\
		int capacity; 					\
		_bucket *buckets; 				\
	}

_W_PRIVATE _W_HOT char *intToHex(char *value) {
	int input;
	char *res;
	input = atoi(value);
	res = calloc(strlen(value)+1024, sizeof(int));
	sprintf(res, "0x%x", input);
	return res;
}

_W_PRIVATE _W_HOT char *floatToHex(char *value) {
	float input;
	uint32_t convVal;
	char *res;
	input = atof(value);
	convVal = 0;
	memcpy(&convVal, &input, sizeof(float));
	res = calloc(strlen(value)+1024, sizeof(int));
	sprintf(res, "0x%x", convVal);
	return res;
}

/*because fuck the log10 shift magic fucking up old hardware*/
_W_PRIVATE int wGetDigits(int val) {
	int digits = 0;
	int n = abs(val);
	if(!n) return 1;
	for(;n>0;digits++) n /= 10;
	return digits;
}

/*C char ptr utilities*/

_W_PRIVATE int isCharAny(char c, char *any) {
	int i = 0, size;
	if(!any) return -1;
	size = strlen(any);
	for(;i<size;i++) {
		if(any[i]==c) return 1;
	}
	return 0;
}

_W_PRIVATE int findIndexOfAnyRight(char *str, char *any) {
	int i = 0;
	if(!str||!any) return -1;
	for(i=strlen(str)-1;i>=0;i--) {
		if(isCharAny(str[i], any)) return i;
	}
	return i;
} 

_W_PRIVATE char *removeUpTo(char *str, int i) {
	char *n = NULL;
	char *res = NULL;
	int start = 0, size = 0;
	if(!str||i<0) return n;
	n = str + i+1;
	res = (char *)wAlloc((_wGetCStrSize(n)+1)*sizeof(char));
	strcpy(res, n);
	return res;
}

_W_PRIVATE char *stripFilenameFromPath(char *path) {
	int i;
	if(!path) return NULL;
	i = findIndexOfAnyRight(path, "\\/");
	return removeUpTo(path, i);
}

/*Basic string utilities*/

typedef struct {
	char *data;
	unsigned int count;
	unsigned int reserved;
} wString;

_W_PRIVATE wString wInitString(int reserved) {
	wString res;
	if(reserved>0) {
		res.reserved = (unsigned int)reserved;
		res.data = (char *)wAlloc(res.reserved*sizeof(char));
	} else {res.data = NULL; res.reserved = 0;}
	res.count = 0; 
	return res;
}

_W_PRIVATE wString wToString(char *data) {
	wString res;
	res.data = data;
	res.count = _wGetCStrSize(data);
	res.reserved = 0;
	return res;
}

_W_PRIVATE char *wToCString(wString string) {
	string.data[string.count] = '\0';
	return string.data;
}

_W_PRIVATE void wAssign(wString *string, char *data) {
	int i = 0;
	if(!data) return;
	i = _wGetCStrSize(data);
	if(!string->reserved) {
		string->data = (char *)wAlloc(i*sizeof(char));
		string->reserved = i;
	}
	if(i>string->reserved) {
		string->data = (char *)wResize(string->data, i*sizeof(char));
		string->reserved = i;
	}
	string->count = i;
	memcpy(string->data, data, string->count*sizeof(char));
}

_W_PRIVATE void wRemove(wString *string) {
	if(!string) return;
	if(!string->reserved) return;
	wFree(string->data);
	string->data = NULL;
	string->reserved = 0;
	string->count = 0;
}

_W_PRIVATE void wAppend(wString *dst, wString *input) {
	int cursor = 0, newSize = 0;
	char *start = NULL;
	if(!input) return;
	newSize = dst->count+input->count-1; 
	if(newSize>dst->reserved) 
		dst->data = (char *)wResize(dst->data, newSize*sizeof(char));
	if(dst->count>0) cursor = dst->count-1;
	else cursor = dst->count;
	start = dst->data+cursor;
	memcpy(start, input->data, input->count*sizeof(char));
	dst->count = newSize;
	dst->reserved = newSize;
}

_W_PRIVATE void wAppendLine(wString *dst, wString *input) {
	wString nl = wInitString(1);
	wAssign(&nl, "\n");
	wAppend(dst, &nl);
	wRemove(&nl);
	wAppend(dst, input);
}

_W_PRIVATE void wCAppend(wString *dst, char *input) {
	int cursor = 0, newSize = 0;
	int inputSize = 0;
	char *start = NULL;
	if(!input) return;
	inputSize = _wGetCStrSize(input);
	if(!dst) *dst = wInitString(inputSize);
	newSize = dst->count+inputSize-1; 
	if(newSize>dst->reserved) 
		dst->data = (char *)wResize(dst->data, newSize*sizeof(char));
	if(dst->count>0) cursor = dst->count-1;
	else cursor = dst->count;
	start = dst->data+cursor;
	memcpy(start, input, inputSize*sizeof(char));
	dst->count = newSize;
	dst->reserved = newSize;
}

_W_PRIVATE void wCAppendLine(wString *dst, char *input) {
	wCAppend(dst, "\n");
	wCAppend(dst, input);
}

/*Fmt and printing*/

_W_PRIVATE void _wFmtInt(wString *string, int i) {
	int MAXOUTLEN = 64;
	int base = 10;
	int digits;
	char *cursor = NULL;
	int j = i;
	wString res;
	digits = wGetDigits(i);
	res = wInitString(MAXOUTLEN);
	if(i<0) {
		char n = '-';
		i = abs(i);
		wCAppend(&res, &n);
	}
	res.count += digits;
	cursor = res.data+res.count-1;
	if(!i) *cursor = '0';
	else {
		for(;i>0;i/=10) {
			*cursor = i%base+'0';
			cursor--;
		}
	}
	res.data[res.count] = '\0';
	res.count++;
	wAppend(string, &res);
}

_W_PRIVATE void _wFmtStr(wString *string, wString s) {
	wAppend(string, &s);
}

_W_PRIVATE void _wCFmtStr(wString *string, char *s) {
	wCAppend(string, s);
}

/*@TODO handle floats and maybe some stadard wellang structs or something.*/
_W_PRIVATE wString wCFmt(char *fmt, ...) {
	int i, fmtSize = _wGetCStrSize(fmt);
	wString res = wInitString(fmtSize);
	va_list args;
	wAssign(&res, "\0");
	
	va_start(args, fmt);
	for(i=0;i<fmtSize-1;i++) {
		char c = fmt[i];
		char tmp[2];
		char next = '\0';
		char last = '\0';
		tmp[0] = c;
		tmp[1] = '\0';
		if(i+1<fmtSize-1) next = fmt[i+1];
		if(i-1>=0) last = fmt[i-1];
		if(c=='%'&&next!='\0') {
			switch(next) {
				case 'd': _wFmtInt(&res, va_arg(args, int)); break;
				case 's': _wCFmtStr(&res, va_arg(args, char *)); break;
				case 'w': _wFmtStr(&res, va_arg(args, wString)); break;
				default: wCAppend(&res, tmp); break;
			}
		} else if(last!='%') wCAppend(&res, tmp);
	}
	va_end(args);

	return res;
}

#endif
