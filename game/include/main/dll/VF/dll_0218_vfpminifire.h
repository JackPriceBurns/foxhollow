#ifndef MAIN_DLL_VF_DLL_0218_VFPMINIFIRE_H_
#define MAIN_DLL_VF_DLL_0218_VFPMINIFIRE_H_

#include "types.h"
#include "game/objects/object.h"
#include "dlls/object_descriptor.h"
#include "global.h"

typedef struct VfpMinifireState
{
    f32 baseY; /* 0x00: downward hit-scan result, then rebased to
                         (localPosY - that) as the fall threshold */
    u8 pad4[6];
    u8 burstStarted; /* 0x0A: flame burst has fired; fading out */
} VfpMinifireState;

int VFP_MiniFire_getExtraSize(void);
int VFP_MiniFire_getObjectTypeId(void);
void VFP_MiniFire_free(int obj);
void VFP_MiniFire_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 vis);
void VFP_MiniFire_hitDetect(void);
void VFP_MiniFire_update(GameObject* obj);
void VFP_MiniFire_init(GameObject* obj, u8* init);
void VFP_MiniFire_release(void);
void VFP_MiniFire_initialise(void);

extern ObjectDescriptor gVFP_MiniFireObjDescriptor;

#endif /* MAIN_DLL_VF_DLL_0218_VFPMINIFIRE_H_ */
