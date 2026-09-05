#include "test_support.h"
#include "../game/src/dlls/objects/251/251.c"

static int bitWrites;

void mainSetBits(int bit, int value) {
    FH_CHECK(bit == 37);
    FH_CHECK(value == 0);
    bitWrites++;
}

int main(void) {
    // Given a pressure switch tracking ten objects allocated above 4 GB.
    GameObject* pad = fh_test_alloc_high(1, sizeof(*pad));
    PressureSwitchFBState* state = fh_test_alloc_high(1, PressureSwitchFB_getExtraSize());
    GameObject* objects = fh_test_alloc_high(10, sizeof(*objects));
    PressureSwitchFBPlacement placement = {0};
    ObjSeqState sequence = {0};
    pad->extra = state;
    pad->anim.placementData = (s16*)&placement.base;
    pad->anim.flags |= OBJANIM_FLAG_OWNS_PLACEMENT_DATA;
    placement.pressedGameBit = 37;
    placement.base.posX = 13.0f;
    placement.base.posZ = 27.0f;
    pad->anim.localPosY = 45.0f;
    for (int i = 0; i < 10; i++) {
        state->trackedObjects[i] = &objects[i];
        objects[i].anim.localPosX = 100.0f + i;
        objects[i].anim.localPosZ = 200.0f + i;
    }

    // When its animation captures the tracked positions.
    sequence.curEventId = PRESSURESWITCHFB_ANIM_COMMAND_CAPTURE_POSITIONS;
    PressureSwitchFB_animEventCallback(pad, 0, &sequence);

    // Then every object reference and its corresponding coordinates are preserved.
    for (int i = 0; i < 10; i++) {
        FH_CHECK(state->trackedObjects[i] == &objects[i]);
        FH_CHECK(state->trackedPositions[i].x == 100.0f + i);
        FH_CHECK(state->trackedPositions[i].z == 200.0f + i);
    }
    FH_CHECK(sequence.curEventId == PRESSURESWITCHFB_ANIM_COMMAND_IDLE);
    FH_CHECK(bitWrites == 0);

    // When the animation resets the switch.
    sequence.curEventId = PRESSURESWITCHFB_ANIM_COMMAND_RESET;
    PressureSwitchFB_animEventCallback(pad, 0, &sequence);

    // Then all ten references are cleared and the switch restores its saved Y and placement Z.
    for (int i = 0; i < 10; i++) {
        FH_CHECK(state->trackedObjects[i] == NULL);
    }
    FH_CHECK(bitWrites == 1);
    FH_CHECK(pad->anim.localPosY == 45.0f);
    FH_CHECK(pad->anim.localPosZ == 27.0f);
    FH_CHECK(sequence.curEventId == PRESSURESWITCHFB_ANIM_COMMAND_IDLE);
    free(objects);
    free(state);
    free(pad);
    return EXIT_SUCCESS;
}
