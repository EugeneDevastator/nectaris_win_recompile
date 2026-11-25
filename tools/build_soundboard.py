"""Build docs/audio/soundboard.html from the extracted SFX manifest.

The page embeds everything (no fetch), so it works when opened directly from
disk with a file:// URL.  Each of the 36 original SE resources gets a player
and the event roles can be reassigned with dropdowns; the resulting mapping can
be copied or downloaded as assets/data/audio.json.
"""
import html
import json
import os

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
MANIFEST = os.path.join(ROOT, 'assets', 'audio', 'manifest.json')
CONFIG = os.path.join(ROOT, 'assets', 'data', 'audio.json')
OUT = os.path.join(ROOT, 'docs', 'audio', 'soundboard.html')

ROLES = [
    ('select', 'Unit selected', 'Clicking a friendly unit on the map.'),
    ('move', 'Cursor moved', 'Moving the map cursor.'),
    ('entry', 'Battle entry (roll-in)', 'The combat screen rolls in, before the volley.'),
    ('shot', 'Volley / shot', 'First frame of a direct-fire volley.'),
    ('shot_heavy', 'Heavy shot', 'Volley from an indirect/artillery unit.'),
    ('miss', 'Miss / scatter', 'A shot that does not hit a slot.'),
    ('big', 'Destruction', 'Destroyed units explode (all at once).'),
    ('capture', 'Factory capture', 'A factory changes owner.'),
]

with open(MANIFEST, encoding='utf-8') as f:
    manifest = json.load(f)
with open(CONFIG, encoding='utf-8') as f:
    current = json.load(f)


def base_name(role_value):
    return os.path.basename(str(role_value))


cards = []
for entry in manifest['files']:
    name = entry['file']
    stem = os.path.splitext(name)[0]
    dur = entry.get('duration', 0)
    cards.append(
        '<div class="card" data-se="%s">'
        '<div class="head"><b>%s</b><span>%0.2f s</span></div>'
        '<audio controls preload="none" src="../../assets/audio/%s"></audio>'
        '<div class="roles" data-roles="%s"></div>'
        '</div>' % (stem, stem.upper(), dur, html.escape(name), stem)
    )

role_rows = []
for key, label, desc in ROLES:
    opts = ['<option value="">(none)</option>']
    chosen = base_name(current.get(key, ''))
    for entry in manifest['files']:
        name = entry['file']
        sel = ' selected' if name == chosen else ''
        opts.append('<option value="%s"%s>%s</option>'
                    % (html.escape(name), sel, os.path.splitext(name)[0]))
    role_rows.append(
        '<tr><td><code>%s</code><div class="muted">%s</div></td>'
        '<td><select data-role="%s">%s</select></td>'
        '<td class="muted">%s</td></tr>'
        % (key, html.escape(label), key, ''.join(opts), html.escape(desc))
    )

