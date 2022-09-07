//Caroline Munson
//REDID: 822774843
//Edoras: cssc0936@edoras.sdsu.edu

/*
 *  Implementation of FIFO and LRU page replacement algorithm
 *  Please add appropriate level of comments in this file 
 */

#include "vmalgorithm.h"


/* Generate an access pattern
 * Example: 3, 2, 2, 1, 1  indicates the page 3, 2, 2, 1, 1 in order
 */
void generateAccessPattern()
{
   int i;
   srand(time(0));
   accessPattern = (int *)malloc( sizeof(int) * AccessPatternLength);   
   printf("The randomized Access Pattern: ");
   for(i=0; i< AccessPatternLength; i++)
   {
		     accessPattern[i] = rand() % ReferenceSZ;
        printf("%d ", accessPattern[i]);       
   }
   printf("\n");
}

/*
 * Initialize the parameters of simulated memory
 */
void initializePageFrame()
{
   int i;
   memory.PageFrameList = (int *)malloc( sizeof(int)* FrameNR );    // dynamic allocated FrameNR frames to be used in memory
   memory.nextToReplaced =0;          // indicate the new frame to be replaced as 0
   for(i=0; i< FrameNR; i++)
   {
			memory.PageFrameList[i] = -1;  // initialization to -1 indicating all frames are unused 
   }

}

// Print the pages loaded in memory
void printPageFrame()
{
   int i;
   for(i=0; i< FrameNR; i++)
   {
			printf("%2d ",memory.PageFrameList[i]);       
   }
   printf("\n");
}


/*
 *  Print the access pattern in order
 */
void printAccessPattern()
{
   int i;
   printf("The Same Access Pattern: ");
   for(i=0; i< AccessPatternLength; i++)
   {
        printf("%d ", accessPattern[i]);       
   }
   printf("\n");

}


/*
 * Return: number of the page fault using FIFO page replacement algorithm
 */
int FIFO()
{
   int numPageFaults = 0; 
   
   //Loop through page accesses
   int i;
   for (i = 0; i < AccessPatternLength; i++){
      int detectPageFault = 1; //default set to 1 if page fault occurs, 0 if otherwise detected in for loop

      //Check if page already exists in memory
      int j;
      for (j = 0; j < FrameNR; j++){
         if (memory.PageFrameList[j] == accessPattern[i]){ //if the value is already present in the page frame 
            detectPageFault = 0; //page fault did not occur
            break;
         }
      }

      //Page fault occurred
      if (detectPageFault == 1) {
         numPageFaults++;
         memory.PageFrameList[memory.nextToReplaced] = accessPattern[i]; //replacing the old page with new one   
         memory.nextToReplaced = (memory.nextToReplaced+1)%FrameNR; //incrementing next to be replaced
      }
      printPageFrame();
   
   }
   
   return numPageFaults;
}



/*
 * Return: number of the page fault using LRU page replacement algorithm
 */

int LRU()
{
   int numPageFaults = 0;

   int accessTimeArray[FrameNR]; //Creating a secondary array of access times for each page frame
   
   //Initializing the secondary accessTimeArray with -1
   int i = 0;
   for(i = 0; i < FrameNR; i++){
      accessTimeArray[i]= -1;
   }

   //Looping through page accesses
   int j;
   for (j = 0; j < AccessPatternLength; j++){

      int detectPageFault = 1; 

      //Check if page already exists in memory
      int k;
         for (k = 0; k < FrameNR; k++){
            if (memory.PageFrameList[k] == accessPattern[j]){ //value is already present in page frame
               detectPageFault = 0;
               accessTimeArray[k] = j; //update the access time to the current iteration number
               break;
            }
         }

         //Page fault occurred
         if (detectPageFault == 1) {
            numPageFaults++;

            //finding the element in the accessTime array with the least current access time
            int lowestValueIndex = 0; 
            int g;
            for (g = 0; g < FrameNR; g++){
               if (accessTimeArray[g] < accessTimeArray[lowestValueIndex]){
                  lowestValueIndex = g;
               }
            }

            memory.nextToReplaced = lowestValueIndex; //setting next to be replaced to the item that was accessed last
            memory.PageFrameList[memory.nextToReplaced] = accessPattern[j]; //replacing old page value with new
            accessTimeArray[memory.nextToReplaced] = j; //updating the access time to the current iteration number
         }
         printPageFrame();
      
   }
   return numPageFaults;
   





}

