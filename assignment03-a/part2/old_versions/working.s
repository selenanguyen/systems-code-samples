
.global _start

.select.str:
	.asciz "Select an operation\n"

.opadd.str:
	.asciz "1 - add\n"

.opsub.str:
	.asciz "2 - subtract\n"

.negative:
	.ascii "-"

.newline:
	.ascii "\n"

.bss	
	userchoice: .skip 8	
	userinput: .skip 8
	userinput2: .skip 8

.data
	digits: .byte 0,0,0,0,0,0,0,0
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

_add:
	movq $0, %rax
	movq $0, %rdi
	movq $userinput, %rsi
	movq $8, %rdx	
	syscall

	movq userinput, %r12

	movq $0, %rax
	movq $0, %rdi
	movq $userinput2, %rsi
	movq $8, %rdx
	syscall
	
	movq userinput2, %r13
	
	subq $2608, %r12 
	subq $2608, %r13	

	addq %r12, %r13
	
	############# PRINTING NUMBER SECTION #######
_outputNumber:
	test %r13, %r13
	js _neg

_returnToPrint:
	
	movq %r13, %rax
	movq $0, %r15

.loop:
	movq $10, %rdi
	movq $0, %rdx
	idivq %rdi
	movq %rdx, digits(,%r15,1)

	addq $48,digits(,%r15,1)
	incq %r15

	cmpq $0, %rax
	jne .loop

.chars:
	decq %r15
	leaq digits(,%r15,1), %rsi
	movq $1, %rax
	movq $1, %rdi
	movq $1, %rdx
	syscall

	cmpq $0, %r15
	jne .chars
	
	jmp _returnToEnd

_sub:
	movq $0, %rax
	movq $0, %rdi
	movq $userinput, %rsi
	movq $8, %rdx	
	syscall

	movq userinput, %r12

	movq $0, %rax
	movq $0, %rdi
	movq $userinput2, %rsi
	movq $8, %rdx
	syscall
	
	movq userinput2, %r13
	
	subq $2608, %r12 
	subq $2608, %r13	

	subq %r13, %r12
	movq %r12, %r13

	jmp _outputNumber

_neg:
	imul $-1, %r13
	movq $1,%rax           
	movq $1,%rdi          
	leaq .negative, %rsi
	movq $1, %rdx
	syscall
	
	jmp _returnToPrint

_writeEndLine:
	movq $1,%rax           
	movq $1,%rdi          
	leaq .newline, %rsi  
	movq $1, %rdx                   
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

	movq $0, %rax
	movq $0, %rdi
	movq $userchoice, %rsi
	movq $8, %rdx
	syscall

	cmpq $0xa31,userchoice
	je _add
	
	jne _sub

_returnToEnd:

	call _writeEndLine
	call _exitProgram
	
	pop %rbp

