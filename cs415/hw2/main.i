loadI   1024     => r0  //    ra  rb
loadI   1        => r1  //a   r1
loadI   2        => r2  //b   ""  r2
subI    r2,  4   => r3  //c   r3  ""
add     r1,  r2  => r4  //d   ""  r4
addI    r4,  1   => r5  //e   ""  r5
mult    r3,  r5  => r9        r9  ""  no spill
sub     r5,  r9  => r6  //f   r6  ""  no spill
add     r4,  r5  => r10       ""  r10 no spill
add     r10, r6  => r7  //g   r7  ""  no spill
add     r7,  r1  => r8  //h   r8  ""  no spill
storeAI r8       => r0, -4
output 1020
