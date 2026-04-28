#ifndef OSNAME_H
#define OSNAME_H 1

/*
 * Modified by Tristan Wellman -  9/29/25
 * Fix the __MACH typo, Fix __VM__ and __MVS__ redefs on same-def systems.
 * */

/* C-Ware License
 *
 * Copyright (c) 2025, Tristan Wellman
 * Copyright (c) 2022, C-Ware
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Redistributions of modified source code must append a copyright notice in
 *    the form of 'Copyright <YEAR> <NAME>' to each modified source file's
 *    copyright notice, and the standalone license file if one exists.
 *
 * A "redistribution" can be constituted as any version of the source code
 * that is intended to comprise some other derivative work of this code. A
 * fork created for the purpose of contributing to any version of the source
 * does not constitute a truly "derivative work" and does not require listing.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* This enum stores a "best-approximation" OS list */
#define OS_UNIX 1      /* UNIX and UNIX-like systems, including POSIX compatibility layers */
#define OS_WINDOWS 2   /* Microsoft Windows and its successors, including Win16/3.1 */
#define OS_DOS 3       /* Microsoft MS-DOS, IBM PC-DOS DRA DR-DOS */
#define OS_OS2 4       /* IBM OS/2 2.x */
#define OS_S370 5      /* IBM MVS/370, MVS/SP, MVS/XA, MVS/ESA, OS/390, z/OS, VM/370, VM/SP, VM/ESA, z/VM, VSE/ESA, z/VSE, VSEn */
#define OS_DEC 6       /* DEC RSX-11, (Open)VMS, RSTS/E, TOPS-10, TOPS-20 */
#define OS_MACINTOSH 7 /* Classic Mac OS (OS X is considered to be a UNIX) */
#define OS_AMIGA 8     /* AmigaOS (<4 and 4), MorphOS, AROS */
#define OS_OTHER 9     /* Placeholder for systems with either partial POSIX compatiblity or is of a class not listed above */

#ifdef _AIX
#    define OSNAME "AIX"
#    define OSCLASS OS_UNIX
#endif

#ifdef __ANDROID__
#    define OSNAME "Android"
#    define OSCLASS OS_UNIX
#endif

#ifdef UTS
#    define OSNAME "UTS"
#    define OSCLASS OS_UNIX
#endif

#ifdef aegis
#    define OSNAME "Aegis"
#    define OSCLASS OS_UNIX
#endif

#ifdef __BEOS__
#    define OSNAME "BeOS"
#    define OSCLASS OS_OTHER
#endif

#ifdef __FreeBSD__
#    define OSNAME "FreeBSD"
#    define OSCLASS OS_UNIX
#endif

#ifdef __NetBSD__
#    define OSNAME "NetBSD"
#    define OSCLASS OS_UNIX
#endif

#ifdef __OpenBSD__
#    define OSNAME "OpenBSD"
#    define OSCLASS OS_UNIX
#endif

#ifdef __bsdi__
#    define OSNAME "BSD/OS"
#    define OSCLASS OS_UNIX
#endif

#ifdef __DragonFly__
#    define OSNAME "DragonFly BSD"
#    define OSCLASS OS_UNIX
#endif

#ifdef __convex__
#    define OSNAME "ConvexOS"
#    define OSCLASS OS_UNIX
#endif

#ifdef __CYGWIN__
#    define OSNAME "Windows NT (Cygwin)"
#    define OSCLASS OS_UNIX
#endif

#if defined __DGUX__ || DGUX
#    define OSNAME "DG/UX"
#    define OSCLASS OS_UNIX
#endif

#if defined __SEQUENT__ || sequent
#    define OSNAME "DYNIX/ptx"
#    define OSCLASS OS_UNIX
#endif

#ifdef __ECOS
#    define OSNAME "eCos"
#    define OSCLASS OS_OTHER
#endif

#ifdef __EMX__
#    define OSNAME "OS/2 (EMX)"
#    define OSCLASS OS_UNIX
#endif

#ifdef __gnu_hurd__
#    define OSNAME "GNU/Hurd"
#    define OSCLASS OS_UNIX
#endif

#if defined __gnu_linux__ || defined __linux__ || defined linux
#    define OSNAME "GNU/Linux"
#    define OSCLASS OS_UNIX
#endif

#if defined _hpux || defined hpux || defined __hpux
#    define OSNAME "HP-UX"
#    define OSCLASS OS_UNIX
#endif

#ifdef __OS400__
#    define OSNAME "OS/400"
#    define OSCLASS OS_OTHER
#endif

#if defined __sgi || defined sgi
#    define OSNAME "IRIX"
#    define OSCLASS OS_UNIX
#endif

#ifdef __INTEGRITY
#    define OSNAME "INTEGRITY"
#    define OSCLASS OS_OTHER
#endif

#ifdef __Lynx__
#    define OSNAME "LynxOS"
#    define OSCLASS OS_OTHER
#endif

#if defined macintosh || defined Macintosh
#    define OSNAME "Classic Mac OS"
#    define OSTYPE MACINTOSH
#endif

#ifdef __APPLE__
#    ifdef __MACH__
#        define OSNAME "Mac OS X"
#        define OSCLASS OS_UNIX
#    endif
#endif

#if defined __OS9000 || defined _OSK
#    define OSNAME "OS-9"
#    define OSCLASS OS_OTHER
#endif

#ifdef __MORPHOS__
#    define OSNAME "MorphOS"
#    define OSCLASS OS_AMIGA
#endif

