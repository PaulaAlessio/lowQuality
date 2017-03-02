#include "sequence.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void get_sequence(Sequence* seq, char* buffer,int c1, int c2, int k){
      switch ( k%4 ){
       case 0: 
          memcpy(seq -> line1 ,buffer+c1,c2-c1); 
          seq -> line1[c2-c1]='\0';
          break; 
       case 1: 
          memcpy(seq -> line2 ,buffer+c1,c2-c1);
          seq -> L = c2-c1;
          seq -> line2[c2-c1]='\0';
          break; 
       case 2: 
          memcpy(seq -> line3 ,buffer+c1,c2-c1); 
          seq -> line3[c2-c1]='\0';
          seq -> start = 0;
          break; 
       case 3: 
          memcpy(seq -> line4 ,buffer+c1,c2-c1); 
          seq -> line4[c2-c1]='\0';
          break; 
    }
   
}
void get_sequence_filt(Sequence* seq, char* buffer,int c1, int c2, int k){
      switch ( k%4 ){
       case 0: 
          memcpy(seq -> line1 ,buffer+c1,c2-c1); 
          seq -> line1[c2-c1]='\0';
          break; 
       case 1: 
          memcpy(seq -> line2 ,buffer+c1,c2-c1);
          seq -> L = c2-c1;
          seq -> line2[c2-c1]='\0';
          break; 
       case 2: 
          memcpy(seq -> line3 ,buffer+c1,c2-c1); 
          seq -> line3[c2-c1]='\0';
          seq -> start = 0;
          char *trim = "TRIM";
          int i; 
          if ( (i = strindex(seq-> line3,trim)) > 0 ){
            i +=6;
            int i1 = i; 
            char tmp[20];
            while(seq -> line3[i] != '\0'){
              if( seq -> line3[i] ==  ':'  ){
                memcpy(tmp,seq->line3+i1,i-i1);
                tmp[i-i1] = '\0';
                seq -> start = atoi(tmp);
                break;
              }
              i++; 
            }
          }
          break; 
       case 3: 
          memcpy(seq -> line4 ,buffer+c1,c2-c1); 
          seq -> line4[c2-c1]='\0';
          break; 
    }
   
}

// Returns index of t in s
int strindex(char *s, char *t){
        int i, j, k;
         for (i = 0; s[i] != '\0'; i++) {
             for (j = i, k=0; t[k]!='\0' && s[j]==t[k]; j++, k++)
                 ;
              if (k > 0 && t[k] == '\0')
                  return i;
               }
          return -1;
}

