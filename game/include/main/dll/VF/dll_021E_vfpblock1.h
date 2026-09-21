#ifndef MAIN_DLL_VF_DLL_021E_VFPBLOCK1_H_
#define MAIN_DLL_VF_DLL_021E_VFPBLOCK1_H_

#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct VfpBlock1State {
    s16 gameBitId;
} VfpBlock1State;

typedef struct VfpBlock1Placement {
    ObjPlacement base;
    s8 rotXByte;
    u8 pad19[5];
    s16 gameBitId;
} VfpBlock1Placement;

int VFP_Block1_getExtraSize(void);
int VFP_Block1_getObjectTypeId(void);
void VFP_Block1_free(GameObject* obj);
void VFP_Block1_render(void);
void VFP_Block1_hitDetect(void);
void VFP_Block1_update(GameObject* obj);
void VFP_Block1_init(GameObject* obj, VfpBlock1Placement* data);
void VFP_Block1_release(void);
void VFP_Block1_initialise(void);

#endif /* MAIN_DLL_VF_DLL_021E_VFPBLOCK1_H_ */
