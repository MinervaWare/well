	.text
	.global switch
switch:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movq $0, -8(%rbp)
	movq %rdi,-8(%rbp)
	movq %rsi,%rdi
	movq -8(%rbp),%rsi
	addq $32, %rsp
	popq %rbp
	ret
	.text
	.global main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movq wl_str_testLocal(%rip), %r10
	movq %r10, -8(%rbp)
	movq $5, -16(%rbp)
	movq $2, -24(%rbp)
	movq wl_str_sw(%rip), %r10
	movq %r10, -32(%rbp)
	movq -8(%rbp),%rdi
	call printf
	movq -16(%rbp),%rdi
	movq -24(%rbp),%rsi
	call switch
	movq %rsi,%rdx
	movq %rdi,%rsi
	movq -32(%rbp),%rdi
	call printf
	movq %rdi, %rax
	addq $32, %rsp
	popq %rbp
	ret
	.text
	.global wl_str_testLocal
.rawwl_strtestLocal:
	.asciz "I am the Walrus\n"
	.data
	.align 8
wl_str_testLocal:
	.quad .rawwl_strtestLocal
	.text
	.global wl_str_sw
.rawwl_strsw:
	.asciz "a: %ld, b: %ld\n"
	.data
	.align 8
wl_str_sw:
	.quad .rawwl_strsw
