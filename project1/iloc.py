import re
from collections import defaultdict

FEASIBLE_SET = 3

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
    if re.match('^r[0-9]+$', ident):
        return True
    return False


class Instruction(object):

    """Docstring for Instruction. """

    def __init__(self, line):
        """TODO: to be defined1.

        :line: TODO

        """
        self.opcode = None

        tokens = line.split()
        opcode = tokens.pop(0)
        if opcode not in OPCODES:
            raise Exception("No such opcode '%s'" % opcode)
        self.opcode = opcode

        try:
            delim = tokens.index('=>')
        except ValueError:
            delim = len(tokens)

        self._process_operands(tokens[0:delim])
        self._process_outputs(tokens[delim + 1:])

    def _process_operands(self, operands):
        if len(operands) < 1:
            return
        self.operand1 = operands[0].strip(',')
        try:
            self.operand2 = operands[1].lstrip(',')
        except IndexError:
            pass

    def _process_outputs(self, outputs):
        if len(outputs) < 1:
            return

        self.output1 = outputs[0].strip(',')
        try:
            self.output2 = outputs[1].lstrip(',')
        except IndexError:
            pass

    def get_args(self):
        ret = []
        for attr in ['operand1', 'operand2', 'output1', 'output2']:
            if hasattr(self, attr):
                ret.append(getattr(self, attr))

        return ret

    def get_registers(self):
        regs = []
        for attr in self.get_args():
            if is_register(attr):
                regs.append(attr)

        return regs

    def __str__(self):
        ret = self.opcode
        if hasattr(self, 'operand1'):
            ret += ' %s' % self.operand1
        if hasattr(self, 'operand2'):
            ret += ', %s' % self.operand2

        if hasattr(self, 'output1'):
            ret += ' => %s' % self.output1
        if hasattr(self, 'output2'):
            ret += ' %s' % self.output2

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
            self.program.append(Instruction(line))

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
        if target_registers < FEASIBLE_SET:
            raise Exception("Too few registers on target machine")

        register_mappings = {}
        used_registers = self.get_sorted_registers()
        reg_number = FEASIBLE_SET
        for reg in used_registers[0:target_registers - FEASIBLE_SET]:
            register_mappings[reg[0]] = 'r%d' % reg_number
            reg_number += 1

    def __str__(self):
        out = ''
        for line in self.program:
            out += '%s\n' % line
        return out.strip()
