import os, json, struct, csv
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
D = os.path.join(ROOT, 'dump')
OUT = os.path.join(ROOT, 'assets')
os.makedirs(os.path.join(OUT, 'art'), exist_ok=True)
os.makedirs(os.path.join(OUT, 'data'), exist_ok=True)
os.makedirs(os.path.join(OUT, 'maps'), exist_ok=True)

# ---------------- palette ----------------
raw = open(os.path.join(D, 'palette_bgr.bin'), 'rb').read()
PAL = [(raw[i*4+2], raw[i*4+1], raw[i*4]) for i in range(256)]
json.dump([{'r':r,'g':g,'b':b} for r,g,b in PAL],
          open(os.path.join(OUT, 'art', 'palette.json'), 'w'), indent=0)

# ---------------- tile bank -> editable PNG ----------------
tb = open(os.path.join(D, 'tilebank.bin'), 'rb').read()
sb = open(os.path.join(D, 'spritebank.bin'), 'rb').read()

def sheet_8bit(data, tw, th, cols, rows, bank):
    img = Image.new('RGBA', (cols*tw, rows*th), (0, 0, 0, 0))
    px = img.load()
    n = len(data)//(tw*th)
    for t in range(min(n, cols*rows)):
        tx, ty = (t % cols)*tw, (t//cols)*th
        for y in range(th):
            for x in range(tw):
                v = data[t*tw*th + y*tw + x]
                if v == 0: continue
                r, g, b = PAL[((bank << 4) | (v & 0xF)) & 0xFF]
                # the memory banks are bottom-up: flip inside each tile
                px[tx+x, ty+(th-1-y)] = (r, g, b, 255)
    return img

# font/UI tiles (bank 0) and the raw tile bank indexed by nibble
sheet_8bit(tb, 16, 16, 64, 32, 0).save(os.path.join(OUT, 'art', 'tiles_ui_bank0.png'))
for b in range(8, 16):
    sheet_8bit(sb, 32, 32, 32, 32, b).save(os.path.join(OUT, 'art', f'units_bank{b:02d}.png'))

# ---------------- map background ----------------
terr = open(os.path.join(D, 'terrain.bin'), 'rb').read()
tm = open(os.path.join(D, 'tilemap.bin'), 'rb').read()
MW, MH = 30, 20
bg = Image.new('RGB', (MW*16, MH*16), (0, 0, 0))
bpx = bg.load()
for ty in range(MH):
    for tx in range(MW):
        e = struct.unpack_from('<H', tm, ((ty+2)*64 + (tx+1))*2)[0]
        tile = e & 0x7ff
        bank = (e >> 0xb) + 1
        base = tile*256
        for y in range(16):
            for x in range(16):
                v = tb[base + y*16 + x]
                if v == 0: continue
                bpx[tx*16+x, ty*16+(15-y)] = PAL[((bank << 4) | (v & 0xF)) & 0xFF]
bg.save(os.path.join(OUT, 'art', 'map_background.png'))
print('map background', bg.size)

# ---------------- tables ----------------
tables = json.load(open(os.path.join(D, 'tables.json'), encoding='utf-8'))
def clean_name(s):
    s = s.replace('\ufffd', '')
    return s.strip()

units = []
for u in tables['unit_types']:
    units.append({
        'id': u['id'],
        'name': clean_name(u['name']),
        'attack_ground': u['atk_ground'],
        'attack_air': u['atk_air'],
        'defense': u['defense'],
        'counter_ground': u['counter_ground'],
        'counter_air': u['counter_air'],
        'movement': u['attack_rating'],
        'movement_class': u['movement_class'],
        'ability_flags': u['ability_flags'],
        'max_hp': 7,
    })
json.dump({'units': units}, open(os.path.join(OUT, 'data', 'units.json'), 'w', encoding='utf-8'),
          indent=2, ensure_ascii=False)

terrain = {
    'defense': tables['terrain_defense'],
    'move_cost': tables['terrain_move_cost'],
    'names': {str(i): n for i, n in enumerate(
        ['void','grass','forest','mountain','water','plain','city','factory',
         'hq','road','bridge','base','ruin','special','x13','x14'])},
}
json.dump(terrain, open(os.path.join(OUT, 'data', 'terrain.json'), 'w'), indent=2)

combat = {
    'hp_damage_multiplier': tables['hp_damage_multiplier'],
    'variance': {'20-24': 15, '60-64': 20, '80-82': 2, '98-99': 40, 'default': '(150-roll)/10'},
    'formula': 'see RE_FINDINGS.md section 5',
}
json.dump(combat, open(os.path.join(OUT, 'data', 'combat.json'), 'w'), indent=2)

json.dump({'map_byte_translate': tables['map_byte_translate']},
          open(os.path.join(OUT, 'data', 'map_translate.json'), 'w'), indent=0)

# ---------------- NMD maps -> JSON ----------------
def parse_nmd(path):
    b = open(path, 'rb').read()
    if len(b) < 19 or not b.startswith(b'NEC MAP'):
        return None
    w, h, attr = b[16], b[17], b[18]
    grid = list(b[19:19+w*h])
    off = 19 + w*h
    unitsout = []
    while off + 2 < len(b):
        if b[off] == 0xFF: break
        t = b[off]
        def coord(v, limit):
            n = v & 0x1F
            if n < limit: return n
            n = v & 0x0F
            if n < limit: return n
            return None
        x = coord(b[off+1], w)
        y = coord(b[off+2], h)
        if x is not None and y is not None:
            unitsout.append({'type': t & 0x1F, 'owner': 1 if (t & 0x80) else 0,
                             'x': x, 'y': y})
        off += 3
        if len(unitsout) >= 56: break
    return {'magic': b[:16].decode('latin1').rstrip(), 'width': w, 'height': h,
            'attr': attr, 'raw_grid': grid, 'units': unitsout}

maps = []
for dirpath, _, files in os.walk(os.path.join(ROOT, 'game', 'MAPDATA')):
    for fn in files:
        if not fn.lower().endswith('.nmd'): continue
        p = os.path.join(dirpath, fn)
        m = parse_nmd(p)
        if not m: continue
        m['name'] = os.path.splitext(fn)[0]
        m['folder'] = os.path.basename(dirpath)
        m['is_deploy'] = m['folder'].startswith('ƒ†')
        # translated terrain (low nibble)
        tr = tables['map_byte_translate']
        m['terrain'] = [[tr[v] & 0xF for v in m['raw_grid'][y*m['width']:(y+1)*m['width']]]
                        for y in range(m['height'])]
        maps.append(m)

deploy_by_base = {m['name'].replace('_', ''): m for m in maps if m['is_deploy']}
merged = 0
for m in maps:
    if not m['is_deploy'] and not m['units']:
        d = deploy_by_base.get(m['name'].replace('_', ''))
        if d and d['units']:
            m['units'] = d['units']
            merged += 1
    tag = 'deploy_' if m['is_deploy'] else ''
    out = {k: v for k, v in m.items() if k != 'is_deploy'}
    json.dump(out, open(os.path.join(OUT, 'maps', tag + m['name'] + '.json'), 'w'), indent=1)
print('exported maps:', len(maps), 'with units:', sum(1 for m in maps if m['units']),
      'merged from deploy:', merged)

# ---------------- all-units test mission ----------------
W, H = 15, 10
import random
random.seed(20240101)
test = {
    'name': 'all_units_test',
    'width': W, 'height': H,
    'background': '',
    'terrain': [[random.randint(1, 12) for _ in range(W)] for _ in range(H)],
    'units': [],
}
# 12 player units + 12 enemy units of each available type, laid out on two rows
for i in range(24):
    x = 1 + i % 12
    y = 1 + (i // 12) * 6
    test['units'].append({'type': i, 'owner': 0, 'x': x, 'y': y})
    test['units'].append({'type': i, 'owner': 1, 'x': x, 'y': y + 2})
json.dump(test, open(os.path.join(OUT, 'maps', 'all_units_test.json'), 'w'), indent=2)
print('test mission written')
