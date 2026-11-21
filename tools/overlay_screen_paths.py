"""Track battle-sprite blobs on screen frames and draw their paths.

This is a capture-validation view that does not depend on the queue -> screen
mapping: it diffs each frame against an empty reference, finds sprite blobs and
draws the trajectories as lines on a chosen frame.

Usage:
  python tools/overlay_screen_paths.py CAPTURE_DIR [--panel left|both] [--out x.png]
"""
import argparse
import glob
import os
import sys
from collections import deque

import numpy as np
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
PANELS = {'left': (64, 96, 320, 352), 'right': (320, 96, 576, 352),
          'both': (64, 96, 576, 352)}


def blobs(mask, min_px=30):
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
                            min(xs), min(ys), max(xs), max(ys)))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('capture')
    ap.add_argument('--panel', default='both')
    ap.add_argument('--out', default='')
    ap.add_argument('--ref', type=int, default=0)
    ap.add_argument('--from', dest='f0', type=int, default=0)
    ap.add_argument('--to', dest='f1', type=int, default=400)
    args = ap.parse_args()

    cap = os.path.join(ROOT, args.capture) if not os.path.isabs(args.capture) else args.capture
    files = sorted(glob.glob(os.path.join(cap, 'f*.png')))
    if not files:
        print('no frames')
        return
    x0, y0, x1, y1 = PANELS[args.panel]
    ref = np.asarray(Image.open(files[min(args.ref, len(files) - 1)]).convert('RGB'),
                     dtype=np.int16)[y0:y1, x0:x1]

    tracks = []      # list of [last_centroid, points]
    frames_used = []
    for i, f in enumerate(files):
        # sample index encoded in the file name (f0000.png -> 0)
        try:
            idx = int(os.path.basename(f)[1:5])
        except ValueError:
            idx = i
        if idx < args.f0 or idx > args.f1:
            continue
        im = np.asarray(Image.open(f).convert('RGB'), dtype=np.int16)[y0:y1, x0:x1]
        mask = np.abs(im - ref).sum(axis=2) > 60
        bs = blobs(mask)
        frames_used.append((idx, f, im, bs))
        for b in bs:
            c = (b[0], b[1])
            best = None
            bestd = 60.0
            for tr in tracks:
                d = ((tr[0][0] - c[0]) ** 2 + (tr[0][1] - c[1]) ** 2) ** 0.5
                if d < bestd:
                    bestd = d
                    best = tr
            if best is None:
                tracks.append([c, [c]])
            else:
                best[0] = c
                best[1].append(c)

    if not frames_used:
        print('no frames in range')
        return
    idx, f, im, bs = frames_used[-1]
    img = Image.fromarray(im.astype(np.uint8))
    dr = ImageDraw.Draw(img)
    colors = [(255, 80, 80), (80, 255, 80), (90, 170, 255), (255, 220, 60),
              (255, 120, 255), (80, 220, 220), (255, 150, 60), (180, 255, 120)]
    long_tracks = [t for t in tracks if len(t[1]) >= 4]
    for n, tr in enumerate(long_tracks):
        col = colors[n % len(colors)]
        dr.line([(p[0], p[1]) for p in tr[1]], fill=col, width=2)
        dr.ellipse([tr[1][-1][0] - 3, tr[1][-1][1] - 3,
                    tr[1][-1][0] + 3, tr[1][-1][1] + 3], outline=col, width=2)
    out = args.out or os.path.join(ROOT, 'port', 'build', 'screen_paths.png')
    img.save(out)
    print('tracks', len(long_tracks), 'frames', len(frames_used), '->', out)


if __name__ == '__main__':
    main()
