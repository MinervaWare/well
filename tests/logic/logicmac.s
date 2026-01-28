	.section __TEXT,__text
	.global _logic
	.p2align 2
_logic:
	sub sp, sp, #16
	stp x29, x30, [sp, #0]
	add x29, sp, #0
	adrp x0,wl_int_a@PAGE
	ldr w0, [x0, wl_int_a@PAGEOFF]
	mvn w1, w0
	sxtw x1, w1
	str x1, [sp]
	adrp x0,wl_str.ntest@PAGE
	add x0, x0, wl_str.ntest@PAGEOFF
	bl _printf
	ldp x29, x30, [sp, #0]
	add sp, sp, #16
	ret
	.section __TEXT,__text
	.global _math
	.p2align 2
_math:
	ldp x29, x30, [sp, #0]
	add sp, sp, #16
	ret
	.section __TEXT,__text
	.global _main
	.p2align 2
_main:
	sub sp, sp, #16
	stp x29, x30, [sp, #0]
	add x29, sp, #0
	adrp x0,wl_str.hello@PAGE
	add x0, x0, wl_str.hello@PAGEOFF
	bl _printf
	bl _logic
	bl _math
	mov x0, #0
	ldp x29, x30, [sp, #0]
	add sp, sp, #16
	ret
wl_str.hello:
	.asciz "- - - Bitwise/Arithmetic logic test - - -\n"

	.global wl_int_a
	.p2align 2,0x0
wl_int_a:
	.long 0x1

	.global wl_int_b
	.p2align 2,0x0
wl_int_b:
	.long 0x1

	.global wl_int_c
	.p2align 2,0x0
wl_int_c:
	.long 0x7c3
wl_str.ntest:
	.asciz "NOT::  Should be: -2.  Is: %d\n"
wl_str.atest:
	.asciz "AND::  Should be:  1.  Is: %d\n"
wl_str.otest:
	.asciz "OR::   Should be:  1.  Is: %d\n"
wl_str.notest:
	.asciz "NOR::  Should be: -2.  Is: %d\n"
wl_str.natest:
	.asciz "NAND:: Should be: -2.  Is: %d\n"
wl_str.xtest:
	.asciz "XOR::  Should be:  0.  Is: %d\n"
wl_str.adtest:
	.asciz "ADD::  Should be:  2.  Is: %d\n"
wl_str.sutest:
	.asciz "SUB::  Should be:  0.  Is: %d\n"
wl_str.mutest:
	.asciz "MUL::  Should be:  1.  Is: %d\n"
wl_str.ditest:
	.asciz "DIV::  Should be:  1.  Is: %d\n"
wl_str.motest:
	.asciz "MOD::  Should be:  0.  Is: %d\n"
