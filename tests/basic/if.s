	.section __TEXT,__text
	.global _jeT
	.p2align 2
_jeT:
	sub sp, sp, #32
	stp x29, x30, [sp, #32]
	add x29, sp, #32
	adrp x28, wl_str_jets@PAGE
	add x28, x28, wl_str_jets@PAGEOFF
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
	ldr x0, [sp, #8]
	ldr x1, [sp, #16]
	bl _printf
	b .wl_jeT_is_0_cont
	.section __TEXT,__text
	.global _jneT
	.p2align 2
_jneT:
	sub sp, sp, #32
	stp x29, x30, [sp, #32]
	add x29, sp, #32
	adrp x28, wl_str_jnets@PAGE
	add x28, x28, wl_str_jnets@PAGEOFF
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
	ldr x0, [sp, #8]
	ldr x1, [sp, #16]
	bl _printf
	b .wl_jneT_is_0_cont
	.section __TEXT,__text
	.global _jgeT
	.p2align 2
_jgeT:
	sub sp, sp, #32
	stp x29, x30, [sp, #32]
	add x29, sp, #32
	adrp x28, wl_str_jgets@PAGE
	add x28, x28, wl_str_jgets@PAGEOFF
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
	ldr x0, [sp, #8]
	ldr x1, [sp, #16]
	bl _printf
	b .wl_jgeT_is_0_cont
	.section __TEXT,__text
	.global _jleT
	.p2align 2
_jleT:
	sub sp, sp, #32
	stp x29, x30, [sp, #32]
	add x29, sp, #32
	adrp x28, wl_str_jlets@PAGE
	add x28, x28, wl_str_jlets@PAGEOFF
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
	ldr x0, [sp, #8]
	ldr x1, [sp, #16]
	bl _printf
	b .wl_jleT_is_0_cont
	.section __TEXT,__text
	.global _main
	.p2align 2
_main:
	sub sp, sp, #32
	stp x29, x30, [sp, #32]
	add x29, sp, #32
	adrp x0,wl_str_iftest@PAGE
	add x0, x0, wl_str_iftest@PAGEOFF
	bl _printf
	bl _jeT
	bl _jneT
	bl _jgeT
	bl _jleT
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
