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
// Controls: LMB select / move / attack, RMB pan, ESC cancel, SPACE end turn, R restart, F1 debug.

#include "raylib.h"
#include "rlgl.h"
#include <nlohmann/json.hpp>

#include "combat.h"
#include "model.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <ctime>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
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
static json gFactoryData;
static bool gFactoryDataLoaded = false;

static std::vector<std::string> gTerrainNames;
static std::vector<int> gRemap, gPanelTable, gPanelIndirect;
static std::vector<Texture2D> gCombatPanels;
static Texture2D gFxBoom = {0}, gFxDebris = {0};
static Texture2D gEnemyFlipTex = {0};
static Texture2D gFxShell = {0}, gFxFlash = {0}, gFxStreak = {0};
static Texture2D gUnitSprite[24];
static Texture2D gUnitSpriteFlip[24];
static Texture2D gUnitMapTex[24], gUnitMapFlipTex[24];
static Texture2D gVfxHalf[64];
static Texture2D gVfxQuad[16];
static Texture2D gVfxSplat = {0};   // shared wreck: mine's dead sprite
static bool gDebugWire = false;     // F1: polygon wireframe / double-sided debug
static Shader gUnitRecolor = {0};   // luminance recolor for enemy units
static int gUnitTintLoc = -1;
static int gUnitAmountLoc = -1;
static int gUnitLutFromLoc = -1;
static int gUnitLutToLoc = -1;
static int gUnitLutCountLoc = -1;
// Per unit type: exact blue -> green palette pairs taken from the game art.
static std::vector<std::pair<Vector3, Vector3>> gUnitLut[24];
// Packed impostor frames (all directional frames in one atlas).
struct ImpostorRect {
    int x = 0, y = 0, w = 0, h = 0;
};
static Texture2D gImpostorAtlas = {0};
static std::vector<ImpostorRect> gImpostorRects[24];
// Compass frame table per unit: E, SE, S, SW, W, NW, N, NE.
static int gImpostorDirs[24][8] = {};
static int gImpostorFlips[24][8] = {};
static float gImpostorScale[24] = {};
// Heavy ground units that always enter side-on (Giant, Atlas) instead of
// turning toward the slot.
static bool gImpostorSide[24] = {};

// Combat VFX config (assets/data/combat_vfx.json).  Frames refer to resource-23
// half frames; quads are groups of four consecutive halves assembled 2x2.
struct CombatVfxClass {
    std::vector<int> death;
    float scale = 1.0f;
};
struct CombatUnitVfx {
    std::string cls;
    std::vector<int> projectile[2];   // optional per-unit override, 0=left 1=right
    std::vector<int> muzzle[2];
    std::vector<int> death;
    float scale = -1.0f;
};
static std::map<std::string, CombatVfxClass> gVfxClasses;
struct EntryPath {
    float start[2] = {0, 0};
    float mid[2] = {0, 0};
};
static std::map<std::string, EntryPath> gEntryPaths;
// Real per-slot entry curves captured from the live sprite-object table
// (tools/build_entry_curves.py).  Positions are panel normalised 0..1.
struct EntryCurve {
    Vector2 slot = {0, 0};
    Vector2 start = {0, 0};
    float t0 = 0.0f;
    float duration = 0.0f;
    std::vector<Vector3> pts;   // x = time, y = u, z = v
};
static std::map<std::string, std::vector<EntryCurve>> gEntryCurves;
// Editable perspective/sprite settings (assets/data/combat_perspective.json):
// per slot: sprite variant number, scale, settled position; plus a global
// entry-speed multiplier.
struct PerspSlot {
    Vector2 slot = {0, 0};
    int sprite = -1;      // -1 = auto direction from movement, else frame index
    int spriteEnd = -1;   // optional frame once settled (-1 = keep the entry one)
    int flip = -1;        // -1 = auto from movement, 0/1 = force
    float scale = 1.0f;   // multiplier on the depth-based scale
};
static std::map<std::string, std::vector<PerspSlot>> gPersp;
static float gEntrySpeed = 1.0f;

// Hex direction order of the unit frames: 0=E, 1=NE, 2=N, 5=SE.  The sheet only
// contains the forward/right-facing angles, so left-facing movement uses the
// mirrored frame with flip=true and the sprite is drawn horizontally mirrored.
// Compass sector used by the impostor tables:
// 0=E, 1=SE, 2=S, 3=SW, 4=W, 5=NW, 6=N, 7=NE  (screen coordinates, +y down).
static int compassSector(float dx, float dy) {
    if (std::fabs(dx) < 1e-5f && std::fabs(dy) < 1e-5f) return 4;
    float ang = std::atan2(dy, dx) * 180.0f / PI;
    int s = (int)std::lround(ang / 45.0f);
    return (s % 8 + 8) % 8;
}
static CombatUnitVfx gUnitVfx[24];
static std::vector<int> gVfxProjectile[2], gVfxMuzzle[2], gVfxHit, gVfxMiss;
static int gVfxMissQuad = -1;   // packed 64x64 miss sheet (44 in the original)
static float gVfxDeathStep = 0.09f;
static Texture2D gBackdrop = {0};
static bool gBackdropDirty = true;
static Font gUiFont = {0};
static bool gUiFontReady = false;

static const char* kGameDir = nullptr;
static std::string gAssetDir;
// Resolved unit folder names, e.g. "unit_02_ax87" (id prefix + readable name).
static std::string gUnitDirName[24];
static void resolveUnitFolders() {
    std::error_code ec;
    for (auto& e : std::filesystem::directory_iterator(gAssetDir + "units", ec)) {
        if (!e.is_directory()) continue;
        std::string n = e.path().filename().string();
        if (n.rfind("unit_", 0) != 0 || n.size() < 7) continue;
        int id = std::atoi(n.c_str() + 5);
        if (id >= 0 && id < 24) gUnitDirName[id] = n;
    }
}
static std::string unitPath(int t, const char* file) {
    if (t >= 0 && t < 24 && !gUnitDirName[t].empty())
        return gAssetDir + "units/" + gUnitDirName[t] + "/" + file;
    return gAssetDir + TextFormat("units/unit_%02d/%s", t, file);
}
static float gPanX = 0.0f, gPanY = 0.0f;
static bool gMapNeedsCenter = true;   // set by loadMission, consumed by the map UI
static float gMapPixelW = 0.0f, gMapPixelH = 0.0f;
static float gHexSize = 30.0f;
static float gStepX = 45.0f, gStepY = 51.96f;
// Map unit sprite size (0.85x the old 1.75-hex size).
static inline float mapUnitSize() { return gHexSize * 1.75f * 0.85f; }
static float gHexH = 51.96f;
static float gOriginX = 20.0f, gOriginY = 20.0f;

// Recompute the map bounds for the current mission.  The zoom (hex size and
// step) is fixed for the session, so units stay the same size on every map;
// only the pan limits follow the new width/height.
static void updateMapGeometry() {
    gMapPixelW = gOriginX * 2 + gStepX * (gMission.w - 1) + 2 * gHexSize;
    gMapPixelH = gOriginY * 2 + gStepY * (gMission.h - 1) + gHexH + gStepY * 0.5f;
}

// Non-macro aliases (raylib's RED/GREEN are macros, so they cannot be used as
// local constant names in some expressions).
static const Color kRed = RED;
static const Color kGreen = GREEN;

// ------------------------------------------------------------------ hex
// Original movement table (0x42b248): cost = table[terrain][movement_class].
//  - movement class 4+ (air) ignores terrain and always costs 1
//  - cost bit 0x80 = impassable
//  - cost 0x7f = consumes the unit's whole movement allowance (and may only be
//    entered with full movement remaining, matching the original flood fill)
static constexpr int kMoveAll = 0x7f;
static constexpr int kMoveImpassable = 0x80;

static inline int moveCost(const Mission& m, int x, int y, int moveClass) {
    if (moveClass >= 4) return 1;                 // aircraft: ignore terrain
    return gTerrain[terrainAt(m, x, y)].cost[std::clamp(moveClass, 0, 3)];
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

    // ----- combat VFX defaults (used if combat_vfx.json is missing) -----
    gVfxProjectile[0] = {4};
    gVfxProjectile[1] = {18};
    gVfxMuzzle[0] = {14, 15, 16, 23};
    gVfxMuzzle[1] = {17, 19, 21, 22};
    gVfxHit = {25};
    gVfxMiss = {27};
    gVfxDeathStep = 0.09f;
    gVfxClasses.clear();
    gVfxClasses["air"] = {{32, 36}, 0.90f};
    gVfxClasses["infantry"] = {{32, 36, 44}, 0.85f};
    gVfxClasses["bike"] = {{32, 36, 44}, 0.90f};
    gVfxClasses["vehicle"] = {{32, 36, 44, 48, 52, 56}, 1.00f};
    gVfxClasses["artillery"] = {{32, 36, 44, 48, 52, 56}, 1.00f};
    gVfxClasses["heavy"] = {{32, 36, 44, 48, 52, 56}, 1.15f};
    for (int i = 0; i < 24; ++i) gUnitVfx[i] = CombatUnitVfx{};

    std::ifstream vf(gAssetDir + "data/combat_vfx.json");
    if (vf) {
        json vj = json::parse(vf);
        auto ints = [](const json& a) {
            std::vector<int> out;
            if (a.is_array()) for (auto& x : a) out.push_back(x.get<int>());
            return out;
        };
        if (vj.contains("frames")) {
            const json& f = vj["frames"];
            if (f.contains("projectile_left")) gVfxProjectile[0] = ints(f["projectile_left"]);
            if (f.contains("projectile_right")) gVfxProjectile[1] = ints(f["projectile_right"]);
            if (f.contains("muzzle_left")) gVfxMuzzle[0] = ints(f["muzzle_left"]);
            if (f.contains("muzzle_right")) gVfxMuzzle[1] = ints(f["muzzle_right"]);
            if (f.contains("hit")) gVfxHit = ints(f["hit"]);
            if (f.contains("miss")) gVfxMiss = ints(f["miss"]);
            if (f.contains("miss_quad")) gVfxMissQuad = f.value("miss_quad", -1);
            gVfxDeathStep = f.value("death_step_seconds", gVfxDeathStep);
        }
        if (vj.contains("classes")) {
            for (auto it = vj["classes"].begin(); it != vj["classes"].end(); ++it) {
                CombatVfxClass c;
                c.death = ints(it.value().value("death", json::array()));
                c.scale = it.value().value("scale", 1.0f);
                gVfxClasses[it.key()] = c;
            }
        }
        if (vj.contains("units")) {
            for (auto it = vj["units"].begin(); it != vj["units"].end(); ++it) {
                int t = std::atoi(it.key().c_str());
                if (t < 0 || t >= 24) continue;
                CombatUnitVfx& u = gUnitVfx[t];
                u.cls = it.value().value("class", std::string());
                if (it.value().contains("death")) u.death = ints(it.value()["death"]);
                if (it.value().contains("scale")) u.scale = it.value().value("scale", -1.0f);
                if (it.value().contains("projectile_left"))
                    u.projectile[0] = ints(it.value()["projectile_left"]);
                if (it.value().contains("projectile_right"))
                    u.projectile[1] = ints(it.value()["projectile_right"]);
                if (it.value().contains("muzzle_left"))
                    u.muzzle[0] = ints(it.value()["muzzle_left"]);
                if (it.value().contains("muzzle_right"))
                    u.muzzle[1] = ints(it.value()["muzzle_right"]);
            }
        }
    }

    // Entry paths decoded from the original's live sprite queue.
    gEntryPaths.clear();
    gEntryPaths["ground"] = {{35.7f, 6.6f}, {17.7f, 5.6f}};
    gEntryPaths["air"] = {{31.0f, 12.7f}, {10.2f, 6.1f}};
    gEntryPaths["bike"] = {{35.7f, 10.0f}, {17.7f, 8.0f}};
    std::ifstream ep(gAssetDir + "data/combat_entry_paths.json");
    if (ep) {
        json ej = json::parse(ep);
        auto readPath = [&](const json& v) {
            EntryPath p;
            if (v.contains("start") && v["start"].size() >= 2) {
                p.start[0] = v["start"][0].get<float>();
                p.start[1] = v["start"][1].get<float>();
            }
            if (v.contains("mid") && v["mid"].size() >= 2) {
                p.mid[0] = v["mid"][0].get<float>();
                p.mid[1] = v["mid"][1].get<float>();
            }
            return p;
        };
        if (ej.contains("classes")) {
            for (auto it = ej["classes"].begin(); it != ej["classes"].end(); ++it)
                gEntryPaths[it.key()] = readPath(it.value());
        }
        if (ej.contains("types")) {
            for (auto it = ej["types"].begin(); it != ej["types"].end(); ++it)
                gEntryPaths["t" + it.key()] = readPath(it.value());
        }
        if (ej.contains("terrain_types")) {
            for (auto it = ej["terrain_types"].begin(); it != ej["terrain_types"].end(); ++it)
                gEntryPaths["t" + it.key()] = readPath(it.value());
        }
    }

    // Real per-slot entry curves captured from the live object table.
    gEntryCurves.clear();
    std::ifstream ec(gAssetDir + "data/combat_entry_curves.json");
    if (ec) {
        json ej = json::parse(ec);
        for (const char* side : {"left", "right"}) {
            if (!ej.contains(side)) continue;
            for (const auto& v : ej[side]) {
                EntryCurve c;
                if (v.contains("slot") && v["slot"].size() >= 2) {
                    c.slot = {v["slot"][0].get<float>(), v["slot"][1].get<float>()};
                }
                if (v.contains("start") && v["start"].size() >= 2) {
                    c.start = {v["start"][0].get<float>(), v["start"][1].get<float>()};
                }
                c.t0 = v.value("t0", 0.0f);
                c.duration = v.value("duration", 0.0f);
                if (v.contains("samples")) {
                    for (const auto& s : v["samples"]) {
                        if (s.size() >= 3)
                            c.pts.push_back({s[0].get<float>(), s[1].get<float>(), s[2].get<float>()});
                    }
                }
                if (!c.pts.empty()) gEntryCurves[side].push_back(c);
            }
        }
    }

    // Editable perspective/sprite settings.
    gPersp.clear();
    std::ifstream pf(gAssetDir + "data/combat_perspective.json");
    if (pf) {
        json pj = json::parse(pf);
        gEntrySpeed = pj.value("speed", 1.0f);
        if (gEntrySpeed <= 0.0f) gEntrySpeed = 1.0f;
        for (const char* side : {"left", "right"}) {
            if (!pj.contains(side)) continue;
            for (const auto& r : pj[side]) {
                PerspSlot p;
                if (r.contains("slot") && r["slot"].size() >= 2)
                    p.slot = {r["slot"][0].get<float>(), r["slot"][1].get<float>()};
                p.sprite = r.value("sprite", -1);
                p.spriteEnd = r.value("sprite_end", -1);
                p.flip = r.value("flip", -1);
                p.scale = r.value("scale", 1.0f);
                gPersp[side].push_back(p);
            }
        }
    }
}

static std::vector<std::string> listCampaign(const std::string& dir) {
    std::vector<std::string> camps, maps;
    std::string test;
    std::error_code ec;
    for (const auto& e : std::filesystem::directory_iterator(dir, ec)) {
        std::string n = e.path().filename().string();
        if (n == "all_units_test.json") { test = e.path().string(); continue; }
        if (n.rfind("deploy_", 0) == 0) continue;
        bool campaign = n.rfind("campaign_", 0) == 0;
        bool editor = n.size() == 9 && (n[0] == 'M') && n[1] >= 'a' && n[1] <= 'd' &&
                      std::isdigit((unsigned char)n[2]) && std::isdigit((unsigned char)n[3]);
        if ((campaign || editor) && n.size() > 5 && n.substr(n.size() - 5) == ".json")
            (campaign ? camps : maps).push_back(e.path().string());
    }
    std::sort(camps.begin(), camps.end());
    std::sort(maps.begin(), maps.end());
    std::vector<std::string> out;
    if (!test.empty()) out.push_back(test);   // test map, then campaign 1..16
    out.insert(out.end(), camps.begin(), camps.end());
    out.insert(out.end(), maps.begin(), maps.end());
    return out;
}

static void loadFactoryData() {
    if (gFactoryDataLoaded) return;
    gFactoryDataLoaded = true;
    std::string path = gAssetDir + "data/factories.json";
    if (!std::filesystem::exists(path)) return;
    json j = loadJson(path);
    if (j.is_object() && j.contains("missions")) gFactoryData = std::move(j);
}

static const Factory* factoryAt(const Mission& m, int x, int y) {
    for (const Factory& f : m.factories)
        if (f.x == x && f.y == y) return &f;
    return nullptr;
}

static int factoryIndexAt(const Mission& m, int x, int y) {
    for (int i = 0; i < (int)m.factories.size(); ++i)
        if (m.factories[i].x == x && m.factories[i].y == y) return i;
    return -1;
}

static bool loadMission(const std::string& path) {    json j = loadJson(path);
    gPanX = gPanY = 0.0f;
    gMapNeedsCenter = true;
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
        u.facing = ju.value("facing", 90.0f);
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
    // Factory definitions (assets/data/factories.json) for the campaign.
    gMission.factories.clear();
    loadFactoryData();
    if (gFactoryData.is_object() && gFactoryData.contains("missions")) {
        for (const auto& jm : gFactoryData["missions"]) {
            if (jm.value("mission", -1) != gMission.number &&
                jm.value("name", std::string("")) != gMission.name)
                continue;
            if (!jm.contains("factories")) continue;
            for (const auto& jf : jm["factories"]) {
                Factory f;
                f.group = jf.value("group", (int)gMission.factories.size() + 1);
                f.x = jf.value("x", 0);
                f.y = jf.value("y", 0);
                f.terrain = jf.value("terrain", 0);
                f.capacity = jf.value("capacity", 12);
                std::string owner = jf.value("owner", std::string("neutral"));
                f.owner = owner == "player" ? 0 : (owner == "enemy" ? 1 : -1);
                if (jf.contains("roster")) {
                    for (const auto& ju : jf["roster"])
                        f.roster.push_back(ju.value("type", 0));
                }
                gMission.factories.push_back(std::move(f));
            }
            break;
        }
    }
    // Optional full-map backdrop. Loading is deferred until the GL context
    // exists (see ensureBackdrop); a missing file is not fatal.
    gBackdropDirty = true;
    updateMapGeometry();
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
// Enemy zone of control: the six hexes around every living enemy unit.
// (The original applies ZOC to all unit types, including aircraft.)
static std::vector<unsigned char> enemyZoc(const Mission& m, int moverOwner) {
    std::vector<unsigned char> zoc(m.w * m.h, 0);
    for (const Unit& e : m.units) {
        if (e.hp <= 0 || e.carried || e.owner == moverOwner) continue;
        for (int d = 0; d < 6; ++d) {
            int nx, ny;
            hexNeighbor(e.x, e.y, d, nx, ny);
            if (inBounds(m, nx, ny)) zoc[ny * m.w + nx] = 1;
        }
    }
    return zoc;
}

static std::vector<int> reachable(const Mission& m, int slot) {
    std::vector<int> dist(m.w * m.h, 1 << 29);
    const Unit& u = m.units[slot];
    const UnitType& t = gTypes[u.type];
    using P = std::pair<int, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    dist[u.y * m.w + u.x] = 0;

    const std::vector<unsigned char> zoc = enemyZoc(m, u.owner);
    const bool startInZoc = zoc[u.y * m.w + u.x] != 0;

    // Starting inside an enemy ZOC: only one hex of movement this turn.
    if (startInZoc) {
        for (int d = 0; d < 6; ++d) {
            int nx, ny;
            hexNeighbor(u.x, u.y, d, nx, ny);
            if (!inBounds(m, nx, ny)) continue;
            int mc = moveCost(m, nx, ny, t.moveClass);
            if (mc & kMoveImpassable) continue;
            if (unitAt(m, nx, ny) >= 0) continue;
            int eff = (mc == kMoveAll) ? t.movePts : mc;
            if (eff <= t.movePts) dist[ny * m.w + nx] = eff;
        }
        return dist;
    }

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
            if (mc & kMoveImpassable) continue;
            int ni = unitAt(m, nx, ny);
            if (ni >= 0 && ni != slot) continue;   // blocked
            int nc;
            if (mc == kMoveAll) {
                // Original: entering "all movement" terrain is only possible
                // when the unit has its full allowance left; it then ends.
                if (c != 0 || t.movePts <= 0) continue;
                nc = t.movePts;
            } else {
                nc = c + mc;
            }
            if (nc <= t.movePts && nc < dist[ny * m.w + nx]) {
                dist[ny * m.w + nx] = nc;
                // Entering an enemy ZOC ends movement: do not expand from it.
                if (!zoc[ny * m.w + nx]) pq.push({nc, ny * m.w + nx});
            }
        }
    }
    return dist;
}

