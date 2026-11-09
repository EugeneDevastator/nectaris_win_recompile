"""Fast template match: find which decoded sprite pages appear in a battle frame."""
import json, os
import numpy as np
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
M = {r['id']: r for r in json.load(open(os.path.join(D, 'manifest.json')))['resources']}
palraw = open(os.path.join(D, 'palette.bin'), 'rb').read()
PAL = np.array([(palraw[4 + i * 4], palraw[5 + i * 4], palraw[6 + i * 4]) for i in range(256)], dtype=np.int16)

frame = np.asarray(Image.open(os.path.join(ROOT, 'dump', 'battle_sprites', 'frames', 'f25.png')).convert('RGB'), dtype=np.int16)
H, W, _ = frame.shape
# precompute colour -> list of (y,x)
colour_index = {}
colors, counts = np.unique(frame.reshape(-1, 3), axis=0, return_counts=True)
for c, n in zip(colors, counts):
    colour_index[tuple(int(v) for v in c)] = int(n)


def load(rid):
    return open(os.path.join(D, M[rid]['expanded']), 'rb').read()


def sprite(raw, t, bank):
    out = np.zeros((32, 32, 3), dtype=np.int16)
    mask = np.zeros((32, 32), dtype=bool)
    for y in range(32):
        for x in range(32):
            v = raw[t * 1024 + y * 32 + x]
            if v:
                out[y, x] = PAL[((bank << 4) | (v & 15)) & 255]
                mask[y, x] = True
    return out[::-1], mask[::-1]


def match(cand, mask, x0=40, x1=300, y0=150, y1=330):
    hh, ww = cand.shape[:2]
    ys, xs = np.nonzero(mask)
    if len(ys) < 40:
        return -1, 0, 0
    pix = [tuple(int(v) for v in cand[y, x]) for y, x in zip(ys, xs)]
    # anchor: rarest candidate colour
    uniq = {}
    for p in pix:
        uniq[p] = uniq.get(p, 0) + 1
    anchor = min(uniq, key=lambda c: colour_index.get(c, 0) + uniq[c])
    ay, ax = zip(*[(y, x) for y, x, p in zip(ys, xs, pix) if p == anchor])
    ay = np.array(ay); ax = np.array(ax)
    # all frame positions of anchor colour
    pos = np.argwhere(np.all(frame == np.array(anchor, dtype=np.int16), axis=2))
    pos = pos[(pos[:, 0] >= y0) & (pos[:, 0] < y1) & (pos[:, 1] >= x0) & (pos[:, 1] < x1)]
    best = (-1.0, 0, 0)
    # subsample anchor offsets to keep the check cheap
    step = max(1, len(ay) // 24)
    ay = ay[::step]; ax = ax[::step]
    for py, px in pos:
        top = py - ay[0]
        left = px - ax[0]
        if top < 0 or left < 0 or top + hh > H or left + ww > W:
            continue
        yy = top + ay; xx = left + ax
        ok = np.all(frame[yy, xx] == cand[ay, ax], axis=1)
        score = ok.mean()
        if score > best[0]:
            best = (float(score), int(left), int(top))
    return best


results = []
for rid in (13, 14, 15, 16, 40, 42, 43, 44, 2, 3, 53, 54, 55, 56):
    raw = load(rid)
    n = min(M[rid]['units'], len(raw) // 1024)
    for t in range(n):
        for bank in (9, 10):
            c32, m32 = sprite(raw, t, bank)
            if m32.sum() < 80:
                continue
            cand = np.repeat(np.repeat(c32, 2, axis=0), 2, axis=1)
            mm = np.repeat(np.repeat(m32, 2, axis=0), 2, axis=1)
            sc, x, y = match(cand, mm)
            if sc > 0.85:
                results.append((round(sc, 3), rid, t, bank, x, y))
                print('MATCH %.3f res %d sprite %d bank %d at %d,%d' % (sc, rid, t, bank, x, y), flush=True)

results.sort(reverse=True)
json.dump(results, open(os.path.join(ROOT, 'dump', 'battle_sprite_matches.json'), 'w'), indent=1)
print('total', len(results))
