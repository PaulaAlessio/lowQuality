#include "stats_info.h"

/*
 * Static functions*/

static int get_tile(char *line1){
   int i, Dp = 0, i2 = 0 , i1 = 0; 
   char tile[10]; 
   // Obtain tile 
   for (i = 0; Dp < 5; i++){
      if (line1[i] == ':') {
         Dp++; 
         i1 = i2+1;
         i2 = i; 
      }
   }
   memcpy(tile,  line1+i1,i2-i1);
   tile[i2-i1] = '\0';
   return(atoi(tile));
}

static int belongsto(int k, int *qual_tags, int nQ){
   int i; 
   for (i = 0 ; i< nQ; i++){
      if (k == qual_tags[i]) return 1;
   }
   return 0; 
}

static int cmpfunc (const void * a, const void * b){
      return ( *(int*)a - *(int*)b );
}

/***************************************************************/

void init_info(Info *res, const int read_len, const int ntiles, const int minQ, const int nQ){
  int d1, d2, d3, d4; 
  int i; 
  d1 = N_ACGT*ntiles;
  d2 = read_len + 1;
  d3 = ntiles*read_len*nQ;
  d4 = N_ACGT*read_len;

  // Allocate memory
  res -> tile_tags = (int*) calloc(ntiles , sizeof(int));
  res -> qual_tags = (int*) malloc(nQ * sizeof(int));
  for ( i = 0 ; i < nQ ; i++) res -> qual_tags[i] = i; 
  res -> lowQ_ACGT_tile = (long int*) calloc(d1 , sizeof(long int));
  res -> ACGT_tile = (long int*) calloc(d1, sizeof(long int));
  res -> reads_MlowQ = (long int*) calloc(d2,  sizeof(long int));
  res -> QPosTile_table = (long int*) calloc(d3,  sizeof(long int));
  res -> ACGT_pos = (long int*) calloc(d4,  sizeof(long int));
  // Initializations
  res -> nQ = nQ;
  res -> read_len = read_len;
  res -> ntiles = ntiles;
  res -> tile_pos = 0;
  res -> minQ = minQ;
  res -> nreads = 0;
  res -> reads_wN = 0;
}

void free_info(Info* res){
   free(res -> tile_tags);
   free(res -> qual_tags);
   free(res -> lowQ_ACGT_tile); 
   free(res -> ACGT_tile);      
   free(res -> reads_MlowQ);    
   free(res -> QPosTile_table);
   free(res -> ACGT_pos);
   free(res); 
}

void read_info(Info *res, char *file){
   FILE *f; 
   int d1, d2, d3, d4; 
   f = fopen(file, "rb");
   fread(&(res -> read_len),sizeof(int),1,f);
   fread(&(res -> ntiles),sizeof(int),1,f);
   fread(&(res -> minQ),sizeof(int),1,f);
   fread(&(res -> nQ),sizeof(int),1,f);
   fread(&(res -> nreads),sizeof(int),1,f);
   fread(&(res -> reads_wN), sizeof(int),1,f);
   d1 = N_ACGT*(res -> ntiles);
   d2 = (res -> read_len) + 1;
   d3 = (res -> ntiles)*(res -> read_len)*(res -> nQ);
   d4 = N_ACGT*(res -> read_len);
   // Allocate memory
   res -> tile_tags = (int*) malloc(res -> ntiles);
   res -> qual_tags = (int*) malloc(res -> nQ);
   res -> lowQ_ACGT_tile = (long int*) malloc(d1 * sizeof(long int));
   res -> ACGT_tile = (long int*) malloc(d1 * sizeof(long int));
   res -> reads_MlowQ = (long int*) malloc(d2 * sizeof(long int));
   res -> QPosTile_table = (long int*) malloc(d3 * sizeof(long int));
   fread(res -> tile_tags, sizeof(int),res->ntiles,f);
   fread(res -> qual_tags, sizeof(int),res->nQ,f);
   fread(res -> lowQ_ACGT_tile, sizeof(long int),d1,f);
   fread(res -> ACGT_tile, sizeof(long int)*d1,1,f);
   fread(res -> reads_MlowQ, sizeof(long int)*d2,1,f);
   fread(res -> QPosTile_table, sizeof(long int)*d3,1,f);
   fread(res -> ACGT_pos, sizeof(long int)*d4,1,f);
   fclose(f);
}

