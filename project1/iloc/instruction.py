import re
import logging
from collections import defaultdict


class SpillDict(defaultdict):

    def __init__(self):
        self.mem_offset = 0
        super(SpillDict, self).__init__(self._newmap)

    def _newmap(self):
        self.mem_offset -= 4
        return Address(self.mem_offset)


def create_arg(value):
    if value is None:
        return Arg(value)

    if isinstance(value, Arg):
        return value.translate()

    try:
        return VirtualRegister(value)
    except AssertionError:
        return Address(value)


class flyweight(object):
    def __init__(self, cls):
        self._cls = cls
        self._instances = dict()

    def __call__(self, *args, **kargs):
        return self._instances.setdefault((args, tuple(kargs.items())),
                                          self._cls(*args, **kargs))


class Arg(object):

    def __init__(self, value):
        self.value = value
        self.logger = logging.getLogger('allocator.iloc.arg')

    def is_register(self):
        return isinstance(self, Register)

    def is_address(self):
        return isinstance(self, Address)

    def translate(self):
        return self

    def __str__(self):
        if self.value is None:
            return ''
        return '%s' % self.value

    def __repr__(self):
        return '%s' % self.value


@flyweight
class Address(Arg):

    def __init__(self, value):
        value = int(value)
        Arg.__init__(self, value)


class Register(Arg):

    def __init__(self, value):
        reg_regex = '^r[0-9]+$'
        assert re.match(reg_regex, str(value)), \
            '%s does not match %s' % (value, reg_regex)
        Arg.__init__(self, value)


@flyweight
class PhysicalRegister(Register):

    def __init__(self, value):
        Register.__init__(self, value)
        self.mapped_from = None
        self.mapped_lines = set()

FEASIBLE_SET = [PhysicalRegister('r%d' % i) for i in range(3)]


@flyweight
class VirtualRegister(Register):

    spill_maps = SpillDict()

    def __init__(self, value):
        self.spilled = False
        self.mapped_to = None
        self.mapped_currently = False
        self.spilled = False
        self.live_range = (-1, -1)
        Register.__init__(self, value)

    def map_to(self, preg):
        self.logger.debug('Mapping %s to %s', self, preg)
        self.mapped_to = preg
        preg.mapped_from = self
        self.mapped_currently = True

    def make_spill(self, bu=False):
        self.mapped_currently = False
        # special case because bottom up and top-down expect different behavior
        if bu:
            # if already spilled
            if self.spilled:
                return []

        self.logger.debug('Spilling %s to %s', self, self.spill_maps[self])
        self.spilled = True
        return [
            Instruction('addI', FEASIBLE_SET[0], self.spill_maps[self],
                        out1=FEASIBLE_SET[1]),
            Instruction('store', self.mapped_to, out1=FEASIBLE_SET[1])
        ]

    def make_load(self, load_to):
        if not self.spilled:
            raise Exception('Trying to load non-spilled register %s?' %
                            self.value)
        self.logger.debug('Loading %s from %s', self, self.spill_maps[self])
        self.mapped_to = load_to
        load_to.mapped_from = self
        return [
            Instruction('addI', FEASIBLE_SET[0], self.spill_maps[self],
                        out1=load_to),
            Instruction('load', load_to, out1=load_to)
        ]

    def extend_live_range(self, line_num):
        if self.live_range[0] < 0:
            self.live_range = (line_num, line_num)
        else:
            self.live_range = (self.live_range[0], line_num - 1)

    def translate(self):
        if self.mapped_to is not None:
            return self.mapped_to
        return self


OPCODES = [
    'loadI',
    'load',
    # 'loadAI',
    # 'loadAO',
    'store',
    # 'storeAI',
    # 'storeAO',
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

OPCODES = OPCODES + ['\n// %s' % op for op in OPCODES]


class Instruction(object):

    """Docstring for Instruction. """

    def __init__(self, opcode=None, in1=None, in2=None,
                 out1=None, out2=None, line=None):
        if line is not None:
            self._new_from_line(line)
            return

        # if opcode not in OPCODES:
        #     raise Exception("No such opcode '%s'" % opcode)
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
        # if opcode not in OPCODES:
        #     raise Exception("No such opcode '%s'" % opcode)
        self.opcode = opcode

        try:
            delim = tokens.index('=>')
        except ValueError:
            delim = len(tokens)

        self._process_inputs(tokens[0:delim])
        self._process_outputs(tokens[delim + 1:])

    def _process_inputs(self, inputs):
        self.input1 = create_arg(None)
        self.input2 = create_arg(None)

        if len(inputs) < 1:
            return
        self.input1 = create_arg(inputs[0].strip(','))
        try:
            self.input2 = create_arg(inputs[1].lstrip(','))
        except IndexError:
            pass

    def _process_outputs(self, outputs):
        self.output1 = create_arg(None)
        self.output2 = create_arg(None)

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
        elif isinstance(out1, Register):
            if not self.opcode.startswith('store'):
                out1tmp = FEASIBLE_SET[2]
                post_instructions += self.generate_store(out1tmp, mmaps[out1])
                out1 = out1tmp
            else:
                out1tmp = FEASIBLE_SET[2]
                pre_instructions += self.generate_load(mmaps[out1], out1tmp)
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

    def new_generate_load(self):
        pass

    def __str__(self):
        ret = self.opcode
        if self.input1.value is not None:
            ret += ' %s' % self.input1
        if self.input2.value is not None:
            ret += ', %s' % self.input2

        if self.output1.value is not None:
            ret += ' => %s' % self.output1
        if self.output2.value is not None:
            ret += ', %s' % self.output2

        return ret
