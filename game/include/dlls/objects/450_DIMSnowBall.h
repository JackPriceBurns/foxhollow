#ifndef DLLS_OBJECTS_450_DIMSNOWBALL_H_
#define DLLS_OBJECTS_450_DIMSNOWBALL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct DimSnowBallSpawnerPlacement DimSnowBallSpawnerPlacement;

int dimsnowball1c2_getExtraSize(void);
int dimsnowball1c2_getObjectTypeId(void);
void dimsnowball1c2_free(void);
void dimsnowball1c2_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dimsnowball1c2_hitDetect(void);
void dimsnowball1c2_update(GameObject* obj);
void dimsnowball1c2_init(GameObject* obj, const DimSnowBallSpawnerPlacement* placement);
void dimsnowball1c2_release(void);
void dimsnowball1c2_initialise(void);

extern ObjectDescriptor gDIMSnowBall1C2ObjDescriptor;

#endif /* DLLS_OBJECTS_450_DIMSNOWBALL_H_ */
