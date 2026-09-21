#ifndef DLLS_OBJECTS_498_SB_CAGEKYTE_H_
#define DLLS_OBJECTS_498_SB_CAGEKYTE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/objseq.h"

typedef struct SBCageKyteState {
    s16 chirpTimer;
} SBCageKyteState;

STATIC_ASSERT(offsetof(SBCageKyteState, chirpTimer) == 0x00);
STATIC_ASSERT(sizeof(SBCageKyteState) == 0x02);

int SB_CageKyte_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int SB_CageKyte_getExtraSize(void);
int SB_CageKyte_getObjectTypeId(void);
void SB_CageKyte_free(void);
void SB_CageKyte_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void SB_CageKyte_hitDetect(void);
void SB_CageKyte_update(GameObject* obj);
void SB_CageKyte_init(GameObject* obj);
void SB_CageKyte_release(void);
void SB_CageKyte_initialise(void);

extern ObjectDescriptor gSB_CageKyteObjDescriptor;

#endif /* DLLS_OBJECTS_498_SB_CAGEKYTE_H_ */
