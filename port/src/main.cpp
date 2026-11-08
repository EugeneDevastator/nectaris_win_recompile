// Nectaris (1997) portable engine â€” tactical core.
// Data-readable port of the original Win32 game. Cutscenes are intentionally skipped.
//
// Assets (all editable):
//   assets/data/units.json      unit stats
//   assets/data/terrain.json    terrain defense + move costs
//   assets/data/combat.json     damage variance / HP multipliers
//   assets/art/units_bank10.png enemy sprites (32x32 grid)
//   assets/art/units_bank09.png player sprites (32x32 grid)
//   assets/art/map_background.png
//   assets/maps/*.json          missions
//
// Controls: LMB select / move / attack, RMB or ESC cancel, SPACE end turn, R restart, F1 debug.

#include "raylib.h"
#include <nlohmann/json.hpp>

#include "combat.h"
#include "model.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#ifdef _WIN32
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* hWnd,
                                                           const char* text,
                                                           const char* caption,
                                                           unsigned int type);
#endif

using json = nlohmann::json;

static std::vector<UnitType> gTypes;
static std::vector<TerrainDef> gTerrain;
static CombatRules gCombatRules;
static Mission gMission;

static std::vector<std::string> gTerrainNames;
static std::vector<int> gRemap, gPanelTable, gPanelIndirect;
static std::vector<Texture2D> gCombatPanels;
static Texture2D gFxBoom = {0}, gFxDebris = {0};
static Texture2D gEnemyFlipTex = {0};
static Texture2D gFxShell = {0}, gFxFlash = {0}, gFxStreak = {0};
static Texture2D gUnitSprite[24];
static Texture2D gUnitSpriteFlip[24];
static Texture2D gUnitMapTex[24], gUnitMapFlipTex[24];
static Texture2D gVfx[16];
static Texture2D gBackdrop = {0};
static bool gBackdropDirty = true;
static Font gUiFont = {0};
static bool gUiFontReady = false;

static const char* kGameDir = nullptr;
static std::string gAssetDir;

// Non-macro aliases (raylib's RED/GREEN are macros, so they cannot be used as
// local constant names in some expressions).
static const Color kRed = RED;
static const Color kGreen = GREEN;

// ------------------------------------------------------------------ hex
static inline int moveCost(const Mission& m, int x, int y, int moveClass) {
    int c = gTerrain[terrainAt(m, x, y)].cost[std::clamp(moveClass, 0, 3)];
    return c;
}

// ------------------------------------------------------------------ load
static json loadJson(const std::string& path) {
    std::ifstream f(path);
    if (!f) {
        std::cerr << "cannot open " << path << "\n";
        std::exit(1);
    }
    return json::parse(f);
}

static void loadData() {
    json u = loadJson(gAssetDir + "data/units.json");
    gTypes.clear();
    for (const auto& j : u["units"]) {
        UnitType t;
        t.id = j.value("id", 0);
        t.name = j.value("name", std::string("?"));
        t.atkGround = j.value("attack_ground", 0);
        t.atkAir = j.value("attack_air", 0);
        t.def = j.value("defense", 0);
        t.ctrGround = j.value("counter_ground", 1);
        t.ctrAir = j.value("counter_air", 1);
        t.movePts = j.value("movement", 0);
        t.moveClass = j.value("movement_class", 0);
        t.ability = j.value("ability_flags", 0);
        t.maxHp = j.value("max_hp", 7);
        gTypes.push_back(t);
    }

    json tr = loadJson(gAssetDir + "data/terrain.json");
    gTerrain.assign(16, TerrainDef{});
    for (int i = 0; i < 16 && i < (int)tr["defense"].size(); ++i)
        gTerrain[i].def = tr["defense"][i];
    for (int i = 0; i < 16 && i < (int)tr["move_cost"].size(); ++i)
        for (int c = 0; c < 4 && c < (int)tr["move_cost"][i].size(); ++c)
            gTerrain[i].cost[c] = tr["move_cost"][i][c];

    json cb = loadJson(gAssetDir + "data/combat.json");
    if (cb.contains("hp_damage_multiplier"))
        gCombatRules.hpMult = cb["hp_damage_multiplier"].get<std::vector<int>>();
    if (gCombatRules.hpMult.empty())
        gCombatRules.hpMult = {100, 105, 110, 120, 130, 140, 160, 200};
    if (cb.contains("variance") && cb["variance"].is_array()) {
        gCombatRules.variance.clear();
        for (const auto& b : cb["variance"]) {
            gCombatRules.variance.push_back(
                {b.value("min", 0), b.value("max", 0), b.value("mult", 10)});
        }
    }
    gCombatRules.varianceBase = cb.value("variance_base", gCombatRules.varianceBase);
    gCombatRules.varianceDiv = cb.value("variance_divisor", gCombatRules.varianceDiv);
    gCombatRules.supportDivisor = cb.value("support_divisor", gCombatRules.supportDivisor);
    gCombatRules.surroundDivisor = cb.value("surround_divisor", gCombatRules.surroundDivisor);
    gCombatRules.hpBonus = cb.value("hp_bonus", gCombatRules.hpBonus);
    gCombatRules.destroyThreshold =
        cb.value("destroy_threshold", gCombatRules.destroyThreshold);
    if (cb.contains("xp")) {
        const auto& x = cb["xp"];
        gCombatRules.xpHit = x.value("hit", gCombatRules.xpHit);
        gCombatRules.xpKill = x.value("kill", gCombatRules.xpKill);
        gCombatRules.xpNoDamage = x.value("no_damage", gCombatRules.xpNoDamage);
        gCombatRules.maxXp = x.value("max", gCombatRules.maxXp);
        gCombatRules.rankDivisor = x.value("rank_divisor", gCombatRules.rankDivisor);
    }

    gTerrainNames.assign(16, "?");
    if (tr.contains("names")) {
        for (int i = 0; i < 16; ++i) {
            auto it = tr["names"].find(std::to_string(i));
            if (it != tr["names"].end()) gTerrainNames[i] = it->get<std::string>();
        }
    }

    std::ifstream cf(gAssetDir + "data/combat_terrain.json");
    if (cf) {
        json ct = json::parse(cf);
        gRemap = ct.value("terrain_remap", std::vector<int>{});
        gPanelTable = ct.value("panel_table", std::vector<int>{});
        gPanelIndirect = ct.value("panel_indirect", std::vector<int>{});
    }
}

static std::vector<std::string> listCampaign(const std::string& dir) {
    std::vector<std::string> camps, maps;
    std::error_code ec;
    for (const auto& e : std::filesystem::directory_iterator(dir, ec)) {
        std::string n = e.path().filename().string();
        if (n == "all_units_test.json" || n.rfind("deploy_", 0) == 0) continue;
        bool campaign = n.rfind("campaign_", 0) == 0;
        bool editor = n.size() == 9 && (n[0] == 'M') && n[1] >= 'a' && n[1] <= 'd' &&
                      std::isdigit((unsigned char)n[2]) && std::isdigit((unsigned char)n[3]);
        if ((campaign || editor) && n.size() > 5 && n.substr(n.size() - 5) == ".json")
            (campaign ? camps : maps).push_back(e.path().string());
    }
    std::sort(camps.begin(), camps.end());
    std::sort(maps.begin(), maps.end());
    camps.insert(camps.end(), maps.begin(), maps.end());
    return camps;
}

static bool loadMission(const std::string& path) {    json j = loadJson(path);
    gMission = Mission{};
    gMission.name = j.value("name", std::string("mission"));
    gMission.number = j.value("mission", 0);
    gMission.w = j.value("width", 0);
    gMission.h = j.value("height", 0);
    gMission.background = j.value("background", std::string(""));
    if (gMission.w <= 0 || gMission.h <= 0) return false;
    gMission.terrain.assign(gMission.w * gMission.h, 1);
    if (j.contains("terrain") && j["terrain"].is_array()) {
        for (int y = 0; y < gMission.h; ++y)
            for (int x = 0; x < gMission.w; ++x)
                gMission.terrain[y * gMission.w + x] =
                    j["terrain"][y][x].get<int>();
    }
    for (const auto& ju : j["units"]) {
        Unit u;
        u.type = ju.value("type", 0);
        u.owner = ju.value("owner", 0);
        u.x = ju.value("x", 0);
        u.y = ju.value("y", 0);
        u.hp = ju.value("hp", 7);
        // XP lives in the original HP byte (bits 3..7); rank is xp / 4.
        // Mission files may specify either "xp" or the older "rank" tier.
        u.rank = ju.value("rank", 0);
        u.xp = ju.value("xp", std::clamp(u.rank, 0, 7) * 4);
        u.rank = rankFromXp(u.xp, gCombatRules.rankDivisor);
        gMission.units.push_back(u);
    }
    if (j.contains("garrison") && j["garrison"].is_array()) {
        for (const auto& ju : j["garrison"]) {
            Unit u;
            u.type = ju.value("type", 0);
            u.owner = ju.value("owner", 0);
            u.x = ju.value("x", 0);
            u.y = ju.value("y", 0);
            u.hp = ju.value("hp", 7);
            u.xp = ju.value("xp", 0);
            u.rank = rankFromXp(u.xp, gCombatRules.rankDivisor);
            gMission.garrison.push_back(u);
        }
    }
    // Optional full-map backdrop. Loading is deferred until the GL context
    // exists (see ensureBackdrop); a missing file is not fatal.
    gBackdropDirty = true;
    return true;
}