#ifdef __AROS__
#    define OSNAME "AROS"
#    define OSCLASS OS_AMIGA
#endif

#if defined AMIGA || defined __amigaos__
#    define OSNAME "AmigaOS"
#    define OSCLASS OS_AMIGA
#endif

#if defined mpeix || defined __mpexl
#    define OSNAME "MPE/iX"
#    define OSCLASS OS_OTHER
#endif

#if defined MSDOS || defined __MSDOS__ || defined _MSDOS || defined __DOS__
#    define OSNAME "MS-DOS"
#    define OSCLASS OS_DOS
#endif

#ifdef __TANDEM
#    define OSNAME "NonStop OS"
#    define OSCLASS OS_OTHER
#endif

#if defined OS2 || defined _OS2 || defined __OS2__ || defined __TOS_OS2__
#    define OSNAME "OS/2"
#    define OSCLASS OS_OS2
#endif

#ifdef EPLAN9
#    define OSNAME "Plan 9"
#    define OSCLASS OS_OTHER
#endif

#if defined __QNX__ || defined __QNXNTO__
#    define OSNAME "QNX"
#    define OSCLASS OS_UNIX
#endif

#if defined sun || defined __sun
#    if defined __SVR4 || defined __svr4
#        define OSNAME "Solaris"
#        define OSCLASS OS_UNIX
#    else
#        define OSNAME "SunOS"
#        define OSCLASS OS_UNIX
#    endif
#endif

#ifdef __VOS__
#    define OSNAME "VOS"
#    define OSCLASS OS_OTHER
#endif

#if defined __osf__ || defined __osf
#    define OSNAME "OSF/1"
#    define OSCLASS OS_UNIX
#endif

#if defined ultrix || defined __ultrix || defined __ultrix__ || defined __SYSTYPE_BSD || defined __ULTRIX__
#    define OSNAME "ULTRIX"
#    define OSCLASS OS_UNIX
#endif

#if defined sco 
#   if defined _UNIXWARE7
#       define OSNAME "UnixWare"
#       define OSCLASS OS_UNIX
#   else
#       define OSNAME "OpenServer"
#       define OSCLASS OS_UNIX
#   endif
#endif

#if defined VMS || defined __VMS
#    define OSNAME "VMS"
#    define OSCLASS OS_VMS
#endif

#ifdef __VM__
#    define OSNAME "VM/CMS"
#    define OSCLASS OS_S370
#elif defined(__MVS__)
#    define OSNAME "MVS"
#    define OSCLASS OS_S370
#elif defined(__EDC_LE)
#    ifndef __VM__
#        define OSNAME "VSE"
#        define OSCLASS OS_S370
#    elif defined( __MVS__)
#        define OSNAME "VSE"
#        define OSCLASS OS_S370
#    endif
#endif

#if defined __MCP__
#    define OSNAME "MCP"
#    define OSCLASS OS_OTHER
#endif

#if defined _NETWARE_ || defined __NETWARE__
#    define OSNAME "NetWare"
#    define OSCLASS OS_OTHER
#endif

#ifdef __MACH__
#    ifndef __APPLE__
#        define OSNAME "NeXTSTEP"
#        define OSCLASS OS_UNIX
#    endif
#endif

#ifdef pyr
#    define OSNAME "DC/OSx"
#    define OSCLASS OS_UNIX
#endif

#if defined sinux || defined sinix
#    define OSNAME "Reliant UNIX"
#    define OSCLASS OS_UNIX
#endif

#ifdef _UNICOS
#    define OSNAME "UNICOS"
#    define OSCLASS OS_UNIX
#endif

#if defined _CRAY || defined _crayx1
#    define OSNAME "UNICOS/mp"
#    define OSCLASS OS_UNIX
#endif

#ifdef _UWIN
#    define OSNAME "Windows NT (U/Win)"
#    define OSCLASS OS_WINDOWS
#endif

#if defined __VXWORKS__ || defined __vxworks
#    define OSNAME "VxWorks"
#    define OSCLASS OS_OTHER
#endif

#ifdef _WIN32_WCE
#    define OSNAME "Windows CE"
#    define OSCLASS OS_WINDOWS
#endif

/* Double-test to weed out NT POSIX subsystem */
#if defined _WIN32 || defined _WIN64 || defined __WIN32__
#    ifndef _POSIX
#        define OSNAME "Windows NT"
#        define OSCLASS OS_WINDOWS
#    endif
#endif

#if defined _WIN32
#    if defined _POSIX
#        define OSNAME "Windows NT (POSIX subsystem)"
#        define OSCLASS OS_WINDOWS
#    endif
#endif

#ifdef _WIN16
#    define OSNAME "Windows 3.x"
#    define OSCLASS OS_WINDOWS
#endif

/* This tests for 4.x BSD */
#ifndef OSNAME
#    ifdef unix
#    include <sys/param.h>
#        ifdef BSD4_2
#            define OSNAME "4.2 BSD"
#            define OSCLASS OS_UNIX
#        endif
#        ifdef BSD4_3
#            define OSNAME "4.3 BSD"
#            define OSCLASS OS_UNIX
#        endif
#        ifdef BSD4_4
#            define OSNAME "4.4 BSD"
#            define OSCLASS OS_UNIX
#        endif
#        ifdef _SCO_NAMELEN
#            define OSNAME "SCO OpenServer/UnixWare"
#            define OSCLASS OS_UNIX
#        endif
#    endif
#endif

#endif
