loadI     1024      => r0
loadI     5         => r1  //input
storeAI   r1        => r0, 0
loadI     1         => r1  //result
storeAI   r1        => r0, 4
loadI     1         => r1  //constant
storeAI   r1        => r0, 8

loadAI    r0, 8     => r1
loadAI    r0, 0     => r2
cmp_LT    r1, r2    => r1
cbr       r1        => loop, done
loop:
  loadAI  r0, 0     => r1
  loadAI  r0, 4     => r2
  mult    r2, r1    => r2
  subI    r1, 1     => r1
  storeAI r1        => r0, 0
  storeAI r2        => r0, 4
  loadAI  r0, 8     => r2
  cmp_LT  r2, r1    => r2
  cbr     r2        => loop, done
done:
  loadAI  r0, 4     => r1
  storeAI r1        => r0, -4
  output 1020
