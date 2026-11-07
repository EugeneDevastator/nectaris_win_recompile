from PIL import Image
import math, json

ref = Image.open(r'ref\session\frame_00219.png').convert('RGB')
sheet = Image.open(r'assets\art\units_bank09.png').convert('RGBA')
px = ref.load()

# locate a clean blue tank near (135,293) by "not grey" + blue-ish
def is_blue(r, g, b):
    return b > 90 and b >= r and (b - r > 20 or g > 150)

# bounding box in a search window
x0, y0, x1, y1 = 110, 275, 165, 315
pts = [(x, y) for y in range(y0, y1) for x in range(x0, x1) if is_blue(*px[x, y])]
if pts:
    xs = [p[0] for p in pts]; ys = [p[1] for p in pts]
    bx0, bx1, by0, by1 = min(xs), max(xs), min(ys), max(ys)
    print('bbox', bx0, by0, bx1, by1, 'size', bx1-bx0+1, by1-by0+1)
    crop = ref.crop((bx0-1, by0-1, bx1+2, by1+2)).resize((32, 32), Image.NEAREST)
    crop.resize((192, 192), Image.NEAREST).save(r'dump\real_tank_32.png')
    rp = crop.load()

    # compare silhouettes + coarse colour to every sprite
    best = []
    for idx in range(64, 208):
        sx, sy = (idx % 32) * 32, (idx // 32) * 32
        sp = sheet.crop((sx, sy, sx + 32, sy + 32))
        p = sp.load()
        for flip in (False, True):
            inter = 0; union = 0; col = 0; n = 0
            for y in range(32):
                for x in range(32):
                    r, g, b, a = p[31-x if flip else x, y]
                    on = a > 128
                    tr, tg, tb = rp[x, y]
                    ton = is_blue(tr, tg, tb)
                    if on and ton:
                        inter += 1
                        col += (r-tr)**2 + (g-tg)**2 + (b-tb)**2
                        n += 1
                    if on or ton:
                        union += 1
            if union and n:
                iou = inter/union
                rmse = math.sqrt(col/n)
                best.append((iou, -rmse, idx, flip))
    best.sort(reverse=True)
    for b in best[:15]:
        print('iou=%.3f rmse=%.1f idx=%d flip=%s' % (b[0], -b[1], b[2], b[3]))
else:
    print('no blue pixels')
