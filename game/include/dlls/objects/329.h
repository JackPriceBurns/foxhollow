#ifndef DLLS_OBJECTS_329_H_
#define DLLS_OBJECTS_329_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define WINDLIFT_SLOT_COUNT 14

typedef struct WindLiftPlacement {
    ObjPlacement base;
    s8 unknown18;
    s8 heightParam;
    s16 pullStrength;
    s16 riderGameBit;
    s16 activationGameBit;
} WindLiftPlacement;

typedef struct WindLiftSlot {
    int riderObject;
    f32 unknown04;
    f32 speedDelta;
    f32 riseSpeed;
    u8 phaseFlags;
    u8 oscillationCounter;
    u8 pad12[0x02];
    int linkIndex;
} WindLiftSlot;

typedef struct WindLiftState {
    int duration;
    int activationGameBit;
    int riderGameBit;
    int enableGameBit;
    int unknown10;
    int timer;
    WindLiftSlot slots[WINDLIFT_SLOT_COUNT];
    int unknown168;
    int unknown16C;
    f32 liftHeight;
    u8 musicActive : 1;
    u8 enabled : 1;
    u8 unusedFlags : 6;
    u8 pad175[0x03];
} WindLiftState;

STATIC_ASSERT(offsetof(WindLiftPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(WindLiftPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(WindLiftPlacement, heightParam) == 0x19);
STATIC_ASSERT(offsetof(WindLiftPlacement, pullStrength) == 0x1A);
STATIC_ASSERT(offsetof(WindLiftPlacement, riderGameBit) == 0x1C);
STATIC_ASSERT(offsetof(WindLiftPlacement, activationGameBit) == 0x1E);

STATIC_ASSERT(offsetof(WindLiftSlot, riderObject) == 0x00);
STATIC_ASSERT(offsetof(WindLiftSlot, unknown04) == 0x04);
STATIC_ASSERT(offsetof(WindLiftSlot, speedDelta) == 0x08);
STATIC_ASSERT(offsetof(WindLiftSlot, riseSpeed) == 0x0C);
STATIC_ASSERT(offsetof(WindLiftSlot, phaseFlags) == 0x10);
STATIC_ASSERT(offsetof(WindLiftSlot, oscillationCounter) == 0x11);
STATIC_ASSERT(offsetof(WindLiftSlot, pad12) == 0x12);
STATIC_ASSERT(offsetof(WindLiftSlot, linkIndex) == 0x14);
STATIC_ASSERT(sizeof(WindLiftSlot) == 0x18);

STATIC_ASSERT(offsetof(WindLiftState, duration) == 0x000);
STATIC_ASSERT(offsetof(WindLiftState, activationGameBit) == 0x004);
STATIC_ASSERT(offsetof(WindLiftState, riderGameBit) == 0x008);
STATIC_ASSERT(offsetof(WindLiftState, enableGameBit) == 0x00C);
STATIC_ASSERT(offsetof(WindLiftState, unknown10) == 0x010);
STATIC_ASSERT(offsetof(WindLiftState, timer) == 0x014);
STATIC_ASSERT(offsetof(WindLiftState, slots) == 0x018);
STATIC_ASSERT(offsetof(WindLiftState, unknown168) == 0x168);
STATIC_ASSERT(offsetof(WindLiftState, unknown16C) == 0x16C);
STATIC_ASSERT(offsetof(WindLiftState, liftHeight) == 0x170);
STATIC_ASSERT(offsetof(WindLiftState, pad175) == 0x175);
STATIC_ASSERT(sizeof(WindLiftState) == 0x178);

void windLift_updateRider(GameObject* obj, GameObject* rider, WindLiftSlot* slot, f32 pullStrength, int riderGameBit,
                          int isPlayer, u32 duration, f32 radius);
int windLift_getExtraSize(void);
int windLift_getObjectTypeId(void);
void windLift_free(GameObject* obj);
void windLift_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void windLift_hitDetect(void);
void windLift_update(GameObject* obj);
void windLift_init(GameObject* obj, WindLiftPlacement* placement);
void windLift_release(void);
void windLift_initialise(void);

extern ObjectDescriptor gWindLiftObjDescriptor;

#endif /* DLLS_OBJECTS_329_H_ */
