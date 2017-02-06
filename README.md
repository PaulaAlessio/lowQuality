## Synopsis

Obtains statistics from the reads quality of a fastq file

## Code Example

`bin/lowQuality -i test/test2.fq.bz2 -l 51 -q 27 -t 214 -o test/output.bin`

### Help Dialog
Usage: `lowQuality -i [INPUT_FILE.fq] -l [READ_LENGTH] -t [NUMBER_OF_TILES] -o [OUTPUT_FILE]` 
Reads in a fq file (gz, bz2, z formats also accepted) and performs a set of 
quality tests. 
Output is in binary format, (write the reader!).
*  `-h` prints help dialog.
*  `-i` Input file `[*fq|*fq.gz|*fq.bz2]`. Required option.
*  `-l` Read length. Length of the reads. Required option.
*  `-t` Number of tiles. Required option. 
*  `-q` Minimum quality allowed. Optional option. Default = 27 .
*  `-o` Output file. required option.


## Motivation

A short description of the motivation behind the creation and maintenance of the project. This should explain **why** the project exists.

## Installation

 - Clone the repository.
 - Run make.
 - Executable: /bin/lowQuality


## Tests

In the folder examples one can run tests! 

## Contributors

Paula Perez 

## License

BSD. See LICENSE.txt
