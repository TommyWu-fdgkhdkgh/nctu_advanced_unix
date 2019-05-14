; RDI, RSI, RDX, RCX, R8, R9

JB_RBX	EQU 0
JB_RBP  EQU 1
JB_R12  EQU 2
JB_R13  EQU 3
JB_R14  EQU 4
JB_R15  EQU 5
JB_RSP  EQU 6
JB_PC   EQU 7
JB_SIZE EQU (8*8)


section .data


section .text

;	Q:為什麼要push	r10??
;<<<sys_read
global sys_read:function
sys_read:
	push	r10
	mov	r10,	rcx
	mov	rax,	0
	syscall
	pop	r10
	ret
;<<<sys_write
global sys_write:function
sys_write:
	push	r10
	mov	r10,	rcx
	mov	rax,	1
	syscall
	pop	r10
	ret
;<<<sys_open
global sys_open:function
sys_open:
	push	r10
	mov	r10,	rcx
	mov	rax,	2
	syscall
	pop	r10
	ret
;<<<sys_close
global sys_close:function
sys_close:
	push	r10
	mov	r10,	rcx
	mov	rax,	3
	syscall
	pop	r10
	ret
;<<<sys_exit
global sys_exit:function
sys_exit:
	push	r10
	mov	r10,	rcx
	mov	rax,	60
	syscall
	pop	r10
	ret
;<<<sys_alarm
global sys_alarm:function
sys_alarm:
	push	r10
	mov	r10,	rcx
	mov	rax,	37
	syscall
	pop	r10
	ret
;<<<sys_pause
global sys_pause:function
sys_pause:
	push	r10
	mov	r10,	rcx
	mov	rax,	34
	syscall
	pop	r10
	ret
;<<<sys_nanosleep
global sys_nanosleep:function
sys_nanosleep:
	push	r10
	mov	r10,	rcx
	mov	rax,	35
	syscall
	pop r10
	ret
;<<<sys_rt_sigprocmask
global sys_rt_sigprocmask:function
sys_rt_sigprocmask:
	push	r10
	mov	r10,	rcx
	mov	rax,	14
	syscall
	pop	r10
	ret
;<<<sys_rt_sigpending
global sys_rt_sigpending:function
sys_rt_sigpending:
	push	r10
	mov	r10,	rcx
	mov	rax,	127
	syscall
	pop	r10
	ret
;<<<sys_rt_sigaction
global sys_rt_sigaction:function
sys_rt_sigaction:
	push	r10
	mov	r10,	rcx
	mov	rax,	13
	syscall
	pop	r10
	ret
;<<<sys_rt_sigreturn
global sys_rt_sigreturn:function
sys_rt_sigreturn:
	;push	r10
	;mov	r10,	rcx
	mov	rax,	15
	syscall
	;pop	r10
	;ret
;<<<setjmp
global setjmp:function
setjmp:
	;保存rbp
	push	rbp
	mov	rbp,	rsp	
	
	;在stack上清出一個空間
	sub	rsp,	0x10
	
	mov	[rdi+JB_RBX*8],	rbx
	mov	rbx,	[rbp+8]

	mov	[rdi+JB_RBP*8],	rbp
	mov	[rdi+JB_R12*8],	r12
	mov	[rdi+JB_R13*8],	r13
	mov	[rdi+JB_R14*8],	r14
	mov	[rdi+JB_R15*8],	r15
	mov	[rdi+JB_RSP*8],	rsp
	mov	[rdi+JB_PC*8],	rbx

	mov	rax,	0

	;回復原本的rsp
	mov	rsp,	rbp
	;回復原本的rbp
	pop	rbp
	ret
;<<<longjmp
global longjmp:function
longjmp:
	;push	rbp
	;mov	rbp,	rsp

	mov	rbx,	[rdi+JB_RBX*8]
	mov	r12,	[rdi+JB_R12*8]
	mov	r13,	[rdi+JB_R13*8]
	mov	r14,	[rdi+JB_R14*8]
	mov	r15,	[rdi+JB_R15*8]
	mov	eax,	esi
	mov	rsp,	[rdi+JB_RSP*8]
	mov	rbp,	[rdi+JB_RBP*8]
	mov	rdx,	[rdi+JB_PC*8]
	jmp	rdx

	;mov	rsp,	rbp
	;pop	rbp
	;ret






