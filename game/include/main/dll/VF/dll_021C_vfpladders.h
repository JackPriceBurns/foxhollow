#ifndef MAIN_DLL_VF_DLL_021C_VFPLADDERS_H_
#define MAIN_DLL_VF_DLL_021C_VFPLADDERS_H_

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"
#include "dlls/object_descriptor.h"

typedef struct VfpLaddersPlacement {
    ObjPlacement base;
    s8 rotXByte;
    u8 pad19[0x1E - 0x19];
    s16 baseGameBit;
    s16 triggerGameBit;
} VfpLaddersPlacement;

STATIC_ASSERT(offsetof(VfpLaddersPlacement, base.posY) == 0x0C);
STATIC_ASSERT(offsetof(VfpLaddersPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(VfpLaddersPlacement, baseGameBit) == 0x1E);
STATIC_ASSERT(offsetof(VfpLaddersPlacement, triggerGameBit) == 0x20);

int vfpladders_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int VFP_Ladders_getExtraSize(void);
int VFP_Ladders_getObjectTypeId(void);
void VFP_Ladders_free(GameObject* obj);
void VFP_Ladders_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible);
void VFP_Ladders_hitDetect(GameObject* obj);
void VFP_Ladders_update(GameObject* obj);
void VFP_Ladders_init(GameObject* obj, const VfpLaddersPlacement* placement);
void VFP_Ladders_release(void);
void VFP_Ladders_initialise(void);

extern ObjectDescriptor gVFP_LaddersObjDescriptor;

#endif /* MAIN_DLL_VF_DLL_021C_VFPLADDERS_H_ */
