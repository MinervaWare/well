	.extern printf
	.global jeT
	.p2align 2
jeT:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x15, wl_str_jets@PAGE
	add x15, x15, wl_str_jets@PAGEOFF
	str x15, [sp, 39]
	mov x15, 10
	str x15, [sp, #47]
	ldr x15, [sp, #47]
	cmp x15, #10
	beq .wl_jeT_is_0
.wl_jeT_is_0_cont:
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
.wl_jeT_is_0:
	ldr x0, [sp, #39]
	ldr x1, [sp, #47]
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	b .wl_jeT_is_0_cont
	.global jneT
	.p2align 2
jneT:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x15, wl_str_jnets@PAGE
	add x15, x15, wl_str_jnets@PAGEOFF
	str x15, [sp, 39]
	mov x15, 5
	str x15, [sp, #47]
	ldr x15, [sp, #47]
	cmp x15, #10
	bne .wl_jneT_is_0
.wl_jneT_is_0_cont:
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
.wl_jneT_is_0:
	ldr x0, [sp, #39]
	ldr x1, [sp, #47]
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	b .wl_jneT_is_0_cont
	.global jgeT
	.p2align 2
jgeT:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x15, wl_str_jgets@PAGE
	add x15, x15, wl_str_jgets@PAGEOFF
	str x15, [sp, 39]
	mov x15, 11
	str x15, [sp, #47]
	ldr x15, [sp, #47]
	cmp x15, #10
	bne .wl_jgeT_is_0
.wl_jgeT_is_0_cont:
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
.wl_jgeT_is_0:
	ldr x0, [sp, #39]
	ldr x1, [sp, #47]
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	b .wl_jgeT_is_0_cont
	.global jleT
	.p2align 2
jleT:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x15, wl_str_jlets@PAGE
	add x15, x15, wl_str_jlets@PAGEOFF
	str x15, [sp, 39]
	mov x15, 9
	str x15, [sp, #47]
	ldr x15, [sp, #47]
	cmp x15, #10
	ble .wl_jleT_is_0
.wl_jleT_is_0_cont:
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
.wl_jleT_is_0:
	ldr x0, [sp, #39]
	ldr x1, [sp, #47]
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	b .wl_jleT_is_0_cont
	.global _main
	.p2align 2
_main:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x0,wl_str_iftest@PAGE
	add x0, x0, wl_str_iftest@PAGEOFF
	str x1, [sp, #0]
	str x2, [sp, #8]
	str x3, [sp, #16]
	str x4, [sp, #24]
	str x5, [sp, #32]
	str x6, [sp, #40]
	bl _printf
	bl jeT
	bl jneT
	bl jgeT
	bl jleT
	mov x0, #0
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
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
