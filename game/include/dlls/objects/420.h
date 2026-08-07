#ifndef DLLS_OBJECTS_420_H_
#define DLLS_OBJECTS_420_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define NW_ICE_OBJECT_GROUP_ID 0x3C

typedef struct NwIcePlacement {
    ObjPlacement base;
    u8 unknown18[3];
    u8 pairId;
    u8 unknown1C[4];
} NwIcePlacement;

typedef struct NwIceState {
    GameObject* pairedIceObject;
} NwIceState;

STATIC_ASSERT(sizeof(NwIcePlacement) == 0x20);
STATIC_ASSERT(offsetof(NwIcePlacement, pairId) == 0x1B);
STATIC_ASSERT(sizeof(NwIceState) == 0x4);
STATIC_ASSERT(offsetof(NwIceState, pairedIceObject) == 0x0);

int NW_ice_getExtraSize(void);
void NW_ice_free(GameObject* obj);
void NW_ice_render(void);
void NW_ice_update(GameObject* obj);
void NW_ice_init(GameObject* obj);

extern ObjectDescriptor gNW_iceObjDescriptor;

#endif /* DLLS_OBJECTS_420_H_ */
