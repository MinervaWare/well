/*Copyright (c) 2024-2026 MinervaWare LLC*/
/*Copyright (c) 2022-2024 Tristan Wellman*/
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
	S390,
	S390X,
	IBM_Z,
	MIPS,
	MIPS64,
	HPPA,
	SPU, /*CBE SPU. Must be manually called for linkage with ppc*/
	RV64, /*RISC-V 64 generic*/
	RV32
};

static enum cpuType CPU =
#if defined __alpha__ || _M_ALPHA
	ALPHA;
#elif defined __amd64__ || __x86_64__
	AMD_X86_64;
#elif defined __arm__ || _M_ARM || \
		__ARM_ARCH_7__ || __aarch64__
#	if defined __APPLE__
		ARM_MAC;
#	else
#	if defined __ARM_ARCH_7A__ || __ARM_ARCH_7R__ \
		|| __ARM_ARCH_7M__ || __ARM_ARCH_7EM__
		ARMv7;
#	elif defined __ARM_ARCH_8A || __ARM_ARCH_8A__ || __ARM_ARCH_81A__ \
		|| __ARM_ARCH_82A__ || __aarch64__
		ARMv8;
#	endif
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
#elif defined __370__
#	error "Wellang does not currently support s370!"
#elif defined __zarch__
	IBM__Z;
#elif defined __s390__
	S390;
#elif defined __s390x
	S390X;
#elif defined __mips__ || mips || __mips
	MIPS;
#elif defined __mips64__ || __mips64
	MIPS64;
#elif defined __hppa__ || __HPPA__ || __hppa
	HPPA;
#elif defined __riscv
#	if defined __riscv_xlen && __riscv_xlen==32
		RV32;
#	else
		RV64;
#	endif
#else
#	warning "Unsupported CPU architecture, defaulting to I386." 
	I386; /*Default to x86*/
#endif

#define SPUGCC "spu-elf-gcc"
#define SPUEMB "embedspu"

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
		case S390: str="System/390";break; \
		case S390X: str="System/s90x";break; \
		case IBM_Z: str="System/Z";break; \
		case MIPS: str="MIPS";break; \
		case MIPS64: str="MIPS64";break; \
		case HPPA: str="HPPA";break; \
		case SPU: str="SPU";break; \
		case RV64: str="RISC-V64 Generic";break; \
		case RV32: str="RISC-V32 Generc";break;};

#define SETCPUENUM(_str) { \
		if(!strcmp(_str, "ALPHA")) CPU = ALPHA; \
		else if(!strcmp(_str, "AMD_X86_64")) CPU = AMD_X86_64; \
		else if(!strcmp(_str, "ARM_MAC")) CPU = ARM_MAC; \
		else if(!strcmp(_str, "ARMv7")) CPU = ARMv7; \
		else if(!strcmp(_str, "ARMv8")) CPU = ARMv8; \
		else if(!strcmp(_str, "I386")) CPU = I386; \
		else if(!strcmp(_str, "POWERPC")) CPU = POWERPC; \
		else if(!strcmp(_str, "RS6000")) CPU = RS6000; \
		else if(!strcmp(_str, "SPARC")) CPU = SPARC; \
		else if(!strcmp(_str, "S390")) CPU = S390; \
		else if(!strcmp(_str, "S390X")) CPU = S390X; \
		else if(!strcmp(_str, "IBM_Z")) CPU = IBM_Z; \
		else if(!strcmp(_str, "MIPS")) CPU = MIPS; \
		else if(!strcmp(_str, "MIPS64")) CPU = MIPS64; \
		else if(!strcmp(_str, "HPPA")) CPU = HPPA; \
		else if(!strcmp(_str, "SPU")) CPU = SPU; \
		else if(!strcmp(_str, "RV32")) CPU = RV32; \
		else if(!strcmp(_str, "RV64")) CPU = RV64;}

#endif
