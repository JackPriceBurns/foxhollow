#include "test_support.h"
#include "../game/src/dlls/objects/590/590.c"

static GameObject* attacker;
static int hitting = 1;

int timerCountDown(f32* timer) {
    return 0;
}

int ObjHits_GetPriorityHitWithPosition(GameObject* obj, GameObject** hitObject, int* sphere, u32* damage,
                                     f32* x, f32* y, f32* z) {
    *hitObject = attacker;
    *damage = 1;
    *x = 0;
    *y = 0;
    *z = 0;
    return hitting;
}

int arrayIndexOf(int* array, int count, int value) {
    FH_CHECK(count == 2 && value == 1);
    return 0;
}

void Obj_SpawnHitLightAndFade(GameObject* obj, const Vec3f* pos, f32 scale) {
}

void Sfx_PlayFromObject(GameObject* obj, u16 sfxId) {
}

void spawnExplosion(GameObject* obj, f32 scale, u8 kind, u8 flag4, u8 flag8, u8 flag10, u8 doShake, u8 flag20,
                    u8 initialFlags) {
    FH_UNEXPECTED_CALL();
}

int Obj_UpdateLightningCluster(GameObject* obj, LightningEffect** entries, int count, f32 intensity,
                               ModelLightStruct** light) {
    FH_UNEXPECTED_CALL();
    return 0;
}

void s16toFloat(f32* timer, s16 duration) {
    FH_UNEXPECTED_CALL();
}

void ObjHits_DisableObject(GameObject* obj) {
    FH_UNEXPECTED_CALL();
}

void Obj_FreeObject(GameObject* obj) {
    FH_UNEXPECTED_CALL();
}

void Obj_RemoveFromUpdateList(GameObject* obj) {
    FH_UNEXPECTED_CALL();
}

int main(void) {
    // Given a thornbush with ten health and a colliding attacker allocated above 4 GB.
    GameObject obj = {0};
    DrakordThornbushState* state = fh_test_alloc_high(1, drakord_thornbush_getExtraSize());
    attacker = fh_test_alloc_high(1, sizeof(*attacker));
    obj.extra = state;
    state->health = 10;

    // When the same attacker remains in contact across two collision updates.
    drakord_thornbush_hitDetect(&obj);
    drakord_thornbush_hitDetect(&obj);

    // Then only the first contact damages the thornbush.
    FH_CHECK(state->health == 9);

    // When contact ends and the same attacker hits again.
    hitting = 0;
    drakord_thornbush_hitDetect(&obj);
    hitting = 1;
    drakord_thornbush_hitDetect(&obj);

    // Then the new contact causes one additional point of damage.
    FH_CHECK(state->health == 8);
    free(state);
    free(attacker);
    return EXIT_SUCCESS;
}
