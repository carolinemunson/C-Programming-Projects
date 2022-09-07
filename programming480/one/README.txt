Name: Caroline Munson
cssc0936@edoras.sdsu.edu
REDID: 822774843

Program 1:
Files in directory: makefile, mulproc, README

makefile:Alphabetcount and specialcount .o are listed as dependencies. I modified the makefile to import my existing program 0 files by generating and linking the .o files for alphabetcount and specialcount. The changes are reflected in lines 4, 12, 13, and 17.

mulproc: The program works to simultaneously run two child processes in parallel- The first child process is alphabetcount, which counts the quantity of each alphabet letter using their ASCII symbols in all files in the data folder. The second child process is specialcount, which counts the quantity of five case insensitive special words (he, she, they, him, and me) by removing all punctuation and capitalization to catch edge cases, then use strcmp to compare each word in the data file to the special words, storing the results in an array.
In a high level summary, a piece of parent code controls two asynchronous child processes using fork. We run the processes in parallel, which in order to do so, the parent must wait for the child processes to complete, then output the counted results. 
In order to run these processes in parallel, we must use fork twice in a nested if statement to differentiate between parent and children processes. I created two pid_t variables in order to store the pid of each child process. We then compare each pid_t to zero, which indicates a child process, and if true, we call specialcount or alphabetcount accordingly. You call fork the first time, and it either has a pid of zero, which indicates its a child, or a pid greater than zero, which means the process is a parent. If the processs is parent, we call fork again. We repeat the same steps, comparing the pid to zero again to see if we're still the parent or the new second child. If at this point we're still the parent, we must wait for each process to complete then print out the results. Each child process works by creating the path to write the file results, then calls the respective count function.
I included the following libraries: unistd(used for fork and getpid), count.h(my own file), sys/wait.h (used for wait), and sys/types.h(used for pid_t)

All other files in the one directory are imported directly from program zero.
