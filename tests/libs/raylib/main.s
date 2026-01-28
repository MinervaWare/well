	.section __TEXT,__text
	.global _run
	.p2align 2
_run:
	sub sp, sp, #16
	stp x29, x30, [sp, #0]
	add x29, sp, #0
	bl _BeginDrawing
	bl _EndDrawing
	bl _WindowShouldClose
	tbz x0, #0, _run
	bl _CloseWindow
	adrp x0,wl_int_retz@PAGE
	add x0, x0, wl_int_retz@PAGEOFF
	bl _exit
	ldp x29, x30, [sp, #0]
	add sp, sp, #16
	ret
	.section __TEXT,__text
	.global _main
	.p2align 2
_main:
	sub sp, sp, #16
	stp x29, x30, [sp, #0]
	add x29, sp, #0
	adrp x0,wl_int_screenWidth@PAGE
	add x0, x0, wl_int_screenWidth@PAGEOFF
	adrp x1,wl_int_screenHeight@PAGE
	add x1, x1, wl_int_screenHeight@PAGEOFF
	adrp x2,wl_str.title@PAGE
	add x2, x2, wl_str.title@PAGEOFF
	bl _InitWindow
	adrp x0,wl_int_fpsLimit@PAGE
	add x0, x0, wl_int_fpsLimit@PAGEOFF
	bl _SetTargetFPS
	bl _WindowShouldClose
	tbz x0, #0, _run
	bl _CloseWindow
	mov x0, #0
	ldp x29, x30, [sp, #0]
	add sp, sp, #16
	ret
wl_str.title:
	.asciz "Hello Raylib!"

	.global wl_int_screenWidth
	.p2align 2,0x0
wl_int_screenWidth:
	.long 480

	.global wl_int_screenHeight
	.p2align 2,0x0
wl_int_screenHeight:
	.long 480

	.global wl_int_fpsLimit
	.p2align 2,0x0
wl_int_fpsLimit:
	.long 60

	.global wl_int_retz
	.p2align 2,0x0
wl_int_retz:
	.long 1
