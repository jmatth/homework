#!/bin/bash

rm -f dummy.dat
for i in 1 2 4 8 16 32 64 128 256 512; do
    ./dummy $i | tee -a dummy.dat
done

# cat dummy.dat | gnuplot -e 'set terminal pdf; set output "dummy.pdf"; plot "-" with linespoints'
