import struct, os, sys

path='game\\Nec.exe'
b=open(path,'rb').read()
def u16(o): return struct.unpack_from('<H',b,o)[0]
def u32(o): return struct.unpack_from('<I',b,o)[0]
pe=u32(0x3c); nsec=u16(pe+6); opt=pe+24; magic=u16(opt)
dd=opt+(96 if magic==0x10b else 112)
rsrc_rva=u32(dd+16); dbg_rva=u32(dd+48)
sh=opt+(224 if magic==0x10b else 240)
sects=[]
for i in range(nsec):
    o=sh+i*40
    vsz,va,rsz,raw=struct.unpack_from('<IIII',b,o+8)
    sects.append((va,vsz,raw,rsz))
def rva2off(rva):
    for va,vsz,raw,rsz in sects:
        if va<=rva<va+max(vsz,rsz): return raw+(rva-va)
base=rva2off(rsrc_rva)
leaves=[]
def walk(o,path_ids):
    nn=u16(base+o+12); ni=u16(base+o+14)
    for i in range(nn+ni):
        e=base+o+16+i*8
        nf,o2=struct.unpack_from('<II',b,e)
        if nf&0x80000000:
            so=base+(nf&0x7fffffff); ln=u16(so); ident=b[so+2:so+2+ln*2].decode('utf-16le')
        else: ident=nf
        if o2&0x80000000: walk(o2&0x7fffffff,path_ids+[ident])
        else:
            e2=base+o2; drva,sz,cp,_=struct.unpack_from('<IIII',b,e2)
            leaves.append((path_ids+[ident],drva,sz))
walk(0,[])

os.makedirs('game/extracted/wav',exist_ok=True)
n=0
for p,rva,sz in leaves:
    if p[0]=='WAVE':
        off=rva2off(rva)
        data=b[off:off+sz]
        fn=f"game/extracted/wav/{p[1]}_{p[2]}.wav"
        open(fn,'wb').write(data)
        hdr=data[:12]
        fmt=struct.unpack_from('<HHIIHH',data,20) if data[:4]==b'RIFF' else None
        print(f"{p[1]}: {sz} bytes  head={hdr}  fmt={fmt}")
        n+=1
print("waves:",n)
