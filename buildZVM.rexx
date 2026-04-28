/*REXX*/

/*Copyright (c) 2024-2026 MinervaWare LLC*/
/*Copyright (c) 2022-2024 Tristan Wellman*/

/* Well Build Script for Z/VM 4.4*/

/* Save all the Well src and include files together in a VMARC */
/* Save this as an EXEC file or use REXXC */

'GLOBAL TXTLIB SCEELKED'
'GLOBAL lOADLIB SCEERUN'

say ' - - Copyright (c) 2024-2026 MinervaWare LLC - -'
say ' - - Copyright (c) 2022-2024 Tristan Wellman - -'
say ' - - 		Compiling Wellang for Z/VM 		  - -'

src. = ''
	src.1 = 'ALPHA'
	src.2 = 'AMDX8664'
	src.3 = 'ARM_MAC'
	src.4 = 'ARMv7'
	src.5 = 'asmout'
	src.6 = 'main'
	src.7 = 'MIPS32'
	src.6 = 'parser'
	src.8 = 'PPC'
	src.9 = 'util'
	src.9 = 'extract'
	src.10 = 'ap_inter'
	src.11 = 'argparse'

args = ''

do i=1 while src.i <> ''
	'CC' src.i '(LONGNAME'
	args = args src.i
	say '    CC' src.i
end

'CPLINK' args
'GENMOD CPOBJ'
