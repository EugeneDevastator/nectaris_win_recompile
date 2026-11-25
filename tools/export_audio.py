"""Extract every SFX WAVE resource from the original game.

Source: game/Nec.exe resource type WAVE (the 36 SE01..SE36 effects).  Writes
the same files to assets/audio/, release/assets/audio/ and
port/build/assets/audio/, plus a manifest with duration/format/hash so the
catalog is verifiable.

Music is separate (game/SOUND/*.MID) and is not an SFX.
"""
import hashlib
import io
import json
import os
import struct
import wave

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
EXE = os.path.join(ROOT, 'game', 'Nec.exe')
OUTS = [os.path.join(ROOT, 'assets', 'audio'),
        os.path.join(ROOT, 'release', 'assets', 'audio'),
        os.path.join(ROOT, 'port', 'build', 'assets', 'audio')]

b = open(EXE, 'rb').read()


def u16(o):
    return struct.unpack_from('<H', b, o)[0]


def u32(o):
    return struct.unpack_from('<I', b, o)[0]


pe = u32(0x3c)
nsec = u16(pe + 6)
opt = pe + 24
magic = u16(opt)
dd = opt + (96 if magic == 0x10b else 112)
rsrc_rva = u32(dd + 16)
sh = opt + (224 if magic == 0x10b else 240)
sects = []
for i in range(nsec):
    o = sh + i * 40
    vsz, va, rsz, raw = struct.unpack_from('<IIII', b, o + 8)
    sects.append((va, vsz, raw, rsz))


def rva2off(rva):
    for va, vsz, raw, rsz in sects:
        if va <= rva < va + max(vsz, rsz):
            return raw + (rva - va)
    raise ValueError('bad rva %x' % rva)


base = rva2off(rsrc_rva)
leaves = []


def walk(o, path_ids):
    nn = u16(base + o + 12)
    ni = u16(base + o + 14)
    for i in range(nn + ni):
        e = base + o + 16 + i * 8
        nf, o2 = struct.unpack_from('<II', b, e)
        if nf & 0x80000000:
            so = base + (nf & 0x7fffffff)
            ln = u16(so)
            ident = b[so + 2:so + 2 + ln * 2].decode('utf-16le')
        else:
            ident = nf
        if o2 & 0x80000000:
            walk(o2 & 0x7fffffff, path_ids + [ident])
        else:
            e2 = base + o2
            drva, sz, cp, _ = struct.unpack_from('<IIII', b, e2)
            leaves.append((path_ids + [ident], drva, sz))


walk(0, [])
waves = []
for p, rva, sz in leaves:
    if p[0] != 'WAVE':
        continue
    off = rva2off(rva)
    data = b[off:off + sz]
    number = p[1]
    if isinstance(number, str):
        digits = ''.join(ch for ch in number if ch.isdigit())
        name = 'se%02d' % int(digits or 0)
    else:
        name = 'se%02d' % (number & 0xffff)
    waves.append((name, data))

waves.sort()
manifest = []
for d in OUTS:
    os.makedirs(d, exist_ok=True)
for name, data in waves:
    wav_path = name + '.wav'
    for d in OUTS:
        with open(os.path.join(d, wav_path), 'wb') as f:
            f.write(data)
    info = {'file': wav_path, 'bytes': len(data),
            'sha1': hashlib.sha1(data).hexdigest()}
    try:
        with wave.open(io.BytesIO(data), 'rb') as w:
            info.update({'channels': w.getnchannels(),
                         'sample_rate': w.getframerate(),
                         'bits': w.getsampwidth() * 8,
                         'duration': round(w.getnframes() / w.getframerate(), 3)})
    except wave.Error:
        info['error'] = 'not a RIFF/WAVE file'
    manifest.append(info)

with open(os.path.join(OUTS[0], 'manifest.json'), 'w', encoding='utf-8') as f:
    json.dump({'source': 'game/Nec.exe WAVE resources',
               'count': len(manifest), 'files': manifest}, f, indent=1)
print('wrote', len(manifest), 'SFX to', OUTS[0])
