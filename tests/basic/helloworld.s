	.global main
	.p2align 2
main:
	sub sp, sp, #32
	stp x29, x30, [sp, #32]
	add x29, sp, #32
	adrp x0,wl_str_text
	add x0, x0, :lo12:wl_str_text
	bl printf
	mov x0, #0
	ldp x29, x30, [sp, #32]
	add sp, sp, #32
	ret
wl_str_text:
	.asciz "Hello World!\n"
