#ifndef DLLS_OBJECTS_221_GCBADDIESHIELD_H_
#define DLLS_OBJECTS_221_GCBADDIESHIELD_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct GCbaddieShieldPlacement {
    ObjPlacement base; /* 0x00 */
    s16 unk18;         /* 0x18 */
    s16 lifetime;      /* 0x1A */
} GCbaddieShieldPlacement;

typedef struct GCbaddieShieldState {
    f32 remainingLifetime; /* 0x00 */
    u8 pad04[4];           /* 0x04 */
} GCbaddieShieldState;

STATIC_ASSERT(offsetof(GCbaddieShieldPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(GCbaddieShieldPlacement, unk18) == 0x18);
STATIC_ASSERT(offsetof(GCbaddieShieldPlacement, lifetime) == 0x1A);
STATIC_ASSERT(sizeof(GCbaddieShieldPlacement) == 0x1C);
STATIC_ASSERT(offsetof(GCbaddieShieldState, remainingLifetime) == 0x0);
STATIC_ASSERT(sizeof(GCbaddieShieldState) == 0x8);

int GCbaddieShield_getExtraSize(void);
int GCbaddieShield_getObjectTypeId(void);
void GCbaddieShield_free(GameObject* obj);
void GCbaddieShield_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void GCbaddieShield_hitDetect(GameObject* obj);
void GCbaddieShield_update(GameObject* obj);
void GCbaddieShield_init(GameObject* obj, GCbaddieShieldPlacement* placement);
void GCbaddieShield_release(void);
void GCbaddieShield_initialise(void);

extern ObjectDescriptor gGCbaddieShieldObjDescriptor;

#endif /* DLLS_OBJECTS_221_GCBADDIESHIELD_H_ */
