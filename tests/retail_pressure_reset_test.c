#include "test_support.h"
#include "../game/src/dlls/objects/251/251.c"

static int bitWrites;

void mainSetBits(int bit, int value) {
    FH_CHECK(bit == 37 && value == 0);
    bitWrites++;
}

int main(void) {
    // Given a switch displaced from its placement on every axis and a saved reset height.
    GameObject pad = {0};
    PressureSwitchFBState state = {0};
    PressureSwitchFBPlacement placement = {0};
    ObjSeqState sequence = {0};
    pad.extra = &state;
    pad.anim.placementData = (s16*)&placement.base;
    pad.anim.flags |= OBJANIM_FLAG_OWNS_PLACEMENT_DATA;
    placement.pressedGameBit = 37;
    placement.base.posX = 13.0f;
    placement.base.posY = 17.0f;
    placement.base.posZ = 27.0f;
    pad.anim.localPosX = 91.0f;
    pad.anim.localPosY = 92.0f;
    pad.anim.localPosZ = 93.0f;
    state.targetPosY = 45.0f;

    // When the animation resets the switch.
    sequence.curEventId = PRESSURESWITCHFB_ANIM_COMMAND_RESET;
    PressureSwitchFB_animEventCallback(&pad, 0, &sequence);

    // Then retail's reset bug leaves X displaced while restoring the saved Y and placement Z.
    FH_CHECK(pad.anim.localPosX == 91.0f);
    FH_CHECK(pad.anim.localPosY == 45.0f);
    FH_CHECK(pad.anim.localPosZ == 27.0f);
    FH_CHECK(bitWrites == 1);
    FH_CHECK(sequence.curEventId == PRESSURESWITCHFB_ANIM_COMMAND_IDLE);
    return EXIT_SUCCESS;
}
