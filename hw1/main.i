loadI   1024     => r0
loadI   1        => r1  //a
loadI   2        => r2  //b
subI    r2,  4   => r3  //c
add     r1,  r2  => r4  //d
addI    r4,  1   => r5  //e
mult    r3,  r5  => r9
sub     r5,  r9  => r6  //f
add     r4,  r5  => r10
add     r10, r6  => r7  //g
add     r7,  r1  => r8  //h
storeAI r8       => r0, -4
output 1020
