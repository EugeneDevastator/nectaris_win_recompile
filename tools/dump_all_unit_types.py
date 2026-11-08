"""Dump unit tables for type 0..35, including static structures (24+)."""
import struct

d = open(r'C:\OpenCode-Drive\SharedF\nectariswin\game\Nec.exe', 'rb').read()
base = 0x400000

def off(va):
    # .data only
    return 0x27c00 + (va - 0x42a000)

def u8(va):
    return d[off(va)]

def nameat(va, n=8):
    raw = d[off(va):off(va) + n]
    raw = raw.split(b'\0')[0]
    return raw.decode('cp932', 'replace')

TABLES = {
    'sprite': 0x42b170,
    'atk_g': 0x42b1a0,
    'atk_a': 0x42b1b8,
    'move': 0x42b1d0,
    'rng_g': 0x42b1e8,
    'rng_a': 0x42b200,
    'def': 0x42b218,
    'ttype': 0x42b230,
    'flags': 0x42b288,
}

print('%-3s %-10s %-10s %4s %4s %3s %3s %3s %3s %3s %3s %-8s' %
      ('id', 'name', 'name2', 'spr', 'spr2', 'atg', 'ata', 'mov', 'rg', 'ra', 'def', 'ttype'))
for t in range(0, 40):
    n1 = nameat(0x42b2a0 + t * 8)
    n2 = nameat(0x42b358 + t * 8)
    if not n1 and not n2:
        continue
    vals = {k: u8(va + t) for k, va in TABLES.items() if k != 'sprite'}
    spr = d[off(0x42b170) + t * 2]
    spr2 = d[off(0x42b170) + t * 2 + 1]
    print('%-3d %-10s %-10s %4d %4d %3d %3d %3d %3d %3d %3d 0x%02x' %
          (t, n1, n2, spr, spr2, vals['atk_g'], vals['atk_a'], vals['move'],
           vals['rng_g'], vals['rng_a'], vals['def'], vals['ttype']))
