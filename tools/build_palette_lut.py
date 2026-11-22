"""Build per-unit blue -> green palette LUTs from the captured frame art.

Every unit's combat_blue pixels mapped to combat_green are consistent (the
battle palettes differ per unit type but the transform is exact).  The port's
recolour shader uses the game's own colours instead of a generic green ramp.

Output: assets/data/unit_palette_lut.json
"""
import glob
import json
import os
import re

import numpy as np
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
FRAMES = os.path.join(ROOT, 'assets', 'art', 'unit_frames')
OUT = os.path.join(ROOT, 'assets', 'data', 'unit_palette_lut.json')


def main():
    luts = {}
    for blue in sorted(glob.glob(os.path.join(FRAMES, 'unit_*_combat_blue.png'))):
        m = re.search(r'unit_(\d\d)_', os.path.basename(blue))
        if not m:
            continue
        t = int(m.group(1))
        green = blue.replace('_combat_blue', '_combat_green')
        if not os.path.exists(green):
            continue
        ib = np.asarray(Image.open(blue).convert('RGBA'))
        ig = np.asarray(Image.open(green).convert('RGBA'))
        if ib.shape != ig.shape:
            continue
        mask = ib[:, :, 3] > 0
        pairs = luts.setdefault(str(t), {})
        for cb, cg in zip(ib[mask][:, :3], ig[mask][:, :3]):
            key = tuple(int(v) for v in cb)
            pairs[key] = tuple(int(v) for v in cg)   # deterministic
    out = {}
    for t, pairs in sorted(luts.items(), key=lambda kv: int(kv[0])):
        lst = [[list(k), list(v)] for k, v in sorted(pairs.items())]
        out[t] = lst
        print('unit %2s colors %d' % (t, len(lst)))
    json.dump(out, open(OUT, 'w', encoding='utf-8'), indent=1)
    print('wrote', OUT)


if __name__ == '__main__':
    main()
