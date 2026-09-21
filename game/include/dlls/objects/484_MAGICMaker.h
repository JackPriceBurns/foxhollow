#ifndef DLLS_OBJECTS_484_MAGICMAKER_H_
#define DLLS_OBJECTS_484_MAGICMAKER_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

#define MAGICMAKER_SPAWN_OBJECT_COUNT 6

int magicmaker_getExtraSize(void);
int magicmaker_getObjectTypeId(void);
void magicmaker_free(void);
void magicmaker_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void magicmaker_hitDetect(void);
void magicmaker_update(GameObject* obj);
void magicmaker_init(void);
void magicmaker_release(void);
void magicmaker_initialise(void);

extern u16 gMagicMakerSpawnObjectIds[MAGICMAKER_SPAWN_OBJECT_COUNT];
extern ObjectDescriptor10WithPadding gMAGICMakerObjDescriptor;

#endif /* DLLS_OBJECTS_484_MAGICMAKER_H_ */
