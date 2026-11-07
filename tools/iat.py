import struct
b=open(r'game\Nec.exe','rb').read()
def u16(o): return struct.unpack_from('<H',b,o)[0]
def u32(o): return struct.unpack_from('<I',b,o)[0]
pe=u32(0x3c); nsec=u16(pe+6); opt=pe+24; magic=u16(opt)
dd=opt+(96 if magic==0x10b else 112)
imp_rva=u32(dd+8); imp_sz=u32(dd+12)
sh=opt+(224 if magic==0x10b else 240)
sects=[]
for i in range(nsec):
    o=sh+i*40; vsz,va,rsz,raw=struct.unpack_from('<IIII',b,o+8); sects.append((va,vsz,raw,rsz))
def rva2off(rva):
    for va,vsz,raw,rsz in sects:
        if va<=rva<va+max(vsz,rsz): return raw+(rva-va)
o=rva2off(imp_rva); base=0x400000
idx=0
want={'GetKeyState','GetAsyncKeyState','GetKeyboardState','GetCursorPos','SetCursorPos','ScreenToClient','ClientToScreen','GetMessageA','PeekMessageA','GetKeyState'}
while True:
    d=o+idx*20
    oft,ts,fc,name_rva,ft=struct.unpack_from('<IIIII',b,d)
    if name_rva==0: break
    no=rva2off(name_rva); dll=b[no:b.index(b'\0',no)].decode()
    thunk=rva2off(oft if oft else ft)
    iat_va=base+ft
    i=0
    while True:
        t=u32(thunk)
        if t==0: break
        if t&0x80000000:
            nm=f"ord{t&0xffff}"
        else:
            no2=rva2off(t); nm=b[no2+2:b.index(b'\0',no2+2)].decode()
        if nm in want:
            print(f"{dll:14} {nm:18} IAT=0x{iat_va+i*4:08x}")
        thunk+=4; i+=1
    idx+=1