// ------------------------------------------------------------------ transport
static bool isTransport(const UnitType& t) { return (t.ability & 0x20) != 0; }

// NC-1 Mule (ability 0x20, ground mover): mines, infantry, motorcycles and
// field guns.  C-61 Pelican (air mover): everything except other aircraft.
// The cargo id list is the original unit table's categories.
static bool canCarry(const UnitType& carrier, const UnitType& cargo) {
    if (!isTransport(carrier)) return false;
    if (carrier.moveClass == 4 || carrier.id == 22) {
        return cargo.moveClass != 4;                 // Pelican: no aircraft
    }
    switch (cargo.id) {
        case 0:   // M-77 Trigger (mines)
        case 11:  // SG-4 Hadrian (field gun)
        case 12:  // MR-22 Octopus (field gun)
        case 17:  // SS-80 Atlas (field gun)
        case 18:  // GX-77 Charlie (infantry)
        case 19:  // GX-87 Kilroy (infantry)
        case 20:  // CBX-1 Panther (motorcycle)
            return cargo.id != 9;                    // never the Giant
        default:
            return false;
    }
}

// A unit can shift into a transport when the transport's cell is inside its
// movement range (the original "Shift" command).  The transport's own cell is
// occupied, so test the cost from each adjacent reachable cell instead.
static bool inShiftRange(const Mission& m, int cargoSlot, int transSlot) {
    const Unit& c = m.units[cargoSlot];
    const Unit& t = m.units[transSlot];
    const UnitType& ct = gTypes[c.type];
    int enter = moveCost(m, t.x, t.y, ct.moveClass);
    if (enter & kMoveImpassable) return false;
    if (enter == kMoveAll) enter = ct.movePts;
    std::vector<int> dist = reachable(m, cargoSlot);
    for (int d = 0; d < 6; ++d) {
        int nx, ny;
        hexNeighbor(t.x, t.y, d, nx, ny);
        if (!inBounds(m, nx, ny)) continue;
        if (dist[ny * m.w + nx] + enter <= ct.movePts) return true;
    }
    return false;
}

// Cargo may only be dropped on the original deployment terrain: pavement,
// flat ground, factory/base or city.  Water, forest, mountains and ruins are
// not valid drop zones.
static bool canUnloadTerrain(int t) {
    return t == 1 || t == 5 || t == 6 || t == 7 || t == 9 || t == 10 || t == 11;
}

// Keep cargo glued to its transport and destroy it only when the whole
// transport formation is wiped out.
static void syncCargo(Mission& m) {
    for (int i = 0; i < (int)m.units.size(); ++i) {
        Unit& u = m.units[i];
        if (u.cargo < 0) continue;
        if (u.cargo >= (int)m.units.size()) { u.cargo = -1; continue; }
        Unit& c = m.units[u.cargo];
        if (u.hp <= 0) {                 // transport destroyed -> cargo lost
            c.hp = 0;
            c.carried = false;
            u.cargo = -1;
            continue;
        }
        if (c.hp <= 0) { u.cargo = -1; c.carried = false; continue; }
        c.carried = true;
        c.x = u.x;
        c.y = u.y;
    }
}

// ------------------------------------------------------------------ AI
static void queueCombat(int atk, int def, int hpA0, int hpD0, int hpA1, int hpD1,
                        const CombatResult& r, int mode);

static int nearestPlayerTarget(const Mission& m, int slot) {
    const Unit& u = m.units[slot];
    int best = -1, bestD = 1 << 20;
    for (int j = 0; j < (int)m.units.size(); ++j) {
        const Unit& p = m.units[j];
        if (p.hp <= 0 || p.owner != 0 || p.carried) continue;
        int d = std::abs(p.x - u.x) + std::abs(p.y - u.y);
        if (d < bestD) { bestD = d; best = j; }
    }
    return best;
}

// Best destination cell from the precomputed reachable map, using the same
// scoring as the original simple AI: close in / hold firing range.
static int aiDestination(const Mission& m, int slot, int targetSlot,
                         const std::vector<int>& dist) {
    const Unit& u = m.units[slot];
    const Unit& target = m.units[targetSlot];
    bool targetAir = gTypes[target.type].moveClass == 4;
    int range = targetAir ? gTypes[u.type].ctrAir : gTypes[u.type].ctrGround;
    int bestCell = -1, bestScore = 1 << 20;
    for (int y = 0; y < m.h; ++y)
        for (int x = 0; x < m.w; ++x) {
            int idx = y * m.w + x;
            if (dist[idx] >= (1 << 29)) continue;
            if (unitAt(m, x, y) >= 0 && !(x == u.x && y == u.y)) continue;
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
    return bestCell;
}

// Reconstruct the travelled cells from the Dijkstra map (target -> source).
static std::vector<std::pair<int, int>> movementPath(const Mission& m, int slot,
                                                     const std::vector<int>& dist,
                                                     int tx, int ty) {
    std::vector<std::pair<int, int>> rev;
    const Unit& u = m.units[slot];
    const UnitType& t = gTypes[u.type];
    int cx = tx, cy = ty, guard = 0;
    while ((cx != u.x || cy != u.y) && guard++ < 512) {
        rev.push_back({cx, cy});
        int cur = dist[cy * m.w + cx];
        int cost = moveCost(m, cx, cy, t.moveClass);
        if (cost & kMoveImpassable) break;
        if (cost == kMoveAll) cost = t.movePts;
        int nx2 = -1, ny2 = -1;
        for (int d = 0; d < 6; ++d) {
            int nx, ny;
            hexNeighbor(cx, cy, d, nx, ny);
            if (!inBounds(m, nx, ny)) continue;
            if (dist[ny * m.w + nx] + cost == cur) { nx2 = nx; ny2 = ny; break; }
        }
        if (nx2 < 0) break;
        cx = nx2;
        cy = ny2;
    }
    std::reverse(rev.begin(), rev.end());
    return rev;
}

// ------------------------------------------------------------------ hex map
static int CELL = 64;

static void hexCenter(int x, int y, float& cx, float& cy) {
    cx = gOriginX + gPanX + gHexSize + x * gStepX;
    cy = gOriginY + gPanY + gHexH * 0.5f + y * gStepY + ((x & 1) ? gStepY * 0.5f : 0.0f);
}

// Flat-top, odd-q offset. Returns false if outside the map.
static bool pixelToHex(float px, float py, int w, int h, int& outX, int& outY) {
    float rx = px - (gOriginX + gPanX + gHexSize);
    float ry = py - (gOriginY + gPanY + gHexH * 0.5f);
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
        // Align the original rectangular map cell grid to the hex cell centres.
        // The hex grid is odd-q offset, so the backdrop sits half a step right
        // and half a step up from the map origin.
        float cw = ((float)m.w + 0.5f) * gStepX;
        float ch = ((float)m.h + 0.5f) * gStepY;
        float x0 = gOriginX + gPanX + gHexSize - gStepX * 0.5f;
        float y0 = gOriginY + gPanY + gHexH * 0.5f - gStepY * 0.5f;
        DrawTexturePro(gBackdrop, {0, 0, (float)gBackdrop.width, (float)gBackdrop.height},
                       {x0, y0, cw, ch}, {0, 0}, 0, WHITE);
    }
    for (int y = 0; y < m.h; ++y)
        for (int x = 0; x < m.w; ++x) {
            float cx, cy;
            hexCenter(x, y, cx, cy);
            if (!haveBg) {
                Color c = kTerrainCols[terrainAt(m, x, y)];
                DrawPoly({cx, cy}, 6, gHexSize - 1.0f, 0, c);
            }
            // Over the original backdrop the art carries the terrain colours;
            // only a very faint cell grid remains readable.
            DrawPolyLines({cx, cy}, 6, gHexSize - 1.0f, 0,
                          Fade(BLACK, haveBg ? 0.14f : 0.35f));
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

// Small filled 5-point star (experience rank marker).
static void drawStar(float cx, float cy, float r, Color c) {
    const float kPi = 3.14159265f;
    Vector2 pts[10];
    for (int i = 0; i < 10; ++i) {
        float a = -kPi / 2.0f + i * kPi / 5.0f;
        float rr = (i & 1) ? r * 0.45f : r;
        pts[i] = {cx + std::cos(a) * rr, cy + std::sin(a) * rr};
    }
    for (int i = 0; i < 10; ++i)
        DrawTriangle({cx, cy}, pts[(i + 1) % 10], pts[i], c);
}

static void drawUnitSprite(const Mission& m, const Unit& u, Texture2D playerTex, Texture2D enemyTex) {
    // The bank icons are left-facing side views.  The original map only turns
    // the icon in 45-degree sectors: straight left/right stay horizontal, the
    // four diagonals tilt by 30 degrees, and straight up/down by 90 degrees
    // (where the mirrored copy looks identical, so we always use the base).
    float a = u.facing;
    while (a <= -180.0f) a += 360.0f;
    while (a > 180.0f) a -= 360.0f;
    int sector = (int)std::lround((a + 135.0f) / 45.0f) & 7;
    static const bool mirror[8] = {true, true, true, false, false, false, false, false};
    static const float turn[8] = {-30.0f, 0.0f, 30.0f, -90.0f, -30.0f, 0.0f, 30.0f, 90.0f};
    Texture2D& tex = mirror[sector] ? gUnitMapFlipTex[u.type] : gUnitMapTex[u.type];
    if (!tex.id) return;
    float size = mapUnitSize();
    float hx, hy;
    hexCenter(u.x, u.y, hx, hy);
    // raylib subtracts `origin` from the destination, so pass the centre point
    float cx = (u.px >= 0) ? u.px + size / 2 : hx;
    float cy = (u.py >= 0) ? u.py + size / 2 : hy;
    Rectangle src{0, 0, (float)tex.width, (float)tex.height};
    Rectangle dst{cx, cy, size, size};
    Color tint = (u.owner == 0) ? WHITE : Color{150, 255, 150, 255};
    DrawTexturePro(tex, src, dst, {size / 2, size / 2}, turn[sector], tint);
}

// ------------------------------------------------------------------ combat screen
// One combatant as shown on screen.  Sides are explicit: the view always puts
// team 0 (player) on the left and team 1 (computer) on the right, no matter who
// initiated the attack.  Everything the renderer needs is precomputed here.
struct CombatSide {
    int unit = -1;          // mission unit slot
    int team = 0;           // 0 player, 1 computer
    int terrain = 0;
    int panel = 0;          // background panel index
    int hp0 = 0, hp1 = 0;   // formations before / after
    int atkPct = 0, defPct = 0;
    int atkDisplay = 0, defDisplay = 0;   // cumulative own + support totals
    int support = 0, expPct = 0, xpGain = 0, damage = 0;
    int surroundPct = 0;
};

struct CombatView {
    bool active = false;
    bool ranged = false;
    CombatSide left;        // always the player
    CombatSide right;       // always the computer
    CombatResult r;         // shared simulation info (surround, terrain ...)
    float t = 0;            // animation clock (ride-in / fire / explosions)
};
static std::deque<CombatView> gCombatQueue;
static CombatView gCv;

static int terrainRemap(int t) {
    if (t >= 0 && t < (int)gRemap.size()) return gRemap[t];
    return (t < 0 || t > 15) ? 0 : t;
}

// Mirrors FUN_00415790: the two panels are drawn with param4 = 8 (first) and 0
// (second), each using its own terrain first and the opponent's second.
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

static const std::vector<int>& emptyVfx() {
    static const std::vector<int> e;
    return e;
}

static const CombatUnitVfx& unitVfx(int type) {
    static const CombatUnitVfx empty;
    return (type >= 0 && type < 24) ? gUnitVfx[type] : empty;
}

static const CombatVfxClass* vfxClassFor(int type) {
    const CombatUnitVfx& u = unitVfx(type);
    auto it = gVfxClasses.find(u.cls);
    if (it != gVfxClasses.end()) return &it->second;
    return nullptr;
}

static const std::vector<int>& vfxProjectileFor(int type, int side) {
    const CombatUnitVfx& u = unitVfx(type);
    if (!u.projectile[side & 1].empty()) return u.projectile[side & 1];
    return gVfxProjectile[side & 1];
}

static const std::vector<int>& vfxMuzzleFor(int type, int side) {
    const CombatUnitVfx& u = unitVfx(type);
    if (!u.muzzle[side & 1].empty()) return u.muzzle[side & 1];
    return gVfxMuzzle[side & 1];
}

static const std::vector<int>& vfxDeathFor(int type) {
    const CombatUnitVfx& u = unitVfx(type);
    if (!u.death.empty()) return u.death;
    if (const CombatVfxClass* c = vfxClassFor(type)) return c->death;
    return emptyVfx();
}

static float vfxScaleFor(int type) {
    const CombatUnitVfx& u = unitVfx(type);
    if (u.scale >= 0.0f) return u.scale;
    if (const CombatVfxClass* c = vfxClassFor(type)) return c->scale;
    return 1.0f;
}

static void drawVfxHalf(int frame, Vector2 center, float size, Color tint) {
    if (frame < 0 || frame >= 64) return;
    Texture2D& tex = gVfxHalf[frame];
    if (!tex.id) return;
    DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height},
                   {center.x - size / 2, center.y - size / 2, size, size},
                   {0, 0}, 0, tint);
}

static void drawVfxQuad(int startFrame, Vector2 center, float size, Color tint) {
    int idx = startFrame / 4;
    if (idx < 0 || idx >= 16) return;
    Texture2D& tex = gVfxQuad[idx];
    if (!tex.id) return;
    DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height},
                   {center.x - size / 2, center.y - size / 2, size, size},
                   {0, 0}, 0, tint);
}

static CombatSide makeCombatSide(int slot, int hp0, int hp1,
                                 const CombatResult& r, bool isAttacker) {
    CombatSide s;
    s.unit = slot;
    s.team = gMission.units[slot].owner;
    s.terrain = terrainAt(gMission, gMission.units[slot].x, gMission.units[slot].y);
    s.hp0 = hp0;
    s.hp1 = hp1;
    s.atkPct = isAttacker ? r.atkStrength : r.ctrStrength;
    s.defPct = isAttacker ? r.ctrDefense : r.defStrength;
    s.atkDisplay = isAttacker ? r.dispAtkA : r.dispAtkD;
    s.defDisplay = isAttacker ? r.dispDefA : r.dispDefD;
    s.support = 0;
    {
        const auto& list = isAttacker ? r.supportAtkUnits : r.supportDefUnits;
        for (const auto& c : list) s.support += c.value;
    }
    s.expPct = isAttacker ? r.expPctA : r.expPctD;
    s.xpGain = isAttacker ? r.xpToAtk : r.xpToDef;
    s.damage = isAttacker ? r.dmgToDef : r.dmgToAtk;
    s.surroundPct = isAttacker ? 0 : r.surroundPctD;
    return s;
}

// The renderer only receives explicit screen sides.  Each side carries its
// unit, team and terrain; the background panel is derived from that side's
// terrain (own terrain first, opponent second).
static CombatView buildCombatView(const CombatSide& left, const CombatSide& right,
                                  const CombatResult& r, bool ranged) {
    CombatView v;
    v.r = r;
    v.ranged = ranged;
    v.left = left;
    v.right = right;
    v.left.panel = combatPanelFor(v.left.terrain, v.right.terrain, ranged, true);
    v.right.panel = combatPanelFor(v.right.terrain, v.left.terrain, ranged, false);
    return v;
}

// Normal attack flow: turn an attacker/defender exchange into a view whose
// sides are fixed by team.  Player (team 0) is always left, computer (team 1)
// always right, even when the computer initiated the attack.
static CombatView buildCombatViewFromExchange(int atk, int def,
                                              int hpA0, int hpD0, int hpA1, int hpD1,
                                              const CombatResult& r, int mode) {
    CombatSide attacker = makeCombatSide(atk, hpA0, hpA1, r, true);
    CombatSide defender = makeCombatSide(def, hpD0, hpD1, r, false);
    const CombatSide& left = (attacker.team == 0) ? attacker : defender;
    const CombatSide& right = (attacker.team == 0) ? defender : attacker;
    return buildCombatView(left, right, r, mode > 0);
}

