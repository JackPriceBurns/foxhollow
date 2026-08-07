#ifndef DLLS_OBJECTS_496_SB_KYTECAGE_H_
#define DLLS_OBJECTS_496_SB_KYTECAGE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

/* SB_KyteCage_getExtraSize() allocates this complete 0x08-byte state. */
typedef struct SBKyteCageState {
    GameObject* kyte;
    u8 seqLatch;
    u8 releaseStage;
    u8 unknown06[2];
} SBKyteCageState;

/*
 * Only the placement prefix consumed by this DLL is modeled. The active-target
 * retail placement width is not yet evidenced.
 */
typedef struct SBKyteCagePlacementView {
    ObjPlacement base;
    s8 rotXByte;
} SBKyteCagePlacementView;

STATIC_ASSERT(offsetof(SBKyteCageState, kyte) == 0x00);
STATIC_ASSERT(offsetof(SBKyteCageState, seqLatch) == 0x04);
STATIC_ASSERT(offsetof(SBKyteCageState, releaseStage) == 0x05);
STATIC_ASSERT(offsetof(SBKyteCageState, unknown06) == 0x06);
STATIC_ASSERT(sizeof(SBKyteCageState) == 0x08);

STATIC_ASSERT(offsetof(SBKyteCagePlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(SBKyteCagePlacementView, rotXByte) == 0x18);

int SB_KyteCage_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int SB_KyteCage_getExtraSize(void);
int SB_KyteCage_getObjectTypeId(void);
void SB_KyteCage_free(GameObject* obj);
void SB_KyteCage_render(void);
void SB_KyteCage_hitDetect(void);
void SB_KyteCage_update(GameObject* obj);
void SB_KyteCage_init(GameObject* obj, SBKyteCagePlacementView* placement);
void SB_KyteCage_release(void);
void SB_KyteCage_initialise(void);

extern ObjectDescriptor gSB_KyteCageObjDescriptor;

#endif /* DLLS_OBJECTS_496_SB_KYTECAGE_H_ */
