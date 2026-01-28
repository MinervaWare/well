	.global switch
	.p2align 2
switch:
	ldp x29, x30, [sp, #32]
	add sp, sp, #32
	ret
	.global main
	.p2align 2
main:
	sub sp, sp, #32
	stp x29, x30, [sp, #32]
	add x29, sp, #32
	adrp x28, wl_str_testLocal
	add x28, x28, :lo12:wl_str_testLocal
	str x28, [sp, 8]
	mov x28, 5
	str x28, [sp, #16]
	mov x28, 2
	str x28, [sp, #24]
	adrp x28, wl_str_sw
	add x28, x28, :lo12:wl_str_sw
	str x28, [sp, 32]
	adrp x28, wl_str_fstr
	add x28, x28, :lo12:wl_str_fstr
	str x28, [sp, 40]
	bl printf
	bl switch
	bl printf
	mov x0, #0
	ldp x29, x30, [sp, #32]
	add sp, sp, #32
	ret
wl_str_testLocal:
	.asciz "I am the Walrus\n"
wl_str_sw:
	.asciz "a: %d, b: %d\n"
wl_str_fstr:
	.asciz "f: %f\n"

	.global wl_fl_fl
	.p2align 2,0x0
wl_fl_fl:
	.long 0x41200
