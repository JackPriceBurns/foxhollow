#ifndef DLLS_OBJECTS_519_WM_WORM_H_
#define DLLS_OBJECTS_519_WM_WORM_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

typedef struct WmWormPlacement WmWormPlacement;

int WM_Worm_getExtraSize(void);
int WM_Worm_getObjectTypeId(void);
void WM_Worm_free(GameObject* obj);
void WM_Worm_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void WM_Worm_hitDetect(void);
void WM_Worm_update(GameObject* obj);
void WM_Worm_init(GameObject* obj, const WmWormPlacement* placement);
void WM_Worm_release(void);
void WM_Worm_initialise(void);

extern ObjectDescriptor gWM_WormObjDescriptor;

#endif
