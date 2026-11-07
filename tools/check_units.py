import os, glob
for p in glob.glob(r'game\MAPDATA\*\*.NMD'):
    b = open(p,'rb').read()
    if not b.startswith(b'NEC MAP'): continue
    w,h = b[16], b[17]
    off = 19 + w*h
    if off >= len(b): continue
    recs = []
    while off+2 < len(b) and b[off] != 0xff:
        recs.append(b[off:off+3]); off += 3
    if not recs: continue
    bad1f = sum(1 for t,x,y in recs if (x&0x1f) >= w or (y&0x1f) >= h)
    bad0f = sum(1 for t,x,y in recs if (x&0x0f) >= w or (y&0x0f) >= h)
    print(f"{os.path.basename(p):10s} {w:2d}x{h:2d} units={len(recs):2d} "
          f"bad(1f)={bad1f:2d} bad(0f)={bad0f:2d} sample={[ (t&0x1f,x&0x1f,y&0x1f,x&0xf,y&0xf) for t,x,y in recs[:3]]}")
