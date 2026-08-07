#ifndef DLLS_OBJECTS_506_WM_SEQOBJEC_H_
#define DLLS_OBJECTS_506_WM_SEQOBJEC_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef struct WMSeqObjectState {
    u8 unknown00;
} WMSeqObjectState;

STATIC_ASSERT(offsetof(WMSeqObjectState, unknown00) == 0x00);
STATIC_ASSERT(sizeof(WMSeqObjectState) == 0x01);

/*
 * Only the fields consumed by this DLL are modeled. Active-target placement
 * data is unavailable, so this does not claim a complete record width.
 */
typedef struct WMSeqObjectPlacementView {
    ObjPlacement base;
    s8 rotationXByte;
    s8 mode;
} WMSeqObjectPlacementView;

STATIC_ASSERT(offsetof(WMSeqObjectPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(WMSeqObjectPlacementView, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(WMSeqObjectPlacementView, mode) == 0x19);

int WM_seqobject_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int WM_seqobject_getExtraSize(void);
int WM_seqobject_getObjectTypeId(void);
void WM_seqobject_free(void);
void WM_seqobject_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void WM_seqobject_hitDetect(void);
void WM_seqobject_update(GameObject* obj);
void WM_seqobject_init(GameObject* obj, const WMSeqObjectPlacementView* placement);
void WM_seqobject_release(void);
void WM_seqobject_initialise(void);

extern ObjectDescriptor gWM_seqobjectObjDescriptor;

#endif /* DLLS_OBJECTS_506_WM_SEQOBJEC_H_ */
