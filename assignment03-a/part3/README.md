# Part 3 - Cycle Count Tool

## Introduction

You may be wondering how good are compilers at generating assembly from our C code (Similar to the way you may have a good and bad translator when communicating in different languages)? How can we be sure that our compiler is generating efficient assembly? There are afterall, many different ways to write a program that generates the same output.

## Our Tool

Part of being a good systems programmer, is the ability to build tools that measure. In this case, you are going to write a small parser in C that counts the number of instructions and counts how many instructions are needed to run that program from top to bottom.

As an example, if an assembly program had the following code:

```asm
MOVQ a, %rax
MOVQ b, %rbx
ADDQ %rbx, %rax
IMULQ %rbx
MOVQ %rax, c
```

Your [tool.c](./tool.c) will report a summary of the total Instructions(i.e. ADDQ, MOVQ, IMUQ, etc.) found. Additionally, you will estimate the total cycles needed for hardware to execute this code.

```asm
ADD 1
MOV 3
IMUL 1

Total Instructions = 5
Total Cycles = ???
```

## Specifications for tool.c

* Your tool should read in a file name through the command line arguments for what file is being analyzed 
  * You will run your program with: `./tool barbones.s`
  * (hint, investigate what argc and argv are for how to read in `barebones.s` as input to your program).
* [barebones.s](./barebones.s) is provided to be used as an example input file for your tool.
* You will modify a file called [tool.c](./tool.c) which you will implement your tool in. 
  * At the very least, your program should output counts for: ADD, SUB, MUL, DIV, MOV, LEA, PUSH, POP, RET.
    * i.e. For your analysis (and for the sake of simplicity), consider ADDQ, ADDB, ADDL, etc. each as an 'ADD' instruction.
* You may find [https://fresh2refresh.com/c-programming/c-strings/c-strcat-function/](https://fresh2refresh.com/c-programming/c-strings/c-strcat-function/) helpful for learning some string processing in C.
* Use the Agner Fog's instruction table ([http://www.agner.org/optimize/instruction_tables.pdf](http://www.agner.org/optimize/instruction_tables.pdf)) to estimate how many cycles the program will run. That is, take an instruction like MOV which typically takes 1 cycle, and sum them together and output the result.
  * You will use the **Nehalem** Intel architecture to find your values.
  * Note that this is again an estimate (MOV with different data types itself takes different number of cycles! So your answer will be an approximation. I am expecting there to be some variance, even within the class--attempt something reasonable).
 
## Static and Dynamic Analysis Exercise questions

**Answer** the following questions in [exercises.md](./exercises.md)


# Rubric

<table>
  <tbody>
    <tr>
      <th>Points</th>
      <th align="center">Description</th>
    </tr>
      <td>40% (Cycle Count Tool)</td>
      <td align="left"><ul><li>30% Does your cycle count tool work? </li><li>i.e. no memory leaks</li><li>Number of instructions are correctly counted</li><li>(Are MoVq, MOVQ, or movq all counted?)</li><li>*Some* approximation of the cycles is given.</li></ul></td>
    </tr>   
    <tr>
      <td>5% (Write up)</td>
      <td align="left"><ul><li>Did you answer the questions on the static analysis and dynamic analysis in exercises.md?</li></ul></td>
    </tr>       
  </tbody>
</table>

# Resources to help

- [http://www.cplusplus.com/reference/cstring/](http://www.cplusplus.com/reference/cstring/)

# Feedback Loop

(An optional task that will reinforce your learning throughout the semester)

 * (Optional for fun) Try running barebones or another assembly program after compiling with -O0 and -O3 and see if the cycle count changes from your tool.
