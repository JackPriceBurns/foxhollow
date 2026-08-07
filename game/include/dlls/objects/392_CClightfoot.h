#ifndef DLLS_OBJECTS_392_CCLIGHTFOOT_H_
#define DLLS_OBJECTS_392_CCLIGHTFOOT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef struct CCLightfootPlacement {
    ObjPlacement base;
    u8 unknown18[0x1A - 0x18];
    u8 rotXByte;
    u8 unknown1B[0x20 - 0x1B];
} CCLightfootPlacement;

STATIC_ASSERT(sizeof(CCLightfootPlacement) == 0x20);
STATIC_ASSERT(offsetof(CCLightfootPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CCLightfootPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(CCLightfootPlacement, rotXByte) == 0x1A);
STATIC_ASSERT(offsetof(CCLightfootPlacement, unknown1B) == 0x1B);

typedef struct CCLightfootState {
    GameObject* attachedWeapon;
    GameObject* playerObject;
    union {
        struct {
            GameObject* targetActorA;
            GameObject* targetActorB;
        };
        GameObject* targetActors[2];
    };
    u8 phase;
    u8 flags;
    u8 unknown12[2];
    f32 idleSfxTimer;
} CCLightfootState;

STATIC_ASSERT(sizeof(CCLightfootState) == 0x18);
STATIC_ASSERT(offsetof(CCLightfootState, attachedWeapon) == 0x00);
STATIC_ASSERT(offsetof(CCLightfootState, playerObject) == 0x04);
STATIC_ASSERT(offsetof(CCLightfootState, targetActorA) == 0x08);
STATIC_ASSERT(offsetof(CCLightfootState, targetActorB) == 0x0C);
STATIC_ASSERT(offsetof(CCLightfootState, targetActors) == 0x08);
STATIC_ASSERT(offsetof(CCLightfootState, phase) == 0x10);
STATIC_ASSERT(offsetof(CCLightfootState, flags) == 0x11);
STATIC_ASSERT(offsetof(CCLightfootState, unknown12) == 0x12);
STATIC_ASSERT(offsetof(CCLightfootState, idleSfxTimer) == 0x14);

extern ObjectDescriptor gCCLightfootObjDescriptor;

int ccLightfoot_animationEventCallback(GameObject* obj, int unusedArg, ObjSeqState* animUpdate);
int ccLightfoot_getExtraSize(void);
void ccLightfoot_free(GameObject* obj, int keepWeapon);
void ccLightfoot_selectCombatPhase(CCLightfootState* state, GameObject* targetObject, f32 distanceSquared);
void ccLightfoot_update(GameObject* obj);
void ccLightfoot_init(GameObject* obj, const CCLightfootPlacement* placement);

#endif /* DLLS_OBJECTS_392_CCLIGHTFOOT_H_ */
