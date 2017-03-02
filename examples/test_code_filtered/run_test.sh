#!/bin/bash 

# Script that runs the test: 

../../bin/lowQuality -i test_10seq.fq -l 51  -t 96 -o test_10seq.bin -f

Rscript get_data.R  
