	.text
	.global main
main:
	pushl %ebp
	movl %esp, %ebp
	subl $32, %esp
	movl wl_str_text,%edi
	call printf
	movl %edi, %eax
	addl $32, %esp
	popl %ebp
	ret
	.text
	.global wl_str_text
.rawwl_strtext:
	.asciz "Hello World!\n"
	.data
	.align 8
wl_str_text:
	.long .rawwl_strtext
