// init.h 
/* Functions
 * that read the arguments passed to the program. */

#ifndef init_H
#define init_H
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

void prontHelpDialog();
void get_arg(int argc,  char **argv, 
      char **inputfile, int *read_len, 
      int *ntiles, int* minQ, char **outputfile );
#endif
