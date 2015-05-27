#!/bin/bash

rm -f fork.dat
for i in 1 2 4 8 16 32 64 128 256 512 1024; do
    ./fork $i | tee -a fork.dat
done

# cat fork.dat | gnuplot -e 'set terminal pdf; set output "fork.pdf"; plot "-" with linespoints'
