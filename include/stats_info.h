/* Construct the quality report variables and update them 
 * */
#ifndef STATS_INFO_H
#define STATS_INFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sequence.h"
#define ZEROQ 33
#define NQ 46
#define N_ACGT 5


// Struct that stores output
typedef struct statsinfo{
   // GOAL: REMOVE FIRST_TILE, CUR_TILE OLD_TILE
   int read_len, ntiles, nQ, minQ, tile_pos,nreads,reads_wN;
   // Names of the existing tiles
   int *tile_tags; 
   // Names of the existing qualities
   int *qual_tags;
   // Number of low Quality A,C,G,T,N per tile
   long int* lowQ_ACGT_tile; // DIM: ntiles * N_ACGT
   // Number of A,C,G,T,N per tile.  
   // to compute the fraction of lowQuality bases per tile and per nucleotide.
   long int* ACGT_tile;      // DIM:  ntiles * N_ACGT
   // Number of reads with M (position) lowQuality bases.
   long int* reads_MlowQ;    // DIM: read_len + 1  
   // Number of bases of a given quality per tile.
   long int* QPosTile_table; // DIM: ntiles * NQ * read_len
   // Number of A,C,G,T,N per position
   long int* ACGT_pos; // DIM: read_len* N_ACGT  
} Info;

void init_info(Info* res, const int read_len, const int ntiles, 
     const int minQ, const int nQ);
void free_info(Info* res);
void read_info(Info* res, char* file);
void write_info(Info* res, char* file);
void print_info(Info* res, char *infofile);


void get_first_tile(Info* res, Sequence* seq);
void update_info(Info* res, Sequence* seq);
int  update_ACGT_counts(long int* ACGT_low,  char ACGT);
void update_Qpos_table(long int* Qpos_table, Sequence *seq, const int Ncol);
void update_Qtile_table(long int* Qtile_table, Sequence *seq, const int read_len,int tile_cc);
void update_ACGT_pos(long int* ACGT_pos, Sequence *seq, int read_len);
void resize_info(Info* res);

#endif
