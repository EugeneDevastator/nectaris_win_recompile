"""Track battle-screen sprite blobs across captured frames.

Uses dump/combat_full/frames (one PNG per frame).  The panel backgrounds are
static, so diffing against an empty frame isolates the moving unit sprites.
Prints centroid + bbox per connected blob so entry paths can be compared.
"""
import glob
import os
import sys
from collections import deque

import numpy as np
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
FRAMES = sorted(glob.glob(os.path.join(ROOT, 'dump', 'combat_full', 'frames', '*.png')))
PANELS = {'left': (64, 144, 320, 400), 'right': (320, 144, 576, 400)}


def blobs(mask, min_px=24):
    h, w = mask.shape
    seen = np.zeros_like(mask, dtype=bool)
    out = []
    for y in range(h):
        for x in range(w):
            if not mask[y, x] or seen[y, x]:
                continue
            q = deque([(x, y)])
            seen[y, x] = True
            pts = []
            while q:
                cx, cy = q.popleft()
                pts.append((cx, cy))
                for nx, ny in ((cx + 1, cy), (cx - 1, cy), (cx, cy + 1), (cx, cy - 1)):
                    if 0 <= nx < w and 0 <= ny < h and mask[ny, nx] and not seen[ny, nx]:
                        seen[ny, nx] = True
                        q.append((nx, ny))
            if len(pts) >= min_px:
                xs = [p[0] for p in pts]
                ys = [p[1] for p in pts]
                out.append((sum(xs) / len(xs), sum(ys) / len(ys),
                            min(xs), min(ys), max(xs), max(ys), len(pts)))
    return sorted(out, key=lambda b: (b[1], b[0]))


def main():
    which = sys.argv[1] if len(sys.argv) > 1 else 'left'
    f0, f1 = (int(sys.argv[2]), int(sys.argv[3])) if len(sys.argv) > 3 else (4, 60)
    x0, y0, x1, y1 = PANELS[which]
    ref = np.asarray(Image.open(FRAMES[f0]).convert('RGB'), dtype=np.int16)[y0:y1, x0:x1]
    for i in range(f0, min(f1, len(FRAMES))):
        im = np.asarray(Image.open(FRAMES[i]).convert('RGB'), dtype=np.int16)[y0:y1, x0:x1]
        diff = np.abs(im - ref).sum(axis=2)
        mask = diff > 40
        bs = blobs(mask)
        if not bs:
            continue
        parts = ' | '.join('%4.0f,%4.0f %2dx%2d' % (b[0], b[1], b[4] - b[2] + 1, b[5] - b[3] + 1)
                           for b in bs)
        print('%3d  n=%d  %s' % (i, len(bs), parts))


if __name__ == '__main__':
    main()
