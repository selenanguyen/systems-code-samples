

In a brief sentence or two,  modify this file with your answers.

1. In each of the programs what was the bug?

- prog: The program constructs a linked list from Z -> A in reverse order so that the linked list has items in the correct order. It does this by looping through the ascii numbers for Z -> A. It then prints it out in correct order from A -> Z. The program seg-faults after printing out the nodes. The instruction `mov -0x18(%rbp),%rax` is moving a value from offset -0x18 from the base pointer into %rax, which is 0. In the next instruction, `movzbl (%rax),%eax`, it is trying to dereference 0, which causes the seg-fault.

- prog1: There is a call to `mov -0x8(%rbp),%rax`. Accessing the memory location with -8 offset from %rbp (the base pointer) is an inaccessible memory location, which is causing the seg-fault. Additionally, the base pointer and the stack pointer are the same--since we looking at a location relative to the base pointer, I think it is possible that memory was not allocated properly because the stack pointer was not moved.

2. What file and lines did the bugs occur?

- prog: The program crashes at prog2.c:57, which is main+96. It is crashing on the dereference of %rax `(%rax)`.

- prog1: The program crashes at seg.c:15, which is initialize_front+4; it is crashing when accessing `-0x8(%rbp)`.

3. What would your educated guess be for a fix for each bug?

- prog: Correct the location that is being moved into %rax; it is not the right location, since its value is 0.

- prog1: The call to `-0x8(%rbp)` needs to be changed. 
