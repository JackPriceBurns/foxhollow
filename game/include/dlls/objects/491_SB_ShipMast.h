#ifndef DLLS_OBJECTS_491_SB_SHIPMAST_H_
#define DLLS_OBJECTS_491_SB_SHIPMAST_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

int SB_ShipMast_getExtraSize(void);
int SB_ShipMast_getObjectTypeId(void);
void SB_ShipMast_free(void);
void SB_ShipMast_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void SB_ShipMast_hitDetect(void);
void SB_ShipMast_update(GameObject* obj);
void SB_ShipMast_init(void);
void SB_ShipMast_release(void);
void SB_ShipMast_initialise(void);

extern ObjectDescriptor gSB_ShipMastObjDescriptor;

#endif /* DLLS_OBJECTS_491_SB_SHIPMAST_H_ */
