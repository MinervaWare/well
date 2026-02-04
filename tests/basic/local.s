	.section	".opd","aw"
	.global switch
	.align 3
switch:
	.quad .wl_switch,.TOC.@tocbase, 0
	.section	".text"
.wl_switch:
	mflr 0
	std 0,16(1)
	std 31,-8(1)
	stdu 1,-128(1)
	mr 31,1
	li 11,0
	std 11,56(31)
	ld 3,56(31)
	mr 3,4
	ld 4,56(31)
	addi 1,31,128
	ld 0,16(1)
	mtlr 0
	ld 31,-8(1)
	blr
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
	addis 11,2,wl_str_testLocal@toc@ha
	addi 11,11,wl_str_testLocal@toc@l
	std 11,56(31)
	li 11,5
	std 11,64(31)
	li 11,2
	std 11,72(31)
	addis 11,2,wl_str_sw@toc@ha
	addi 11,11,wl_str_sw@toc@l
	std 11,80(31)
	ld 3,56(31)
	bl printf
	nop
	mr 9,3
	ld 3,64(31)
	ld 4,72(31)
	bl switch
	nop
	mr 5,4
	mr 4,3
	ld 3,80(31)
	bl printf
	nop
	mr 9,3
	li 3,0
	addi 1,31,128
	ld 0,16(1)
	mtlr 0
	ld 31,-8(1)
	blr
wl_str_testLocal:
	.asciz "I am the Walrus\n"
wl_str_sw:
	.asciz "a: %d, b: %d\n"
