"""Overlay sprite-queue unit positions on a captured battle frame.

Validates the queue -> screen mapping.  For a chosen capture sample it reads the
queue JSON, computes each 4-chunk unit's screen position with
  X = (field0 + offset) * 2 - 32
  Y =  field1 * 2 - 32
and draws the group box plus its recent trajectory.  The offset is a parameter
so the right panel mapping can be fitted visually.

Usage:
  python tools/overlay_queue.py CAPTURE_DIR SAMPLE [offset]
"""
import glob
import json
import os
import sys

from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))


def main():
    cap = sys.argv[1]
    sample = int(sys.argv[2])
    offset = int(sys.argv[3]) if len(sys.argv) > 3 else 0
    cap = os.path.join(ROOT, cap) if not os.path.isabs(cap) else cap
    png = None
    for cand in (sample - sample % 8, sample - sample % 4, sample):
        p = os.path.join(cap, 'f%04d.png' % cand)
        if os.path.exists(p):
            png = p
            break
    if not png:
        print('no frame png near', sample)
        return
    im = Image.open(png).convert('RGB')
    dr = ImageDraw.Draw(im)

    # history of group positions (last 80 samples)
    hist = {}
    for i in range(max(0, sample - 80), sample + 1):
        p = os.path.join(cap, 'f%04d.json' % i)
        if not os.path.exists(p):
            continue
        try:
            entries = json.load(open(p))
        except Exception:
            continue
        by = {}
        for e in entries:
            if (e['x'] == 0 and e['y'] == 0):
                continue
            k = (e['bank'], e['pal'], e['sprite'] & ~3)
            g = by.setdefault(k, {'x': [], 'y': []})
            g['x'].append(e['y'])   # field0 = X
            g['y'].append(e['x'])   # field1 = Y
        for k, g in by.items():
            if len(g['x']) < 4:
                continue
            sx = (min(g['x']) + (offset if k[0] else 0)) * 2 - 32 + 32
            sy = min(g['y']) * 2 - 32 + 32
            hist.setdefault(k, []).append((sx, sy))

    colors = [(255, 80, 80), (80, 255, 80), (90, 170, 255), (255, 220, 60),
              (255, 120, 255), (80, 220, 220), (255, 150, 60), (180, 255, 120)]
    for idx, (k, pts) in enumerate(sorted(hist.items())):
        if len(pts) < 3:
            continue
        col = colors[idx % len(colors)]
        if len(pts) > 1:
            dr.line(pts, fill=col, width=2)
        x, y = pts[-1]
        dr.ellipse([x - 4, y - 4, x + 4, y + 4], outline=col, width=2)
        dr.text((x + 6, y - 6), '%d/%d' % (k[1], k[2]), fill=col)
    out = os.path.join(ROOT, 'port', 'build', 'queue_overlay_%d_%d.png' % (sample, offset))
    im.save(out)
    print('wrote', out)


if __name__ == '__main__':
    main()
