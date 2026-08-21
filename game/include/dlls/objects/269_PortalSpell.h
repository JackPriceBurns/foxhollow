#ifndef DLLS_OBJECTS_269_PORTALSPELL_H_
#define DLLS_OBJECTS_269_PORTALSPELL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct PortalSpellDoorPlacement {
    ObjPlacement base;
    s8 rotXByte;
    u8 unknown19[3];
    s16 rotY;
    s16 openedGameBit;
} PortalSpellDoorPlacement;

STATIC_ASSERT(offsetof(PortalSpellDoorPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(PortalSpellDoorPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(PortalSpellDoorPlacement, unknown19) == 0x19);
STATIC_ASSERT(offsetof(PortalSpellDoorPlacement, rotY) == 0x1C);
STATIC_ASSERT(offsetof(PortalSpellDoorPlacement, openedGameBit) == 0x1E);

int PortalSpellDoor_getExtraSize(void);
int PortalSpellDoor_getObjectTypeId(void);
void PortalSpellDoor_free(GameObject* obj);
void PortalSpellDoor_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void PortalSpellDoor_hitDetect(void);
void PortalSpellDoor_update(GameObject* obj);
void PortalSpellDoor_init(GameObject* obj, const PortalSpellDoorPlacement* placement);
void PortalSpellDoor_release(void);
void PortalSpellDoor_initialise(void);

extern ObjectDescriptor gPortalSpellDoorObjDescriptor;

#endif /* DLLS_OBJECTS_269_PORTALSPELL_H_ */
