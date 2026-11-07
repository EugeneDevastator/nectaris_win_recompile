import struct, json, csv, os
b = open(r'game\Nec.exe','rb').read()
BASE_VA, BASE_OFF = 0x42a000, 0x27c00
def arr(va, n):
    o = BASE_OFF + (va - BASE_VA)
    return list(b[o:o+n])

names = []
for i in range(32):
    o = BASE_OFF + (0x42b2a0 - BASE_VA) + i*8
    raw = b[o:o+8].split(b'\0')[0]
    try: s = raw.decode('cp932')
    except Exception: s = raw.decode('latin1')
    names.append(s)

types = []
for i in range(24):
    types.append(dict(
        id=i, name=names[i],
        atk_ground=arr(0x42b1a0,24)[i],
        atk_air=arr(0x42b1b8,24)[i],
        defense=arr(0x42b1d0,24)[i],
        counter_ground=arr(0x42b1e8,24)[i],
        counter_air=arr(0x42b200,24)[i],
        attack_rating=arr(0x42b218,24)[i],
        movement_class=arr(0x42b230,24)[i],
        ability_flags=arr(0x42b288,24)[i],
    ))

data = dict(
    unit_types=types,
    creature_names=names[24:],
    hp_damage_multiplier=arr(0x42b158,8),
    terrain_defense=arr(0x42b160,16),
    terrain_move_cost=[arr(0x42b248,64)[i*4:i*4+4] for i in range(16)],
    map_byte_translate=arr(0x42a530,256),
)
os.makedirs('dump', exist_ok=True)
open('dump/tables.json','w',encoding='utf-8').write(json.dumps(data, indent=2, ensure_ascii=False))
with open('dump/units.csv','w',newline='',encoding='utf-8') as f:
    w = csv.DictWriter(f, fieldnames=list(types[0].keys()))
    w.writeheader(); w.writerows(types)
print(json.dumps(data, indent=2, ensure_ascii=False))
