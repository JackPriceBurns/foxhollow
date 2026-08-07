#ifndef DLLS_OBJECTS_486_DIMBOSSCRAC_H_
#define DLLS_OBJECTS_486_DIMBOSSCRAC_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/*
 * Only the fields consumed by this DLL are modeled. The active-target retail
 * placement files are unavailable, so this does not claim a complete record
 * width.
 */
typedef struct DIMbosscrackparPlacementView {
    ObjPlacement base;
    u8 unknown18[0x02];
    s16 particleEffectOffset;
    u8 unknown1C[0x02];
    s16 triggerGameBit;
    u8 unknown20[0x02];
    s8 rotationZByte;
    s8 rotationYByte;
    s8 rotationXByte;
} DIMbosscrackparPlacementView;

STATIC_ASSERT(offsetof(DIMbosscrackparPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(DIMbosscrackparPlacementView, particleEffectOffset) == 0x1A);
STATIC_ASSERT(offsetof(DIMbosscrackparPlacementView, triggerGameBit) == 0x1E);
STATIC_ASSERT(offsetof(DIMbosscrackparPlacementView, rotationZByte) == 0x22);
STATIC_ASSERT(offsetof(DIMbosscrackparPlacementView, rotationYByte) == 0x23);
STATIC_ASSERT(offsetof(DIMbosscrackparPlacementView, rotationXByte) == 0x24);

int DIMbosscrackpar_SeqFn(GameObject* obj);
int DIMbosscrackpar_getExtraSize(void);
int DIMbosscrackpar_getObjectTypeId(void);
void DIMbosscrackpar_free(GameObject* obj);
void DIMbosscrackpar_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                            s8 visible);
void DIMbosscrackpar_hitDetect(void);
void DIMbosscrackpar_update(GameObject* obj);
void DIMbosscrackpar_init(GameObject* obj, DIMbosscrackparPlacementView* placement);
void DIMbosscrackpar_release(void);
void DIMbosscrackpar_initialise(void);

extern ObjectDescriptor gDIMbosscrackparObjDescriptor;

#endif /* DLLS_OBJECTS_486_DIMBOSSCRAC_H_ */
