	.text
	.global foo
foo:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	call printf
	addq $32, %rsp
	popq %rbp
	ret
	.text
	.global main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movq wl_str_test(%rip), %r10
	movq %r10, -8(%rbp)
	movq $10, -16(%rbp)
	movq -16(%rbp),%rsi
	movq %rdi, %rax
	addq $32, %rsp
	popq %rbp
	ret
	.text
	.global wl_str_test
.rawwl_strtest:
	.asciz "I am the Walrus: %d\n"
	.data
	.align 8
wl_str_test:
	.quad .rawwl_strtest
