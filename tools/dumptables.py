import struct
b=open(r'game\Nec.exe','rb').read()
BASE_VA=0x42a000; BASE_OFF=0x27c00
def voff(va): return BASE_OFF+(va-BASE_VA)
def dump(va,n):
    off=voff(va); d=b[off:off+n]
    print(f"--- VA 0x{va:06x} (file 0x{off:x}) ---")
    for i in range(0,n,16):
        print("  %04x  %-47s |%s|"%(i,' '.join('%02x'%x for x in d[i:i+16]),
              ''.join(chr(x) if 32<=x<127 else '.' for x in d[i:i+16])))
# stats area before names
for va in (0x42b1e0,0x42b200,0x42b218,0x42b230,0x42b248,0x42b270,0x42b288):
    dump(va,0x20)
# name table 32*8
print("\n=== 8-byte name records at 0x42b2a0 ===")
for i in range(32):
    va=0x42b2a0+i*8; off=voff(va); raw=b[off:off+8]
    try: s=raw.split(b'\0')[0].decode('cp932')
    except Exception: s=repr(raw)
    print(f"  [{i:2d}] {va:06x} {raw.hex():16s} {s!r}")
