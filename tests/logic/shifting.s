	.text
	.global shiftBasic
shiftBasic:
	lda $30, -16($30)
	stq $26, 0($30)
	stq $15, 8($30)
	bis $31, $30, $15
	lda $16, wl_int_i
	addq $19, $16, $19
	ldgp $29, 0($26)
	bis $31, $19, $1
	mov $1, $0
	mov $15, $30
	ldq $26, 0($30)
	ldq $15, 8($30)
	lda $30, 16($30)
	ret $31, ($26), 1
	.text
	.global shiftWell
shiftWell:
	lda $30, -16($30)
	stq $26, 0($30)
	stq $15, 8($30)
	bis $31, $30, $15
	lda $19, wl_int_i
	ldgp $29, 0($26)
	bis $31, $19, $1
	mov $1, $0
	mov $15, $30
	ldq $26, 0($30)
	ldq $15, 8($30)
	lda $30, 16($30)
	ret $31, ($26), 1
	.text
	.global printIStatus
printIStatus:
	lda $30, -16($30)
	stq $26, 0($30)
	stq $15, 8($30)
	bis $31, $30, $15
	lda $18, $0
	lda $16, wl_str_a0
	lda $17, wl_int_i
	jsr $26, printf
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
	lda $16, wl_str_welcome
	jsr $26, printf
	lda $0, wl_int_i
	jsr $26, printIStatus
	jsr $26, shiftBasic
	jsr $26, printIStatus
	lda $0, wl_int_i
	jsr $26, printIStatus
	jsr $26, shiftWell
	jsr $26, printIStatus
	ldgp $29, 0($26)
	bis $31, 0, $1
	mov $1, $0
	mov $15, $30
	ldq $26, 0($30)
	ldq $15, 8($30)
	lda $30, 16($30)
	ret $31, ($26), 1
wl_str_welcome:
	.asciz "- - - Bit shifting tests - -\n";

	.global wl_int_i
	.p2align 2,0x0
wl_int_i:
	.long 0xa
wl_str_a0:
	.asciz "%d : %b\n"
