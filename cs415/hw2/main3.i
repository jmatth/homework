loadI   1024     => r0  //    r3  r4
loadI   1        => r3  //a   r1
loadI   2        => r4  //b   ""  r2
storeAI r3       => r0, -8
subI    r4,  4   => r3  //c   r3  ""
loadAI  r0,  -8  => r1  //
add     r1,  r4  => r4  //d   ""  r4
storeAI r4       => r0, -12
loadAI  r0, -12  => r1  //
addI    r1,  1   => r4  //e   ""  r5
mult    r3,  r4  => r3  //    r9  ""
sub     r4,  r3  => r3  //f   r6  ""
loadAI  r0, -12  => r1  //
add     r1,  r4  => r4  //    ""  r10
add     r4,  r3  => r3  //g   r7  ""
loadAI  r0,  -8  => r1  //
add     r3,  r1  => r3  //h   r8  ""
storeAI r3       => r0, -4
output 1020
