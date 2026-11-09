"""Pick the fully-typed tutorial page from each captured run and export it."""
import os, glob, hashlib
from PIL import Image, ImageDraw

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
SRC = os.path.join(ROOT, 'dump', 'manual_pages')
OUT = os.path.join(ROOT, 'assets', 'art', 'tutorial')
os.makedirs(OUT, exist_ok=True)
SECTIONS = ['flow', 'commands', 'transport', 'capture']


def text_pixels(im):
    g = im.convert('L')
    n = 0
    for y in range(300, 470):
        for x in range(60, 590):
            if g.getpixel((x, y)) > 150:
                n += 1
    return n


picked = []
for sec in SECTIONS:
    files = sorted(glob.glob(os.path.join(SRC, sec + '_*.png')))
    if not files:
        continue
    counts = [text_pixels(Image.open(f)) for f in files]
    print(sec, 'frames', len(files), 'counts', counts)
    # a page is complete when its text-pixel count plateaus, then drops
    best = []
    i = 0
    while i < len(counts):
        j = i
        while j + 1 < len(counts) and counts[j + 1] >= counts[j] - 10:
            j += 1
        peak = max(range(i, j + 1), key=lambda k: counts[k])
        if counts[peak] > 60:
            best.append(files[peak])
        i = j + 1
    # dedupe consecutive identical images
    uniq = []
    seen = set()
    for f in best:
        h = hashlib.md5(open(f, 'rb').read()).hexdigest()
        if h in seen:
            continue
        seen.add(h)
        uniq.append(f)
    for n, f in enumerate(uniq):
        dst = os.path.join(OUT, '%s_page_%02d.png' % (sec, n))
        Image.open(f).save(dst)
        picked.append(dst)
    print('  picked', len(uniq), 'pages')

# contact sheet
cols = 4
rows = (len(picked) + cols - 1) // cols
sheet = Image.new('RGB', (cols * 330, rows * 260), (20, 20, 28))
d = ImageDraw.Draw(sheet)
for i, p in enumerate(picked):
    im = Image.open(p).convert('RGB').resize((320, 240), Image.NEAREST)
    x, y = (i % cols) * 330 + 5, (i // cols) * 260 + 16
    sheet.paste(im, (x, y))
    d.text((x, y - 12), os.path.basename(p), fill=(255, 230, 100))
sheet.save(os.path.join(ROOT, 'dump', 'tutorial_pages_contact.png'))
print('total pages', len(picked))
