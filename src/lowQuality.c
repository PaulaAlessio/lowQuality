#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "init.h"
#include "fopen_gen.h"
#include "sequence.h"
#include "stats_info.h"

#define B_LEN 131072
#define NTILES 96 // default value 
#define MINQ 27   // default value

int main(int argc, char *argv[]){
   FILE* f;
   int j = 0, k = 0, c1 = 0, c2 = -1;  
   char buffer[B_LEN + 1];
   int newlen;
   int offset = 0; 
   Info* res= malloc(sizeof *res); 
   Sequence* seq = malloc(sizeof *seq);
   char  *inputfile = NULL, *outputfile=NULL;
   int read_len = 0; 
   int ntiles = NTILES; 
   int minQ = MINQ ; 
   clock_t start, end;
   double cpu_time_used;
   time_t rawtime;
   struct tm * timeinfo;

   // Start the clock 
   start = clock();
   time ( &rawtime );
   timeinfo = localtime ( &rawtime );
  
   // Get arguments  
   get_arg(argc, argv, &inputfile, &read_len, 
           &ntiles, &minQ, &outputfile);
   fprintf(stderr ,"Starting program at: %s", asctime (timeinfo) );
   fprintf(stderr, "- Input file: %s\n", inputfile);
   fprintf(stderr, "- Read length: %d\n", read_len);
   fprintf(stderr, "- Number of tiles: %d\n", ntiles);
   fprintf(stderr, "- Min quality: %d\n", minQ);
   fprintf(stderr, "- Output file: %s\n", outputfile);
    
   // Opening file
   f = fopen_gen(inputfile,"r");
   
   // Initialize struct that will contain the output
   init_info(res, read_len, ntiles, minQ,NQ);

   // Read the fastq file
   while ( (newlen = fread(buffer+offset,1,B_LEN-offset,f) ) > 0 ){

      newlen += offset+1;
       
      buffer[newlen] =  '\0';
      for( j =0 ; buffer[j] != '\0' ; j++){
         c1 = c2 + 1;
         if (buffer[j]== '\n'){
           c2 = j;
           get_sequence(seq,buffer,c1,c2, k);
           if( (k % 4) == 3 ){
              
              if (res -> nreads == 0) get_first_tile(res,seq);
              update_info(res,seq);
              if (res -> nreads % 1000000 == 0) 
                 fprintf(stderr, "  %10d reads have been read.\n",res -> nreads);
           } 
           k++;
         }
     } 
     offset = newlen - c1 -1;
     if(offset >  -1)
       memcpy(buffer,buffer+c1,offset);
     c2 = -1;
     c1 = 0;
   } // end while 

   // Closing file
   fprintf(stderr, "- Finished reading file.\n");
   fclose(f);
   
   // resize Info
   resize_info(res);
   
   // Open file and write to disk (binary)
   fprintf(stderr, "- Writing data structure to file %s.\n",outputfile);
   write_info(res,outputfile);

   // Print to the standard output
   print_info(res);

   // Free memory
   free_info(res);
   
   
   // Read struct from disk (binary)
//   res  = malloc(sizeof *res);
//   fprintf(stderr, "- Read data structure from file %s.\n",outputfile);
//   read_info(res,outputfile);
//   printf("\nPRINTING res from file: \n");
//   print_info(res);
//

   // Obtaining elapsed time
   end = clock();
   cpu_time_used = (double)(end - start)/CLOCKS_PER_SEC;
   time ( &rawtime );
   timeinfo = localtime ( &rawtime );
   fprintf(stderr ,"Finishing program at: %s", asctime (timeinfo) );
   fprintf(stderr, "Time elapsed: %f s.\n",cpu_time_used);
   return 1;
}
