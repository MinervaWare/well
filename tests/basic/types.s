	.section __TEXT,__text
	.global _main
	.p2align 2
_main:
	sub sp, sp, #16
	stp x29, x30, [sp, #0]
	add x29, sp, #0
	mov x0, #0
	ldp x29, x30, [sp, #0]
	add sp, sp, #16
	ret
wl_str.str:
	.asciz "STRING"

	.global wl_ch_ch
wl_ch_ch:
	.byte 67

	.global wl_int_i
	.p2align 2,0x0
wl_int_i:
	.long 0x5c6

	.global wl_int_f
	.p2align 2,0x0
wl_int_f:
	.long 0x4048f
