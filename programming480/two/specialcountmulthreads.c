//Caroline Munson
//RED ID: 822774843
//cssc0936@edoras.sdsu.edu

/*
 * specialcountmulthreads.c - this file implements the alphabetcountmulthreads function.
 */


/**
  The specialcountmulthreads function counts the frequency of each special word (he, she, they, him and me  (case insensitive)) in all the .txt files under
  directory of the given path and write the results to a file named as filetowrite. Different with programming assignment#0, you need to implement the program using mutithreading.
  
  Input: 
      path - a pointer to a char string [a character array] specifying the path of the directory; and
      filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
      specialfreq - a pointer to a long array storing the frequency of each special word "he","she","they","him", "me", which should be already up-to-date;
	      specialfreq[0] - > # of "he" counted
	      specialfreq[1] - > # of "she" counted
		  specialfreq[2] - > # of "they" counted
		  specialfreq[3] - > # of "him" counted
		  specialfreq[4] - > # of "me" counted
      num_threads - number of the threads running in parallel to process the files
      
       
  Output: a new file named as filetowrite with the frequency of each alphabet letter written in
  
  Requirements:
1)	Multiple threads are expected to run in parallel to share the workload, i.e. suppose 3 threads to process 30 files, then each thread should process 10 files;
2)	When a thread is created, a message should be print out showing which files this thread will process, for example:
Thread id = 274237184 starts processing files with index from 0 to 10!
3)	When a file is being processed, a message should be print out showing which thread (thread_id = xxx) is processing this file, for example:
Thread id = 265844480 is processing file input_11.txt
4)	When a thread is done with its workload, a message should be print out showing which files this thread has done with work, for example:
      Thread id = 274237184 is done !
5)	The array: long specialfreq[ ] should always be up-to-date, i.e. it always has the result of all the threads counted so far.  [You may need to use mutexes to protect this critical region.]


You should have the screen printing should be similar as follows:

 Thread id = 274237184 starts processing files with index from 0 to 10!
 Thread id = 265844480 starts processing files with index from 11 to 22!
 Thread id = 257451776 starts processing files with index from 23 to 31!

 Thread id = 265844480 is processing file input_11.txt
 Thread id = 257451776 is processing file input_22.txt
 Thread id = 274237184 is processing file input_00.txt
  � � 

 Thread id = 274237184 is done !
 Thread id = 265844480 is done !
 Thread id = 257451776 is done !

 The results are counted as follows:
 he -> 250932
 she -> 181764
 ... ...
 �  � 
*/

#include <pthread.h>
#include <stdio.h> 
#include <stdlib.h>
#include "count.h"
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <ctype.h>
#include <time.h>

char* strdup(const char*); //needed since implicit declaration if not included

char totalPaths[35][100]; //global variable to make it easier for threads to access specific paths

pthread_mutex_t mutex; //ensures two threads don't access the same memory at once

// removes punctuation from the string
char* removePunc (char* inputStr){
  // printf("before edit: %s\n", inputStr);

    //Checks each end of the word for punctuation and spaces then removes
    int last_index = strlen(inputStr) - 1;
    while(!(isalpha(inputStr[last_index])) && strlen(inputStr) > 0){
      inputStr[last_index] = '\0';
      last_index--;
    }

    //Checks front of the word and shifts all characters to left if needed
    while(!(isalpha(inputStr[0])) && strlen(inputStr) > 0){
      memmove(inputStr, inputStr+1, strlen(inputStr+1) + 1);
    }
    
    // printf("after edit: %s\n", inputStr);
    return inputStr;
}

//Changed from strcmp to strcasecmp to save time
//Parameter is long pointer to directly change specialfreq
//Locking and unlocking mutex to prevent other parallel threads from accessing the same memory
void count(char* string, long* ptr){
    if (!strcasecmp(string, "he")) {
        pthread_mutex_lock(&mutex);
        ptr[0]++;
        pthread_mutex_unlock(&mutex);
    }
    else if(!strcasecmp(string, "she")) {
        pthread_mutex_lock(&mutex);
        ptr[1]++;
        pthread_mutex_unlock(&mutex);
    }
    else if(!strcasecmp(string, "they")){ 
        pthread_mutex_lock(&mutex);
        ptr[2]++;
        pthread_mutex_unlock(&mutex);
    }
    else if(!strcasecmp(string, "him")){
        pthread_mutex_lock(&mutex);
        ptr[3]++;
        pthread_mutex_unlock(&mutex);
    }
    else if(!strcasecmp(string, "me")){
        pthread_mutex_lock(&mutex);
        ptr[4]++;
        pthread_mutex_unlock(&mutex);
    }
}

//Saves all paths into global variable 
void collectingFilePaths(char *path, char* fileName, int index) {
  if (path == NULL){
      return;
  }
  char *path2 = strdup(path); //modify copy of string
  path2 = strcat(path2, "/"); //concatenating the file path with the file name
  path2 = strcat(path2, fileName);

  strcpy(totalPaths[index], path2);
}

void* threading(void* args){
    struct container* st = (struct container*) args; //First create a struct pointer that points to related struct for the thread
    
    printf("Thread id = %lu is starting to process files %d to %d\n", pthread_self(), st->first, st->last);

    //Starting at first file index and going to last file index, parse through opening files
    for(int i = st->first; i <= st->last; i++){ 

      FILE* fp = fopen(totalPaths[i], "r");

      if (fp == NULL){
        perror("No file found");
      }
      
      printf("Thread id = %lu processing file input%02d.txt\n", pthread_self(), i);
      char wordByWord[1000] = ""; //arbitrarily large array size to catch any length of line

      //Reading in data word by word 
      while (fscanf(fp, "%999s", wordByWord)==1) {
        char* outputStr = removePunc(wordByWord);
        count(outputStr, st->specfreqptr);

      }
      fclose(fp); 
      memset(wordByWord, 0, 1000);
    }

    printf("Thread id = %lu is finished with all its files!\n", pthread_self());
  }

void specialcountmulthreads(char *path, char *filetowrite, long specialfreq[], int num_threads)
{

  int pathIndex = 0;

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
      collectingFilePaths(path, in_file->d_name, pathIndex);
      pathIndex++;
  }
  closedir(directory);


  //Create an array of containers to hold the first and last index of each range of files for each thread
  struct container arrayOfContainers[num_threads];

  pthread_t thread[num_threads]; //Creating multiple threads
  int index = 0;
  int increment = (31/num_threads) - 1; //Number of files divided by number of threads (- 1 to account for first file)

  pthread_mutex_init(&mutex, NULL);

  for(int i = 0; i < num_threads; i++){
    arrayOfContainers[i].first = index;
    
    if(i == num_threads - 1){//creating last thread
      arrayOfContainers[i].last = 30; 
    }
    else {
      index += increment;
      arrayOfContainers[i].last = index;
      index++; //ensuring that no files are read twice
    }
    arrayOfContainers[i].specfreqptr = specialfreq; //Allows thread to change specialfreq[] in real time
    pthread_create(&thread[i], NULL, threading, &arrayOfContainers[i]);
  }

  //Ensures the main thread has to wait for all other threads to finish before continuing
  for(int i = 0; i < num_threads; i++){
      pthread_join(thread[i], NULL);
  }

  pthread_mutex_destroy(&mutex);


}