	.text
	.global logic
logic:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	movq wl_int_a(%rip),%rdi
	movq %rdi, %rsi
	notq %rsi
	movq wl_str_ntest(%rip),%rdi
	call printf
	movq wl_int_a(%rip),%rdi
	movq wl_int_b(%rip),%rsi
	movq %rdi, %rdx
	andq %rsi, %rdx
	movq %rdx,%rsi
	movq wl_str_atest(%rip),%rdi
	call printf
	movq wl_int_a(%rip),%rdi
	movq wl_int_b(%rip),%rsi
	movq %rdi, %rdx
	orq %rsi, %rdx
	movq %rdx,%rsi
	movq wl_str_otest(%rip),%rdi
	call printf
	movq wl_int_a(%rip),%rdi
	movq wl_int_b(%rip),%rsi
	movq %rdi, %rdx
	orq %rsi, %rdx
	notq %rdx
	movq %rdx,%rsi
	movq wl_str_notest(%rip),%rdi
	call printf
	movq wl_int_a(%rip),%rdi
	movq wl_int_b(%rip),%rsi
	movq %rdi, %rdx
	andq %rsi, %rdx
	notq %rdx
	movq %rdx,%rsi
	movq wl_str_natest(%rip),%rdi
	call printf
	movq wl_int_a(%rip),%rdi
	movq wl_int_b(%rip),%rsi
	movq %rdi, %rdx
	xorq %rdx, %rdx
	movq %rdx,%rsi
	movq wl_str_xtest(%rip),%rdi
	call printf
	addq $16, %rsp
	popq %rbp
	ret
	.text
	.global math
math:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	movq wl_int_a(%rip),%rdi
	movq wl_int_b(%rip),%rsi
	movq %rdi, %rdx
	addq %rdx, %rdx
	movq %rdx,%rsi
	movq wl_str_adtest(%rip),%rdi
	call printf
	movq wl_int_a(%rip),%rdi
	movq wl_int_b(%rip),%rsi
	movq %rdi, %rdx
	subq %rdx, %rdx
	movq %rdx,%rsi
	movq wl_str_sutest(%rip),%rdi
	call printf
	movq wl_int_a(%rip),%rdi
	movq wl_int_b(%rip),%rsi
	movq %rdi, %rdx
	imulq %rdx, %rdx
	movq %rdx,%rsi
	movq wl_str_mutest(%rip),%rdi
	call printf
	movq wl_int_a(%rip),%rdi
	movq wl_int_b(%rip),%rsi
	movq %rdi, %rax
	cqto
	idivq %rsi
	movq %rax, %rdx
	movq %rdx,%rsi
	movq wl_str_ditest(%rip),%rdi
	call printf
	movq wl_int_a(%rip),%rdi
	movq wl_int_b(%rip),%rsi
	movq %rdi, %rax
	cqto
	idivq %rsi
	movq %rdx, %rdx
	movq %rdx,%rsi
	movq wl_str_motest(%rip),%rdi
	call printf
	addq $16, %rsp
	popq %rbp
	ret
	.text
	.global main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	movq wl_str_hello(%rip),%rdi
	call printf
	call logic
	call math
	movq %rdi, %rax
	addq $16, %rsp
	popq %rbp
	ret
	.text
	.global wl_str_hello
.rawwl_strhello:
	.asciz "- - - Bitwise/Arithmetic logic test - - -\n"
	.data
	.align 8
wl_str_hello:
	.quad .rawwl_strhello

	.global wl_int_a
	.p2align 2,0x0
wl_int_a:
	.long 0x1

	.global wl_int_b
	.p2align 2,0x0
wl_int_b:
	.long 0x1
	.text
	.global wl_str_ntest
.rawwl_strntest:
	.asciz "NOT::  Should be: -2.  Is: %d\n"
	.data
	.align 8
wl_str_ntest:
	.quad .rawwl_strntest
	.text
	.global wl_str_atest
.rawwl_stratest:
	.asciz "AND::  Should be:  1.  Is: %d\n"
	.data
	.align 8
wl_str_atest:
	.quad .rawwl_stratest
	.text
	.global wl_str_otest
.rawwl_strotest:
	.asciz "OR::   Should be:  1.  Is: %d\n"
	.data
	.align 8
wl_str_otest:
	.quad .rawwl_strotest
	.text
	.global wl_str_notest
.rawwl_strnotest:
	.asciz "NOR::  Should be: -2.  Is: %d\n"
	.data
	.align 8
wl_str_notest:
	.quad .rawwl_strnotest
	.text
	.global wl_str_natest
.rawwl_strnatest:
	.asciz "NAND:: Should be: -2.  Is: %d\n"
	.data
	.align 8
wl_str_natest:
	.quad .rawwl_strnatest
	.text
	.global wl_str_xtest
.rawwl_strxtest:
	.asciz "XOR::  Should be:  0.  Is: %d\n"
	.data
	.align 8
wl_str_xtest:
	.quad .rawwl_strxtest
	.text
	.global wl_str_adtest
.rawwl_stradtest:
	.asciz "ADD::  Should be:  2.  Is: %d\n"
	.data
	.align 8
wl_str_adtest:
	.quad .rawwl_stradtest
	.text
	.global wl_str_sutest
.rawwl_strsutest:
	.asciz "SUB::  Should be:  0.  Is: %d\n"
	.data
	.align 8
wl_str_sutest:
	.quad .rawwl_strsutest
	.text
	.global wl_str_mutest
.rawwl_strmutest:
	.asciz "MUL::  Should be:  1.  Is: %d\n"
	.data
	.align 8
wl_str_mutest:
	.quad .rawwl_strmutest
	.text
	.global wl_str_ditest
.rawwl_strditest:
	.asciz "DIV::  Should be:  1.  Is: %d\n"
	.data
	.align 8
wl_str_ditest:
	.quad .rawwl_strditest
	.text
	.global wl_str_motest
.rawwl_strmotest:
	.asciz "MOD::  Should be:  0.  Is: %d\n"
	.data
	.align 8
wl_str_motest:
	.quad .rawwl_strmotest
