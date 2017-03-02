#!/bin/bash

awk -F '' 'BEGIN{ 
      for (i=1; i<=NF; i++){
         A[i] = 0; 
         C[i] = 0; 
         G[i] = 0; 
         T[i] = 0; 
         N[i] = 0;
      } 
   } 
   { 
     for (i=1; i<=NF; i++){ 
        if($i~"A") { A[i]+=1 }; 
        if($i~"C") { C[i]+=1 }; 
        if($i~"G") { G[i]+=1 };
        if($i~"T") { T[i]+=1 };
        if($i~"N") { N[i]+=1 };    
      }
   }
   END{ 
     for(i=1; i<=NF; i++) 
        printf"%d %d %d %d %d\n",A[i],C[i], G[i],T[i],N[i] 
  }' < test_10seq.fa
