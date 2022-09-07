#include <unistd.h>
#include "count.h"
#include <sys/wait.h>
#include <sys/types.h>

/*
 This program will fork two child processes running the two programs generated in programming zero in parallel.
 Hint: You mant need to use fork(), exec() family, wait(), exit(), getpid() and etc ...
 
 Requirements:
 
 1) Exactly two child processes are created, one to run testspecial program and the other is to run testalphabet program;
 2) When a program starts to run, print a message to the output screen showing which process (with PID) is running which program, for example:
    "CHILD <16741> process is executing testalphabet program!"
    
 3) When a program is done, the process exits and at the same time, a message should be print to the output screen showing which  process (with PID) is done with the program, for example:
    "CHILD <16741> process has done with testalphabet program !"
    
 4) The messages should match the real execution orders, i.e. when the testspecial program starts/ends, the right message should be print out. So you need to figure out how to get the starting/ending time of each 
 process.
   
   
 The expected screen print out should be similar as follows:
 
 CHILD <16741> process is executing testalphabet program!
 CHILD <16742> process is executing testspecial program!
he -> 250932
she -> 181764
... ...

CHILD <16742> process has done with testspecial program !
a -> 2861232
b -> 494472
... ...

CHILD <16741> process has done with testalphabet program !
 */


void openReadFiles(char* filename) {
    //open file and check it opens successfully
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }

    //Read and print file character by character until reaching end of file
    char testChar;
    while (1) {
        testChar = fgetc(fp);
        if (testChar == EOF) break;
        else{
            printf("%c", testChar);
        }
    }
}

int main(void) {

    pid_t child_special, child_alphabet;

    //Fork child process then run special count
    child_special = fork();
    if (child_special == 0) { //received no pid from fork- this indicates child
        printf("CHILD <PID: %d> process is executing testspecial program!\n", getpid());
        char *path = "../data";
        char *filetowrite = "../result/specialresult.txt";
        long charfreq[SPECIALSIZE] = {0}; // array to store the frequency of 5 special characters
        specialcount(path, filetowrite, charfreq); // count the frequency of special characters and write into file
    }

    //If you make it to this else statement you received a pid, indicating parent.
    else {
        //The parent will then fork again and if it's the child, we call alphabetcount
        child_alphabet = fork();
        if (child_alphabet == 0) {
            printf("CHILD <PID: %d> process is executing testalphabet program!\n", getpid());
            //Function call from testalphabetcount.c
            char *alphapath = "../data";           // the data *.txt files are under this folder
            char *alphafiletowrite = "../result/result.txt";    // the frequency of all alphabetical letters will be written in this file
            long alphabetfreq[ALPHABETSIZE] = {0}; // array to store the frequency of each alphablet letter

            alphabetlettercount(alphapath, alphafiletowrite, alphabetfreq);  // process the data files
        }

        //Still the parent- The parent will open and read the output data from the children as the child processes complete
        else{
            //Running the first process message
            pid_t finishChildProcess = wait(NULL); //wait returns the pid of the child process
            if (finishChildProcess == child_special){
                openReadFiles("../result/specialresult.txt");
                printf("CHILD <PID: %d> process is done executing testspecial program! See results above!\n", child_special);
            }
            else{
                openReadFiles("../result/result.txt");
                printf("CHILD <PID: %d> process is done executing testalphabet program! See results above!\n", child_alphabet);
            }

            //Second process message
            finishChildProcess = wait(NULL); //wait returns the pid of the child process
            if (finishChildProcess == child_special){
                openReadFiles("../result/specialresult.txt");
                printf("CHILD <PID: %d> process is done executing testspecial program! See results above!\n", child_special);
            }
            else{
                openReadFiles("../result/result.txt");
                printf("CHILD <PID: %d> process is done executing testalphabet program! See results above!\n", child_alphabet);
            }
        }
    }
	return 0;
	
}
