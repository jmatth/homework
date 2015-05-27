loadI 1024      => r0
loadI 5         => r1  //input
loadI 1         => r2  //result
loadI 1         => r3  //constant
cmp_LT r3, r1   => r4
cbr   r4        => loop, done
loop:
  mult r2, r1   => r2
  subI r1, 1    => r1
  cmp_LT r3, r1 => r4
  cbr   r4      => loop, done
done:
  storeAI r2    => r0, -4
  output 1020