void write_info(Info *res, char *file){
   FILE *f; 
   int d1, d2, d3, d4; 
   d1 = N_ACGT*(res -> ntiles);
   d2 = (res -> read_len) + 1;
   d3 = (res -> ntiles)*(res -> read_len)*(res-> nQ);
   d4 = N_ACGT*(res -> read_len);
   f = fopen(file, "wb");
   fwrite(&(res -> read_len), sizeof(int),1,f);
   fwrite(&(res -> ntiles), sizeof(int),1,f);
   fwrite(&(res -> minQ), sizeof(int),1,f);
   fwrite(&(res -> nQ), sizeof(int),1,f);
   fwrite(&(res -> nreads), sizeof(int),1,f);
   fwrite(&(res -> reads_wN), sizeof(int),1,f);
   fwrite(res -> tile_tags, sizeof(int),res->ntiles,f);
   fwrite(res -> qual_tags, sizeof(int),res->nQ,f);
   fwrite(res -> lowQ_ACGT_tile, sizeof(long int),d1,f);
   fwrite(res -> ACGT_tile, sizeof(long int)*d1,1,f);
   fwrite(res -> reads_MlowQ, sizeof(long int)*d2,1,f);
   fwrite(res -> QPosTile_table, sizeof(long int)*d3,1,f);
   fwrite(res -> ACGT_pos, sizeof(long int)*d4,1,f);
   fclose(f);
}

void print_info(Info* res, char *infofile){
   int i,j;
   long int max = 0; 
   FILE *f; 
   f = fopen(infofile,"w");
   fprintf(f, "- Read length: %d\n", res -> read_len);
   fprintf(f, "- Number of tiles: %d\n", res -> ntiles);
   fprintf(f, "- Total number of reads: %d\n", res -> nreads);
   fprintf(f, "- Number associated with the first tile: %d\n", res -> tile_tags[0]);
   fprintf(f, "- Min Quality: %d\n", res -> minQ);
   fprintf(f, "- Number of ACGT in the first tile: \n");
   fprintf(f, "  A = %ld, C = %ld, G = %ld, T = %ld , N = %ld\n", 
         res -> ACGT_tile[0], 
         res -> ACGT_tile[1], 
         res -> ACGT_tile[2], 
         res -> ACGT_tile[3], 
         res -> ACGT_tile[4]);
   fprintf(f, "\n- Number of ACGT with low Quality in the first tile:\n");
   fprintf(f, "  A = %ld, C = %ld, G = %ld, T = %ld, N = %ld\n", 
         res -> lowQ_ACGT_tile[0], 
         res -> lowQ_ACGT_tile[1], 
         res -> lowQ_ACGT_tile[2], 
         res -> lowQ_ACGT_tile[3], 
         res -> lowQ_ACGT_tile[4]);
   fprintf(f, "\n- Number of nucleotides of quality (row) in position (col) in the first tile\n ");   
   fprintf(f, "  Position: ");
   for (j = 1 ; j <= res -> read_len; j++) fprintf(f, "%d ",j);
   fprintf(f, "\n");
   for (i = 0; i < (res -> nQ); i++){
      fprintf(f, "  Q = %c : ",(char) (res -> qual_tags[i] + ZEROQ));
      for(j = 0 ; j< res -> read_len; j++){
         fprintf(f, "%ld ", res -> QPosTile_table[i*(res -> read_len) +j]);
      }
      fprintf(f, "\n");
   }
   fprintf(f, "\n- Number of reads with M low quality nucleotides: \n");
   for (i = 0; i < (res -> read_len +1); i++){
      fprintf(f, "  M lowQ = %2d,  Nreads = %ld \n", i, res -> reads_MlowQ[i]);
      if (res -> reads_MlowQ[i] > max && i >0) max = res -> reads_MlowQ[i];
   } 
   fprintf(f, "\n- Histogram with M low quality nucleotides in tile 1: \n\n");
   if (max > 0){
      for (i = 1; i < (res -> read_len +1); i++){
         fprintf(f, "MlowQ = %2d| ",i);
         for ( j = 0 ; j < ( res -> reads_MlowQ[i]*80)/max; j++)
            fprintf(f, "*");
         fprintf(f, "\n");
   
      }
   } else{
      fprintf(f, "   Plotting an histogram not possible (would lead to division by zero).\n");
   } 
   fprintf(f, "\n- Number of nucleotides per position: \n\n");
   fprintf(f, "         A       C       G       T       N   \n");
   for (j =  0; j < res -> read_len; j++){
      fprintf(f, "%3d: %7ld %7ld %7ld %7ld %7ld \n", j+1,
           res -> ACGT_pos[N_ACGT *j ], 
           res -> ACGT_pos[N_ACGT *j+ 1 ], 
           res -> ACGT_pos[N_ACGT *j+ 2 ], 
           res -> ACGT_pos[N_ACGT *j+ 3 ], 
           res -> ACGT_pos[N_ACGT *j+ 4 ]); 
   }
   fclose(f);
}

void get_first_tile(Info* res, Sequence* seq){
   res->tile_tags[0] = get_tile(seq ->line1);

}

