#ifndef DLLS_OBJECTS_242_ICEBLAST_H_
#define DLLS_OBJECTS_242_ICEBLAST_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef enum IceblastObjectId {
    ICEBLAST_OBJECT_ID = 0x4EC
} IceblastObjectId;

typedef struct IceblastPlacement {
    ObjPlacement base;
    u8 unknown18;
    s8 hitVolumeMode;
    s16 initialLaunchTimer;
    u16 unknown1C;
    u8 unknown1E[6];
} IceblastPlacement;

STATIC_ASSERT(offsetof(IceblastPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(IceblastPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(IceblastPlacement, hitVolumeMode) == 0x19);
STATIC_ASSERT(offsetof(IceblastPlacement, initialLaunchTimer) == 0x1A);
STATIC_ASSERT(offsetof(IceblastPlacement, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(IceblastPlacement, unknown1E) == 0x1E);
STATIC_ASSERT(sizeof(IceblastPlacement) == 0x24);

int iceblast_getExtraSize(void);
int iceblast_getObjectTypeId(void);
void iceblast_free(GameObject* obj);
void iceblast_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5);
void iceblast_hitDetect(GameObject* obj);
void iceblast_update(GameObject* obj);
void iceblast_init(GameObject* obj, const IceblastPlacement* placement);
void iceblast_release(void);
void iceblast_initialise(void);

extern ObjectDescriptor gIceblastObjDescriptor;

#endif /* DLLS_OBJECTS_242_ICEBLAST_H_ */
