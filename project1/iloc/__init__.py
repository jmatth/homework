import re
import logging
from collections import defaultdict
from instruction import Instruction

FEASIBLE_SET = ['r%d' % i for i in range(3)]


class SpillDict(defaultdict):

    def __init__(self):
        self.mem_offset = 0
        super(SpillDict, self).__init__(self._newmap)

    def _newmap(self):
        self.mem_offset -= 4
        return self.mem_offset


class ILoc(object):

    """Docstring for ILoc. """

    def __init__(self, target_registers, reader=None):

        self.logger = logging.getLogger('allocator.iloc')

        self.program = []
        self.target_registers = target_registers

        if reader is None:
            return

        for line in reader:
            line = line.strip()
            if line.startswith('//') or len(line) is 0:
                continue

            # FIXME: this doesn't detect missing commas
            self.program.append(Instruction(line=line))

    def add_instruction(self, instr):
        self.program.append(instr)

    def append_instructions(self, instrs):
        self.program += instrs

    def get_register_count(self):
        reg_count = defaultdict(lambda: 0)

        for instruction in self.program:
            registers = instruction.get_registers()
            for reg in registers:
                reg_count[reg] += 1

        return reg_count

    def get_sorted_registers(self):
        registers = self.get_register_count()
        return sorted(
            [(a, b) for a, b in registers.iteritems()],
            key=lambda x: x[1],
            reverse=True
        )

    def spill_no_live(self, target_registers):
        if target_registers < len(FEASIBLE_SET):
            raise Exception("Too few registers on target machine")

        new_program = ILoc(self.target_registers)
        new_program.add_instruction(
            Instruction('loadI', 1024, out1=FEASIBLE_SET[0])
        )
        register_mappings = {}
        spill_mappings = SpillDict()
        used_registers = self.get_sorted_registers()
        reg_number = len(FEASIBLE_SET) + 1
        for reg in used_registers[0:target_registers - len(FEASIBLE_SET)]:
            self.logger.debug('mapping %s to r%d' % (reg[0], reg_number))
            register_mappings[reg[0]] = 'r%d' % reg_number
            reg_number += 1

        for instr in self.program:
            new_program.append_instructions(
                instr.create_mapped_instructions(
                    register_mappings,
                    spill_mappings
                )
            )

        return new_program

    def __str__(self):
        out = ''
        for line in self.program:
            out += '%s\n' % line
        return out.strip()
