import os, time, sys
sys.path.insert(0, os.path.dirname(__file__))
from control import Driver

game = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'game'))
d = Driver(game)
try:
    print("pid", d.proc.pid)
    time.sleep(8)
    print("VKs polled:", d.vkseen())
    print("diag:", d.diag())
    d.snap(r"C:\OpenCode-Drive\SharedF\nectariswin\vk_before.png")
finally:
    d.close()
