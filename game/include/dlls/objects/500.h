#ifndef DLLS_OBJECTS_500_H_
#define DLLS_OBJECTS_500_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

/* dll500_getExtraSize() allocates this complete 0x01-byte state. */
typedef struct Dll1F4State {
    u8 active;
} Dll1F4State;

/*
 * Only the placement prefix consumed by this DLL is modeled. The active-target
 * retail placement width is not yet evidenced.
 */
typedef struct Dll1F4PlacementView {
    ObjPlacement base;
    s8 rotXSwing;
    u8 unknown19;
    u8 rotXStatic;
} Dll1F4PlacementView;

STATIC_ASSERT(offsetof(Dll1F4State, active) == 0x00);
STATIC_ASSERT(sizeof(Dll1F4State) == 0x01);

STATIC_ASSERT(offsetof(Dll1F4PlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(Dll1F4PlacementView, rotXSwing) == 0x18);
STATIC_ASSERT(offsetof(Dll1F4PlacementView, unknown19) == 0x19);
STATIC_ASSERT(offsetof(Dll1F4PlacementView, rotXStatic) == 0x1A);

int dll500_getExtraSize(void);
void dll500_free(GameObject* obj);
void dll500_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
int dll500_processAnimEvents(GameObject* obj, int unused, ObjSeqState* animUpdate);
void dll500_update(GameObject* obj);
void dll500_init(GameObject* obj, const Dll1F4PlacementView* placement);

extern ObjectDescriptor gDll1F4ObjDescriptor;

#endif /* DLLS_OBJECTS_500_H_ */
