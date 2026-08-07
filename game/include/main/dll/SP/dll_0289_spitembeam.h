#ifndef MAIN_DLL_SP_DLL_0289_SPITEMBEAM_H_
#define MAIN_DLL_SP_DLL_0289_SPITEMBEAM_H_

#include "game/objects/object.h"
#include "types.h"
#include "global.h"
#include "game/objects/object_setup.h"

typedef struct SpitembeamPlacement
{
    ObjPlacement base;
    u8 pad18[0x1A - 0x18];
    s16 itemIndex; /* 0x1A: shop item slot this beam marks */
    u8 pad1C[0x20 - 0x1C];
} SpitembeamPlacement;

STATIC_ASSERT(sizeof(SpitembeamPlacement) == 0x20);

int spitembeam_getExtraSize(void);
int spitembeam_getObjectTypeId(void);
void spitembeam_free(void);
void spitembeam_render(void);
void spitembeam_hitDetect(void);
void spitembeam_update(GameObject* obj);
void spitembeam_init(GameObject* obj);
void spitembeam_release(void);
void spitembeam_initialise(void);

#endif /* MAIN_DLL_SP_DLL_0289_SPITEMBEAM_H_ */
