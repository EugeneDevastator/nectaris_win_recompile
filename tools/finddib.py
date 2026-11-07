import struct
b=open(r'game\Nec.exe','rb').read()
def scan(buf, label):
    print(f"== DIB/BITMAPINFOHEADER scan in {label} ==")
    found=[]
    for i in range(0,len(buf)-40):
        if buf[i:i+4] != b'\x28\x00\x00\x00': continue
        w,h,pl,bc=struct.unpack_from('<iiHH',buf,i+4)
        comp,isz=struct.unpack_from('<II',buf,i+16)
        if pl!=1 or bc not in (1,4,8,24,32): continue
        if not (1<=abs(w)<=2048 and 1<=abs(h)<=2048): continue
        if comp not in (0,3): continue
        found.append((i,w,h,bc,comp,isz))
    print(f"  {len(found)} candidates")
    for i,w,h,bc,comp,isz in found[:60]:
        print(f"   off=0x{i:06x} w={w} h={h} bpp={bc} comp={comp} imgsize={isz} (pixels={w*h*bc//8})")
    return found

scan(b[0x27c00:0x27c00+0x56200], ".data")
scan(b, "whole file (limited)")
