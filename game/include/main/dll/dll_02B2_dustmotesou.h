#ifndef MAIN_DLL_DUSTMOTESOU_H_
#define MAIN_DLL_DUSTMOTESOU_H_

#include "global.h"
#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

typedef struct DustMoteSourcePlacement DustMoteSourcePlacement;

extern ObjectDescriptor gDustMoteSouObjDescriptor;

int dustmotesou_getExtraSize(void);
int dustmotesou_getObjectTypeId(void);
void dustmotesou_free(GameObject* obj);
void dustmotesou_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible);
void dustmotesou_hitDetect(void);
void dustmotesou_update(GameObject* obj);
void dustmotesou_init(GameObject* obj, const DustMoteSourcePlacement* placement);
void dustmotesou_release(void);
void dustmotesou_initialise(void);

#endif
