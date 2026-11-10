// Battle simulation for the port, reverse-engineered from the original
// Nectaris executable.
//
//   FUN_0041b810  core attack/defence/support/surround maths
//   FUN_00420cd0  variance rolls + new HP
//   FUN_00421170  roll -> damage multiplier table
//   FUN_0040c770 / FUN_0040cb50  original 16-bit LFSR
//   FUN_00415850 / FUN_0040a460  XP award and rank tiers
//
// The module is pure: resolveCombat() computes a CombatResult and does not
// touch the mission.  applyCombatResult() commits HP + XP.
#pragma once

#include "model.h"

#include <algorithm>
#include <cstdint>
#include <vector>

// Original RNG (FUN_0040cb50 / FUN_0040c770).  Dat_495740/41 hold the two
// state bytes; DAT_00495742 is always zero in the shipped game.
struct CombatRules;
class CombatRng {
public:
    void seed(uint32_t s) {
        a_ = (uint8_t)(s & 0xFF);
        b_ = (uint8_t)((s >> 8) & 0xFF);
        if (a_ == 0 && b_ == 0) a_ = 1;  // the LFSR would be stuck
    }
    int roll100();   // FUN_0040c770: (byte * 100) >> 8 -> 0..99
    int variance(const CombatRules& rules);  // FUN_00421170

private:
    uint8_t step();  // FUN_0040cb50
    uint8_t a_ = 1, b_ = 0;
};

// Everything the combat screen and logs need, in one place.
struct VarianceBand {
    int lo = 0, hi = 0, mult = 10;   // inclusive roll range -> multiplier / 10
};

// Combat constants extracted from the executable.  Editable via
// assets/data/combat.json so the simulation can be tuned without a rebuild.
struct CombatRules {
    std::vector<int> hpMult{100, 105, 110, 120, 130, 140, 160, 200};  // 0x42b158
    std::vector<VarianceBand> variance{
        {20, 24, 15}, {60, 64, 20}, {80, 82, 2}, {98, 99, 40}};
    int varianceBase = 150;      // (varianceBase - roll) / varianceDiv
    int varianceDiv = 10;
    int supportDivisor = 2;      // sum / ((hpA + 1) * divisor)
    int surroundDivisor = 2;     // defence / divisor when encircled
    int hpBonus = 50;            // added before dividing by 100
    int destroyThreshold = 8;    // newValue > 8 -> destroyed
    int xpHit = 4;
    int xpKill = 8;
    int xpNoDamage = 8;
    int maxXp = 31;
    int rankDivisor = 4;         // rank = xp / rankDivisor
};

struct CombatResult {
    // core inputs / intermediate strengths
    int atkSlot = 0, defSlot = 0;
    int distance = 1;
    bool ranged = false;          // mode != 0 (indirect)
    bool surrounded = false;      // defender surrounded by attacker ZOC
    int surroundPct = 0;          // 0 or 50 (display, defender)
    bool surroundedA = false;     // attacker surrounded by defender ZOC
    int surroundPctA = 0;         // 0 or 50 (display, attacker)
    bool surroundedD = false;     // same as `surrounded` (explicit side)
    int surroundPctD = 0;

    int hpA = 0, hpD = 0;         // HP before the fight
    int hA = 0, hD = 0;           // original `hp + 1` working values

    int baseAtk = 0;              // FUN_0041bc10(attacker, defender)
    int baseCtr = 0;              // counter base (0 when the defender cannot)
    int supportAtk = 0;           // attacker support fire
    int supportDef = 0;           // defender support
    int unitDefA = 0, unitDefD = 0;   // raw unit defence
    int terrainDefA = 0;          // effective terrain defence % (0 for air)
    int terrainDefD = 0;
    int defBeforeSurround = 0;    // defD + terD + supD
    int defAfterSurround = 0;     // after halving, before the 100 clamp
    int expPctA = 0, expPctD = 0; // experience bonus in percent
    int atkStrength = 0;          // clamped attack %
    int defStrength = 0;          // clamped defence %
    int ctrStrength = 0;          // clamped counter attack %
    int ctrDefense = 0;           // `defence[A] + terrain[A]` clamped

    int rankA = 0, rankD = 0;     // HP-multiplier tiers (0..7)

    // damage
    int rawDmgToDef = 0, rawDmgToAtk = 0;  // before variance
    int varToDef = 0, varToAtk = 0;        // multiplier / 10
    int dmgToDef = 0, dmgToAtk = 0;        // after variance
    int newHpDef = 0, newHpAtk = 0;        // 0..7, 0 = destroyed
    bool defDied = false, atkDied = false;

    // original XP awards (FUN_00415850): 4 for a hit, 8 for a kill /
    // undamaged defence
    int xpToAtk = 0, xpToDef = 0;
};

// FUN_0041b810 + FUN_00420cd0.  Pure; `mode` is hexDistance - 1.
CombatResult resolveCombat(const Mission& m,
                           const std::vector<UnitType>& types,
                           const std::vector<TerrainDef>& terrain,
                           const CombatRules& rules,
                           int atkSlot, int defSlot, int mode,
                           CombatRng& rng);

// Commit HP and XP.  Mirrors FUN_00420ff0 + FUN_00415850 + FUN_0040a460.
void applyCombatResult(Mission& m, const CombatResult& r, const CombatRules& rules);

// Target selection: adjacent for direct units, rings 2..range for indirect.
bool canAttackTarget(const Mission& m, const std::vector<UnitType>& types,
                     int atkSlot, int defSlot, int* distance = nullptr);
std::vector<int> attackableTargets(const Mission& m,
                                   const std::vector<UnitType>& types,
                                   int atkSlot);

// Original XP -> rank tier: tiers live in bits 5..7, i.e. xp / 4, max 7.
inline int rankFromXp(int xp, int divisor = 4) {
    return (divisor <= 0) ? 0 : std::clamp(xp, 0, 31) / divisor;
}
