import struct, math, collections
b=open(r'game\Nec.exe','rb').read()
off,size=0x27c00,0x56200
d=b[off:off+size]

def ent(x):
    if not x: return 0
    c=collections.Counter(x)
    n=len(x)
    return -sum((v/n)*math.log2(v/n) for v in c.values())

print("== entropy per 4KB (offset, entropy, zero%) ==")
for i in range(0,len(d),4096):
    blk=d[i:i+4096]
    z=blk.count(0)/len(blk)
    print(f"{off+i:06x}  H={ent(blk):.2f}  zero={z:.2f}")

print("\n== scan for palette-like blocks (1024 bytes, 4th byte constant/zero) ==")
for i in range(0,len(d)-1024):
    # candidate: bytes at i+3, i+7, ... all equal and small
    sample=[d[i+3+4*k] for k in range(0,64)]
    if len(set(sample))==1 and sample[0] < 0x10:
        # verify all 256 alpha bytes
        if all(d[i+3+4*k]==sample[0] for k in range(256)):
            rgb=[(d[i+4*k],d[i+4*k+1],d[i+4*k+2]) for k in range(256)]
            mx=max(max(c) for c in rgb)
            print(f"  off=0x{off+i:06x} alpha={sample[0]} maxcomp={mx} first={rgb[:4]}")

print("\n== scan for LOGPALETTE (WORD count=0x0100 then 256 RGBA, flags byte) ==")
for i in range(0,len(d)-1026):
    if d[i]==0 and d[i+1]==1:
        flags=[d[i+2+4*k+3] for k in range(256)]
        if len(set(flags))<=2:
            print(f"  off=0x{off+i:06x} flagset={set(flags)}")
