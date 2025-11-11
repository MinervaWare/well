	.text
	.global on_ready
on_ready:
	ret
	.text
	.global on_message
on_message:
	ret
	.text
	.global main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $16, %rsp
	movq wl_str_BOT_TOKEN(%rip),%rdi
	call discord_init
	movq wl_int_DISCORD_GATEWAY_MESSAGE_CONTENT(%rip),%rsi
	call discord_add_intents
	movq wl_str_test(%rip),%rdi
	call printf
	movl $0, %eax
	addq $16, %rsp
	popq %rbp
	ret
	.text
	.global wl_str_BOT_TOKEN
.rawwl_strBOT_TOKEN:
	.ascii "TOKEN"
	.data
	.align 8
wl_str_BOT_TOKEN:
	.quad .rawwl_strBOT_TOKEN
	.text
	.global wl_str_test
.rawwl_strtest:
	.ascii "DO NOT REDEEM!\n"
	.data
	.align 8
wl_str_test:
	.quad .rawwl_strtest
