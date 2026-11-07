from PIL import Image, ImageDraw

frame = Image.open(r'ref\seq\after_021.png').convert('RGB')
sheet = Image.open(r'assets\art\units_bank09.png').convert('RGBA')
px = frame.load()

def is_blue(r, g, b):
    return b > 90 and b >= r and (b - r > 20 or g > 150)

# cluster blue pixels in the left panel
pts = [(x, y) for y in range(180, 380) for x in range(64, 320) if is_blue(*px[x, y])]
cl = []
for p in pts:
    for c in cl:
        if abs(p[0]-c[0]) < 20 and abs(p[1]-c[1]) < 14:
            c[0] = (c[0]*c[2]+p[0])/(c[2]+1); c[1] = (c[1]*c[2]+p[1])/(c[2]+1); c[2] += 1
            break
    else:
        cl.append([p[0], p[1], 1])
cl = sorted([c for c in cl if c[2] > 30], key=lambda c: -c[2])
print('tanks', [(round(c[0]), round(c[1]), c[2]) for c in cl[:10]])

# crop the largest cluster with a fixed window, upscale
if cl:
    cx, cy = int(cl[0][0]), int(cl[0][1])
    crop = frame.crop((cx-22, cy-16, cx+22, cy+16))   # 44x32
    crop.resize((176, 128), Image.NEAREST).save(r'dump\real_tank_new.png')
    # candidate sprites at the same aspect (32x32 shown 4x)
    out = Image.new('RGB', (7*176, 150), (10, 10, 16))
    d = ImageDraw.Draw(out)
    out.paste(crop.resize((176, 128), Image.NEAREST), (0, 20))
    d.text((2, 2), 'real 44x32', fill=(255, 255, 0))
    for i, idx in enumerate([160, 164, 165, 166, 167, 168]):
        sx, sy = (idx % 32)*32, (idx//32)*32
        sp = sheet.crop((sx, sy, sx+32, sy+32)).resize((128, 128), Image.NEAREST)
        x = (i+1)*176
        out.paste(sp, (x, 20), sp)
        d.text((x+2, 2), str(idx), fill=(255, 255, 0))
    out.save(r'dump\real_vs_sprite.png')
    print('saved')

