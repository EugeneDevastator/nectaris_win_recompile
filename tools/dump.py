import re,struct
b=open(r'game\Nec.exe','rb').read()
print("== 'NEC ' occurrences ==")
for m in re.finditer(rb'NEC.', b):
    off=m.start()
    ctx=b[off:off+40]
    print(f"0x{off:06x}: {ctx!r}")
print("\n== hexdumps ==")
def hx(off,n=192):
    print(f"--- file 0x{off:06x} ---")
    for i in range(0,n,16):
        c=b[off+i:off+i+16]
        print("  %06x  %-48s %s"%(off+i,' '.join(f'{x:02x}' for x in c),''.join(chr(x) if 32<=x<127 else '.' for x in c)))
hx(0x27c00,256)
hx(0x2c2a0,160)
hx(0x41c00,128)
hx(0x65500,128)
