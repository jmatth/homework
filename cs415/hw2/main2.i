loadI   1024     => r0  //    r3  r4  r5
loadI   1        => r3  //a   r1
loadI   2        => r4  //b   ""  r2
subI    r4,  4   => r5  //c   ""  ""  r3
add     r3,  r4  => r4  //d   ""  r4  ""
storeAI r3       => r0, -8 // ""  ""  ""
addI    r4,  1   => r3  //e   r5  ""  ""
mult    r5,  r3  => r5  //    ""  ""  r9
sub     r3,  r5  => r5  //f   ""  ""  r6
add     r4,  r3  => r3  //    r10 ""  ""
add     r3,  r5  => r4  //g   ""  r7  ""
loadAI  r0, -8   => r1  //    ""  ""  ""
add     r4,  r1  => r3  //h   r8  ""  ""
storeAI r3       => r0, -4
output 1020
