#!/usr/bin/env python
# encoding: utf-8

import sys
import iloc
import logging

VALID_ALGOS = ['b', 't', 's']


def main(target_registers, algo, file_name):
    logging.basicConfig()
    logger = logging.getLogger('allocator')
    if algo not in VALID_ALGOS:
        print 'ERROR: invalid allocation algo "%s"' % algo
        sys.exit(1)

    if target_registers < len(iloc.FEASIBLE_SET):
        print ('ERROR: to few registers on target architecture, need at lest'
               ' %d' % len(iloc.FEASIBLE_SET))

    try:
        input_file = open(file_name)
    except IOError:
        print 'ERROR: cannot open input file %s' % input_file
        sys.exit(1)

    original_program = iloc.ILoc(input_file, target_registers)
    if algo == 's':
        for instr in original_program.spill_no_live(target_registers):
            print instr


if __name__ == '__main__':
    if len(sys.argv) != 4:
        print 'ERROR: Wrong number of arguments'
        sys.exit(1)

    main(int(sys.argv[1]), sys.argv[2], sys.argv[3])
