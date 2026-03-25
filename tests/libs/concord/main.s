	.global _on_ready
	.p2align 2
_on_ready:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x15, wl_str_ready@PAGE
	add x15, x15, wl_str_ready@PAGEOFF
	str x15, [sp, 39]
	ldr x0, [sp, #39]
	bl _printf
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
	.global _on_message
	.p2align 2
_on_message:
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
	.global _main
	.p2align 2
_main:
	sub sp, sp, #80
	stp x29, x30, [sp, #64]
	add x29, sp, #64
	adrp x15, wl_str_client@PAGE
	add x15, x15, wl_str_client@PAGEOFF
	str x15, [sp, 39]
	adrp x0,wl_str_json@PAGE
	add x0, x0, wl_str_json@PAGEOFF
	bl _discord_config_init
	str x0, [sp, #39]
	ldr x0, [sp, #39]
	adrp x1,_DISCORD_GATEWAY_MESSAGE_CONTENT@PAGE
	add x1, x1, _DISCORD_GATEWAY_MESSAGE_CONTENT@PAGEOFF
	bl _discord_add_intents
	ldr x0, [sp, #39]
	adrp x1,_on_ready@PAGE
	add x1, x1, _on_ready@PAGEOFF
	bl _discord_set_on_ready
	ldr x0, [sp, #39]
	adrp x1,_on_message@PAGE
	add x1, x1, _on_message@PAGEOFF
	bl _discord_set_on_message_create
	ldr x0, [sp, #39]
	bl _discord_run
	mov x0, x0
	ldp x29, x30, [sp, #64]
	add sp, sp, #80
	ret
wl_str_json:
	.asciz "./config.json"
wl_str_ready:
	.asciz "I READY!"
wl_str_client:
	.asciz ""
