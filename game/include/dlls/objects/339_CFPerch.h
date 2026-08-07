#ifndef DLLS_OBJECTS_339_CFPERCH_H_
#define DLLS_OBJECTS_339_CFPERCH_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/objseq.h"

int cfPerch_sequenceCallback(GameObject* obj, int unused, ObjSeqState* animUpdate);
int cfPerch_getExtraSize(void);
int cfPerch_getObjectTypeId(void);
void cfPerch_free(GameObject* obj);
void cfPerch_render(void);
void cfPerch_hitDetect(void);
void cfPerch_update(GameObject* obj);
void cfPerch_init(GameObject* obj);
void cfPerch_release(void);
void cfPerch_initialise(void);

extern ObjectDescriptor gCFPerchObjDescriptor;

#endif /* DLLS_OBJECTS_339_CFPERCH_H_ */
