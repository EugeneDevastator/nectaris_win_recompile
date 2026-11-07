b=open(r'game\Nec.exe','rb').read()
base=0x27c00-0x42a000
def d(va,n):
    o=base+va
    return b[o:o+n]
tables=[
 (0x42b140,'? 16b'),
 (0x42b158,'HP-tier damage %'),
 (0x42b160,'terrain defense (16)'),
 (0x42b170,'? word table'),
 (0x42b1a0,'attack vs ground (32)'),
 (0x42b1b8,'attack vs air (32)'),
 (0x42b1d0,'defense (32)'),
 (0x42b1e8,'counter flag ground (32)'),
 (0x42b200,'counter flag air (32)'),
 (0x42b218,'attack rating (32)'),
 (0x42b230,'movement (32)'),
 (0x42b248,'terrain move cost'),
]
for va,name in tables:
    n=32
    print(f"0x{va:06x}  {name}")
    print("   " + " ".join(f"{x:3d}" for x in d(va,n)))
    print("   " + " ".join(f"{x:02x}" for x in d(va,n)))
    print()
