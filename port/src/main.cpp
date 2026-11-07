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

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
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

// ------------------------------------------------------------------ data
struct UnitType {
    int id = 0;
    std::string name;
    int atkGround = 0, atkAir = 0, def = 0;
    int ctrGround = 1, ctrAir = 1;
    int movePts = 0, moveClass = 0, ability = 0, maxHp = 7;
};

struct TerrainDef {
    int def = 0;
    int cost[4] = {1, 1, 1, 1};
};

struct Unit {
    int type = 0;
    int owner = 0;          // 0 player, 1 enemy
    int x = 0, y = 0;
    int hp = 7;             // 0..7
    int rank = 0;           // 0..7 (damage multiplier tier)
    bool acted = false;
    float px = -1, py = -1; // smoothed pixel position
    float facing = 0;       // map icon rotation (degrees)
};

struct Mission {
    std::string name;
    int w = 0, h = 0;
    std::vector<int> terrain;   // w*h, 0..15
    std::vector<Unit> units;
    std::string background;
};

static std::vector<UnitType> gTypes;
static std::vector<TerrainDef> gTerrain;
static std::vector<int> gHpMult;
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

static const char* kGameDir = nullptr;
static std::string gAssetDir;

// ------------------------------------------------------------------ hex
// Odd-column offset hex (matches FUN_00403490).
static const int DX[6] = {0, 1, 1, 0, -1, -1};
static const int DY_EVEN[6] = {-1, -1, 0, 1, 0, -1};
static const int DY_ODD[6] = {-1, 0, 1, 1, 1, 0};

static inline void hexNeighbor(int x, int y, int d, int& nx, int& ny) {
    nx = x + DX[d];
    ny = y + ((x & 1) ? DY_ODD[d] : DY_EVEN[d]);
}

static inline bool inBounds(const Mission& m, int x, int y) {
    return x >= 0 && y >= 0 && x < m.w && y < m.h;
}

static inline int terrainAt(const Mission& m, int x, int y) {
    if (!inBounds(m, x, y)) return 0;
    int t = m.terrain[y * m.w + x];
    return (t < 0 || t > 15) ? 0 : t;
}

static inline int moveCost(const Mission& m, int x, int y, int moveClass) {
    int c = gTerrain[terrainAt(m, x, y)].cost[std::clamp(moveClass, 0, 3)];
    return c;
}

static int unitAt(const Mission& m, int x, int y) {
    for (int i = 0; i < (int)m.units.size(); ++i) {
        const Unit& u = m.units[i];
        if (u.hp > 0 && u.x == x && u.y == y) return i;
    }
    return -1;
}

static bool adjacent(int x1, int y1, int x2, int y2) {
    for (int d = 0; d < 6; ++d) {
        int nx, ny;
        hexNeighbor(x1, y1, d, nx, ny);
        if (nx == x2 && ny == y2) return true;
    }
    return false;
}

// Odd-q offset -> cube distance.
static int hexDistance(int x1, int y1, int x2, int y2) {
    auto cube = [](int x, int y, int& q, int& r) {
        q = x;
        r = y - (x - (x & 1)) / 2;
    };
    int q1, r1, q2, r2;
    cube(x1, y1, q1, r1);
    cube(x2, y2, q2, r2);
    int dq = q1 - q2, dr = r1 - r2, ds = (-q1 - r1) - (-q2 - r2);
    return (std::abs(dq) + std::abs(dr) + std::abs(ds)) / 2;
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
    gHpMult.clear();
    for (const auto& v : cb["hp_damage_multiplier"]) gHpMult.push_back(v.get<int>());
    if (gHpMult.empty()) gHpMult = {100, 105, 110, 120, 130, 140, 160, 200};

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
    std::vector<std::string> out;
    std::error_code ec;
    for (const auto& e : std::filesystem::directory_iterator(dir, ec)) {
        std::string n = e.path().filename().string();
        if (n.size() == 9 && (n[0] == 'M') && n[1] >= 'a' && n[1] <= 'd' &&
            std::isdigit((unsigned char)n[2]) && std::isdigit((unsigned char)n[3]) &&
            n.substr(4) == ".json") {
            out.push_back(e.path().string());
        }
    }
    std::sort(out.begin(), out.end());
    return out;
}