// ------------------------------------------------------------------ combat
// The battle simulation itself lives in combat.cpp (original FUN_0041b810 /
// FUN_00420cd0 / FUN_00421170).  These are the glue helpers for the port's
// mission data.
static CombatRng gCombatRng;

static CombatResult resolveBattle(int atk, int def, int mode) {
    return resolveCombat(gMission, gTypes, gTerrain, gCombatRules, atk, def, mode, gCombatRng);
}

static std::vector<int> attackable(const Mission& m, int slot) {
    return attackableTargets(m, gTypes, slot);
}

// ------------------------------------------------------------------ movement
static std::vector<int> reachable(const Mission& m, int slot) {
    std::vector<int> dist(m.w * m.h, 1 << 29);
    const Unit& u = m.units[slot];
    const UnitType& t = gTypes[u.type];
    using P = std::pair<int, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    dist[u.y * m.w + u.x] = 0;
    pq.push({0, u.y * m.w + u.x});
    while (!pq.empty()) {
        auto [c, idx] = pq.top();
        pq.pop();
        if (c != dist[idx]) continue;
        int x = idx % m.w, y = idx / m.w;
        for (int d = 0; d < 6; ++d) {
            int nx, ny;
            hexNeighbor(x, y, d, nx, ny);
            if (!inBounds(m, nx, ny)) continue;
            int mc = moveCost(m, nx, ny, t.moveClass);
            if (mc >= 0x7f) continue;
            int ni = unitAt(m, nx, ny);
            if (ni >= 0 && ni != slot) continue;   // blocked
            int nc = c + mc;
            if (nc <= t.movePts && nc < dist[ny * m.w + nx]) {
                dist[ny * m.w + nx] = nc;
                pq.push({nc, ny * m.w + nx});
            }
        }
    }
    return dist;
}

// ------------------------------------------------------------------ AI
static void queueCombat(int atk, int def, int hpA0, int hpD0, int hpA1, int hpD1,
                        const CombatResult& r, int mode);

static void aiTurn() {
    // Simple: each enemy moves one step toward the nearest player unit and attacks if adjacent.
    for (int i = 0; i < (int)gMission.units.size(); ++i) {
        Unit& u = gMission.units[i];
        if (u.hp <= 0 || u.owner != 1 || u.acted) continue;

        int best = -1, bestD = 1 << 20;
        for (int j = 0; j < (int)gMission.units.size(); ++j) {
            const Unit& p = gMission.units[j];
            if (p.hp <= 0 || p.owner != 1) continue;
            int d = std::abs(p.x - u.x) + std::abs(p.y - u.y);
            if (d < bestD) { bestD = d; best = j; }
        }
        if (best < 0) continue;
        const Unit& target = gMission.units[best];

        auto dist = reachable(gMission, i);
        int bestCell = -1, bestScore = 1 << 20;
        bool targetAir = gTypes[target.type].moveClass == 4;
        int range = targetAir ? gTypes[u.type].ctrAir : gTypes[u.type].ctrGround;
        for (int y = 0; y < gMission.h; ++y)
            for (int x = 0; x < gMission.w; ++x) {
                int idx = y * gMission.w + x;
                if (dist[idx] >= (1 << 29)) continue;
                if (unitAt(gMission, x, y) >= 0 && !(x == u.x && y == u.y)) continue;
                int d = hexDistance(x, y, target.x, target.y);
                int s;
                if (range >= 2) {
                    if (d < 2) s = 100 + (2 - d);
                    else if (d > range) s = d - range;
                    else s = std::abs(d - range) / 4;
                } else {
                    s = d;
                }
                if (s < bestScore) { bestScore = s; bestCell = idx; }
            }
        if (bestCell >= 0) {
            u.x = bestCell % gMission.w;
            u.y = bestCell / gMission.w;
        }
        auto targets = attackable(gMission, i);
        if (!targets.empty()) {
            int tgt = targets.front();
            Unit& D = gMission.units[tgt];
            int mode = hexDistance(u.x, u.y, D.x, D.y) - 1;
            int hpA0 = u.hp, hpD0 = D.hp;
            CombatResult r = resolveBattle(i, tgt, mode);
            applyCombatResult(gMission, r, gCombatRules);
            queueCombat(i, tgt, hpA0, hpD0, u.hp, D.hp, r, mode);
            std::cout << "[AI] " << gTypes[u.type].name << " -> " << gTypes[D.type].name
                      << "  dmg " << r.dmgToDef << "/" << r.dmgToAtk
                      << "  hp " << r.newHpDef << "/" << r.newHpAtk << "\n";
        }
        u.acted = true;
    }
}

// ------------------------------------------------------------------ hex map
static int CELL = 64;
static float gHexSize = 30.0f;
static float gStepX = 45.0f, gStepY = 51.96f;
static float gHexH = 51.96f;
static float gOriginX = 20.0f, gOriginY = 20.0f;

static void hexCenter(int x, int y, float& cx, float& cy) {
    cx = gOriginX + gHexSize + x * gStepX;
    cy = gOriginY + gHexH * 0.5f + y * gStepY + ((x & 1) ? gStepY * 0.5f : 0.0f);
}

// Flat-top, odd-q offset. Returns false if outside the map.
static bool pixelToHex(float px, float py, int w, int h, int& outX, int& outY) {
    float rx = px - (gOriginX + gHexSize);
    float ry = py - (gOriginY + gHexH * 0.5f);
    float q = (2.0f / 3.0f * rx) / gHexSize;
    float r = (-1.0f / 3.0f * rx + 0.57735027f * ry) / gHexSize;
    float cq = q, cr = r, cs = -q - r;
    float rq = std::round(cq), rr = std::round(cr), rs = std::round(cs);
    float dq = std::fabs(rq - cq), dr = std::fabs(rr - cr), ds = std::fabs(rs - cs);
    if (dq > dr && dq > ds) rq = -rr - rs;
    else if (dr > ds) rr = -rq - rs;
    else rs = -rq - rr;
    int col = (int)rq;
    int row = (int)rr + (col - (col & 1)) / 2;
    if (col < 0 || row < 0 || col >= w || row >= h) return false;
    outX = col;
    outY = row;
    return true;
}

static const Color kTerrainCols[16] = {
    {16, 18, 28, 255},    // 0 void
    {40, 92, 52, 255},    // 1 grass
    {28, 64, 40, 255},    // 2 forest
    {92, 82, 62, 255},    // 3 mountain
    {28, 52, 96, 255},    // 4 water
    {58, 66, 44, 255},    // 5 plain
    {96, 96, 96, 255},    // 6 city
    {72, 72, 88, 255},    // 7 factory
    {120, 80, 40, 255},   // 8 hq
    {84, 84, 84, 255},    // 9 road
    {60, 70, 80, 255},    // 10 bridge
    {100, 60, 60, 255},   // 11 base
    {70, 60, 50, 255},    // 12 ruin
    {80, 40, 80, 255},    // 13 special
    {30, 30, 30, 255},    // 14
    {30, 30, 30, 255},    // 15
};

static void ensureBackdrop() {
    if (!gBackdropDirty) return;
    gBackdropDirty = false;
    if (gBackdrop.id) { UnloadTexture(gBackdrop); gBackdrop = Texture2D{0}; }
    if (!gMission.background.empty()) {
        std::string bp = gMission.background;
        if (bp[0] != '/' && bp.find(':') == std::string::npos) bp = gAssetDir + bp;
        if (FileExists(bp.c_str())) {
            gBackdrop = LoadTexture(bp.c_str());
            SetTextureFilter(gBackdrop, TEXTURE_FILTER_POINT);
        }
    }
}

