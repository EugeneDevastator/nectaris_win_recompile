"""Sweep palette banks/frames with the LIVE BATTLE palette to identify battle art."""
import json, os
import numpy as np
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
M = {r['id']: r for r in json.load(open(os.path.join(D, 'manifest.json')))['resources']}
palraw = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = np.array([(palraw[4 + i * 4], palraw[5 + i * 4], palraw[6 + i * 4]) for i in range(256)], dtype=np.int16)

FRAMES = ['f10', 'f15', 'f20', 'f25']
frames = {}
for f in FRAMES:
    frames[f] = np.asarray(Image.open(os.path.join(ROOT, 'dump', 'battle_sprites', 'frames', f + '.png')).convert('RGB'), dtype=np.int16)
hists = {}
for f, im in frames.items():
    colors, counts = np.unique(im.reshape(-1, 3), axis=0, return_counts=True)
    hists[f] = {tuple(int(v) for v in c): int(n) for c, n in zip(colors, counts)}


def load_external(rid):
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


def match(cand, mask, frame, hist, x0=36, x1=320, y0=140, y1=340):
    H, W = frame.shape[:2]
    hh, ww = cand.shape[:2]
    ys, xs = np.nonzero(mask)
    if len(ys) < 80:
        return -1.0, 0, 0
    pix = [tuple(int(v) for v in cand[y, x]) for y, x in zip(ys, xs)]
    uniq = {}
    for p in pix:
        uniq[p] = uniq.get(p, 0) + 1
    if len(uniq) < 3:
        return -1.0, 0, 0
    anchor = min(uniq, key=lambda c: hist.get(c, 0) + uniq[c])
    ay = np.array([y for y, x, p in zip(ys, xs, pix) if p == anchor])
    ax = np.array([x for y, x, p in zip(ys, xs, pix) if p == anchor])
    ay = ay[::max(1, len(ay) // 24)]
    ax = ax[::max(1, len(ax) // 24)]
    pos = np.argwhere(np.all(frame == np.array(anchor, dtype=np.int16), axis=2))
    pos = pos[(pos[:, 0] >= y0) & (pos[:, 0] < y1) & (pos[:, 1] >= x0) & (pos[:, 1] < x1)]
    if len(pos) > 3000:
        pos = pos[::len(pos) // 3000 + 1]
    best = (-1.0, 0, 0)
    for py, px in pos:
        top = py - ay[0]; left = px - ax[0]
        if top < 0 or left < 0 or top + hh > H or left + ww > W:
            continue
        yy = top + ay; xx = left + ax
        score = float(np.all(frame[yy, xx] == cand[ay, ax], axis=1).mean())
        if score > best[0]:
            best = (score, int(left), int(top))
    return best


out = []
for rid in (13, 14, 40, 42, 43, 44):
    raw = load_external(rid)
    n = min(M[rid]['units'], len(raw) // 1024)
    for t in range(n):
        for bank in range(16):
            c32, m32 = sprite(raw, t, bank)
            if m32.sum() < 80:
                continue
            cand = np.repeat(np.repeat(c32, 2, axis=0), 2, axis=1)
            mm = np.repeat(np.repeat(m32, 2, axis=0), 2, axis=1)
            best = (-1, None, 0, 0)
            for f in FRAMES:
                sc, x, y = match(cand, mm, frames[f], hists[f])
                if sc > best[0]:
                    best = (sc, f, x, y)
            if best[0] > 0.9:
                line = (round(best[0], 3), rid, t, bank, best[1], best[2], best[3])
                out.append(line)
                print('MATCH', line, flush=True)

out.sort(reverse=True)
json.dump(out, open(os.path.join(ROOT, 'dump', 'battle_sprite_matches_battlepal.json'), 'w'), indent=1)
print('total', len(out))
