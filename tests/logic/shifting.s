	.extern printf
	.global shiftBasic
	.p2align 2
shiftBasic:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x0,wl_int_i@PAGE
	ldr w0, [x0, wl_int_i@PAGEOFF]
	add x3, x0, x0
	mov x0, x3
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
	.global shiftWell
	.p2align 2
shiftWell:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x3,wl_int_i@PAGE
	ldr w3, [x3, wl_int_i@PAGEOFF]
	mov x0, x3
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
	.global printIStatus
	.p2align 2
printIStatus:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	mov x2, x6
	adrp x0,wl_str_a0@PAGE
	add x0, x0, wl_str_a0@PAGEOFF
	adrp x1,wl_int_i@PAGE
	ldr w1, [x1, wl_int_i@PAGEOFF]
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
	adrp x0,wl_str_welcome@PAGE
	add x0, x0, wl_str_welcome@PAGEOFF
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	adrp x6,wl_int_i@PAGE
	ldr w6, [x6, wl_int_i@PAGEOFF]
	bl printIStatus
	bl shiftBasic
	bl printIStatus
	adrp x6,wl_int_i@PAGE
	ldr w6, [x6, wl_int_i@PAGEOFF]
	bl printIStatus
	bl shiftWell
	bl printIStatus
	mov x0, x0
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
wl_str_welcome:
	.asciz "- - - Bit shifting tests - -\n";

	.global wl_int_i
	.p2align 2,0x0
wl_int_i:
	.long 0xa
wl_str_a0:
	.asciz "%d : %b\n"
