	.section __TEXT,__text
	.global _switch
	.p2align 2
_switch:
	sub sp, sp, #32
	stp x29, x30, [sp, #16]
	add x29, sp, #16
	adrp wl_int_tmp,x0@PAGE
	add wl_int_tmp, wl_int_tmp, x0@PAGEOFF
	adrp x0,x1@PAGE
	add x0, x0, x1@PAGEOFF
	adrp x1,wl_int_tmp@PAGE
	ldr w1, [x1, wl_int_tmp@PAGEOFF]
	ldp x29, x30, [sp, #16]
	add sp, sp, #32
	ret
	.section __TEXT,__text
	.global _main
	.p2align 2
_main:
	sub sp, sp, #32
	stp x29, x30, [sp, #16]
	add x29, sp, #16
	adrp x0,wl_str.testLocal@PAGE
	add x0, x0, wl_str.testLocal@PAGEOFF
	bl _printf
	adrp x0,wl_int_a@PAGE
	ldr w0, [x0, wl_int_a@PAGEOFF]
	adrp x1,wl_int_b@PAGE
	ldr w1, [x1, wl_int_b@PAGEOFF]
	bl _switch
	adrp x2,x1@PAGE
	add x2, x2, x1@PAGEOFF
	adrp x1,x0@PAGE
	add x1, x1, x0@PAGEOFF
	adrp x0,wl_str.sw@PAGE
	add x0, x0, wl_str.sw@PAGEOFF
	bl _printf
	mov x0, #0
	ldp x29, x30, [sp, #16]
	add sp, sp, #32
	ret

	.global wl_int_c
	.p2align 2,0x0
wl_int_c:
	.long 0x0

	.global wl_int_tmp
	.p2align 2,0x0
wl_int_tmp:
	.long 0x0
wl_str.testLocal:
	.asciz "I am the Walrus\n"

	.global wl_int_a
	.p2align 2,0x0
wl_int_a:
	.long 0x5

	.global wl_int_b
	.p2align 2,0x0
wl_int_b:
	.long 0x2
wl_str.sw:
	.asciz "a: %d, b: %d\n"
