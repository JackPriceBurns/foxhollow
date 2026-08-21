#ifndef MAIN_DLL_DR_DLL_0268_DRCAGECONTROL_H_
#define MAIN_DLL_DR_DLL_0268_DRCAGECONTROL_H_

#include "game/objects/object.h"
#include "dlls/object_descriptor.h"
#include "game/objects/object_setup.h"

typedef struct CageControlPlacement
{
    ObjPlacement base;
    u8 unknown18[6];
    s16 armGameBit;
    s16 watchGameBit;
    u8 unknown22[6];
} CageControlPlacement;

STATIC_ASSERT(offsetof(CageControlPlacement, armGameBit) == 0x1e);
STATIC_ASSERT(offsetof(CageControlPlacement, watchGameBit) == 0x20);
STATIC_ASSERT(sizeof(CageControlPlacement) == 0x28);


int DR_CageControl_SeqFn(GameObject* obj);
int DR_CageControl_getExtraSize(void);
int DR_CageControl_getObjectTypeId(void);
void DR_CageControl_free(void);
void DR_CageControl_render(GameObject* obj, u32 p2, u32 p3, u32 p4, u32 p5, s8 visible);
void DR_CageControl_hitDetect(void);
void DR_CageControl_update(GameObject* obj);
void DR_CageControl_init(GameObject* obj, const CageControlPlacement* placement);
void DR_CageControl_release(void);
void DR_CageControl_initialise(void);

extern ObjectDescriptor gDrCageControlObjDescriptor;

#endif /* MAIN_DLL_DR_DLL_0268_DRCAGECONTROL_H_ */
