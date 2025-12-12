	.text
	.global switch
switch:
	lda $30, -16($30)
	stq $26, 0($30)
	stq $15, 8($30)
	bis $31, $30, $15
	lda $22, 0($31)
	stq $22, 8($23)
	mov $22, $16
	stq $22, 8($23)
	mov $16, $17
	ldq $22, 8($23)
	mov $22, $17
	mov $15, $30
	ldq $26, 0($30)
	ldq $15, 8($30)
	lda $30, 16($30)
	ret $31, ($26), 1
	ret
	.text
	.global main
main:
	ldgp $29, 0($27)
	lda $30, -16($30)
	stq $26, 0($30)
	stq $15, 8($30)
	bis $31, $30, $15
	lda $22, wl_str_testLocal
	stq $22, 8($23)
	lda $22, 5($31)
	stq $22, 16($23)
	lda $22, 2($31)
	stq $22, 24($23)
	lda $22, wl_str_sw
	stq $22, 32($23)
	ldq $22, 8($23)
	mov $22, $16
	jsr $26, printf
	ldq $22, 16($23)
	mov $22, $16
	ldq $22, 24($23)
	mov $22, $17
	jsr $26, switch
	mov $18, $17
	mov $17, $16
	ldq $22, 32($23)
	mov $22, $16
	jsr $26, printf
	ldgp $29, 0($26)
	bis $31, 0, $1
	mov $1, $0
	mov $15, $30
	ldq $26, 0($30)
	ldq $15, 8($30)
	lda $30, 16($30)
	ret $31, ($26), 1
wl_str_testLocal:
	.asciz "I am the Walrus\n"
wl_str_sw:
	.asciz "a: %d, b: %d\n"
