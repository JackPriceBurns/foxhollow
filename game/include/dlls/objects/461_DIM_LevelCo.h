#ifndef DLLS_OBJECTS_461_DIM_LEVELCO_H_
#define DLLS_OBJECTS_461_DIM_LEVELCO_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "global.h"

/* The sole active-target retail placement is exactly the common six-word head. */
typedef ObjPlacement DimLevelControlPlacement;

typedef struct DimLevelControlState {
    f32 messageTimer;
    s32 musicLatchMask;
    u8 lostInBlizzardState;
    u8 unknown09;
    s16 dayNightMusicTrigger;
    u8 lostInBlizzardDialogueFired;
    u8 dinoHornGroupEnabled;
    union {
        u8 statusFlags;
        struct {
            u8 statusGameBitD0B : 1;
            u8 statusGameBitD0C : 1;
            u8 statusGameBitD0D : 1;
            u8 statusGameBitD0E : 1;
            u8 cannonStatusGameBit : 1;
        };
    };
    u8 unknown0F;
} DimLevelControlState;

STATIC_ASSERT(sizeof(DimLevelControlPlacement) == 0x18);

STATIC_ASSERT(offsetof(DimLevelControlState, messageTimer) == 0x00);
STATIC_ASSERT(offsetof(DimLevelControlState, musicLatchMask) == 0x04);
STATIC_ASSERT(offsetof(DimLevelControlState, lostInBlizzardState) == 0x08);
STATIC_ASSERT(offsetof(DimLevelControlState, unknown09) == 0x09);
STATIC_ASSERT(offsetof(DimLevelControlState, dayNightMusicTrigger) == 0x0A);
STATIC_ASSERT(offsetof(DimLevelControlState, lostInBlizzardDialogueFired) == 0x0C);
STATIC_ASSERT(offsetof(DimLevelControlState, dinoHornGroupEnabled) == 0x0D);
STATIC_ASSERT(offsetof(DimLevelControlState, statusFlags) == 0x0E);
STATIC_ASSERT(offsetof(DimLevelControlState, unknown0F) == 0x0F);
STATIC_ASSERT(sizeof(DimLevelControlState) == 0x10);

int dim_levelcontrol_getExtraSize(void);
void dim_levelcontrol_free(GameObject* unused);
void dim_levelcontrol_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                             s8 visible);
void dim_levelcontrol_update(GameObject* obj);
void dim_levelcontrol_init(GameObject* obj);

extern ObjectDescriptor gDIM_LevelControlObjDescriptor;

#endif /* DLLS_OBJECTS_461_DIM_LEVELCO_H_ */
