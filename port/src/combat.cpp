// Battle simulation, ported line-for-line from the original executable.
// See combat.h and RE_FINDINGS.md section 5 for the address map.
#include "combat.h"

#include <cstdint>

// ------------------------------------------------------------------ RNG
// FUN_0040cb50: 16-bit LFSR over DAT_00495740/41.  DAT_00495742 is always 0.
uint8_t CombatRng::step() {
    uint16_t t = (uint16_t)(((uint16_t)((b_ ^ a_) << 8)) | a_);
    t = (uint16_t)(t * 2);
    uint8_t na = (uint8_t)(t & 0xFF) | (uint8_t)((((b_ >> 6) ^ a_)) & 1);
    uint8_t nb = (uint8_t)(t >> 8);
    a_ = na;
    b_ = nb;
    return (uint8_t)(a_ ^ b_);
}

// FUN_0040c770: (rand_byte * 100) >> 8.
int CombatRng::roll100() {
    return (int)(((unsigned)step() * 100u) >> 8);
}

// FUN_00421170: roll -> damage multiplier / 10.
int CombatRng::variance() {
    int r = roll100();
    if (r > 0x13 && r < 0x19) return 0x0F;   // 20..24  -> 1.5x
    if (r > 0x3B && r < 0x41) return 0x14;   // 60..64  -> 2.0x
    if (r > 0x4F && r < 0x53) return 0x02;   // 80..82  -> 0.2x
    if (r < 0x62) return (0x96 - r) / 10;    // 0..97   -> 1.5x .. 0.5x
    return 0x28;                             // 98..99  -> 4.0x
}

// ------------------------------------------------------------------ helpers
namespace {

int rankOf(const Unit& u) {
    return std::clamp(u.rank, 0, 7);
}

int hpMultAt(const std::vector<int>& hpMult, int rank) {
    if (hpMult.empty()) {
        static const int kDefault[8] = {100, 105, 110, 120, 130, 140, 160, 200};
        return kDefault[std::clamp(rank, 0, 7)];
    }
    return hpMult[std::clamp(rank, 0, (int)hpMult.size() - 1)];
}

// Original FUN_0041bc10: pick the ground/air attack column from the
// defender's airborne flag (DAT_0042b288[type] & 0x40).
int attackValue(const Mission& m, const std::vector<UnitType>& types,
                int attSlot, int defSlot) {
    const UnitType& at = types[m.units[attSlot].type];
    const UnitType& de = types[m.units[defSlot].type];
    return de.airborne() ? at.atkAir : at.atkGround;
}

// Original ZOC test (FUN_00423a80 + FUN_0041bd40): a cell is flagged 0x40
// when it is adjacent to an idle unit owned by the current player.  The
// defender counts as surrounded only when all six neighbours carry 0x40.
bool inAttackerZoc(const Mission& m, int owner, int cx, int cy, int atkSlot) {
    for (int i = 0; i < (int)m.units.size(); ++i) {
        const Unit& u = m.units[i];
        if (u.hp <= 0 || u.owner != owner || u.acted) continue;
        if (hexDistance(u.x, u.y, cx, cy) == 1) return true;
    }
    (void)atkSlot;
    return false;
}

}  // namespace

