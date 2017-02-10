#include "sequence.h"

void get_sequence(Sequence* seq, char* buffer,int c1, int c2, int k){
      switch ( k%4 ){
       case 0: 
          memcpy(seq -> line1 ,buffer+c1,c2-c1); 
          seq -> line1[c2-c1]='\0';
          break; 
       case 1: 
          memcpy(seq -> line2 ,buffer+c1,c2-c1);
          seq -> line2[c2-c1]='\0';
          break; 
       case 2: 
          memcpy(seq -> line3 ,buffer+c1,c2-c1); 
          seq -> line3[c2-c1]='\0';
          break; 
       case 3: 
          memcpy(seq -> line4 ,buffer+c1,c2-c1); 
          seq -> line4[c2-c1]='\0';
          break; 
    }
   
}
