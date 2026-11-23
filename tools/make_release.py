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


def _mirror_merge(src, dst):
    """Copy src over dst, then delete files under dst that no longer exist."""
    for base, _dirs, files in os.walk(src):
        rel = os.path.relpath(base, src)
        target = dst if rel == '.' else os.path.join(dst, rel)
        os.makedirs(target, exist_ok=True)
        for name in files:
            shutil.copy2(os.path.join(base, name), os.path.join(target, name))
    for base, _dirs, files in os.walk(dst, topdown=False):
        rel = os.path.relpath(base, dst)
        for name in files:
            srel = os.path.normpath(os.path.join(rel, name))
            if not os.path.exists(os.path.join(src, srel)):
                try:
                    os.remove(os.path.join(base, name))
                except OSError:
                    pass
        if rel != '.':
            try:
                os.rmdir(base)
            except OSError:
                pass


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
        try:
            shutil.rmtree(dst_assets)
            shutil.copytree(ASSETS, dst_assets)
        except PermissionError:
            # A preview/AV handle can keep a stale directory open.  Mirror over
            # the existing tree instead: copy current files, then prune files
            # that no longer exist in assets/.
            _mirror_merge(ASSETS, dst_assets)
    else:
        shutil.copytree(ASSETS, dst_assets)
    print('mirrored assets ->', dst_assets)

    if os.path.exists(ZIP):
        os.remove(ZIP)
    # Only the documented payload: the exe, map16.png and assets/.  Anything
    # else in release/ (stale or locked files) must never leak into the zip.
    with zipfile.ZipFile(ZIP, 'w', zipfile.ZIP_DEFLATED) as z:
        for base, _dirs, files in os.walk(RELEASE):
            for name in files:
                path = os.path.join(base, name)
                rel = os.path.relpath(path, RELEASE).replace('\\', '/')
                if rel != 'nectaris.exe' and rel != 'map16.png' and not rel.startswith('assets/'):
                    continue
                z.write(path, rel)
    size = os.path.getsize(ZIP)
    print('wrote %s (%.2f MB)' % (ZIP, size / (1024.0 * 1024.0)))
    return 0


if __name__ == '__main__':
    sys.exit(main())
