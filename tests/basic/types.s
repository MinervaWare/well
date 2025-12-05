	.text
	.global main
main:
	pushl %ebp
	movl %esp, %ebp
	subl $32, %esp
	movl %edi, %eax
	addl $32, %esp
	popl %ebp
	ret
	.text
	.global wl_str_str
.rawwl_strstr:
	.asciz "STRING"
	.data
	.align 8
wl_str_str:
	.long .rawwl_strstr

	.global wl_ch_ch
wl_ch_ch:
	.byte 67

	.global wl_int_i
	.p2align 2,0x0
wl_int_i:
	.long 0x5

	.global wl_fl_f
	.p2align 2,0x0
wl_fl_f:
	.long 0x4
