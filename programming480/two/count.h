/* charcount.h - This header file include type definitions (including function prototypes) and macros 
   used for the programing assignment two.
*/

#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <ctype.h>
#include <stdlib.h>


#define SPECIALSIZE 5     //The total number of special word "he","she","they","him" and "me" (case insensitive)

struct container{
    long* specfreqptr;
    int first;
    int last;
};

char* removePunc (char* inputStr);

void count(char* string, long* ptr);

void collectingFilePaths(char *path, char* fileName, int index);

void* threading(void* args);

void specialcountmulthreads(char *path, char *filetowrite, long specialfreq[], int num_threads);