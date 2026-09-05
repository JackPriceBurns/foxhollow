#include "test_support.h"
#include "../game/src/dlls/objects/475/475.c"

f32 timeDelta = 1;
static GameObject* platform;
static GameObject* player;
static int trigger;
static int bitWrites;
static int lastBitValue;

GameObject* Obj_GetPlayerObject(void) {
    return player;
}

u32 mainGetBit(int bit) {
    FH_CHECK(bit == 0x234);
    return trigger;
}

void mainSetBits(int bit, int value) {
    FH_CHECK(bit == 0x123);
    lastBitValue = value;
    bitWrites++;
}

void Sfx_PlayFromObject(GameObject* obj, u16 sound) {
    FH_CHECK(obj == platform && (sound == 0x113 || sound == 0x1f8));
}

void Sfx_StopObjectChannel(GameObject* obj, int channel) {
    FH_CHECK(obj == platform && channel == 8);
}

int main(void) {
    // Given a boarded platform at its upper stop with the player third in the native contact array.
    GameObject obj = {0};
    GameObject other = {0};
    Dll1DBState state = {0};
    Dll1DBPlacementView placement = {0};
    ObjHitboxTransformState contacts = {0};
    platform = &obj;
    player = fh_test_alloc_high(1, sizeof(*player));
    obj.extra = &state;
    obj.anim.hitboxTransformState = &contacts;
    obj.anim.placementData = (s16*)&placement.base;
    placement.base.posY = 300;
    fh_test_write_be16(&placement.boardedGameBit, 0x123);
    fh_test_write_be16(&placement.triggerGameBit, 0x234);
    contacts.contactObjects[0] = &other;
    contacts.contactObjects[1] = &other;
    contacts.contactObjects[2] = player;
    contacts.contactObjectCount = 3;
    state.motionState = 1;
    state.boarded = 1;

    // When contact is checked at the upper stop.
    dll_1DB_update(&obj);

    // Then the player is still aboard and the platform stays at the top.
    FH_CHECK(state.motionState == 1 && state.contactLost == 0 && bitWrites == 0);

    // When the player leaves and then boards again.
    contacts.contactObjectCount = 2;
    dll_1DB_update(&obj);
    FH_CHECK(state.contactLost == 1 && state.motionState == 1);
    contacts.contactObjectCount = 3;
    dll_1DB_update(&obj);

    // Then reboarding starts a fall from zero vertical velocity.
    FH_CHECK(state.motionState == 4 && state.verticalVelocity == 0);

    // Given the falling platform is about to overshoot its lower stop at excessive speed.
    obj.anim.localPosY = 65;
    state.verticalVelocity = -5;

    // When it advances one frame.
    dll_1DB_update(&obj);

    // Then speed is capped and the lower stop lies exactly 235.5 below the placement height.
    FH_CHECK(state.verticalVelocity == -1.5f && obj.anim.localPosY == 64.5f);
    FH_CHECK(state.motionState == 2 && bitWrites == 1 && lastBitValue == 1);

    // When the player stays aboard at the bottom, then leaves.
    dll_1DB_update(&obj);
    FH_CHECK(state.motionState == 2 && bitWrites == 1);
    contacts.contactObjectCount = 2;
    dll_1DB_update(&obj);

    // Then leaving clears the boarded bit and starts the return trip.
    FH_CHECK(state.motionState == 3 && state.boarded == 0 && state.verticalVelocity == 0);
    FH_CHECK(bitWrites == 2 && lastBitValue == 0);

    // Given a rider on the rising platform just below the upper stop.
    contacts.contactObjectCount = 3;
    obj.anim.localPosY = 299.5f;
    state.verticalVelocity = 5;

    // When it rises through the upper stop.
    dll_1DB_update(&obj);

    // Then it clamps upward speed and height and remembers that the rider reached the top.
    FH_CHECK(state.verticalVelocity == 1.5f && obj.anim.localPosY == 300);
    FH_CHECK(state.motionState == 1 && state.boarded == 1 && state.contactLost == 0);

    // Given an empty platform at the top with its external trigger set.
    contacts.contactObjectCount = 0;
    state.boarded = 0;
    trigger = 1;

    // When the external trigger is processed.
    dll_1DB_update(&obj);

    // Then it descends even without a rider.
    FH_CHECK(state.motionState == 4 && state.verticalVelocity == 0);

    // When that trigger clears while the empty platform is still falling.
    trigger = 0;
    obj.anim.localPosY = 200;
    dll_1DB_update(&obj);

    // Then the platform reverses and clears the boarded game bit.
    FH_CHECK(state.motionState == 3 && lastBitValue == 0 && bitWrites == 3);
    free(player);
    return EXIT_SUCCESS;
}