static void drawHexMap(const Mission& m) {
    ensureBackdrop();
    bool haveBg = gBackdrop.id != 0;
    if (haveBg) {
        float x0 = gOriginX;
        float y0 = gOriginY;
        float x1 = gOriginX + gHexSize * 2.0f + (m.w - 1) * gStepX;
        float y1 = gOriginY + gHexH + (m.h - 1) * gStepY + gStepY * 0.5f;
        DrawTexturePro(gBackdrop, {0, 0, (float)gBackdrop.width, (float)gBackdrop.height},
                       {x0, y0, x1 - x0, y1 - y0}, {0, 0}, 0, WHITE);
    }
    for (int y = 0; y < m.h; ++y)
        for (int x = 0; x < m.w; ++x) {
            float cx, cy;
            hexCenter(x, y, cx, cy);
            Color c = kTerrainCols[terrainAt(m, x, y)];
            if (haveBg) c.a = 96;   // keep the terrain type readable over the art
            DrawPoly({cx, cy}, 6, gHexSize - 1.0f, 0, c);
            DrawPolyLines({cx, cy}, 6, gHexSize - 1.0f, 0,
                          Fade(BLACK, haveBg ? 0.55f : 0.35f));
        }
}


static void drawBackdropArt(const Mission& m) {
    // Flat terrain fills with no seams; used to generate per-map backdrop PNGs.
    for (int y = 0; y < m.h; ++y)
        for (int x = 0; x < m.w; ++x) {
            float cx, cy;
            hexCenter(x, y, cx, cy);
            DrawPoly({cx, cy}, 6, gHexSize + 1.0f, 0, kTerrainCols[terrainAt(m, x, y)]);
        }
}

static void drawTerrainColors(const Mission& m) {
    static const Color cols[16] = {
        {16, 18, 28, 255},    // 0 void
        {40, 92, 52, 255},    // 1 grass
        {28, 64, 40, 255},    // 2 forest
        {92, 82, 62, 255},    // 3 mountain
        {28, 52, 96, 255},    // 4 water
        {58, 66, 44, 255},    // 5 plain
        {96, 96, 96, 255},    // 6 city
        {72, 72, 88, 255},    // 7 factory
        {120, 80, 40, 255},   // 8 hq
        {84, 84, 84, 255},    // 9 road
        {60, 70, 80, 255},    // 10 bridge
        {100, 60, 60, 255},   // 11 base
        {70, 60, 50, 255},    // 12 ruin
        {80, 40, 80, 255},    // 13 special
        {30, 30, 30, 255},    // 14
        {30, 30, 30, 255},    // 15
    };
    for (int y = 0; y < m.h; ++y)
        for (int x = 0; x < m.w; ++x) {
            int t = terrainAt(m, x, y);
            DrawRectangle(x * CELL, y * CELL, CELL, CELL, cols[t]);
            DrawRectangleLines(x * CELL, y * CELL, CELL, CELL, Fade(BLACK, 0.15f));
        }
}

static void drawUnitSprite(const Mission& m, const Unit& u, Texture2D playerTex, Texture2D enemyTex) {
    Texture2D& tex = (u.owner == 0) ? gUnitMapTex[u.type] : gUnitMapFlipTex[u.type];
    if (!tex.id) return;
    float size = gHexSize * 1.75f;
    float hx, hy;
    hexCenter(u.x, u.y, hx, hy);
    // raylib subtracts `origin` from the destination, so pass the centre point
    float cx = (u.px >= 0) ? u.px + size / 2 : hx;
    float cy = (u.py >= 0) ? u.py + size / 2 : hy;
    Rectangle src{0, 0, (float)tex.width, (float)tex.height};
    Rectangle dst{cx, cy, size, size};
    Color tint = (u.owner == 0) ? WHITE : Color{150, 255, 150, 255};
    DrawTexturePro(tex, src, dst, {size / 2, size / 2}, u.facing, tint);
}

// ------------------------------------------------------------------ combat screen
struct CombatView {
    bool active = false;
    int atk = -1, def = -1;
    int hpA0 = 0, hpD0 = 0, hpA1 = 0, hpD1 = 0;
    int mode = 0;
    int panelA = 0, panelB = 0;
    CombatResult r;
    float t = 0;   // animation clock (ride-in / fire / explosions)
};
static std::deque<CombatView> gCombatQueue;
static CombatView gCv;

static int terrainRemap(int t) {
    if (t >= 0 && t < (int)gRemap.size()) return gRemap[t];
    return (t < 0 || t > 15) ? 0 : t;
}

// Mirrors FUN_00415790: the two panels are drawn with param4 = 8 (first) and 0
// (second), and param2/param3 order differs per panel:
//   first  = FUN(0x180, atkTerrain, defTerrain, 8)
//   second = FUN(0x190, defTerrain, atkTerrain, 0)
// Direct tables index [remap[p2]*16 + remap[p3] + param4];
// indirect tables index [remap[p2]*2 + (param4>>3)].
static int combatPanelFor(int terrainA, int terrainB, bool indirect, bool first) {
    int a = terrainRemap(terrainA), b = terrainRemap(terrainB);
    int idx = 0;
    if (!indirect) {
        int k = a * 16 + b + (first ? 8 : 0);
        if (k >= 0 && k < (int)gPanelTable.size()) idx = gPanelTable[k];
    } else {
        int k = a * 2 + (first ? 1 : 0);
        if (k >= 0 && k < (int)gPanelIndirect.size()) idx = gPanelIndirect[k];
    }
    if (gCombatPanels.empty()) return 0;
    int n = (int)gCombatPanels.size();
    idx %= n;
    if (idx < 0) idx += n;
    return idx;
}

static void queueCombat(int atk, int def, int hpA0, int hpD0, int hpA1, int hpD1,
                        const CombatResult& r, int mode) {
    CombatView v;
    v.atk = atk;
    v.def = def;
    v.hpA0 = hpA0; v.hpD0 = hpD0; v.hpA1 = hpA1; v.hpD1 = hpD1;
    v.r = r;
    v.mode = mode;
    int ta = terrainAt(gMission, gMission.units[atk].x, gMission.units[atk].y);
    int td = terrainAt(gMission, gMission.units[def].x, gMission.units[def].y);
    v.panelA = combatPanelFor(ta, td, mode > 0, true);
    v.panelB = combatPanelFor(td, ta, mode > 0, false);
    gCombatQueue.push_back(v);
}

static void startNextCombat() {
    if (!gCv.active && !gCombatQueue.empty()) {
        gCv = gCombatQueue.front();
        gCombatQueue.pop_front();
        gCv.active = true;
    }
}

static void drawHpBar(float x, float y, float w, float h, int hp0, int hp1, float mix, Color col) {
    DrawRectangle((int)x, (int)y, (int)w, (int)h, Color{30, 30, 34, 255});
    float v0 = std::clamp(hp0 / 7.0f, 0.0f, 1.0f);
    float v1 = std::clamp(hp1 / 7.0f, 0.0f, 1.0f);
    float v = v0 + (v1 - v0) * std::clamp(mix, 0.0f, 1.0f);
    DrawRectangle((int)x, (int)y, (int)(w * v), (int)h, col);
    DrawRectangleLines((int)x, (int)y, (int)w, (int)h, Color{90, 90, 100, 255});
}

static void drawExplosion(float x, float y, float r, float phase, float alpha = 1.0f) {
    // Tuned VFX: a single explosion frame with a simple fade-out.
    alpha = std::clamp(alpha, 0.0f, 1.0f);
    if (gVfx[7].id) {
        float s = r * 2.6f;
        DrawTexturePro(gVfx[7], {0, 0, (float)gVfx[7].width, (float)gVfx[7].height},
                       {x - s / 2, y - s / 2, s, s}, {0, 0}, 0, Fade(WHITE, alpha));
        return;
    }
    if (gFxBoom.id) {
        float s = r * 1.9f;
        DrawTexturePro(gFxBoom, {0, 0, (float)gFxBoom.width, (float)gFxBoom.height},
                       {x - s / 2, y - s / 2, s, s}, {s / 2, s / 2},
                       phase * 57.29578f, Fade(WHITE, 0.95f * alpha));
        return;
    }
    DrawCircleV({x, y}, r, Fade(Color{255, 160, 30, 255}, alpha));
    DrawCircleV({x, y}, r * 0.62f, Fade(Color{255, 255, 170, 255}, alpha));
    for (int i = 0; i < 10; ++i) {
        float a = phase + i * 0.6283185f;
        float r0 = r * 0.7f + (i % 3) * 2.0f;
        float r1 = r * (1.5f + (i % 4) * 0.15f);
        DrawLineEx({x + std::cos(a) * r0, y + std::sin(a) * r0},
                   {x + std::cos(a) * r1, y + std::sin(a) * r1},
                   3, Fade(Color{255, 120, 20, 255}, alpha));
    }
}

