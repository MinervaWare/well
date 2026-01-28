/*Copyright (c) 2022-2025 MinervaWare LLC*/
#include <sys/types.h>
#include <time.h>
#include "util.h"
#include "wdata.h"
#include "parser.h"
#include "werror.h"
#include "asmout.h"
#include "cpu.h"

wData *data = NULL;

int argCheckOption(struct ArgparseParser *parser,
		char *option1, char *option2);
void runArgParsing(wData *data);
void initArgParseArgs(wData *data, int argc, char *argv[]); 
void compileFile(wData *data); 

int main(int argc, char **argv) {

	clock_t start, end;
	struct parserData *p;
	char *cpuStr = NULL;
	char timeBuf[100];
	AsmOut output;

	start = clock();

	data = calloc(1, sizeof(wData));
	data->main = NULL;
	data->fileName = NULL;
	data->includedFiles = NULL;
	data->includeSize = 0;
	data->outputFile = NULL;
	data->ccFlags = NULL;
	data->flags = NULL;
	data->flagLen = 0;
	data->cap = 0;
	data->ldflags = NULL;
	data->KEEPASM = 0;
	data->USEINFO = 0;
	data->USELD = 0;
	data->COBJ = 0;
	initArgParseArgs(data, argc, argv);
	runArgParsing(data);
	data->cpu = CPU;

	WASSERT(data->main!=NULL,
			"FATAL:: No File provided\n");

	p = initParser(data);
	parseProgram(p);

	initAsmOut(p, &output);
	convertToAsm(&output);

	/*freeParser(p);
	 * This breaks on FreeBSD so I'm not gonna worry about it for now*/
	freeAsm(&output);
	argparse_free(data->argParser);
	if(data->main!=NULL) fclose(data->main);

	end = clock();
	GETCPUSTR(CPU, cpuStr);
	snprintf(timeBuf, sizeof(timeBuf), "Compile time: %fs, %fms on %s", 
			((double)(end-start) / CLOCKS_PER_SEC), 
			(((double)(end-start) / CLOCKS_PER_SEC) * 1000), cpuStr);
	WLOG(INFO, timeBuf);

	WERROR_EXIT();

	compileFile(data);
	free(data);
	return 0;
}

int argCheckOption(struct ArgparseParser *parser,
		char *option1, char *option2) {
	if(argparse_option_exists(*parser, option1)!=ARGPARSE_NOT_FOUND ||
			argparse_option_exists(*parser, option2)!=ARGPARSE_NOT_FOUND) return 1;
	return 0;
}

#define HELPMLEN 1024
void outputCPUHelp() {
	char *cpuHelp = calloc(HELPMLEN, sizeof(char));
	snprintf(cpuHelp, sizeof(char)*HELPMLEN,
		"WELLANG CPU OPTIONS:\n\n"
		"Architectures with \"(TODO)\" are not yet implemented with Wellang.\n"
		"Usage Example: well foo.well --arch AMD_X86_64\n"
		"AMD_X86_64   | Intel/AMD x86_64 architecture\n%s",
		"I386         | 32-bit Intel/AMD x86 architecture\n"
		"ARMv8        | aarch64 architecture\n"
		"ARM_MAC      | Outputs the same as ARMv8\n"
		"ARMv7  (TODO)| 32-bit aarch architecture\n"
		"POWERPC(TODO)| 32-bit and 64-bit ppc architecture\n%s"
		"RS6000 (TODO)| IBM POWER architecture\n"
		"ALPHA        | Alpha architecture\n"
		"SZ_IBM (TODO)| IBM s370/390 architecture\n"
		"SPARC  (TODO)| Sun Sparc architecture\n"
		"MIPS   (TODO)| Mips architecture\n"
		"HPPA   (TODO)| HP Precision Architecture\n");
	WLOG(INFO, cpuHelp);
	free(cpuHelp);
	exit(0);
}

