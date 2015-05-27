#!/bin/bash

ILOC=/ilab/users/uli/cs415/spring2015/ILOC_Simulator/sim

failed_blocks=""
failed_count=0

# for block in blocks/block*.i; do
#   expected_output=$($ILOC -i 1024 0 1 < $block | grep -v Executed)
#
#   for algo in s t b; do
#     for regs in 5 10 20 100; do
#       algo_output=$(./alloc $regs $algo $block 2> /dev/null | $ILOC -i 1024 0 1 | grep -v Executed)
#       if [[ "$algo_output" != "$expected_output" ]]; then
#         echo "ERROR: $algo with $regs registers failed on $block"
#         echo -e "Expected:\n$expected_output"
#         echo -e "Got:\n$algo_output"
#         (( failed_count += 1))
#         failed_blocks="${failed_blocks}${block} "
#       fi
#     done
#   done
# done

for block in blocks/report*.i; do
  input="-i 1024 0 1"
  # if [[ "$block" == "blocks/report1.i" ]]; then
  #   input="-i 4000 0 10 20 30 40 50 60 70 80 90"
  # elif [[ "$block" == "blocks/report2.i" ]]; then
  #   input="-i 1024 0 1"
  # elif [[ "$block" == "blocks/report3.i" ]]; then
  #   input="-i 1024 0 1"
  # elif [[ "$block" == "blocks/report4.i" ]]; then
  #   input="-i 1024 0 1 2"
  # elif [[ "$block" == "blocks/report5.i" ]]; then
  #   input="-i 1024 1"
  # elif [[ "$block" == "blocks/report6.i" ]]; then
  #   input="-i 1024 0 1"
  # fi

  expected_output=$($ILOC $input < $block | grep -v Executed)

  for algo in s t b; do
    for regs in 5 8 10 15 20; do
      algo_output=$(./alloc $regs $algo $block 2> /dev/null | $ILOC $input | grep -v Executed)
      if [[ "$algo_output" != "$expected_output" ]]; then
        echo "ERROR: $algo with $regs registers failed on $block"
        echo -e "Expected:\n\e[32m$expected_output\e[0m"
        echo -e "Got:\n\e[31m$algo_output\e[0m"
        (( failed_count += 1))
        failed_blocks="${failed_blocks}${block} "
      fi
    done
  done
done

echo -e "\nTotal failed: $failed_count"
echo "Failed blocks: $failed_blocks"
