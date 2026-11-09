"""Locate combat units in a live frame, then IoU-match against every sprite source."""
import json, os
import numpy as np
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
M = {r['id']: r for r in json.load(open(os.path.join(D, 'manifest.json')))['resources']}
palraw = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'palette.bin'), 'rb').read()
PAL = np.array([(palraw[4 + i * 4], palraw[5 + i * 4], palraw[6 + i * 4]) for i in range(256)], dtype=np.int16)
frame = np.asarray(Image.open(os.path.join(ROOT, 'dump', 'battle_sprites', 'frames', 'f10.png')).convert('RGB'), dtype=np.int16)

# bank-9 colour set
bank9 = {tuple(int(v) for v in PAL[9 * 16 + i]) for i in range(16)} - {(0, 0, 0)}
mask = np.zeros(frame.shape[:2], dtype=bool)
for c in bank9:
    mask |= np.all(frame == np.array(c, dtype=np.int16), axis=2)

# connected components (4-neighbour, simple BFS)
from collections import deque
seen = np.zeros_like(mask)
comps = []
H, W = mask.shape
for y in range(H):
    for x in range(W):
        if mask[y, x] and not seen[y, x]:
            q = deque([(y, x)]); seen[y, x] = True
            ys = []; xs = []
            while q:
                cy, cx = q.popleft(); ys.append(cy); xs.append(cx)
                for dy, dx in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                    ny, nx = cy + dy, cx + dx
                    if 0 <= ny < H and 0 <= nx < W and mask[ny, nx] and not seen[ny, nx]:
                        seen[ny, nx] = True; q.append((ny, nx))
            if len(xs) > 120:
                comps.append((min(xs), min(ys), max(xs), max(ys), len(xs)))
comps.sort(key=lambda c: -c[4])
print('components', comps[:12])

# merge nearby comps into 64x64 unit boxes roughly
boxes = []
for c in comps:
    x0, y0, x1, y1, n = c
    if (x1 - x0) < 16 or (y1 - y0) < 10:
        continue
    boxes.append((x0, y0, x1, y1))
print('boxes', boxes[:12])

# build sprite sources
sources = []  # (label, mask32)
sb = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'spritebank.bin'), 'rb').read()


def m32_from_bytes(chunk):
    m = np.zeros((32, 32), dtype=bool)
    for y in range(32):
        for x in range(32):
            m[y, x] = chunk[y * 32 + x] != 0
    return m[::-1]


for idx in range(1024):
    m = m32_from_bytes(sb[idx * 1024:(idx + 1) * 1024])
    if 40 <= m.sum() <= 900:
        sources.append(('sb%d' % idx, m))
for rid in (13, 14, 23):
    raw = open(os.path.join(D, M[rid]['expanded']), 'rb').read()
    n = min(M[rid]['units'], len(raw) // 1024)
    for t in range(n):
        m = m32_from_bytes(raw[t * 1024:(t + 1) * 1024])
        if 40 <= m.sum() <= 900:
            sources.append(('r%d_%d' % (rid, t), m))

# match each box crop
best_rows = []
for bi, (x0, y0, x1, y1) in enumerate(boxes[:8]):
    cx0 = max(0, x0 - (64 - (x1 - x0)) // 2)
    cy0 = max(0, y0 - (64 - (y1 - y0)) // 2)
    crop = mask[cy0:cy0 + 64, cx0:cx0 + 64]
    if crop.shape != (64, 64):
        continue
    scores = []
    for lab, m in sources:
        mm = np.repeat(np.repeat(m, 2, 0), 2, 1)
        inter = np.logical_and(crop, mm).sum()
        union = np.logical_or(crop, mm).sum()
        iou = inter / union if union else 0
        scores.append((iou, lab, int(m.sum())))
    scores.sort(reverse=True)
    print('BOX', bi, (x0, y0, x1, y1), 'crop@', (cx0, cy0), 'top:', scores[:6])
    best_rows.append((bi, (x0, y0, x1, y1), scores[:10]))

json.dump([(b, list(map(int, box)), s) for b, box, s in best_rows],
          open(os.path.join(ROOT, 'dump', 'unit_shape_matches.json'), 'w'), indent=1)
