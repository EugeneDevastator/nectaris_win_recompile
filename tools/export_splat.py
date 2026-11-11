"""Export the shared destroyed-unit marker ("splat").

The mine (M-77, unit type 0) has a second combat frame that is a black splat;
the original game leaves that shape behind when a unit is destroyed.  The port
reuses it as one shared wreck sprite for every destroyed unit, so this script
just copies that frame into the combat effects folder.

Usage:
  python tools/export_splat.py
"""
import json
import os
import shutil

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
SRC = os.path.join(ROOT, 'assets', 'art', 'unit_frames', 'unit_00_frame_01.png')
DST = os.path.join(ROOT, 'assets', 'art', 'combat_effects', 'splat.png')
MANIFEST = os.path.join(ROOT, 'assets', 'art', 'combat_effects', 'manifest.json')


def main():
    if not os.path.exists(SRC):
        raise SystemExit('missing source frame: ' + SRC)
    shutil.copyfile(SRC, DST)
    if os.path.exists(MANIFEST):
        with open(MANIFEST, encoding='utf-8') as f:
            data = json.load(f)
        data['wrecks'] = {
            'splat': 'splat.png',
            'source': 'unit_frames/unit_00_frame_01.png (M-77 mine dead frame)',
            'use': 'one splat per unit destroyed in the current combat exchange',
        }
        with open(MANIFEST, 'w', encoding='utf-8') as f:
            json.dump(data, f, ensure_ascii=False, indent=2)
    print('wrote', DST)


if __name__ == '__main__':
    main()
