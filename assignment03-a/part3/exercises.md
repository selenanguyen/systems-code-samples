## Static Analysis 
 
Our Cycle Count Tool is what we call a 'static analysis'. That is, it uncovers information about our programs before it is running (during compile-time). Given that our tool uncovers information before the program, what is (at least) one pro of this, and one con you can think of?

Pro:
1. *Static analysis is beneficial because it's quicker and cost-effective. It requires less effort, because you don't need to run your code to find errors, determine performance or detect other information about the program.*
2. *It is convenient for finding information and therefore improving performance or fixing errors as you design a program.*
3. *It can detect areas of code that may require more testing or deeper review, as well as suggest improvements to make code more maintainable.*

Con:
1. *Static analysis may be less beneficial because it is less comprehensive. It only detects general errors/information and therefore cannot provide information specific to an iteration of the program.*
2. *A program may compile or indicate good performance (like low cycle counts) but may not detect problems that may occur when the program is actually being run.*
3. *Errors detected by static analysis are predictions based on analysis of the code, and can miss errors that can only be revealed at the moment of their occurrences.*

## Dynamic Analysis

The opposite of a static analysis is a dynamic analysis. Dynamic analysis tools record and return information about programs that are in the process or have finished executing. An example of a dynamic analysis tool is [valgrind](http://valgrind.org/). What do you think a pro and con of dynamic analysis would be?

Pro:
1. *Dynamic analysis can be more benficial to static analysis because it is more comprehensive and provides information specific to an iteration. For instance, if a program breaks or has poor performance for specific data, dynamic analysis can provide more specific information for debugging.*
2. *With sufficient testing, it can reveal more vulnerabilities in the program, as well as program errors that static analysis may not detect, such as memory leaks or null pointer dereferencing.*
3. *If you need information about how a program performs with specific data input, dynamic analysis provides this. It allows analysis of specific iterations to provide useful information to the user (e.g. information about behavior with edge cases).*

Con:
1. *Dynamic analysis is less cost-efficient. It requires the source code as well as real data to run the program and detect information about it. It requires more effort, resources and sufficient testing to provide comprehensive information and detect vulnerabilities in a program.*
2. *Dynamic analysis only offers information specific to an iteration. This can be limiting. Because dynamic analysis reveals information based only on concrete inputs, if testing or inputted data is insufficient, other defects in the program may not be revealed.*
3. *Once a program is run, the execution of potential errors may lead to unpredictable consequences. It cannot predict potential errors based on source code; errors are only revealed as they occur, which can snowball into unpredictable behaviors.*
