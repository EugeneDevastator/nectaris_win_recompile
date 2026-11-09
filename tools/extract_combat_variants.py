"""Extract and label the generated combat-unit rotation variants from combat captures."""
import os, json, hashlib, collections
import numpy as np
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'ai_turn2')
OUT = os.path.join(ROOT, 'dump', 'combat_variants')
os.makedirs(OUT, exist_ok=True)
pal = open(os.path.join(D, 'palette.bin'), 'rb').read()
PAL = [(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)]
BANK = 9

units = json.load(open(os.path.join(ROOT, 'assets', 'data', 'units.json'), encoding='utf-8'))['units']
names = {u['id']: u['name'] for u in units}


def mask_from(sb, idx):
    a = np.frombuffer(sb[idx * 1024:(idx + 1) * 1024], dtype=np.uint8).reshape(32, 32)
    return (a != 0)[::-1]


def iou(a, b):
    u = np.logical_or(a, b).sum()
    return np.logical_and(a, b).sum() / u if u else 0.0


def render(sb, idx):
    im = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    px = im.load()
    for y in range(32):
        for x in range(32):
            v = sb[idx * 1024 + y * 32 + x]
            if v:
                px[x, 31 - y] = PAL[((BANK << 4) | (v & 15)) & 255] + (255,)
    return im


# collect unique spritebank states
states = {}
for f in sorted(os.listdir(D)):
    if f.startswith('sb_') and f.endswith('.bin'):
        sb = open(os.path.join(D, f), 'rb').read()
        h = hashlib.md5(sb[128 * 1024:160 * 1024]).hexdigest()[:8]
        states.setdefault(h, (f, sb))
print('unique 128..159 states', len(states))

# map sprites for identification
ref_sb = next(iter(states.values()))[1]
targets = {i: mask_from(ref_sb, i) for i in range(64, 87)}

groups = {}
for h, (f, sb) in states.items():
    for slot in range(128, 160, 4):
        ms = [mask_from(sb, slot + k) for k in range(4)]
        if sum(m.sum() for m in ms) < 200:
            continue
        votes = collections.Counter()
        for m in ms:
            if m.sum() < 30:
                continue
            best = max(((iou(m, t), ti) for ti, t in targets.items()))
            if best[0] > 0.55:
                votes[best[1]] += 1
        unit = votes.most_common(1)[0][0] if votes else None
        # dedupe by pixel content
        gkey = hashlib.md5(b''.join(sb[(slot + k) * 1024:(slot + k + 1) * 1024] for k in range(4))).hexdigest()[:10]
        groups.setdefault(gkey, (slot, sb, unit, h))

print('unique rotation groups', len(groups))
items = sorted(groups.values(), key=lambda g: (g[2] is None, g[2] or 0))
cols = 8
cellw, cellh = 4 * 140 + 8, 170
rows = (len(items) + cols - 1) // cols
sheet = Image.new('RGB', (cols * cellw, rows * cellh), (18, 18, 26))
d = ImageDraw.Draw(sheet)
manifest = []
for i, (slot, sb, unit, h) in enumerate(items):
    if unit is None:
        continue
    label = names.get(unit - 64, '?')
    gx = (i % cols) * cellw
    gy = (i // cols) * cellh
    d.text((gx + 4, gy + 18), 'unit %d %s' % (unit - 64, label), fill=(120, 255, 120))
    files = []
    for rot in range(4):
        im = render(sb, slot + rot).resize((128, 128), Image.NEAREST)
        sheet.paste(im, (gx + 4 + rot * 140, gy + 40), im)
        fn = 'unit%02d_%s_rot%d.png' % (unit - 64, ''.join(c if c.isalnum() else '_' for c in label), rot)
        render(sb, slot + rot).save(os.path.join(OUT, fn))
        files.append(fn)
    manifest.append({'unit': unit - 64, 'name': label, 'source_slot': slot, 'files': files})
sheet.save(os.path.join(OUT, 'contact.png'))
json.dump(manifest, open(os.path.join(OUT, 'manifest.json'), 'w'), indent=1)
print('wrote', OUT, 'groups', len(manifest))
