	.text
	.global jeT
jeT:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movq wl_str_jets(%rip), %r10
	movq %r10, -8(%rbp)
	movq $10, -16(%rbp)
	cmpq $10, -16(%rbp)
	je .wl_jeT_is_0
.wl_jeT_is_0_cont:
	addq $32, %rsp
	popq %rbp
	ret
.wl_jeT_is_0:
	movq -8(%rbp),%rdi
	movq -16(%rbp),%rsi
	call printf
	jmp .wl_jeT_is_0_cont
	.text
	.global jneT
jneT:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movq wl_str_jnets(%rip), %r10
	movq %r10, -8(%rbp)
	movq $5, -16(%rbp)
	cmpq $10, -16(%rbp)
	jne .wl_jneT_is_0
.wl_jneT_is_0_cont:
	addq $32, %rsp
	popq %rbp
	ret
.wl_jneT_is_0:
	movq -8(%rbp),%rdi
	movq -16(%rbp),%rsi
	call printf
	jmp .wl_jneT_is_0_cont
	.text
	.global jgeT
jgeT:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movq wl_str_jgets(%rip), %r10
	movq %r10, -8(%rbp)
	movq $11, -16(%rbp)
	cmpq $10, -16(%rbp)
	jne .wl_jgeT_is_0
.wl_jgeT_is_0_cont:
	addq $32, %rsp
	popq %rbp
	ret
.wl_jgeT_is_0:
	movq -8(%rbp),%rdi
	movq -16(%rbp),%rsi
	call printf
	jmp .wl_jgeT_is_0_cont
	.text
	.global jleT
jleT:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movq wl_str_jlets(%rip), %r10
	movq %r10, -8(%rbp)
	movq $9, -16(%rbp)
	cmpq $10, -16(%rbp)
	jle .wl_jleT_is_0
.wl_jleT_is_0_cont:
	addq $32, %rsp
	popq %rbp
	ret
.wl_jleT_is_0:
	movq -8(%rbp),%rdi
	movq -16(%rbp),%rsi
	call printf
	jmp .wl_jleT_is_0_cont
	.text
	.global main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movq wl_str_iftest(%rip),%rdi
	call printf
	call jeT
	call jneT
	call jgeT
	call jleT
	movq %rdi, %rax
	addq $32, %rsp
	popq %rbp
	ret
	.text
	.global wl_str_iftest
.rawwl_striftest:
	.asciz "- - - If Statement Operator Tests - - -\n"
	.data
	.align 8
wl_str_iftest:
	.quad .rawwl_striftest
	.text
	.global wl_str_jets
.rawwl_strjets:
	.asciz "(i == 10) pass\n"
	.data
	.align 8
wl_str_jets:
	.quad .rawwl_strjets
	.text
	.global wl_str_jnets
.rawwl_strjnets:
	.asciz "(i != 10) pass\n"
	.data
	.align 8
wl_str_jnets:
	.quad .rawwl_strjnets
	.text
	.global wl_str_jgets
.rawwl_strjgets:
	.asciz "(i >= 10) pass\n"
	.data
	.align 8
wl_str_jgets:
	.quad .rawwl_strjgets
	.text
	.global wl_str_jlets
.rawwl_strjlets:
	.asciz "(i <= 10) pass\n"
	.data
	.align 8
wl_str_jlets:
	.quad .rawwl_strjlets
