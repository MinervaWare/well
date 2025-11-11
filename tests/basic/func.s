	.section __TEXT,__text
	.global _print
	.p2align 2
_print:
	sub sp, sp, #16
	stp x29, x30, [sp, #0]
	add x29, sp, #0
	bl _printf
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
	adrp x0,wl_str.text@PAGE
	add x0, x0, wl_str.text@PAGEOFF
	bl _print
	mov x0, #0
	ldp x29, x30, [sp, #0]
	add sp, sp, #16
	ret
wl_str.text:
	.asciz "Hello World!\n"
