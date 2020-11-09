# Part 2 Assembly - Writing Functions in Assembly

# Introduction

For this part of the assignent you are going to be practicing  [Assembly programming](https://en.wikipedia.org/wiki/Assembly_language)! Note that, in the past when teaching assembly, I have noted that this can be a more difficult assignment for the following reasons:

## Assignment Strategy

1. You cannot simply dive into the code without a plan.
2. You really should do the readings, or at least study an assembly cheat sheet before and while you are attempting the assignment.
3. You should use the [compiler generated assembly](https://godbolt.org/) as a helper--but only use really really small examples to test your understanding (It will be obvious if you write your code in C, and submit the assembly--you may not do that). 
4. Luckily--for this assignment we are taking a small jump into the assembly. In future courses, you may write some assembly from scratch!
5. Finally, Professor Shah recorded a few videos on assembly programming that may be of use: 
[https://www.youtube.com/playlist?list=PLvv0ScY6vfd9BSBznpARlKGziF1xrlU54](https://www.youtube.com/playlist?list=PLvv0ScY6vfd9BSBznpARlKGziF1xrlU54) -- these won't solve all problems, but hopefully give you a kickstart!

**Note** It is important that you work on the assignment on the Khoury servers (That is where you are testing, and your personal machine may have a different architecture!). You should be using a text editor like vim, nano, or emacs while working remotely. Practicing using these tools over time will help you streamline an optimal workflow weather you are working remotely or on your local desktop.

## Assembly Refresher

We have previously looked at some examples of assembly code. And we additionally have an understanding of how the stack stores data in memory by pushing in data. "Removing" data is then done by manipulating the stack pointer which is stored in the %rsp register (remember, %rsp always points to the top of the stack). There are some concerns however that you may have in your mind:

- How do I write 'functions' in assembly to make assembly code writing *easier* and more *maintainable*.?
- If we only have 16 registers, how can I write functions with a lot of arguments?
- How do I jump into a function and return back to where I left off?

You might have yet more questions even--and that is wonderful! Remember, assembly programming is sort of similar to solving a puzzle and getting everything to fit just right. Let me however remind you to the jobs of a procedure/function/method to orient you with what we must achieve:

1. Pass control
	- Start executing from start of procedure.
	- Return back to where we called from.
2. Pass data
	- Procedure arguments and return value are passed.
3. Memory management
	- Memory allocated in the procedure, and then deallocated on return.
4. Try to do this in the most minimal way.

## Background on functions

<img align="right" width="400px" src="http://www.cs.virginia.edu/~evans/cs216/guides/stack-convention.png" alt="Stack from http://www.cs.virginia.edu/~evans/cs216/guides/x86.html">

Writing a function in assembly almost follows a 'design recipe', and it is very useful to draw a picture of the stack (an example is shown to the right) as you write your function. Think of a function as broken into three different parts.

1. The setup - Sets up our stack
2. The body - Does the work
3. The finish/return - Pops things off the stack so we can resume execution to where we left off.

### The setup

This is where we are starting the execution of our function. Typically, it looks something like this

```asm
pushq %rbp
movq %rsp, %rbp
pushq %rbx
```

### The body

This is where the work is done.

### The finish/return

This is where we have to pop our stack, because we are done with our temporary (i.e. local) variables. Our temporaries or local variables are things like the arguments of a function that were passed in. Typically this looks something like this:

```asm
movq -8(%rbp),%rbx
movq %rbp, %rsp
popq %rbp
ret
```

## Assembly Example 1

I want to provide a few assembly examples, so you can see how the work is done. Try these examples first before jumping straight into assembly programming. This first example is of using a subroutine, or a sort of 'function call' with no arguments. This is a nice way to keep our code relatively clean.

```asm
# Assemble with
# 1.) gcc - c funcion.s
# 2.) ld function.o -o function
# 3.) ./function
.global _start

# Here we have a label, and we directly place some ASCII text
# into our program.
.welcome:
    .ascii "Welcome to this example\12" # A 24 character long
                                        # string. Note the \12
                                        # at the end is a 'line feed'
.text

# _printWelcome is a subroutine (i.e. a block of code we can execute)
# This is one way to 'modularize' our code.
_printWelcome:
    movq $1, %rax
    movq $1, %rdi
    leaq .welcome,%rsi # Load effective address, is a
    movq $24, %rdx
    syscall
    ret

# A subroutine to exit the program
_exit:
    movq $60, %rax
    xor %rdi, %rdi
    syscall

# The entry point into our program
_start:

    call _printWelcome
    call _exit
```

## Assembly Example 2

Here is an example of a simple function that is provided by a nice set of videos: [https://www.youtube.com/watch?v=S-ZDUYMoy3Y&list=PLHMcG0zmCZcj7hKyHgQGGEZ41UWKaISKU](https://www.youtube.com/watch?v=S-ZDUYMoy3Y&list=PLHMcG0zmCZcj7hKyHgQGGEZ41UWKaISKU) See if you can follow along! I recommend watching all of the videos to truly understand the work being done--they might be helpful for Task 1!

## Task 1

Your task is to write two assembly functions--an `long add(long, long)` and `long subtract(long,long)` function in a file called *function.s*. A user will select which of the two functions to execute before terminating. A sample of the output from your program should look something like the following.

```
Select an operation
1 - add
2 - subtract
1
6
5
11
```

Here is the same output with my comments (To make it clear)

```
Select an operation # Use a syscall to write this out
1 - add		    # Use a syscall to write this out
2 - subtract        # Use a syscall to write this out
1 		    # The user selected '1'
6                   # The first input
5                   # The second input
11                  # 6+5 = 11, so print out '11' 
                    # Note: if this had been subtraction 
		    # (i.e. user selected '2' at the start), it would be 6-5 = 1                   
```

Some things I foresee in this assignment that may be tricky are:

- How to get user input.
- Think about if we read in a '1' is that stored as a *long with a value of 1* or some other representation of the ascii 1.
	- What if we sum to relatively large numbers, like 5000 and 6000? How would we output them, and would it be useful to have a function to do so?

# Rubric

<table>
  <tbody>
    <tr>
      <th>Points</th>
      <th align="center">Description</th>
    </tr>
    <tr>
      <td>45% (Does your function.s work.)</td>
      <td align="left"><ul><li>10% for a correct output.</li><li>10% for a correct add function</li><li>10% for a correct subtract function</li><li>15% for structure of program (no weird formatting, no wasteful instructions)</li></ul></td>
    </tr>      
  </tbody>
</table>

# F.A.Q.

- Q: Can I do this assignment before using GDB?
	- A: Probably not very easily--using gdb shows us registers addresses and values directly. Do the GDB part first.
- Q: May I use C functions in my assembly?
	- A: Unfortunately no--we can write better ones in assembly ourselves using syscalls.
- Q: May I assume valid user input?
	- A: Yes, users will not input values like '#' + '?L' or anything weird to sum or subtract.

# Resources to help

- A handy syscall table
	- [https://filippo.io/linux-syscall-table/](https://filippo.io/linux-syscall-table/)

# Feedback Loop

(An optional task that will reinforce your learning throughout the semester)

- Continue using the Compiler Explorer as you write your C code. [https://godbolt.org](https://godbolt.org)
