#ifndef MAIN_DLL_SPELL_PLACE_H_
#define MAIN_DLL_SPELL_PLACE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_setup.h"

typedef enum SpellPlaceMode {
    SPELL_PLACE_MODE_SEQUENCE_A = 1,
    SPELL_PLACE_MODE_SEQUENCE_B = 2,
} SpellPlaceMode;

typedef struct SpellPlacePlacement {
    ObjPlacement base;
    s8 yawByte;
    u8 pad19[5];
    s16 completionGameBit;
    s16 activationGameBit;
} SpellPlacePlacement;

typedef struct SpellPlaceState {
    s16 completionGameBit;
    s16 activationGameBit;
    u8 completionLatched;
    u8 pad05;
} SpellPlaceState;

STATIC_ASSERT(sizeof(SpellPlacePlacement) == 0x24);
STATIC_ASSERT(offsetof(SpellPlacePlacement, yawByte) == 0x18);
STATIC_ASSERT(offsetof(SpellPlacePlacement, completionGameBit) == 0x1E);
STATIC_ASSERT(offsetof(SpellPlacePlacement, activationGameBit) == 0x20);

STATIC_ASSERT(sizeof(SpellPlaceState) == 0x06);
STATIC_ASSERT(offsetof(SpellPlaceState, completionGameBit) == 0x00);
STATIC_ASSERT(offsetof(SpellPlaceState, activationGameBit) == 0x02);
STATIC_ASSERT(offsetof(SpellPlaceState, completionLatched) == 0x04);

extern ObjectDescriptor gDfpSpellPlaceObjDescriptor;
extern ObjectDescriptor gVfpSpellPlaceObjDescriptor;

#endif
