	.text
	.align 2
	.global main	
	.set nomips16
	.set nomicromips
	.ent main
main:
	.set noreorder
	.cpload $25
	addiu $sp,$sp,-32
	sw $31,28($sp)
	sw $fp,24($sp)
	move $fp,$sp
	.cprestore 16	
	lw $25,%call16(printf)($28)	
	.reloc 1f,R_MIPS_JALR,printf
1:	jalr $25
	nop
	lw $28,16($fp)
	move $2,$4
	.end main
	.rdata
	.align 2
wl_str_text:
	.asciz "Hello World!\n"
