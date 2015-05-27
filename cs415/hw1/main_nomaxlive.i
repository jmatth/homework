loadI   1024     => r0

loadI   1        => r1  //a, r1
storeAI r1       => r0, 0

loadI   2        => r1  //b, r2
storeAI r1       => r0, 4

loadAI  r0,  4   => r1
subI    r1,  4   => r2  //c, r3
storeAI r2       => r0, 8

loadAI  r0,  0   => r1
loadAI  r0,  4   => r2
add     r1,  r2  => r2  //d, r4
storeAI r2       => r0, 12

loadAI  r0,  12  => r1
addI    r1,  1   => r3  //e, r5

loadAI  r0,  8   => r1
mult    r1,  r3  => r2
storeAI r2       => r0, 16 //r9
loadAI  r0,  16  => r1
sub     r3,  r1  => r2  //f, r6
storeAI r2       => r0, 20

loadAI  r0,  12  => r1
add     r1,  r3  => r2
storeAI r2       => r0, 24 //r10
loadAI  r0,  24  => r1
loadAI  r0,  20  => r2
add     r1,  r2  => r2  //g, r7
storeAI r2       => r0, 28

loadAI  r0,  28  => r1
loadAI  r0,  0   => r2
add     r1,  r2  => r2  //h, r8
storeAI r2       => r0, 32

loadAI  r0,  32  => r1
storeAI r1       => r0, -4
output 1020
