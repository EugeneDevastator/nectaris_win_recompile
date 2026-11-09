"""Package the art pages extracted from live memory into editable assets.

Inputs:
  dump/art_resources/manifest.json + expanded tile/sprite banks
  dump/boot_mem/*.png             intro/title screens from the 8bpp framebuffer
  dump/battlecap/f009..f024.png   real combat frames from the framebuffer
  port/build/assets/art/combat_units (battle unit compositions)

Outputs:
  assets/art/extracted_resources/  resource pages (palette variants)
  assets/art/cutscenes/intro/      key intro/title frames
  assets/art/battle_animation/     combat materialisation frames
  assets/art/combat_units/         battle unit sprites
  assets/art/combat_terrain_raw/   raw transparent combat panels
  assets/data/art_resources.json   manifest
  assets/data/battle_palette_cycle.json
"""
import os, sys, json, shutil, struct
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
SRC = os.path.join(ROOT, 'dump', 'art_resources')
OUT = os.path.join(ROOT, 'assets', 'art')
RES = os.path.join(OUT, 'extracted_resources')
os.makedirs(RES, exist_ok=True)
M = json.load(open(os.path.join(SRC, 'manifest.json'), encoding='utf-8'))['resources']
pal = open(os.path.join(SRC, 'palette.bin'), 'rb').read()
PAL = [(pal[4+i*4], pal[5+i*4], pal[6+i*4]) for i in range(256)]


def render_tiles(raw, count, bank, scale=2, cols=16):
    tw = th = 16
    rows = max(1, (count + cols - 1) // cols)
    im = Image.new('RGBA', (cols*tw*scale, rows*th*scale), (0,0,0,0))
    px = im.load()
    for t in range(count):
        b = t*256
        if b+256 > len(raw): break
        sx = (t % cols)*tw*scale; sy = (t//cols)*th*scale
        for y in range(16):
            for x in range(16):
                v = raw[b+y*16+x]
                if not v: continue
                r,g,bl = PAL[(bank << 4) | (v & 0xF)]
                for yy in range(scale):
                    for xx in range(scale):
                        px[sx+x*scale+xx, sy+(15-y)*scale+yy] = (r,g,bl,255)
    return im


def render_sprites(raw, count, bank, scale=2, cols=8):
    tw = th = 32
    rows = max(1, (count+cols-1)//cols)
    im = Image.new('RGBA', (cols*tw*scale, rows*th*scale), (0,0,0,0))
    px = im.load()
    for t in range(count):
        b = t*1024
        if b+1024 > len(raw): break
        sx = (t % cols)*tw*scale; sy = (t//cols)*th*scale
        for y in range(32):
            for x in range(32):
                v = raw[b+y*32+x]
                if not v: continue
                r,g,bl = PAL[(bank << 4) | (v & 0xF)]
                for yy in range(scale):
                    for xx in range(scale):
                        px[sx+x*scale+xx, sy+(31-y)*scale+yy] = (r,g,bl,255)
    return im


exports = []
for r in M:
    raw = open(os.path.join(SRC, r['expanded']), 'rb').read()
    banks = [6, 9] if r['kind'] == 'tile' else [9, 10]
    files = []
    for bank in banks:
        im = (render_tiles(raw, r['units'], bank) if r['kind'] == 'tile'
              else render_sprites(raw, r['units'], bank))
        fn = 'res_%02d_%s_bank%02d.png' % (r['id'], r['kind'], bank)
        im.save(os.path.join(RES, fn))
        files.append(fn)
    r2 = dict(r)
    r2['exports'] = files
    exports.append(r2)

# --- cutscene frames from the memory framebuffer ---
CUT = os.path.join(OUT, 'cutscenes', 'intro')
os.makedirs(CUT, exist_ok=True)
boot = os.path.join(ROOT, 'dump', 'boot_mem')
key_frames = [4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60,
              64, 68, 72, 76, 80, 88, 96, 104, 114, 118, 122, 126, 130]
for n in key_frames:
    src = os.path.join(boot, 'f_%03d.png' % n)
    if os.path.exists(src):
        shutil.copy2(src, os.path.join(CUT, 'intro_%03d.png' % n))

# --- battle animation frames from the memory framebuffer ---
BAT = os.path.join(OUT, 'battle_animation')
os.makedirs(BAT, exist_ok=True)
bc = os.path.join(ROOT, 'dump', 'battlecap')
for n in range(9, 25):
    src = os.path.join(bc, 'f%03d.png' % n)
    if os.path.exists(src):
        im = Image.open(src)
        # keep the 512x480 playfield area used by the combat screen
        if im.size[0] >= 640:
            im = im.crop((64, 96, 576, 480))
        im.save(os.path.join(BAT, 'combat_%03d.png' % n))

# --- battle unit art already composed from spritebank_battle ---
src_units = os.path.join(ROOT, 'port', 'build', 'assets', 'art', 'combat_units')
dst_units = os.path.join(OUT, 'combat_units')
if os.path.isdir(src_units):
    os.makedirs(dst_units, exist_ok=True)
    for fn in os.listdir(src_units):
        if fn.lower().endswith('.png'):
            shutil.copy2(os.path.join(src_units, fn), os.path.join(dst_units, fn))

# --- raw combat panels with alpha ---
src_pan = os.path.join(ROOT, 'dump', 'panels_live')
dst_pan = os.path.join(OUT, 'combat_terrain_raw')
if os.path.isdir(src_pan):
    os.makedirs(dst_pan, exist_ok=True)
    for fn in os.listdir(src_pan):
        if fn.lower().endswith('.png'):
            shutil.copy2(os.path.join(src_pan, fn), os.path.join(dst_pan, fn))

# --- battle palette cycle tables (static .data) ---
EXE = os.path.join(ROOT, 'game', 'Nec.exe')
data = open(EXE, 'rb').read()
ib = 0x400000
secs = [(0x1000,0x27400,0x400,0x27400),(0x2a000,0x6c064,0x27c00,0x56200)]
def off(va):
    rva = va - ib
    for sva,vs,rp,rs in secs:
        if sva <= rva < sva+max(vs,rs):
            return rp + (rva-sva)
    raise ValueError(hex(va))
def table(va, n=128):
    o = off(va)
    return list(struct.unpack('<%dH' % n, data[o:o+n*2]))
cycle = {
    'tables': {
        'red': table(0x432310),
        'green': table(0x432390),
        'blue': table(0x432410),
    },
    'frames': 8,
    'note': 'Original battle animation recolours the unit palette; see FUN_004164b0.',
}
json.dump(cycle, open(os.path.join(ROOT, 'assets', 'data', 'battle_palette_cycle.json'), 'w'), indent=1)

# --- manifest ---
manifest = {
    'source': 'live Nec.exe process memory (resource decoder FUN_0040cd80)',
    'scratch_buffer': '0x484730 (0x20000 bytes)',
    'resource_pages': exports,
    'cutscene_frames': sorted(os.listdir(CUT)),
    'battle_animation_frames': sorted(os.listdir(BAT)),
    'battle_units': sorted(os.listdir(dst_units)) if os.path.isdir(dst_units) else [],
    'combat_panels_raw': sorted(os.listdir(dst_pan)) if os.path.isdir(dst_pan) else [],
}
json.dump(manifest, open(os.path.join(ROOT, 'assets', 'data', 'art_resources.json'), 'w'), indent=1)
print('resource pages:', len(exports))
print('cutscene frames:', len(manifest['cutscene_frames']))
print('battle frames:', len(manifest['battle_animation_frames']))
print('battle units:', len(manifest['battle_units']))
print('wrote', OUT)
