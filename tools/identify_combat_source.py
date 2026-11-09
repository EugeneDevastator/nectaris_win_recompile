"""Identify whether the combat screen units are res13 (map) or res14 (rotations)."""
import os, json
import numpy as np
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
M = {r['id']: r for r in json.load(open(os.path.join(D, 'manifest.json')))['resources']}
pal = open(os.path.join(ROOT, 'dump', 'ai_turn2', 'palette.bin'), 'rb').read()
PAL = np.array([(pal[4 + i * 4], pal[5 + i * 4], pal[6 + i * 4]) for i in range(256)], dtype=np.int16)
frame = np.asarray(Image.open(os.path.join(ROOT, 'dump', 'ai_turn2', 'f176.png')).convert('RGB'), dtype=np.int16)

# locate bright blue unit pixels
cols = [tuple(int(v) for v in PAL[9 * 16 + i]) for i in (1, 2, 3)]
mask = np.zeros(frame.shape[:2], bool)
for c in cols:
    mask |= np.all(frame == np.array(c, dtype=np.int16), axis=2)

from scipy import ndimage
lab, n = ndimage.label(mask, structure=np.ones((3, 3)))
boxes = []
for i in range(1, n + 1):
    ys, xs = np.nonzero(lab == i)
    if len(xs) > 60:
        boxes.append((xs.min(), ys.min(), xs.max(), ys.max(), len(xs)))
boxes.sort(key=lambda b: -b[4])
print('boxes', boxes[:10])


def sprite_rgb(raw, t, bank):
    im = np.zeros((32, 32, 3), dtype=np.int16)
    m = np.zeros((32, 32), bool)
    for y in range(32):
        for x in range(32):
            v = raw[t * 1024 + y * 32 + x]
            if v:
                im[y, x] = PAL[((bank << 4) | (v & 15)) & 255]
                m[y, x] = True
    return im[::-1], m[::-1]


def best_in(crop, cand, cm):
    hh, ww = cand.shape[:2]
    best = (-1, 0, 0)
    H, W = crop.shape[:2]
    for y in range(0, H - hh + 1):
        for x in range(0, W - ww + 1):
            sub = crop[y:y + hh, x:x + ww]
            eq = np.all(sub == cand, axis=2)
            score = (eq & cm).sum() / max(1, cm.sum())
            if score > best[0]:
                best = (float(score), x, y)
    return best


x0, y0, x1, y1, _ = boxes[0]
cx = max(0, (x0 + x1) // 2 - 24)
cy = max(0, (y0 + y1) // 2 - 24)
crop = frame[cy:cy + 48, cx:cx + 48]
Image.fromarray(crop.astype('uint8')).resize((192, 192), Image.NEAREST).save(
    os.path.join(ROOT, 'dump', 'combat_unit_crop.png'))

for rid in (13, 14):
    raw = open(os.path.join(D, M[rid]['expanded']), 'rb').read()
    n = min(M[rid]['units'], len(raw) // 1024)
    scores = []
    for t in range(n):
        c, m = sprite_rgb(raw, t, 9)
        if m.sum() < 80:
            continue
        s, x, y = best_in(crop, c, m)
        scores.append((s, t, x, y))
    scores.sort(reverse=True)
    print('res', rid, 'top', [(round(s, 3), t, x, y) for s, t, x, y in scores[:6]])
