	.text
	.global shiftBasic
shiftBasic:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	movq wl_int_i(%rip),%rdi
	movq %rdi, %rcx
	addq %rcx, %rcx
	movq %rcx, %rax
	addq $16, %rsp
	popq %rbp
	ret
	.text
	.global shiftWell
shiftWell:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	movq wl_int_i(%rip),%rcx
	movq %rcx, %rax
	addq $16, %rsp
	popq %rbp
	ret
	.text
	.global printIStatus
printIStatus:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	movq %rax,%rdx
	movq wl_str_a0(%rip),%rdi
	movq wl_int_i(%rip),%rsi
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
	movq wl_str_welcome(%rip),%rdi
	call printf
	movq wl_int_i(%rip),%rax
	call printIStatus
	call shiftBasic
	call printIStatus
	movq wl_int_i(%rip),%rax
	call printIStatus
	call shiftWell
	call printIStatus
	movq %rdi, %rax
	addq $16, %rsp
	popq %rbp
	ret
	.text
	.global wl_str_welcome
.rawwl_strwelcome:
	.asciz "- - - Bit shifting tests - -\n";
	.data
	.align 8
wl_str_welcome:
	.quad .rawwl_strwelcome

	.global wl_int_i
	.p2align 2,0x0
wl_int_i:
	.long 0xA
	.text
	.global wl_str_a0
.rawwl_stra0:
	.asciz "%d : %b\n"
	.data
	.align 8
wl_str_a0:
	.quad .rawwl_stra0
