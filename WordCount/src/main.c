/*
 * main.c
 * Author: Armando Ferrara
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>
#include <fileparser.h>
#include <workerhelper.h>

int main (int argc, char *argv[])
{
    int my_rank ;
    int num_proc ; /* total number of processes */
    int total_number_of_byte; /* the total number of byte for the file */
    long chunk_size; /* Usual byte chunk size for file : total_number_of_byte/num_proc */
    short bonus; /* How many larger chunks, total_number_of_byte % num_proc */
    long start; /* Start for one of the chunk*/
    struct node * myPortion; /* my portion file to read */
    FILE *fp; /* reference to the file */
      my_rank = 0;
      num_proc = 1;
      start = 0;
    /* Code for process 0 (master)*/
    if(my_rank == 0)
    {

       // get the total number of bytes by the sum of all the byte size from files
       total_number_of_byte = getTotalFileSizes();
       // make a division for the number of process
       chunk_size =  total_number_of_byte / num_proc;
       // calculate the number of bonus byte that are not counted in chunk_size
       bonus = total_number_of_byte % num_proc; // i.e. remainder

       // from i=1 because i don't want to compute the work for the master
       for (short i = 1, start = chunk_size ; start < total_number_of_byte && i < num_proc ; i++)
       {
    	   if(bonus > 0)
    	   {
    		   int tmpChunkIncreased = chunk_size + 1;
    		   start = start + tmpChunkIncreased;
    		   bonus --;
    	   }
    	   else
    	   {
    		   start = start + chunk_size;
    	   }
       }

    	   // return a list of file that i need to analyze
    	   myPortion = whichIsMyPortion(start, chunk_size);


    	   struct node * np;
           short i=0;
    	   // if there are other files to read
    	   for (np = myPortion; np != NULL; np = (struct node*)np->next)
    	   {
    		   // open the  files from the list
    		    	   fp = fopen(np->data, "r"); // read mode

    		    	   if (fp == NULL)
    		    	   {
    		    	        perror("Error while opening the file.\n");
    		    	        exit(EXIT_FAILURE);
    		    	   }


    		          // rationale, if the last worker stop to read in the middle of a word, it ends to read the word.
    		    	 // the next worker does not read the same word.

    		    	   buildfrequencieshash(fp);
    	   }


    }//the slaves
    hashmapwordreport();

       	   /* display all the elements */
       	   printf("ok");
}