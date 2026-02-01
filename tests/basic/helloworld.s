	.section	".opd","aw"
	.global main
	.align 3
main:
	.quad .wl_main,.TOC.@tocbase, 0
	.section	".text"
.wl_main:
	mflr 0
	std 0,16(1)
	std 31,-8(1)
	stdu 1,-128(1)
	mr 31,1
	addis 3,2,wl_str_text@toc@ha
	addi 3,3,wl_str_text@toc@l
	bl printf
	nop
	mr 9,3
	li 3,0
	addi 1,31,128
	ld 0,16(1)
	mtlr 0
	ld 31,-8(1)
	blr
wl_str_text:
	.asciz "Hello World!\n"
