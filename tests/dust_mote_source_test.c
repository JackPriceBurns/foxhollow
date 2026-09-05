#include "test_support.h"
#include "../game/src/dlls/objects/690_DustMoteSou/DustMoteSou.c"

static GameObject* source;
static int enabled;
static int bitReads;
static int spawns;
static int spawnKind;
static int expectedB;

u32 mainGetBit(int bit) {
    FH_CHECK(bit == 0x123);
    bitReads++;
    return enabled;
}

static void checkSpawn(GameObject* obj, f32 scale, int effect, int a, int b, void* origin, int kind) {
    FH_CHECK(obj == source && scale == 0.15625f);
    FH_CHECK(effect == 6 && a == 2 && b == expectedB && origin == NULL);
    spawnKind = kind;
    spawns++;
}

void objfx_spawnMaskedHitEffect(GameObject* obj, f32 scale, u8 effect, u8 a, u8 b, void* origin) {
    checkSpawn(obj, scale, effect, a, b, origin, 4);
}

void objfx_spawnHitEffectBurst(GameObject* obj, f32 scale, u8 effect, u8 a, u8 b, GameObject* origin) {
    checkSpawn(obj, scale, effect, a, b, origin, 5);
}

void objfx_spawnBoxBurst(GameObject* obj, u8 effect, f32 scale, u8 a, u8 b, u8 chance,
                         f32 x, f32 y, f32 z, void* origin, int flags) {
    checkSpawn(obj, scale, effect, a, b, origin, 1);
    FH_CHECK(chance == 37 && x == 7 && y == 8 && z == 9 && flags == 0);
}

void objfx_spawnArcedBurst(GameObject* obj, u8 effect, f32 scale, u8 a, u8 b, int chance,
                           f32 x, f32 y, f32 z, void* origin, int flags) {
    checkSpawn(obj, scale, effect, a, b, origin, 2);
    FH_CHECK(chance == 37 && x == 7 && y == 8 && z == 9 && flags == 0);
}

void objfx_spawnDirectionalBurst(GameObject* obj, u8 effect, f32 scale, u8 a, u8 b, u8 chance,
                                 f32 x, void* origin, int flags) {
    checkSpawn(obj, scale, effect, a, b, origin, 3);
    FH_CHECK(chance == 37 && x == 7 && flags == 0);
}

int main(void) {
    for (int native = 0; native <= 1; native++) {
        // Given equivalent file-backed and owned placements with a small effect scale and a game-bit gate.
        GameObject obj = {0};
        DustMoteSourcePlacement placement = {0};
        source = &obj;
        obj.anim.placementData = (s16*)&placement.base;
        if (native) {
            obj.anim.flags |= OBJANIM_FLAG_OWNS_PLACEMENT_DATA;
            placement.scale = 0.15625f;
            placement.gameBit = 0x123;
        } else {
            fh_test_write_be32(&placement.scale, 0x3e200000);
            fh_test_write_be16(&placement.gameBit, 0x123);
        }
        placement.effectId = 6;
        placement.effectParamA = 2;
        placement.effectParamB = expectedB = 3;
        placement.spawnChance = 37;
        placement.spreadX = 7;
        placement.spreadY = 8;
        placement.spreadZ = 9;
        enabled = spawns = bitReads = 0;

        // When the source updates while the gate is clear.
        dustmotesou_update(&obj);

        // Then the decoded game bit suppresses all spawning.
        FH_CHECK(spawns == 0 && bitReads == 1);

        for (int mode = 0; mode <= 2; mode++) {
            // Given the gate opens for each of the three burst modes.
            enabled = 1;
            placement.burstMode = mode;
            spawns = 0;

            // When the source emits its effect.
            dustmotesou_update(&obj);

            // Then the appropriate emitter receives the same decoded scale and placement parameters.
            FH_CHECK(spawns == 1 && spawnKind == mode + 1);
        }

        // Given the no-game-bit sentinel and a disabled third parameter.
        fh_test_write_be16(&placement.gameBit, 0xffff);
        placement.effectParamB = expectedB = 0;
        enabled = bitReads = spawns = 0;

        // When an ordinary source updates.
        dustmotesou_update(&obj);

        // Then no game-bit query occurs and the missing parameter suppresses the ordinary burst.
        FH_CHECK(bitReads == 0 && spawns == 0);

        // When the same parameters drive the tail-light variant.
        obj.anim.romDefNo = 0x807;
        dustmotesou_update(&obj);

        // Then retail permits the zero third parameter and selects the masked emitter.
        FH_CHECK(spawns == 1 && spawnKind == 4 && bitReads == 0);

        // When they drive the firework variant instead.
        obj.anim.romDefNo = 0x80e;
        spawns = 0;
        dustmotesou_update(&obj);

        // Then the firework burst also accepts the zero third parameter.
        FH_CHECK(spawns == 1 && spawnKind == 5);

        // When the required first parameter is removed from the firework.
        placement.effectParamA = 0;
        spawns = 0;
        dustmotesou_update(&obj);

        // Then even that special variant stops emitting.
        FH_CHECK(spawns == 0);
    }
    return EXIT_SUCCESS;
}
