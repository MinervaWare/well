	.extern printf
	.global switch
	.p2align 2
switch:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	mov x15, 0
	str x15, [sp, #39]
	str x0, [sp, #39]
	mov x0, x1
	ldr x1, [sp, #39]
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
	.global _main
	.p2align 2
_main:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x15, wl_str_testLocal@PAGE
	add x15, x15, wl_str_testLocal@PAGEOFF
	str x15, [sp, 39]
	mov x15, 5
	str x15, [sp, #47]
	mov x15, 2
	str x15, [sp, #55]
	adrp x15, wl_str_sw@PAGE
	add x15, x15, wl_str_sw@PAGEOFF
	str x15, [sp, 63]
	ldr x0, [sp, #39]
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	ldr x0, [sp, #47]
	ldr x1, [sp, #55]
	bl switch
	mov x2, x1
	mov x1, x0
	ldr x0, [sp, #63]
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	mov x0, #0
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
wl_str_testLocal:
	.asciz "I am the Walrus\n"
wl_str_sw:
	.asciz "a: %d, b: %d\n"
