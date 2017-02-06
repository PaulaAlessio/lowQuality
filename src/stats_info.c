#include "stats_info.h"



void init_info(Info *res, const int read_len, const int ntiles, const int minQ){
  int d1, d2, d3; 
  d1 = N_ACGT*ntiles;
  d2 = read_len + 1;
  d3 = ntiles*read_len*NQ;
  
  // Allocate memory
  res -> lowQ_ACGT_tile = (long int*) malloc(d1 * sizeof(long int));
  res -> ACGT_tile = (long int*) malloc(d1 * sizeof(long int));
  res -> reads_MlowQ = (long int*) malloc(d2 * sizeof(long int));
  res -> QPosTile_table = (long int*) malloc(d3 * sizeof(long int));
  // Initializations
  res -> read_len = read_len;
  res -> ntiles = ntiles;
  res -> minQ = minQ;
  res -> first_tile = FIRSTTILE; 
  res -> cur_tile = 0; 
  res -> old_tile = 0;
  memset(res->lowQ_ACGT_tile,0,sizeof(long int)*d1);
  memset(res->ACGT_tile,0,sizeof(long int)*d1);
  memset(res->reads_MlowQ,0,sizeof(long int)*d2);
  memset(res->QPosTile_table,0,sizeof(long int)*d3);
}

void free_info(Info* res){
   free(res -> lowQ_ACGT_tile); 
   free(res -> ACGT_tile);      
   free(res -> reads_MlowQ);    
   free(res -> QPosTile_table);
   free(res); 
}

void read_info(Info *res, char *file){
   FILE *f; 
   int d1, d2, d3; 
   f = fopen(file, "rb");
   fread(&(res -> read_len),sizeof(int),1,f);
   fread(&(res -> ntiles),sizeof(int),1,f);
   fread(&(res -> first_tile),sizeof(int),1,f);
   fread(&(res -> minQ),sizeof(int),1,f);
   d1 = N_ACGT*(res -> ntiles);
   d2 = (res -> read_len) + 1;
   d3 = (res -> ntiles)*(res -> read_len)*NQ;
   // Allocate memory
   res -> lowQ_ACGT_tile = (long int*) malloc(d1 * sizeof(long int));
   res -> ACGT_tile = (long int*) malloc(d1 * sizeof(long int));
   res -> reads_MlowQ = (long int*) malloc(d2 * sizeof(long int));
   res -> QPosTile_table = (long int*) malloc(d3 * sizeof(long int));
   fread(res -> lowQ_ACGT_tile, sizeof(long int),d1,f);
   fread(res -> ACGT_tile, sizeof(long int)*d1,1,f);
   fread(res -> reads_MlowQ, sizeof(long int)*d2,1,f);
   fread(res -> QPosTile_table, sizeof(long int)*d3,1,f);
   fclose(f);
}

void write_info(Info *res, char *file){
   FILE *f; 
   int d1, d2, d3; 
   d1 = N_ACGT*(res -> ntiles);
   d2 = (res -> read_len) + 1;
   d3 = (res -> ntiles)*(res -> read_len)*NQ;
   f = fopen(file, "wb");
   fwrite(&(res -> read_len), sizeof(int),1,f);
   fwrite(&(res -> ntiles), sizeof(int),1,f);
   fwrite(&(res -> first_tile), sizeof(int),1,f);
   fwrite(&(res -> minQ), sizeof(int),1,f);
   fwrite(res -> lowQ_ACGT_tile, sizeof(long int),d1,f);
   fwrite(res -> ACGT_tile, sizeof(long int)*d1,1,f);
   fwrite(res -> reads_MlowQ, sizeof(long int)*d2,1,f);
   fwrite(res -> QPosTile_table, sizeof(long int)*d3,1,f);
   fclose(f);
}