// vfx_00 is a 2x2 sheet; cell (0,0) is a missile pointing right.
static void drawProjectile(float x, float y, float s, float angle) {
    if (!gVfx[0].id) {
        DrawCircleV({x, y}, s * 0.18f, ORANGE);
        return;
    }
    Rectangle src{0, 0, gVfx[0].width * 0.5f, gVfx[0].height * 0.5f};
    DrawTexturePro(gVfx[0], src, {x, y, s, s}, {s / 2, s / 2}, angle, WHITE);
}

static void drawEffect(Texture2D& tex, float x, float y, float s, float rot, Color tint) {
    if (!tex.id) return;
    DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height},
                   {x, y, s, s}, {s / 2, s / 2}, rot, tint);
}

// ------------------------------------------------------------------ UI font
// raylib loads the BDF directly (SUPPORT_FILEFORMAT_BDF is enabled in
// CMakeLists).  All interface text goes through uiText so the fallback default
// font still works if the asset is missing.
static void loadUiFont() {
    std::string path = gAssetDir + "fonts/not_jam_mono_clean_16.bdf";
    if (!FileExists(path.c_str())) return;
    gUiFont = LoadFontEx(path.c_str(), 16, nullptr, 0);
    if (gUiFont.texture.id) {
        SetTextureFilter(gUiFont.texture, TEXTURE_FILTER_POINT);
        gUiFontReady = true;
    }
}

static float uiFontScale(int size) {
    if (!gUiFontReady || !gUiFont.texture.id) return 2.0f;
    // Snap to an integer multiple of the font's native pixel height.
    // A 13px font then renders at 13, 26, 39, ... only.
    int scale = (int)std::lround((size * 2.0) / (double)gUiFont.baseSize);
    if (scale < 1) scale = 1;
    return (float)scale;
}

static float uiFontPixels(int size) {
    if (!gUiFontReady || !gUiFont.texture.id) return (float)(size * 2);
    return (float)gUiFont.baseSize * uiFontScale(size);
}

static float uiSpacing(int size) {
    if (!gUiFontReady || !gUiFont.texture.id) return 0.0f;
    // At least one native pixel between glyphs, scaled with the font.
    return std::max(1.0f, uiFontScale(size));
}

static void uiText(const char* text, int x, int y, int size, Color color) {
    if (gUiFontReady && gUiFont.texture.id) {
        DrawTextEx(gUiFont, text, {(float)x, (float)y}, uiFontPixels(size), uiSpacing(size), color);
    } else {
        DrawText(text, x, y, size * 2, color);
    }
}