void update_info(Info* res, Sequence* seq){
   int i; 
   long int  lowQ = 0;
   int min_quality = ZEROQ + (res -> minQ);
   int tile = get_tile(seq -> line1);
   int Ns = 0;  
   if( res->tile_tags[res -> tile_pos ] != tile){
      res -> tile_tags [++(res -> tile_pos)] = tile;
   }  
   for (i = 0 ; i < seq -> L; i++){
      Ns += update_ACGT_counts(res->ACGT_tile+(res -> tile_pos)*N_ACGT ,seq -> line2[i]);
      if(seq -> line4[i] < min_quality){
         update_ACGT_counts(res->lowQ_ACGT_tile + (res -> tile_pos)*N_ACGT ,seq -> line2[i]);
         lowQ++;
      }
   }
//   if(res->tile_pos>=96)
//      fprintf(stderr, "tiles mayor!\n");
   if ( Ns > 0 ) res -> reads_wN++;
   update_Qtile_table(res -> QPosTile_table, seq , res->read_len ,res -> tile_pos);
   update_ACGT_pos(res -> ACGT_pos, seq , res -> read_len);
   res -> reads_MlowQ[lowQ]++;
   res -> nreads++;
}

int update_ACGT_counts(long int* ACGT_low,  char ACGT){
   switch (ACGT){
      case 'A': case 'a': 
         ACGT_low[0]++;
         break;
      case 'C': case 'c':
         ACGT_low[1]++;
         break;
      case 'G': case 'g':
         ACGT_low[2]++;
         break;
      case 'T': case 't':
         ACGT_low[3]++;
         break;
      // maybe it's better to include anything that is not N
      case 'N': case 'n':
         ACGT_low[4]++;
         return 1;
         break;
   }
   return 0;
}

void update_Qpos_table(long int* Qpos_table, Sequence *seq, const int Ncol){
   int pos = seq -> start;
   int i = 0; 
   while (seq -> line4[i] != '\0'){
      Qpos_table[((int)seq -> line4[i] - ZEROQ)*Ncol + pos]++; 
      i++;
      pos++;
   }
}

void update_Qtile_table(long int* Qtile_table, Sequence *seq, const int read_len,
      int tile_cc){
   int pos = seq -> start;
   int i = 0; 
   // Mucha atencion con los 'indices 
   while (seq -> line4[i] != '\0'){
      //fprintf(stderr,"%d ",tile_cc*read_len*NQ + ((int)seq -> line4[i] - ZEROQ)*read_len + pos);
      if (((int)seq -> line4[i] - ZEROQ) > 41)
          fprintf(stderr," Quality is too large, %d . Are your data Phred+33? Redefine ZEROQ otherwise. \n",((int)seq -> line4[i] - ZEROQ));
      Qtile_table[tile_cc*read_len*NQ + ((int)seq -> line4[i] - ZEROQ)*read_len + pos]++; 
      i++;
      pos++;
   }
}

void update_ACGT_pos(long int* ACGT_pos, Sequence *seq, int read_len){
   int pos = seq -> start;
   int i = 0;
   while (seq -> line2[i] != '\0'){
      update_ACGT_counts(ACGT_pos+N_ACGT*pos, seq -> line2[i]);
      pos++;
      i++;
   }
}

void resize_info(Info* res){
   int i,j,k; 
   int d1, d3;
   int nQ = 0; 
   long int* QPosTile_table;
   if(res -> ntiles != res -> tile_pos + 1){ 
      res -> ntiles = res -> tile_pos + 1; 
      d1 =  N_ACGT*(res -> ntiles);
      // Reallocate to reduce de size of lowQ_ACGT_tile and ACGT_tile
      (res -> ACGT_tile) = (long int *)realloc(res -> ACGT_tile,
            d1*sizeof(long int));
      (res -> lowQ_ACGT_tile) = (long int *)realloc(res -> lowQ_ACGT_tile,
            d1*sizeof(long int));
   }
   // We find out how many qualities are contained in the file; 
   for (i = 0 ; i< (res -> ntiles); i++ ){
      for(j = 0; j < (res -> nQ); j++){
         for (k = 0; k < (res -> read_len) ; k++){
            if (res -> QPosTile_table[i*(res -> read_len)*(res -> nQ) + 
                  j*(res -> read_len) + k ] !=0 && belongsto(j,res -> qual_tags,nQ)==0 ){
               res -> qual_tags[nQ] = j;
               nQ++;
               break; 
            }
         }
      }
   }
   // sort the qual_tags
   qsort(res->qual_tags,nQ,sizeof(int),cmpfunc);
   d3 = (res -> ntiles)*(res -> read_len)*nQ;
   QPosTile_table = (long int *) malloc(d3 * sizeof(long int));
   // initialize the new array
   for (i = 0 ; i< (res -> ntiles); i++ ){
      for(j = 0; j < nQ; j++){
         for (k = 0; k < (res -> read_len) ; k++){
             QPosTile_table[ i*(res -> read_len)*(nQ) + j*(res -> read_len) + k ] = 
                res -> QPosTile_table[i*(res -> read_len)*(res -> nQ) + 
                     (res->qual_tags[j])*(res -> read_len) + k];
         }
      }
   }
   res -> nQ = nQ;
   res -> QPosTile_table = (long int * ) realloc(res -> QPosTile_table, d3* sizeof(long int ));
   memcpy( res -> QPosTile_table,QPosTile_table,d3*sizeof(long int) );
   free(QPosTile_table);
}
