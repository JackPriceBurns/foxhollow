#ifndef DLLS_OBJECTS_506_WM_SEQOBJEC_H_
#define DLLS_OBJECTS_506_WM_SEQOBJEC_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef struct WMSeqObjectPlacement {
    ObjPlacement base;
    s8 rotationXByte;
    s8 mode;
} WMSeqObjectPlacement;

STATIC_ASSERT(offsetof(WMSeqObjectPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(WMSeqObjectPlacement, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(WMSeqObjectPlacement, mode) == 0x19);

int WM_seqobject_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int WM_seqobject_getExtraSize(void);
int WM_seqobject_getObjectTypeId(void);
void WM_seqobject_free(void);
void WM_seqobject_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void WM_seqobject_hitDetect(void);
void WM_seqobject_update(GameObject* obj);
void WM_seqobject_init(GameObject* obj, const WMSeqObjectPlacement* placement);
void WM_seqobject_release(void);
void WM_seqobject_initialise(void);

extern ObjectDescriptor gWM_seqobjectObjDescriptor;

#endif /* DLLS_OBJECTS_506_WM_SEQOBJEC_H_ */
