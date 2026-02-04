	.section	".opd","aw"
	.global jeT
	.align 3
jeT:
	.quad .wl_jeT,.TOC.@tocbase, 0
	.section	".text"
.wl_jeT:
	mflr 0
	std 0,16(1)
	std 31,-8(1)
	stdu 1,-128(1)
	mr 31,1
	addis 11,2,wl_str_jets@toc@ha
	addi 11,11,wl_str_jets@toc@l
	std 11,56(31)
	li 11,10
	std 11,64(31)
	ld 11,64(31)
	cmpwi 0,10,11
	beq 0,.wl_jeT_is_0
.wl_jeT_is_0_cont:
	addi 1,31,128
	ld 0,16(1)
	mtlr 0
	ld 31,-8(1)
	blr
.wl_jeT_is_0:
	li 3,jets
	ld 4,64(31)
	bl printf
	nop
	mr 9,3
	bl .wl_jeT_is_0_cont
	.section	".opd","aw"
	.global jneT
	.align 3
jneT:
	.quad .wl_jneT,.TOC.@tocbase, 0
	.section	".text"
.wl_jneT:
	mflr 0
	std 0,16(1)
	std 31,-8(1)
	stdu 1,-128(1)
	mr 31,1
	addis 11,2,wl_str_jnets@toc@ha
	addi 11,11,wl_str_jnets@toc@l
	std 11,56(31)
	li 11,5
	std 11,64(31)
	ld 11,64(31)
	cmpwi 0,10,11
	bne 0,.wl_jneT_is_0
.wl_jneT_is_0_cont:
	addi 1,31,128
	ld 0,16(1)
	mtlr 0
	ld 31,-8(1)
	blr
.wl_jneT_is_0:
	ld 3,56(31)
	ld 4,64(31)
	bl printf
	nop
	mr 9,3
	bl .wl_jneT_is_0_cont
	.section	".opd","aw"
	.global jgeT
	.align 3
jgeT:
	.quad .wl_jgeT,.TOC.@tocbase, 0
	.section	".text"
.wl_jgeT:
	mflr 0
	std 0,16(1)
	std 31,-8(1)
	stdu 1,-128(1)
	mr 31,1
	addis 11,2,wl_str_jgets@toc@ha
	addi 11,11,wl_str_jgets@toc@l
	std 11,56(31)
	li 11,11
	std 11,64(31)
	ld 11,64(31)
	cmpwi 0,10,11
	bne 0,.wl_jgeT_is_0
.wl_jgeT_is_0_cont:
	addi 1,31,128
	ld 0,16(1)
	mtlr 0
	ld 31,-8(1)
	blr
.wl_jgeT_is_0:
	ld 3,56(31)
	ld 4,64(31)
	bl printf
	nop
	mr 9,3
	bl .wl_jgeT_is_0_cont
	.section	".opd","aw"
	.global jleT
	.align 3
jleT:
	.quad .wl_jleT,.TOC.@tocbase, 0
	.section	".text"
.wl_jleT:
	mflr 0
	std 0,16(1)
	std 31,-8(1)
	stdu 1,-128(1)
	mr 31,1
	addis 11,2,wl_str_jlets@toc@ha
	addi 11,11,wl_str_jlets@toc@l
	std 11,56(31)
	li 11,9
	std 11,64(31)
	ld 11,64(31)
	cmpwi 0,10,11
	blt 0,.wl_jleT_is_0
.wl_jleT_is_0_cont:
	addi 1,31,128
	ld 0,16(1)
	mtlr 0
	ld 31,-8(1)
	blr
.wl_jleT_is_0:
	ld 3,56(31)
	ld 4,64(31)
	bl printf
	nop
	mr 9,3
	bl .wl_jleT_is_0_cont
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
	addis 3,2,wl_str_iftest@toc@ha
	addi 3,3,wl_str_iftest@toc@l
	bl printf
	nop
	mr 9,3
	bl jeT
	nop
	bl jneT
	nop
	bl jgeT
	nop
	bl jleT
	nop
	li 3,0
	addi 1,31,128
	ld 0,16(1)
	mtlr 0
	ld 31,-8(1)
	blr
wl_str_iftest:
	.asciz "- - - If Statement Operator Tests - - -\n"
wl_str_jets:
	.asciz "(i == 10) pass\n"
wl_str_jnets:
	.asciz "(i != 10) pass\n"
wl_str_jgets:
	.asciz "(i >= 10) pass\n"
wl_str_jlets:
	.asciz "(i <= 10) pass\n"