static bool loadMission(const std::string& path) {    json j = loadJson(path);
    gMission = Mission{};
    gMission.name = j.value("name", std::string("mission"));
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
        u.rank = ju.value("rank", 0);
        gMission.units.push_back(u);
    }
    return true;
}

// ------------------------------------------------------------------ combat
struct CombatResult {
    int dmgToDef = 0, dmgToAtk = 0;
    int newHpDef = 0, newHpAtk = 0;
    bool defDied = false, atkDied = false;
    int atkStrength = 0, defStrength = 0;
    int ctrStrength = 0, ctrDefStrength = 0;
    int terrainA = 0, terrainB = 0;
};

static int varianceMultiplier() {
    int r = std::rand() % 100;
    if (r >= 20 && r < 25) return 15;
    if (r >= 60 && r < 65) return 20;
    if (r >= 80 && r < 83) return 2;
    if (r >= 98) return 40;
    return (150 - r) / 10;
}

// Port of FUN_0041b810 + FUN_00420cd0.
static CombatResult resolveCombat(int ai, int di, int mode) {
    CombatResult out;
    Unit& A = gMission.units[ai];
    Unit& D = gMission.units[di];
    const UnitType& TA = gTypes[A.type];
    const UnitType& TD = gTypes[D.type];

    auto atkVs = [&](int attSlot, int defSlot) -> int {
        const UnitType& at = gTypes[gMission.units[attSlot].type];
        const UnitType& de = gTypes[gMission.units[defSlot].type];
        return (de.moveClass == 4) ? at.atkAir : at.atkGround;
    };

    const int hA = A.hp + 1;
    const int hD = D.hp + 1;

    const int canA = (TD.moveClass == 4) ? TA.ctrAir : TA.ctrGround;
    const int canD = (TA.moveClass == 4) ? TD.ctrAir : TD.ctrGround;

    int baseA = 0, baseD = 0;
    if (mode == 0) {
        if (canA < 2) baseA = atkVs(ai, di);
        if (canD == 1) baseD = atkVs(di, ai);
    } else {
        if (canA >= 2) baseA = atkVs(ai, di);
        baseD = 0;
    }

    const int defA = TA.def;
    const int defD = TD.def;
    const int terA = gTerrain[terrainAt(gMission, A.x, A.y)].def;
    const int terD = gTerrain[terrainAt(gMission, D.x, D.y)].def;

    int supA = 0, supD = 0;
    for (int d = 0; d < 6; ++d) {
        int nx, ny;
        hexNeighbor(D.x, D.y, d, nx, ny);
        int s = unitAt(gMission, nx, ny);
        if (s >= 0 && s != ai && gMission.units[s].owner == A.owner)
            supA += atkVs(s, di) * (gMission.units[s].hp + 1);
    }
    supA /= (hA * 2);
    for (int d = 0; d < 6; ++d) {
        int nx, ny;
        hexNeighbor(A.x, A.y, d, nx, ny);
        int s = unitAt(gMission, nx, ny);
        if (s >= 0 && s != di && gMission.units[s].owner == D.owner)
            supD += gTypes[gMission.units[s].type].def * (gMission.units[s].hp + 1);
    }
    supD /= (hD * 2);

    const int atkStr = std::min(100, baseA + supA);
    const int defStr = std::min(100, defD + terD + supD);

    const int hpMultA = gHpMult[std::clamp(A.rank, 0, (int)gHpMult.size() - 1)];
    const int hpMultD = gHpMult[std::clamp(D.rank, 0, (int)gHpMult.size() - 1)];

    int dmgToD = hA * hpMultA * ((100 - defStr) * atkStr / 100) / 100;
    int ctrDef = std::min(100, defA + terA);
    int ctrVal = hpMultD * ((100 - ctrDef) * baseD / 100) / 100;
    int dmgToA = hD * ctrVal;

    dmgToD = varianceMultiplier() * dmgToD / 10;
    dmgToA = varianceMultiplier() * dmgToA / 10;

    auto apply = [](int hp, int dmg) {
        int bonus = (hp >= 2) ? 50 : 0;
        int nh = (bonus + hp * 100 - dmg) / 100;
        if (nh > 8) nh = 0;
        return std::clamp(nh, 0, 7);
    };

    out.dmgToDef = dmgToD;
    out.dmgToAtk = dmgToA;
    out.newHpDef = apply(D.hp, dmgToD);
    out.newHpAtk = apply(A.hp, dmgToA);
    out.defDied = out.newHpDef <= 0;
    out.atkDied = out.newHpAtk <= 0;
    out.atkStrength = atkStr;
    out.defStrength = defStr;
    out.ctrStrength = baseD;
    out.ctrDefStrength = ctrDef;
    out.terrainA = terA;
    out.terrainB = terD;

    A.hp = out.newHpAtk;
    D.hp = out.newHpDef;
    return out;
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

static std::vector<int> attackable(const Mission& m, int slot) {
    std::vector<int> out;
    const Unit& u = m.units[slot];
    const UnitType& t = gTypes[u.type];
    for (int i = 0; i < (int)m.units.size(); ++i) {
        if (i == slot || m.units[i].hp <= 0) continue;
        const Unit& e = m.units[i];
        if (e.owner == u.owner) continue;
        bool air = gTypes[e.type].moveClass == 4;
        int atk = air ? t.atkAir : t.atkGround;
        int range = air ? t.ctrAir : t.ctrGround;
        if (atk == 0 || range < 1) continue;
        int d = hexDistance(u.x, u.y, e.x, e.y);
        // direct-fire units only hit adjacent; indirect units only hit rings 2..range
        if (range == 1) {
            if (d == 1) out.push_back(i);
        } else if (d >= 2 && d <= range) {
            out.push_back(i);
        }
    }
    return out;
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
            CombatResult r = resolveCombat(i, tgt, mode);
            queueCombat(i, tgt, hpA0, hpD0, u.hp, D.hp, r, mode);
            std::cout << "[AI] " << gTypes[u.type].name << " -> " << gTypes[D.type].name
                      << "  dmg " << r.dmgToDef << "/" << r.dmgToAtk
                      << "  hp " << D.hp << "/" << u.hp << "\n";
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

static void drawHexMap(const Mission& m) {
    for (int y = 0; y < m.h; ++y)
        for (int x = 0; x < m.w; ++x) {
            float cx, cy;
            hexCenter(x, y, cx, cy);
            DrawPoly({cx, cy}, 6, gHexSize - 1.0f, 0, kTerrainCols[terrainAt(m, x, y)]);
            DrawPolyLines({cx, cy}, 6, gHexSize - 1.0f, 0, Fade(BLACK, 0.35f));
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
    int dmgDef = 0, dmgAtk = 0;
    int mode = 0;
    int panelA = 0, panelB = 0;
    int atkStrength = 0, defStrength = 0, ctrStrength = 0, ctrDefStrength = 0;
    int terrainA = 0, terrainB = 0;
    float t = 0;
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
    v.dmgDef = r.dmgToDef; v.dmgAtk = r.dmgToAtk;
    v.atkStrength = r.atkStrength; v.defStrength = r.defStrength;
    v.ctrStrength = r.ctrStrength; v.ctrDefStrength = r.ctrDefStrength;
    v.terrainA = r.terrainA; v.terrainB = r.terrainB;
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
        gCv.t = 0;
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

static void drawExplosion(float x, float y, float r, float phase) {
    int fi = std::clamp((int)(phase * 3.0f), 0, 15);
    if (gVfx[fi].id) {
        float s = r * 2.6f;
        DrawTexturePro(gVfx[fi], {0, 0, (float)gVfx[fi].width, (float)gVfx[fi].height},
                       {x - s / 2, y - s / 2, s, s}, {0, 0}, 0, WHITE);
        return;
    }
    if (gFxBoom.id) {
        float s = r * 1.9f;
        DrawTexturePro(gFxBoom, {0, 0, (float)gFxBoom.width, (float)gFxBoom.height},
                       {x - s / 2, y - s / 2, s, s}, {s / 2, s / 2},
                       phase * 57.29578f, Fade(WHITE, 0.95f));
        return;
    }
    DrawCircleV({x, y}, r, Color{255, 160, 30, 255});
    DrawCircleV({x, y}, r * 0.62f, Color{255, 255, 170, 255});
    for (int i = 0; i < 10; ++i) {
        float a = phase + i * 0.6283185f;
        float r0 = r * 0.7f + (i % 3) * 2.0f;
        float r1 = r * (1.5f + (i % 4) * 0.15f);
        DrawLineEx({x + std::cos(a) * r0, y + std::sin(a) * r0},
                   {x + std::cos(a) * r1, y + std::sin(a) * r1},
                   3, Color{255, 120, 20, 255});
    }
}

static void drawEffect(Texture2D& tex, float x, float y, float s, float rot, Color tint) {
    if (!tex.id) return;
    DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height},
                   {x, y, s, s}, {s / 2, s / 2}, rot, tint);
}

static void drawCombatScreen(const CombatView& v, Texture2D playerTex, Texture2D enemyTex) {
    int W = GetScreenWidth(), H = GetScreenHeight();
    const Color NAVY = {0, 0, 80, 255};
    const Color PANEL = {0, 0, 64, 255};
    const Color BORDER = {224, 192, 24, 255};
    const Color TXT = {236, 236, 240, 255};
    const Color DIM = {168, 168, 178, 255};
    ClearBackground(BLACK);

    // Original layout: a 512x480 playfield centred in the 640x480 screen.
    // The combat field is exactly two 256x256 panels (logical 128x128 each).
    float playW = std::min(W * 0.80f, H * 1.066667f);  // 512/480 of the screen height
    float playX = (W - playW) * 0.5f;
    float topH  = H * 0.20f;      // 96 / 480
    float pan   = playW * 0.50f;  // 256 physical, also 128 logical
    float panY  = H * 0.30f;      // 144 / 480
    float botY  = H * 0.8333f;    // 400 / 480
    float botH  = H - botY;

    const Unit& A = gMission.units[v.atk];
    const Unit& D = gMission.units[v.def];
    const UnitType& TA = gTypes[A.type];
    const UnitType& TD = gTypes[D.type];

    // ----- top: names + EXP
    DrawRectangle((int)playX, 0, (int)playW, (int)topH, NAVY);
    int nameSize = (int)(topH * 0.30f);
    DrawText(TA.name.c_str(), (int)(playX + playW * 0.03f), (int)(topH * 0.34f), nameSize, TXT);
    DrawText(TD.name.c_str(), (int)(playX + playW * 0.82f), (int)(topH * 0.34f), nameSize, TXT);
    int expSize = (int)(topH * 0.22f);
    DrawText("EXP", (int)(playX + playW * 0.255f), (int)(topH * 0.38f), expSize, TXT);
    DrawText("EXP", (int)(playX + playW * 0.715f), (int)(topH * 0.38f), expSize, TXT);
    auto expBox = [&](float fx, int val) {
        float s = topH * 0.46f;
        float x = playX + playW * fx;
        float y = topH * 0.22f;
        DrawRectangleRec({x, y, s, s}, PANEL);
        DrawRectangleLinesEx({x, y, s, s}, std::max(2.0f, topH * 0.035f), BORDER);
        DrawText(TextFormat("%d", val), (int)(x + s * 0.33f), (int)(y + s * 0.12f),
                 (int)(s * 0.70f), BORDER);
    };
    expBox(0.335f, (v.dmgAtk > 0 && v.t > 2.45f) ? v.hpA1 + 1 : v.hpA0 + 1);
    expBox(0.613f, (v.t > 2.45f) ? v.hpD1 + 1 : v.hpD0 + 1);

    // ----- middle: black field, two colour-keyed terrain panels
    float t = v.t;
    int nA = (v.dmgAtk > 0 && t > 2.45f) ? (v.hpA1 + 1) : (v.hpA0 + 1);
    int nD = (t > 2.45f) ? (v.hpD1 + 1) : (v.hpD0 + 1);
    nA = std::clamp(nA, 0, 8);
    nD = std::clamp(nD, 0, 8);

    // 8 formation slots, staggered 2 x 4, mirrored for the defender.  These
    // coordinates come from the real battle capture (frame_00219).
    static const float FORM[8][2] = {
        {0.40f, 0.37f}, {0.23f, 0.34f}, {0.40f, 0.53f}, {0.19f, 0.47f},
        {0.36f, 0.72f}, {0.16f, 0.67f}, {0.31f, 0.89f}, {0.08f, 0.83f}
    };
    float us = pan * 0.125f;
    float axPanel = playX;
    float dxPanel = playX + pan;

    // The original materialises the vehicles one by one over ~2 s.
    auto panelPoint = [&](float px, float slotX, float slotY) {
        return Vector2{px + slotX * pan, panY + slotY * pan};
    };

    // units ride in from the screen edges and arrive at their slot at the time
    // they used to pop in (0.05 + i*0.235 s)
    auto drawArmy = [&](float px, int count, int type, Texture2D& tex, Color flash,
                        float nudge, bool mirror) {
        Texture2D& bt = (type >= 0 && type < 24)
                            ? (mirror ? gUnitSprite[type] : gUnitSpriteFlip[type])
                            : tex;
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
            float s = us * 1.3f;
            DrawTexturePro(bt, src, {x - s / 2, y - s / 2, s, s}, {0, 0}, 0, c);
        }
    };

    float fireT = 2.15f, fireDur = 0.35f;
    float rk = (t > fireT && t < fireT + fireDur)
                   ? std::sin((t - fireT) / fireDur * 3.14159f)
                   : 0.0f;
    float recoilA = -us * 0.22f * rk;
    float recoilD = us * 0.22f * rk;
    Color tintA = (t > 2.45f && t < 2.95f) ? Color{255, 140, 140, 255} : WHITE;
    Color tintD = tintA;

    // draw terrain first, then armies on top
    auto drawPanel = [&](int idx, float x) {
        if (gCombatPanels.empty() || !gCombatPanels[idx].id) return;
        Texture2D& tex = gCombatPanels[idx];
        DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height},
                       {x, panY, pan, pan}, {0, 0}, 0, WHITE);
    };
    drawPanel(v.panelA, axPanel);
    drawPanel(v.panelB, dxPanel);
    if (v.panelA != v.panelB)
        DrawRectangle((int)(dxPanel) - 1, (int)panY, 2, (int)pan, BLACK);

    drawArmy(axPanel, nA, A.type, playerTex, tintA, recoilA, false);
    drawArmy(dxPanel, nD, D.type, enemyTex, tintD, recoilD, true);

    // ----- both sides fire at the same time
    if (t > fireT && t < fireT + fireDur) {
        float k = (t - fireT) / fireDur;
        Vector2 pa = panelPoint(axPanel, FORM[0][0], FORM[0][1]);
        pa.x += recoilA;
        drawEffect(gVfx[1], pa.x + us * 0.5f, pa.y, us * 1.15f, 0.0f, WHITE);
        float ax0 = pa.x + us * 0.4f, ax1 = dxPanel + pan * 0.60f;
        drawEffect(gVfx[2], ax0 + (ax1 - ax0) * k, pa.y, us, 0.0f, WHITE);

        Vector2 pd = panelPoint(dxPanel, 1.0f - FORM[0][0], FORM[0][1]);
        pd.x += recoilD;
        drawEffect(gVfx[1], pd.x - us * 0.5f, pd.y, us * 1.15f, 0.0f, WHITE);
        float dx0 = pd.x - us * 0.4f, dx1 = axPanel + pan * 0.40f;
        drawEffect(gVfx[2], dx0 + (dx1 - dx0) * k, pd.y, us, 0.0f, WHITE);
    }

    // ----- explosions on both sides at the same time
    if (t > 2.45f && t < 3.20f) {
        for (int side = 0; side < 2; ++side) {
            float pxp = side == 0 ? axPanel : dxPanel;
            for (int i = 0; i < 4; ++i) {
                int j = i * 2;
                float sx = side == 0 ? FORM[j][0] : (1.0f - FORM[j][0]);
                Vector2 q = panelPoint(pxp, sx, FORM[j][1]);
                float ph = (t - 2.45f) * 7.0f + i * 1.6f;
                drawExplosion(q.x + std::sin(ph) * us * 0.5f,
                              q.y + std::cos(ph * 1.3f) * us * 0.4f,
                              us * (0.55f + 0.15f * std::sin(ph * 2.0f)), ph);
            }
        }
    }

    if (v.dmgDef > 0 && t > 2.45f)
        DrawText(TextFormat("-%d", v.dmgDef), (int)(dxPanel + pan * 0.30f), (int)(panY + 6),
                 (int)(H * 0.042f), Color{255, 90, 90, 255});
    if (v.dmgAtk > 0 && t > 2.45f)
        DrawText(TextFormat("-%d", v.dmgAtk), (int)(axPanel + pan * 0.06f), (int)(panY + 6),
                 (int)(H * 0.042f), ORANGE);

    // ----- bottom: attack / defense / terrain
    DrawRectangle((int)playX, (int)botY, (int)playW, (int)botH, NAVY);
    auto statBox = [&](float x, float w, const char* l1, const char* l2) {
        float h = botH * 0.72f;
        float y = botY + botH * 0.14f;
        DrawRectangleRec({x, y, w, h}, PANEL);
        DrawRectangleLinesEx({x, y, w, h}, std::max(2.0f, botH * 0.06f), BORDER);
        int fs = (int)(botH * 0.24f);
        DrawText(l1, (int)(x + w * 0.06f), (int)(y + h * 0.16f), fs, TXT);
        DrawText(l2, (int)(x + w * 0.06f), (int)(y + h * 0.52f), fs, TXT);
    };
    statBox(playX + playW * 0.02f, playW * 0.30f,
            TextFormat("ATK %d", v.atkStrength), TextFormat("DEF %d", v.defStrength));
    statBox(playX + playW * 0.68f, playW * 0.30f,
            TextFormat("ATK %d", v.ctrStrength), TextFormat("DEF %d", v.ctrDefStrength));

    DrawText("TERRAIN", (int)(playX + playW * 0.42f), (int)(botY + botH * 0.10f),
             (int)(botH * 0.20f), TXT);
    DrawText(TextFormat("%d%%", gTerrain[std::clamp(v.terrainA, 0, 15)].def),
             (int)(playX + playW * 0.40f), (int)(botY + botH * 0.52f), (int)(botH * 0.22f), DIM);
    DrawText(TextFormat("%d%%", gTerrain[std::clamp(v.terrainB, 0, 15)].def),
             (int)(playX + playW * 0.55f), (int)(botY + botH * 0.52f), (int)(botH * 0.22f), DIM);

    DrawText("SPACE / click", (int)(playX + playW * 0.40f), (int)(H - botH * 0.20f),
             (int)(botH * 0.14f), DIM);
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
    std::string shotPath = (argc > 4) ? argv[4] : "port_shot.png";
    bool selftest = (argc > 3 && std::string(argv[3]) == "--selftest");

    loadData();
    std::string missionPath = (argc > 2) ? argv[2] : gAssetDir + "maps/all_units_test.json";
    if (!loadMission(missionPath)) {
        std::cerr << "cannot load mission " << missionPath << "\n";
        return 1;
    }

    auto campaign = listCampaign(gAssetDir + "maps");
    int campaignIdx = 0;
    for (int i = 0; i < (int)campaign.size(); ++i)
        if (campaign[i] == missionPath) campaignIdx = i;

    if (selftest) {
        Mission saved = gMission;
        gMission.units.clear();
        gMission.units.push_back(Unit{0, 0, 1, 1, 7, 0, false});
        gMission.units.push_back(Unit{0, 1, 2, 1, 7, 0, false});
        std::cout << "attacker,defender,dmg_def,dmg_atk,new_hp_def,new_hp_atk\n";
        for (int a = 0; a < (int)gTypes.size(); ++a) {
            for (int d = 0; d < (int)gTypes.size(); ++d) {
                gMission.units[0] = Unit{a, 0, 1, 1, 7, 0, false};
                gMission.units[1] = Unit{d, 1, 2, 1, 7, 0, false};
                CombatResult r = resolveCombat(0, 1, 0);
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
    int panelW = 360;

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
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(mapW + panelW, std::max(mapH, 720), "Nectaris - portable");
    SetTargetFPS(60);


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
        std::string p = gAssetDir + TextFormat("units/unit_%02d/horizontal.png", t);
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
                    DrawText(TextFormat("%d %s", t, gTypes[t].name.c_str()),
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
        int a = -1, d = -1;
        for (int i = 0; i < (int)gMission.units.size(); ++i) {
            if (gMission.units[i].hp <= 0) continue;
            if (gMission.units[i].owner == 0 && gMission.units[i].type == 4) a = i;
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
            gCv.hpA0 = 7; gCv.hpD0 = 7; gCv.hpA1 = 7; gCv.hpD1 = 4;
            gCv.dmgDef = 312; gCv.dmgAtk = 148; gCv.mode = 0;
            gCv.atkStrength = 720; gCv.defStrength = 240;
            gCv.ctrStrength = 560; gCv.ctrDefStrength = 400;
            gCv.terrainA = terrainAt(gMission, gMission.units[a].x, gMission.units[a].y);
            gCv.terrainB = terrainAt(gMission, gMission.units[d].x, gMission.units[d].y);
            int ta = terrainAt(gMission, gMission.units[a].x, gMission.units[a].y);
            int td = terrainAt(gMission, gMission.units[d].x, gMission.units[d].y);
            gCv.panelA = combatPanelFor(ta, td, false, true);
            gCv.panelB = combatPanelFor(td, ta, false, false);
            gCv.t = 2.7f;
            if (argc > 5) gCv.t = (float)atof(argv[5]);
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
            if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                gCv.t = 10.0f;
            BeginDrawing();
            drawCombatScreen(gCv, playerTex, enemyTex);
            EndDrawing();
            if (gCv.t > 4.6f) {
                gCv.active = false;
                startNextCombat();
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
                    CombatResult r = resolveCombat(selected, hit, mode);
                    std::cout << "[combat] " << gTypes[sel.type].name << " vs "
                              << gTypes[gMission.units[hit].type].name
                              << "  dmg " << r.dmgToDef << "/" << r.dmgToAtk
                              << "  hp " << gMission.units[hit].hp << "/" << sel.hp << "\n";
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
            DrawPolyLinesEx({cx, cy}, 6, gHexSize - 2, 0, 2.5f, RED);
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
                              6, 5, u.owner == 0 ? GREEN : RED);
        }
        for (const auto& f : fx) {
            float a = std::clamp(f.t, 0.0f, 1.0f);
            float cx, cy;
            hexCenter((int)f.x, (int)f.y, cx, cy);
            if (f.onDefender)
                DrawPoly({cx, cy}, 6, gHexSize - 2, 0, Fade(RED, 0.35f * a));
            if (f.dmg > 0)
                DrawText(TextFormat("-%d", f.dmg), (int)(cx - 16),
                         (int)(cy - 20 - (1.0f - a) * 20), 20,
                         f.onDefender ? RED : ORANGE);
        }
        if (selected >= 0) {
            const Unit& u = gMission.units[selected];
            float cx, cy;
            hexCenter(u.x, u.y, cx, cy);
            DrawPolyLinesEx({cx, cy}, 6, gHexSize, 0, 3.5f, YELLOW);
        }

        // side panel
        int px = mapW + 16;
        DrawText("NECTARIS", px, 20, 24, RAYWHITE);
        DrawText(TextFormat("Turn: %s", currentPlayer == 0 ? "PLAYER" : "ENEMY"), px, 60, 20,
                 currentPlayer == 0 ? GREEN : RED);
        DrawText(TextFormat("Mission: %s", gMission.name.c_str()), px, 90, 16, LIGHTGRAY);
        int alive0 = 0, alive1 = 0;
        for (auto& u : gMission.units) {
            if (u.hp > 0) (u.owner == 0 ? alive0 : alive1)++;
        }
        DrawText(TextFormat("Player units: %d", alive0), px, 120, 18, RAYWHITE);
        DrawText(TextFormat("Enemy units:  %d", alive1), px, 144, 18, RAYWHITE);
        if (!campaign.empty())
            DrawText(TextFormat("Campaign %d/%d  [N] next", campaignIdx + 1,
                                (int)campaign.size()), px, 172, 16, GRAY);

        if (selected >= 0) {
            const Unit& u = gMission.units[selected];
            const UnitType& t = gTypes[u.type];
            DrawText(TextFormat("%s  HP %d  MOV %d", t.name.c_str(), u.hp, t.movePts), px, 190,
                     18, YELLOW);
            DrawText(TextFormat("ATK G/A %d/%d  DEF %d", t.atkGround, t.atkAir, t.def), px, 214,
                     16, LIGHTGRAY);
            DrawText("LMB move / attack", px, 250, 16, GRAY);
        }
        DrawText("SPACE end turn   R restart", px, 300, 16, GRAY);
        DrawText("ESC / RMB cancel", px, 320, 16, GRAY);

        if (alive1 == 0) DrawText("VICTORY", px, 380, 28, GREEN);
        if (alive0 == 0) DrawText("DEFEAT", px, 380, 28, RED);

        if (debug) {
            DrawText(TextFormat("cell %d,%d  terrain %d", mx, my,
                                inMap ? terrainAt(gMission, mx, my) : -1),
                     px, 420, 16, LIGHTGRAY);
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
    CloseAudioDevice();
    CloseWindow();
    return 0;
}




