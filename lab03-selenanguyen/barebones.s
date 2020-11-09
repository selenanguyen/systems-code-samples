# Build an executable using the following:
#
# clang barebones.s -o barebones  # clang is another compiler like gcc
#
.text
_barebones:

.data
	
.globl main

main:
					# (1) What are we setting up here?
					# Ans:
	pushq %rbp			# This code begins saving 
	movq  %rsp, %rbp		# registers, if needed.		

					# (2) What is going on here
					# Ans:
	movq $1, %rax			# The value 1 is moved into
	movq $1, %rdi			# 8-byte registers  %rdi and
	leaq .hello.str,%rsi		# %rax
					# 'leaq' loads the effective
					# address of the source (.hello.str)
					# into destination (%rsi) to output.


					# (3) What is syscall? We did not talk about this
					# in class.
					# Ans: A syscall is a system call,
					# or the interface between the program
					# and Linux kernal. They are used for
					# system tasks (e.g. printing to
					# the terminal).

	syscall				# Which syscall is being run?
					# Ans: syscall is invoking the OS
					# to do a write (print to terminal).
					# (sys_write)

					# (4) What would another option be instead of 
					# using a syscall to achieve this?
					# Ans: Another option would be to
					# write to stdout through its file 
					# descriptor (1).

	movq	$60, %rax		# (5) We are again setting up another syscall
	movq	$0, %rdi		# What command is it?
					# Ans: this syscall invokes the OS
					# to exit.
	syscall

	popq %rbp			# (Note we do not really need
					# this command here after the syscall)

.hello.str:
	.string "Hello World!\n"
	.size	.hello.str,13		# (6) Why is there a 13 here?
					# Ans: It is the total size of
					# the string (13 characters).
