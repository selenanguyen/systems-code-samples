.global _start

.intro.str:
	.ascii "Select an operation\n1 - add\n2 - subtract\n"

# Somewhere to store the input
.bss
	o: .skip 8
        a: .skip 8 # allocates room for 8 bytes
        b: .skip 8

.text

_getOperation:
	movq $0, %rax # read call
        movq $0, %rdi # stdin
        movq $o, %rsi # buffer
        movq $8, %rdx # size
        syscall
	ret

_getInputs:
	movq $0, %rax # read call
	movq $0, %rdi # stdin
	movq $a, %rsi # buffer
	movq $8, %rdx # size
	syscall
        
	movq $0, %rax # read call
        movq $0, %rdi # stdin
        movq $b, %rsi # buffer
        movq $8, %rdx # size
        syscall
	ret

_exitProgram:
        movq $60, %rax
        movq $0, %rdi
        syscall

_add:
	movq $a, %r11
	movq $b, %r12
        subq %r11, '0'
	subq %r12, '0'
	addq %r11, %r12
        movq $1, %rax
        movq $1, %rdi
        movq %r11, %rsi
        movq $2, %rdx
        syscall
	ret

_subtract:
        #subq $a, $b
        #movq $1, %rax
        #movq $1, %rdi
        #leaq $a, %rsi
        #movq $2, %rdx
        #syscall
	ret

_start:
	movq %rsp, %rbp

	# Prompt user to select an operation
	movq $1, %rax
	movq $1, %rdi
	leaq .intro.str, %rsi
	movq $41, %rdx
	syscall
	
	# Take user input
	call _getOperation
	call _getInputs

	#print value...
	#movq $1, %rax
        #movq $1, %rdi
	#movq $b, %rsi
	#movq $a, %r13
        #leaq %r13, %rsi
        #movq $1, %rdx
        #syscall
	
	call _add	
	#push %r13
	#movq $1, %r13
	#call _add
	#cmpq %r13, $o
	#je _add
	#movq $2, %r13
	#cmpq %r13, $o
	#je _subtract
	
	call _exitProgram
	pop %rbp
