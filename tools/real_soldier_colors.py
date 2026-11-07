import os, json
from PIL import Image
from collections import Counter

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
real = Image.open(os.path.join(ROOT, 'ref', 'seq', 'after_021.png')).convert('RGB')
# soldier around (549,334)
c = Counter(real.crop((549-34, 334-30, 549+34, 334+30)).getdata())
pal = json.load(open(os.path.join(ROOT, 'assets', 'art', 'palette.json')))
PAL = [(p['r'], p['g'], p['b']) for p in pal]
print('real soldier colors:')
for col, n in c.most_common(20):
    banks = []
    for bk in range(16):
        for i in range(16):
            if PAL[bk*16+i] == col:
                banks.append((bk, i))
    print(' ', col, n, 'palette', banks[:6])
