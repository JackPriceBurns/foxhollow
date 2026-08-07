#ifndef DLLS_OBJECTS_365_IMICEPILLAR_H_
#define DLLS_OBJECTS_365_IMICEPILLAR_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

int imIcePillar_getExtraSize(void);
int imIcePillar_getObjectTypeId(void);
void imIcePillar_free(void);
void imIcePillar_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void imIcePillar_hitDetect(void);
void imIcePillar_update(void);
void imIcePillar_init(void);
void imIcePillar_release(void);
void imIcePillar_initialise(void);

extern ObjectDescriptor gIMIcePillarObjDescriptor;

#endif /* DLLS_OBJECTS_365_IMICEPILLAR_H_ */
