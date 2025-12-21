	.extern OEInitRenderer
	.extern OEEnableDebugInfo
	.extern OEEnableFXAA
	.extern OEEnableSSGI
	.extern OERendererIsRunning
	.extern OEPollEvents
	.extern OERenderFrame
	.extern OEDrawObject
	.extern OEGetObjectFromName
	.text
	.global event
event:
	addq $32, %rsp
	popq %rbp
	ret
	.text
	.global render
render:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movq wl_str_obj(%rip), %r10
	movq %r10, -8(%rbp)
	movq -8(%rbp),%rdi
	call OEGetObjectFromName
	movq %rax,%rdi
	call OEDrawObject
	addq $32, %rsp
	popq %rbp
	ret
	.text
	.global frameLoop
frameLoop:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	call OERendererIsRunning
	cmpq $1, %rax
	je .wl_frameLoop_is_0
.wl_frameLoop_is_0_cont:
	addq $32, %rsp
	popq %rbp
	ret
.wl_frameLoop_is_0:
	movq event(%rip),%rdi
	call OEPollEvents
	movq render(%rip),%rdi
	call OERenderFrame
	call frameLoop
	jmp .wl_frameLoop_is_0_cont
	.text
	.global main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movq $1280, -8(%rbp)
	movq $720, -16(%rbp)
	movq wl_str_title(%rip), %r10
	movq %r10, -24(%rbp)
	movq -8(%rbp),%rdi
	movq -16(%rbp),%rsi
	movq -24(%rbp),%rdx
	movq $0,%rcx
	call OEInitRenderer
	call OEEnableDebugInfo
	call OEEnableFXAA
	movq $32,%rdi
	movq $8,%rsi
	call OEEnableSSGI
	call frameLoop
	movq %rdi, %rax
	addq $32, %rsp
	popq %rbp
	ret
	.text
	.global wl_str_obj
.rawwl_strobj:
	.asciz "OECube"
	.data
	.align 8
wl_str_obj:
	.quad .rawwl_strobj
	.text
	.global wl_str_title
.rawwl_strtitle:
	.asciz "OE"
	.data
	.align 8
wl_str_title:
	.quad .rawwl_strtitle
