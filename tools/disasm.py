import pefile
import capstone
import sys

pe = pefile.PE('game/Nec.exe')
data = pe.get_memory_mapped_image()
base = 0x400000
start = int(sys.argv[1], 16)
end = int(sys.argv[2], 16)
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
for ins in md.disasm(data[start - base:end - base], start):
    print('%08x  %-16s %s %s' % (ins.address, ins.bytes.hex(), ins.mnemonic, ins.op_str))
