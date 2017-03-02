// init.h 
/* Functions
 * that read the arguments passed to the program. */

#ifndef init_H
#define init_H

void prontHelpDialog();
void get_arg(int argc,  char **argv, 
      char **inputfile, int *read_len, 
      int *ntiles, int* minQ, char **outputfile, 
      int *ptrf );
#endif
