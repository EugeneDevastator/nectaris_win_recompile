"""Extract the original combat HP/number frame as a standalone UI sprite.

`ref/battle1.gif` is a 1:1 640x480 framebuffer capture of the original battle
screen.  The left HP box sits at (234,20)-(278,76): a 2 px yellow outer border,
a 2 px black gap, a 2 px dark-red inner border and a navy fill.  The digit is
erased by restoring the interior fill colour, leaving the frame-only sprite.
"""
import os
from PIL import Image

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
SRC = os.path.join(ROOT, 'ref', 'battle1.gif')
OUT = os.path.join(ROOT, 'assets', 'art', 'ui_unit_frame.png')

BOX = (234, 20, 278, 76)          # left HP box, 44x56
INNER = (6, 6, 38, 50)            # fill area inside the red border
FILL = (0, 0, 80)                 # original navy interior

im = Image.open(SRC).convert('RGB')
frame = im.crop(BOX).copy()
px = frame.load()
for y in range(INNER[1], INNER[3]):
    for x in range(INNER[0], INNER[2]):
        px[x, y] = FILL
frame.save(OUT)
print('wrote', OUT, frame.size)
