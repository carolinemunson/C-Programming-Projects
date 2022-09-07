/*
 * specialcount.c - this file implements the specialcount function.
 */


#include <stdio.h> 
#include "count.h"
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char* removePunc (char* inputStr){
    int index = 0;
    char* outputStr = (char *) malloc(sizeof(char) * strlen(inputStr));
    for (int i = 0; i < strlen(inputStr); i++) {
        if (!ispunct((unsigned char) inputStr[i])) {
            outputStr[index] = inputStr[i];
            index++;
        }
    }
    outputStr[index] = '\0'; //null terminator lets us know when the string is done
    return outputStr;
}

void count(char* string, long alphabetfreq[]){
    if (!strcmp(string, "he")) {
        alphabetfreq[0]++;
    }
    else if(!strcmp(string, "she")) {
        alphabetfreq[1]++;
    }
    else if(!strcmp(string, "they")){
        alphabetfreq[2]++;
    }
    else if(!strcmp(string, "him")){
        alphabetfreq[3]++;
    }
    else if(!strcmp(string, "me")){
        alphabetfreq[4]++;
    }
}
void openAndReadFile(char *path, char* fileName, long alphabetfreq[]) {
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

    char wordByWord[1000]; //arbitrarily large array size to catch any length word
    while (fscanf(fp, "%999s", wordByWord)==1) {
        for (int i = 0; i < strlen(wordByWord); i++) {
            wordByWord[i] = tolower(wordByWord[i]);
        }
        char* strCopy =strdup(wordByWord);
        char* outputStr= removePunc(strCopy);
        count(outputStr, alphabetfreq);

    }
    fclose(fp);

}

void findFilesInDirectory(char* path, long alphabetfreq[]) {
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
        openAndReadFile(path, in_file->d_name, alphabetfreq);
    }
    closedir(directory);

}

void outputspecialfreq(long charfreq[], char *filetowrite)
{
    FILE* fp = fopen(filetowrite, "w");
    if (fp == NULL) {
        return;
    }
    for(int i = 0; i < SPECIALSIZE; i++)
    {
        switch(i)
        {
            case 0:
                fprintf(fp, "he -> %d\n", charfreq[i]);
                break;
            case 1:
                fprintf(fp, "she -> %d\n", charfreq[i]);
                break;
            case 2:
                fprintf(fp, "they -> %d\n", charfreq[i]);
                break;
            case 3:
                fprintf(fp, "him -> %d\n", charfreq[i]);
                break;
            case 4:
                fprintf(fp, "me -> %d\n", charfreq[i]);
                break;
        }
    }
}

void specialcount(char *path, char *filetowrite, long charfreq[])
{

    findFilesInDirectory(path, charfreq);
    outputspecialfreq(charfreq, filetowrite);
		
}
/**
  The specialcharcount function counts the frequency of the following 5 special words:
  "he"   "she"   "they"    "him"    "me"
   
  in all the .txt files under directory of the given path and write the results to a file named as filetowrite.
  
  Input: 
  
      path - a pointer to a char string [a character array] specifying the path of the directory; and
      filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
      charfreq - a pointer to a long array storing the frequency of the above 5 special words
      
      charfreq[0]: the frequency of "he"
      charfreq[1]: the frequency of "she"
      charfreq[2]: the frequency of "they"
      charfreq[3]: the frequency of "him"
      charfreq[4]: the frequency of "me"

  
  Output: a new file named as filetowrite with the frequency of the above 5 special words written in
  
  Steps recommended to finish the function:
  1) Find all the files ending with .txt and store in filelist.
  2) Read all files in the filelist one by one and count the frequency of each of the above special words. The array 
  long charfreq[] always has the up-to-date frequencies of special words counted so far. If the word is upper case, convert
  it to lower case first. 
  3) Write the result in the output file: filetowrite in following format: 
  
     character -> frequency
     
     example:
     he -> 20
     she -> 11
     they -> 20
     him -> 11
     me -> 12     
     
  Assumption:  
  1) You can assume there is no sub-directory under the given path so you don't have to search the files 
  recursively.    
  2) Only .txt files are counted and other types of files should be ignored.
  
*/

