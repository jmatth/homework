#!/usr/bin/env python
# encoding: utf-8

import os
import string
import sys
import subprocess as sub  
# Main
def GetTime(cmd):
    args = ["%s%s" % ("./",cmd),"hello"]
    sum=0
    n=0
    for x in range(0, 10):
        p= sub.Popen(args ,stdout = sub.PIPE, stderr= sub.PIPE)
        output, err = p.communicate()
        s1 = output.split('\n')
        s2 = s1[0].split(' ')
        sum  = sum+ float(s2[3])
        n= n+1
    return sum/n
def main() :
    spell_seq_time = GetTime("spell_seq");

    print "spell_seq_time = ", spell_seq_time
    
    spell_t2s_time = GetTime("spell_t2_singleloop");

    print "spell_t2_singleloop time = ", spell_t2s_time, "Speedup = ",spell_seq_time/spell_t2s_time , "X" 

    spell_t2f_time = GetTime("spell_t2_fastest");

    print "spell_t2_fastest time = ", spell_t2f_time, "Speedup = ",spell_seq_time/spell_t2f_time , "X"

    spell_t4s_time = GetTime("spell_t4_singleloop");
    print "spell_t4_singleloop time = ", spell_t4s_time, "Speedup = ",spell_seq_time/spell_t4s_time , "X" 

    spell_t4f_time = GetTime("spell_t4_fastest");

    print "spell_t4_fastest time = ", spell_t4f_time, "Speedup = ",spell_seq_time/spell_t4f_time , "X"
if __name__ == "__main__":
   main()
