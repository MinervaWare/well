	.global _main
	.p2align 2
_main:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x0,wl_str_text@PAGE
	add x0, x0, wl_str_text@PAGEOFF
	bl _printf
	mov x0, x0
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
wl_str_text:
	.asciz "Hello World!\n"
