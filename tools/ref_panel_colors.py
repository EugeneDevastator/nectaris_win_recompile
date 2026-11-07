import os, json
from PIL import Image
from collections import Counter

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
im = Image.open(os.path.join(ROOT, 'dump', 'battle_right_panel.png')).convert('RGB')
c = Counter(im.getdata())
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
print('reference colors:')
for col, n in c.most_common(20):
    hits = []
    for bk in range(16):
        for i in range(16):
            if PAL[bk*16+i] == col:
                hits.append((bk, i))
    print(' ', col, n, 'palette', hits[:8])
