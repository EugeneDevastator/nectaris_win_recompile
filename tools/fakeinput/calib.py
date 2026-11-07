from PIL import Image
import sys
img = Image.open(r'C:\OpenCode-Drive\SharedF\nectariswin\shot_t2.png').convert('RGB')
w,h = img.size
print('size', w, h)
px = img.load()
# count near-white and near-black per row, whole width
for y in range(h):
    wh = sum(1 for x in range(w) if all(c>170 for c in px[x,y]))
    if wh > 20:
        xs=[x for x in range(w) if all(c>170 for c in px[x,y])]
        print(f"y={y:3d} white={wh:4d} x={min(xs)}..{max(xs)}")