static void queueCombat(int atk, int def, int hpA0, int hpD0, int hpA1, int hpD1,
                        const CombatResult& r, int mode) {
    gCombatQueue.push_back(buildCombatViewFromExchange(atk, def, hpA0, hpD0, hpA1, hpD1, r, mode));
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
    if (gVfxQuad[8].id) {   // first double-size frame (start 32)
        float s = r * 2.6f;
        DrawTexturePro(gVfxQuad[8], {0, 0, (float)gVfxQuad[8].width,
                                    (float)gVfxQuad[8].height},
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

// Fallback shell when a unit has no configured projectile frames: cell 01 is
// the right-facing missile from the single-cell set.
static void drawProjectile(float x, float y, float s, float angle) {
    Texture2D& tex = gVfxHalf[1];
    if (!tex.id) {
        DrawCircleV({x, y}, s * 0.18f, ORANGE);
        return;
    }
    DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height},
                   {x, y, s, s}, {s / 2, s / 2}, angle, WHITE);
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

struct FactoryLayout {
    Rectangle panel;
    Rectangle grid;
    Rectangle info;
    float cellW = 0, cellH = 0;
};

struct DeployCell {
    int x = 0, y = 0;
    int transportSlot = -1;   // >= 0: deploy directly into this transport
};

static FactoryLayout factoryLayout(int W, int H) {
    FactoryLayout L;
    float pw = std::min((float)W - 60.0f, 1040.0f);
    float ph = std::min((float)H - 40.0f, 680.0f);
    float px = (W - pw) * 0.5f, py = (H - ph) * 0.5f;
    L.panel = {px, py, pw, ph};
    float header = 78.0f, footer = 52.0f, infoH = 126.0f, pad = 28.0f;
    L.grid = {px + pad, py + header, pw - pad * 2.0f,
              ph - header - footer - infoH - 18.0f};
    L.info = {px + pad, L.grid.y + L.grid.height + 10.0f,
              pw - pad * 2.0f, infoH};
    L.cellW = L.grid.width / 4.0f;
    L.cellH = L.grid.height / 3.0f;
    return L;
}

static Rectangle factorySlotRect(const FactoryLayout& L, int i) {
    float x = L.grid.x + (i % 4) * L.cellW;
    float y = L.grid.y + (i / 4) * L.cellH;
    return {x + 6.0f, y + 6.0f, L.cellW - 12.0f, L.cellH - 12.0f};
}

static Color factoryOwnerColor(int owner) {
    if (owner == 0) return Color{145, 210, 255, 255};
    if (owner == 1) return Color{255, 205, 80, 255};
    return Color{190, 195, 205, 255};
}

static void drawFactoryScreen(const Factory& f, int sel, bool canDeploy) {
    int W = GetScreenWidth(), H = GetScreenHeight();
    FactoryLayout L = factoryLayout(W, H);
    Color ownerCol = factoryOwnerColor(f.owner);

    DrawRectangle(0, 0, W, H, Color{8, 9, 14, 255});
    DrawRectangleRounded(L.panel, 0.025f, 8, Color{20, 23, 32, 255});
    DrawRectangleLinesEx(L.panel, 2.0f, Color{70, 76, 92, 255});

    char title[128];
    snprintf(title, sizeof(title), "FACTORY %d  -  %s", f.group,
             f.owner == 0 ? "PLAYER" : (f.owner == 1 ? "ENEMY" : "NEUTRAL"));
    uiText(title, (int)L.panel.x + 28, (int)L.panel.y + 20, 24, ownerCol);
    char sub[128];
    snprintf(sub, sizeof(sub), "HEX %d,%d   STORED %d/%d   CLICK A UNIT",
             f.x, f.y, (int)f.roster.size(), f.capacity);
    uiText(sub, (int)L.panel.x + 28, (int)L.panel.y + 50, 14, LIGHTGRAY);

    for (int i = 0; i < 12; ++i) {
        Rectangle r = factorySlotRect(L, i);
        DrawRectangleRounded(r, 0.06f, 6, Color{42, 45, 58, 255});
        if (i == sel) {
            DrawRectangleRoundedLinesEx(r, 0.06f, 6, 3.0f, ownerCol);
        } else {
            DrawRectangleRoundedLinesEx(r, 0.06f, 6, 1.5f, Color{78, 82, 96, 255});
        }
        if (i < (int)f.roster.size()) {
            int t = f.roster[i];
            if (t < 0 || t >= 24) continue;
            Texture2D* tex = &gUnitMapTex[t];
            if (!tex->id) tex = &gUnitSprite[t];
            if (tex->id) {
                float icon = std::min(r.width, r.height) * 0.58f;
                float ix = r.x + (r.width - icon) * 0.5f;
                float iy = r.y + 8.0f;
                Rectangle src{0, 0, (float)tex->width, (float)tex->height};
                DrawTexturePro(*tex, src, {ix, iy, icon, icon}, {0, 0}, 0.0f, WHITE);
            }
            const char* nm = (t < (int)gTypes.size()) ? gTypes[t].name.c_str() : "?";
            uiText(nm, (int)r.x + 8, (int)(r.y + r.height - 26.0f), 16, RAYWHITE);
        } else {
            uiText("-", (int)(r.x + r.width * 0.5f - 4.0f),
                   (int)(r.y + r.height * 0.5f - 12.0f), 16, Color{90, 94, 106, 255});
        }
    }

    DrawRectangleRounded(L.info, 0.03f, 6, Color{14, 16, 23, 255});
    DrawRectangleLinesEx(L.info, 1.5f, Color{62, 68, 82, 255});
    if (sel >= 0 && sel < (int)f.roster.size()) {
        int t = f.roster[sel];
        if (t >= 0 && t < (int)gTypes.size()) {
            const UnitType& ut = gTypes[t];
            uiText(ut.name.c_str(), (int)L.info.x + 20, (int)L.info.y + 14, 22, ownerCol);
            char stats[192];
            snprintf(stats, sizeof(stats),
                     "HP 8/8    MOV %d    ATK G/A %d/%d    DEF %d",
                     ut.movePts, ut.atkGround, ut.atkAir, ut.def);
            uiText(stats, (int)L.info.x + 20, (int)L.info.y + 48, 16, LIGHTGRAY);
            const char* kind = ut.airborne() ? "AIR UNIT" : "GROUND UNIT";
            uiText(kind, (int)L.info.x + 20, (int)L.info.y + 78, 14,
                   Color{145, 155, 170, 255});
            if (ut.indirectCapable())
                uiText("INDIRECT FIRE", (int)L.info.x + 180, (int)L.info.y + 78, 14,
                       Color{145, 155, 170, 255});
            if (canDeploy)
                uiText("SPACE: deploy this unit", (int)L.info.x + 360,
                       (int)L.info.y + 78, 14, Color{120, 235, 150, 255});
            else
                uiText("VIEW ONLY", (int)L.info.x + 360, (int)L.info.y + 78, 14,
                       Color{190, 120, 120, 255});
        }
    } else {
        uiText("EMPTY SLOT", (int)L.info.x + 20, (int)L.info.y + 22, 20,
               Color{120, 124, 136, 255});
        uiText("No unit stored in this slot.", (int)L.info.x + 20,
               (int)L.info.y + 58, 14, GRAY);
    }

    uiText(canDeploy
               ? "ARROWS select   LMB select   SPACE deploy   ESC close"
               : "ARROWS select   LMB select   ESC / SPACE close",
           (int)L.panel.x + 28, (int)(L.panel.y + L.panel.height - 34.0f), 12, GRAY);
}

static void drawTargetingFrame(const Mission& m, int srcX, int srcY, int type,
                               const std::vector<DeployCell>& cells,
                               const char* title, bool unloading, Color srcCol,
                               const char* sourceLabel) {
    BeginDrawing();
    ClearBackground(Color{12, 12, 16, 255});
    drawHexMap(m);

    // Existing units, same map presentation as the tactical view.
    float usz = mapUnitSize();
    for (const auto& u : m.units) {
        if (u.hp <= 0 || u.carried) continue;
        drawUnitSprite(m, u, Texture2D{0}, Texture2D{0});
        float cx, cy;
        hexCenter(u.x, u.y, cx, cy);
        if (u.px >= 0) { cx = u.px + usz / 2; cy = u.py + usz / 2; }
        for (int i = 0; i < u.hp; ++i)
            DrawRectangle((int)(cx - u.hp * 4 + i * 8),
                          (int)(cy + gHexSize * 0.55f), 6, 5,
                          u.owner == 0 ? kGreen : kRed);
        int stars = std::min(u.rank, 7);
        if (stars > 0) {
            float sy = cy + gHexSize * 0.55f + 9.0f;
            float sx0 = cx - (stars - 1) * 5.0f;
            for (int i = 0; i < stars; ++i)
                drawStar(sx0 + i * 10.0f, sy, 3.6f, Color{255, 208, 60, 255});
        }
    }

    // Source cell and legal target cells.
    float fx, fy;
    hexCenter(srcX, srcY, fx, fy);
    DrawPolyLinesEx({fx, fy}, 6, gHexSize, 0, 3.5f, srcCol);
    for (const DeployCell& c : cells) {
        float cx, cy;
        hexCenter(c.x, c.y, cx, cy);
        Color col = c.transportSlot >= 0 ? Color{90, 210, 255, 255}
                                         : Color{100, 245, 145, 255};
        DrawPoly({cx, cy}, 6, gHexSize - 2, 0, Fade(col, 0.34f));
        DrawPolyLinesEx({cx, cy}, 6, gHexSize - 2, 0, 3.0f, col);
    }

    int px = (int)gMapPixelW + 16;
    const UnitType& ut = gTypes[type];
    Color titleCol = unloading ? Color{90, 210, 255, 255}
                               : Color{120, 235, 150, 255};
    uiText(title, px, 16, 24, titleCol);
    uiText(ut.name.c_str(), px, 58, 20, RAYWHITE);
    uiText(TextFormat("MOV %d   ATK G/A %d/%d   DEF %d",
                      ut.movePts, ut.atkGround, ut.atkAir, ut.def),
           px, 92, 14, LIGHTGRAY);
    uiText("Click a highlighted hex.", px, 145, 16, RAYWHITE);
    uiText("Green = valid ground", px, 185, 14, Color{120, 235, 150, 255});
    if (!unloading) {
        uiText("Blue = friendly transport", px, 210, 14, Color{90, 210, 255, 255});
        uiText("Transport must carry it.", px, 235, 12, GRAY);
        uiText("ESC / SPACE cancel", px, 300, 12, GRAY);
        uiText(sourceLabel, px, 360, 12, GRAY);
    } else {
        uiText("ESC / SPACE cancel", px, 240, 12, GRAY);
        uiText(sourceLabel, px, 290, 12, GRAY);
    }
    EndDrawing();
}

// Combat sprite texture for a unit type.  Frames come from the packed atlas
// (assets/art/unit_impostors_atlas.png); frame -1 uses the base sprite.
static Texture2D* combatUnitTexture(int type, int frame, bool rightSide) {
    if (type < 0 || type >= 24) return nullptr;
    if (frame >= 0 && gImpostorAtlas.id && !gImpostorRects[type].empty())
        return &gImpostorAtlas;
    Texture2D& base = rightSide ? gUnitSprite[type] : gUnitSpriteFlip[type];
    return base.id ? &base : nullptr;
}

static int combatFrameCount(int type, bool rightSide) {
    (void)rightSide;
    if (type < 0 || type >= 24) return 0;
    return (int)gImpostorRects[type].size();
}

static void drawCombatScreen(const CombatView& v, Texture2D playerTex, Texture2D enemyTex) {    (void)playerTex;
    (void)enemyTex;
    int W = GetScreenWidth(), H = GetScreenHeight();
    const Color NAVY = {0, 0, 80, 255};
    const Color PANEL = {0, 0, 64, 255};
    const Color BORDER = {224, 192, 24, 255};
    const Color TXT = {236, 236, 240, 255};
    const Color DIM = {168, 168, 178, 255};
    ClearBackground(BLACK);
    if (gDebugWire) rlDisableBackfaceCulling();

    // Original layout: a 512x480 playfield centred in the 640x480 screen.
    // Two square panels fill the width; the report bar takes what is left.
    float playW = std::min(W * 0.80f, H * 1.066667f);  // 512/480 of the screen height
    float playX = (W - playW) * 0.5f;
    float topH  = H * 0.20f;      // 96 / 480
    bool ranged = v.ranged;
    float gap = ranged ? playW * 0.05f : 0.0f;

    // Adjacent attacks must fill the whole combat view: make the panels square
    // and size them from the width, leaving a readable report bar below.
    float basePx = (gUiFontReady && gUiFont.baseSize > 0) ? (float)gUiFont.baseSize : 16.0f;
    int wantScale = 2;
    float reportNeed = 5.0f * basePx * wantScale * 1.12f + 16.0f;
    if (reportNeed > H * 0.34f) {
        wantScale = 1;
        reportNeed = 5.0f * basePx * 1.12f + 16.0f;
    }
    float panH = playW * 0.50f;
    float panY = topH;
    float minReport = std::max(H * 0.14f, reportNeed);
    if (panY + panH + minReport > H) panH = std::max(H * 0.24f, H - panY - minReport);
    float botY = panY + panH;
    float botH = H - botY;
    float panW = ranged ? (playW - gap) * 0.5f : playW * 0.5f;
    float axPanel = playX + (playW - (2.0f * panW + gap)) * 0.5f;
    float dxPanel = axPanel + panW + gap;

    const Unit& L = gMission.units[v.left.unit];
    const Unit& R = gMission.units[v.right.unit];
    const UnitType& TL = gTypes[L.type];
    const UnitType& TR = gTypes[R.type];
    const CombatResult& r = v.r;

    // ----- top: names + HP (initial, then the post-combat count)
    DrawRectangle((int)playX, 0, (int)playW, (int)topH, NAVY);
    // Header layout: group 1 = name + HP box on each side, group 2 = support
    // lists underneath, separated by a rule.  Every element is positioned from
    // the measured name width so nothing overlaps when names get long.
    const float nameSizeF = topH * 0.24f;
    int nameSize = (int)nameSizeF;
    const float nameY = topH * 0.13f;
    float nfs = uiFontPixels(nameSize);
    float lw = gUiFontReady ? MeasureTextEx(gUiFont, TL.name.c_str(), nfs, uiSpacing(nameSize)).x
                            : (float)MeasureText(TL.name.c_str(), nameSize * 2);
    float rw = gUiFontReady ? MeasureTextEx(gUiFont, TR.name.c_str(), nfs, uiSpacing(nameSize)).x
                            : (float)MeasureText(TR.name.c_str(), nameSize * 2);
    float nxL = playX + playW * 0.03f;
    float nxR = playX + playW * 0.97f - rw;
    // Keep the name and its HP box from overlapping even for long names: the
    // name is allowed at most the half-width minus the box and a margin.
    const float hpS0 = topH * 0.32f;
    const float hpGap0 = playW * 0.018f;
    const float maxNameW = playW * 0.5f - hpS0 - hpGap0 - playW * 0.04f;
    while (nameSize > 8 && (lw > maxNameW || rw > maxNameW)) {
        --nameSize;
        nfs = uiFontPixels(nameSize);
        lw = gUiFontReady ? MeasureTextEx(gUiFont, TL.name.c_str(), nfs, uiSpacing(nameSize)).x
                          : (float)MeasureText(TL.name.c_str(), nameSize * 2);
        rw = gUiFontReady ? MeasureTextEx(gUiFont, TR.name.c_str(), nfs, uiSpacing(nameSize)).x
                          : (float)MeasureText(TR.name.c_str(), nameSize * 2);
        nxR = playX + playW * 0.97f - rw;
    }
    uiText(TL.name.c_str(), (int)nxL, (int)nameY, nameSize, TXT);
    uiText(TR.name.c_str(), (int)nxR, (int)nameY, nameSize, TXT);

    const float hpS = topH * 0.32f;
    const float hpY = topH * 0.07f;
    auto hpBox = [&](float x, int val) {
        float s = hpS;
        float y = hpY;
        DrawRectangleRec({x, y, s, s}, PANEL);
        DrawRectangleLinesEx({x, y, s, s}, std::max(2.0f, topH * 0.030f), BORDER);
        const char* txt = TextFormat("%d", val);
        float fs = s * 0.70f;
        if (gUiFontReady) {
            int scale = (int)std::lround(fs / (double)gUiFont.baseSize);
            if (scale < 1) scale = 1;
            fs = (float)gUiFont.baseSize * scale;
            float sp = std::max(1.0f, (float)scale);
            Vector2 m = MeasureTextEx(gUiFont, txt, fs, sp);
            DrawTextEx(gUiFont, txt, {x + (s - m.x) * 0.5f, y + (s - m.y) * 0.5f},
                       fs, sp, BORDER);
        } else {
            DrawText(txt, (int)(x + s * 0.33f), (int)(y + s * 0.12f),
                     (int)fs, BORDER);
        }
    };
    const float hpGap = playW * 0.018f;
    float hpXL = std::min(nxL + lw + hpGap, playX + playW * 0.48f - hpS);
    float hpXR = std::max(nxR - hpS - hpGap, playX + playW * 0.52f);
    // Thin team colour tabs identify each half of the header.
    DrawRectangle((int)playX, 0, (int)(playW * 0.5f), std::max(3, (int)(topH * 0.02f)),
                  Color{70, 130, 230, 255});
    DrawRectangle((int)(playX + playW * 0.5f), 0, (int)(playW * 0.5f),
                  std::max(3, (int)(topH * 0.02f)), Color{70, 190, 100, 255});

    float t = v.t;
    // Shooting is 1.2x slower than the original 0.35 s window.
    float fireT = 2.55f, fireDur = 0.42f;
    float impactT = fireT + fireDur + 0.10f;
    int hpL0 = v.left.hp0;
    int hpR0 = v.right.hp0;
    int hpL1 = v.left.hp1;
    int hpR1 = v.right.hp1;
    int nL0 = std::clamp(hpL0 + 1, 0, 8);   // initial vehicles, left side
    int nR0 = std::clamp(hpR0 + 1, 0, 8);
    int nL1 = (hpL1 > 0) ? std::clamp(hpL1 + 1, 0, 8) : 0;  // survivors
    int nR1 = (hpR1 > 0) ? std::clamp(hpR1 + 1, 0, 8) : 0;
    bool impact = t > impactT;
    hpBox(hpXL, impact ? nL1 : nL0);
    hpBox(hpXR, impact ? nR1 : nR0);

    // Header group separator between the name/HP row and the support lists.
    float supportY = hpY + hpS + topH * 0.06f;
    DrawLineEx({playX + playW * 0.02f, supportY - topH * 0.03f},
               {playX + playW * 0.98f, supportY - topH * 0.03f}, 1.5f,
               Color{255, 255, 255, 45});

    // ----- support contributor lists under the HP boxes
    auto smallText = [&](float x, float y, const char* s, Color c) {
        if (gUiFontReady)
            DrawTextEx(gUiFont, s, {x, y}, (float)gUiFont.baseSize, 1.0f, c);
        else
            DrawText(s, (int)x, (int)y, 16, c);
    };
    auto supportRow = [&](const CombatSide& side, float x0, float maxX) {
        bool attacker = (side.unit == v.r.atkSlot);
        const auto& list = attacker ? v.r.supportAtkUnits : v.r.supportDefUnits;
        // Readable integer-scaled original font sized to the free band between
        // the header rule and the bottom of the name bar.
        float freeH = topH - supportY - 2.0f;
        int scale = 1;
        if (gUiFontReady && gUiFont.baseSize > 0) {
            scale = (int)(std::min(topH * 0.10f, freeH / 4.6f) / (float)gUiFont.baseSize);
            if (scale < 1) scale = 1;
            if (scale > 3) scale = 3;
        }
        float fs = gUiFontReady ? (float)gUiFont.baseSize * scale : 16.0f;
        float sp = gUiFontReady ? (float)scale : 1.0f;
        auto rowText = [&](float x, float y, const char* s, Color c) {
            if (gUiFontReady) DrawTextEx(gUiFont, s, {x, y}, fs, sp, c);
            else DrawText(s, (int)x, (int)y, 16, c);
        };
        auto rowWidth = [&](const char* s) {
            return gUiFontReady ? MeasureTextEx(gUiFont, s, fs, sp).x
                                : (float)MeasureText(s, 16);
        };
        int total = 0;
        for (const SupportContribution& c : list) total += c.value;
        float y = supportY;
        const float rowH = fs * 1.55f;
        const float sz = fs * 1.40f;
        const char* head = TextFormat("%s +%d", attacker ? "ASUP" : "DSUP", total);
        rowText(x0, y, head, total > 0 ? BORDER : DIM);
        float x = x0 + rowWidth(head) + 10.0f;
        int row = 0, count = 0;
        // Allied contributors only, drawn in the side's own colour.  Ranged
        // attacks have no support at all, so the row simply reads "none".
        for (const SupportContribution& c : list) {
            if (c.slot < 0 || c.slot >= (int)gMission.units.size()) continue;
            const Unit& N = gMission.units[c.slot];
            if (N.owner != side.team) continue;
            const char* txt = TextFormat("x%d +%d", c.count, c.value);
            float tw = rowWidth(txt);
            float itemW = sz + 3 + tw + 8;
            if (x + itemW > maxX) {
                if (row >= 1) break;
                row = 1;
                x = x0;
                y += rowH;
            }
            Texture2D& tex = gUnitMapTex[N.type];
            Color tint = (side.team == 0) ? WHITE : Color{150, 255, 150, 255};
            if (tex.id) {
                DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height},
                               {x, y, sz, sz}, {0, 0}, 0, tint);
            }
            x += sz + 3;
            rowText(x, y, txt, BORDER);
            x += tw + 8.0f;
            ++count;
        }
        if (!count) rowText(x, y, "none", DIM);
    };
    supportRow(v.left, playX + playW * 0.03f, playX + playW * 0.47f);
    supportRow(v.right, playX + playW * 0.53f, playX + playW * 0.97f);
    // Centre divider keeps the two support groups visually separate.
    DrawLineEx({playX + playW * 0.50f, hpY},
               {playX + playW * 0.50f, topH - 2.0f}, 1.5f,
               Color{255, 255, 255, 35});

    // ----- middle: terrain close-ups + ride-in + simultaneous fire
    static const float FORM[8][2] = {
        {0.40f, 0.37f}, {0.23f, 0.34f}, {0.40f, 0.53f}, {0.19f, 0.47f},
        {0.36f, 0.72f}, {0.16f, 0.67f}, {0.31f, 0.89f}, {0.08f, 0.83f}
    };
    // Settled slot position: editable perspective config, then measured entry
    // curves, then FORM.
    auto slotPos = [&](bool right, int i) -> Vector2 {
        const auto& ps = gPersp[right ? "right" : "left"];
        if (!ps.empty()) return ps[std::min<int>(i, (int)ps.size() - 1)].slot;
        const auto& cs = gEntryCurves[right ? "right" : "left"];
        if (!cs.empty()) return cs[std::min<int>(i, (int)cs.size() - 1)].slot;
        int k = std::clamp(i, 0, 7);
        return {(right ? (1.0f - FORM[k][0]) : FORM[k][0]), FORM[k][1]};
    };
    float us = std::min(panW, panH) * 0.125f;

    auto panelPoint = [&](float px, float slotX, float slotY) {
        return Vector2{px + slotX * panW, panY + slotY * panH};
    };
    // Visual centre of a settled slot: matches how drawArmy places the sprite
    // (the recorded slot is the 64x64 cell origin; frames are centred in it).
    auto slotVisualCenter = [&](bool right, int idx) -> Vector2 {
        Vector2 s = slotPos(right, idx);
        float mul = 1.0f;
        const auto& ps = gPersp[right ? "right" : "left"];
        if (!ps.empty()) mul = ps[std::min<int>(idx, (int)ps.size() - 1)].scale;
        float cfg = std::max(0.2f, (0.72f + 0.80f * std::clamp(s.y, 0.0f, 1.0f)) * mul);
        float pxs = (panW / 256.0f) * cfg;
        Vector2 dest = panelPoint(right ? dxPanel : axPanel, s.x, s.y);
        return {dest.x + 32.0f * pxs, dest.y + 32.0f * pxs};
    };

    auto drawPanel = [&](int idx, float x, float w) {
        if (gCombatPanels.empty() || !gCombatPanels[idx].id) return;
        Texture2D& tex = gCombatPanels[idx];
        DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height},
                       {x, panY, w, panH}, {0, 0}, 0, WHITE);
    };
    // clip the battle scene to the two panels so entries (especially aircraft
    // descending from above) never draw over the name bar or report
    BeginScissorMode((int)playX, (int)panY, (int)playW, (int)panH);
    // v.left / v.right already carry their fixed-side background panel.
    if (ranged) {
        drawPanel(v.left.panel, axPanel, panW);
        drawPanel(v.right.panel, dxPanel, panW);
    } else {
        // 1px overlap hides the floating-point seam between the two panels
        drawPanel(v.left.panel, axPanel, panW + 1.0f);
        drawPanel(v.right.panel, dxPanel, panW);
    }

    // Only a side that actually took damage flashes red.
    bool hurtL = (v.left.damage > 0);
    bool hurtR = (v.right.damage > 0);
    bool flashOn = (t > impactT && t < impactT + 0.50f);
    Color flashA = (hurtL && flashOn) ? Color{255, 150, 150, 255} : WHITE;
    Color flashD = (hurtR && flashOn) ? Color{255, 150, 150, 255} : WHITE;
    float rk = (t > fireT && t < fireT + fireDur)
                   ? std::sin((t - fireT) / fireDur * 3.14159f)
                   : 0.0f;
    float recoilA = -us * 0.20f * rk;
    float recoilD = us * 0.20f * rk;

    // Original rhythm: arrival at 0.05 + i*0.235 s; rear formation rows are
    // drawn smaller (perspective), and destroyed slots vanish one by one as
    // their explosions play instead of all at once.
    int dbgDrawn[2] = {0, 0};
    auto drawArmy = [&](float px, int count0, int count1, int type, bool mirror,
                        float nudge, Color flash) {
        if (type < 0 || type >= 24) return;
        Texture2D* baseTex = combatUnitTexture(type, -1, mirror);
        if (!baseTex || !baseTex->id) return;
        Color team = WHITE;   // team recolour is done by the shader
        Color c = {
            (unsigned char)(team.r * flash.r / 255),
            (unsigned char)(team.g * flash.g / 255),
            (unsigned char)(team.b * flash.b / 255),
            (unsigned char)flash.a
        };
        // Entry curves captured from the live sprite-object table
        // (assets/data/combat_entry_curves.json): each slot is parked at the
        // panel edge and slides in linearly at its own start time, all slots
        // arriving together just before the firing phase.
        const auto& curves = gEntryCurves[mirror ? "right" : "left"];
        const auto& persp = gPersp[mirror ? "right" : "left"];
        float speed = (gEntrySpeed > 0.0f) ? gEntrySpeed : 1.0f;
        int frameCount = combatFrameCount(type, mirror);
        if (gUnitRecolor.id) {
            // Team tint via luminance ramp: blue art for both sides, enemy is
            // recoloured green while keeping the original highlights/shadows.
            Vector4 tint = mirror ? Vector4{0.42f, 0.95f, 0.52f, 1.0f}
                                  : Vector4{1.0f, 1.0f, 1.0f, 1.0f};
            float amount = mirror ? 1.0f : 0.0f;
            SetShaderValue(gUnitRecolor, gUnitTintLoc, &tint, SHADER_UNIFORM_VEC4);
            SetShaderValue(gUnitRecolor, gUnitAmountLoc, &amount, SHADER_UNIFORM_FLOAT);
            // Feed this unit's exact palette pairs (game green colours).
            auto& lut = gUnitLut[type];
            int n = (int)std::min<size_t>(lut.size(), 16);
            if (n > 0) {
                float fromv[48], tov[48];
                for (int k = 0; k < n; ++k) {
                    fromv[k * 3 + 0] = lut[k].first.x;
                    fromv[k * 3 + 1] = lut[k].first.y;
                    fromv[k * 3 + 2] = lut[k].first.z;
                    tov[k * 3 + 0] = lut[k].second.x;
                    tov[k * 3 + 1] = lut[k].second.y;
                    tov[k * 3 + 2] = lut[k].second.z;
                }
                SetShaderValueV(gUnitRecolor, gUnitLutFromLoc, fromv, SHADER_UNIFORM_VEC3, n);
                SetShaderValueV(gUnitRecolor, gUnitLutToLoc, tov, SHADER_UNIFORM_VEC3, n);
            }
            SetShaderValue(gUnitRecolor, gUnitLutCountLoc, &n, SHADER_UNIFORM_INT);
            BeginShaderMode(gUnitRecolor);
        }
        auto curveAt = [&](const EntryCurve& cv, float local, Vector2& out) {
            float lt = local * speed;
            float dur = cv.duration * speed;
            if (lt <= 0.0f || cv.pts.empty()) { out = cv.start; return; }
            if (lt >= dur) { out = cv.slot; return; }
            size_t k = 0;
            while (k + 1 < cv.pts.size() && cv.pts[k + 1].x * speed < lt) ++k;
            const Vector3& a = cv.pts[k];
            const Vector3& b = cv.pts[std::min(k + 1, cv.pts.size() - 1)];
            float ta = a.x * speed, tb = b.x * speed;
            float span = tb - ta;
            float f = span > 1e-6f ? (lt - ta) / span : 1.0f;
            out = {a.y + (b.y - a.y) * f, a.z + (b.z - a.z) * f};
        };
        for (int i = 0; i < count0; ++i) {
            if (i >= count1) {
                float death = impactT + (i - count1) * 0.12f;
                if (t >= death) continue;      // slot already exploded
            }
            Vector2 slotUv, posUv;
            bool settled = true;
            bool autoFlip = false;
            int autoDir = std::min(3, std::max(0, frameCount - 1));
            int dbgSec = -1;
            if (!curves.empty()) {
                const EntryCurve& cv = curves[std::min<int>(i, (int)curves.size() - 1)];
                slotUv = cv.slot;
                float local = t - cv.t0;
                curveAt(cv, local, posUv);
                settled = (local * speed >= cv.duration * speed);
                // Use the whole recorded displacement for the facing so the
                // flip never changes once the unit stops; the per-unit table
                // maps compass sectors to atlas frames.
                int sec = compassSector(cv.slot.x - cv.start.x,
                                        cv.slot.y - cv.start.y);
                dbgSec = sec;
                autoDir = (frameCount > 0)
                              ? (gImpostorDirs[type][sec] % frameCount) : 0;
                autoFlip = gImpostorFlips[type][sec] != 0;
            } else {
                float sx = mirror ? (1.0f - FORM[i][0]) : FORM[i][0];
                slotUv = {sx, FORM[i][1]};
                posUv = slotUv;
            }
            // Settled units face one another: both sides use the E frame, the
            // right panel horizontally mirrored.
            if (settled && frameCount > 0) {
                autoDir = gImpostorDirs[type][0] % frameCount;
                autoFlip = mirror;
                dbgSec = 0;
            }
            // Giant and Atlas always roll in from the side horizontally; they
            // never turn toward the slot during the entry.
            if (gImpostorSide[type]) {
                autoDir = 0;
                autoFlip = mirror;
                dbgSec = mirror ? 4 : 0;
            }
            // Perspective/sprite settings are user-editable per slot; a slot
            // edit also shifts the whole recorded path by the same delta.
            int sprite = autoDir, spriteEnd = -1, flipMode = -1;
            float scaleMul = 1.0f;
            Vector2 delta = {0, 0};
            if (!persp.empty()) {
                const PerspSlot& ps = persp[std::min<int>(i, (int)persp.size() - 1)];
                if (ps.sprite >= 0) sprite = ps.sprite;
                spriteEnd = ps.spriteEnd;
                flipMode = ps.flip;
                scaleMul = ps.scale;
                delta = {ps.slot.x - slotUv.x, ps.slot.y - slotUv.y};
                slotUv = ps.slot;
            }
            posUv.x += delta.x;
            posUv.y += delta.y;
            // Perspective is active during the slide: the sprite scale follows
            // the unit's current depth, not its final slot.
            float depthNow = std::clamp(posUv.y, 0.0f, 1.0f);
            float cfgScale = std::max(0.2f, (0.72f + 0.80f * depthNow) * scaleMul);
            if (settled && spriteEnd >= 0) sprite = spriteEnd;
            bool useFrames = (frameCount > 0) && gImpostorAtlas.id;
            ImpostorRect ir = {0, 0, baseTex ? (int)baseTex->width : 0,
                               baseTex ? (int)baseTex->height : 0};
            if (useFrames) ir = gImpostorRects[type][sprite % frameCount];
            Texture2D* tex = useFrames ? &gImpostorAtlas : baseTex;
            if (!tex || !tex->id) tex = baseTex;
            if (!tex || !tex->id) continue;
            Rectangle src = {(float)ir.x, (float)ir.y, (float)ir.w, (float)ir.h};
            // The impostor sheet only has the forward/right-facing angles;
            // left-facing motion mirrors it horizontally.
            bool drawFlip = (flipMode < 0) ? autoFlip : (flipMode != 0);
            if (useFrames && drawFlip)
                src = {(float)ir.x, (float)ir.y, -(float)ir.w, (float)ir.h};
            Vector2 dest = panelPoint(px, posUv.x, posUv.y);
            Rectangle dr;
            if (useFrames) {
                // Directional frames are cropped to their opaque bounds; draw
                // them 1:1 with the original panel scale and centre them in the
                // recorded 64x64 slot cell.
                float cellPs = (panW / 256.0f) * cfgScale;
                float fs = cellPs * gImpostorScale[type];
                float fw = ir.w * fs, fh = ir.h * fs;
                float cx = dest.x + nudge + 32.0f * cellPs;
                // Normal frames occupy up to 32 source pixels in the 64 px
                // slot.  Larger sprites (HMB-2 Giant) keep the same ground
                // line by shifting up by half their extra height, instead of
                // hanging over the bottom report.
                float extraH = std::max(0.0f, fh - 32.0f * cellPs);
                float cy = dest.y + 32.0f * cellPs - extraH * 0.5f;
                dr = {std::round(cx - fw / 2), std::round(cy - fh / 2), fw, fh};
            } else {
                float s = panW * 0.25f * cfgScale;
                dr = {dest.x + nudge, dest.y, s, s};
            }
            DrawTexturePro(*tex, src, dr, {0, 0}, 0, c);
            dbgDrawn[mirror ? 1 : 0]++;
            if (gDebugWire) {
                // Double-sided wireframe of the sprite quad (diagonals show the
                // two triangles); the magenta tick marks mirrored sprites.
                DrawRectangleLinesEx(dr, 1.0f, Color{90, 255, 255, 220});
                DrawLineEx({dr.x, dr.y}, {dr.x + dr.width, dr.y + dr.height}, 1.0f,
                           Color{90, 255, 255, 120});
                DrawLineEx({dr.x + dr.width, dr.y}, {dr.x, dr.y + dr.height}, 1.0f,
                           Color{90, 255, 255, 120});
                if (drawFlip) {
                    float my = dr.y + dr.height * 0.5f;
                    DrawLineEx({dr.x + 2, my}, {dr.x + 14, my}, 2.0f, MAGENTA);
                }
                const char* tag = TextFormat("%c f%d%s s%d", mirror ? 'R' : 'L',
                                             sprite, drawFlip ? "<" : "", dbgSec);
                DrawText(tag, (int)dr.x, (int)(dr.y - 13.0f), 12, LIME);
            }
        }
        if (gUnitRecolor.id) EndShaderMode();
    };

    drawArmy(axPanel, nL0, impact ? nL1 : nL0, L.type, false, recoilA, flashA);
    drawArmy(dxPanel, nR0, impact ? nR1 : nR0, R.type, true, recoilD, flashD);

    // ----- surrounding hex diagrams (own unit): allied/enemy cells, plus a
    // strip of this side's participating supporters so the allied support is
    // always shown on the allied side (even if they stand next to the enemy).
    auto hexStatus = [&](const CombatSide& side, float boxX) {
        bool attacker = (side.unit == v.r.atkSlot);
        bool surrounded = !attacker && v.r.surroundedD;
        const auto& list = attacker ? v.r.supportAtkUnits : v.r.supportDefUnits;
        float r = std::max(6.5f, panW * 0.030f);
        float bw = 5.4f * r + 10.0f;
        float bh = 30.0f + 5.28f * r;
        float bx = boxX, by = panY + 6.0f;
        DrawRectangleRounded({bx, by, bw, bh}, 0.12f, 6, Color{0, 0, 0, 170});
        if (surrounded) DrawRectangleLinesEx({bx, by, bw, bh}, 2.0f, kRed);
        const char* tag = attacker ? "ATK" : "DEF";
        float tagW = gUiFontReady
                         ? MeasureTextEx(gUiFont, tag, (float)gUiFont.baseSize, 1.0f).x
                         : (float)MeasureText(tag, 16);
        smallText(bx + (bw - tagW) * 0.5f, by + 2, tag, BORDER);
        if (surrounded) smallText(bx + bw - 26.0f, by + 2, "SURR", kRed);
        Vector2 center = {bx + bw * 0.5f, by + 12.0f + 2.64f * r};
        const float d = 1.72f * r;
        Color teamCol = side.team == 0 ? Color{60, 120, 220, 220}
                                      : Color{60, 180, 90, 220};
        for (int k = 0; k < 6; ++k) {
            int nx, ny;
            hexNeighbor(side.unit >= 0 ? gMission.units[side.unit].x : 0,
                        side.unit >= 0 ? gMission.units[side.unit].y : 0, k, nx, ny);
            int occ = unitAt(gMission, nx, ny);
            // Active = the two combatants plus everyone in either support list.
            bool sup = false;
            for (const SupportContribution& c : list)
                if (occ >= 0 && c.slot == occ) { sup = true; break; }
            bool participant = false;
            int ownerTeam = -1;
            if (occ >= 0) {
                ownerTeam = gMission.units[occ].owner;
                participant = sup || occ == v.r.atkSlot || occ == v.r.defSlot;
                for (const SupportContribution& c : v.r.supportAtkUnits)
                    if (c.slot == occ) participant = true;
                for (const SupportContribution& c : v.r.supportDefUnits)
                    if (c.slot == occ) participant = true;
            }
            float a = (-90.0f + 60.0f * k) * 3.14159265f / 180.0f;
            Vector2 p = {center.x + std::cos(a) * d, center.y + std::sin(a) * d};
            // Team colour for units taking part in the combat, grey for the
            // rest, dark for empty cells.
            Color fill = (occ < 0) ? Color{45, 48, 60, 170}
                        : participant ? (ownerTeam == 0 ? Color{60, 120, 220, 230}
                                                      : Color{60, 180, 90, 230})
                        : Color{105, 105, 112, 210};
            DrawPoly(p, 6, r * 0.92f, 0, fill);
            DrawPolyLinesEx(p, 6, r * 0.92f, 0, sup ? 2.2f : 1.0f,
                            sup ? Color{255, 230, 150, 255}
                                : Color{120, 120, 130, 220});
            if (gDebugWire) {
                DrawPolyLinesEx(p, 6, r * 0.92f, 0, 1.0f, MAGENTA);
                DrawCircleV(p, 1.5f, MAGENTA);
            }
        }
        DrawPoly(center, 6, r * 0.98f, 0, teamCol);
        DrawPolyLinesEx(center, 6, r * 0.98f, 0, 1.5f, WHITE);
        if (gDebugWire) DrawPolyLinesEx(center, 6, r * 0.98f, 0, 1.0f, LIME);
        if (side.unit >= 0) {
            int type = gMission.units[side.unit].type;
            Texture2D& icon = gUnitMapTex[type];
            if (icon.id) {
                float s = r * 1.8f;
                DrawTexturePro(icon, {0, 0, (float)icon.width, (float)icon.height},
                               {center.x - s / 2, center.y - s / 2, s, s}, {0, 0}, 0,
                               side.team == 0 ? WHITE : Color{150, 255, 150, 255});
            }
        }
        // Allied support strip: one icon per participating unit.
        int shown = 0;
        const float isz = 13.0f, igap = 3.0f;
        float iy = by + bh - isz - 2.0f;
        for (const SupportContribution& c : list) {
            if (shown >= 4) break;
            if (c.slot < 0 || c.slot >= (int)gMission.units.size()) continue;
            const Unit& su = gMission.units[c.slot];
            float ix = bx + 4.0f + shown * (isz + igap);
            DrawRectangleLinesEx({ix - 1.0f, iy - 1.0f, isz + 2.0f, isz + 2.0f},
                                 1.0f, Color{255, 190, 60, 255});
            Texture2D& tex = gUnitMapTex[su.type];
            if (tex.id) {
                DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height},
                               {ix, iy, isz, isz}, {0, 0}, 0,
                               side.team == 0 ? WHITE : Color{150, 255, 150, 255});
            }
            ++shown;
        }
        if (shown == 0) smallText(bx + 4, iy + 2, "none", DIM);
    };    {
        float r = std::max(6.5f, panW * 0.030f);
        float bw = 5.4f * r + 10.0f;
        hexStatus(v.left, axPanel + panW - bw - 8.0f);
        hexStatus(v.right, dxPanel + 8.0f);
    }

    // ----- simultaneous fire: every surviving shot aims at a killed vehicle
    // slot when possible, otherwise at a deterministic scatter point.
    auto scatter = [&](int side, int i) {
        int h = (i * 73 + side * 41 + 17) % 100;
        int v = (i * 47 + side * 67 + 23) % 100;
        float rx = 0.12f + 0.76f * (h / 100.0f);
        float ry = 0.14f + 0.72f * (v / 100.0f);
        return panelPoint(side == 0 ? dxPanel : axPanel, rx, ry);
    };

    Vector2 fromL[8], toL[8], fromR[8], toR[8];
    int killedR = std::max(0, nR0 - nR1);   // right-side (enemy) vehicles lost
    int killedL = std::max(0, nL0 - nL1);   // left-side (player) vehicles lost
    for (int i = 0; i < nL0; ++i) {
        fromL[i] = slotVisualCenter(false, i);
        if (i < killedR) {
            int slot = nR1 + i;
            toL[i] = slotVisualCenter(true, slot);
        } else {
            toL[i] = scatter(0, i);
        }
    }
    for (int i = 0; i < nR0; ++i) {
        fromR[i] = slotVisualCenter(true, i);
        if (i < killedL) {
            int slot = nL1 + i;
            toR[i] = slotVisualCenter(false, slot);
        } else {
            toR[i] = scatter(1, i);
        }
    }

    if (t > fireT && t < fireT + fireDur) {
        // Only a side that can actually shoot fires.  Indirect attacks never
        // provoke a counter (original sets the counter strength to 0 in that
        // branch), so the defender stays silent against artillery.
        const bool attackerIsLeft = (v.left.unit == v.r.atkSlot);
        const bool counterFires = (v.r.ctrStrength > 0 || v.r.rawDmgToAtk > 0);
        const bool leftFires = attackerIsLeft || counterFires;
        const bool rightFires = !attackerIsLeft || counterFires;
        auto fire = [&](Vector2* from, Vector2* to, int count, float nudge, int side, int type) {
            const std::vector<int>& proj = vfxProjectileFor(type, side);
            const std::vector<int>& muzz = vfxMuzzleFor(type, side);
            for (int i = 0; i < count; ++i) {
                float st = fireT + i * 0.012f;
                if (t < st) continue;
                float k = std::clamp((t - st) / fireDur, 0.0f, 1.0f);
                Vector2 p = {from[i].x + nudge + (to[i].x - from[i].x - nudge) * k,
                             from[i].y + (to[i].y - from[i].y) * k};
                float ang = std::atan2(to[i].y - from[i].y,
                                       to[i].x - from[i].x) * 57.29578f;
                if (!proj.empty()) {
                    int frame = proj[(int)((t - st) / 0.055f) % (int)proj.size()];
                    drawVfxHalf(frame, p, us * 1.25f, WHITE);
                } else {
                    drawProjectile(p.x, p.y, us * 0.85f, ang);
                }
                if (k < 0.25f) {
                    Vector2 m = {from[i].x + nudge, from[i].y};
                    if (!muzz.empty()) {
                        int frame = muzz[(int)((t - st) / 0.03f) % (int)muzz.size()];
                        drawVfxHalf(frame, m, us * 1.35f, WHITE);
                    } else {
                        DrawCircleV(m, us * 0.20f, Color{255, 220, 120, 255});
                    }
                }
            }
        };
        if (leftFires) fire(fromL, toL, nL0, recoilA, 0, L.type);
        if (rightFires) fire(fromR, toR, nR0, recoilD, 1, R.type);
    }

    // ----- wrecks: the mine's dead sprite (black splat) replaces the unit
    // sprite exactly when the destruction explosion starts, and stays on the
    // ground for the result view.  One per unit destroyed in THIS exchange,
    // staggered with its explosion.
    if (gVfxSplat.id && t > impactT) {
        // 0.7x the previous crater size, centred on the destroyed unit's old
        // visual position (not the 64x64 cell origin).
        float sw = us * 1.60f * 0.7f;
        float sh = sw * (float)gVfxSplat.height / (float)gVfxSplat.width;
        auto wreck = [&](bool rightSide, int baseSlot, int killed) {
            if (killed <= 0) return;
            for (int i = 0; i < killed; ++i) {
                if (t < impactT + i * 0.12f) continue;
                Vector2 c = slotVisualCenter(rightSide, baseSlot + i);
                DrawTexturePro(gVfxSplat,
                               {0, 0, (float)gVfxSplat.width, (float)gVfxSplat.height},
                               {c.x - sw * 0.5f, c.y - sh * 0.5f, sw, sh},
                               {0, 0}, 0, WHITE);
            }
        };
        wreck(true, nR1, killedR);    // enemy vehicles lost to the left side
        wreck(false, nL1, killedL);   // left vehicles lost to the right side
    }

    if (t > impactT && t < impactT + 1.10f) {
        const bool attackerIsLeft = (v.left.unit == v.r.atkSlot);
        const bool counterFires = (v.r.ctrStrength > 0 || v.r.rawDmgToAtk > 0);
        const bool leftFires = attackerIsLeft || counterFires;
        const bool rightFires = !attackerIsLeft || counterFires;
        float dt = t - impactT;
        auto impact = [&](Vector2* to, int count, int killed, int targetType,
                          int damage, int targetSide) {
            const std::vector<int>& death = vfxDeathFor(targetType);
            const std::vector<int>& hit = gVfxHit;
            const std::vector<int>& miss = gVfxMiss;
            for (int i = 0; i < count; ++i) {
                if (i < killed && !death.empty()) {
                    // Stagger kills: the original removes the formation slots
                    // one at a time instead of all in the same frame.
                    float ddel = dt - i * 0.12f;
                    if (ddel < 0.0f) continue;
                    int step = std::clamp((int)(ddel / gVfxDeathStep), 0, (int)death.size() - 1);
                    float fadeStart = (float)death.size() * gVfxDeathStep;
                    float alpha = 1.0f - std::clamp((ddel - fadeStart) / 0.35f, 0.0f, 1.0f);
                    drawVfxQuad(death[step], to[i],
                                us * 2.40f * vfxScaleFor(targetType),
                                Fade(WHITE, alpha));
                } else if (damage > 0 && !hit.empty()) {
                    int frame = hit[(int)(dt / 0.10f) % (int)hit.size()];
                    float a = 1.0f - std::clamp((dt - 0.40f) / 0.40f, 0.0f, 1.0f);
                    drawVfxHalf(frame, to[i], us * 1.55f, Fade(WHITE, a));
                } else if (gVfxMissQuad >= 0 &&
                           gVfxQuad[gVfxMissQuad / 4].id) {
                    // Packed miss sheet (44): four small sprites in one 2x2
                    // square, used as a short ground burst.
                    float a = 1.0f - std::clamp((dt - 0.30f) / 0.30f, 0.0f, 1.0f);
                    drawVfxQuad(gVfxMissQuad, to[i], us * 1.60f, Fade(WHITE, a));
                } else if (!miss.empty()) {
                    // Missed shot: small, short-lived ground burst (clearly not
                    // the multi-frame destruction explosion).
                    int frame = miss[(int)(dt / 0.12f) % (int)miss.size()];
                    float a = 1.0f - std::clamp((dt - 0.30f) / 0.30f, 0.0f, 1.0f);
                    drawVfxHalf(frame, to[i], us * 0.90f, Fade(WHITE, a));
                } else {
                    drawExplosion(to[i].x, to[i].y, us * 0.70f, dt * 7.0f, 1.0f);
                }
            }
            (void)targetSide;
        };
        if (leftFires) impact(toL, nL0, killedR, R.type, v.left.damage, 1);
        if (rightFires) impact(toR, nR0, killedL, L.type, v.right.damage, 0);
    }

    // ----- bottom: persistent simulation report (integer-scaled original font)
    EndScissorMode();
    DrawRectangle((int)playX, (int)botY, (int)playW, (int)botH, NAVY);

    const CombatSide& SL = v.left;
    const CombatSide& SR = v.right;
    char l1[64], l2[64], l3[64], l4[64];
    char r1[64], r2[64], r3[64], r4[64];
    char m1[64], m2[64], m3[64], m4[64], m5[64];
    std::snprintf(l1, sizeof(l1), "ATK %d", SL.atkDisplay);
    std::snprintf(l2, sizeof(l2), "DEF %d", SL.defDisplay);
    std::snprintf(l3, sizeof(l3), "SURR%d SUP%d", SL.surroundPct, SL.support);
    std::snprintf(l4, sizeof(l4), "HP %d>%d XP+%d", SL.hp0, SL.hp1, SL.xpGain);
    std::snprintf(r1, sizeof(r1), "ATK %d", SR.atkDisplay);
    std::snprintf(r2, sizeof(r2), "DEF %d", SR.defDisplay);
    std::snprintf(r3, sizeof(r3), "SURR%d SUP%d", SR.surroundPct, SR.support);
    std::snprintf(r4, sizeof(r4), "HP %d>%d XP+%d", SR.hp0, SR.hp1, SR.xpGain);
    std::snprintf(m1, sizeof(m1), "SIMULATION");
    std::snprintf(m2, sizeof(m2), "DMG %d/%d", SL.damage, SR.damage);
    std::snprintf(m3, sizeof(m3), "SURR %d/%d", SL.surroundPct, SR.surroundPct);
    std::snprintf(m4, sizeof(m4), "TER %d/%d", r.terrainDefA, r.terrainDefD);
    std::snprintf(m5, sizeof(m5), "MODE %s", v.ranged ? "RANGED" : "DIRECT");

    const float colGap = playW * 0.025f;
    const float avail = playW * 0.96f;
    float wL = 0, wC = 0, wR = 0;
    auto measureCols = [&](int s) {
        float fs = basePx * s;
        float sp = std::max(1.0f, (float)s);
        auto tw = [&](const char* str) {
            return gUiFontReady ? MeasureTextEx(gUiFont, str, fs, sp).x
                                : (float)MeasureText(str, (int)fs);
        };
        wL = std::max(std::max(tw(l1), tw(l2)), std::max(tw(l3), tw(l4)));
        wC = std::max(std::max(tw(m1), tw(m2)), std::max(tw(m3), tw(m4)));
        wC = std::max(wC, tw(m5));
        wR = std::max(std::max(tw(r1), tw(r2)), std::max(tw(r3), tw(r4)));
    };
    int rscale = 1;
    for (int s = wantScale; s >= 1; --s) {
        float lh = basePx * s * 1.12f;
        measureCols(s);
        if (5.0f * lh + 10.0f <= botH && wL + wC + wR + 2.0f * colGap <= avail) {
            rscale = s;
            break;
        }
    }
    float rfs = basePx * rscale;
    float lh = rfs * 1.12f;
    float rsp = std::max(1.0f, (float)rscale);
    auto line = [&](float x, float y, const char* s, Color c) {
        if (gUiFontReady) DrawTextEx(gUiFont, s, {x, y}, rfs, rsp, c);
        else DrawText(s, (int)x, (int)y, (int)rfs, c);
    };

    // Columns are laid out from their measured widths, never from fixed
    // fractions, so long values cannot run into the next group.
    measureCols(rscale);
    float colL = playX + playW * 0.02f;
    float colC = colL + wL + colGap;
    float colR = colC + wC + colGap;
    float y0 = botY + std::max(6.0f, (botH - 5.0f * lh) * 0.35f);

    auto lineW = [&](const char* s) {
        return gUiFontReady ? MeasureTextEx(gUiFont, s, rfs, rsp).x
                            : (float)MeasureText(s, (int)rfs);
    };
    line(colL + (wL - lineW("PLAYER")) * 0.5f, y0, "PLAYER", BORDER);
    line(colC + (wC - lineW("SIMULATION")) * 0.5f, y0, "SIMULATION", BORDER);
    line(colR + (wR - lineW("ENEMY")) * 0.5f, y0, "ENEMY", BORDER);
    y0 += lh;

    line(colL, y0, l1, TXT);
    line(colC, y0, m2, TXT);
    line(colR, y0, r1, TXT);

    line(colL, y0 + lh, l2, TXT);
    line(colC, y0 + lh, m3, (SL.surroundPct || SR.surroundPct) ? kRed : DIM);
    line(colR, y0 + lh, r2, TXT);

    line(colL, y0 + lh * 2, l3, SL.surroundPct ? kRed : DIM);
    line(colC, y0 + lh * 2, m4, DIM);
    line(colR, y0 + lh * 2, r3, SR.surroundPct ? kRed : DIM);

    line(colL, y0 + lh * 3, l4, DIM);
    line(colC, y0 + lh * 3, m5, DIM);
    line(colR, y0 + lh * 3, r4, DIM);

    if (gDebugWire) {
        // Layout group bounds: header, panels, report and its columns.
        DrawRectangleLinesEx({playX, 0, playW, topH}, 1.0f, LIME);
        DrawRectangleLinesEx({axPanel, panY, panW, panH}, 1.0f, LIME);
        DrawRectangleLinesEx({dxPanel, panY, panW, panH}, 1.0f, LIME);
        DrawRectangleLinesEx({playX, botY, playW, botH}, 1.0f, LIME);
        DrawRectangleLinesEx({colL, botY, wL, botH}, 1.0f, ORANGE);
        DrawRectangleLinesEx({colC, botY, wC, botH}, 1.0f, ORANGE);
        DrawRectangleLinesEx({colR, botY, wR, botH}, 1.0f, ORANGE);
        DrawText(TextFormat("F1 WIRE  f=frame  <flipped  s=sector  L%d R%d",
                            dbgDrawn[0], dbgDrawn[1]),
                 6, H - 18, 12, LIME);
    }
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
    bool factoryshot = (argc > 3 && std::string(argv[3]) == "--factoryshot");
    bool inspectionshot = (argc > 3 && std::string(argv[3]) == "--inspectshot");
    bool previewshot = (argc > 3 && std::string(argv[3]) == "--previewshot");
    if (inspectionshot || previewshot) shotMode = true;   // reuse the hidden-window shot path
    if (std::getenv("NECTARIS_DEBUG_WIRE")) gDebugWire = true;
    std::string shotPath = (argc > 4) ? argv[4] : "port_shot.png";
    bool selftest = (argc > 3 && std::string(argv[3]) == "--selftest");

    loadData();
    std::string missionPath = (argc > 2) ? argv[2] : gAssetDir + "maps/all_units_test.json";
    if (!loadMission(missionPath)) {
        std::cerr << "cannot load mission " << missionPath << "\n";
        return 1;
    }
    gCombatRng.seed((uint32_t)std::time(nullptr));

    // --inspectshot: open the first enemy unit in the side panel for a still.
    int shotInspect = -1;
    if (inspectionshot) {
        for (int i = 0; i < (int)gMission.units.size(); ++i) {
            if (gMission.units[i].hp > 0 && !gMission.units[i].carried &&
                gMission.units[i].owner == 1) {
                shotInspect = i;
                break;
            }
        }
    }
    // --previewshot: select a unit with an adjacent target and hover that
    // target, so the support-role highlight can be captured.
    int shotSelect = -1, shotHover = -1;
    if (previewshot) {
        for (int i = 0; i < (int)gMission.units.size() && shotSelect < 0; ++i) {
            const Unit& u = gMission.units[i];
            if (u.hp <= 0 || u.carried || u.owner != 0) continue;
            for (int t : attackableTargets(gMission, gTypes, i)) {
                if (hexDistance(u.x, u.y, gMission.units[t].x,
                                gMission.units[t].y) == 1) {
                    shotSelect = i;
                    shotHover = t;
                    break;
                }
            }
        }
    }

    auto campaign = listCampaign(gAssetDir + "maps");
    int campaignIdx = 0;
    {
        std::string want = std::filesystem::path(missionPath).filename().string();
        for (int i = 0; i < (int)campaign.size(); ++i)
            if (std::filesystem::path(campaign[i]).filename().string() == want) campaignIdx = i;
    }

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

    if (argc > 3 && std::string(argv[3]) == "--reach") {
        int slot = (argc > 4) ? std::atoi(argv[4]) : 0;
        if (slot < 0 || slot >= (int)gMission.units.size()) {
            std::cout << "bad slot\n";
            return 1;
        }
        const Unit& u = gMission.units[slot];
        auto dist = reachable(gMission, slot);
        std::cout << "reach " << gTypes[u.type].name << " slot " << slot
                  << " class " << gTypes[u.type].moveClass
                  << " mp " << gTypes[u.type].movePts << "\n";
        for (int y = 0; y < gMission.h; ++y) {
            for (int x = 0; x < gMission.w; ++x) {
                int d = dist[y * gMission.w + x];
                if (d >= (1 << 29)) std::cout << " ##";
                else std::cout << TextFormat("%3d", d);
            }
            std::cout << "\n";
        }
        return 0;
    }

    if (argc > 3 && std::string(argv[3]) == "--demo") {
        for (int t = 0; t < 3; ++t) {
            for (auto& u : gMission.units) u.acted = (u.owner == 0);
            for (;;) {
                int slot = -1;
                for (int i = 0; i < (int)gMission.units.size(); ++i) {
                    const Unit& u = gMission.units[i];
                    if (u.owner == 1 && u.hp > 0 && !u.acted && !u.carried) { slot = i; break; }
                }
                if (slot < 0) break;
                Unit& u = gMission.units[slot];
                int tgt = nearestPlayerTarget(gMission, slot);
                if (tgt < 0) { u.acted = true; continue; }
                std::vector<int> dist = reachable(gMission, slot);
                int cell = aiDestination(gMission, slot, tgt, dist);
                if (cell >= 0) { u.x = cell % gMission.w; u.y = cell / gMission.w; }
                auto targets = attackable(gMission, slot);
                if (!targets.empty()) {
                    int t2 = targets.front();
                    Unit& D = gMission.units[t2];
                    int mode = hexDistance(u.x, u.y, D.x, D.y) - 1;
                    CombatResult r = resolveBattle(slot, t2, mode);
                    applyCombatResult(gMission, r, gCombatRules);
                }
                u.acted = true;
            }
        }
        return 0;
    }

    // Fixed zoom for this session: pick it from the launch mission so the map
    // fits at start, then keep the same unit size when switching with N/B.
    gHexSize = std::clamp(std::min(1000.0f / (1.5f * gMission.w + 1.0f),
                                   620.0f / (1.7320508f * gMission.h + 0.5f)), 14.0f, 42.0f);
    gStepX = 1.5f * gHexSize;
    gHexH = 1.7320508f * gHexSize;
    gStepY = gHexH;
    gOriginX = 24.0f;
    gOriginY = 24.0f;
    updateMapGeometry();
    int mapW = (int)gMapPixelW;
    int mapH = (int)gMapPixelH;
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
    if (factoryshot) SetConfigFlags(FLAG_WINDOW_HIDDEN);
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(mapW + panelW, std::max(mapH, 720), "Nectaris - portable");
    SetExitKey(KEY_NULL);          // ESC is the in-game cancel key, not quit
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
    resolveUnitFolders();
    for (int t = 0; t < 24; ++t) {
        std::string p = unitPath(t, "combat_alive.png");
        if (!FileExists(p.c_str()))
            p = unitPath(t, "horizontal.png");
        gUnitSprite[t] = LoadTexture(p.c_str());
        Image im = LoadImage(p.c_str());
        if (im.data) {
            ImageFlipHorizontal(&im);
            gUnitSpriteFlip[t] = LoadTextureFromImage(im);
            UnloadImage(im);
        }
        std::string v = unitPath(t, "horizontal.png");
        if (!FileExists(v.c_str()))
            v = unitPath(t, "vertical.png");
        gUnitMapTex[t] = LoadTexture(v.c_str());
        Image vm = LoadImage(v.c_str());
        if (vm.data) {
            ImageFlipHorizontal(&vm);
            gUnitMapFlipTex[t] = LoadTextureFromImage(vm);
            UnloadImage(vm);
        }
    }
    // VFX organization taken from the live queues: cells 0..31 are complete
    // single-cell effects; cells 32..63 form eight double-size 64x64 frames.
    for (int i = 0; i < 32; ++i) {
        gVfxHalf[i] = LoadTexture((gAssetDir +
            TextFormat("art/combat_effects/small/vfx_small_%02d.png", i)).c_str());
    }
    for (int start : {32, 36, 40, 48, 52}) {
        gVfxQuad[start / 4] = LoadTexture((gAssetDir +
            TextFormat("art/combat_effects/big/vfx_big_%02d.png", start)).c_str());
    }
    // 44/56/60 are 2x2 sheets of four separate sprites, not explosion frames.
    for (int start : {44, 56, 60}) {
        gVfxQuad[start / 4] = LoadTexture((gAssetDir +
            TextFormat("art/combat_effects/packed/vfx_packed_%02d.png", start)).c_str());
    }
    gVfxSplat = LoadTexture((gAssetDir + "art/combat_effects/splat.png").c_str());
    gFxShell = LoadTexture((gAssetDir + "art/combat_effects/effect_195.png").c_str());
    gFxFlash = LoadTexture((gAssetDir + "art/combat_effects/effect_193.png").c_str());
    gFxStreak = LoadTexture((gAssetDir + "art/combat_effects/effect_195.png").c_str());

    // Exact blue -> green palette pairs from the game's own frame art.
    {
        std::ifstream lf(gAssetDir + "data/unit_palette_lut.json");
        if (lf) {
            json lj = json::parse(lf);
            for (auto it = lj.begin(); it != lj.end(); ++it) {
                int t = std::atoi(it.key().c_str());
                if (t < 0 || t >= 24) continue;
                for (const auto& e : it.value()) {
                    if (e.size() < 2 || e[0].size() < 3 || e[1].size() < 3) continue;
                    Vector3 a{e[0][0].get<float>() / 255.0f,
                              e[0][1].get<float>() / 255.0f,
                              e[0][2].get<float>() / 255.0f};
                    Vector3 b{e[1][0].get<float>() / 255.0f,
                              e[1][1].get<float>() / 255.0f,
                              e[1][2].get<float>() / 255.0f};
                    gUnitLut[t].push_back({a, b});
                }
            }
        }
    }

    // Packed impostor frame atlas (all directional unit frames).
    {
        for (int t = 0; t < 24; ++t) {
            static const int defDirs[8] = {0, 1, 1, 1, 0, 2, 2, 2};
            static const int defFlips[8] = {0, 0, 0, 1, 1, 1, 0, 0};
            for (int k = 0; k < 8; ++k) {
                gImpostorDirs[t][k] = defDirs[k];
                gImpostorFlips[t][k] = defFlips[k];
            }
            gImpostorScale[t] = 1.0f;
            gImpostorSide[t] = false;
        }
        std::ifstream imf(gAssetDir + "data/unit_impostors.json");
        if (imf) {
            json ij = json::parse(imf);
            std::string atlas = gAssetDir +
                ij.value("atlas", std::string("art/unit_impostors_atlas.png"));
            gImpostorAtlas = LoadTexture(atlas.c_str());
            if (gImpostorAtlas.id && ij.contains("units")) {
                for (auto it = ij["units"].begin(); it != ij["units"].end(); ++it) {
                    int t = std::atoi(it.key().c_str());
                    if (t < 0 || t >= 24) continue;
                    const json& u = it.value();
                    const json* rects = nullptr;
                    if (u.is_array()) {
                        rects = &u;
                    } else if (u.contains("frames")) {
                        rects = &u["frames"];
                        if (u.contains("dirs")) {
                            for (int k = 0; k < 8 && k < (int)u["dirs"].size(); ++k)
                                gImpostorDirs[t][k] = u["dirs"][k].get<int>();
                        }
                        if (u.contains("flips")) {
                            for (int k = 0; k < 8 && k < (int)u["flips"].size(); ++k)
                                gImpostorFlips[t][k] = u["flips"][k].get<int>();
                        }
                        if (u.contains("side_facing"))
                            gImpostorSide[t] = u["side_facing"].get<bool>();
                        gImpostorScale[t] = u.value("frame_scale", 1.0f);
                    }
                    if (rects) {
                        for (const auto& r : *rects) {
                            if (r.size() >= 4)
                                gImpostorRects[t].push_back({r[0], r[1], r[2], r[3]});
                        }
                    }
                }
            }
        }
    }

    // Luminance-based team recolor for unit sprites (replaces the flat vertex
    // tint): the art's own shading is preserved and ramped through the team
    // colour.  amount=0 keeps the original palette, amount=1 recolors fully.
    {
        const char* vs =
            "#version 330\n"
            "in vec3 vertexPosition;\n"
            "in vec2 vertexTexCoord;\n"
            "in vec4 vertexColor;\n"
            "uniform mat4 mvp;\n"
            "out vec2 fragTexCoord;\n"
            "out vec4 fragColor;\n"
            "void main(){ fragTexCoord=vertexTexCoord; fragColor=vertexColor;"
            " gl_Position=mvp*vec4(vertexPosition,1.0); }\n";
        const char* fs =
            "#version 330\n"
            "in vec2 fragTexCoord;\n"
            "in vec4 fragColor;\n"
            "uniform sampler2D texture0;\n"
            "uniform vec4 recolorTint;\n"
            "uniform float recolorAmount;\n"
            "uniform vec3 lutFrom[16];\n"
            "uniform vec3 lutTo[16];\n"
            "uniform int lutCount;\n"
            "out vec4 finalColor;\n"
            "void main(){\n"
            "  vec4 tex = texture(texture0, fragTexCoord);\n"
            "  float l = dot(tex.rgb, vec3(0.299,0.587,0.114));\n"
            "  vec3 shadow = recolorTint.rgb * 0.30;\n"
            "  vec3 mid = recolorTint.rgb;\n"
            "  vec3 high = mix(recolorTint.rgb, vec3(1.0), 0.45);\n"
            "  vec3 ramp = l < 0.5 ? mix(shadow, mid, l*2.0)\n"
            "                      : mix(mid, high, (l-0.5)*2.0);\n"
            // Prefer the game's exact palette pair for this unit; fall back to
            // the luminance ramp for colours outside the captured palette.
            "  vec3 mapped = ramp;\n"
            "  float bd = 1e9;\n"
            "  for (int i = 0; i < 16; ++i) {\n"
            "    if (i >= lutCount) break;\n"
            "    float d = distance(tex.rgb, lutFrom[i]);\n"
            "    if (d < bd) { bd = d; mapped = lutTo[i]; }\n"
            "  }\n"
            "  if (bd > 0.012) mapped = ramp;\n"
            "  vec3 col = mix(tex.rgb, mapped, recolorAmount);\n"
            "  finalColor = vec4(col, tex.a) * fragColor;\n"
            "}\n";
        gUnitRecolor = LoadShaderFromMemory(vs, fs);
        if (gUnitRecolor.id) {
            gUnitTintLoc = GetShaderLocation(gUnitRecolor, "recolorTint");
            gUnitAmountLoc = GetShaderLocation(gUnitRecolor, "recolorAmount");
            gUnitLutFromLoc = GetShaderLocation(gUnitRecolor, "lutFrom[0]");
            gUnitLutToLoc = GetShaderLocation(gUnitRecolor, "lutTo[0]");
            gUnitLutCountLoc = GetShaderLocation(gUnitRecolor, "lutCount");
        } else {
            std::cout << "unit recolor shader failed to load\n";
        }
    }

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
            if (argc > 7 && std::string(argv[7]) == "enemy") std::swap(a, d);
            CombatRng shotRng;
            shotRng.seed(0xC0FFEE);
            CombatResult r = resolveCombat(gMission, gTypes, gTerrain, gCombatRules,
                                           a, d, shotMode, shotRng);
            gCv = buildCombatViewFromExchange(a, d, gMission.units[a].hp, gMission.units[d].hp,
                                              r.newHpAtk, r.newHpDef, r, shotMode);
            std::cout << "supportA " << r.supportAtk << " (" << r.supportAtkUnits.size()
                      << " units) supportD " << r.supportDef << " ("
                      << r.supportDefUnits.size() << " units) surroundD " << r.surroundPctD
                      << " surroundA " << r.surroundPctA << "\n";
            gCv.active = true;
            gCv.t = (argc > 5) ? (float)std::atof(argv[5]) : 2.70f;
            std::cout << "combat panels L=" << gCv.left.panel << " (terrain "
                      << gCv.left.terrain << ") R=" << gCv.right.panel
                      << " (terrain " << gCv.right.terrain << ")"
                      << " screen " << GetScreenWidth() << "x" << GetScreenHeight()
                      << " render " << GetRenderWidth() << "x" << GetRenderHeight() << "\n";
            for (int k = 0; k < 6; ++k) {
                BeginDrawing();
                drawCombatScreen(gCv, playerTex, enemyTex);
                EndDrawing();
            }
            TakeScreenshot(shotPath.c_str());
        }
        return 0;
    }

    if (factoryshot) {
        int want = (argc > 5) ? std::atoi(argv[5]) : 1;
        const Factory* f = nullptr;
        for (const Factory& cf : gMission.factories) {
            if (cf.group == want) { f = &cf; break; }
        }
        if (!f && !gMission.factories.empty()) f = &gMission.factories.front();
        if (f) {
            for (int k = 0; k < 6; ++k) {
                BeginDrawing();
                drawFactoryScreen(*f, 0, f->owner == 0);
                EndDrawing();
            }
            TakeScreenshot(shotPath.c_str());
        } else {
            std::cerr << "factory not found for mission " << gMission.name << "\n";
        }
        return f ? 0 : 1;
    }

    int selected = shotSelect;
    int inspect = shotInspect;  // enemy (or busy) unit opened for information only
    int currentPlayer = 0;
    bool showReach = false;
    bool debug = false;
    bool factoryOpen = false;
    int factoryIndex = -1;
    int factorySel = 0;
    bool deployMode = false;
    int deployFactory = -1;
    int deploySlot = -1;
    int deployType = -1;
    std::vector<DeployCell> deployCells;
    bool unloadMode = false;
    int unloadTransport = -1;
    int unloadCargo = -1;
    std::vector<DeployCell> unloadCells;
    int unloadAnimSlot = -1;
    float unloadAnimT = 0.0f, unloadAnimDur = 0.70f;
    float unloadSX = 0, unloadSY = 0, unloadTX = 0, unloadTY = 0;
    int boardAnimSlot = -1, boardAnimTransport = -1;
    float boardAnimT = 0.0f, boardAnimDur = 0.60f;
    float boardSX = 0, boardSY = 0, boardTX = 0, boardTY = 0;
    std::vector<int> reach;
    std::vector<int> targets;
    if (shotSelect >= 0) targets = attackable(gMission, shotSelect);
    std::string notice;
    float noticeT = 0.0f;

    // Animated enemy turn: one unit at a time, stepped along its move path.
    bool aiActive = false;
    float aiWait = 0.0f;
    int aiSlot = -1;
    std::vector<std::pair<int, int>> aiPath;
    int aiPathIdx = 0;
    bool aiAnimating = false;
    int aiAnimSlot = -1;
    float aiAnimT = 0.0f, aiAnimDur = 0.13f;
    float aiFromX = 0, aiFromY = 0, aiToX = 0, aiToY = 0;

    struct Fx { float t; float x, y; int dmg; bool onDefender; };
    std::vector<Fx> fx;
    InitAudioDevice();
    Sound sndSelect = LoadSound((gAssetDir + "audio/se01.wav").c_str());
    Sound sndMove   = LoadSound((gAssetDir + "audio/se02.wav").c_str());
    Sound sndHit    = LoadSound((gAssetDir + "audio/se07.wav").c_str());

    auto resetSelection = [&]() {
        selected = -1;
        inspect = -1;
        showReach = false;
        reach.clear();
        targets.clear();
    };

    auto resetAiTurn = [&]() {
        aiActive = false;
        aiWait = 0.0f;
        aiSlot = -1;
        aiPath.clear();
        aiPathIdx = 0;
        aiAnimating = false;
        aiAnimSlot = -1;
    };

    auto selectUnit = [&](int slot) {
        selected = slot;
        inspect = -1;
        reach = reachable(gMission, slot);
        targets = attackable(gMission, slot);
        showReach = true;
    };

    auto say = [&](const std::string& s) {
        notice = s;
        noticeT = 2.5f;
    };

    auto resetActionModes = [&]() {
        factoryOpen = false;
        factoryIndex = -1;
        factorySel = 0;
        deployMode = false;
        deployFactory = deploySlot = deployType = -1;
        deployCells.clear();
        unloadMode = false;
        unloadTransport = unloadCargo = -1;
        unloadCells.clear();
        boardAnimSlot = boardAnimTransport = -1;
        unloadAnimSlot = -1;
    };

    auto openFactory = [&](int fi) {
        if (fi < 0 || fi >= (int)gMission.factories.size()) return;
        resetSelection();
        factoryOpen = true;
        factoryIndex = fi;
        factorySel = 0;
        if (IsSoundValid(sndSelect)) PlaySound(sndSelect);
    };

    // Factory deployment is deliberately separate from movement: the stored
    // unit is spawned on an adjacent cell (or directly into a transport).
    auto startDeploy = [&](int fi, int slot) -> bool {
        if (fi < 0 || fi >= (int)gMission.factories.size()) return false;
        Factory& f = gMission.factories[fi];
        if (f.owner != currentPlayer) return false;
        if (slot < 0 || slot >= (int)f.roster.size()) return false;
        int type = f.roster[slot];
        if (type < 0 || type >= (int)gTypes.size()) return false;
        const UnitType& ut = gTypes[type];

        deployCells.clear();
        for (int d = 0; d < 6; ++d) {
            int nx, ny;
            hexNeighbor(f.x, f.y, d, nx, ny);
            if (!inBounds(gMission, nx, ny)) continue;
            int ni = unitAt(gMission, nx, ny);
            if (ni >= 0) {
                const Unit& t = gMission.units[ni];
                if (t.owner == f.owner && !t.carried &&
                    isTransport(gTypes[t.type]) && t.cargo < 0 &&
                    canCarry(gTypes[t.type], ut))
                    deployCells.push_back({nx, ny, ni});
                continue;                 // occupied; only empty transports
            }
            bool ok;
            if (ut.airborne() || ut.moveClass == 4) {
                ok = true;                // aircraft ignore terrain
            } else {
                int terr = terrainAt(gMission, nx, ny);
                ok = terr != 0 && terr != 4 &&
                     (moveCost(gMission, nx, ny, ut.moveClass) & kMoveImpassable) == 0;
            }
            if (ok) deployCells.push_back({nx, ny, -1});
        }
        if (deployCells.empty()) {
            say("No free adjacent deployment cell");
            return false;
        }
        deployFactory = fi;
        deploySlot = slot;
        deployType = type;
        deployMode = true;
        factoryOpen = false;
        resetSelection();
        if (IsSoundValid(sndSelect)) PlaySound(sndSelect);
        return true;
    };

    auto startUnload = [&](int transSlot) -> bool {
        if (transSlot < 0 || transSlot >= (int)gMission.units.size()) return false;
        Unit& t = gMission.units[transSlot];
        if (t.cargo < 0 || t.cargo >= (int)gMission.units.size()) return false;
        Unit& c = gMission.units[t.cargo];
        if (c.acted) {
            say("Cargo must wait one full turn before unloading");
            return false;
        }
        unloadCells.clear();
        for (int d = 0; d < 6; ++d) {
            int nx, ny;
            hexNeighbor(t.x, t.y, d, nx, ny);
            if (!inBounds(gMission, nx, ny)) continue;
            if (unitAt(gMission, nx, ny) >= 0) continue;
            if (!canUnloadTerrain(terrainAt(gMission, nx, ny))) continue;
            unloadCells.push_back({nx, ny, -1});
        }
        if (unloadCells.empty()) {
            say("No valid unload hex next to the transport");
            return false;
        }
        unloadTransport = transSlot;
        unloadCargo = t.cargo;
        unloadMode = true;
        resetSelection();
        if (IsSoundValid(sndSelect)) PlaySound(sndSelect);
        return true;
    };

    // Move a friendly unit onto an eligible, empty transport in shift range.
    auto boardCargo = [&](int cargoSlot, int transSlot) -> bool {
        Unit& c = gMission.units[cargoSlot];
        Unit& t = gMission.units[transSlot];
        if (c.carried || t.carried || c.owner != t.owner) return false;
        if (!isTransport(gTypes[t.type])) return false;
        if (t.cargo >= 0) { say("Transport is already carrying a unit"); return false; }
        if (c.cargo >= 0) { say("A loaded transport cannot be loaded again"); return false; }
        if (!canCarry(gTypes[t.type], gTypes[c.type])) {
            say(gTypes[t.type].name + " cannot carry " + gTypes[c.type].name);
            return false;
        }
        if (!inShiftRange(gMission, cargoSlot, transSlot)) {
            say("Transport is outside the unit's shift range");
            return false;
        }
        float sx, sy, tx, ty;
        hexCenter(c.x, c.y, sx, sy);
        hexCenter(t.x, t.y, tx, ty);
        float usz = mapUnitSize();
        boardSX = (c.px >= 0) ? c.px : sx - usz / 2;
        boardSY = (c.py >= 0) ? c.py : sy - usz / 2;
        boardTX = (t.px >= 0) ? t.px : tx - usz / 2;
        boardTY = (t.py >= 0) ? t.py : ty - usz / 2;
        boardAnimSlot = cargoSlot;
        boardAnimTransport = transSlot;
        boardAnimT = 0.0f;
        // Keep the cargo visible until the slide into the transport ends.
        c.carried = false;
        c.acted = true;
        c.px = boardSX;
        c.py = boardSY;
        if (IsSoundValid(sndSelect)) PlaySound(sndSelect);
        say(gTypes[c.type].name + " loaded onto " + gTypes[t.type].name);
        if (!t.acted) selectUnit(transSlot);
        else resetSelection();
        return true;
    };

    auto aiStartWaypoint = [&]() -> bool {
        if (aiSlot < 0 || aiPathIdx >= (int)aiPath.size()) return false;
        Unit& u = gMission.units[aiSlot];
        float hx, hy;
        hexCenter(aiPath[aiPathIdx].first, aiPath[aiPathIdx].second, hx, hy);
        float usz = mapUnitSize();
        aiFromX = (u.px >= 0) ? u.px : hx - usz / 2;
        aiFromY = (u.py >= 0) ? u.py : hy - usz / 2;
        aiToX = hx - usz / 2;
        aiToY = hy - usz / 2;
        aiAnimT = 0.0f;
        aiAnimating = true;
        aiAnimSlot = aiSlot;
        ++aiPathIdx;
        return true;
    };

    auto aiFinishAction = [&]() {
        if (aiSlot >= 0 && aiSlot < (int)gMission.units.size()) {
            Unit& u = gMission.units[aiSlot];
            auto atkTargets = attackable(gMission, aiSlot);
            if (!atkTargets.empty()) {
                int tgt = atkTargets.front();
                Unit& D = gMission.units[tgt];
                int mode = hexDistance(u.x, u.y, D.x, D.y) - 1;
                int hpA0 = u.hp, hpD0 = D.hp;
                CombatResult r = resolveBattle(aiSlot, tgt, mode);
                applyCombatResult(gMission, r, gCombatRules);
                queueCombat(aiSlot, tgt, hpA0, hpD0, u.hp, D.hp, r, mode);
                std::cout << "[AI] " << gTypes[u.type].name << " -> " << gTypes[D.type].name
                          << "  dmg " << r.dmgToDef << "/" << r.dmgToAtk
                          << "  hp " << r.newHpDef << "/" << r.newHpAtk << "\n";
                startNextCombat();
            }
            u.acted = true;
        }
        aiSlot = -1;
        aiAnimSlot = -1;
        aiAnimating = false;
        aiPath.clear();
        aiPathIdx = 0;
        aiWait = 0.30f;
    };

    auto aiBeginAction = [&]() -> bool {
        int slot = -1;
        for (int i = 0; i < (int)gMission.units.size(); ++i) {
            const Unit& u = gMission.units[i];
            if (u.owner == 1 && u.hp > 0 && !u.acted && !u.carried) { slot = i; break; }
        }
        if (slot < 0) return false;
        Unit& u = gMission.units[slot];
        int tgt = nearestPlayerTarget(gMission, slot);
        if (tgt < 0) {
            u.acted = true;
            aiWait = 0.15f;
            return true;
        }
        std::vector<int> dist = reachable(gMission, slot);
        int cell = aiDestination(gMission, slot, tgt, dist);
        aiSlot = slot;
        aiPath.clear();
        aiPathIdx = 0;
        aiAnimating = false;
        aiAnimSlot = -1;
        if (cell >= 0) {
            int tx = cell % gMission.w, ty = cell / gMission.w;
            aiPath = movementPath(gMission, slot, dist, tx, ty);
            float usz = mapUnitSize();
            if (u.px < 0) {
                float hx, hy;
                hexCenter(u.x, u.y, hx, hy);
                u.px = hx - usz / 2;
                u.py = hy - usz / 2;
            }
            u.x = tx;
            u.y = ty;
        }
        if (!aiPath.empty()) aiStartWaypoint();
        else aiWait = 0.18f;      // no move: brief pause, then resolve attack
        std::cout << "[ai] " << gTypes[u.type].name << " "
                  << u.x << "," << u.y << " path " << aiPath.size() << std::endl;
        return true;
    };

    std::cout << "loaded " << gMission.name << " " << gMission.w << "x" << gMission.h
              << ", units " << gMission.units.size() << std::endl;

    int frame = 0;
    while (!WindowShouldClose()) {
        if (gCv.active) {
            gCv.t += GetFrameTime();
            if (IsKeyPressed(KEY_F1)) gDebugWire = !gDebugWire;
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
        if (deployMode) {
            if (deployFactory < 0 || deployFactory >= (int)gMission.factories.size() ||
                deployType < 0 || deployType >= (int)gTypes.size()) {
                deployMode = false;
                continue;
            }
            Factory& f = gMission.factories[deployFactory];
            bool finished = false;
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_SPACE)) {
                say("Deployment cancelled");
                deployMode = false;
                deployFactory = deploySlot = deployType = -1;
                deployCells.clear();
                finished = true;
            } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                int mx = 0, my = 0;
                bool inMap = pixelToHex(GetMousePosition().x, GetMousePosition().y,
                                        gMission.w, gMission.h, mx, my);
                const DeployCell* target = nullptr;
                if (inMap) {
                    for (const DeployCell& c : deployCells)
                        if (c.x == mx && c.y == my) { target = &c; break; }
                }
                if (target) {
                    Unit u;
                    u.type = deployType;
                    u.owner = f.owner;
                    u.hp = 7;
                    u.rank = 0;
                    u.xp = 0;
                    u.acted = true;
                    u.facing = 90.0f;
                    u.px = u.py = -1.0f;
                    if (target->transportSlot >= 0) {
                        int ts = target->transportSlot;
                        float sx, sy, tx, ty;
                        hexCenter(f.x, f.y, sx, sy);
                        hexCenter(gMission.units[ts].x, gMission.units[ts].y, tx, ty);
                        float usz = mapUnitSize();
                        u.x = f.x;
                        u.y = f.y;
                        boardSX = sx - usz / 2;
                        boardSY = sy - usz / 2;
                        boardTX = (gMission.units[ts].px >= 0)
                                      ? gMission.units[ts].px : tx - usz / 2;
                        boardTY = (gMission.units[ts].py >= 0)
                                      ? gMission.units[ts].py : ty - usz / 2;
                        u.px = boardSX;
                        u.py = boardSY;
                        int idx = (int)gMission.units.size();
                        gMission.units.push_back(u);
                        boardAnimSlot = idx;
                        boardAnimTransport = ts;
                        boardAnimT = 0.0f;
                        say(gTypes[deployType].name + " deployed into transport");
                    } else {
                        u.x = target->x;
                        u.y = target->y;
                        gMission.units.push_back(u);
                        say(gTypes[deployType].name + " deployed");
                    }
                    if (deploySlot >= 0 && deploySlot < (int)f.roster.size())
                        f.roster.erase(f.roster.begin() + deploySlot);
                    // Keep the side-panel garrison count in sync with the roster.
                    for (auto it = gMission.garrison.begin();
                         it != gMission.garrison.end(); ++it) {
                        if (it->type == deployType && it->x == f.x && it->y == f.y) {
                            gMission.garrison.erase(it);
                            break;
                        }
                    }
                    if (IsSoundValid(sndMove)) PlaySound(sndMove);
                    deployMode = false;
                    deployFactory = deploySlot = deployType = -1;
                    deployCells.clear();
                    finished = true;
                }
            }
            if (deployMode) {
                drawTargetingFrame(gMission, f.x, f.y, deployType, deployCells,
                                   "DEPLOY UNIT", false,
                                   factoryOwnerColor(f.owner),
                                   TextFormat("Factory %d  at %d,%d",
                                              f.group, f.x, f.y));
                continue;
            }
            if (finished) continue;
        }
        if (unloadMode) {
            if (unloadTransport < 0 || unloadTransport >= (int)gMission.units.size() ||
                unloadCargo < 0 || unloadCargo >= (int)gMission.units.size()) {
                unloadMode = false;
                continue;
            }
            Unit& t = gMission.units[unloadTransport];
            Unit& c = gMission.units[unloadCargo];
            bool finished = false;
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_SPACE)) {
                say("Unload cancelled");
                unloadMode = false;
                unloadTransport = unloadCargo = -1;
                unloadCells.clear();
                finished = true;
            } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                int mx = 0, my = 0;
                bool inMap = pixelToHex(GetMousePosition().x, GetMousePosition().y,
                                        gMission.w, gMission.h, mx, my);
                const DeployCell* target = nullptr;
                if (inMap) {
                    for (const DeployCell& cell : unloadCells)
                        if (cell.x == mx && cell.y == my) { target = &cell; break; }
                }
                if (target) {
                    float sx, sy, tx, ty;
                    hexCenter(t.x, t.y, sx, sy);
                    hexCenter(target->x, target->y, tx, ty);
                    float usz = mapUnitSize();
                    // Dedicated slide-out animation from the transport cell to
                    // the chosen target hex.
                    unloadSX = (t.px >= 0) ? t.px : sx - usz / 2;
                    unloadSY = (t.py >= 0) ? t.py : sy - usz / 2;
                    unloadTX = tx - usz / 2;
                    unloadTY = ty - usz / 2;
                    unloadAnimSlot = unloadCargo;
                    unloadAnimT = 0.0f;
                    c.px = unloadSX;
                    c.py = unloadSY;
                    c.x = target->x;
                    c.y = target->y;
                    c.carried = false;
                    c.acted = true;
                    c.facing = std::atan2(ty - sy, tx - sx) * 57.29578f - 90.0f;
                    t.cargo = -1;
                    t.acted = true;
                    if (IsSoundValid(sndMove)) PlaySound(sndMove);
                    say(gTypes[c.type].name + " unloaded");
                    unloadMode = false;
                    unloadTransport = unloadCargo = -1;
                    unloadCells.clear();
                    finished = true;
                }
            }
            if (unloadMode) {
                drawTargetingFrame(gMission, t.x, t.y, c.type, unloadCells,
                                   "UNLOAD UNIT", true, Color{90, 210, 255, 255},
                                   TextFormat("Transport %s  at %d,%d",
                                              gTypes[t.type].name.c_str(), t.x, t.y));
                continue;
            }
            if (finished) continue;
        }
        if (factoryOpen) {
            if (factoryIndex < 0 || factoryIndex >= (int)gMission.factories.size()) {
                factoryOpen = false;
                factoryIndex = -1;
            } else {
                Factory& f = gMission.factories[factoryIndex];
                if (IsKeyPressed(KEY_ESCAPE)) {
                    factoryOpen = false;
                } else if (IsKeyPressed(KEY_SPACE)) {
                    if (f.owner == currentPlayer && factorySel >= 0 &&
                        factorySel < (int)f.roster.size()) {
                        startDeploy(factoryIndex, factorySel);
                    } else {
                        factoryOpen = false;
                    }
                } else {
                    if (IsKeyPressed(KEY_LEFT))
                        factorySel = (factorySel % 4 == 0) ? factorySel + 3 : factorySel - 1;
                    if (IsKeyPressed(KEY_RIGHT))
                        factorySel = (factorySel % 4 == 3) ? factorySel - 3 : factorySel + 1;
                    if (IsKeyPressed(KEY_UP))
                        factorySel = (factorySel < 4) ? factorySel + 8 : factorySel - 4;
                    if (IsKeyPressed(KEY_DOWN))
                        factorySel = (factorySel >= 8) ? factorySel - 8 : factorySel + 4;
                    factorySel = std::clamp(factorySel, 0, 11);

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        Vector2 m = GetMousePosition();
                        FactoryLayout L = factoryLayout(GetScreenWidth(), GetScreenHeight());
                        bool hitSlot = false;
                        for (int i = 0; i < 12; ++i) {
                            if (CheckCollisionPointRec(m, factorySlotRect(L, i))) {
                                factorySel = i;
                                hitSlot = true;
                                break;
                            }
                        }
                        if (!hitSlot && !CheckCollisionPointRec(m, L.panel))
                            factoryOpen = false;
                    }
                }
                BeginDrawing();
                ClearBackground(Color{8, 9, 14, 255});
                drawFactoryScreen(f, factorySel, f.owner == currentPlayer);
                EndDrawing();
                continue;
            }
        }
        // ----- world-map layout, computed before hit-testing so the map
        // viewport can clip the cursor and the art stays centred in its frame.
        const int scrW = GetScreenWidth(), scrH = GetScreenHeight();
        const int sideW = std::clamp((int)(scrW * 0.33f), 420, 600);
        const int mapAreaW = scrW - sideW;
        Rectangle mapFrame{6.0f, 6.0f, (float)(mapAreaW - 12),
                           (float)(scrH - 12)};
        Rectangle mapView{mapFrame.x + 4, mapFrame.y + 4,
                          mapFrame.width - 8, mapFrame.height - 8 - 34};
        // Centre the mission map inside the viewport when the mission is loaded
        // (initial load, N/B, R) or the window changes.  Panning afterwards is
        // left alone.
        static int ctrViewW = -1, ctrViewH = -1;
        if (gMapNeedsCenter || ctrViewW != (int)mapView.width ||
            ctrViewH != (int)mapView.height) {
            gPanX = mapView.x + (mapView.width - gMapPixelW) * 0.5f;
            gPanY = mapView.y + (mapView.height - gMapPixelH) * 0.5f;
            gMapNeedsCenter = false;
            ctrViewW = (int)mapView.width; ctrViewH = (int)mapView.height;
        }
        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && !IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            Vector2 d = GetMouseDelta();
            // Grab-style pan: the map follows the cursor.
            gPanX += d.x;
            gPanY += d.y;
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            // Edge auto-scroll: drag to a window edge and the map keeps moving
            // in that direction, so one gesture can cross the whole map.
            Vector2 mp = GetMousePosition();
            float W = (float)GetScreenWidth(), H = (float)GetScreenHeight();
            const float edge = 42.0f;
            float speed = 900.0f * GetFrameTime();
            if (mp.x < edge) gPanX -= speed * (1.0f - mp.x / edge);
            else if (mp.x > W - edge) gPanX += speed * ((mp.x - (W - edge)) / edge);
            if (mp.y < edge) gPanY -= speed * (1.0f - mp.y / edge);
            else if (mp.y > H - edge) gPanY += speed * ((mp.y - (H - edge)) / edge);
        }
        {
            // Mouse wheel: scroll the view vertically (up = look up).
            float wheel = GetMouseWheelMove();
            if (wheel != 0.0f) gPanY += wheel * gStepY * 0.75f;
            // Allow a little empty space around the map so its edges can be
            // inspected comfortably (about 1.5 hexes of padding per side).
            const float padX = gStepX * 1.5f;
            const float padY = gStepY * 1.5f;
            float lx = gMapPixelW * 0.5f + padX, ly = gMapPixelH * 0.5f + padY;
            gPanX = std::clamp(gPanX, -lx, lx);
            gPanY = std::clamp(gPanY, -ly, ly);
        }
        int mx = 0, my = 0;
        bool inMap = pixelToHex(mouse.x, mouse.y, gMission.w, gMission.h, mx, my) &&
                      CheckCollisionPointRec(mouse, mapView);
        if (shotHover >= 0 && shotHover < (int)gMission.units.size()) {
            mx = gMission.units[shotHover].x;
            my = gMission.units[shotHover].y;
            inMap = true;
        }

        if (IsKeyPressed(KEY_SPACE) && !aiActive) {
            if (selected >= 0) {
                resetSelection();          // SPACE cancels a selection first
            } else if (currentPlayer == 0) {
                currentPlayer = 1;
                for (auto& u : gMission.units) u.acted = (u.owner == 0);
                std::cout << "[turn] enemy" << std::endl;
                // Start the animated enemy turn; units act one at a time.
                aiActive = true;
                aiWait = 0.45f;
                aiSlot = -1;
                aiPath.clear();
                aiPathIdx = 0;
                aiAnimating = false;
                aiAnimSlot = -1;
                resetSelection();
            } else {
                currentPlayer = 0;
                for (auto& u : gMission.units) u.acted = (u.owner == 1);
            }
        }
        if (IsKeyPressed(KEY_R)) {
            loadMission(missionPath);
            resetSelection();
            resetActionModes();
            resetAiTurn();
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
            resetActionModes();
            resetAiTurn();
            fx.clear();
            gCombatQueue.clear();
            gCv.active = false;
            currentPlayer = 0;
        }
        if (IsKeyPressed(KEY_B) && !campaign.empty()) {
            campaignIdx = (campaignIdx - 1 + (int)campaign.size()) % (int)campaign.size();
            loadMission(campaign[campaignIdx]);
            missionPath = campaign[campaignIdx];
            resetSelection();
            resetActionModes();
            resetAiTurn();
            fx.clear();
            gCombatQueue.clear();
            gCv.active = false;
            currentPlayer = 0;
        }
        if (IsKeyPressed(KEY_ESCAPE))
            resetSelection();

        if (!aiActive && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int hit = inMap ? unitAt(gMission, mx, my) : -1;
            if (selected < 0) {
                if (hit >= 0 && gMission.units[hit].owner == currentPlayer &&
                    !gMission.units[hit].acted && !gMission.units[hit].carried) {
                    selectUnit(hit);
                    if (IsSoundValid(sndSelect)) PlaySound(sndSelect);
                } else if (hit >= 0) {
                    // Enemy unit (or a friendly unit that already acted): show
                    // its information without allowing any order.
                    inspect = (inspect == hit) ? -1 : hit;
                    if (IsSoundValid(sndSelect)) PlaySound(sndSelect);
                } else if (inMap) {
                    inspect = -1;
                    int fi = factoryIndexAt(gMission, mx, my);
                    if (fi >= 0) openFactory(fi);
                }
            } else {
                Unit& sel = gMission.units[selected];
                if (hit == selected) {
                    resetSelection();      // clicking the same unit again deselects
                }
                // Clicking another friendly unit selects it; clicking a friendly
                // transport tries to shift the selected unit aboard instead.
                else if (hit >= 0 && gMission.units[hit].owner == currentPlayer &&
                         !gMission.units[hit].acted && !gMission.units[hit].carried) {
                    if (isTransport(gTypes[gMission.units[hit].type])) {
                        if (!boardCargo(selected, hit)) {
                            selectUnit(hit);
                            if (IsSoundValid(sndSelect)) PlaySound(sndSelect);
                        }
                    } else {
                        selectUnit(hit);
                        if (IsSoundValid(sndSelect)) PlaySound(sndSelect);
                    }
                }
                // attack?
                else if (hit >= 0 && std::find(targets.begin(), targets.end(), hit) != targets.end()) {
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
                } else if (inMap && hit < 0 && !sel.acted &&
                           reach[my * gMission.w + mx] <= gTypes[sel.type].movePts) {
                    float ox, oy, nx2, ny2;
                    hexCenter(sel.x, sel.y, ox, oy);
                    hexCenter(mx, my, nx2, ny2);
                    sel.facing = std::atan2(ny2 - oy, nx2 - ox) * 57.29578f - 90.0f;
                    sel.x = mx;
                    sel.y = my;
                    sel.acted = true;          // movement consumes the unit's turn
                    inspect = -1;
                    if (IsSoundValid(sndMove)) PlaySound(sndMove);
                    // after moving, allow an immediate attack
                    targets = attackable(gMission, selected);
                    showReach = false;
                } else if (hit >= 0) {
                    // Unit that is not a legal target: inspect it instead.
                    inspect = (inspect == hit) ? -1 : hit;
                } else if (inMap) {
                    int fi = factoryIndexAt(gMission, mx, my);
                    if (fi >= 0) openFactory(fi);
                }
            }
        }

        // U opens the adjacent unload-target selection for the chosen transport.
        if (!aiActive && IsKeyPressed(KEY_U) && selected >= 0) {
            startUnload(selected);
        }

        syncCargo(gMission);

        float dt = GetFrameTime();

        // ----- animated enemy turn: one unit at a time, hex by hex
        if (aiActive && currentPlayer == 1 && !gCv.active && gCombatQueue.empty()) {
            if (aiAnimating) {
                aiAnimT += dt;
                float k = std::clamp(aiAnimT / aiAnimDur, 0.0f, 1.0f);
                float e = k * k * (3.0f - 2.0f * k);
                if (aiSlot >= 0 && aiSlot < (int)gMission.units.size()) {
                    Unit& u = gMission.units[aiSlot];
                    u.px = aiFromX + (aiToX - aiFromX) * e;
                    u.py = aiFromY + (aiToY - aiFromY) * e;
                }
                if (k >= 1.0f) {
                    if (aiSlot >= 0 && aiSlot < (int)gMission.units.size()) {
                        Unit& u = gMission.units[aiSlot];
                        u.px = aiToX;
                        u.py = aiToY;
                    }
                    aiAnimating = false;
                    aiAnimSlot = -1;
                    if (aiPathIdx < (int)aiPath.size()) aiStartWaypoint();
                    else aiWait = 0.12f;
                }
            } else if (aiWait > 0.0f) {
                aiWait -= dt;
            } else if (aiSlot >= 0) {
                aiFinishAction();
            } else if (!aiBeginAction()) {
                // Enemy turn finished: hand control back to the player.
                aiActive = false;
                currentPlayer = 0;
                for (auto& u : gMission.units) u.acted = (u.owner == 1);
                resetSelection();
                std::cout << "[turn] player" << std::endl;
            }
        }

        // smooth movement + effect timers
        float usz = mapUnitSize();
        for (int ui = 0; ui < (int)gMission.units.size(); ++ui) {
            Unit& u = gMission.units[ui];
            float hx, hy;
            hexCenter(u.x, u.y, hx, hy);
            float tx = hx - usz / 2, ty = hy - usz / 2;
            if (aiAnimating && ui == aiAnimSlot) {
                // driven by the enemy-turn animation above
            } else if (ui == unloadAnimSlot && unloadAnimT < unloadAnimDur) {
                unloadAnimT += dt;
                float k = std::clamp(unloadAnimT / unloadAnimDur, 0.0f, 1.0f);
                float e = k * k * (3.0f - 2.0f * k);   // smoothstep
                u.px = unloadSX + (unloadTX - unloadSX) * e;
                u.py = unloadSY + (unloadTY - unloadSY) * e;
                if (k >= 1.0f) unloadAnimSlot = -1;
            } else if (ui == boardAnimSlot && boardAnimT < boardAnimDur) {
                boardAnimT += dt;
                float k = std::clamp(boardAnimT / boardAnimDur, 0.0f, 1.0f);
                float e = k * k * (3.0f - 2.0f * k);   // smoothstep
                u.px = boardSX + (boardTX - boardSX) * e;
                u.py = boardSY + (boardTY - boardSY) * e;
                if (k >= 1.0f) {
                    if (boardAnimTransport >= 0 &&
                        boardAnimTransport < (int)gMission.units.size()) {
                        Unit& tr = gMission.units[boardAnimTransport];
                        u.x = tr.x;
                        u.y = tr.y;
                        u.carried = true;      // hide only after the slide
                        tr.cargo = ui;
                    }
                    boardAnimSlot = -1;
                    boardAnimTransport = -1;
                }
            } else {
                if (u.px < 0) { u.px = tx; u.py = ty; }
                u.px += (tx - u.px) * 0.25f;
                u.py += (ty - u.py) * 0.25f;
            }
        }
        for (auto& f : fx) f.t -= dt;
        fx.erase(std::remove_if(fx.begin(), fx.end(),
                                [](const Fx& f) { return f.t <= 0; }), fx.end());
        if (noticeT > 0) noticeT -= dt;

        BeginDrawing();
        ClearBackground(Color{19, 33, 45, 255});

        // Deep-space backdrop: only the margins around the map art are visible,
        // the rest sits behind the opaque map backdrop.
        DrawCircleV({mapView.x + mapView.width * 0.12f,
                     mapView.y + mapView.height * 0.34f}, 230.0f,
                    Color{34, 70, 120, 16});
        DrawCircleV({mapView.x + mapView.width * 0.86f,
                     mapView.y + mapView.height * 0.72f}, 190.0f,
                    Color{40, 50, 100, 14});
        for (int i = 0; i < 1500; ++i) {
            int sx = (int)mapView.x + (i * 197 + 53) % std::max(1, (int)mapView.width);
            int sy2 = (int)mapView.y + (i * 131 + 17) % std::max(1, (int)mapView.height);
            int b = i % 13;
            Color sc = (b == 0) ? Color{230, 238, 250, 255}
                     : (b < 4) ? Color{160, 185, 220, 255}
                     : (b < 8) ? Color{105, 130, 175, 255}
                               : Color{70, 92, 130, 255};
            DrawPixel(sx, sy2, sc);
            if (b == 0) {
                DrawPixel(sx + 1, sy2, sc);
                DrawPixel(sx, sy2 + 1, sc);
                DrawPixel(sx + 1, sy2 + 1, sc);
            }
        }
        BeginScissorMode((int)mapView.x, (int)mapView.y,
                         (int)mapView.width, (int)mapView.height);
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
            if (u.hp <= 0 || u.carried) continue;
            drawUnitSprite(gMission, u, playerTex, enemyTex);
            float cx, cy;
            hexCenter(u.x, u.y, cx, cy);
            if (u.px >= 0) { cx = u.px + usz / 2; cy = u.py + usz / 2; }
            if (u.acted)
                DrawPoly({cx, cy}, 6, gHexSize - 2, 0, Fade(BLACK, 0.35f));
            for (int i = 0; i < u.hp; ++i)
                DrawRectangle((int)(cx - u.hp * 4 + i * 8), (int)(cy + gHexSize * 0.55f),
                              6, 5, u.owner == 0 ? kGreen : kRed);
            int stars = std::min(u.rank, 7);
            if (stars > 0) {
                float sy = cy + gHexSize * 0.55f + 9.0f;
                float sx0 = cx - (stars - 1) * 5.0f;
                for (int i = 0; i < stars; ++i)
                    drawStar(sx0 + i * 10.0f, sy, 3.6f, Color{255, 208, 60, 255});
            }
            if (u.cargo >= 0) {   // loaded transport marker
                DrawCircleV({cx + gHexSize * 0.62f, cy - gHexSize * 0.55f}, 4.5f, SKYBLUE);
                DrawCircleLines((int)(cx + gHexSize * 0.62f),
                                (int)(cy - gHexSize * 0.55f), 4.5f, BLACK);
            }
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
        // Attack preview: hovering a valid target marks the units that would
        // join the exchange as support (direct attacks only).
        {
            int hoverSlot = inMap ? unitAt(gMission, mx, my) : -1;
            if (selected >= 0 && hoverSlot >= 0 &&
                std::find(targets.begin(), targets.end(), hoverSlot) != targets.end()) {
                const Unit& A = gMission.units[selected];
                const Unit& D = gMission.units[hoverSlot];
                if (hexDistance(A.x, A.y, D.x, D.y) == 1) {
                    float cx, cy;
                    hexCenter(D.x, D.y, cx, cy);
                    DrawPolyLinesEx({cx, cy}, 6, gHexSize, 0, 4.0f, WHITE);
                    // allies of the attacker next to the defender = attack support
                    for (int d = 0; d < 6; ++d) {
                        int nx, ny;
                        hexNeighbor(D.x, D.y, d, nx, ny);
                        if (!inBounds(gMission, nx, ny)) continue;
                        int s = unitAt(gMission, nx, ny);
                        if (s < 0 || s == selected || s == hoverSlot) continue;
                        if (gMission.units[s].owner != A.owner) continue;
                        float hx, hy;
                        hexCenter(nx, ny, hx, hy);
                        DrawPolyLinesEx({hx, hy}, 6, gHexSize - 1, 0, 3.0f,
                                        Color{255, 190, 60, 255});                    }
                    // allies of the defender next to the attacker = defence support
                    for (int d = 0; d < 6; ++d) {
                        int nx, ny;
                        hexNeighbor(A.x, A.y, d, nx, ny);
                        if (!inBounds(gMission, nx, ny)) continue;
                        int s = unitAt(gMission, nx, ny);
                        if (s < 0 || s == hoverSlot || s == selected) continue;
                        if (gMission.units[s].owner != D.owner) continue;
                        float hx, hy;
                        hexCenter(nx, ny, hx, hy);
                        DrawPolyLinesEx({hx, hy}, 6, gHexSize - 1, 0, 3.0f,
                                        Color{235, 70, 70, 255});
                    }
                }
            }
        }
        if (inspect >= 0) {
            if (inspect >= (int)gMission.units.size() ||
                gMission.units[inspect].hp <= 0 || gMission.units[inspect].carried) {
                inspect = -1;
            } else {
                const Unit& u = gMission.units[inspect];
                float cx, cy;
                hexCenter(u.x, u.y, cx, cy);
                DrawPolyLinesEx({cx, cy}, 6, gHexSize, 0, 2.5f, SKYBLUE);
            }
        }

        // ----- palette / frame: clean navy with a slate-blue rule, matching
        // the UI mock.  The map viewport keeps the original game art.
        const Color kInk{8, 16, 26, 255};
        const Color kFrame{75, 100, 122, 255};
        const Color kText{185, 193, 202, 255};
        const Color kLabel{160, 171, 182, 255};
        const Color kWhite{245, 248, 252, 255};
        const Color kTitle{245, 221, 158, 255};
        const Color kGold{255, 232, 140, 255};
        const Color kCyan{64, 255, 255, 255};
        const Color kGreen{110, 246, 92, 255};
        const Color kStatus{90, 210, 110, 255};
        const Color kBlue{61, 172, 252, 255};
        const Color kDim{130, 143, 156, 255};

        auto textW = [&](const char* t, int size) -> float {
            return gUiFontReady
                       ? MeasureTextEx(gUiFont, t, uiFontPixels(size), uiSpacing(size)).x
                       : (float)MeasureText(t, size * 2);
        };
        auto rightText = [&](const char* t, int size, float rx, float ry, Color c) {
            uiText(t, (int)(rx - textW(t, size)), (int)ry, size, c);
        };

        if (inMap) {
            float hcx, hcy;
            hexCenter(mx, my, hcx, hcy);
            DrawPolyLinesEx({hcx, hcy}, 6, gHexSize - 1.0f, 0, 1.5f,
                            Fade(kGold, 0.55f));
        }
        EndScissorMode();

        // Window and map-area rules.
        DrawRectangleLinesEx({1, 1, (float)scrW - 2, (float)scrH - 2}, 2, kFrame);
        DrawRectangleLinesEx({mapFrame.x, mapFrame.y,
                              mapFrame.width, mapFrame.height}, 2, kFrame);

        // ----- bottom terrain strip inside the map frame
        Rectangle strip{mapView.x, mapView.y + mapView.height,
                        mapView.width, 34.0f};
        DrawRectangle((int)strip.x - 2, (int)strip.y, (int)strip.width + 4, 2, kFrame);
        int terr = inMap ? terrainAt(gMission, mx, my) : -1;
        if (terr >= 0) {
            Rectangle sw{strip.x + 10, strip.y + 9, 16, 16};
            DrawRectangleRec(sw, kTerrainCols[terr]);
            DrawRectangleLinesEx(sw, 1, kFrame);
            uiText(gTerrainNames[terr].c_str(), (int)strip.x + 34,
                   (int)strip.y + 9, 10, kText);
            rightText(TextFormat("DEF %+d%%", gTerrain[terr].def), 10,
                      strip.x + strip.width - 12, strip.y + 9, kTitle);
            rightText(TextFormat("%d,%d", mx, my), 10,
                      strip.x + strip.width - 108, strip.y + 9, kDim);
        } else {
            uiText("MOVE CURSOR OVER MAP", (int)strip.x + 14,
                   (int)strip.y + 9, 10, kText);
        }

        // ----- sidebar
        const int panelX = mapAreaW;
        const int sideWpx = scrW - mapAreaW;
        const int px = panelX + 26;
        const int innerW = sideWpx - 52;

        int alive0 = 0, alive1 = 0;
        for (auto& u : gMission.units) {
            if (u.hp > 0 && !u.carried) (u.owner == 0 ? alive0 : alive1)++;
        }

        int y = 16;
        float titleW = textW("NECTARIS", 20);
        uiText("NECTARIS", (int)(px + (innerW - titleW) * 0.5f), y, 20, kTitle);
        y += 62;
        DrawRectangle(px, y, innerW, 2, kFrame);
        y += 14;

        // Mission / campaign.
        std::string mname = gMission.name;
        for (char& c : mname) {
            c = (char)std::toupper((unsigned char)c);
            if (c == '_') c = ' ';
        }
        uiText(mname.c_str(), px, y, 15, kText);
        if (!campaign.empty()) {
            const char* num = TextFormat("%02d / %02d", campaignIdx + 1,
                                         (int)campaign.size());
            // Keep the large mock-style number when it fits; long test-map
            // names fall back to the small size instead of overlapping.
            bool fits = textW(mname.c_str(), 15) + textW(num, 15) + 20.0f <= innerW;
            rightText(num, fits ? 15 : 10, (float)(px + innerW),
                      (float)(y + (fits ? 0 : 10)), kText);
        }
        y += 42;

        uiText(currentPlayer == 0 ? "PLAYER ACTIVE" : "ENEMY ACTIVE",
               px, y, 15, currentPlayer == 0 ? kCyan : kGreen);
        y += 44;

        // Forces.
        auto forceRow = [&](int ry, Color mark, const char* label, int count) {
            DrawRectangle(px, ry + 8, 14, 14, mark);
            uiText(label, px + 26, ry, 15, kText);
            rightText(TextFormat("%d", count), 15,
                      (float)(px + innerW), (float)ry, kWhite);
        };
        forceRow(y, kBlue, "PLAYER", alive0);
        y += 34;
        forceRow(y, kGreen, "ENEMY", alive1);
        y += 40;
        DrawRectangle(px, y, innerW, 2, kFrame);
        y += 18;

        // Unit card.
        int infoSlot = (selected >= 0) ? selected : inspect;
        bool haveInfo = infoSlot >= 0 && infoSlot < (int)gMission.units.size() &&
                        gMission.units[infoSlot].hp > 0 &&
                        !gMission.units[infoSlot].carried;
        if (haveInfo) {
            const Unit& u = gMission.units[infoSlot];
            const UnitType& t = gTypes[u.type];
            bool mine = (u.owner == currentPlayer);
            Color accent = (u.owner == 0) ? kBlue : kGreen;

            DrawRectangle(px, y, 66, 66, kInk);
            DrawRectangleLinesEx({(float)px, (float)y, 66, 66}, 2, kFrame);
            Texture2D& icon = gUnitMapTex[u.type];
            if (icon.id) {
                DrawTexturePro(icon, {0, 0, (float)icon.width, (float)icon.height},
                               {(float)px + 2, (float)y + 2, 62, 62}, {0, 0}, 0,
                               u.owner == 0 ? WHITE : Color{150, 255, 150, 255});
            }
            uiText(t.name.c_str(), px + 82, y - 2, 15, kGold);
            uiText(TextFormat("%s / %s", u.owner == 0 ? "ALLIED" : "ENEMY",
                              u.acted ? "ACTED" : "READY"),
                   px + 82, y + 34, 10, u.acted ? kDim : kStatus);
            for (int i = 0; i < u.hp; ++i)
                DrawRectangle(px + 82 + i * 10, y + 58, 8, 7, accent);
            rightText(TextFormat("%d / 7", u.hp), 10,
                      (float)(px + innerW), (float)(y + 58), kWhite);

            int sy = y + 84;
            int half = innerW / 2;
            bool wideStats = innerW >= 420;
            // The mock's property block is set in the same face as the rest of
            // the UI; here it is doubled (32 px) with compact values so both
            // columns stay aligned inside the card.
            auto stat = [&](int sx, int ry, const char* label, const char* value) {
                uiText(label, sx, ry, 15, kLabel);
                rightText(value, 15, (float)(sx + half - 22), (float)ry, kWhite);
            };
            int c1 = px + 4, c2 = px + half + 6;
            stat(c1, sy, "ATK", TextFormat("%d/%d", t.atkGround, t.atkAir));
            stat(c2, sy, "CTR", TextFormat("%d/%d", t.ctrGround, t.ctrAir));
            stat(c1, sy + 36, "DEF", TextFormat("%d", t.def));
            stat(c2, sy + 36, "MOVE", TextFormat("%d", t.movePts));
            stat(c1, sy + 72, "RANK", TextFormat("%d", u.rank));
            stat(c2, sy + 72, "XP", TextFormat("%d", u.xp));
            int terrU = t.airborne()
                            ? 0
                            : gTerrain[terrainAt(gMission, u.x, u.y)].def;
            stat(c1, sy + 108, wideStats ? "TERRAIN" : "TERR",
                 TextFormat("%+d%%", terrU));
            stat(c2, sy + 108, "TYPE",
                 t.airborne() ? "AIR" : (wideStats ? "GROUND" : "GND"));

            const char* hint1;
            const char* hint2;
            if (!mine) {
                hint1 = "INFO ONLY - SELECT YOUR UNIT";
                hint2 = "ENEMY ORDERS ARE NOT AVAILABLE";
            } else if (u.cargo >= 0) {
                const Unit& c = gMission.units[u.cargo];
                hint1 = TextFormat("CARGO %s  HP %d",
                                   gTypes[c.type].name.c_str(), c.hp);
                hint2 = "U  CHOOSE UNLOAD HEX";
            } else if (isTransport(t)) {
                hint1 = "EMPTY TRANSPORT";
                hint2 = "SELECT A UNIT, THEN THIS TRANSPORT";
            } else {
                hint1 = "LMB  MOVE / ATTACK";
                hint2 = "CLICK A TRANSPORT TO LOAD";
            }
            uiText(hint1, px + 4, y + 228, 10, kText);
            uiText(hint2, px + 4, y + 244, 10, kDim);
        } else {
            DrawRectangle(px, y, 66, 66, kInk);
            DrawRectangleLinesEx({(float)px, (float)y, 66, 66}, 2, kFrame);
            float dashW = textW("-", 15);
            uiText("-", (int)(px + (66 - dashW) * 0.5f), y + 16, 15, kDim);
            uiText("NO UNIT SELECTED", px + 82, y + 8, 15, kText);
            uiText(gMission.factories.empty() ? "CLICK A UNIT TO INSPECT IT"
                                              : "CLICK A UNIT OR A FACTORY",
                   px + 82, y + 46, 10, kDim);
            uiText("SELECT A UNIT FOR ORDERS AND STATS", px + 4, y + 86, 10, kDim);
        }
        // Controls.
        int fy = scrH - 160;
        DrawRectangle(px, fy - 18, innerW, 2, kFrame);
        auto keyRow = [&](int ky, const char* k, const char* label) {
            rightText(k, 15, (float)(px + 78), (float)ky, kGold);
            uiText(label, px + 94, ky + 9, 10, kText);
        };
        keyRow(fy, "SPACE", "END TURN / DESELECT");
        keyRow(fy + 36, "R", "RESTART MISSION");
        keyRow(fy + 72, "N / B", "NEXT / PREVIOUS MISSION");
        keyRow(fy + 108, "LMB", "SELECT   DRAG   PAN");

        if (noticeT > 0)
            uiText(notice.c_str(), px, scrH - 18, 10, kTitle);
        if (debug)
            uiText(TextFormat("cell %d,%d  terrain %d", mx, my,
                              inMap ? terrainAt(gMission, mx, my) : -1),
                   (int)mapView.x + 8, (int)mapView.y + 6, 10, kDim);

        // End-of-mission banner over the map.
        if (alive0 == 0 || alive1 == 0) {
            const char* msg = (alive1 == 0) ? "VICTORY" : "DEFEAT";
            Color mc = (alive1 == 0) ? kGreen : kTitle;
            float tw = textW(msg, 26);
            float bx = mapView.x + (mapView.width - tw) * 0.5f;
            float by = mapView.y + mapView.height * 0.5f - 30.0f;
            DrawRectangle((int)mapView.x, (int)(by - 18),
                          (int)mapView.width, 84, Fade(kInk, 0.84f));
            DrawRectangle((int)mapView.x, (int)(by - 19),
                          (int)mapView.width, 1, kFrame);
            DrawRectangle((int)mapView.x, (int)(by + 65),
                          (int)mapView.width, 1, kFrame);
            uiText(msg, (int)bx, (int)by, 26, mc);
            const char* sub = (alive1 == 0) ? "MISSION COMPLETE" : "ALL UNITS LOST";
            float sw2 = textW(sub, 10);
            uiText(sub, (int)(mapView.x + (mapView.width - sw2) * 0.5f),
                   (int)(by + 44), 10, kText);
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
    if (gImpostorAtlas.id) UnloadTexture(gImpostorAtlas);
    for (int i = 0; i < 64; ++i) if (gVfxHalf[i].id) UnloadTexture(gVfxHalf[i]);
    for (int i = 0; i < 16; ++i) if (gVfxQuad[i].id) UnloadTexture(gVfxQuad[i]);
    if (gVfxSplat.id) UnloadTexture(gVfxSplat);
    if (gUiFontReady) UnloadFont(gUiFont);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}




