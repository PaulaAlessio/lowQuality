# lowQuality user manual

Obtains statistics from the reads quality of a fastq file. 
The binary output is then read by an `Rmd` script that generates a 
html visualization of the data. 

## Installation

 - Clone the repository.
 - Run make.
 

## Running the program

Usage `C` executable (in folder `bin`): 

```
Usage: lowQuality -i [INPUT_FILE.fq] -l [READ_LENGTH] -t [NUMBER_OF_TILES] -o [OUTPUT_FILE] 
Reads in a fq file (gz, bz2, z formats also accepted) and performs a set of 
quality tests. 
Output in binary format.
 -h prints help dialog.
 -i Input file [*fq|*fq.gz|*fq.bz2]. Required option.
 -l Read length. Length of the reads. Required option.
 -t Number of tiles. Default 96. 
 -q Minimum quality allowed. Optional option. Default 27 .
 -o Output file. required option.
 -f If present, the file will be treated as a filtered file (output from filter_trim).
```

Usage `Rmd` scripts (in folder `R`): 

1. Pro sample, 
   ```
    Rscript -e "rmarkdown::render('PATH/TO/quality_report.Rmd',
          params=list(inputfile='PATH/TO/test.out'),
          output_file='PATH/TO/HTML_OUTPUT_FILE.html')"
   ```
2. Summary for all samples
   ```
    Rscript -e "TO BE DONE WHEN I HAVE THE SAMPLES"
   ```

Depends on: `rmarkdown`, `pheatmap` R packages.

Can be used with filtered `fastq` files from `trim_filtered`

## Output description

- Binary output: 
   * `int` (4B) : read length (`read_len`), 
   * `int` (4B) : number of tiles (`ntiles`),
   * `int` (4B) : minimum quality accepted (`minQ`),   
   * `int` (4B) : number of possible qualities (`nQ`), 
   * `int` (4B) : number of reads (`nreads`),
   * `int` (4B) : number of reads containing N's (`reads_wN`),
   * `ntiles*int ` (4x`ntiles`B) : tile tags (`tile_tags`),
   * `nQxint` (4x`nQ`B) : quality tags (`quality tags`),
   * `5 x ntiles x (long int)` (5x`ntiles`x8B) :  # (A,C,G,T,N) per tile with low quality  (`lowQ_ACGT_tile`),
   * `5 x ntiles x (long int)` (5x`ntiles`x8B) : # (A,C,G,T,N) per tile (`ACGT_tile`),
   * `(read_len+1) x (long int)` ((`read_len`+1)x8B) : number of reads with at least `m` low quality nucleotides    (`reads_MlowQ`),
   * `ntiles x read_len x nQ x (long int)` (`ntiles`x`read_len`x`nQ`x8B) : nucleotides  per tile per position with a given quality (`QposTile_table`).
   * `5 x read_len x (long int)` (5x`read_len`x8B ): # (A,C,G,T,N) per position, (`ACGT_pos`)

- html output:
   * Table with general information ,
   * Plot 1: # reads with at least m low Q nucleotides,
   * Low Q nucleotide proportion,
   * Average quality per position per tile,
   * Low Q nucleotides proportion per position per tile,
   * Low Q nucleotides proportion per position for all tiles.
   * Nucleotide content per position

## Example 
  
   An example is given in the folder `examples`. To run an example, type, 

``` 
    ./bin/lowQuality -i ./examples/test.fq.bz2 -l 51 -o ./example/my_output.bin
    Rscript -e "rmarkdown::render('./R/quality_report.Rmd',
                 params=list(inputfile='./examples/my_output.bin'),
                 output_file='./examples/my_test_quality.html')"
```

## Contributors

Paula Pérez Rubio 

## License

GPL v3 (see LICENSE.txt)
