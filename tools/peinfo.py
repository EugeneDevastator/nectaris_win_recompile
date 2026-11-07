import struct, sys, os

def u16(b, o): return struct.unpack_from('<H', b, o)[0]
def u32(b, o): return struct.unpack_from('<I', b, o)[0]

RES_TYPES = {1:'CURSOR',2:'BITMAP',3:'ICON',4:'MENU',5:'DIALOG',6:'STRING',7:'FONTDIR',
             8:'FONT',9:'ACCELERATOR',10:'RCDATA',11:'MESSAGETABLE',12:'GROUP_CURSOR',
             14:'GROUP_ICON',16:'VERSION',17:'DLGINCLUDE',19:'PLUGPLAY',20:'VXD',21:'ANICURSOR',22:'ANIICON',23:'HTML',24:'MANIFEST'}

def main(path):
    b = open(path,'rb').read()
    pe = u32(b,0x3c)
    nsec = u16(b,pe+6)
    opt = pe+24
    magic = u16(b,opt)
    dd = opt + (96 if magic==0x10b else 112)
    rsrc_rva = u32(b,dd+2*8); rsrc_sz = u32(b,dd+2*8+4)
    imp_rva = u32(b,dd+1*8); imp_sz = u32(b,dd+1*8+4)
    dbg_rva = u32(b,dd+6*8); dbg_sz = u32(b,dd+6*8+4)
    sect = []
    sh = opt + (224 if magic==0x10b else 240)
    for i in range(nsec):
        o = sh + i*40
        name = b[o:o+8].rstrip(b'\0').decode('latin1')
        vsz, va, rsz, raw = struct.unpack_from('<IIII', b, o+8)
        sect.append((name,va,vsz,raw,rsz))
    def rva2off(rva):
        for name,va,vsz,raw,rsz in sect:
            if va <= rva < va+max(vsz,rsz):
                return raw + (rva-va)
        return None
    print(f"File: {path}  PE32 magic={magic:04x} nsec={nsec}")
    print(f"rsrc rva={rsrc_rva:x} size={rsrc_sz}  imp rva={imp_rva:x} size={imp_sz}  dbg rva={dbg_rva:x} size={dbg_sz}")
    print("\n== SECTIONS ==")
    for name,va,vsz,raw,rsz in sect:
        print(f"  {name:8} va={va:08x} vsz={vsz:8x} raw={raw:08x} rsz={rsz:8x}")
    # debug
    if dbg_sz:
        o = rva2off(dbg_rva)
        for i in range(dbg_sz//28):
            p = o+i*28
            typ, size, addr, ptr = struct.unpack_from('<IIII', b, p+12)
            if typ == 2:
                sig = b[ptr:ptr+4]
                print(f"\n== CODEVIEW == at file 0x{ptr:x} sig={sig}")
                if sig == b'RSDS':
                    guid = b[ptr+4:ptr+4+16].hex()
                    age = u32(b,ptr+20)
                    name = b[ptr+24:ptr+24+260].split(b'\0')[0].decode('latin1')
                    print(f"  GUID={guid} age={age} pdb={name}")
                elif sig == b'NB10':
                    print(f"  NB10: off={u32(b,ptr+4):x} ts={u32(b,ptr+8):08x} age={u32(b,ptr+12)} pdb={b[ptr+16:ptr+16+260].split(chr(0).encode())[0]}")
    # imports
    print("\n== IMPORTS ==")
    o = rva2off(imp_rva)
    idx = 0
    while True:
        d = o + idx*20
        oft, ts, fc, name_rva, ft = struct.unpack_from('<IIIII', b, d)
        if name_rva == 0: break
        no = rva2off(name_rva)
        dll = b[no:b.index(b'\0',no)].decode('latin1')
        thunk = rva2off(oft if oft else ft)
        names = []
        while True:
            t = u32(b, thunk)
            if t == 0: break
            if t & 0x80000000:
                names.append(f"ord{t & 0xffff}")
            else:
                no2 = rva2off(t)
                names.append(b[no2+2:b.index(b'\0',no2+2)].decode('latin1'))
            thunk += 4
        print(f"  {dll}: {', '.join(names)}")
        idx += 1
    # resources
    if not rsrc_rva: 
        print("\nNo resources"); return
    print("\n== RESOURCES ==")
    base = rva2off(rsrc_rva)
    leaves = []
    def walk(off, depth, path_ids):
        # directory: characteristics, timestamp, major, minor, named, id, each entry 8 bytes
        nnamed = u16(b, base+off+12); nid = u16(b, base+off+14)
        n = nnamed + nid
        for i in range(n):
            e = base+off+16+i*8
            namefield, off2 = struct.unpack_from('<II', b, e)
            ident = None
            if namefield & 0x80000000:
                so = base + (namefield & 0x7fffffff)
                ln = u16(b, so)
                ident = b[so+2:so+2+ln*2].decode('utf-16le', 'replace')
            else:
                ident = namefield
            if off2 & 0x80000000:
                walk(off2 & 0x7fffffff, depth+1, path_ids+[ident])
            else:
                e2 = base + off2
                data_rva, size, cp, res = struct.unpack_from('<IIII', b, e2)
                leaves.append((path_ids+[ident], data_rva, size))
    walk(0, 0, [])
    from collections import defaultdict
    bytype = defaultdict(list)
    for p, rva, size in leaves:
        bytype[p[0]].append((p[1:], rva, size))
    total = 0
    for t in sorted(bytype, key=lambda x: (str(type(x)), str(x))):
        tn = t
        if isinstance(t, int): tn = RES_TYPES.get(t, f'#{t}')
        items = bytype[t]
        print(f"  {tn}  ({len(items)} entries, {sum(s for _,_,s in items)} bytes)")
        total += sum(s for _,_,s in items)
        for sub, rva, size in items[:40]:
            names = '/'.join(str(x) for x in sub)
            print(f"      [{names}] rva={rva:08x} size={size}")
        if len(items) > 40: print(f"      ... {len(items)-40} more")
    print(f"  TOTAL resource bytes: {total}")

main(sys.argv[1])
