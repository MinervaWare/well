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
	li 3,0
	addi 1,31,128
	ld 0,16(1)
	mtlr 0
	ld 31,-8(1)
	blr
wl_str_str:
	.asciz "STRING"

	.global wl_ch_ch
wl_ch_ch:
	.byte 67

	.global wl_int_i
	.p2align 2,0x0
wl_int_i:
	.long 0x5c6

	.global wl_fl_f
	.p2align 2,0x0
wl_fl_f:
	.long 0x4048f
