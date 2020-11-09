.global _start

########################################
#This section contains string constants.
#The first 3 are for printing to the user
#The 4th is for outputting a - symbol to denote a negative value
#The 5th is for printing a new line
########################################
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

.equ STDOUT, 1
.equ WRITE, 1
.equ STDIN, 0
.equ READ, 0
.equ EXIT, 60
.equ EXIT_SUCCESS, 0

########################################
#This section contains variables
#userchoice = input for 1 or 2, determining whether to add/subtract
#userinput = the first digit inputted
#userinput2 = the second digit inputted
#digits is used as temporary storage to print the full number
########################################
.bss	
	userchoice: .skip 8	
	userinput: .skip 8
	userinput2: .skip 8

.data
	digits: .byte 0,0,0,0,0,0,0,0

#######################################
#Code section
#######################################
.text
#######################################
#This section prints out the welcome message to the user:
#Select an operation
#1 - add
#2 - subtract
######################################
#It uses the string constants defined above.
#We move 1 to rax because we want to do a write system call.
#We move 1 to rdi to represent stdout as the file descriptor.
#We move the ascii constants from above into rsi, the buffer.
#We move the length of the respective strings to rdx.
#We call a syscall after each one to print to the user
#We do a ret to return back to _start
######################################
_writeWelcome:
	movq $WRITE, %rax
	movq $STDOUT, %rdi
	leaq .select.str, %rsi
	movq $20, %rdx
	syscall
	
	movq $WRITE, %rax
	movq $STDOUT, %rdi
	leaq .opadd.str, %rsi
	movq $8, %rdx
	syscall
	
	movq $WRITE, %rax
	movq $STDOUT, %rdi
	leaq .opsub.str, %rsi
	movq $13, %rdx
	syscall
	ret

######################################
#This section takes in two numerical digit inputs from the user
#and stores them in two registers, r12 and r13.
######################################
#We move 0 to rax because we want to do a read system call.
#We move 0 to rdi to represent stdin as the file descriptor
#We move the constants from .bss, where we want the input
#	 to be stored, to rsi (the buffer).
#We move $8, the size of the input, to rdx, the count.
#We move the inputs into two different registers, r12 and r13.
#We subtract 2608 from the registers to remove the new-line character.
#We return to _start using ret.
#At the end of this section, the two numbers in r12 and r13
#	are the numerical digits that the user inputted.
#####################################
_takeNumInputs:
	movq $READ, %rax
	movq $STDIN, %rdi
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

	ret


#####################################
#This section adds the two registers r12 and r13,
#stores the result in r13, and jumps to the _outputNumber
#label to print the number stored in r13.
####################################
_add:
	addq %r12, %r13
	jmp _outputNumber

###################################
#This section subtracts r13 from r12, storing the value in r12.
#Since the next section we are jumping to is expecting the value
#to be in r13, we move the value in r12 to r13.
###################################
_sub:
	subq %r13, %r12
	movq %r12, %r13
	jmp _outputNumber

###################################
#This section (through the end of _returnToPrint and .chars)
# 	prints out the number in r13 to the user.
#It it designed to handle non-single digit values.
#This section was adapted from hello15.s on Piazza.
##################################
#The first step is to test if the number is negative.
#For that we use test. Then the sign register is set to the correct value.
#Then we do a js (jump signed) to _neg).
#_neg will jump to _returnToPrint, otherwise if the test is not true we
#continue to _returnToPrint anyway.
#Then we print a big number
##################################
#The first step is to move the value of %r13 into %rax.
# %rax is where we do division
#r15 is counting the number of digits.
#In the loop to get each digit, we divide by 10.
#The remainder is stored in rdx. The quotient is stored in rax.
#We move the remainder into the digits constant defined previously
#We add 48 to that constant to convert to ascii.
#We increment r15 to count the number of digits total.
#We compare rax (the quotient) to zero to determine if we need to continue
#	dividing. If we do, jump to .loop.
#################################
#Once all the numbers are in digits, we have to print them out.
#We are looping through the ascii characters stored in digits, backwards.
#We load the current digit into rsi, then print it the usual way
#using syscall.
#If r15 is not zero, we continue printing until it is.
#When we're done we jump to _returnToEnd to close the program
#################################
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
	movq $WRITE, %rax
	movq $STDOUT, %rdi
	movq $1, %rdx
	syscall

	cmpq $0, %r15
	jne .chars
	
	jmp _returnToEnd

####################################
#This section turns the value in r13 positive
#(the only way to be here is if the value was originally negative).
#Then it prints a - symbol, so that before the positive
#number is outputted, a - symbol is outputted to denote the number
#is negative.
####################################
#We multiply r13 by -1 to turn the value positive.
#We put 1 in rax for the syscall type
#We put 1 in rdi to represent stdout.
#We put the negative symbol in rsi to print
#The length of the string containing the negative symbol is 1.
#We syscall, then return to the section that prints the number
###################################
_neg:
	imul $-1, %r13

	movq $WRITE, %rax           
	movq $STDOUT, %rdi          
	leaq .negative, %rsi
	movq $1, %rdx
	syscall
	
	jmp _returnToPrint


##################################
#This section prints a new line character after the number is printed.
##################################
#1 is moved into rax, to represent a write syscall
#1 is moved into rdi to represent stdout.
#We load the new line character into rsi
#The length of the string containing the new line character is 1.
#We syscall and return to the section that called this section
#################################
_writeEndLine:
	movq $WRITE, %rax           
	movq $STDOUT, %rdi          
	leaq .newline, %rsi  
	movq $1, %rdx                   
	syscall        

	ret


################################
#This section exits the program with a syscall.
################################
#We move 60, the number representing sys_exit, into rax.
#We move 0 into rdi so that the program exits with "exit code 0".
#We syscall and return to the section that called this section (_returnToEnd).
###############################
_exitProgram:
	movq $EXIT, %rax
	movq $EXIT_SUCCESS, %rdi
	syscall
	ret


###############################
#This section is the "main function" of our program.
###############################
#We move the stack pointer to the base pointer.
#We write the welcome message (call _writeWelcome) which ret's to here.
#We take inputs from the user (call _takeNumInputs) which ret's to here.
###############################
#We take in a user input to determine whether we are adding or subtracting.
#	1 = addition and 2 = subtraction. However, for simplicity we treat
#	all values != 1 to be subtraction.
###############################
#We put 0 into rax to represent a read syscall
#We put 0 in rdi to represent stdin for the file descriptor
#We put userchoice into rsi as the buffer.
#We put 8 into rdx as the size of the buffer.
#We syscall to execute the user input step
#We compare the userchoice to $0xa31, which is the hex value for 1
#	and a newline character.
#If they are equal, we jump to the add routine.
#If they are not equal, we jump to the sub(tract) routine.
##############################
_start:

	movq %rsp, %rbp

	call _writeWelcome
	call _takeNumInputs

	movq $0, %rax
	movq $0, %rdi
	movq $userchoice, %rsi
	movq $8, %rdx
	syscall

	cmpq $0xa31, userchoice
	je _add
	jne _sub

##############################
#This section is the end of the program. It is jumped to from the
#	printing of the number.
##############################
#We call writeEndLine to print a new line to the user at the
#since the numerical value of the output was just printed
#We call exitProgram to do an exit syscall
#Then we pop the base pointer, as is customary.
##############################
_returnToEnd:

	call _writeEndLine
	call _exitProgram
	
	pop %rbp
