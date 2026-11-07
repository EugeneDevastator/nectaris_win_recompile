import struct
b=open(r'game\\Nec.exe','rb').read()
def u16(o): return struct.unpack_from('<H',b,o)[0]
def u32(o): return struct.unpack_from('<I',b,o)[0]
# menu resource at rva 0x1525c4 -> file offset via sections
pe=u32(0x3c); nsec=u16(pe+6); opt=pe+24; sh=opt+224
sects=[]
for i in range(nsec):
    o=sh+i*40; vsz,va,rsz,raw=struct.unpack_from('<IIII',b,o+8); sects.append((va,vsz,raw,rsz))
def rva2off(rva):
    for va,vsz,raw,rsz in sects:
        if va<=rva<va+max(vsz,rsz): return raw+(rva-va)
off=rva2off(0x1525c4)
data=b[off:off+308]
print("hex:", data.hex())
# parse MENU template
p=0
ver,hsz=struct.unpack_from('<HH',data,p); p=hsz
print("version",hex(ver),"headersize",hsz)
MF_POPUP=0x10; MF_END=0x80; MF_SEPARATOR=0x800; MF_GRAYED=1; MF_CHECKED=8
def parse(p,depth):
    while p < len(data):
        flags,mid=struct.unpack_from('<HH',data,p); p+=4
        if flags & 0x0800:  # separator
            print("  "*depth+"---")
        else:
            # string: zero-terminated, aligned to even
            s=[]
            while data[p]!=0:
                s.append(chr(data[p])); p+=1
            p+=1
            if p%2: p+=1
            print("  "*depth+f"[{mid}] {''.join(s)}  flags=0x{flags:04x}")
        if flags & MF_POPUP:
            p=parse(p,depth+1)
        if flags & MF_END:
            return p
    return p
parse(p,0)
