import os, sys, struct, ctypes
sys.path.insert(0, os.path.join(os.path.dirname(__file__)))
import dump_mem as dm
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
# find the Nectaris process
import subprocess
pid = None
out = subprocess.check_output(['tasklist', '/FI', 'IMAGENAME eq Nec.exe', '/FO', 'CSV', '/NH'], text=True)
for line in out.splitlines():
    parts = [p.strip('"') for p in line.split('","')]
    if parts and parts[0].lower() == 'nec.exe':
        pid = int(parts[1]); break
print('pid', pid)
h = dm.k32.OpenProcess(0x1F0FFF, False, pid)
base = dm.module_base(h)
def u8(va): return dm.read(h, base, va, 1)[0]
def u32(va): return struct.unpack('<I', dm.read(h, base, va, 4))[0]
print('atk', u8(0x48327b), 'def', u8(0x483274), 'type', u8(0x48327c),
      'ta', u8(0x481db3), 'td', u8(0x481db4), 'ind', u8(0x481dbc),
      'anim', u8(0x483490), 'tick', u8(0x483492))
pal = open(os.path.join(ROOT, 'dump', 'palette_bgr.bin'), 'rb').read()
PALR = bytearray(768)
for i in range(256):
    PALR[i*3] = pal[i*4+2]; PALR[i*3+1] = pal[i*4+1]; PALR[i*3+2] = pal[i*4]
sp = u32(0x480f18)
data = dm.read_abs(h, sp, 704*544)
img = Image.frombuffer('P', (704, 544), data, 'raw', 'P', 0, 1)
img = img.transpose(Image.FLIP_TOP_BOTTOM).crop((32, 32, 672, 512))
img.putpalette(bytes(PALR))
img.convert('RGB').save(os.path.join(ROOT, 'ref', 'replay', 'now.png'))
print('saved now.png')
dm.k32.CloseHandle(h)
