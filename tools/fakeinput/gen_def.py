import struct, sys

path = sys.argv[1] if len(sys.argv) > 1 else r'C:\Windows\SysWOW64\winmm.dll'
out  = sys.argv[2] if len(sys.argv) > 2 else 'winmm.def'
b = open(path, 'rb').read()

def u16(o): return struct.unpack_from('<H', b, o)[0]
def u32(o): return struct.unpack_from('<I', b, o)[0]

pe = u32(0x3c); nsec = u16(pe+6); opt = pe+24; magic = u16(opt)
dd = opt + (96 if magic == 0x10b else 112)
exp_rva = u32(dd+0)
sh = opt + (224 if magic == 0x10b else 240)
sects = []
for i in range(nsec):
    o = sh + i*40
    vsz, va, rsz, raw = struct.unpack_from('<IIII', b, o+8)
    sects.append((va, vsz, raw, rsz))

def rva2off(rva):
    for va, vsz, raw, rsz in sects:
        if va <= rva < va+max(vsz, rsz):
            return raw + (rva-va)
    raise ValueError(hex(rva))

e = rva2off(exp_rva)
nfunc  = u32(e+20)
nnames = u32(e+24)
a_names = rva2off(u32(e+32))
a_ords  = rva2off(u32(e+36))

names = []
for i in range(nnames):
    nr = rva2off(u32(a_names + i*4))
    s = b[nr:b.index(b'\0', nr)].decode('ascii')
    names.append(s)

names = sorted(set(names))
with open(out, 'w', encoding='ascii', newline='\r\n') as f:
    f.write('LIBRARY winmm\nEXPORTS\n')
    for n in names:
        f.write(f'  {n}=winmm_sys.{n}\n')
print(f'wrote {out}: {len(names)} exports (func={nfunc} names={nnames})')
