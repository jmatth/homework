#!/bin/bash

ILOC=/ilab/users/uli/cs415/spring2015/ILOC_Simulator/sim

failed_blocks=""
failed_count=0

TIMES=times.csv

echo 'block,algo,registers,alloc_time,alloced_instructions,original_instructions' > $TIMES

for block in blocks/report*.i; do
  input=""
  if [[ "$block" == "blocks/report1.i" ]]; then
    input="-i 4000 0 10 20 30 40 50 60 70 80 90"
  elif [[ "$block" == "blocks/report2.i" ]]; then
    input="-i 1024 0 1"
  elif [[ "$block" == "blocks/report3.i" ]]; then
    input="-i 1024 0 1"
  elif [[ "$block" == "blocks/report4.i" ]]; then
    input="-i 1024 0 1 2"
  elif [[ "$block" == "blocks/report5.i" ]]; then
    input="-i 1024 1"
  elif [[ "$block" == "blocks/report6.i" ]]; then
    input="-i 1024 0 1"
  fi
  expected_output=$($ILOC $input < $block | grep -v Executed)

  for algo in s t b; do
    for regs in 5 10 20; do
      echo -n `basename $block`, >> $TIMES
      echo -n ${algo}, >> $TIMES
      echo -n ${regs}, >> $TIMES

      alloc_command="./alloc $regs $algo $block"
      (time ./alloc 5 b blocks/report1.i > /dev/null ) 2>&1 | tail -n2 | head -n1 | cut -f2 | tr -d '\n' >> $TIMES
      echo -n , >> $TIMES
      $alloc_command | $ILOC | tail -n2 | head -n1 | tee -a testout | cut -d' ' -f8 | tr -d '\n' >> $TIMES
      echo -n , >> $TIMES
      $ILOC < $block | tail -n2 | head -n1 | tee -a testout | cut -d' '  -f8 >> $TIMES
    done
  done
done
