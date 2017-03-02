getQualStats <- function(path){

   to.read = file(path,"rb")
   N_ACGT = 5
   res <- list() 
   res$read_len <- readBin(to.read, integer(), endian = "little")
   res$ntiles <- readBin(to.read, integer(), endian = "little")
   res$minQ <- readBin(to.read, integer(), endian = "little")
   res$nQ <- readBin(to.read, integer(), endian = "little")
   res$nreads <- readBin(to.read, integer(), endian = "little")
   res$reads_wN <- readBin(to.read, integer(), endian = "little")
   
   res$base_tags <- c("A","C","G","T","N")
   d1 = N_ACGT*(res$ntiles);
   d2 = (res$read_len) + 1;
   d3 = (res$ntiles)*(res$read_len)*(res$nQ);
   d4 = N_ACGT*(res$read_len);
   res$tile_tags <- readBin(to.read, integer(), n=res$ntiles, endian = "little")
   res$qual_tags <- readBin(to.read, integer(), n=res$nQ, endian = "little")
   res$lowQ_ACGT_tile <- t(array(readBin(to.read, integer(), n=d1,size=8, endian = "little"),
                                 dim=c(N_ACGT,res$ntiles), dimnames=list(res$base_tags,res$tile_tags)))
   res$ACGT_tile <- t(array(readBin(to.read, integer(), n=d1,size=8, endian = "little"),
                                 dim=c(N_ACGT,res$ntiles), dimnames=list(res$base_tags,res$tile_tags)))
   res$reads_MlowQ <- t(array(readBin(to.read, integer(), n=d2,size=8, endian = "little"),
                              dim=res$read_len+1,dimnames = list(0:res$read_len))) 
   res$QPosTile_table <- array(readBin(to.read, integer(), n=d3,size=8, endian = "little"), 
                                    dim=c(res$read_len,res$nQ,res$ntiles),
                                    dimnames=list(1:res$read_len,res$qual_tags,res$tile_tags) )
   
   res$ACGT_pos <- array(readBin(to.read, integer(), n=d4,size=8, endian = "little"),
                                 dim=c(N_ACGT,res$read_len), dimnames=list(res$base_tags,1:res$read_len))
   close(to.read)
   res
}


