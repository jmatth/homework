#!/bin/bash

rm -f cacheLine.dat
for i in 1 2 4 8 16 32 64 128 256 512 1024; do
    ./cacheLine $i | tee -a cacheLine.dat
done

# cat cacheLine.dat | gnuplot -e 'set terminal pdf; set output "cacheLine.pdf"; plot "-" with linespoints'
