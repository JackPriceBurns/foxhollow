#include "main/dll/baddie_frozen.h"
#include "main/dll/wispbaddie_baddie.h"

typedef enum BattleDroidStateFlag {
    BATTLE_DROID_STATE_FLAG_DEFAULT = 1 << 0,
    BATTLE_DROID_STATE_FLAG_HIT = 1 << 3,
    BATTLE_DROID_STATE_FLAG_ACTIVE = 1 << 7,
    BATTLE_DROID_STATE_FLAG_HIT_10 = 1 << 5,
} BattleDroidStateFlag;

void battleDroidUpdateWhileFrozen(GameObject* obj, EnemyState* state, GameObject* attacker, int hit, int hitArg,
                                  int hitCount, Vec* hitPos, int sector) {
    (void)obj;
    (void)attacker;
    (void)hitArg;
    (void)hitCount;
    (void)hitPos;
    (void)sector;

    if (hit == 0x10) {
        state->flags2E8 |= BATTLE_DROID_STATE_FLAG_HIT_10;
    } else {
        state->flags2E8 |= BATTLE_DROID_STATE_FLAG_HIT;
    }
}

void battleDroidUpdate(GameObject* obj, EnemyState* state) {
    (void)obj;
    (void)state;
}

void battleDroidUpdateAttack(GameObject* obj, EnemyState* state) {
    GameObject* target = state->trackedObj;

    baddieTurnTowardPoint(obj, state, target->anim.localPosX, target->anim.localPosZ, 0xF, 0);
}

void battleDroidInit(GameObject* obj, EnemyState* state) {
    (void)obj;

    state->sightRange = 60.0f;
    state->flags2E4 = BATTLE_DROID_STATE_FLAG_DEFAULT | BATTLE_DROID_STATE_FLAG_ACTIVE;
    state->animPlaySpeed = 0.005f;
    state->gravity = 0.17f;
    state->drag = 0.97f;
    state->moveId0 = 0;
    state->moveSpeedScale0 = 3.0f;
    state->moveId1 = 0;
    state->moveSpeedScale1 = 1.25f;
    state->moveId2 = 0;
    state->moveSpeedScale2 = 3.0f;
}
