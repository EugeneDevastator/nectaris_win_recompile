"""Build one horizontal combat-frame strip per unit (combat-blue palette) and
place it in the release unit folders.
"""
import os, glob, re, json
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
SRC = os.path.join(ROOT, 'assets', 'art', 'unit_frames')
DESTS = [os.path.join(ROOT, 'release', 'assets', 'units'),
         os.path.join(ROOT, 'port', 'build', 'assets', 'units')]

manifest = {}
for t in range(23):
    files = glob.glob(os.path.join(SRC, 'unit_%02d_frame_*_combat_blue.png' % t))
    if not files:
        continue
    files.sort(key=lambda p: int(re.search(r'frame_(\d+)', p).group(1)))
    ims = [Image.open(f).convert('RGBA') for f in files]
    gap = 2
    H = max(im.height for im in ims)
    W = sum(im.width for im in ims) + gap * (len(ims) - 1)
    strip = Image.new('RGBA', (W, H), (0, 0, 0, 0))
    x = 0
    frames = []
    for im in ims:
        y = H - im.height
        strip.alpha_composite(im, (x, y))
        frames.append({'x': x, 'y': y, 'w': im.width, 'h': im.height})
        x += im.width + gap
    for dest in DESTS:
        d = os.path.join(dest, 'unit_%02d' % t)
        if not os.path.isdir(d):
            continue
        strip.save(os.path.join(d, 'combat_frames.png'))
        json.dump({'palette': 'combat_blue', 'frames': frames},
                  open(os.path.join(d, 'combat_frames.json'), 'w'), indent=1)
    manifest[t] = {'frames': len(ims), 'width': W, 'height': H}
    print('unit %2d frames %d strip %dx%d' % (t, len(ims), W, H))
json.dump(manifest, open(os.path.join(SRC, 'combat_strips_manifest.json'), 'w'), indent=1)
print('done')
