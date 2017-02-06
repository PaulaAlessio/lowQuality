#include "sequence.h"

void get_read(Read* read, char* buffer,int c1, int c2, int k){
      switch ( k%4 ){
       case 0: 
          memcpy(read -> line1 ,buffer+c1,c2-c1); 
          read -> line1[c2-c1]='\0';
          break; 
       case 1: 
          memcpy(read -> line2 ,buffer+c1,c2-c1);
          read -> line2[c2-c1]='\0';
          break; 
       case 2: 
          memcpy(read -> line3 ,buffer+c1,c2-c1); 
          read -> line3[c2-c1]='\0';
          break; 
       case 3: 
          memcpy(read -> line4 ,buffer+c1,c2-c1); 
          read -> line4[c2-c1]='\0';
          break; 
    }
   

}
