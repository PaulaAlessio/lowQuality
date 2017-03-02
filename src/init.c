#include "init.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

void printHelpDialog()
{
   const char dialog[] =
     "Usage: lowQuality -i [INPUT_FILE.fq] -l [READ_LENGTH] -t [NUMBER_OF_TILES] -o [OUTPUT_FILE] \n"
     "Reads in a fq file (gz, bz2, z formats also accepted) and performs a set of \n"
     "quality tests. \n"
     "Output in binary format.\n"
      " -h prints help dialog.\n"
      " -i Input file [*fq|*fq.gz|*fq.bz2]. Required option.\n"
      " -l Read length. Length of the reads. Required option.\n"
      " -t Number of tiles. Default 96. \n"
      " -q Minimum quality allowed. Optional option. Default 27 .\n"
      " -o Output file. required option.\n"
      " -f If present, the file will be treated as a filtered file (output from filter_trim).\n";
   fprintf(stderr, "%s", dialog );
   exit(EXIT_SUCCESS);
}


void get_arg(int argc,  char **argv, 
      char **inputfile, int *read_len, 
      int *ntiles, int* minQ, char **outputfile, 
      int *ptrf ){
   
   char tmp;
   if (argc != 11 && argc != 9 && argc != 7  && argc != 3 && 
       argc != 12 && argc != 10 && argc != 8  && argc != 4){  
      printHelpDialog();
   }
   while ( (tmp = getopt(argc,argv,"hi:l:t:q:o:f")) != -1){
      switch(tmp){
         case 'h': //show the HelpDialog
           printHelpDialog();
           break;
         case 'i':
           *inputfile = optarg;
           break; 
         case 'l':
           *read_len = atoi(optarg);
           break;
         case 't':
           *ntiles = atoi(optarg);
           break;
         case 'q':
           *minQ = atoi(optarg);
           break;
         case 'f': 
           *ptrf = 1;  
           break; 
         case 'o':
           *outputfile = optarg;
           break; 
         default:
           fprintf(stderr, "%s: option `-%c' is invalid: ignored\n",
                                 argv[0], optopt);
           printHelpDialog();
           break;
      }         
              
   }
   // Checking the required options
   if(*read_len == 0){
      fprintf(stderr,"read length was not properly initialized. \n");
      printHelpDialog();
   }
   if (*inputfile==NULL){
      fprintf(stderr,"Input file was not properly initialized. \n");
      printHelpDialog();
   }
   if (*inputfile==NULL){
      fprintf(stderr,"Output file was not properly initialized. \n");
      printHelpDialog();
   }
}

