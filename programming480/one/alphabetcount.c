/*
 * alphabetcount.c - this file implements the alphabetlettercount function.
 */

#include <stdio.h>
#include "count.h"
#include <dirent.h>
#include <string.h>

/**
  The alphabetlettercount function counts the frequency of each alphabet letter (A-Z a-z, case sensitive) in all the .txt files under
  directory of the given path and write the results to a file named as filetowrite.
  
  Input: 
      path - a pointer to a char string [a character array] specifying the path of the directory; and
      filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
      alphabetfreq - a pointer to a long array storing the frequency of each alphabet letter from A - Z a - z:
      alphabetfreq[0]: the frequency of 'A'
      alphabetfreq[1]: the frequency of 'B'
         ... ...
      alphabetfreq[25]:the frequency of 'Z'
      alphabetfreq[26]:the frequency of 'a'
         ...  ...
      alphabetfreq[51]:the frequency of 'z'
  
  Output: a new file named as filetowrite with the frequency of each alphabet letter written in
  
  Steps recommended to finish the function:
  1) Find all the files ending with .txt and store in filelist.
  2) Read all files in the filelist one by one and count the frequency of each alphabet letter only (A-Z a - z). The array 
  long alphabetfreq[] always has the up-to-date frequencies of alphabet letters counted so far. 
  3) Write the result in the output file: filetowrite in following format: 
  
     letter -> frequency
     
     example:
     A -> 200
     B -> 101
     ... ...
     
  Assumption:  
  1) You can assume there is no sub-directory under the given path so you don't have to search the files 
  recursively.    
  2) Only .txt files are counted and other types of files should be ignored.
  
*/
void alphacount(char currChar, long alphabetfreq[]){
    //Counting the lowercase characters
    if ((int)currChar >= 65 && (int)currChar<=90) {
        int ascii = (int)currChar - 65;
        alphabetfreq[ascii]++;
    }
    //Counting the uppercase characters
    if ((int)currChar >= 97 && (int)currChar <= 122) {
        int ascii2 = (int)currChar - 97 + 26; //+26 to account for the offset
        alphabetfreq[ascii2]++;
    }


}
void readFile(char *path, char* fileName, long alphabetfreq[]) {
    if (path == NULL){
        return;
    }
    char *path2 = strdup(path); //modify copy of string
    path2 = strcat(path2, "/"); //concatenating the file path with the file name
    path2 = strcat(path2, fileName);

    FILE* fp = fopen(path2, "r");
    char testChar;

    if (fp == NULL){
        return;
    }

    while (1) {
        testChar = fgetc(fp); //read file character by character
        if (testChar == EOF) break;
        alphacount(testChar, alphabetfreq);
    }
    fclose(fp);

}

void findFiles(char* path, long alphabetfreq[]) {
    //char* filenames[sizeof(char*) * 1000];
    struct dirent* in_file; //pointer to object that holds file names
    DIR* directory = opendir(path); //open directory

    if (directory == NULL) {
        printf("File not found");
        return;
    }

    while ((in_file = readdir(directory)) != NULL) { //loops through reading each file
        int lengthFileName = strlen(in_file->d_name); //for simplicity

        //checking for .txt file extension
        if (lengthFileName<=4){
            continue;
        }
        if('t' != in_file->d_name[lengthFileName-1]){
            continue;
        }
        if( 'x' != in_file->d_name[lengthFileName-2]) {
            continue;
        }

        if('t' != in_file->d_name[lengthFileName-3]){
            continue;
        }
        if('.'!= in_file->d_name[lengthFileName-4]) {
            continue;
        }
            readFile(path, in_file->d_name, alphabetfreq);
    }
    closedir(directory);

}

void outputalphabetfreq(long charfreq[], char* filetowrite)
{
    FILE* fp = fopen(filetowrite, "w");
    if (fp == NULL) {
        return;
    }
    for(int i = 0; i < ALPHABETSIZE/2; i++)   // print upper case letter frequency
    {
        fprintf(fp, "%c -> %d\n", (char)(i+65), charfreq[i]);
    }

    for(int i = ALPHABETSIZE/2; i < ALPHABETSIZE; i++)  // print lower case letter frequency
    {
        fprintf(fp, "%c -> %d\n", (char)(i+65 + 6), charfreq[i]);
    }
}
void alphabetlettercount(char *path, char *filetowrite, long alphabetfreq[])
{

    findFiles(path, alphabetfreq);
    outputalphabetfreq(alphabetfreq, filetowrite);
}



