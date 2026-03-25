	.extern discord_config_init
	.extern discord_add_intents
	.extern discord_set_on_ready
	.extern discord_set_on_message_create
	.extern discord_run
	.extern discord_create_message
	.extern _log_log
	.text
	.global log_info
log_info:
	lda $30, -16($30)
	stq $26, 0($30)
	stq $15, 8($30)
	bis $31, $30, $15
	jsr $26, _log_log
	mov $15, $30
	ldq $26, 0($30)
	ldq $15, 8($30)
	lda $30, 16($30)
	ret $31, ($26), 1
	ret

	.global wl_int_DISCORD_GATEWAY_MESSAGE_CONTENT
	.p2align 2,0x0
wl_int_DISCORD_GATEWAY_MESSAGE_CONTENT:
	.long 0x8000
