	.global _main
	.p2align 2
_main:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	mov x15, 128
	str x15, [sp, #39]
	adrp x15, wl_z_randomData@PAGE
	add x15, x15, wl_z_randomData@PAGEOFF
	str x15, [sp, 47]
	ldr x0, [sp, #39]
	bl _alloc
	add x0, x0, x0
	adrp x0,_randomData@PAGE
	add x0, x0, _randomData@PAGEOFF
	ldr x1, [sp, #39]
	bl _resize
	adrp x0,_randomData@PAGE
	add x0, x0, _randomData@PAGEOFF
	bl _free
	mov x0, x0
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
