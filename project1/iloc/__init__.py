import re
import pdb
import logging
from itertools import cycle
from collections import defaultdict
from instruction import Instruction, Address, PhysicalRegister, \
                        VirtualRegister, SpillDict, FEASIBLE_SET


BASE_ADDR = 1020


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

    def register_to_register(self):
        new_program = ILoc(self.target_registers)
        new_program.add_instruction(
            Instruction('loadI', BASE_ADDR, out1=FEASIBLE_SET[0])
        )

        physical_registers = FEASIBLE_SET[-2:] + \
            [PhysicalRegister('r%d' % i) for i in
             range(len(FEASIBLE_SET),
             self.target_registers)]

        vregisters = sorted(
            [reg for reg in self.get_register_count()],
            key=lambda x: int(x.value[1:]),
        )

        self.logger.debug('vregs: %s', vregisters)
        self.logger.debug('pregs: %s', physical_registers)

        for i in range(len(vregisters)):
            vregisters[i].map_to(physical_registers[i])

        for instr in self.program:
            new_program.add_instruction(Instruction(
                instr.opcode,
                instr.input1,
                instr.input2,
                instr.output1,
                instr.output2,
            ))

        return new_program


    def spill_no_live(self):

        new_program = ILoc(self.target_registers)
        new_program.add_instruction(
            Instruction('loadI', BASE_ADDR, out1=FEASIBLE_SET[0])
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

        # fuck it, the other one works with k == F
        if self.target_registers == len(FEASIBLE_SET):
            return self.spill_no_live()

        new_program = ILoc(self.target_registers)
        new_program.add_instruction(
            Instruction('loadI', BASE_ADDR, out1=FEASIBLE_SET[0])
        )
        physical_registers = FEASIBLE_SET[-1:] + \
            [PhysicalRegister('r%d' % i) for i in
             range(len(FEASIBLE_SET),
             self.target_registers)]

        def ensure(register, pre_instructions):
            # is it already loaded?
            if inreg.mapped_currently:
                return

            # is there a free register?
            for physreg in physical_registers:
                if physreg.mapped_from is None:
                    pre_instructions += register.make_load(physreg)
                    register.mapped_currently = True
                    return

            physreg = self.allocate(physical_registers, line_num,
                                    pre_instructions)
            pre_instructions += register.make_load(physreg)
            register.mapped_currently = True

        line_num = 0
        # feasible_iter = cycle(FEASIBLE_SET[1:])
        for instr in self.program:
            self.logger.debug(instr)
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

            inputs = [i for i in instr.get_inputs() if i.is_register()]
            outputs = []

            if instr.opcode.startswith('store'):
                inputs += [i for i in instr.get_outputs()
                           if i.is_register()]
            else:
                outputs = [i for i in instr.get_outputs()
                           if i.is_register()]

            for inreg in inputs:
                ensure(inreg, pre_instructions)

            for inreg in inputs:
                if self.get_next_use(inreg, line_num) < 0:
                    self.logger.debug('input %s not used again, freeing',
                                      inreg)
                    inreg.mapped_to.mapped_from = None

            for outreg in outputs:
                for physreg in physical_registers:
                    if physreg.mapped_from is None:
                        outreg.map_to(physreg)
                        break
                else:
                    physreg = self.allocate(physical_registers, line_num + 1,
                                            pre_instructions)
                    outreg.map_to(physreg)

            new_program.append_instructions(pre_instructions)
            new_program.add_instruction(Instruction(
                instr.opcode,
                *(instr.get_args())
            ))
            new_program.append_instructions(post_instructions)

        return new_program

    def spill_live(self):
        # fuck it, the other one works with k == F
        if self.target_registers == len(FEASIBLE_SET):
            return self.spill_no_live()

        new_program = ILoc(self.target_registers)
        new_program.add_instruction(
            Instruction('loadI', BASE_ADDR, out1=FEASIBLE_SET[0])
        )
        physical_registers = [PhysicalRegister('r%d' % i) for i in
                              range(len(FEASIBLE_SET),
                                    self.target_registers)]

        # Calculate the live ranges
        registers = set()
        line_num = 0
        for instr in self.program:
            line_num += 1
            for arg in instr.get_args():
                if arg.is_register():
                    arg.extend_live_range(line_num)
                    registers.add(arg)

        live_counts = defaultdict(lambda: 0)
        line_regs = defaultdict(lambda: [])
        for r in registers:
            for line in range(r.live_range[0], r.live_range[1] + 1):
                live_counts[line] += 1
                line_regs[line].append(r)

        reg_uses = self.get_register_count()
        for line in line_regs:
            line_regs[line] = sorted(line_regs[line],
                                     key=lambda x: reg_uses[x])

        sorted_lines = sorted(
            [(a, b) for a, b in live_counts.iteritems()],
            key=lambda x: x[1],
            reverse=True
        )

        mapped_registers = set()
        for line in [x[0] for x in sorted_lines]:
            # These get spilled
            for register in line_regs[line][:-(len(physical_registers))]:
                register.spilled = True
            preg_iter = iter(physical_registers)
            for register in line_regs[line][-len(physical_registers):]:
                if register.spilled or register in mapped_registers:
                    continue
                preg = preg_iter.next()
                if line in preg.mapped_lines:
                    for mreg in mapped_registers:
                        self.logger.debug(
                            'checking if %s:%s and %s:%s overlap',
                            mreg, mreg.live_range,
                            register, register.live_range
                        )
                        if mreg.mapped_to == preg and \
                                (mreg.live_range[0] <= register.live_range[1]
                                 or
                                 mreg.live_range[1] >= register.live_range[0]):
                            self.logger.debug('evicting %s from %s for %s',
                                              mreg, preg, register)
                            mreg.spilled = True
                            mreg.mapped_to = None
                register.map_to(preg)
                mapped_registers.add(register)
                self.logger.debug('%s has live range %s',
                                  register, register.live_range)
                for i in range(register.live_range[0],
                               register.live_range[1] + 1):
                    preg.mapped_lines.add(i)

        feasible_iter = cycle(FEASIBLE_SET[1:])
        for instr in self.program:
            post_instructions = []
            new_program.add_instruction(Instruction(
                '\n// %s' % instr.opcode,
                instr.input1.value,
                instr.input2.value,
                instr.output1.value,
                instr.output2.value,
            ))

            for inarg in instr.get_inputs():
                if inarg.is_register() and inarg.spilled:
                    new_program.append_instructions(
                        inarg.make_load(feasible_iter.next())
                    )

            if instr.opcode.startswith('store'):
                for inarg in instr.get_outputs():
                    if inarg.is_register() and inarg.spilled:
                        new_program.append_instructions(
                            inarg.make_load(feasible_iter.next())
                        )

            if not instr.opcode.startswith('store'):
                for outarg in instr.get_outputs():
                    if outarg.is_register() and outarg.spilled:
                        outarg.mapped_to = FEASIBLE_SET[2]
                        post_instructions += outarg.make_spill()

            new_program.add_instruction(Instruction(
                instr.opcode,
                instr.input1,
                instr.input2,
                instr.output1,
                instr.output2,
            ))
            new_program.append_instructions(post_instructions)

        return new_program

    def get_next_use(self, reg, line_num):
        line = line_num
        for instr in self.program[line_num:]:
            line += 1
            for ireg in instr.get_registers():
                if reg == ireg:
                    return line
        return -1

    def allocate(self, physical_registers, line_num, pre_instructions):
        reglist = []
        for instr in self.program[line_num - 1:]:
            reglist += instr.get_registers()

        farthest_away = None
        farthest_index = -1

        for physreg in physical_registers:
            if physreg.mapped_from is None:
                return physreg

            try:
                new_index = reglist.index(physreg.mapped_from)
            except ValueError:
                self.logger.debug('%s never used again, freeing',
                                  physreg.mapped_from)
                return physreg
            if new_index > farthest_index:
                farthest_index = new_index
                farthest_away = physreg.mapped_from

        if farthest_away is None:
            raise Exception('No allocatable register found?')
        physreg = farthest_away.mapped_to
        pre_instructions += farthest_away.make_spill(True)
        return physreg

    def find_farthest(self, line_num):
        seen = set()
        farthest = None
        for instr in self.program[line_num - 1:]:
            for reg in instr.get_registers():
                if reg.mapped_currently and reg not in seen:
                    seen.add(reg)
                    farthest = reg
        return farthest

    def __str__(self):
        out = ''
        for line in self.program:
            out += '%s\n' % line
        return out.strip()
