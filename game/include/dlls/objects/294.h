#ifndef DLLS_OBJECTS_294_H_
#define DLLS_OBJECTS_294_H_

#include "global.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"

typedef struct MMPTriggerGeyserPlacement {
    ObjPlacement base;
    u8 unknown18[0x3A - 0x18];
    u8 reachScale;
    u8 speed;
    u8 unknown3C;
    u8 rotX;
    u8 rotY;
} MMPTriggerGeyserPlacement;

STATIC_ASSERT(offsetof(MMPTriggerGeyserPlacement, reachScale) == 0x3A);
STATIC_ASSERT(offsetof(MMPTriggerGeyserPlacement, speed) == 0x3B);
STATIC_ASSERT(offsetof(MMPTriggerGeyserPlacement, rotX) == 0x3D);
STATIC_ASSERT(offsetof(MMPTriggerGeyserPlacement, rotY) == 0x3E);

typedef struct MmpGyserventState {
    u8 pad0[0x4 - 0x0];
    f32 nearRadiusSq; /* 0x04: squared near-distance threshold */
    u8 pad8[0xC - 0x8];
    f32 planeNormalX; /* 0x0C: clip-plane normal (vent local forward) */
    f32 planeNormalY; /* 0x10 */
    f32 planeNormalZ; /* 0x14 */
    f32 planeOffset;  /* 0x18: plane d term */
    f32 reachAX;      /* 0x1C: reach endpoint A */
    f32 reachAY;      /* 0x20 */
    f32 reachAZ;      /* 0x24 */
    f32 reachBX;      /* 0x28: reach endpoint B */
    f32 reachBY;      /* 0x2C */
    f32 reachBZ;      /* 0x30 */
    f32 reach;        /* 0x34: eruption reach distance */
    f32 mtx[3][4];    /* 0x38: world->vent-local transform */
} MmpGyserventState;

STATIC_ASSERT(offsetof(MmpGyserventState, nearRadiusSq) == 0x04);
STATIC_ASSERT(offsetof(MmpGyserventState, planeNormalX) == 0x0C);
STATIC_ASSERT(offsetof(MmpGyserventState, planeOffset) == 0x18);
STATIC_ASSERT(offsetof(MmpGyserventState, reachAX) == 0x1C);
STATIC_ASSERT(offsetof(MmpGyserventState, reachBX) == 0x28);
STATIC_ASSERT(offsetof(MmpGyserventState, reach) == 0x34);
STATIC_ASSERT(offsetof(MmpGyserventState, mtx) == 0x38);

void MmpGyservent_setup(GameObject* obj, MMPTriggerGeyserPlacement* placement);
void triggerEvalEndpointCylinders(GameObject* obj, GameObject* seqObj);
void triggerEvalEndpointSpheres(GameObject* obj, GameObject* seqObj);

#endif /* DLLS_OBJECTS_294_H_ */