static void drawCombatScreen(const CombatView& v, Texture2D playerTex, Texture2D enemyTex) {
    (void)playerTex;
    (void)enemyTex;
    int W = GetScreenWidth(), H = GetScreenHeight();
    const Color NAVY = {0, 0, 80, 255};
    const Color PANEL = {0, 0, 64, 255};
    const Color BORDER = {224, 192, 24, 255};
    const Color TXT = {236, 236, 240, 255};
    const Color DIM = {168, 168, 178, 255};
    ClearBackground(BLACK);

    // Original layout: a 512x480 playfield centred in the 640x480 screen.
    // Two square panels fill the width; the report bar takes what is left.
    float playW = std::min(W * 0.80f, H * 1.066667f);  // 512/480 of the screen height
    float playX = (W - playW) * 0.5f;
    float topH  = H * 0.20f;      // 96 / 480
    bool ranged = v.mode > 0;
    float gap = ranged ? playW * 0.05f : 0.0f;

    // Adjacent attacks must fill the whole combat view: make the panels square
    // and size them from the width, leaving a minimum report bar below.
    float panH = playW * 0.50f;
    float panY = topH;
    float minReport = H * 0.14f;
    if (panY + panH + minReport > H) panH = std::max(H * 0.28f, H - panY - minReport);
    float botY = panY + panH;
    float botH = H - botY;
    float panW = ranged ? (playW - gap) * 0.5f : playW * 0.5f;
    float axPanel = playX + (playW - (2.0f * panW + gap)) * 0.5f;
    float dxPanel = axPanel + panW + gap;

    const Unit& A = gMission.units[v.atk];
    const Unit& D = gMission.units[v.def];
    const UnitType& TA = gTypes[A.type];
    const UnitType& TD = gTypes[D.type];
    const CombatResult& r = v.r;

    // ----- top: names + HP (initial, then the post-combat count)
    DrawRectangle((int)playX, 0, (int)playW, (int)topH, NAVY);
    int nameSize = (int)(topH * 0.30f);
    uiText(TA.name.c_str(), (int)(playX + playW * 0.03f), (int)(topH * 0.30f), nameSize, TXT);
    float nfs = uiFontPixels(nameSize);
    float rw = gUiFontReady ? MeasureTextEx(gUiFont, TD.name.c_str(), nfs, uiSpacing(nameSize)).x
                            : (float)MeasureText(TD.name.c_str(), nameSize * 2);
    uiText(TD.name.c_str(), (int)(playX + playW * 0.97f - rw), (int)(topH * 0.30f),
           nameSize, TXT);
    auto hpBox = [&](float fx, int val) {
        float s = topH * 0.46f;
        float x = playX + playW * fx;
        float y = topH * 0.22f;
        DrawRectangleRec({x, y, s, s}, PANEL);
        DrawRectangleLinesEx({x, y, s, s}, std::max(2.0f, topH * 0.035f), BORDER);
        const char* txt = TextFormat("%d", val);
        float fs = s * 0.70f;
        if (gUiFontReady) {
            int scale = (int)std::lround(fs / (double)gUiFont.baseSize);
            if (scale < 1) scale = 1;
            fs = (float)gUiFont.baseSize * scale;
            float sp = std::max(1.0f, (float)scale);
            Vector2 m = MeasureTextEx(gUiFont, txt, fs, sp);
            DrawTextEx(gUiFont, txt, {x + (s - m.x) * 0.5f, y + s * 0.27f},
                       fs, sp, BORDER);
        } else {
            DrawText(txt, (int)(x + s * 0.33f), (int)(y + s * 0.12f),
                     (int)fs, BORDER);
        }
    };

    float t = v.t;
    // Shooting is 1.2x slower than the original 0.35 s window.
    float fireT = 2.15f, fireDur = 0.42f;
    float impactT = fireT + fireDur + 0.10f;
    int nA0 = std::clamp(v.hpA0 + 1, 0, 8);   // initial vehicles
    int nD0 = std::clamp(v.hpD0 + 1, 0, 8);
    int nA1 = (v.hpA1 > 0) ? std::clamp(v.hpA1 + 1, 0, 8) : 0;  // survivors
    int nD1 = (v.hpD1 > 0) ? std::clamp(v.hpD1 + 1, 0, 8) : 0;
    bool impact = t > impactT;
    hpBox(0.28f, impact ? nA1 : nA0);
    hpBox(0.62f, impact ? nD1 : nD0);

    // ----- middle: terrain close-ups + ride-in + simultaneous fire
    static const float FORM[8][2] = {
        {0.40f, 0.37f}, {0.23f, 0.34f}, {0.40f, 0.53f}, {0.19f, 0.47f},
        {0.36f, 0.72f}, {0.16f, 0.67f}, {0.31f, 0.89f}, {0.08f, 0.83f}
    };
    float us = std::min(panW, panH) * 0.125f;

    auto panelPoint = [&](float px, float slotX, float slotY) {
        return Vector2{px + slotX * panW, panY + slotY * panH};
    };

    auto drawPanel = [&](int idx, float x, float w) {
        if (gCombatPanels.empty() || !gCombatPanels[idx].id) return;
        Texture2D& tex = gCombatPanels[idx];
        DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height},
                       {x, panY, w, panH}, {0, 0}, 0, WHITE);
    };
    if (ranged) {
        drawPanel(v.panelA, axPanel, panW);
        drawPanel(v.panelB, dxPanel, panW);
    } else {
        // 1px overlap hides the floating-point seam between the two panels
        drawPanel(v.panelA, axPanel, panW + 1.0f);
        drawPanel(v.panelB, dxPanel, panW);
    }

    Color flashA = (t > impactT && t < impactT + 0.50f) ? Color{255, 150, 150, 255} : WHITE;
    Color flashD = flashA;
    float rk = (t > fireT && t < fireT + fireDur)
                   ? std::sin((t - fireT) / fireDur * 3.14159f)
                   : 0.0f;
    float recoilA = -us * 0.20f * rk;
    float recoilD = us * 0.20f * rk;

    // original materialise rhythm: arrival at 0.05 + i*0.235 s
    auto drawArmy = [&](float px, int count, int type, bool mirror,
                        float nudge, Color flash) {
        if (type < 0 || type >= 24) return;
        Texture2D& bt = mirror ? gUnitSprite[type] : gUnitSpriteFlip[type];
        if (!bt.id) return;
        Color team = mirror ? Color{150, 255, 150, 255} : WHITE;
        Color c = {
            (unsigned char)(team.r * flash.r / 255),
            (unsigned char)(team.g * flash.g / 255),
            (unsigned char)(team.b * flash.b / 255),
            (unsigned char)flash.a
        };
        Rectangle src = {0, 0, (float)bt.width, (float)bt.height};
        const float dur = 0.70f;
        for (int i = 0; i < count; ++i) {
            float arrive = 0.05f + i * 0.235f;
            float start = arrive - dur;
            if (t < start) continue;
            float k = std::clamp((t - start) / dur, 0.0f, 1.0f);
            float ease = 1.0f - (1.0f - k) * (1.0f - k);
            float sx = mirror ? (1.0f - FORM[i][0]) : FORM[i][0];
            Vector2 dest = panelPoint(px, sx, FORM[i][1]);
            float startX = mirror ? (float)W + us : -(float)us;
            float x = startX + (dest.x - startX) * ease;
            float y = dest.y + std::sin(k * 3.14159f * 2.0f) * us * 0.05f;
            x += nudge;
            float s = us * 1.3f * (bt.width / 32.0f);
            DrawTexturePro(bt, src, {x - s / 2, y - s / 2, s, s}, {0, 0}, 0, c);
        }
    };

    drawArmy(axPanel, impact ? nA1 : nA0, A.type, false, recoilA, flashA);
    drawArmy(dxPanel, impact ? nD1 : nD0, D.type, true, recoilD, flashD);

    // ----- simultaneous fire: every surviving shot aims at a killed vehicle
    // slot when possible, otherwise at a deterministic scatter point.
    auto scatter = [&](int side, int i) {
        int h = (i * 73 + side * 41 + 17) % 100;
        int v = (i * 47 + side * 67 + 23) % 100;
        float rx = 0.12f + 0.76f * (h / 100.0f);
        float ry = 0.14f + 0.72f * (v / 100.0f);
        return panelPoint(side == 0 ? dxPanel : axPanel, rx, ry);
    };

    Vector2 fromA[8], toA[8], fromD[8], toD[8];
    int killedD = std::max(0, nD0 - nD1);   // defender vehicles lost
    int killedA = std::max(0, nA0 - nA1);   // attacker vehicles lost
    for (int i = 0; i < nA0; ++i) {
        fromA[i] = panelPoint(axPanel, FORM[i][0], FORM[i][1]);
        if (i < killedD) {
            int slot = nD1 + i;
            toA[i] = panelPoint(dxPanel, 1.0f - FORM[slot][0], FORM[slot][1]);
        } else {
            toA[i] = scatter(0, i);
        }
    }
    for (int i = 0; i < nD0; ++i) {
        fromD[i] = panelPoint(dxPanel, 1.0f - FORM[i][0], FORM[i][1]);
        if (i < killedA) {
            int slot = nA1 + i;
            toD[i] = panelPoint(axPanel, FORM[slot][0], FORM[slot][1]);
        } else {
            toD[i] = scatter(1, i);
        }
    }

    if (t > fireT && t < fireT + fireDur) {
        auto fire = [&](Vector2* from, Vector2* to, int count, float nudge) {
            for (int i = 0; i < count; ++i) {
                float st = fireT + i * 0.012f;
                if (t < st) continue;
                float k = std::clamp((t - st) / fireDur, 0.0f, 1.0f);
                Vector2 p = {from[i].x + nudge + (to[i].x - from[i].x - nudge) * k,
                             from[i].y + (to[i].y - from[i].y) * k};
                float ang = std::atan2(to[i].y - from[i].y,
                                       to[i].x - from[i].x) * 57.29578f;
                drawProjectile(p.x, p.y, us * 0.85f, ang);
                if (k < 0.22f) {
                    Vector2 m = {from[i].x + nudge, from[i].y};
                    if (gVfx[1].id) drawEffect(gVfx[1], m.x, m.y, us * 1.0f, 0.0f, WHITE);
                    else DrawCircleV(m, us * 0.20f, Color{255, 220, 120, 255});
                }
            }
        };
        fire(fromA, toA, nA0, recoilA);
        fire(fromD, toD, nD0, recoilD);
    }

    if (t > impactT && t < impactT + 0.85f) {
        float e = (t - impactT) * 7.0f;
        for (int i = 0; i < nA0; ++i) {
            float alpha = 1.0f - std::clamp((t - impactT - i * 0.02f) / 0.85f, 0.0f, 1.0f);
            drawExplosion(toA[i].x, toA[i].y, us * 0.70f, e + i * 0.35f, alpha);
        }
        for (int i = 0; i < nD0; ++i) {
            float alpha = 1.0f - std::clamp((t - impactT - i * 0.02f) / 0.85f, 0.0f, 1.0f);
            drawExplosion(toD[i].x, toD[i].y, us * 0.70f, e + i * 0.35f, alpha);
        }
    }

    // ----- bottom: persistent simulation report (4 lines, integer-scaled)
    DrawRectangle((int)playX, (int)botY, (int)playW, (int)botH, NAVY);
    // Use the 2x font only when the report columns actually fit; otherwise the
    // 1x native font keeps the three columns from overlapping.
    int rscale = (botH >= 150.0f && playW >= 1050.0f) ? 2 : 1;
    float rfs = (float)gUiFont.baseSize * rscale;   // 32 px or 16 px
    float lh = rfs * 1.18f;
    float rsp = std::max(1.0f, (float)rscale);
    auto line = [&](float x, float y, const char* s, Color c) {
        if (gUiFontReady) DrawTextEx(gUiFont, s, {x, y}, rfs, rsp, c);
        else DrawText(s, (int)x, (int)y, (int)rfs, c);
    };

    float colL = playX + playW * 0.02f;
    float colC = playX + playW * 0.37f;
    float colR = playX + playW * 0.69f;
    float y0 = botY + std::max(botH * 0.07f, (botH - (rfs + 3.0f * lh)) * 0.5f);

    line(colL, y0, "ATTACKER", BORDER);
    line(colC, y0, "SIMULATION", BORDER);
    line(colR, y0, "DEFENDER", BORDER);
    y0 += lh;

    line(colL, y0, TextFormat("ATK %d DEF %d", r.atkStrength, r.ctrDefense), TXT);
    line(colC, y0, TextFormat("SURROUND %d%%", r.surroundPct),
         r.surroundPct ? kRed : DIM);
    line(colR, y0, TextFormat("ATK %d DEF %d", r.ctrStrength, r.defStrength), TXT);

    line(colL, y0 + lh, TextFormat("SUP+%d EXP+%d%%", r.supportAtk, r.expPctA), DIM);
    line(colC, y0 + lh, TextFormat("T%d/%d DEF%d>%d",
         r.terrainDefA, r.terrainDefD, r.defBeforeSurround, r.defAfterSurround), DIM);
    line(colR, y0 + lh, TextFormat("SUP+%d EXP+%d%%", r.supportDef, r.expPctD), DIM);

    line(colL, y0 + lh * 2, TextFormat("HP %d>%d XP+%d", v.hpA0, r.newHpAtk, r.xpToAtk), TXT);
    line(colC, y0 + lh * 2, TextFormat("DMG %d/%d", r.dmgToDef, r.dmgToAtk), TXT);
    line(colR, y0 + lh * 2, TextFormat("HP %d>%d XP+%d", v.hpD0, r.newHpDef, r.xpToDef), TXT);
}

static std::string resolveAssets(int argc, char** argv) {
    auto has = [](const std::string& c) {
        std::error_code ec;
        return std::filesystem::exists(c + "/data/units.json", ec) ||
               std::filesystem::exists(c + "\\data\\units.json", ec);
    };
    if (argc > 1 && argv[1] && argv[1][0]) {
        std::string s = argv[1];
        if (!s.empty() && s.back() != '/' && s.back() != '\\') s += '/';
        return s;
    }
    std::vector<std::string> bases;
    std::error_code ec;
    auto exe = std::filesystem::absolute(argv[0], ec);
    if (!ec) bases.push_back(exe.parent_path().string());
    bases.push_back(".");
    const char* appDir = GetApplicationDirectory();
    if (appDir && *appDir) bases.push_back(appDir);
    for (const auto& b : bases) {
        std::string base = b;
        if (!base.empty() && base.back() != '/' && base.back() != '\\') base += '/';
        for (const char* rel : {"assets", "../assets", "../../assets", "../../../assets"}) {
            std::string c = base + rel;
            if (has(c)) return c + "/";
        }
    }
    return "";
}

