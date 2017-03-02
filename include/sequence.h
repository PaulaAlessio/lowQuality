// sequence.h 
#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <string.h>

#define L_LEN 200

typedef struct sequence{
   char line1[L_LEN], line2[L_LEN], line3[L_LEN], line4[L_LEN];
   int L;
   int start;
} Sequence;

void get_sequence(Sequence* seq, char* buffer,int c1, int c2, int k);

void get_sequence_filt(Sequence* seq, char* buffer,int c1, int c2, int k);

int strindex(char *s, char *t);

#endif
