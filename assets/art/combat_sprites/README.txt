Combat sprite / terrain extractions
===================================

All sprites below are rebuilt from the game's memory banks with the
per-tile VERTICAL FLIP applied (the banks are stored bottom-up).

Sprite sheets (32x32 cells, 4x scale, labels = sprite index):
  blue_battle_sprites_160_191.png    player (blue) combat sprites 160..191
  green_battle_sprites_160_191.png   enemy  (green) combat sprites 160..191
  effects_orange_192_199.png         muzzle/impact/explosion frames 192..199
                                     (orange/white palette bank 6)

Individual 32x32 RGBA sprites (transparent background):
  blue_160.png .. blue_191.png
  green_160.png .. green_191.png

Terrain panels (corrected, 256x256 RGBA, colour-keyed):
  terrain_panels_preview.png         panels 07 (road) and 06 (rock)
  ../combat_terrain/panel_00..23.png

Map/UI banks (corrected orientation):
  ../units_bank09.png                blue map/status icons
  ../units_bank10.png                green map/status icons
  ../units_bank10_flip.png           per-cell mirrored green (combat)
  ../tiles_ui_bank0.png              font/UI tiles (now upright)

Real captured battle frames (1:1 window capture):
  ../../ref/seq/after_000..084.png

NOTE: the "person with the orange gun" is NOT in this map/UI sprite bank.
Only minimap-style vehicle icons and the 160..191 combat pieces are present.
That strongly suggests the game loads a separate battle sprite set at combat
time, or stores it in a format not yet decoded.
