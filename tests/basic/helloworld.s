	.text
	.global main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movq wl_str_text(%rip),%rdi
	call printf
	movq %rdi, %rax
	addq $32, %rsp
	popq %rbp
	ret
	.text
	.global wl_str_text
.rawwl_strtext:
	.asciz "Hello World!\n"
	.data
	.align 8
wl_str_text:
	.quad .rawwl_strtext
