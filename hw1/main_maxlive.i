loadI   1024     => r0

loadI   1        => r1  //a
storeAI r1       => r0, 0

loadI   2        => r3  //b

subI    r3,  4   => r2  //c
storeAI r2       => r0, 4

loadAI  r0,  0   => r1
add     r1,  r3  => r2  //d
storeAI r2       => r0, 8

loadAI  r0,  8   => r1
addI    r1,  1   => r3  //e

loadAI  r0,  4   => r1
mult    r1,  r3  => r2
storeAI r2       => r0, 12
loadAI  r0,  12   => r1
sub     r3,  r1  => r2  //f
storeAI r2       => r0, 16

loadAI  r0,  8   => r1
add     r1,  r3  => r3
loadAI  r0,  16  => r1
add     r3,  r1  => r3  //g

loadAI  r0,  0   => r1
add     r3,  r1  => r3  //h

storeAI r3       => r0, -4
output 1020
