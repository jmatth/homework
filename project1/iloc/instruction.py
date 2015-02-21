from args import *


FEASIBLE_SET = [PhysicalRegister('r%d' % i) for i in range(3)]


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
    'lshift',
    'output',
]


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

        self.input1 = create_arg(in1)
        self.input2 = create_arg(in2)
        self.output1 = create_arg(out1)
        self.output2 = create_arg(out2)

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
        self.input1 = create_arg(inputs[0].strip(','))
        try:
            self.input2 = create_arg(inputs[1].lstrip(','))
        except IndexError:
            pass

    def _process_outputs(self, outputs):
        self.output1 = None
        self.output2 = None

        if len(outputs) < 1:
            return

        self.output1 = create_arg(outputs[0].strip(','))
        try:
            self.output2 = create_arg(outputs[1].lstrip(','))
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
            if isinstance(attr, Register):
                regs.append(attr)

        return regs

    def create_mapped_instructions(self, rmaps, mmaps):
        pre_instructions = []
        post_instructions = []

        in1 = self.input1
        if in1 in rmaps:
            in1 = rmaps[in1]
        elif isinstance(in1, Register):
            in1tmp = FEASIBLE_SET[1]
            pre_instructions += self.generate_load(mmaps[in1], in1tmp)
            in1 = in1tmp

        in2 = self.input2
        if in2 in rmaps:
            in2 = rmaps[in2]
        elif isinstance(in2, Register):
            in2tmp = FEASIBLE_SET[2]
            pre_instructions += self.generate_load(mmaps[in2], in2tmp)
            in2 = in2tmp

        out1 = self.output1
        if out1 in rmaps:
            out1 = rmaps[out1]
        elif isinstance(out1, Register) and not self.opcode.startswith('store'):
            out1tmp = FEASIBLE_SET[2]
            post_instructions += self.generate_store(out1tmp, mmaps[out1])
            out1 = out1tmp

        out2 = self.output2

        return pre_instructions + \
            [Instruction(self.opcode, in1, in2, out1, out2)] + \
            post_instructions

    def generate_load(self, src, dst):
        return [
            Instruction('addI', FEASIBLE_SET[0], src, out1=dst),
            Instruction('load', dst, out1=dst)
        ]

    def generate_store(self, src, dst):
        util_reg = FEASIBLE_SET[1]
        return [
            Instruction('addI', FEASIBLE_SET[0], dst, out1=util_reg),
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
