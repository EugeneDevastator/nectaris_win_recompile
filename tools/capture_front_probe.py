"""One-shot probe: capture a full 8-unit battle for front-row extraction."""
import os
import sys

sys.path.insert(0, os.path.dirname(__file__))
import capture_battle_matrix as m

job = {"id": "frontprobe_01", "atk": 6, "def": 6,
       "terrain_atk": 1, "terrain_def": 1}
n = m.run_job(job)
print("samples", n)