// ------------------------------------------------------------------ resolve
CombatResult resolveCombat(const Mission& m,
                           const std::vector<UnitType>& types,
                           const std::vector<TerrainDef>& terrain,
                           const std::vector<int>& hpMult,
                           int atkSlot, int defSlot, int mode,
                           CombatRng& rng) {
    CombatResult out;
    if (atkSlot < 0 || defSlot < 0) return out;
    if (atkSlot >= (int)m.units.size() || defSlot >= (int)m.units.size()) return out;

    const Unit& A = m.units[atkSlot];
    const Unit& D = m.units[defSlot];
    if (A.type < 0 || A.type >= (int)types.size()) return out;
    if (D.type < 0 || D.type >= (int)types.size()) return out;
    const UnitType& TA = types[A.type];
    const UnitType& TD = types[D.type];

    out.atkSlot = atkSlot;
    out.defSlot = defSlot;
    out.hpA = A.hp;
    out.hpD = D.hp;
    out.hA = A.hp + 1;          // DAT_00481e5f
    out.hD = D.hp + 1;          // DAT_00481e5e
    out.distance = hexDistance(A.x, A.y, D.x, D.y);
    out.ranged = (mode != 0);

    // FUN_0041b810 lines 16853..16864: defence-side counter range.
    const int canA = TD.airborne() ? TA.ctrAir : TA.ctrGround;
    const int canD = TA.airborne() ? TD.ctrAir : TD.ctrGround;

    // Direct fire exchanges; indirect fire only lets the attacker shoot and
    // never provokes a counterattack.
    if (mode == 0) {
        if (canA < 2) out.baseAtk = attackValue(m, types, atkSlot, defSlot);
        if (canD == 1) out.baseCtr = attackValue(m, types, defSlot, atkSlot);
    } else {
        if (canA >= 2) out.baseAtk = attackValue(m, types, atkSlot, defSlot);
        out.baseCtr = 0;
    }

    // FUN_0041bc50 / FUN_0041bc70: unit defence and terrain defence.
    // Air units (ability bit 0x40) ignore terrain entirely.
    const int defA = TA.def;
    const int defD = TD.def;
    const int terA = TA.airborne() ? 0 : terrain[terrainAt(m, A.x, A.y)].def;
    const int terD = TD.airborne() ? 0 : terrain[terrainAt(m, D.x, D.y)].def;
    out.terrainDefA = terA;
    out.terrainDefD = terD;

    // ----- support fire (only during a direct exchange)
    int supA = 0, supD = 0;
    if (mode == 0) {
        // friendly units adjacent to the defender add attack power
        for (int d = 0; d < 6; ++d) {
            int nx, ny;
            hexNeighbor(D.x, D.y, d, nx, ny);
            if (!inBounds(m, nx, ny)) continue;
            int s = unitAt(m, nx, ny);
            if (s < 0 || s == atkSlot) continue;
            const Unit& N = m.units[s];
            if (N.owner != A.owner || N.acted) continue;
            supA += attackValue(m, types, s, defSlot) * (N.hp + 1);
        }
        supA /= (out.hA * 2);

        // enemy units adjacent to the attacker add defensive support
        for (int d = 0; d < 6; ++d) {
            int nx, ny;
            hexNeighbor(A.x, A.y, d, nx, ny);
            if (!inBounds(m, nx, ny)) continue;
            int s = unitAt(m, nx, ny);
            if (s < 0 || s == defSlot) continue;
            const Unit& N = m.units[s];
            if (N.owner != D.owner || N.acted) continue;
            supD += types[N.type].def * (N.hp + 1);
        }
        // Note: the original divides *both* support sums by (attacker HP+1)*2.
        supD /= (out.hA * 2);
    }
    out.supportAtk = supA;
    out.supportDef = supD;

    // ----- surround check (FUN_0041b810 lines 16934..16953)
    bool surrounded = true;
    for (int d = 0; d < 6; ++d) {
        int nx, ny;
        hexNeighbor(D.x, D.y, d, nx, ny);
        if (!inBounds(m, nx, ny) ||
            !inAttackerZoc(m, A.owner, nx, ny, atkSlot)) {
            surrounded = false;
            break;
        }
    }
    out.surrounded = surrounded;

    // ----- attack / defence strengths
    int atkStrength = out.baseAtk + supA;
    int defStrength = defD + terD + supD;
    if (surrounded) defStrength /= 2;
    if (atkStrength > 100) atkStrength = 100;
    if (defStrength > 100) defStrength = 100;
    out.atkStrength = atkStrength;
    out.defStrength = defStrength;

    // ----- damage
    out.rankA = rankOf(A);
    const int multA = hpMultAt(hpMult, out.rankA);
    out.rawDmgToDef =
        out.hA * (multA * (((100 - defStrength) * atkStrength) / 100) / 100);

    if (mode == 0) {
        out.rankD = rankOf(D);
        int ctrBase = out.baseCtr;
        if (surrounded) ctrBase /= 2;      // the surround penalty also halves counter fire
        int ctrDef = defA + terA;
        if (ctrBase > 100) ctrBase = 100;
        if (ctrDef > 100) ctrDef = 100;
        out.ctrStrength = ctrBase;
        out.ctrDefense = ctrDef;
        const int multD = hpMultAt(hpMult, out.rankD);
        const int ctrVal = multD * (((100 - ctrDef) * ctrBase) / 100) / 100;
        out.rawDmgToAtk = out.hD * ctrVal;
    } else {
        out.rankD = rankOf(D);
        out.ctrStrength = 0;
        int ctrDef = defA + terA;
        if (ctrDef > 100) ctrDef = 100;
        out.ctrDefense = ctrDef;
        out.rawDmgToAtk = 0;
    }

    // ----- FUN_00420cd0 variance: defender damage first, then counter damage
    out.varToDef = rng.variance();
    out.dmgToDef = out.varToDef * out.rawDmgToDef / 10;
    out.varToAtk = rng.variance();
    out.dmgToAtk = out.varToAtk * out.rawDmgToAtk / 10;

    // ----- new HP: original keeps (hp + 1) and treats > 8 as destroyed
    auto newValue = [](int hp, int dmg) {
        int bonus = (hp >= 1) ? 50 : 0;    // DAT_00481e5f < 2 -> 0
        int v = (bonus + (hp + 1) * 100 - dmg) / 100;
        if (v > 8) v = 0;
        return v;                          // 0 = dead, else new HP + 1
    };
    const int vD = newValue(D.hp, out.dmgToDef);
    const int vA = newValue(A.hp, out.dmgToAtk);
    out.newHpDef = (vD > 0) ? (vD - 1) : 0;
    out.newHpAtk = (vA > 0) ? (vA - 1) : 0;
    out.defDied = (out.newHpDef == 0);
    out.atkDied = (out.newHpAtk == 0);

    // ----- XP (FUN_00415850 / FUN_0040a460)
    if (out.newHpAtk != 0 && out.newHpDef != D.hp)
        out.xpToAtk = (out.newHpDef == 0) ? 8 : 4;
    if (out.newHpDef != 0)
        out.xpToDef = (out.newHpDef == D.hp) ? 8 : 4;

    return out;
}

