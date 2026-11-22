"""Collect the per-unit impostor frames and pack a single atlas.

For every unit:
  * copy unit_frames/unit_XX_frame_NN_combat_blue.png into
    assets/units/<unit folder>/frames/impostor_NN.png
  * record the frame list in the unit's unit.json

Then pack all frames into assets/art/unit_impostors_atlas.png and write
assets/data/unit_impostors.json (unit -> atlas rects).  The port uses the atlas
at runtime; the per-folder PNGs stay editable and act as a fallback.
"""
import json
import os
import re

from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
FRAMES = os.path.join(ROOT, 'assets', 'art', 'unit_frames')
UNITS = os.path.join(ROOT, 'assets', 'units')
ATLAS = os.path.join(ROOT, 'assets', 'art', 'unit_impostors_atlas.png')
MANIFEST = os.path.join(ROOT, 'assets', 'data', 'unit_impostors.json')

PAD = 2
MAX_W = 1024


def unit_folders():
    out = {}
    for name in os.listdir(UNITS):
        m = re.match(r'unit_(\d\d)_', name)
        if m:
            out[int(m.group(1))] = os.path.join(UNITS, name)
    return out


def main():
    folders = unit_folders()
    frames = {}
    for t in range(24):
        found = []
        for f in sorted(os.listdir(FRAMES)):
            m = re.match(r'unit_%02d_frame_(\d\d)_combat_blue\.png$' % t, f)
            if m:
                found.append((int(m.group(1)), os.path.join(FRAMES, f)))
        if not found:
            continue
        folder = folders.get(t)
        if not folder:
            continue
        fdir = os.path.join(folder, 'frames')
        os.makedirs(fdir, exist_ok=True)
        rel = []
        for idx, src in found:
            dst = os.path.join(fdir, 'impostor_%02d.png' % idx)
            Image.open(src).save(dst)
            rel.append('frames/impostor_%02d.png' % idx)
        frames[t] = [(idx, Image.open(src).convert('RGBA')) for idx, src in found]

        uj = os.path.join(folder, 'unit.json')
        if os.path.exists(uj):
            u = json.load(open(uj, encoding='utf-8'))
            u['impostor_frames'] = rel
            u['impostor_count'] = len(rel)
            json.dump(u, open(uj, 'w', encoding='utf-8'), ensure_ascii=False, indent=1)
        print('unit %2d: %d impostor frames -> %s' % (t, len(rel), folder))

    # Simple shelf packer.
    x = y = 0
    row_h = 0
    rects = {}
    placed = []
    for t in sorted(frames):
        rects[str(t)] = []
        for idx, im in frames[t]:
            w, h = im.size
            if x + w + PAD > MAX_W:
                x = 0
                y += row_h + PAD
                row_h = 0
            rects[str(t)].append([x, y, w, h])
            placed.append((im, x, y))
            x += w + PAD
            row_h = max(row_h, h)
    total_h = y + row_h + PAD
    atlas = Image.new('RGBA', (MAX_W, total_h), (0, 0, 0, 0))
    for im, px, py in placed:
        atlas.alpha_composite(im, (px, py))
    atlas.save(ATLAS)
    json.dump({'atlas': 'art/unit_impostors_atlas.png', 'units': rects},
              open(MANIFEST, 'w', encoding='utf-8'), indent=1)
    print('atlas %dx%d -> %s' % (atlas.width, atlas.height, ATLAS))
    print('manifest ->', MANIFEST)


if __name__ == '__main__':
    main()
