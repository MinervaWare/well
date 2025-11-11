/*Copyright (c) 2024 Tristan Wellman*/
#ifndef CPU_H
#define CPU_H

enum cpuType {
	ALPHA,
	AMD_X86_64, /*64-bit x86*/
	I386, /*32-bit x86*/
	ARM_MAC,
	ARMv7, /*32-bit arm*/
	ARMv8,
	ITANIUM_64, /*You know good and well I ain't ever getting around to this*/
	POWERPC,
	RS6000,
	SPARC,
	SZ_IBM, /*System 370, 390*/
};

static enum cpuType CPU =
#if defined __alpha__ || _M_ALPHA
	ALPHA;
#elif defined __amd64__ || __x86_64__
	AMD_X86_64;
#elif defined __arm__ || _M_ARM || \
		__ARM_ARCH_7__ || __aarch64__
#if defined __APPLE__
	ARM_MAC;
#else
#if defined __ARM_ARCH_7A__ || __ARM_ARCH_7R__ \
		|| __ARM_ARCH_7M__ || __ARM_ARCH_7EM__
	ARMv7;
#elif defined __ARM_ARCH_8A__ || __ARM_ARCH_81A__ \
		|| __ARM_ARCH_82A__
	ARMv8
#endif
#endif
#elif defined __i386__ || _M_IX86 || \
		__X86__ || _x86_ /*mingw & watcom*/
	I386;
#elif defined  __ia64__ || _M_IA64 || __itanium__
	ITANIUM_64;
#elif defined __PPC__ || __powerpc64__ || _M_PPC
	POWERPC;
#elif defined __THW_RS6000 || _POWER
	RS6000;
#elif defined __sparc__ || __sparcv8 || __sparcv9
	SPARC;
#elif defined __370__ || __s390__ || __s390x__ || __zarch__
	SZ_IBM;
#endif

#define GETCPUSTR(cpu_t, str) \
	switch(cpu_t) {\
		case ALPHA: str="ALPHA";break; \
		case AMD_X86_64: str="AMD_X86_64";break; \
		case ARM_MAC: str="ARM_MAC";break; \
		case ARMv7: str="ARMv7";break; \
		case ARMv8: str="ARMv8";break; \
		case I386: str="I386";break; \
		case ITANIUM_64: str="ITANIUM_64";break; \
		case POWERPC: str="POWERPC";break; \
		case RS6000: str="RS/6000";break; \
		case SPARC: str="SPARC";break; \
		case SZ_IBM: str="System/370-90";break;};

#endif
