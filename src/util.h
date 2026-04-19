/*Copyright (c) 2024-2026 MinervaWare LLC*/
/*Copyright (c) 2022-2024 Tristan Wellman*/

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
#include <chash.h>

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
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long int int64_t;
typedef unsigned long int uint64_t;
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

#define WSTR_TABLE_HEAP 1
#define WSTR_TABLE_BUCKET wStrBucket
#define WSTR_TABLE_FREE_KEY(key)
#define WSTR_TABLE_HASH(key, hash)        chash_string_hash(key, hash)
#define WSTR_TABLE_FREE_VALUE(value)      NULL
#define WSTR_TABLE_COMPARE(cmp_a, cmp_b)  chash_string_compare(cmp_a, cmp_b)
#define WSTR_TABLE_INIT(bucket, _key, _value)   strcpy((bucket).key, _key); \
                                                  (bucket).value = _value;
#define WINT_TABLE_HEAP 1
#define WINT_TABLE_BUCKET wIntBucket
#define WINT_TABLE_FREE_KEY(key)
#define WINT_TABLE_HASH(key, hash)        chash_string_hash(key, hash)
#define WINT_TABLE_FREE_VALUE(value)      NULL
#define WINT_TABLE_COMPARE(cmp_a, cmp_b)  chash_string_compare(cmp_a, cmp_b)
#define WINT_TABLE_INIT(bucket, _key, _value)   strcpy((bucket).key, _key); \
                                                  (bucket).value = _value;
typedef struct {
	char key[1024];
	int value;
	int state;
} wStrBucket;

typedef struct {
	int key;
	int value;
	int state;
} wIntBucket;

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
	snprintf(res, sizeof(res), "0x%x", input);
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
	snprintf(res, sizeof(res), "0x%x", convVal);
	return res;
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
	int start = 0, size = 0;
	if(!str||i<0) return n;
	n = str + i+1;
	return strdup(n);
}

_W_PRIVATE char *stripFilenameFromPath(char *path) {
	int i;
	if(!path) return NULL;
	i = findIndexOfAnyRight(path, "\\/");
	return removeUpTo(path, i);
}

/*Allocator(s): For now I will just call standard C 
 * 				functions until I do like a bump allocator or something
 */

typedef enum {
	ALLOC,
	RESIZE,
	FREE
} wAllocMode;

typedef void *(*WALLOCPROC)(int size, wAllocMode mode, void *oldMem);

typedef struct {
	WALLOCPROC allocProc;
} wAllocator;

_W_PRIVATE void *wDefaultAllocatorProc(int size, wAllocMode mode, void *oldMem) {
	switch(mode) {
		case ALLOC: return malloc(size);
		case RESIZE: {
			WASSERT(oldMem!=NULL, "Passed NULL memory to resize!");
			return realloc(oldMem, size);
		}
		case FREE: {
			WASSERT(oldMem!=NULL, "Passed NULL memory to free!");
			free(oldMem); 
		}
	};
	return NULL;
}

static wAllocator wDefaultAllocator;

#define WINITDEFAULTALLOCATOR \
	wDefaultAllocator.allocProc = (WALLOCPROC)wDefaultAllocatorProc;

_W_PRIVATE void *wAlloc(int _size) {
	return wDefaultAllocator.allocProc((_size), ALLOC, NULL);}
_W_PRIVATE void *wResize(void *_oldMem, int _size) {
	return wDefaultAllocator.allocProc((_size), RESIZE, _oldMem);}
_W_PRIVATE void wFree(void *_oldMem) {
	wDefaultAllocator.allocProc(0, FREE, _oldMem);}

/*Basic string utilities*/

typedef struct {
	char *data;
	unsigned int count;
	unsigned int reserved;
} wString;

#define _wGetCStrSize(_data) (strlen(_data)+1)

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
	cursor = dst->count-1;
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
	cursor = dst->count-1;
	start = dst->data+cursor;
	memcpy(start, input, inputSize*sizeof(char));
	dst->count = newSize;
	dst->reserved = newSize;
}

_W_PRIVATE void wCAppendLine(wString *dst, char *input) {
	wCAppend(dst, "\n");
	wCAppend(dst, input);
}

#endif
