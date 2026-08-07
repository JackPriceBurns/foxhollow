#ifndef DLLS_OBJECTS_242_ICEBLAST_H_
#define DLLS_OBJECTS_242_ICEBLAST_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct IceblastPlacement {
    ObjPlacement base;      /* 0x00 */
    u8 pad18;               /* 0x18 */
    s8 hitVolumeMode;       /* 0x19: nonzero selects hit type 3 instead of 1 */
    s16 initialLaunchTimer; /* 0x1A */
    u16 unk1C;              /* 0x1C: initialized by playerCastIceSpell */
    u8 pad1E[6];            /* 0x1E */
} IceblastPlacement;

typedef struct IceblastState {
    f32 launchTimer; /* 0x00 */
} IceblastState;

STATIC_ASSERT(offsetof(IceblastPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(IceblastPlacement, pad18) == 0x18);
STATIC_ASSERT(offsetof(IceblastPlacement, hitVolumeMode) == 0x19);
STATIC_ASSERT(offsetof(IceblastPlacement, initialLaunchTimer) == 0x1A);
STATIC_ASSERT(offsetof(IceblastPlacement, unk1C) == 0x1C);
STATIC_ASSERT(offsetof(IceblastPlacement, pad1E) == 0x1E);
STATIC_ASSERT(sizeof(IceblastPlacement) == 0x24);

STATIC_ASSERT(offsetof(IceblastState, launchTimer) == 0x0);
STATIC_ASSERT(sizeof(IceblastState) == 0x4);

int iceblast_getExtraSize(void);
int iceblast_getObjectTypeId(void);
void iceblast_free(GameObject* obj);
void iceblast_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5);
void iceblast_hitDetect(GameObject* obj);
void iceblast_update(GameObject* obj);
void iceblast_init(GameObject* obj, IceblastPlacement* placement);
void iceblast_release(void);
void iceblast_initialise(void);

extern ObjectDescriptor gIceblastObjDescriptor;

#endif /* DLLS_OBJECTS_242_ICEBLAST_H_ */
