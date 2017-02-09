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
lowQuality -i [INPUT_FILE.fq] -l [READ_LENGTH] -t [NUMBER_OF_TILES] -o [OUTPUT_FILE]` 
Reads in a fq file (gz, bz2, z formats also accepted) and performs a set of 
quality tests. 
Output is in binary format, (write the reader!).
*  `-h` prints help dialog.
*  `-i` Input file `[*fq|*fq.gz|*fq.bz2]`. Required option.
*  `-l` Read length. Length of the reads. Required option.
*  `-t` Number of tiles. Required option. 
*  `-q` Minimum quality allowed. Optional option. Default = 27 .
*  `-o` Output file. required option.
```

Usage `Rmd` script (in folder `R`): 

```
    R -e "rmarkdown::render('quality_report.Rmd',
          params=list(inputfile='test.out'),
          output_file='HTML_OUTPUT_FILE.html')"
```
Depends on: `rmarkdown`, `pheatmap` R packages.

## Output description

- Binary output: 
   * `int` (4B) : read length (`read_len`), 
   * `int` (4B) : number of tiles (`ntiles`),
   * `int` (4B) : minimum quality accepted (`minQ`),   
   * `int` (4B) : number of possible qualities (`nQ`), 
   * `int` (4B) : number of reads (`nreads`),
   * `ntiles*int ` (4x`ntiles`B) : tile tags (`tile_tags`),
   * `nQxint` (4x`nQ`B) : quality tags (`quality tags`),
   * `5 x ntiles x (long int)` (5x`ntiles`x8B) : (A,C,T,G,N) per tile with low quality  (`lowQ_ACGT_tile`),
   * `5 x ntiles x (long int)` (5x`ntiles`x8B) :  (A,C,T,G,N) per tile (`ACGT_tile`),
   * `(read_len+1) x(long int)` ((`read_len`+1)x8B) : number of reads with at least `m` low quality nucleotides    (`reads_MlowQ`),
   * `ntiles x read_lenx nQ x(long int)` (`ntiles`x`read_len`x`nQ`x8B) :  (`QposTile_table`).

- html output:
   * Table with general information ,
   * Plot 1: # reads with at least m low Q nucleotides,
   * Low Q nucleotide proportion,
   * Average quality per position per tile,
   * Low Q nucleotides proportion per position per tile,
   * Low Q nucleotides proportion per position for all tiles.
## Example 
  
   An example is given in the folder `examples`. To run an example, type, 

``` 
    bin/lowQuality -i examples/test.fq.bz2 -l 51 -o example/my_output.bin
    R -e "rmarkdown::render('R/quality_report.Rmd',
                 params=list(inputfile='../examples/my_output.bin'),
                 output_file='../examples/my_test_quality.html')"
```

  

## Contributors

Paula Perez Rubio 

## License

GPL v3 (see LICENSE.txt)
