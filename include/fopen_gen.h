// fopen_gen.h 
/* This functions allow us to open different types of 
 * compressed files: (*.tar.gz, *.gz, *.bz2, *.bam, etc)
 * using the function fopen_gen. It returns a pointer 
 * to FILE that can be then used as usual with fread. */
#ifndef fopen_gen_H
#define fopen_gen_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <fcntl.h> 

#ifdef __STDC__
FILE* fdopen( int, const char* );
#endif

int setCloexec(int fd);
FILE* fopen_gen(const char *path, const  char * mode);

/* Static functions
* static const char* zcatExec(const char* path);
* static int uncompress(const char* path);
* static FILE* funcompress(const char* path);
*/

#endif
