/*Copyright (c) 2024-2026 MinervaWare LLC*/
/*Copyright (c) 2022-2024 Tristan Wellman*/
#ifndef WDATA_H
#define WDATA_H

#include "util.h"
#include "cpu.h"
#include "../include/argparse.h"

typedef struct {
	FILE *main;
	char *fileName;
	char **includedFiles;
	int includeSize;
	struct ArgparseParser argParser;	
	
	int KEEPASM, USEINFO, USELD, COBJ;
	char *outputFile;

	char *ccFlags;
	char **flags;
	int flagLen, cap;

	char *ldflags;
	enum cpuType cpu;
} wData;

#endif