// ------------------------------------------------------------------ apply
namespace {
void addXp(Unit& u, int amount) {
    if (amount <= 0) return;
    u.xp = std::min(31, u.xp + amount);   // FUN_0040a460 caps at 0x1F
    u.rank = rankFromXp(u.xp);            // bits 5..7 = xp / 4
}
}  // namespace

void applyCombatResult(Mission& m, const CombatResult& r) {
    if (r.atkSlot < 0 || r.defSlot < 0) return;
    if (r.atkSlot >= (int)m.units.size() || r.defSlot >= (int)m.units.size()) return;
    Unit& A = m.units[r.atkSlot];
    Unit& D = m.units[r.defSlot];
    A.hp = r.newHpAtk;
    D.hp = r.newHpDef;
    addXp(A, r.xpToAtk);
    addXp(D, r.xpToDef);
}

// ------------------------------------------------------------------ targeting
bool canAttackTarget(const Mission& m, const std::vector<UnitType>& types,
                     int atkSlot, int defSlot, int* distance) {
    if (atkSlot < 0 || defSlot < 0 || atkSlot == defSlot) return false;
    if (atkSlot >= (int)m.units.size() || defSlot >= (int)m.units.size()) return false;
    const Unit& A = m.units[atkSlot];
    const Unit& D = m.units[defSlot];
    if (A.hp <= 0 || D.hp <= 0 || A.owner == D.owner) return false;
    if (A.type < 0 || A.type >= (int)types.size()) return false;
    if (D.type < 0 || D.type >= (int)types.size()) return false;
    const UnitType& TA = types[A.type];
    const UnitType& TD = types[D.type];

    const bool air = TD.airborne();
    const int atk = air ? TA.atkAir : TA.atkGround;
    const int range = air ? TA.ctrAir : TA.ctrGround;
    if (atk == 0 || range < 1) return false;

    const int d = hexDistance(A.x, A.y, D.x, D.y);
    if (distance) *distance = d;

    // Direct-fire units only hit adjacent hexes.  Units with counter/range
    // >= 2 are the artillery class and hit rings 2..range (never adjacent).
    // The original gates the indirect rings on DAT_00495c42 or ability 0x14;
    // that flag is a mission switch we do not export, so the port grants the
    // range to every counter/range >= 2 unit.
    if (range == 1) return d == 1;
    return d >= 2 && d <= range;
}

std::vector<int> attackableTargets(const Mission& m,
                                   const std::vector<UnitType>& types,
                                   int atkSlot) {
    std::vector<int> out;
    for (int i = 0; i < (int)m.units.size(); ++i) {
        int d = 0;
        if (canAttackTarget(m, types, atkSlot, i, &d)) out.push_back(i);
    }
    return out;
}