void print_info(Info* res){
   int i,j;
   long int max = 0; 
   printf("- Read length: %d\n", res -> read_len);
   printf("- Number of tiles: %d\n", res -> ntiles);
   printf("- Number associated with the first tile: %d\n", res -> first_tile);
   printf("- Min Quality: %d\n", res -> minQ);
   printf("- Number of ACGT in the first tile: \n");
   printf("  A = %ld, C = %ld, G = %ld, T = %ld , N = %ld\n", 
         res -> ACGT_tile[0], 
         res -> ACGT_tile[1], 
         res -> ACGT_tile[2], 
         res -> ACGT_tile[3], 
         res -> ACGT_tile[4]);
   printf("\n- Number of ACGT with low Quality in the first tile:\n");
   printf("  A = %ld, C = %ld, G = %ld, T = %ld, N = %ld\n", 
         res -> lowQ_ACGT_tile[0], 
         res -> lowQ_ACGT_tile[1], 
         res -> lowQ_ACGT_tile[2], 
         res -> lowQ_ACGT_tile[3], 
         res -> lowQ_ACGT_tile[4]);
   printf("\n- Number of nucleotides of quality (row) in position (col) in the first tile\n ");   
      printf("  Position: ");
   for (j = 1 ; j <= res -> read_len; j++) printf("%d ",j);
   printf("\n");
   for (i = 0; i < NQ; i++){
      printf("  Q = %c : ",(char) (i + ZEROQ));
      for(j = 0 ; j< res -> read_len; j++){
         printf("%ld ", res -> QPosTile_table[i*(res -> read_len) +j]);
      }
      printf("\n");
   }
   printf("\n- Number of reads with M low quality nucleotides in tile 1: \n");
   for (i = 0; i < (res -> read_len +1); i++){
      printf("  M lowQ = %2d,  Nreads = %ld \n", i, res -> reads_MlowQ[i]);
      if (res -> reads_MlowQ[i] > max && i >0) max = res -> reads_MlowQ[i];
   } 
   printf("\n- Histogram with M low quality nucleotides in tile 1: \n\n");
   for (i = 1; i < (res -> read_len +1); i++){
      printf("MlowQ = %2d| ",i);
      for ( j = 0 ; j < ( res -> reads_MlowQ[i]*80)/max; j++)
         printf("*");
      printf("\n");

   } 
   
}


void get_first_tile(Info* res, Read* read){
   int i1 = 0, i2 = 0, Dp = 0 ,i;
   char tile[10]; 
   // Obtain tile 
   for (i = 0; Dp < 5; i++){
      if (read -> line1[i] == ':') {
         Dp++; 
         i1 = i2 + 1;
         i2 = i; 
      }
   }
   memcpy(tile, read -> line1+i1,i2-i1);
   tile[i2-i1] = '\0';
   res->first_tile = atoi(tile);

}


void update_info(Info* res, Read* read){
   int i, Dp = 0, i2 = 0 , i1 = 0; 
   long int  lowQ = 0;
   int min_quality = ZEROQ + (res -> minQ); 
   char tile[10]; 
   int tile_c = res->cur_tile - res->first_tile;
   // Obtain tile 
   for (i = 0; Dp < 5; i++){
      if (read -> line1[i] == ':') {
         Dp++; 
         i1 = i2+1;
         i2 = i; 
      }
   }
   memcpy(tile, read -> line1+i1,i2-i1);
   tile[i2-i1] = '\0';
   tile_c = atoi(tile) - res->first_tile;
   if(tile_c != res->cur_tile){
      res -> old_tile = res -> cur_tile;
      res -> cur_tile = tile_c;
      printf(" We are in the %dth tile! \n", res ->cur_tile);
   }
   for (i = 0 ; i < res->read_len; i++){
      update_ACGT_counts(res->ACGT_tile+tile_c*N_ACGT ,read -> line2[i]);
      if(read -> line4[i] < min_quality){
         update_ACGT_counts(res->lowQ_ACGT_tile+tile_c*N_ACGT ,read -> line2[i]);
         lowQ++;
      }
   }
   update_Qtile_table(res -> QPosTile_table, read -> line4, res->read_len ,tile_c);
   res -> reads_MlowQ[lowQ]++;
}

void update_ACGT_counts(long int* ACGT_low,  char ACGT){
   switch (ACGT){
      case 'A': 
         ACGT_low[0]++;
         break;
      case 'C': 
         ACGT_low[1]++;
         break;
      case 'G': 
         ACGT_low[2]++;
         break;
      case 'T': 
         ACGT_low[3]++;
         break;
      case 'N': 
         ACGT_low[4]++;
         break;
   }
}

void update_Qpos_table(long int* Qpos_table, char* line4, const int Ncol){
   int pos = 0;
   while (line4[pos] != '\0'){
      Qpos_table[((int)line4[pos] - ZEROQ)*Ncol + pos]++; 
      pos++;
   }
}

void update_Qtile_table(long int* Qtile_table, char* line4, const int read_len,int tile_cc){
   int pos = 0;
   // Mucha atencion con los 'indices 
   while (line4[pos] != '\0'){
      Qtile_table[tile_cc*read_len*NQ + ((int)line4[pos] - ZEROQ)*read_len + pos]++; 
      pos++;
   }
}
