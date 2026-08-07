#ifndef DLLS_OBJECTS_269_PORTALSPELL_H_
#define DLLS_OBJECTS_269_PORTALSPELL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define PORTAL_SPELL_DOOR_STATE_SIZE 0x10

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct PortalSpellDoorPlacement {
    ObjPlacement base; /* 0x00 */
    s8 rotXByte;       /* 0x18: X rotation in 1/256 turns */
    u8 pad19[3];       /* 0x19 */
    s16 rotY;          /* 0x1C: shifted by eight when converted to the runtime rotation */
    s16 openedGameBit; /* 0x1E */
} PortalSpellDoorPlacement;

typedef struct PortalSpellDoorFlags {
    u8 open : 1;
    u8 unused : 7;
} PortalSpellDoorFlags;

typedef struct PortalSpellDoorState {
    u8 pad00[4];                /* 0x00 */
    f32 openAmount;             /* 0x04 */
    s32 openTimer;              /* 0x08 */
    PortalSpellDoorFlags flags; /* 0x0C */
    u8 pad0D[3];                /* 0x0D */
} PortalSpellDoorState;

STATIC_ASSERT(offsetof(PortalSpellDoorPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(PortalSpellDoorPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(PortalSpellDoorPlacement, pad19) == 0x19);
STATIC_ASSERT(offsetof(PortalSpellDoorPlacement, rotY) == 0x1C);
STATIC_ASSERT(offsetof(PortalSpellDoorPlacement, openedGameBit) == 0x1E);

STATIC_ASSERT(sizeof(PortalSpellDoorFlags) == 0x1);

STATIC_ASSERT(offsetof(PortalSpellDoorState, pad00) == 0x0);
STATIC_ASSERT(offsetof(PortalSpellDoorState, openAmount) == 0x4);
STATIC_ASSERT(offsetof(PortalSpellDoorState, openTimer) == 0x8);
STATIC_ASSERT(offsetof(PortalSpellDoorState, flags) == 0xC);
STATIC_ASSERT(offsetof(PortalSpellDoorState, pad0D) == 0xD);
STATIC_ASSERT(sizeof(PortalSpellDoorState) == PORTAL_SPELL_DOOR_STATE_SIZE);

int PortalSpellDoor_getExtraSize(void);
int PortalSpellDoor_getObjectTypeId(void);
void PortalSpellDoor_free(GameObject* obj);
void PortalSpellDoor_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void PortalSpellDoor_hitDetect(void);
void PortalSpellDoor_update(GameObject* obj);
void PortalSpellDoor_init(GameObject* obj, PortalSpellDoorPlacement* placement);
void PortalSpellDoor_release(void);
void PortalSpellDoor_initialise(void);

extern ObjectDescriptor gPortalSpellDoorObjDescriptor;

#endif /* DLLS_OBJECTS_269_PORTALSPELL_H_ */
