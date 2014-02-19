#!/bin/bash

rm -f pipe.dat
for i in 1 2 4 8 16 32 64 128 256 512; do
    ./pipe $i | tee -a pipe.dat
done

# cat pipe.dat | gnuplot -e 'set terminal pdf; set output "pipe.pdf"; plot "-" with linespoints'