void runArgParsing(wData *data) {
	char *help = calloc(HELPMLEN, sizeof(char)); 
	snprintf(help, sizeof(char)*HELPMLEN,
			"WELLANG CLI HELP:\n\n"
			"--help      | -h: Need help? Use -h\n"
			"--output    | -o: Set name of executable output\n"
			"--cobject   | -c: Compiler to object(.o) file.\n%s",
			"--cflags    | -cf: Set your cflags (ex: well main.well -cc '-g -lpthread')\n"
			"--assembly  | -a: Keep the assembly output file\n"
			"--info      | -i: Shows extra debug information at compile time\n\n"
			"--use-gnuld | -use-ld: Use gnu linker rather than gcc\n%s",
			"--ldflags   | -ldflags: Set your gnu linker flags (ex: wesm main.well -use-ld -ldflags '-T link.ld')\n"
			"--arch      | -arch: Set the output architecture, this requires your toolchain to support cross-compilation");

	if(argCheckOption(&data->argParser, "--help", "-h")) {
		WLOG(INFO, help);
		free(help);
		exit(0);
	}

	if(argCheckOption(&data->argParser, "--output", "-o")) {
		int i;
		for(i=0;i<data->argParser.argc;i++) {
			if(!strcmp(data->argParser.argv[i], "-o")||
				!strcmp(data->argParser.argv[i], "--object")) {
				if(i+1<data->argParser.argc) {
					data->outputFile = data->argParser.argv[i+1];
					break;
				}
			}
		}	
	}

	if(argCheckOption(&data->argParser, "--cobject", "-c")) 
		data->COBJ=1;
	if(argCheckOption(&data->argParser, "--info", "-i")) 
		data->USEINFO=1;
	if(argCheckOption(&data->argParser, "--assembly", "-a"))
		data->KEEPASM=1;
	
	if(argCheckOption(&data->argParser, "--cflags", "-cf")) {
		int i;
		for(i=0;i<data->argParser.argc;i++) {
			if(!strcmp(data->argParser.argv[i], "-cf")||
				!strcmp(data->argParser.argv[i], "--cflags")) {
				if(i+1<data->argParser.argc) {
					data->ccFlags = data->argParser.argv[i+1];
					break;
				}
			}
		}	
	}

	if(argCheckOption(&data->argParser, "--arch", "-arch")) {
		int i;
		for(i=0;i<data->argParser.argc;i++) {
			if(!strcmp(data->argParser.argv[i], "-arch")||
				!strcmp(data->argParser.argv[i], "--arch")) {
				if(i+1<data->argParser.argc) {
					if(!strcmp(data->argParser.argv[i+1], "help"))
						outputCPUHelp();
					SETCPUENUM(data->argParser.argv[i+1]); 
					break;
				}
			}
		}	
	}
	free(help);
}

void initArgParseArgs(wData *data, int argc, char **argv) {
	int i;
	data->argParser = argparse_init("well", argc, argv);
	data->KEEPASM=0;
	data->USEINFO=0;
	data->USELD=0;
	data->COBJ=0;
	data->outputFile="a.out";
	data->ccFlags=NULL;
	data->ldflags=NULL;
	data->flagLen=0;
	data->cap=1;
	data->flags = calloc(1, sizeof(char *));

	for(i=0;i<argc;i++) {
		if(strstr(argv[i], ".well")) {
			char *buf;
			data->main = fopen(argv[i], "r");
			buf = calloc(128, sizeof(char));
			snprintf(buf, sizeof(char)*128, "FATAL:: Failed to open file: %s", argv[i]);
			WASSERT(data->main!=NULL, buf);
			data->fileName = argv[i];
			free(buf);
			break;
		}
}

	WASSERT(&data->argParser!=NULL,
			"FATAL:: Failed to init argParse");


	argparse_add_argument(&data->argParser, "arg1");

	argparse_add_option(&data->argParser, "--help", "-h", ARGPARSE_FLAG);
	argparse_add_option(&data->argParser, "--output", "-o", ARGPARSE_FLAG);
	argparse_add_option(&data->argParser, "--cobject", "-c", ARGPARSE_FLAG);
	argparse_add_option(&data->argParser, "--cflags", "-cf", ARGPARSE_FLAG);
	argparse_add_option(&data->argParser, "--assembly", "-a", ARGPARSE_FLAG);
	argparse_add_option(&data->argParser, "--info", "-i", ARGPARSE_FLAG);
	argparse_add_option(&data->argParser, "--use-gnuld", "-use-ld", ARGPARSE_FLAG);
	argparse_add_option(&data->argParser, "--ldflags", "-ldflags", ARGPARSE_FLAG);
	
	/*argparse_error(data->argParser);*/
	
}

void cleanupAsm(wData *data, char *asmOut) {
	char *args[3];
	if(data->KEEPASM) return;
	args[0] = "rm";
	args[1] = asmOut;
	args[2] = NULL;
	execvp("rm", args);
}

void tokenizeCCFlags(wData *data) {
	char *token = strtok(data->ccFlags, " ");
	for(;token!=NULL;data->flagLen++) {
		data->flags[data->flagLen] = calloc(strlen(token)+1, sizeof(char *));
		strcpy(data->flags[data->flagLen], token);
		data->cap++;
		data->flags = (char **)realloc(data->flags, sizeof(char *)*data->cap);
		token = strtok(NULL, " ");
		if(data->flagLen==0&&token==NULL) break;
	}
}

