	.global allocatorProc
	.p2align 2
allocatorProc:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	mov x15, 0
	str x15, [sp, #39]
	mov x15, 0
	str x15, [sp, #47]
	adrp x15, wl_z_oldMem@PAGE
	add x15, x15, wl_z_oldMem@PAGEOFF
	str x15, [sp, 55]
	adrp x15, wl_z_res@PAGE
	add x15, x15, wl_z_res@PAGEOFF
	str x15, [sp, 63]
	str x0, [sp, #39]
	str x1, [sp, #47]
	str x2, [sp, #55]
	adrp x3,wl_int_ALLOC@PAGE
	ldr w3, [x3, wl_int_ALLOC@PAGEOFF]
	ldr x15, [sp, #47]
	cmp x15, x3
	beq .wl_allocatorProc_is_022
.wl_allocatorProc_is_022_cont:
	adrp x3,wl_int_RESIZE@PAGE
	ldr w3, [x3, wl_int_RESIZE@PAGEOFF]
	ldr x15, [sp, #47]
	cmp x15, x3
	beq .wl_allocatorProc_is_022
.wl_allocatorProc_is_022_cont:
	adrp x3,wl_int_FREE@PAGE
	ldr w3, [x3, wl_int_FREE@PAGEOFF]
	ldr x15, [sp, #47]
	cmp x15, x3
	beq .wl_allocatorProc_is_022
.wl_allocatorProc_is_022_cont:
	mov x0, x0
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
.wl_allocatorProc_is_022:
	ldr x0, [sp, #39]
	bl _malloc
	mov x0, x6
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
	b .wl_allocatorProc_is_022_cont
.wl_allocatorProc_is_128:
	ldr x0, [sp, #55]
	ldr x1, [sp, #39]
	bl _realloc
	mov x0, x6
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
	b .wl_allocatorProc_is_128_cont
.wl_allocatorProc_is_235:
	ldr x0, [sp, #55]
	bl free
	mov x0, x0
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
	b .wl_allocatorProc_is_235_cont
	.global alloc
	.p2align 2
alloc:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x1,wl_int_ALLOC@PAGE
	ldr w1, [x1, wl_int_ALLOC@PAGEOFF]
	bl allocatorProc
	mov x0, x6
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
	.global resize
	.p2align 2
resize:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x1,wl_int_RESIZE@PAGE
	ldr w1, [x1, wl_int_RESIZE@PAGEOFF]
	bl allocatorProc
	mov x0, x6
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
	.global free
	.p2align 2
free:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x1,wl_int_FREE@PAGE
	ldr w1, [x1, wl_int_FREE@PAGEOFF]
	bl free
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret

	.global wl_int_ALLOC
	.p2align 2,0x0
wl_int_ALLOC:
	.long 0x0

	.global wl_int_RESIZE
	.p2align 2,0x0
wl_int_RESIZE:
	.long 0x1

	.global wl_int_FREE
	.p2align 2,0x0
wl_int_FREE:
	.long 0x2
