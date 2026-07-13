	.extern printf
	.align 1
	.global switch
switch:
	addi sp,sp,-48
	sd ra,40(sp)
	sd s0,32(sp)
	addi s0,sp,48
	li t0,0
	sw t0,-28(s0)
	sw a0,-28(s0)
	mv a0,a1
	addi a1,s0,-28
	ld ra,40(sp)
	ld s0,32(sp)
	addi sp,sp,48
	jr ra
	ret
	.align 1
	.global main
main:
	addi sp,sp,-48
	sd ra,40(sp)
	sd s0,32(sp)
	addi s0,sp,48
	lla t0,wl_str_testLocal
	sd t0,-28(s0)
	li t0,5
	sw t0,-36(s0)
	li t0,2
	sw t0,-44(s0)
	lla t0,wl_str_sw
	sd t0,-52(s0)
	ld a0,-28(s0)
	call printf@plt
	mv a6,a0
	addi a0,s0,-36
	addi a1,s0,-44
	call switch
	mv a6,a0
	mv a2,a1
	mv a1,a0
	ld a0,-52(s0)
	call printf@plt
	mv a6,a0
	mv a0,a0
	ld ra,40(sp)
	ld s0,32(sp)
	addi sp,sp,48
	jr ra

	.align 3
wl_str_testLocal:
	.string "I am the Walrus\n"

	.align 3
wl_str_sw:
	.string "a: %d, b: %d\n"
	.ident "Well: (GNU/Linux) 0.0.0"
