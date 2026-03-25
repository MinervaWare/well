	.extern printf
	.global logic
	.p2align 2
logic:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x0,wl_int_a@PAGE
	ldr w0, [x0, wl_int_a@PAGEOFF]
	mvn x1, x0
	adrp x0,wl_str_ntest@PAGE
	add x0, x0, wl_str_ntest@PAGEOFF
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	adrp x0,wl_int_a@PAGE
	ldr w0, [x0, wl_int_a@PAGEOFF]
	adrp x1,wl_int_b@PAGE
	ldr w1, [x1, wl_int_b@PAGEOFF]
	and x2, x0, x1
	mov x1, x2
	adrp x0,wl_str_atest@PAGE
	add x0, x0, wl_str_atest@PAGEOFF
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	adrp x0,wl_int_a@PAGE
	ldr w0, [x0, wl_int_a@PAGEOFF]
	adrp x1,wl_int_b@PAGE
	ldr w1, [x1, wl_int_b@PAGEOFF]
	orr x2, x0, x1
	mov x1, x2
	adrp x0,wl_str_otest@PAGE
	add x0, x0, wl_str_otest@PAGEOFF
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	adrp x0,wl_int_a@PAGE
	ldr w0, [x0, wl_int_a@PAGEOFF]
	adrp x1,wl_int_b@PAGE
	ldr w1, [x1, wl_int_b@PAGEOFF]
	orr x2, x0, x1
	mvn x2, x2
	mov x1, x2
	adrp x0,wl_str_notest@PAGE
	add x0, x0, wl_str_notest@PAGEOFF
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	adrp x0,wl_int_a@PAGE
	ldr w0, [x0, wl_int_a@PAGEOFF]
	adrp x1,wl_int_b@PAGE
	ldr w1, [x1, wl_int_b@PAGEOFF]
	and x2, x0, x1
	mvn x2, x2
	mov x1, x2
	adrp x0,wl_str_natest@PAGE
	add x0, x0, wl_str_natest@PAGEOFF
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	adrp x0,wl_int_a@PAGE
	ldr w0, [x0, wl_int_a@PAGEOFF]
	adrp x1,wl_int_b@PAGE
	ldr w1, [x1, wl_int_b@PAGEOFF]
	eor x2, x0, x1
	mov x1, x2
	adrp x0,wl_str_xtest@PAGE
	add x0, x0, wl_str_xtest@PAGEOFF
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
	.global math
	.p2align 2
math:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x0,wl_int_a@PAGE
	ldr w0, [x0, wl_int_a@PAGEOFF]
	adrp x1,wl_int_b@PAGE
	ldr w1, [x1, wl_int_b@PAGEOFF]
	add x2, x0, x1
	mov x1, x2
	adrp x0,wl_str_adtest@PAGE
	add x0, x0, wl_str_adtest@PAGEOFF
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	adrp x0,wl_int_a@PAGE
	ldr w0, [x0, wl_int_a@PAGEOFF]
	adrp x1,wl_int_b@PAGE
	ldr w1, [x1, wl_int_b@PAGEOFF]
	subs x2, x0, x1
	mov x1, x2
	adrp x0,wl_str_sutest@PAGE
	add x0, x0, wl_str_sutest@PAGEOFF
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	adrp x0,wl_int_a@PAGE
	ldr w0, [x0, wl_int_a@PAGEOFF]
	adrp x1,wl_int_b@PAGE
	ldr w1, [x1, wl_int_b@PAGEOFF]
	mul x2, x0, x1
	mov x1, x2
	adrp x0,wl_str_mutest@PAGE
	add x0, x0, wl_str_mutest@PAGEOFF
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	adrp x0,wl_int_a@PAGE
	ldr w0, [x0, wl_int_a@PAGEOFF]
	adrp x1,wl_int_b@PAGE
	ldr w1, [x1, wl_int_b@PAGEOFF]
	sdiv x2, x0, x1
	mov x1, x2
	adrp x0,wl_str_ditest@PAGE
	add x0, x0, wl_str_ditest@PAGEOFF
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	adrp x0,wl_int_a@PAGE
	ldr w0, [x0, wl_int_a@PAGEOFF]
	adrp x1,wl_int_b@PAGE
	ldr w1, [x1, wl_int_b@PAGEOFF]
	sdiv x2, x0, x1
	mul x2, x2, x2
	sub x2, x1, x2
	mov x1, x2
	adrp x0,wl_str_motest@PAGE
	add x0, x0, wl_str_motest@PAGEOFF
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
	.global _main
	.p2align 2
_main:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x0,wl_str_hello@PAGE
	add x0, x0, wl_str_hello@PAGEOFF
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	bl logic
	bl math
	mov x0, #0
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
wl_str_hello:
	.asciz "- - - Bitwise/Arithmetic logic test - - -\n"

	.global wl_int_a
	.p2align 2,0x0
wl_int_a:
	.long 0x1

	.global wl_int_b
	.p2align 2,0x0
wl_int_b:
	.long 0x1
wl_str_ntest:
	.asciz "NOT::  Should be: -2.  Is: %d\n"
wl_str_atest:
	.asciz "AND::  Should be:  1.  Is: %d\n"
wl_str_otest:
	.asciz "OR::   Should be:  1.  Is: %d\n"
wl_str_notest:
	.asciz "NOR::  Should be: -2.  Is: %d\n"
wl_str_natest:
	.asciz "NAND:: Should be: -2.  Is: %d\n"
wl_str_xtest:
	.asciz "XOR::  Should be:  0.  Is: %d\n"
wl_str_adtest:
	.asciz "ADD::  Should be:  2.  Is: %d\n"
wl_str_sutest:
	.asciz "SUB::  Should be:  0.  Is: %d\n"
wl_str_mutest:
	.asciz "MUL::  Should be:  1.  Is: %d\n"
wl_str_ditest:
	.asciz "DIV::  Should be:  1.  Is: %d\n"
wl_str_motest:
	.asciz "MOD::  Should be:  0.  Is: %d\n"
