.global _start

.select.str:
	.asciz "Select an operation\n"

.opadd.str:
	.asciz "1 - add\n"

.opsub.str:
	.asciz "2 - subtract\n"

.bss
	userin: .skip 16
	numinx: .skip 8
	numiny: .skip 8
.text

_writeWelcome:
	movq $1, %rax
	movq $1, %rdi
	leaq .select.str, %rsi
	movq $20, %rdx
	syscall
	
	movq $1, %rax
	movq $1, %rdi
	leaq .opadd.str, %rsi
	movq $8, %rdx
	syscall
	
	movq $1, %rax
	movq $1, %rdi
	leaq .opsub.str, %rsi
	movq $13, %rdx
	syscall
	ret

_getUserin:
	movq $0, %rax
	movq $0, %rdi
	movq $userin, %rsi
	movq $16, %rdx
	syscall
	ret

_add2:
	mov $0, %rax
	mov $0, %rdi
	mov -0x4(%rsp), %rsi
	mov $4, %rdx
	syscall

	#xor %rax, %rax
	#mov $numinx, %rbx
	#sub 0x30, %rbx
	#and %rbx, 0xff
	#mov %rbx, %rax

	#add %rax, %rax
	
	mov $1, %rax
	mov $1, %rdi
	mov -0x4(%rsp), %rsi
	mov $4, %rdx
	syscall
	ret

_add:
	movq $0, %rax
	movq $0, %rdi
	movq $numinx, %rsi
	movq $16, %rdx
	syscall

	movq $0, %rax
	movq $0, %rdi
	movq $numiny, %rsi
	movq $16, %rdx
	syscall
	
	movq $numinx, %r11
	movq $48, %r13
	subq %r11, %r13 
	
	movq $numiny, %r12
	movq $48, %r13
	subq %r12, %r13

	addq %r11, %r12

	movq $48, %r13
	addq %r11, %r13

	movq $1, %rax
	movq $1, %rdi
	movq %r11, %rsi
	movq $8, %rdx
	syscall
	ret

_exitProgram:
	movq $60, %rax
	movq $0, %rdi
	syscall
	ret

_start:

	movq %rsp, %rbp

	call _writeWelcome
	call _getUserin
	#mov 0x30, %r13
	#cmp $userin, %r13
	#je _add
	 call _add2

	call _exitProgram
	
	pop %rbp
