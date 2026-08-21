#ifndef MAIN_DLL_DLL_02AD_SOFTBODY_H
#define MAIN_DLL_DLL_02AD_SOFTBODY_H

#include "global.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "dlls/object_descriptor.h"

typedef struct SoftBodyPlacement
{
    ObjPlacement base;
    u8 rotationZByte;
    u8 rotationYByte;
    u8 rotationXByte;
    u8 scaleByte;
    u8 unknown1C[3];
    u8 phaseDriverDisabled;
} SoftBodyPlacement;

STATIC_ASSERT(offsetof(SoftBodyPlacement, rotationZByte) == 0x18);
STATIC_ASSERT(offsetof(SoftBodyPlacement, scaleByte) == 0x1b);
STATIC_ASSERT(offsetof(SoftBodyPlacement, phaseDriverDisabled) == 0x1f);
STATIC_ASSERT(sizeof(SoftBodyPlacement) == 0x20);

extern ObjectDescriptor gSoftBodyObjDescriptor;

int SoftBody_getExtraSize(void);
int SoftBody_getObjectTypeId(void);
void SoftBody_free(GameObject* obj);
void SoftBody_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible);
void SoftBody_hitDetect(void);
void SoftBody_init(GameObject* obj, const SoftBodyPlacement* placement);
void SoftBody_update(GameObject* obj);
void SoftBody_release(void);
void SoftBody_initialise(void);

#endif
