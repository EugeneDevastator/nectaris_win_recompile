"""Drive Nec.exe to the tactical map using synthetic Space + mouse.

Nothing here moves the real cursor or presses the real keyboard.
"""
import os, time, sys
sys.path.insert(0, os.path.dirname(__file__))
from control import Driver, menu_metrics

OUT = r"C:\OpenCode-Drive\SharedF\nectariswin"
VK_SPACE = 0x20
game = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'game'))

d = Driver(game)
try:
    print("pid", d.proc.pid, "hwnd", d.hwnd)
    d.fake_activate()
    time.sleep(0.3)
    # phase 1: Space through the attract/prologue until the main menu appears
    menu = None
    for i in range(40):
        if i % 5 == 0:
            d.fake_activate()
        d.tap(VK_SPACE)
        d.wake()
        time.sleep(0.45)
        img = d.snap()
        w, h = img.size
        bf, whites = menu_metrics(img)
        area = (w * h) / (640.0 * 480.0)
        if i % 4 == 0:
            print(f"p1 {i:02d} {w}x{h} black={bf:.2f} white={whites}")
        if bf > 0.55 and whites[0] > 800 * area and whites[1] > 700 * area:
            print(f"menu at {i}")
            img.save(os.path.join(OUT, "play_menu.png"))
            menu = (img, w, h)
            break
    if not menu:
        print("no menu found")
        d.snap(os.path.join(OUT, "play_nomenu.png"))
    else:
        img, w, h = menu
        sx, sy = w / 640.0, h / 480.0
        d.click(int(205 * sx), int(376 * sy))
        time.sleep(1.0)
        d.snap(os.path.join(OUT, "play_after_start.png"))
        print("clicked START")
        # phase 2: Space through the tutorial, capture each state
        for i in range(18):
            d.tap(VK_SPACE)
            d.wake()
            time.sleep(0.55)
            d.snap(os.path.join(OUT, f"play_{i:02d}.png"))
            print(f"p2 {i:02d} saved")
finally:
    d.close()
