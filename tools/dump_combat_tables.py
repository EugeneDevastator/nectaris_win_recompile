import json
b = open(r'game\Nec.exe','rb').read()
base = 0x27c00 - 0x42a000
def d(va, n):
    return list(b[base+va:base+va+n])
remap = d(0x432198, 16)
table = d(0x4321a8, 256)
indirect = d(0x432228, 32)
print('remap   =', remap)
print('table min/max =', min(table), max(table))
print('table   =', table)
print('indirect=', indirect)
json.dump({'terrain_remap': remap, 'panel_table': table,
           'panel_indirect': indirect,
           'panels': [f'art/combat_terrain/panel_{i:02d}.png' for i in range(24)]},
          open(r'assets\data\combat_terrain.json', 'w'), indent=1)
print('wrote assets/data/combat_terrain.json')

