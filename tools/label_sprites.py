from PIL import Image, ImageDraw
im = Image.open(r'assets\art\units_bank08.png').convert('RGBA')
# make a labelled montage of sprites 96..191, 8 per row, 4x
cols, rows = 12, 8
cw, ch = 32*4, 32*4 + 14
out = Image.new('RGB', (cols*cw, rows*ch), (20, 20, 24))
d = ImageDraw.Draw(out)
start = 96
for i in range(cols*rows):
    idx = start + i
    sx, sy = (idx % 32)*32, (idx//32)*32
    spr = im.crop((sx, sy, sx+32, sy+32)).resize((128, 128), Image.NEAREST)
    x, y = (i % cols)*cw, (i//cols)*ch
    out.paste(spr, (x, y+14), spr)
    d.text((x+2, y), str(idx), fill=(255, 255, 0))
out.save(r'dump\sprite_labels.png')
print(out.size)
