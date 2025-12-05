	.text
	.global logic
logic:
	pushl %ebp
	movl %esp, %ebp
	subl $32, %esp
	movl wl_int_a,%edi
	movl %edi, %esi
	notl %esi
	movl wl_str_ntest,%edi
	call printf
	movl wl_int_a,%edi
	movl wl_int_b,%esi
	movl %edi, %edx
	andl %esi, %edx
	movl %edx,%esi
	movl wl_str_atest,%edi
	call printf
	movl wl_int_a,%edi
	movl wl_int_b,%esi
	movl %edi, %edx
	orl %esi, %edx
	movl %edx,%esi
	movl wl_str_otest,%edi
	call printf
	movl wl_int_a,%edi
	movl wl_int_b,%esi
	movl %edi, %edx
	orl %esi, %edx
	notl %edx
	movl %edx,%esi
	movl wl_str_notest,%edi
	call printf
	movl wl_int_a,%edi
	movl wl_int_b,%esi
	movl %edi, %edx
	andl %esi, %edx
	notl %edx
	movl %edx,%esi
	movl wl_str_natest,%edi
	call printf
	movl wl_int_a,%edi
	movl wl_int_b,%esi
	movl %edi, %edx
	xorl %edx, %edx
	movl %edx,%esi
	movl wl_str_xtest,%edi
	call printf
	addl $32, %esp
	popl %ebp
	ret
	.text
	.global math
math:
	pushl %ebp
	movl %esp, %ebp
	subl $32, %esp
	movl wl_int_a,%edi
	movl wl_int_b,%esi
	movl %edi, %edx
	addl %edx, %edx
	movl %edx,%esi
	movl wl_str_adtest,%edi
	call printf
	movl wl_int_a,%edi
	movl wl_int_b,%esi
	movl %edi, %edx
	subl %edx, %edx
	movl %edx,%esi
	movl wl_str_sutest,%edi
	call printf
	movl wl_int_a,%edi
	movl wl_int_b,%esi
	movl %edi, %edx
	imull %edx, %edx
	movl %edx,%esi
	movl wl_str_mutest,%edi
	call printf
	movl wl_int_a,%edi
	movl wl_int_b,%esi
	movl %edi, %eax
	cltd
	idivl %esi
	movl %eax, %edx
	movl %edx,%esi
	movl wl_str_ditest,%edi
	call printf
	movl wl_int_a,%edi
	movl wl_int_b,%esi
	movl %edi, %eax
	cltd
	idivl %esi
	movl %edx, %edx
	movl %edx,%esi
	movl wl_str_motest,%edi
	call printf
	addl $32, %esp
	popl %ebp
	ret
	.text
	.global main
main:
	pushl %ebp
	movl %esp, %ebp
	subl $32, %esp
	movl wl_str_hello,%edi
	call printf
	call logic
	call math
	movl %edi, %eax
	addl $32, %esp
	popl %ebp
	ret
	.text
	.global wl_str_hello
.rawwl_strhello:
	.asciz "- - - Bitwise/Arithmetic logic test - - -\n"
	.data
	.align 8
wl_str_hello:
	.long .rawwl_strhello

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
	.long .rawwl_strntest
	.text
	.global wl_str_atest
.rawwl_stratest:
	.asciz "AND::  Should be:  1.  Is: %d\n"
	.data
	.align 8
wl_str_atest:
	.long .rawwl_stratest
	.text
	.global wl_str_otest
.rawwl_strotest:
	.asciz "OR::   Should be:  1.  Is: %d\n"
	.data
	.align 8
wl_str_otest:
	.long .rawwl_strotest
	.text
	.global wl_str_notest
.rawwl_strnotest:
	.asciz "NOR::  Should be: -2.  Is: %d\n"
	.data
	.align 8
wl_str_notest:
	.long .rawwl_strnotest
	.text
	.global wl_str_natest
.rawwl_strnatest:
	.asciz "NAND:: Should be: -2.  Is: %d\n"
	.data
	.align 8
wl_str_natest:
	.long .rawwl_strnatest
	.text
	.global wl_str_xtest
.rawwl_strxtest:
	.asciz "XOR::  Should be:  0.  Is: %d\n"
	.data
	.align 8
wl_str_xtest:
	.long .rawwl_strxtest
	.text
	.global wl_str_adtest
.rawwl_stradtest:
	.asciz "ADD::  Should be:  2.  Is: %d\n"
	.data
	.align 8
wl_str_adtest:
	.long .rawwl_stradtest
	.text
	.global wl_str_sutest
.rawwl_strsutest:
	.asciz "SUB::  Should be:  0.  Is: %d\n"
	.data
	.align 8
wl_str_sutest:
	.long .rawwl_strsutest
	.text
	.global wl_str_mutest
.rawwl_strmutest:
	.asciz "MUL::  Should be:  1.  Is: %d\n"
	.data
	.align 8
wl_str_mutest:
	.long .rawwl_strmutest
	.text
	.global wl_str_ditest
.rawwl_strditest:
	.asciz "DIV::  Should be:  1.  Is: %d\n"
	.data
	.align 8
wl_str_ditest:
	.long .rawwl_strditest
	.text
	.global wl_str_motest
.rawwl_strmotest:
	.asciz "MOD::  Should be:  0.  Is: %d\n"
	.data
	.align 8
wl_str_motest:
	.long .rawwl_strmotest
