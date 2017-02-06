// utils.h  
/* Contains useful functions 
 * */
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sequence.h"
#define ZEROQ 33
#define NQ 41
#define N_ACGT 5
#define FIRSTTILE 1101


// Struct that stores output
typedef struct statsinfo{
   // Number of low Quality A,C,G,T,N per tile
   long int* lowQ_ACGT_tile; // DIM: N_ACGT * ntiles
   // Number of A,C,G,T,N per tile.  
   // To  the fraction of lowQuality bases per tile and per nucleotide.
   long int* ACGT_tile;      // DIM: N_ACGT * ntiles 
   // Number of reads with M (position) lowQuality bases.
   long int* reads_MlowQ;    // DIM: read_len + 1  
   // Number of bases of a given quality per tile.
   long int* QPosTile_table; // DIM: ntiles * NQ * read_len
   int read_len, ntiles, cur_tile, old_tile, first_tile, minQ; 
} Info;



void init_info(Info* res, const int read_len, const int ntiles, 
     int minQ);
void free_info(Info* res);
void read_info(Info* res, char* file);
void write_info(Info* res, char* file);
void print_info(Info* res);


void get_read(Read* read, char* buffer,int c1, int c2, int k);

void get_first_tile(Info* res, Read* read);

void update_info(Info* res, Read* read);

void update_ACGT_counts(long int* ACGT_low,  char ACGT);
void update_Qpos_table(long int* Qpos_table, char* line4, const int Ncol);
void update_Qtile_table(long int* Qtile_table, char* line4, const int read_len,int tile_cc);



#endif
