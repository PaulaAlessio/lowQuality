source("../../R/utils.R")

bin_file <- "test_10seq.bin"
data <- getQualStats(bin_file)

sink("check_output.txt")
cat("read_len = ", data$read_len,"\n")
cat("ntiles = ",data$ntiles,"\n")
cat("minQ = ",data$minQ,"\n")
cat("nQ  = " , data$nQ, "\n")
cat("nreads = " ,data$nreads,"\n")
cat("tile_tags = [",data$tile_tags,"]\n")
cat("qual_tags = [",data$qual_tags,"]\n")
cat("lowQ_ACGT_tile = TILE1 [",data$lowQ_ACGT_tile[1,1:5] ,
                   "] TILE2 [",data$lowQ_ACGT_tile[2,1:5]  ,"]\n")
cat("ACGT_tile = TILE1 [",data$ACGT_tile[1,1:5] ,
                   "] TILE2 [",data$ACGT_tile[2,1:5]  ,"]\n")
cat("reads_MlowQ =[", data$reads_MlowQ,"]\n")
cat("QposTile_table:\n") 
cat("               TILE1       TILE2\n")
cat("   Quality : ")
for (j in 1:data$ntiles){
   for(q in data$qual_tags){
      cat(intToUtf8(33+q),"")
   }
   cat("  ")   
}

for(i in 1:data$read_len){
   cat(sprintf("\n      %2d:    ",i))
   for (j in 1:data$ntiles){
      for(k in 1:data$nQ){
         cat(data$QPosTile_table[i,k,j],"" )
      }
      cat("  ")   
   }

}

cat("\nACGT_pos:\n")
cat("    Bases :  A C G T N")
for (i in 1:data$read_len){
   cat(sprintf("\n      %2d:    ",i))
   for( j in (1:5)){
      cat(data$ACGT_pos[j,i],"")
   }
}
