#ifndef DLLS_OBJECTS_497_SB_SEQDOOR_H_
#define DLLS_OBJECTS_497_SB_SEQDOOR_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

/*
 * Only the placement prefix consumed by this DLL is modeled. The active-target
 * retail placement width is not available, so this does not claim a complete
 * record size.
 */
typedef struct SBSeqDoorPlacementView {
    ObjPlacement base;
    s8 rotXByte;   /* scaled by 0x100 into anim.rotX */
    s8 bankSelect; /* any nonzero value selects animation bank 1 */
} SBSeqDoorPlacementView;

STATIC_ASSERT(offsetof(SBSeqDoorPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(SBSeqDoorPlacementView, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(SBSeqDoorPlacementView, bankSelect) == 0x19);

int SB_SeqDoor_SeqFn(GameObject* obj, int unusedArg, ObjSeqState* animUpdate);
int SB_SeqDoor_getExtraSize(void);
int SB_SeqDoor_getObjectTypeId(void);
void SB_SeqDoor_free(void);
void SB_SeqDoor_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void SB_SeqDoor_hitDetect(void);
void SB_SeqDoor_update(GameObject* obj);
void SB_SeqDoor_init(GameObject* obj, const SBSeqDoorPlacementView* placement);
void SB_SeqDoor_release(void);
void SB_SeqDoor_initialise(void);

extern ObjectDescriptor gSB_SeqDoorObjDescriptor;

#endif /* DLLS_OBJECTS_497_SB_SEQDOOR_H_ */
