#ifndef DLLS_OBJECTS_433_SH_STAFF_H_
#define DLLS_OBJECTS_433_SH_STAFF_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define SHSTAFF_HAZE_CHILD_COUNT 10

typedef struct ObjSeqState ObjSeqState;

typedef struct ShStaffPlacement {
    ObjPlacement base;
    u8 rotZByte;
    u8 rotYByte;
} ShStaffPlacement;

typedef struct ShStaffState {
    u8 phase;
    u8 helpTextVisible;
    u8 hazeFlags;
    u8 pickupMapLoaded;
    f32 hazeFadeTimer;
    f32 carryMatrix[12];
    int hazeChildren[SHSTAFF_HAZE_CHILD_COUNT]; /* GameObject pointers kept in their exact integer storage shape */
    u8 hazeSpawnPending[SHSTAFF_HAZE_CHILD_COUNT];
    u8 unknown6A[2];
    f32 hazeClimbT;
    f32 fizzSfxTimer;
} ShStaffState;

STATIC_ASSERT(offsetof(ShStaffPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(ShStaffPlacement, rotZByte) == 0x18);
STATIC_ASSERT(offsetof(ShStaffPlacement, rotYByte) == 0x19);

STATIC_ASSERT(sizeof(ShStaffState) == 0x74);
STATIC_ASSERT(offsetof(ShStaffState, phase) == 0x00);
STATIC_ASSERT(offsetof(ShStaffState, helpTextVisible) == 0x01);
STATIC_ASSERT(offsetof(ShStaffState, hazeFlags) == 0x02);
STATIC_ASSERT(offsetof(ShStaffState, pickupMapLoaded) == 0x03);
STATIC_ASSERT(offsetof(ShStaffState, hazeFadeTimer) == 0x04);
STATIC_ASSERT(offsetof(ShStaffState, carryMatrix) == 0x08);
STATIC_ASSERT(offsetof(ShStaffState, hazeChildren) == 0x38);
STATIC_ASSERT(offsetof(ShStaffState, hazeSpawnPending) == 0x60);
STATIC_ASSERT(offsetof(ShStaffState, unknown6A) == 0x6A);
STATIC_ASSERT(offsetof(ShStaffState, hazeClimbT) == 0x6C);
STATIC_ASSERT(offsetof(ShStaffState, fizzSfxTimer) == 0x70);

int sh_staff_getExtraSize(void);
void sh_staff_free(GameObject* obj, int freeArg);
void sh_staff_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
int sh_staff_sequenceCallback(GameObject* obj, int unused, ObjSeqState* animUpdate);
void sh_staff_deactivate(GameObject* obj, ShStaffState* state, int clearChildren);
void sh_staff_update(GameObject* obj);

extern ObjectDescriptor gSH_staffObjDescriptor;

#endif /* DLLS_OBJECTS_433_SH_STAFF_H_ */
