"""Match candidate resource sprites against a captured battle frame.

Renders each 32x32 candidate with palette bank 9/10, scales 2x, and slides it
over the battlefield area of a live framebuffer capture, scoring exact colour
agreement on non-transparent pixels.
"""
import json, os, sys
import numpy as np
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
M = {r['id']: r for r in json.load(open(os.path.join(D, 'manifest.json')))['resources']}
palraw = open(os.path.join(D, 'palette.bin'), 'rb').read()
PAL = np.array([(palraw[4 + i * 4], palraw[5 + i * 4], palraw[6 + i * 4]) for i in range(256)], dtype=np.int16)


def load(rid):
    r = M[rid]
    raw = open(os.path.join(D, r['expanded']), 'rb').read()
    return raw


def sprite_rgb(raw, t, bank):
    """Return (32x32x3 int array, mask) for sprite t."""
    out = np.zeros((32, 32, 3), dtype=np.int16)
    mask = np.zeros((32, 32), dtype=bool)
    for y in range(32):
        for x in range(32):
            v = raw[t * 1024 + y * 32 + x]
            if v:
                out[y, x] = PAL[((bank << 4) | (v & 15)) & 255]
                mask[y, x] = True
    # stored bottom-up
    return out[::-1], mask[::-1]


frame = np.asarray(Image.open(os.path.join(ROOT, 'dump', 'battle_sprites', 'frames', 'f25.png')).convert('RGB'), dtype=np.int16)
H, W, _ = frame.shape

# battlefield window
x0, x1, y0, y1 = 40, 300, 150, 320


def best_score(cand, cmask):
    hh, ww = cand.shape[:2]
    best = (-1.0, 0, 0)
    ys = range(y0, min(y1, H - hh))
    xs = range(x0, min(x1, W - ww))
    for y in ys:
        for x in xs:
            sub = frame[y:y + hh, x:x + ww]
            eq = np.all(sub == cand, axis=2)
            score = float((eq & cmask).sum()) / float(cmask.sum())
            if score > best[0]:
                best = (score, x, y)
    return best


results = []
for rid in (13, 14, 15, 16, 40, 42, 43, 44, 2, 3, 53, 54, 55):
    raw = load(rid)
    n = M[rid]['units']
    for t in range(n):
        for bank in (9, 10):
            cand32, m32 = sprite_rgb(raw, t, bank)
            if m32.sum() < 60:
                continue
            cand = np.repeat(np.repeat(cand32, 2, axis=0), 2, axis=1)
            cmask = np.repeat(np.repeat(m32, 2, axis=0), 2, axis=1)
            sc, x, y = best_score(cand, cmask)
            if sc > 0.80:
                results.append((sc, rid, t, bank, x, y))

results.sort(reverse=True)
print('matches >0.80:', len(results))
for r in results[:60]:
    print('score %.3f res %d sprite %d bank %d at %d,%d' % r)

json.dump(results, open(os.path.join(ROOT, 'dump', 'battle_sprite_matches.json'), 'w'), indent=1)
