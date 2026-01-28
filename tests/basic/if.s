	.global jeT
	.p2align 2
jeT:
	sub sp, sp, #32
	stp x29, x30, [sp, #32]
	add x29, sp, #32
	adrp x28, wl_str_jets
	add x28, x28, :lo12:wl_str_jets
	str x28, [sp, 8]
	mov x28, 10
	str x28, [sp, #16]
	ldr x28, [sp, 16]
	cmp x28, #10
	beq .wl_jeT_is_0
.wl_jeT_is_0_cont:
	ldp x29, x30, [sp, #32]
	add sp, sp, #32
	ret
.wl_jeT_is_0:
	bl printf
	.global jneT
	.p2align 2
jneT:
	sub sp, sp, #32
	stp x29, x30, [sp, #32]
	add x29, sp, #32
	adrp x28, wl_str_jnets
	add x28, x28, :lo12:wl_str_jnets
	str x28, [sp, 8]
	mov x28, 5
	str x28, [sp, #16]
	ldr x28, [sp, 16]
	cmp x28, #10
	bne .wl_jneT_is_0
.wl_jneT_is_0_cont:
	ldp x29, x30, [sp, #32]
	add sp, sp, #32
	ret
.wl_jneT_is_0:
	bl printf
	.global jgeT
	.p2align 2
jgeT:
	sub sp, sp, #32
	stp x29, x30, [sp, #32]
	add x29, sp, #32
	adrp x28, wl_str_jgets
	add x28, x28, :lo12:wl_str_jgets
	str x28, [sp, 8]
	mov x28, 11
	str x28, [sp, #16]
	ldr x28, [sp, 16]
	cmp x28, #10
	bne .wl_jgeT_is_0
.wl_jgeT_is_0_cont:
	ldp x29, x30, [sp, #32]
	add sp, sp, #32
	ret
.wl_jgeT_is_0:
	bl printf
	.global jleT
	.p2align 2
jleT:
	sub sp, sp, #32
	stp x29, x30, [sp, #32]
	add x29, sp, #32
	adrp x28, wl_str_jlets
	add x28, x28, :lo12:wl_str_jlets
	str x28, [sp, 8]
	mov x28, 9
	str x28, [sp, #16]
	ldr x28, [sp, 16]
	cmp x28, #10
	ble .wl_jleT_is_0
.wl_jleT_is_0_cont:
	ldp x29, x30, [sp, #32]
	add sp, sp, #32
	ret
.wl_jleT_is_0:
	bl printf
	.global main
	.p2align 2
main:
	sub sp, sp, #32
	stp x29, x30, [sp, #32]
	add x29, sp, #32
	adrp x0,wl_str_iftest
	add x0, x0, :lo12:wl_str_iftest
	bl printf
	bl jeT
	bl jneT
	bl jgeT
	bl jleT
	mov x0, #0
	ldp x29, x30, [sp, #32]
	add sp, sp, #32
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