page = """<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>Nectaris SFX soundboard</title>
<style>
  :root { color-scheme: dark; }
  body { margin: 0; padding: 24px 28px 60px; background: #0d1017;
         color: #e6e9ef; font: 14px/1.45 "Segoe UI", system-ui, sans-serif; }
  h1 { font-size: 22px; margin: 0 0 4px; }
  h2 { font-size: 16px; margin: 30px 0 10px; }
  p.lead { margin: 0 0 18px; color: #9aa4b5; max-width: 900px; }
  .banner { background: #1b2333; border-left: 3px solid #e0a63a;
            padding: 10px 14px; border-radius: 4px; margin: 0 0 22px;
            max-width: 900px; color: #d8dced; }
  .grid { display: grid; grid-template-columns: repeat(auto-fill, minmax(230px, 1fr));
          gap: 12px; }
  .card { background: #161c27; border: 1px solid #263043; border-radius: 6px;
          padding: 10px; }
  .card.used { border-color: #3f7f5a; box-shadow: 0 0 0 1px #2f6b4a inset; }
  .card .head { display: flex; justify-content: space-between; align-items: baseline;
                margin-bottom: 6px; letter-spacing: .04em; }
  .card .head span { color: #8b96a8; font-size: 12px; }
  audio { width: 100%; height: 32px; }
  .roles { margin-top: 6px; font-size: 12px; color: #6fd08c; min-height: 15px; }
  table { border-collapse: collapse; max-width: 1000px; width: 100%; }
  th, td { text-align: left; padding: 7px 10px; border-bottom: 1px solid #232b3a;
           vertical-align: top; font-size: 13px; }
  th { color: #9aa4b5; font-weight: 600; }
  code { background: #1d2532; padding: 1px 5px; border-radius: 3px; }
  select { background: #1d2532; color: #e6e9ef; border: 1px solid #33405a;
           border-radius: 4px; padding: 4px 6px; min-width: 110px; }
  .muted { color: #8b96a8; font-size: 12px; }
  .bar { display: flex; gap: 10px; align-items: center; margin: 10px 0 0; }
  button { background: #2b6cb0; color: #fff; border: 0; border-radius: 4px;
           padding: 8px 14px; font: inherit; cursor: pointer; }
  button.secondary { background: #2a3343; }
  button:hover { filter: brightness(1.15); }
  textarea { width: 100%; max-width: 900px; height: 190px; margin-top: 12px;
             background: #11161f; color: #cfe3ff; border: 1px solid #2a3648;
             border-radius: 5px; padding: 10px; font: 12px/1.4 Consolas, monospace; }
  a { color: #7fb3ff; }
</style>
</head>
<body>
<h1>Nectaris / Military Madness &mdash; SFX soundboard</h1>
<p class="lead">All 36 effects are the original <code>Nec.exe</code> WAVE
resources (8-bit mono 22050 Hz).  Listen to each one, then assign the SE that
belongs to every game event.  Export the result and drop it into
<code>assets/data/audio.json</code> &mdash; the runtime reads that file, so no
rebuild is needed.</p>

<div class="banner"><b>Provisional mapping.</b> The IDs currently selected were
guesses and are known to be wrong.  Please correct them by ear.</div>

<h2>Events</h2>
<table>
  <tr><th>Event</th><th>SE file</th><th>When it plays</th></tr>
  __ROLE_ROWS__
</table>

<h2>All extracted effects</h2>
<div class="grid">
__CARDS__
</div>

<h2>Export</h2>
<div class="bar">
  <button onclick="buildJson()">Refresh JSON</button>
  <button class="secondary" onclick="copyJson()">Copy</button>
  <button class="secondary" onclick="downloadJson()">Download audio.json</button>
  <span class="muted">Then save/overwrite <code>assets/data/audio.json</code>.</span>
</div>
<textarea id="out" spellcheck="false"></textarea>

<script>
const ROLES = __ROLE_KEYS__;
const LABELS = __ROLE_LABELS__;
const selects = Array.from(document.querySelectorAll('select[data-role]'));

function currentMap() {
  const m = {};
  for (const s of selects) m[s.dataset.role] = s.value;
  return m;
}

function refreshCards() {
  const m = currentMap();
  document.querySelectorAll('.card').forEach(card => {
    const se = card.dataset.se;
    const used = ROLES.filter(r => (m[r] || '').replace('.wav','') === se).map(r => LABELS[r]);
    card.classList.toggle('used', used.length > 0);
    card.querySelector('.roles').textContent = used.length ? '\u2192 ' + used.join(', ') : '';
  });
}

function buildJson() {
  const m = currentMap();
  const out = { note: 'Confirmed event mapping for the original SFX.' };
  for (const r of ROLES) out[r] = m[r];
  const text = JSON.stringify(out, null, 1) + '';
  document.getElementById('out').value = text;
  refreshCards();
  return text;
}

function copyJson() {
  const text = buildJson();
  navigator.clipboard.writeText(text).then(
    () => document.querySelector('.bar .muted').textContent = 'Copied.',
    () => { const t = document.getElementById('out'); t.select(); document.execCommand('copy'); });
  document.querySelector('.bar .muted').textContent = 'Copied.';
}

function downloadJson() {
  const text = buildJson();
  const a = document.createElement('a');
  a.href = URL.createObjectURL(new Blob([text], {type: 'application/json'}));
  a.download = 'audio.json';
  a.click();
  setTimeout(() => URL.revokeObjectURL(a.href), 1000);
}

selects.forEach(s => s.addEventListener('change', buildJson));
buildJson();
</script>
</body>
</html>
"""

page = page.replace('__ROLE_ROWS__', '\n  '.join(role_rows))
page = page.replace('__CARDS__', '\n'.join(cards))
page = page.replace('__ROLE_KEYS__',
                    json.dumps([r[0] for r in ROLES]))
page = page.replace('__ROLE_LABELS__',
                    json.dumps({r[0]: r[1] for r in ROLES}))

os.makedirs(os.path.dirname(OUT), exist_ok=True)
with open(OUT, 'w', encoding='utf-8') as f:
    f.write(page)
print('wrote', OUT)
