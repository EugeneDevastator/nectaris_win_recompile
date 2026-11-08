// Shared data model + hex helpers used by the port and the combat module.
// Kept header-only so both translation units see the same definitions.
#pragma once

#include <cstdlib>
#include <string>
#include <vector>

// ------------------------------------------------------------------ data
struct UnitType {
    int id = 0;
    std::string name;
    int atkGround = 0, atkAir = 0, def = 0;
    int ctrGround = 1, ctrAir = 1;
    int movePts = 0, moveClass = 0, ability = 0, maxHp = 7;

    // Original `DAT_0042b288[type] & 0x40`: unit is airborne (ignores terrain).
    bool airborne() const { return (ability & 0x40) != 0; }
    // Original `DAT_0042b288[type] & 0x08`: indirect-capable unit.
    bool indirectCapable() const { return (ability & 0x14) == 0x14; }
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
    int rank = 0;           // 0..7 damage-multiplier tier (XP / 4)
    bool acted = false;
    float px = -1, py = -1; // smoothed pixel position
    float facing = 90;      // map heading: atan2(dy,dx) - 90 (0 = down, 90 = left)
    int xp = 0;             // 0..31 (original: bits 3..7 of the HP byte)

    // Transport state.  A transport stores one cargo formation by slot index;
    // the cargo unit stays in Mission::units but is hidden and inactive.
    int cargo = -1;
    bool carried = false;
};

struct Factory {
    int group = 0;              // 1-based factory/group id in the mission table
    int x = 0, y = 0;
    int terrain = 0;            // 11 player, 12 neutral, 13 enemy
    int owner = -1;             // 0 player, 1 enemy, -1 neutral
    int capacity = 12;
    std::vector<int> roster;    // unit type ids stored in the factory
};

struct Mission {
    std::string name;
    int number = 0;             // campaign mission number (1-based, 0 = custom)
    int w = 0, h = 0;
    std::vector<int> terrain;   // w*h, 0..15
    std::vector<Unit> units;
    std::vector<Unit> garrison; // inside factories/bases (not on the field)
    std::vector<Factory> factories;
    std::string background;
};

// ------------------------------------------------------------------ hex
// Odd-column offset hex (matches FUN_00403490).
inline const int kHexDX[6] = {0, 1, 1, 0, -1, -1};
inline const int kHexDYEven[6] = {-1, -1, 0, 1, 0, -1};
inline const int kHexDYOdd[6] = {-1, 0, 1, 1, 1, 0};

inline void hexNeighbor(int x, int y, int d, int& nx, int& ny) {
    nx = x + kHexDX[d];
    ny = y + ((x & 1) ? kHexDYOdd[d] : kHexDYEven[d]);
}

inline bool inBounds(const Mission& m, int x, int y) {
    return x >= 0 && y >= 0 && x < m.w && y < m.h;
}

inline int terrainAt(const Mission& m, int x, int y) {
    if (!inBounds(m, x, y)) return 0;
    int t = m.terrain[y * m.w + x];
    return (t < 0 || t > 15) ? 0 : t;
}

inline int unitAt(const Mission& m, int x, int y) {
    for (int i = 0; i < (int)m.units.size(); ++i) {
        const Unit& u = m.units[i];
        if (u.hp > 0 && !u.carried && u.x == x && u.y == y) return i;
    }
    return -1;
}

// Odd-q offset -> cube distance.
inline int hexDistance(int x1, int y1, int x2, int y2) {
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
