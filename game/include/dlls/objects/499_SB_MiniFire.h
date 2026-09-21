#ifndef DLLS_OBJECTS_499_SB_MINIFIRE_H_
#define DLLS_OBJECTS_499_SB_MINIFIRE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

typedef struct SBMiniFireState {
    u8 unknown00[2];
} SBMiniFireState;

STATIC_ASSERT(offsetof(SBMiniFireState, unknown00) == 0x00);
STATIC_ASSERT(sizeof(SBMiniFireState) == 0x02);

int SB_MiniFire_getExtraSize(void);
int SB_MiniFire_getObjectTypeId(void);
void SB_MiniFire_free(GameObject* obj);
void SB_MiniFire_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void SB_MiniFire_hitDetect(void);
void SB_MiniFire_update(GameObject* obj);
void SB_MiniFire_init(GameObject* obj);
void SB_MiniFire_release(void);
void SB_MiniFire_initialise(void);

extern int gSbMiniFireResourceVariant;
extern ObjectDescriptor gSB_MiniFireObjDescriptor;

#endif /* DLLS_OBJECTS_499_SB_MINIFIRE_H_ */
