import re
import logging
from itertools import cycle
from collections import defaultdict
from instruction import Instruction, Address, PhysicalRegister, \
                        VirtualRegister, SpillDict, FEASIBLE_SET


class ILoc(object):

    """Docstring for ILoc. """

    def __init__(self, target_registers, reader=None):

        if target_registers < len(FEASIBLE_SET):
            raise Exception('Too few registers on target machine')

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

    def spill_no_live(self):

        new_program = ILoc(self.target_registers)
        new_program.add_instruction(
            Instruction('loadI', 1024, out1=FEASIBLE_SET[0])
        )
        register_mappings = {}
        spill_mappings = SpillDict()
        used_registers = self.get_sorted_registers()
        reg_number = len(FEASIBLE_SET)
        for reg in used_registers[0:self.target_registers - len(FEASIBLE_SET)]:
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

    def spill_bottom_up(self):
        if self.target_registers < len(FEASIBLE_SET):
            raise Exception("Too few registers on target machine")

        # FIXME: fuck it, the other one works with k == F
        if self.target_registers == len(FEASIBLE_SET):
            return self.spill_no_live()

        new_program = ILoc(self.target_registers)
        new_program.add_instruction(
            Instruction('loadI', 1024, out1=FEASIBLE_SET[0])
        )
        physical_registers = [PhysicalRegister('r%d' % i) for i in
                              range(len(FEASIBLE_SET),
                                    self.target_registers)]

        line_num = 0
        feasible_iter = cycle(FEASIBLE_SET[1:])
        for instr in self.program:
            new_program.add_instruction(Instruction(
                '\n// %s' % instr.opcode,
                instr.input1.value,
                instr.input2.value,
                instr.output1.value,
                instr.output2.value,
            ))
            pre_instructions = []
            post_instructions = []
            line_num += 1
            outputs = instr.get_outputs()

            # For each output...
            for outreg in outputs:
                # Skip if it isn't a register
                if not outreg.is_register():
                    continue
                # If it's a spilled register and the opcode is a store type,
                # load it back into a feasible set register
                elif outreg.spilled and instr.opcode.startswith('store'):
                    pre_instructions += outreg.make_load(feasible_iter.next())
                    continue
                # Otherwise, try to map it to an available register
                for physreg in physical_registers:
                    if physreg.mapped_from is None:
                        self.logger.debug('found empty physreg %s', physreg)
                        outreg.map_to(physreg)
                        break
                # If none are available, try to find the one that's used by the
                # farthest away register
                else:
                    farthest_away_reg = None
                    farthest_away_line = 0
                    for physreg in physical_registers:
                        vreg = physreg.mapped_from
                        next_use = self.get_next_use(vreg, line_num)
                        if next_use is -1:
                            vreg.mapped_currently = False
                            pre_instructions += \
                                vreg.make_spill()
                            outreg.map_to(physreg)
                            break
                        elif next_use > farthest_away_line:
                            farthest_away_reg = vreg
                            farthest_away_line = next_use
                    else:
                        if farthest_away_reg is not None:
                            self.logger.debug(
                                'spilling %s for %s',
                                farthest_away_reg,
                                outreg
                            )
                            pre_instructions += farthest_away_reg.make_spill()
                            outreg.map_to(physreg)

            for inreg in instr.get_inputs():
                if not inreg.is_register():
                    continue
                if inreg.spilled:
                    pre_instructions += inreg.make_load(feasible_iter.next())

            new_program.append_instructions(pre_instructions)
            new_program.add_instruction(Instruction(
                instr.opcode,
                instr.input1,
                instr.input2,
                instr.output1,
                instr.output2
            ))
            new_program.append_instructions(post_instructions)

        return new_program

    def get_next_use(self, reg, line_num):
        line = line_num - 1
        for instr in self.program[line_num + 1:]:
            line += 1
            for ireg in instr.get_registers():
                if reg == ireg:
                    return line
        return -1

    def __str__(self):
        out = ''
        for line in self.program:
            out += '%s\n' % line
        return out.strip()
