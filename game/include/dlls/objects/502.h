#ifndef DLLS_OBJECTS_502_H_
#define DLLS_OBJECTS_502_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/*
 * Only the placement prefix consumed by this DLL is modeled. The active-target
 * retail placement width is not yet evidenced.
 */
typedef struct Dll1F6PlacementView {
    ObjPlacement base;
    s8 rotXByte;
} Dll1F6PlacementView;

STATIC_ASSERT(offsetof(Dll1F6PlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(Dll1F6PlacementView, rotXByte) == 0x18);

int dll502_getExtraSize(void);
int dll502_getObjectTypeId(void);
void dll502_free(void);
void dll502_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll502_hitDetect(void);
void dll502_update(GameObject* obj);
void dll502_init(GameObject* obj, const Dll1F6PlacementView* placement);
void dll502_release(void);
void dll502_initialise(void);

extern ObjectDescriptor gDll1F6ObjDescriptor;

#endif /* DLLS_OBJECTS_502_H_ */
