#ifndef DLLS_OBJECTS_222_BADDIEINTERESTP_H_
#define DLLS_OBJECTS_222_BADDIEINTERESTP_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct BaddieInterestPPlacement {
    ObjPlacement base;     /* 0x00; ident is the linked-object ID */
    s8 modeKind;           /* 0x18 */
    s8 triggerProbability; /* 0x19 */
    s16 targetLinkIdLo;    /* 0x1A */
    s16 targetLinkIdHi;    /* 0x1C */
    s16 doneGameBit;       /* 0x1E */
    s16 enableGameBit;     /* 0x20 */
    u8 pad22[0x30 - 0x22]; /* 0x22 */
} BaddieInterestPPlacement;

STATIC_ASSERT(offsetof(BaddieInterestPPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(BaddieInterestPPlacement, modeKind) == 0x18);
STATIC_ASSERT(offsetof(BaddieInterestPPlacement, triggerProbability) == 0x19);
STATIC_ASSERT(offsetof(BaddieInterestPPlacement, targetLinkIdLo) == 0x1A);
STATIC_ASSERT(offsetof(BaddieInterestPPlacement, targetLinkIdHi) == 0x1C);
STATIC_ASSERT(offsetof(BaddieInterestPPlacement, doneGameBit) == 0x1E);
STATIC_ASSERT(offsetof(BaddieInterestPPlacement, enableGameBit) == 0x20);
STATIC_ASSERT(sizeof(BaddieInterestPPlacement) == 0x30);

int BaddieInterestP_getExtraSize(void);
int BaddieInterestP_getObjectTypeId(void);
void BaddieInterestP_free(GameObject* obj);
void BaddieInterestP_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void BaddieInterestP_hitDetect(GameObject* obj);
void BaddieInterestP_update(GameObject* obj);
void BaddieInterestP_init(GameObject* obj);
void BaddieInterestP_release(void);
void BaddieInterestP_initialise(void);

extern ObjectDescriptor gBaddieInterestPObjDescriptor;

#endif /* DLLS_OBJECTS_222_BADDIEINTERESTP_H_ */
