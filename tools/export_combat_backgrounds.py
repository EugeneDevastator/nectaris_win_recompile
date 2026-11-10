"""Export the combat-simulation terrain backgrounds as RGBA PNGs.

The 24 background panels are 16x16 tilemaps (256x256 px) stored in Nec.exe at
the pointer table 0x431148.  Each panel is drawn from the live combat tile bank
with the combat-sim palette.  A tilemap entry's bit 12 selects the palette bank
(1 + bit12), which is what makes 0x18 render as the dark pad grey in bank 2;
only palette index 0 (sky) is transparent.

Outputs (all identical content):
  assets/art/combat_backgrounds/panel_XX.png   canonical
  assets/art/combat_terrain/panel_XX.png       used by the port
  assets/art/combat_terrain_raw/panel_XX.png   legacy name
  dump/panels_live/panel_XX.png                capture folder
  release/assets/art/combat_{backgrounds,terrain}/panel_XX.png
  port/build/assets/art/combat_{backgrounds,terrain}/panel_XX.png
"""
import os, struct, json
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
EXE = os.path.join(ROOT, 'game', 'Nec.exe')
PALFILE = os.path.join(ROOT, 'dump', 'attack2', 'palette_stable.bin')
TBFILE = os.path.join(ROOT, 'dump', 'tilebank_battle.bin')
BASE_VA, BASE_OFF = 0x42a000, 0x27c00
# Tile entries: low 11 bits = tile index, bit 12 selects the palette bank
# (0 -> bank 1, 1 -> bank 2).  Bit 11 is not a bank bit.
# Bank 2 is what turns palette index 0x18 into the dark road/pad grey
# (123,123,123) and 0x1C into the lighter front-face grey (156,156,156).
# Only palette index 0 (sky / empty) is transparent.
BANK_FLAG = 12

b = open(EXE, 'rb').read()
pal = open(PALFILE, 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
tb = open(TBFILE, 'rb').read()


def foff(va):
    return BASE_OFF + (va - BASE_VA)


class Bits:
    def __init__(self, data, pos):
        self.d = data
        self.p = pos
        self.cur = self.d[self.p]
        self.p += 1
        self.left = 8

    def get(self, n):
        v = 0
        for _ in range(n):
            v = (v << 1) | ((self.cur >> 7) & 1)
            self.cur = (self.cur << 1) & 0xFF
            self.left -= 1
            if self.left == 0:
                self.cur = self.d[self.p] if self.p < len(self.d) else 0
                self.p += 1
                self.left = 8
        return v


def lz(data, start, n_out):
    br = Bits(data, start)
    ring = bytearray([0x20] * 256)
    wr = 0xEF
    out = []
    while len(out) < n_out:
        if br.get(1):
            c = br.get(8)
            out.append(c)
            ring[wr] = c
            wr = (wr + 1) & 0xFF
        else:
            off = br.get(8)
            ln = br.get(4) + 2
            src = off
            for _ in range(ln):
                c = ring[src & 0xFF]
                src += 1
                out.append(c)
                ring[wr] = c
                wr = (wr + 1) & 0xFF
                if len(out) >= n_out:
                    break
    return bytes(out[:n_out])


def render(i):
    raw = lz(b, foff(struct.unpack_from('<I', b, foff(0x431148) + i * 4)[0]), 512)
    img = Image.new('RGBA', (256, 256), (0, 0, 0, 0))
    px = img.load()
    for ty in range(16):
        for tx in range(16):
            e = struct.unpack_from('<H', raw, (ty * 16 + tx) * 2)[0]
            base = (e & 0x7FF) * 256
            bank = 1 + ((e >> BANK_FLAG) & 1)
            if base + 256 > len(tb):
                continue
            for y in range(16):
                for x in range(16):
                    v = tb[base + y * 16 + x]
                    if v == 0:
                        continue
                    idx = ((bank << 4) | (v & 0xF)) & 0xFF
                    # tile bank rows are bottom-up
                    px[tx * 16 + x, ty * 16 + (15 - y)] = PAL[idx] + (255,)
    return img


panels = []
for i in range(24):
    im = render(i)
    panels.append(im)

outdirs = [
    os.path.join(ROOT, 'assets', 'art', 'combat_backgrounds'),
    os.path.join(ROOT, 'assets', 'art', 'combat_terrain'),
    os.path.join(ROOT, 'assets', 'art', 'combat_terrain_raw'),
    os.path.join(ROOT, 'dump', 'panels_live'),
    os.path.join(ROOT, 'release', 'assets', 'art', 'combat_backgrounds'),
    os.path.join(ROOT, 'release', 'assets', 'art', 'combat_terrain'),
    os.path.join(ROOT, 'port', 'build', 'assets', 'art', 'combat_backgrounds'),
    os.path.join(ROOT, 'port', 'build', 'assets', 'art', 'combat_terrain'),
]
for d in outdirs:
    os.makedirs(d, exist_ok=True)
for i, im in enumerate(panels):
    fn = 'panel_%02d.png' % i
    for d in outdirs:
        im.save(os.path.join(d, fn))

# contact sheet over a dark starfield-like backdrop so alpha is easy to check
cols = 6
cell = 260
rows = (len(panels) + cols - 1) // cols
sheet = Image.new('RGB', (cols * cell, rows * cell + 20), (10, 10, 20))
d = ImageDraw.Draw(sheet)
for i, im in enumerate(panels):
    x = (i % cols) * cell + 2
    y = (i // cols) * cell + 18
    checker = Image.new('RGB', (256, 256), (10, 10, 20))
    for cy in range(0, 256, 16):
        for cx in range(0, 256, 16):
            if ((cx // 16) + (cy // 16)) & 1:
                checker.paste((24, 24, 40), (cx, cy, cx + 16, cy + 16))
    checker.paste(im, (0, 0), im)
    sheet.paste(checker, (x, y))
    d.text((x + 2, y - 13), 'panel_%02d' % i, fill=(230, 230, 230))
sheet.save(os.path.join(ROOT, 'assets', 'art', 'combat_backgrounds', 'gallery.png'))
json.dump({'source': 'Nec.exe tilemaps @0x431148 + live combat tilebank',
           'palette': 'dump/attack2/palette_stable.bin (combat sim)',
           'tilebank': 'dump/tilebank_battle.bin', 'transparent_index': 0,
           'palette_bank': '1 + ((entry >> 12) & 1)',
           'panels': 24, 'size': [256, 256]},
          open(os.path.join(ROOT, 'assets', 'art', 'combat_backgrounds', 'manifest.json'), 'w'),
          indent=1)
print('wrote 24 transparent background panels to', outdirs[0])
