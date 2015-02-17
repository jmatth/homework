#!/usr/bin/env python
# encoding: utf-8

import sys
import iloc
import operator

VALID_ALGOS = ['b', 't', 's']


def main(target_registers, algo, file_name):
    if algo not in VALID_ALGOS:
        print 'ERROR: invalid allocation algo "%s"' % algo
        sys.exit(1)

    if target_registers < iloc.FEASIBLE_SET:
        print ('ERROR: to few registers on target architecture, need at lest'
               ' %d' % FEASIBLE_SET)

    try:
        input_file = open(file_name)
    except IOError:
        print 'ERROR: cannot open input file %s' % input_file
        sys.exit(1)

    original_program = iloc.ILoc(input_file, target_registers)
    print original_program
    regs = original_program.get_sorted_registers()
    for reg in regs:
        print '%s: %d' % (reg[0], reg[1])


if __name__ == '__main__':
    if len(sys.argv) != 4:
        print 'ERROR: Wrong number of arguments'
        sys.exit(1)

    main(sys.argv[1], sys.argv[2], sys.argv[3])
