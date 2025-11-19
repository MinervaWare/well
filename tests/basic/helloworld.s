	.section __TEXT,__text
	.global _main
	.p2align 2
_main:
	sub sp, sp, #32
	stp x29, x30, [sp, #16]
	add x29, sp, #16
	adrp x0,wl_str.text@PAGE
	add x0, x0, wl_str.text@PAGEOFF
	bl _printf
	mov x0, #0
	ldp x29, x30, [sp, #16]
	add sp, sp, #32
	ret
wl_str.text:
	.asciz "Hello World!\n"
