"""Build the port and refresh the distributable release folder + zip.

Two folders exist on purpose:

  port/build/  CMake working build.  Objects, the freshly built exe, scratch
               screenshots and a copy of `assets/` (CMake's POST_BUILD step
               copies it next to the exe so the build folder runs on its own).
               This is what `tools/test_port_turn.py` launches by default.

  release/     Clean distribution payload: nectaris.exe + assets/ (+ map16.png).
               `nectaris_port.zip` is made from this folder only, so players
               never download object files or test screenshots.

The build step only refreshes port/build/assets, so this script is the one
supported way to publish: it rebuilds, copies the exe, mirrors assets/ into
release/ (deleting stale files) and rewrites nectaris_port.zip.

Usage:
  python tools/make_release.py [--jobs N]
"""
import argparse
import os
import shutil
import subprocess
import sys
import zipfile

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
BUILD = os.path.join(ROOT, 'port', 'build')
RELEASE = os.path.join(ROOT, 'release')
ZIP = os.path.join(ROOT, 'nectaris_port.zip')
ASSETS = os.path.join(ROOT, 'assets')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--jobs', type=int, default=8)
    args = ap.parse_args()

    print('building', BUILD)
    subprocess.check_call(['cmake', '--build', BUILD, '-j', str(args.jobs)])

    exe = os.path.join(BUILD, 'nectaris.exe')
    if not os.path.exists(exe):
        print('FAIL: missing', exe)
        return 1

    os.makedirs(RELEASE, exist_ok=True)
    shutil.copy2(exe, os.path.join(RELEASE, 'nectaris.exe'))

    dst_assets = os.path.join(RELEASE, 'assets')
    if os.path.isdir(dst_assets):
        shutil.rmtree(dst_assets)
    shutil.copytree(ASSETS, dst_assets)
    print('mirrored assets ->', dst_assets)

    if os.path.exists(ZIP):
        os.remove(ZIP)
    with zipfile.ZipFile(ZIP, 'w', zipfile.ZIP_DEFLATED) as z:
        for base, _dirs, files in os.walk(RELEASE):
            for name in files:
                path = os.path.join(base, name)
                z.write(path, os.path.relpath(path, RELEASE))
    size = os.path.getsize(ZIP)
    print('wrote %s (%.2f MB)' % (ZIP, size / (1024.0 * 1024.0)))
    return 0


if __name__ == '__main__':
    sys.exit(main())
