	.extern printf
	.align 1
	.global jeT
jeT:
	addi sp,sp,-48
	sd ra,40(sp)
	sd s0,32(sp)
	addi s0,sp,48
	lla t0,wl_str_jets
	sd t0,-28(s0)
	li t0,10
	sw t0,-36(s0)
	lw t0,-36(s0)
	li t1,10
	bne t0,t1,.wl_jeT_is_09_cont
	jal .wl_jeT_is_09
.wl_jeT_is_09_cont:
	ld ra,40(sp)
	ld s0,32(sp)
	addi sp,sp,48
	jr ra
	ret
.wl_jeT_is_09:
	ld a0,-28(s0)
	addi a1,s0,-36
	call printf@plt
	mv a6,a0
	jal .wl_jeT_is_09_cont
	.align 1
	.global jneT
jneT:
	addi sp,sp,-48
	sd ra,40(sp)
	sd s0,32(sp)
	addi s0,sp,48
	lla t0,wl_str_jnets
	sd t0,-28(s0)
	li t0,5
	sw t0,-36(s0)
	lw t0,-36(s0)
	li t1,10
	beq t0,t1,.wl_jneT_is_019_cont
	jal .wl_jneT_is_019
.wl_jneT_is_019_cont:
	ld ra,40(sp)
	ld s0,32(sp)
	addi sp,sp,48
	jr ra
	ret
.wl_jneT_is_019:
	ld a0,-28(s0)
	addi a1,s0,-36
	call printf@plt
	mv a6,a0
	jal .wl_jneT_is_019_cont
	.align 1
	.global jgeT
jgeT:
	addi sp,sp,-48
	sd ra,40(sp)
	sd s0,32(sp)
	addi s0,sp,48
	lla t0,wl_str_jgets
	sd t0,-28(s0)
	li t0,11
	sw t0,-36(s0)
	lw t0,-36(s0)
	li t1,10
	ble t0,t1,.wl_jgeT_is_029_cont
	jal .wl_jgeT_is_029
.wl_jgeT_is_029_cont:
	ld ra,40(sp)
	ld s0,32(sp)
	addi sp,sp,48
	jr ra
	ret
.wl_jgeT_is_029:
	ld a0,-28(s0)
	addi a1,s0,-36
	call printf@plt
	mv a6,a0
	jal .wl_jgeT_is_029_cont
	.align 1
	.global jleT
jleT:
	addi sp,sp,-48
	sd ra,40(sp)
	sd s0,32(sp)
	addi s0,sp,48
	lla t0,wl_str_jlets
	sd t0,-28(s0)
	li t0,9
	sw t0,-36(s0)
	lw t0,-36(s0)
	li t1,10
	bge t0,t1,.wl_jleT_is_039_cont
	jal .wl_jleT_is_039
.wl_jleT_is_039_cont:
	ld ra,40(sp)
	ld s0,32(sp)
	addi sp,sp,48
	jr ra
	ret
.wl_jleT_is_039:
	ld a0,-28(s0)
	addi a1,s0,-36
	call printf@plt
	mv a6,a0
	jal .wl_jleT_is_039_cont
	.align 1
	.global main
main:
	addi sp,sp,-48
	sd ra,40(sp)
	sd s0,32(sp)
	addi s0,sp,48
	lla a0,wl_str_iftest
	call printf@plt
	mv a6,a0
	call jeT
	mv a6,a0
	call jneT
	mv a6,a0
	call jgeT
	mv a6,a0
	call jleT
	mv a6,a0
	mv a0,a0
	ld ra,40(sp)
	ld s0,32(sp)
	addi sp,sp,48
	jr ra

	.align 3
wl_str_iftest:
	.string "- - - If Statement Operator Tests - - -\n"

	.align 3
wl_str_jets:
	.string "(i == 10) pass\n"

	.align 3
wl_str_jnets:
	.string "(i != 10) pass\n"

	.align 3
wl_str_jgets:
	.string "(i >= 10) pass\n"

	.align 3
wl_str_jlets:
	.string "(i <= 10) pass\n"
	.ident "Well: (GNU/Linux) 0.0.0"
