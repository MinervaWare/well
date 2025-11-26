	.text
	.global main
main:
	ldgp $29, 0($27)
	lda $30, -16($30)
	stq $26, 0($30)
	stq $15, 8($30)
	bis $31, $30, $15
	ldgp $29, 0($26)
	bis $31, 0, $1
	mov $1, $0
	mov $15, $30
	ldq $26, 0($30)
	ldq $15, 8($30)
	lda $30, 16($30)
	ret $31, ($26), 1
wl_str_str:
	.asciz "STRING"

	.global wl_ch_ch
wl_ch_ch:
	.byte 67

	.global wl_int_i
	.p2align 2,0x0
wl_int_i:
	.long 0x5c6

	.global wl_fl_f
	.p2align 2,0x0
wl_fl_f:
	.long 0x4048f
