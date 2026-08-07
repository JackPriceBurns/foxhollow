#ifndef DLLS_OBJECTS_387_MMP_GYSERVE_H_
#define DLLS_OBJECTS_387_MMP_GYSERVE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct MMPGeyserVentPlacement {
    ObjPlacement base;
    u8 unknown18[2];
    s16 moonRockKind;
    u8 unknown1C[2];
    s16 disableGameBit;
    s16 unknown20;
    u8 unknown22[2];
} MMPGeyserVentPlacement;

STATIC_ASSERT(sizeof(MMPGeyserVentPlacement) == 0x24);
STATIC_ASSERT(offsetof(MMPGeyserVentPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(MMPGeyserVentPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(MMPGeyserVentPlacement, moonRockKind) == 0x1A);
STATIC_ASSERT(offsetof(MMPGeyserVentPlacement, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(MMPGeyserVentPlacement, disableGameBit) == 0x1E);
STATIC_ASSERT(offsetof(MMPGeyserVentPlacement, unknown20) == 0x20);
STATIC_ASSERT(offsetof(MMPGeyserVentPlacement, unknown22) == 0x22);

extern ObjectDescriptor gMMPGeyserVentObjDescriptor;

int mmpGeyserVent_getExtraSize(void);
int mmpGeyserVent_getObjectTypeId(void);
void mmpGeyserVent_free(void);
void mmpGeyserVent_render(void);
void mmpGeyserVent_hitDetect(void);
void mmpGeyserVent_update(GameObject* obj);
void mmpGeyserVent_init(GameObject* obj);
void mmpGeyserVent_release(void);
void mmpGeyserVent_initialise(void);

#endif /* DLLS_OBJECTS_387_MMP_GYSERVE_H_ */