void compileFile(wData *data) {
	char *fileDirect = strtok(data->fileName, ".");
	char *args[64];
	int i;
	pid_t pid;
	for(i=0;i<64;i++) args[i] = NULL;
	strcat(fileDirect, ".s");
	if(data->USELD) return;
	if(data->outputFile!=NULL) {
		char buf[256];
		snprintf(buf, sizeof(buf), "-o%s", data->outputFile);
		data->outputFile = calloc(strlen(buf)+1, sizeof(char *));
        strcpy(data->outputFile, buf);
	}
	tokenizeCCFlags(data);
	args[0] = "gcc";
	args[1] = fileDirect;
	args[2] = "-c";
	args[3] = NULL;
	/*if(data->ccFlags==NULL) args[2] = ""; */
	/*if(data->COBJ) args[3] = "-c";*/
	/*if(data->outputFile==NULL) args[4] = NULL;*/
	if(data->USEINFO) {
		char *buf = calloc(256, sizeof(char));
		int i;
		for(i=0;i<ARRLEN(args)-1;i++) {
			char str[100];
			if(args[i]==NULL) break;
			snprintf(str, sizeof(str), "%s ", args[i]);
			strcat(buf, str);
		}
		WLOG(INFO, buf);
	}
	/*run compiler*/
	pid = fork();
	if(pid==0) {
		execvp("gcc", args);
	} else if(pid>0)  {
		int status;
		int i, l;
		char **linkArgs;
		int linkArgLen;
		waitpid(pid, &status, 0);

		for(i=0;i<data->includeSize;i++) {
			args[1] = calloc(strlen(data->includedFiles[i])+1, sizeof(char *));
            strcpy(args[1], data->includedFiles[i]);
			if(data->USEINFO) {
				char *buf = calloc(256, sizeof(char));
				int j;
				for(j=0;j<ARRLEN(args)-1;j++) {
					char str[100];
					if(args[i]==NULL) break;
					snprintf(str, sizeof(str), "%s ", args[j]);
					strcat(buf, str);
				}
				WLOG(INFO, buf);
			}
			pid = fork();
			if(pid==0) execvp("gcc", args);
			else if(pid>0) waitpid(pid, &status, 0);
		}
		/*link objs*/
		linkArgs = calloc(ARRLEN(args)+data->includeSize+data->flagLen+1, sizeof(char*));
		linkArgLen = 0;
		linkArgs[0] = "gcc";
		linkArgs[1] = data->outputFile;
		fileDirect = strtok(fileDirect, ".");
		strcat(fileDirect, ".o");
		linkArgs[2] = calloc(strlen(fileDirect)+1, sizeof(char *));
        strcpy(linkArgs[2], fileDirect);
		linkArgLen = 3;
		for(i=0;i<data->includeSize;i++) {
			char *curInclude = strtok(data->includedFiles[i], ".");
			strcat(curInclude, ".o");
			linkArgs[i+3] = calloc(strlen(curInclude)+1, sizeof(char *));
            strcpy(linkArgs[i+3], curInclude);
		}
		linkArgLen += i;
		/*Get C flags*/
		for(l=0;l<data->flagLen;l++) {
			linkArgs[l+data->includeSize+3] = calloc(strlen(data->flags[l])+1, sizeof(char *));
			strcpy(linkArgs[l+data->includeSize+3], data->flags[l]);
		}
		linkArgs[l+data->includeSize+4] = NULL;
		linkArgLen += l;	
		/*trim up linkArgs*/
		for(i=0;i<linkArgLen;i++) {
			if((linkArgs[i]==NULL||!strcmp(linkArgs[i],""))
					&&i+1<linkArgLen) {
				linkArgs[i] = linkArgs[i+1];
				linkArgs[i+1] = NULL;
				if(linkArgs[i]==NULL) linkArgs[i] = "";
			}
		}
		if(data->USEINFO) {
			char *buf = calloc(256, sizeof(char));
			int j;
			for(j=0;linkArgs[j]!=NULL;j++) {
				char str[100];
				if(linkArgs[j]==NULL) break;
				snprintf(str, sizeof(str), "%s ", linkArgs[j]);	
				strcat(buf, str);
			}
			WLOG(INFO, buf);
		}
		if(fork()==0) execvp("gcc", linkArgs);
		free(linkArgs[2]);
		free(linkArgs);
	}
	free(data->flags);
	/*cleanupAsm(data, fileDirect);*/
}