int main(int argc, char** argv) {
    SetTraceLogLevel(LOG_WARNING);
    gAssetDir = resolveAssets(argc, argv);
    if (gAssetDir.empty()) {
#ifdef _WIN32
        MessageBoxA(nullptr,
                    "Could not find the 'assets' folder.\n\n"
                    "Keep 'assets' next to nectaris.exe, or pass its path as the "
                    "first command-line argument.",
                    "Nectaris", 0x00000010u); // MB_OK | MB_ICONERROR
#else
        std::cerr << "assets folder not found\n";
#endif
        return 1;
    }

    bool shotMode = (argc > 3 && std::string(argv[3]) == "--shot");
    bool combatshot = (argc > 3 && std::string(argv[3]) == "--combatshot");
    bool unitshot = (argc > 3 && std::string(argv[3]) == "--unitshot");
    bool backdropshot = (argc > 3 && std::string(argv[3]) == "--backdropshot");
    std::string shotPath = (argc > 4) ? argv[4] : "port_shot.png";
    bool selftest = (argc > 3 && std::string(argv[3]) == "--selftest");

    loadData();
    std::string missionPath = (argc > 2) ? argv[2] : gAssetDir + "maps/all_units_test.json";
    if (!loadMission(missionPath)) {
        std::cerr << "cannot load mission " << missionPath << "\n";
        return 1;
    }
    gCombatRng.seed((uint32_t)std::time(nullptr));

    auto campaign = listCampaign(gAssetDir + "maps");
    int campaignIdx = 0;
    for (int i = 0; i < (int)campaign.size(); ++i)
        if (campaign[i] == missionPath) campaignIdx = i;

    if (argc > 7 && std::string(argv[3]) == "--resolvetest") {
        int a = std::atoi(argv[4]);
        int d = std::atoi(argv[5]);
        int mode = std::atoi(argv[6]);
        uint32_t seed = (uint32_t)std::strtoul(argv[7], nullptr, 0);
        CombatRng rng;
        rng.seed(seed);
        CombatResult r = resolveCombat(gMission, gTypes, gTerrain, gCombatRules,
                                       a, d, mode, rng);
        std::cout << "raw_def=" << r.rawDmgToDef << " raw_atk=" << r.rawDmgToAtk
                  << " var_def=" << r.varToDef << " var_atk=" << r.varToAtk
                  << " dmg_def=" << r.dmgToDef << " dmg_atk=" << r.dmgToAtk
                  << " hp_def=" << r.newHpDef << " hp_atk=" << r.newHpAtk
                  << " base_atk=" << r.baseAtk << " base_ctr=" << r.baseCtr
                  << " sup_atk=" << r.supportAtk << " sup_def=" << r.supportDef
                  << " atk_str=" << r.atkStrength << " def_str=" << r.defStrength
                  << " ctr_def=" << r.ctrDefense
                  << " ter_a=" << r.terrainDefA << " ter_d=" << r.terrainDefD
                  << " rank_a=" << r.rankA << " rank_d=" << r.rankD
                  << " surround=" << r.surrounded
                  << " xp_atk=" << r.xpToAtk << " xp_def=" << r.xpToDef
                  << "\n";
        return 0;
    }

    if (selftest) {
        Mission saved = gMission;
        gMission.units.clear();
        gMission.units.push_back(Unit{0, 0, 1, 1, 7, 0, false});
        gMission.units.push_back(Unit{0, 1, 2, 1, 7, 0, false});
        std::cout << "attacker,defender,dmg_def,dmg_atk,new_hp_def,new_hp_atk\n";
        CombatRng testRng;
        testRng.seed(12345);
        for (int a = 0; a < (int)gTypes.size(); ++a) {
            for (int d = 0; d < (int)gTypes.size(); ++d) {
                gMission.units[0] = Unit{a, 0, 1, 1, 7, 0, false};
                gMission.units[1] = Unit{d, 1, 2, 1, 7, 0, false};
                CombatResult r = resolveCombat(gMission, gTypes, gTerrain, gCombatRules, 0, 1, 0, testRng);
                std::cout << gTypes[a].name << "," << gTypes[d].name << ","
                          << r.dmgToDef << "," << r.dmgToAtk << ","
                          << r.newHpDef << "," << r.newHpAtk << "\n";
            }
        }
        gMission = saved;
        return 0;
    }

    if (argc > 3 && std::string(argv[3]) == "--demo") {
        for (int t = 0; t < 3; ++t) {
            for (auto& u : gMission.units) u.acted = (u.owner == 0);
            aiTurn();
        }
        return 0;
    }

    gHexSize = std::clamp(std::min(1000.0f / (1.5f * gMission.w + 1.0f),
                                   620.0f / (1.7320508f * gMission.h + 0.5f)), 14.0f, 42.0f);
    gStepX = 1.5f * gHexSize;
    gHexH = 1.7320508f * gHexSize;
    gStepY = gHexH;
    gOriginX = 24.0f;
    gOriginY = 24.0f;
    int mapW = (int)(gOriginX * 2 + gStepX * (gMission.w - 1) + 2 * gHexSize);
    int mapH = (int)(gOriginY * 2 + gStepY * (gMission.h - 1) + gHexH + gStepY * 0.5f);
    int panelW = 620;

    if (argc > 3 && std::string(argv[3]) == "--hextest") {
        int fails = 0;
        for (int y = 0; y < gMission.h; ++y)
            for (int x = 0; x < gMission.w; ++x) {
                float cx, cy;
                hexCenter(x, y, cx, cy);
                int tx, ty;
                if (!pixelToHex(cx, cy, gMission.w, gMission.h, tx, ty) ||
                    tx != x || ty != y)
                    ++fails;
            }
        std::cout << "hextest failures: " << fails << "\n";
        return fails ? 1 : 0;
    }
    if (shotMode) SetConfigFlags(FLAG_WINDOW_HIDDEN);
    if (combatshot) SetConfigFlags(FLAG_WINDOW_HIDDEN);
    if (unitshot) SetConfigFlags(FLAG_WINDOW_HIDDEN);
    if (backdropshot) SetConfigFlags(FLAG_WINDOW_HIDDEN);
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(mapW + panelW, std::max(mapH, 720), "Nectaris - portable");
    SetTargetFPS(60);
    loadUiFont();

    if (backdropshot) {
        float ox = gOriginX, oy = gOriginY;
        gOriginX = gOriginY = 0.0f;
        int bw = (int)(gStepX * (gMission.w - 1) + 2.0f * gHexSize + 2.0f);
        int bh = (int)(gStepY * (gMission.h - 1) + gHexH + gStepY * 0.5f + 2.0f);
        RenderTexture2D rt = LoadRenderTexture(bw, bh);
        BeginTextureMode(rt);
        ClearBackground(BLACK);
        drawBackdropArt(gMission);
        EndTextureMode();
        Image img = LoadImageFromTexture(rt.texture);
        ImageFlipVertical(&img);
        ExportImage(img, shotPath.c_str());
        UnloadImage(img);
        UnloadRenderTexture(rt);
        gOriginX = ox;
        gOriginY = oy;
        return 0;
    }


    Texture2D playerTex = LoadTexture((gAssetDir + "art/units_bank09.png").c_str());
    Texture2D enemyTex = LoadTexture((gAssetDir + "art/units_bank10.png").c_str());
    Texture2D bgTex = {0};
    if (!gMission.background.empty()) {
        std::string bgPath = gAssetDir + gMission.background;
        if (FileExists(bgPath.c_str())) bgTex = LoadTexture(bgPath.c_str());
    }

    gCombatPanels.clear();
    for (int i = 0; i < 24; ++i) {
        std::string p = gAssetDir + TextFormat("art/combat_terrain/panel_%02d.png", i);
        gCombatPanels.push_back(LoadTexture(p.c_str()));
    }
    gFxBoom = LoadTexture((gAssetDir + "art/combat_effects/effect_194.png").c_str());
    gFxDebris = LoadTexture((gAssetDir + "art/combat_effects/effect_196.png").c_str());
    gEnemyFlipTex = LoadTexture((gAssetDir + "art/units_bank10_flip.png").c_str());
    for (int t = 0; t < 24; ++t) {
        std::string p = gAssetDir + TextFormat("units/unit_%02d/combat_alive.png", t);
        if (!FileExists(p.c_str()))
            p = gAssetDir + TextFormat("units/unit_%02d/horizontal.png", t);
        gUnitSprite[t] = LoadTexture(p.c_str());
        Image im = LoadImage(p.c_str());
        if (im.data) {
            ImageFlipHorizontal(&im);
            gUnitSpriteFlip[t] = LoadTextureFromImage(im);
            UnloadImage(im);
        }
        std::string v = gAssetDir + TextFormat("units/unit_%02d/vertical.png", t);
        gUnitMapTex[t] = LoadTexture(v.c_str());
        Image vm = LoadImage(v.c_str());
        if (vm.data) {
            ImageFlipHorizontal(&vm);
            gUnitMapFlipTex[t] = LoadTextureFromImage(vm);
            UnloadImage(vm);
        }
    }
    for (int i = 0; i < 16; ++i) {
        gVfx[i] = LoadTexture((gAssetDir + TextFormat("art/combat_effects/vfx_%02d.png", i)).c_str());
    }
    gFxShell = LoadTexture((gAssetDir + "art/combat_effects/effect_195.png").c_str());
    gFxFlash = LoadTexture((gAssetDir + "art/combat_effects/effect_193.png").c_str());
    gFxStreak = LoadTexture((gAssetDir + "art/combat_effects/effect_195.png").c_str());

    if (unitshot) {
        int W = GetScreenWidth(), H = GetScreenHeight();
        int cols = 12, rows = 4;
        float cw = (float)W / cols, chh = (float)H / rows;
        for (int frame = 0; frame < 3; ++frame) {
            BeginDrawing();
            ClearBackground(Color{10, 10, 18, 255});
            for (int t = 0; t < 24; ++t) {
                for (int side = 0; side < 2; ++side) {
                    int idx = t + side * 24;
                    int cx = (idx % cols) * cw, cy = (idx / cols) * chh;
                    Texture2D& tex = gUnitSprite[t];
                    if (tex.id) {
                        float s = chh * 0.62f;
                        Color tint = side == 0 ? WHITE : Color{150, 255, 150, 255};
                        DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height},
                                       {cx + cw * 0.5f - s / 2, cy + chh * 0.56f - s / 2, s, s},
                                       {0, 0}, 0, tint);
                    }
                    uiText(TextFormat("%d %s", t, gTypes[t].name.c_str()),
                             (int)(cx + 3), (int)(cy + 2), (int)(chh * 0.10f),
                             side == 0 ? Color{140, 210, 255, 255} : Color{150, 255, 150, 255});
                }
            }
            EndDrawing();
        }
        TakeScreenshot(shotPath.c_str());
        return 0;
    }

    if (combatshot) {
        int shotMode = (argc > 6) ? std::atoi(argv[6]) : 0;
        int wantA = (shotMode > 0) ? 14 : 4;  // Lynx for the ranged layout demo
        int a = -1, d = -1;
        for (int i = 0; i < (int)gMission.units.size(); ++i) {
            if (gMission.units[i].hp <= 0) continue;
            if (gMission.units[i].owner == 0 && gMission.units[i].type == wantA) a = i;
            if (gMission.units[i].owner == 1 && gMission.units[i].type == 6) d = i;
        }
        if (a < 0 || d < 0) {
            a = d = -1;
            for (int i = 0; i < (int)gMission.units.size(); ++i) {
                if (gMission.units[i].hp <= 0) continue;
                if (gMission.units[i].owner == 0 && a < 0) a = i;
                if (gMission.units[i].owner == 1 && d < 0 &&
                    (a < 0 || gMission.units[i].type != gMission.units[a].type)) d = i;
            }
        }
        if (a >= 0 && d >= 0) {
            gCv.active = true;
            gCv.atk = a; gCv.def = d;
            gCv.hpA0 = gMission.units[a].hp;
            gCv.hpD0 = gMission.units[d].hp;
            gCv.mode = shotMode;
            CombatRng shotRng;
            shotRng.seed(0xC0FFEE);
            gCv.r = resolveCombat(gMission, gTypes, gTerrain, gCombatRules,
                                  a, d, gCv.mode, shotRng);
            gCv.hpA1 = gCv.r.newHpAtk;
            gCv.hpD1 = gCv.r.newHpDef;
            int ta = terrainAt(gMission, gMission.units[a].x, gMission.units[a].y);
            int td = terrainAt(gMission, gMission.units[d].x, gMission.units[d].y);
            gCv.panelA = combatPanelFor(ta, td, gCv.mode > 0, true);
            gCv.panelB = combatPanelFor(td, ta, gCv.mode > 0, false);
            gCv.t = (argc > 5) ? (float)std::atof(argv[5]) : 2.70f;
            for (int k = 0; k < 6; ++k) {
                BeginDrawing();
                drawCombatScreen(gCv, playerTex, enemyTex);
                EndDrawing();
            }
            TakeScreenshot(shotPath.c_str());
        }
        return 0;
    }

    int selected = -1;
    int currentPlayer = 0;
    bool showReach = false;
    bool debug = false;
    std::vector<int> reach;
    std::vector<int> targets;

    struct Fx { float t; float x, y; int dmg; bool onDefender; };
    std::vector<Fx> fx;
    InitAudioDevice();
    Sound sndSelect = LoadSound((gAssetDir + "audio/se01.wav").c_str());
    Sound sndMove   = LoadSound((gAssetDir + "audio/se02.wav").c_str());
    Sound sndHit    = LoadSound((gAssetDir + "audio/se07.wav").c_str());

    auto resetSelection = [&]() {
        selected = -1;
        showReach = false;
        reach.clear();
        targets.clear();
    };

    auto selectUnit = [&](int slot) {
        selected = slot;
        reach = reachable(gMission, slot);
        targets = attackable(gMission, slot);
        showReach = true;
    };

    std::cout << "loaded " << gMission.name << " " << gMission.w << "x" << gMission.h
              << ", units " << gMission.units.size() << "\n";

    int frame = 0;
    while (!WindowShouldClose()) {
        if (gCv.active) {
            gCv.t += GetFrameTime();
            BeginDrawing();
            drawCombatScreen(gCv, playerTex, enemyTex);
            EndDrawing();
            if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (gCv.t < 3.60f) {
                    gCv.t = 3.60f;      // first input skips to the result
                } else {
                    gCv.active = false;
                    startNextCombat();
                }
            }
            continue;
        }
        Vector2 mouse = GetMousePosition();
        int mx = 0, my = 0;
        bool inMap = pixelToHex(mouse.x, mouse.y, gMission.w, gMission.h, mx, my);

        if (IsKeyPressed(KEY_SPACE)) {
            resetSelection();
            if (currentPlayer == 0) {
                currentPlayer = 1;
                for (auto& u : gMission.units) u.acted = (u.owner == 0);
                aiTurn();
                startNextCombat();
            } else {
                currentPlayer = 0;
                for (auto& u : gMission.units) u.acted = (u.owner == 1);
            }
        }
        if (IsKeyPressed(KEY_R)) {
            loadMission(missionPath);
            resetSelection();
            gCombatQueue.clear();
            gCv.active = false;
            currentPlayer = 0;
        }
        if (IsKeyPressed(KEY_F1)) debug = !debug;
        if (IsKeyPressed(KEY_N) && !campaign.empty()) {
            campaignIdx = (campaignIdx + 1) % (int)campaign.size();
            loadMission(campaign[campaignIdx]);
            missionPath = campaign[campaignIdx];
            resetSelection();
            fx.clear();
            gCombatQueue.clear();
            gCv.active = false;
            currentPlayer = 0;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsKeyPressed(KEY_ESCAPE))
            resetSelection();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int hit = inMap ? unitAt(gMission, mx, my) : -1;
            if (selected < 0) {
                if (hit >= 0 && gMission.units[hit].owner == currentPlayer &&
                    !gMission.units[hit].acted) {
                    selectUnit(hit);
                    if (IsSoundValid(sndSelect)) PlaySound(sndSelect);
                }
            } else {
                Unit& sel = gMission.units[selected];
                // attack?
                if (hit >= 0 && std::find(targets.begin(), targets.end(), hit) != targets.end()) {
                    int mode = hexDistance(sel.x, sel.y, gMission.units[hit].x, gMission.units[hit].y) - 1;
                    float dx = (float)gMission.units[hit].x;
                    float dy = (float)gMission.units[hit].y;
                    float ax = (float)sel.x, ay = (float)sel.y;
                    int hpA0 = sel.hp, hpD0 = gMission.units[hit].hp;
                    CombatResult r = resolveBattle(selected, hit, mode);
                    applyCombatResult(gMission, r, gCombatRules);
                    std::cout << "[combat] " << gTypes[sel.type].name << " vs "
                              << gTypes[gMission.units[hit].type].name
                              << "  dmg " << r.dmgToDef << "/" << r.dmgToAtk
                              << "  hp " << r.newHpDef << "/" << r.newHpAtk << "\n";
                    fx.push_back({1.1f, dx, dy, r.dmgToDef, true});
                    fx.push_back({1.1f, ax, ay, r.dmgToAtk, false});
                    if (IsSoundValid(sndHit)) PlaySound(sndHit);
                    queueCombat(selected, hit, hpA0, hpD0, sel.hp, gMission.units[hit].hp,
                                r, mode);
                    startNextCombat();
                    sel.acted = true;
                    resetSelection();
                } else if (hit == selected) {
                    resetSelection();
                } else if (inMap && hit < 0 &&
                           reach[my * gMission.w + mx] <= gTypes[sel.type].movePts) {
                    float ox, oy, nx2, ny2;
                    hexCenter(sel.x, sel.y, ox, oy);
                    hexCenter(mx, my, nx2, ny2);
                    sel.facing = std::atan2(ny2 - oy, nx2 - ox) * 57.29578f - 90.0f;
                    sel.x = mx;
                    sel.y = my;
                    if (IsSoundValid(sndMove)) PlaySound(sndMove);
                    // after moving, allow an immediate attack
                    targets = attackable(gMission, selected);
                    showReach = false;
                }
            }
        }

        // smooth movement + effect timers
        float dt = GetFrameTime();
        float usz = gHexSize * 1.75f;
        for (auto& u : gMission.units) {
            float hx, hy;
            hexCenter(u.x, u.y, hx, hy);
            float tx = hx - usz / 2, ty = hy - usz / 2;
            if (u.px < 0) { u.px = tx; u.py = ty; }
            u.px += (tx - u.px) * 0.25f;
            u.py += (ty - u.py) * 0.25f;
        }
        for (auto& f : fx) f.t -= dt;
        fx.erase(std::remove_if(fx.begin(), fx.end(),
                                [](const Fx& f) { return f.t <= 0; }), fx.end());

        BeginDrawing();
        ClearBackground(Color{12, 12, 16, 255});
        drawHexMap(gMission);

        if (showReach && selected >= 0) {
            for (int y = 0; y < gMission.h; ++y)
                for (int x = 0; x < gMission.w; ++x) {
                    int idx = y * gMission.w + x;
                    if (reach[idx] <= gTypes[gMission.units[selected].type].movePts) {
                        float cx, cy;
                        hexCenter(x, y, cx, cy);
                        DrawPoly({cx, cy}, 6, gHexSize - 2, 0, Fade(SKYBLUE, 0.24f));
                    }
                }
        }
        for (int t : targets) {
            const Unit& u = gMission.units[t];
            float cx, cy;
            hexCenter(u.x, u.y, cx, cy);
            DrawPolyLinesEx({cx, cy}, 6, gHexSize - 2, 0, 2.5f, kRed);
        }

        for (const auto& u : gMission.units) {
            if (u.hp <= 0) continue;
            drawUnitSprite(gMission, u, playerTex, enemyTex);
            float cx, cy;
            hexCenter(u.x, u.y, cx, cy);
            if (u.px >= 0) { cx = u.px + usz / 2; cy = u.py + usz / 2; }
            if (u.acted)
                DrawPoly({cx, cy}, 6, gHexSize - 2, 0, Fade(BLACK, 0.35f));
            for (int i = 0; i < u.hp; ++i)
                DrawRectangle((int)(cx - u.hp * 4 + i * 8), (int)(cy + gHexSize * 0.55f),
                              6, 5, u.owner == 0 ? kGreen : kRed);
        }
        for (const auto& f : fx) {
            float a = std::clamp(f.t, 0.0f, 1.0f);
            float cx, cy;
            hexCenter((int)f.x, (int)f.y, cx, cy);
            if (f.onDefender)
                DrawPoly({cx, cy}, 6, gHexSize - 2, 0, Fade(kRed, 0.35f * a));
            if (f.dmg > 0)
                uiText(TextFormat("-%d", f.dmg), (int)(cx - 16),
                         (int)(cy - 20 - (1.0f - a) * 20), 20,
                         f.onDefender ? kRed : ORANGE);
        }
        if (selected >= 0) {
            const Unit& u = gMission.units[selected];
            float cx, cy;
            hexCenter(u.x, u.y, cx, cy);
            DrawPolyLinesEx({cx, cy}, 6, gHexSize, 0, 3.5f, YELLOW);
        }

        // side panel
        int px = mapW + 16;
        uiText("NECTARIS", px, 16, 24, RAYWHITE);
        uiText(TextFormat("Turn: %s", currentPlayer == 0 ? "PLAYER" : "ENEMY"), px, 80, 20,
                 currentPlayer == 0 ? kGreen : kRed);
        uiText(gMission.number > 0
                   ? TextFormat("Mission %d: %s", gMission.number, gMission.name.c_str())
                   : TextFormat("Mission: %s", gMission.name.c_str()),
               px, 120, 16, LIGHTGRAY);
        int alive0 = 0, alive1 = 0;
        for (auto& u : gMission.units) {
            if (u.hp > 0) (u.owner == 0 ? alive0 : alive1)++;
        }
        uiText(TextFormat("Player units: %d", alive0), px, 160, 18, RAYWHITE);
        uiText(TextFormat("Enemy units:  %d", alive1), px, 200, 18, RAYWHITE);
        if (!gMission.garrison.empty()) {
            int g0 = 0, g1 = 0;
            for (auto& u : gMission.garrison) (u.owner == 0 ? g0 : g1)++;
            uiText(TextFormat("Garrison: %d (P%d/E%d)", (int)gMission.garrison.size(), g0, g1),
                   px, 240, 16, SKYBLUE);
            if (!campaign.empty())
                uiText(TextFormat("Campaign %d/%d  [N] next", campaignIdx + 1,
                                    (int)campaign.size()), px, 268, 16, GRAY);
        } else if (!campaign.empty())
            uiText(TextFormat("Campaign %d/%d  [N] next", campaignIdx + 1,
                                (int)campaign.size()), px, 240, 16, GRAY);

        if (selected >= 0) {
            const Unit& u = gMission.units[selected];
            const UnitType& t = gTypes[u.type];
            uiText(TextFormat("%s  HP %d  MOV %d", t.name.c_str(), u.hp, t.movePts), px, 290,
                     18, YELLOW);
            uiText(TextFormat("ATK G/A %d/%d  DEF %d", t.atkGround, t.atkAir, t.def), px, 330,
                     16, LIGHTGRAY);
            uiText("LMB move / attack", px, 370, 16, GRAY);
        }
        uiText("SPACE end turn   R restart", px, 430, 16, GRAY);
        uiText("ESC / RMB cancel", px, 470, 16, GRAY);

        if (alive1 == 0) uiText("VICTORY", px, 520, 28, kGreen);
        if (alive0 == 0) uiText("DEFEAT", px, 520, 28, kRed);

        if (debug) {
            uiText(TextFormat("cell %d,%d  terrain %d", mx, my,
                                inMap ? terrainAt(gMission, mx, my) : -1),
                     px, 570, 16, LIGHTGRAY);
        }

        EndDrawing();

        if (shotMode && ++frame >= 30) {
            TakeScreenshot(shotPath.c_str());
            break;
        }
    }

    UnloadTexture(playerTex);
    UnloadTexture(enemyTex);
    if (bgTex.id != 0) UnloadTexture(bgTex);
    if (gBackdrop.id != 0) UnloadTexture(gBackdrop);
    UnloadSound(sndSelect);
    UnloadSound(sndMove);
    UnloadSound(sndHit);
    for (auto& t : gCombatPanels) if (t.id) UnloadTexture(t);
    if (gFxBoom.id) UnloadTexture(gFxBoom);
    if (gFxDebris.id) UnloadTexture(gFxDebris);
    if (gEnemyFlipTex.id) UnloadTexture(gEnemyFlipTex);
    if (gFxShell.id) UnloadTexture(gFxShell);
    if (gFxFlash.id) UnloadTexture(gFxFlash);
    if (gFxStreak.id) UnloadTexture(gFxStreak);
    for (int t = 0; t < 24; ++t) {
        if (gUnitSprite[t].id) UnloadTexture(gUnitSprite[t]);
        if (gUnitSpriteFlip[t].id) UnloadTexture(gUnitSpriteFlip[t]);
        if (gUnitMapTex[t].id) UnloadTexture(gUnitMapTex[t]);
        if (gUnitMapFlipTex[t].id) UnloadTexture(gUnitMapFlipTex[t]);
    }
    for (int i = 0; i < 16; ++i) if (gVfx[i].id) UnloadTexture(gVfx[i]);
    if (gUiFontReady) UnloadFont(gUiFont);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}




