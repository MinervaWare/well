	.align 1
	.global main
main:
	addi sp,sp,-32
	sd ra,24(sp)
	sd s0,16(sp)
	addi s0,sp,32
	lla a0,wl_str_text
	call printf@plt
	mv a6,a0
	mv a0,a0
	ld ra,24(sp)
	ld s0,16(sp)
	addi sp,sp,32
	jr ra

	.align 3
wl_str_text:
	.string "Hello World!\n"
	.ident "Well: (GNU/Linux) 0.0.0"
