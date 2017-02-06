#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include "fopen_gen.h"
#include "sequence.h"
#include "stats_info.h"

#define B_LEN 131072


void printHelpDialog()
{
   const char dialog[] =
     "Usage: lowQuality -i [INPUT_FILE.fq] -l [READ_LENGTH] -t [NUMBER_OF_TILES] -o [OUTPUT_FILE] \n"
     "Reads in a fq file (gz, bz2, z formats also accepted) and performs a set of \n"
     "quality tests. \n"
     "Output is in binary format, (write the reader!).\n"
      " -h prints help dialog.\n"
      " -i Input file [*fq|*fq.gz|*fq.bz2]. Required option.\n"
      " -l Read length. Length of the reads. Required option.\n"
      " -t Number of tiles. Required option. \n"
      " -q Minimum quality allowed. Optional option. Default = 27 .\n"
      " -o Output file. required option.\n";
   fprintf(stderr, "%s", dialog );
   exit(EXIT_SUCCESS);
}





int main(int argc, char *argv[]){
   FILE* f;
   char  *inputfile = NULL, *outputfile=NULL;
   int j = 0, k = 0, c1 = 0, c2 = -1;  
   int nreads = 0 ; 
   char buffer[B_LEN + 1];
   int newlen;
   int offset = 0; 
   Info* res= malloc(sizeof *res); 
   Read* read = malloc(sizeof *read);
   int minQ = 0 ; 
   int read_len = 0; 
   int ntiles = 0;
   char tmp;
   clock_t start, end;
   double cpu_time_used;
   start = clock();
   time_t rawtime;
   struct tm * timeinfo;

   if (argc != 11 && argc != 9) {
      printHelpDialog();
   }
   while ( (tmp = getopt(argc,argv,"hi:l:t:q:o:")) != -1){
      switch(tmp){
         case 'h': //show the HelpDialog
            printHelpDialog();
            break;
         case 'i':
           inputfile = optarg;
           fprintf(stderr, "- Input file: %s\n", optarg);
           break; 
         case 'l':
           read_len = atoi(optarg);
           fprintf(stderr, "- Read length: %d\n", read_len);
           break;
         case 't':
           ntiles = atoi(optarg);
           fprintf(stderr, "- Number of tiles: %d\n", ntiles);
           break;
         case 'q':
           minQ = atoi(optarg);
           fprintf(stderr, "- Min quality: %d\n", minQ);
           break;
         case 'o':
           outputfile = optarg;
           fprintf(stderr, "- Output file: %s\n", optarg);
           break; 
         default:
            printHelpDialog();
            break;
      }         
            
   } 
   if (minQ == 0 ) {
      minQ = 27;
      fprintf(stderr, "Default min quality: %d\n", minQ);
   }
   time ( &rawtime );
   timeinfo = localtime ( &rawtime );
   fprintf(stderr ,"Starting program at: %s", asctime (timeinfo) );
   // Opening file
   f = fopen_gen(inputfile,"r");
    
   init_info(res, read_len, ntiles, minQ,NQ); 
   while ( (newlen = fread(buffer+offset,1,B_LEN-offset,f) ) > 0 ){

      newlen += offset+1;
       
      buffer[newlen] =  '\0';
      for( j =0 ; buffer[j] != '\0' ; j++){
         c1 = c2 + 1;
         if (buffer[j]== '\n'){
           c2 = j;
           get_read(read,buffer,c1,c2, k);
           if( (k % 4) == 3 ){
              nreads++;
              
              if (nreads == 1) get_first_tile(res,read);
              if (nreads % 1000000 == 0) fprintf(stderr, "  %10d reads have been read.\n",nreads);
              update_info(res,read);
           } 
           k++;
         }
     } 
     offset = newlen - c1 -1;
     if(offset >  -1)
       memcpy(buffer,buffer+c1,offset);
     c2 = -1;
     c1 = 0;
   }
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



}
