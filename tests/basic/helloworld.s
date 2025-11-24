	.text
	.global main
main:
	ldgp $29, 0($27)
	lda $30, -16($30)
	stq $26, 0($30)
	stq $15, 8($30)
	bis $31, $30, $15
	lda $16, wl_str_text
	jsr $26, printf
	ldgp $29, 0($26)
	bis $31, 0, $1
	mov $1, $0
	mov $15, $30
	ldq $26, 0($30)
	ldq $15, 8($30)
	lda $30, 16($30)
	ret $31, ($26), 1
wl_str_text:
	.asciz "Hello World!\n"
