	.global switch
	.p2align 2
switch:
	sub sp, sp, #32
	stp x29, x30, [sp, #32]
	add x29, sp, #32
	mov x28, 0
	str x28, [sp, #8]
	str x0, [sp, #8]
	mov x0, x1
	ldr x1, [sp, #8]
	ldp x29, x30, [sp, #32]
	add sp, sp, #32
	ret
	.global _main
	.p2align 2
_main:
	sub sp, sp, #32
	stp x29, x30, [sp, #32]
	add x29, sp, #32
	adrp x28, wl_str_testLocal@PAGE
	add x28, x28, wl_str_testLocal@PAGEOFF
	str x28, [sp, 8]
	mov x28, 5
	str x28, [sp, #16]
	mov x28, 2
	str x28, [sp, #24]
	adrp x28, wl_str_sw@PAGE
	add x28, x28, wl_str_sw@PAGEOFF
	str x28, [sp, 32]
	ldr x0, [sp, #8]
	bl _printf
	ldr x0, [sp, #16]
	ldr x1, [sp, #24]
	bl switch
	mov x2, x1
	mov x1, x0
	ldr x0, [sp, #32]
	bl _printf
	mov x0, #0
	ldp x29, x30, [sp, #32]
	add sp, sp, #32
	ret
wl_str_testLocal:
	.asciz "I am the Walrus\n"
wl_str_sw:
	.asciz "a: %d, b: %d\n"
