#ifndef DLLS_OBJECTS_431_SH_SWAPLIFT_H_
#define DLLS_OBJECTS_431_SH_SWAPLIFT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

typedef struct WarpStoneLiftPlacement WarpStoneLiftPlacement;

extern ObjectDescriptor gWarpStoneLiftObjDescriptor;

int warpstonelift_getExtraSize(void);
int warpstonelift_getObjectTypeId(void);
void warpstonelift_free(void);
void warpstonelift_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void warpstonelift_hitDetect(void);
void warpstonelift_update(GameObject* obj);
void warpstonelift_init(GameObject* obj, const WarpStoneLiftPlacement* placement);
void warpstonelift_release(void);
void warpstonelift_initialise(void);

#endif
