#!/bin/bash

rm -f cacheSize.dat
for i in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096; do
    ./cacheSize $1 $i | tee -a cacheSize.dat
done

# cat cacheSize.dat | gnuplot -e 'set terminal pdf; set output "cacheSize.pdf"; plot "-" with linespoints'
