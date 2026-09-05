#include "test_support.h"
#include "../game/src/dlls/objects/266_Fall_Ladder/Fall_Ladder.c"

f32 timeDelta;
static GameObject* ladder;
static int trigger;
static int base;
static int sounds;
static int sequences;
static int lastSequence;
static int models;

u32 mainGetBit(int bit) {
    FH_CHECK(bit == 0x123 || bit == 0x234);
    return bit == 0x123 ? trigger : base;
}

void Sfx_PlayFromObject(GameObject* obj, u16 sound) {
    FH_CHECK(obj == ladder && sound == 0x4bc);
    sounds++;
}

void Obj_SetActiveModelIndex(GameObject* obj, int index) {
    FH_CHECK(obj == ladder && index == 2);
    models++;
}

static int runSequence(int sequence, void* obj, int flags) {
    FH_CHECK(obj == ladder && flags == -1);
    lastSequence = sequence;
    sequences++;
    return 0;
}

static ObjectTriggerInterface triggers = {.runSequence = runSequence};
static ObjectTriggerInterface* triggerPointer = &triggers;
ObjectTriggerInterface** gObjectTriggerInterface = &triggerPointer;

int main(void) {
    // Given a placement with a native base header and big-endian ladder parameters.
    GameObject obj = {0};
    FallLadderState state = {0};
    FallLadderPlacement placement = {0};
    ladder = &obj;
    obj.extra = &state;
    obj.anim.placementData = (s16*)&placement.base;
    placement.base.posY = 100;
    placement.rotXByte = 3;
    placement.modelIndex = 2;
    fh_test_write_be16(&placement.initialHeightOffset, 30);
    fh_test_write_be16(&placement.triggerGameBit, 0x123);
    fh_test_write_be16(&placement.baseGameBit, 0x234);

    // When the ladder initializes with its trigger clear.
    Fall_Ladders_init(&obj, &placement);

    // Then it starts at the raised height, retaining the decoded bits and arming its one-shot sound.
    FH_CHECK(obj.anim.localPosY == 130 && obj.anim.rotX == 768 && models == 1);
    FH_CHECK(state.triggerGameBit == 0x123 && state.baseGameBit == 0x234);
    FH_CHECK(state.motionState == 0 && state.playFallSound == 1);

    // When its trigger is set, followed by an update shorter than one frame.
    trigger = 1;
    timeDelta = 1;
    Fall_Ladders_update(&obj);
    timeDelta = 0.5f;
    Fall_Ladders_update(&obj);

    // Then the ten-frame integer delay has not advanced, matching retail's truncation.
    FH_CHECK(state.fallDelay == 10 && state.motionState == 0);
    FH_CHECK(obj.anim.localPosY == 130 && sounds == 0);

    // When the remaining delay expires in one update.
    timeDelta = 10;
    Fall_Ladders_update(&obj);

    // Then falling is armed and the sound plays once, while motion waits until the next update.
    FH_CHECK(state.fallDelay == 0 && state.motionState == 1 && state.playFallSound == 0);
    FH_CHECK(sounds == 1 && obj.anim.localPosY == 130);

    // When the next update advances two frames.
    timeDelta = 2;
    Fall_Ladders_update(&obj);

    // Then retail applies gravity once and scales displacement by the frame delta.
    FH_CHECK_NEAR(obj.anim.velocityY, -0.9, 0.00001);
    FH_CHECK_NEAR(obj.anim.localPosY, 128.2, 0.00002);
    FH_CHECK(sounds == 1);

    // Given the falling ladder just above its rest height with downward velocity.
    obj.anim.localPosY = 100.1f;
    obj.anim.velocityY = -1;
    timeDelta = 1;

    // When it crosses the floor.
    Fall_Ladders_update(&obj);

    // Then it clamps to the rest height and rebounds with retail's damping.
    FH_CHECK(obj.anim.localPosY == 100 && state.motionState == 1);
    FH_CHECK_NEAR(obj.anim.velocityY, 0.57, 0.00001);

    for (int bits = 0; bits < 4; bits++) {
        // Given the sequence-controlled variant with each combination of its two game bits.
        obj.anim.romDefNo = 0x548;
        trigger = bits & 1;
        base = (bits >> 1) & 1;
        sequences = 0;

        // When the trigger system updates the ladder.
        Fall_Ladders_update(&obj);

        // Then exactly one set bit selects its sequence, while equal bits select neither.
        FH_CHECK(sequences == (trigger != base));
        if (sequences) {
            FH_CHECK(lastSequence == (trigger ? 0 : 1));
        }
    }

    // Given an owned native placement with a negative height offset and the same game-bit identifiers.
    obj = (GameObject){0};
    state = (FallLadderState){0};
    obj.extra = &state;
    obj.anim.placementData = (s16*)&placement.base;
    obj.anim.flags |= OBJANIM_FLAG_OWNS_PLACEMENT_DATA;
    placement.initialHeightOffset = -30;
    placement.triggerGameBit = 0x123;
    placement.baseGameBit = 0x234;
    trigger = 0;

    // When the ladder initializes from that owned placement.
    Fall_Ladders_init(&obj, &placement);

    // Then the signed offset places it below the base and native fields are not byte-swapped.
    FH_CHECK(state.initialHeightOffset == -30 && obj.anim.localPosY == 70);
    FH_CHECK(state.triggerGameBit == 0x123 && state.baseGameBit == 0x234 && models == 2);
    return EXIT_SUCCESS;
}
