"""Score every sprite resource page by how well its sprites match map unit sprites."""
import json, os
import numpy as np

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump', 'art_resources')
M = {r['id']: r for r in json.load(open(os.path.join(D, 'manifest.json')))['resources']}
sb = open(os.path.join(ROOT, 'dump', 'battle_sprites', 'spritebank.bin'), 'rb').read()


def m32(c):
    m = np.zeros((32, 32), dtype=bool)
    a = np.frombuffer(c[:1024], dtype=np.uint8).reshape(32, 32)
    m = a != 0
    return m[::-1]


targets = {i: m32(sb[i * 1024:(i + 1) * 1024]) for i in range(64, 87)}

for rid in sorted(M):
    r = M[rid]
    if r['kind'] != 'sprite':
        continue
    raw = open(os.path.join(D, r['expanded']), 'rb').read()
    n = min(r['units'], len(raw) // 1024)
    hits = 0
    exact = 0
    per = []
    for t in range(n):
        m = m32(raw[t * 1024:(t + 1) * 1024])
        if m.sum() < 40:
            continue
        best = 0
        for ti, tm in targets.items():
            inter = np.logical_and(m, tm).sum()
            union = np.logical_or(m, tm).sum()
            iou = inter / union if union else 0
            best = max(best, iou)
        per.append(best)
        if best > 0.72:
            hits += 1
        if best > 0.98:
            exact += 1
    if per:
        print('res %2d type %d sprites %3d  match>0.72 %3d  exact %3d  max %.2f mean %.2f' %
              (rid, r['type'], n, hits, exact, max(per), sum(per) / len(per)))
