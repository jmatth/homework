import re
from collections import defaultdict

FEASIBLE_SET_NUM = 2
FEASIBLE_SET = ['r%d' % (i + 1) for i in range(FEASIBLE_SET_NUM)]
BASE_REGISTER = 'r0'

OPCODES = [
    'loadI',
    'load',
    'loadAI',
    'loadAO',
    'store',
    'storeAI',
    'storeAO',
    'nop',
    'addI',
    'add',
    'subI',
    'sub',
    'mult',
    'br',
    'cbr',
    'cmp_LT',
    'cmp_LE',
    'rshift',
    'output',
]


def is_register(ident):
    try:
        if re.match('^r[0-9]+$', ident):
            return True
    except:
        pass
    return False


class spillDict(defaultdict):

    def __init__(self):
        self.mem_offset = 0
        super(spillDict, self).__init__(self._newmap)

    def _newmap(self):
        self.mem_offset -= 4
        return self.mem_offset


class Instruction(object):

    """Docstring for Instruction. """

    def __init__(self, opcode=None, in1=None, in2=None,
                 out1=None, out2=None, line=None):
        if line is not None:
            self._new_from_line(line)
            return

        if opcode not in OPCODES:
            raise Exception("No such opcode '%s'" % opcode)
        self.opcode = opcode

        self.input1 = in1
        self.input2 = in2
        self.output1 = out1
        self.output2 = out2

    def _new_from_line(self, line):
        opcode = None
        in1 = None
        in2 = None
        out1 = None
        out2 = None

        tokens = line.split()
        opcode = tokens.pop(0)
        if opcode not in OPCODES:
            raise Exception("No such opcode '%s'" % opcode)
        self.opcode = opcode

        try:
            delim = tokens.index('=>')
        except ValueError:
            delim = len(tokens)

        self._process_inputs(tokens[0:delim])
        self._process_outputs(tokens[delim + 1:])

    def _process_inputs(self, inputs):
        self.input1 = None
        self.input2 = None

        if len(inputs) < 1:
            return
        self.input1 = inputs[0].strip(',')
        try:
            self.input2 = inputs[1].lstrip(',')
        except IndexError:
            pass

    def _process_outputs(self, outputs):
        self.output1 = None
        self.output2 = None

        if len(outputs) < 1:
            return

        self.output1 = outputs[0].strip(',')
        try:
            self.output2 = outputs[1].lstrip(',')
        except IndexError:
            pass

    def get_args(self):
        return self.get_inputs() + self.get_outputs()

    def get_inputs(self):
        ret = []
        for attr in ['input1', 'input2']:
            if hasattr(self, attr) and getattr(self, attr) is not None:
                ret.append(getattr(self, attr))
        return ret

    def get_outputs(self):
        ret = []
        for attr in ['output1', 'output2']:
            if hasattr(self, attr) and getattr(self, attr) is not None:
                ret.append(getattr(self, attr))
        return ret

    def get_registers(self):
        regs = []
        for attr in self.get_args():
            if is_register(attr):
                regs.append(attr)

        return regs

    def create_mapped_instructions(self, rmaps, mmaps):
        pre_instructions = []
        post_instructions = []

        in1 = self.input1
        if in1 in rmaps:
            in1 = rmaps[in1]
        elif is_register(in1):
            in1tmp = FEASIBLE_SET[0]
            pre_instructions += self.generate_load(mmaps[in1], in1tmp)
            in1 = in1tmp

        in2 = self.input2
        if in2 in rmaps:
            in2 = rmaps[in2]
        elif is_register(in2):
            in2tmp = FEASIBLE_SET[1]
            pre_instructions += self.generate_load(mmaps[in2], in2tmp)
            in2 = in2tmp

        out1 = self.output1
        if out1 in rmaps:
            out1 = rmaps[out1]
        elif is_register(out1) and not self.opcode.startswith('store'):
            out1tmp = FEASIBLE_SET[1]
            post_instructions += self.generate_store(out1tmp, mmaps[out1])
            out1 = out1tmp

        out2 = self.output2

        return pre_instructions + \
            [Instruction(self.opcode, in1, in2, out1, out2)] + \
            post_instructions

    def generate_load(self, src, dst):
        return [
            Instruction('addI', BASE_REGISTER, src, out1=dst),
            Instruction('load', dst, out1=dst)
        ]

    def generate_store(self, src, dst):
        util_reg = FEASIBLE_SET[0]
        return [
            Instruction('addI', BASE_REGISTER, dst, out1=util_reg),
            Instruction('store', src, out1=util_reg)
        ]

    def __str__(self):
        ret = self.opcode
        if self.input1 is not None:
            ret += ' %s' % self.input1
        if self.input2 is not None:
            ret += ', %s' % self.input2

        if self.output1 is not None:
            ret += ' => %s' % self.output1
        if self.output2 is not None:
            ret += ', %s' % self.output2

        return ret


class ILoc(object):

    """Docstring for ILoc. """

    def __init__(self, reader, target_registers):
        """TODO: to be defined1. """

        self.program = []
        self.target_registers = target_registers

        for line in reader:
            line = line.strip()
            if line.startswith('//') or len(line) is 0:
                continue

            # FIXME: this doesn't detect missing commas
            self.program.append(Instruction(line=line))

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
        if target_registers < FEASIBLE_SET_NUM:
            raise Exception("Too few registers on target machine")

        new_program = [Instruction('loadI', 1024, out1=BASE_REGISTER)]
        register_mappings = {}
        spill_mappings = spillDict()
        used_registers = self.get_sorted_registers()
        reg_number = FEASIBLE_SET_NUM + 1
        for reg in used_registers[0:target_registers - FEASIBLE_SET_NUM]:
            register_mappings[reg[0]] = 'r%d' % reg_number
            reg_number += 1

        for instr in self.program:
            new_program += instr.create_mapped_instructions(register_mappings,
                                                            spill_mappings)

        return new_program

    def __str__(self):
        out = ''
        for line in self.program:
            out += '%s\n' % line
        return out.strip()
