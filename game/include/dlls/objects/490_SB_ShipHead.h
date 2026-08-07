#ifndef DLLS_OBJECTS_490_SB_SHIPHEAD_H_
#define DLLS_OBJECTS_490_SB_SHIPHEAD_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

typedef struct SBShipHeadState {
    GameObject* target; /* 0x00: galleon-side target object */
    s8 health;          /* 0x04: initialised to four */
    u8 unk05[3];
    f32 swayA; /* 0x08 */
    f32 swayB; /* 0x0C */
} SBShipHeadState;

STATIC_ASSERT(offsetof(SBShipHeadState, target) == 0x00);
STATIC_ASSERT(offsetof(SBShipHeadState, health) == 0x04);
STATIC_ASSERT(offsetof(SBShipHeadState, swayA) == 0x08);
STATIC_ASSERT(offsetof(SBShipHeadState, swayB) == 0x0C);
STATIC_ASSERT(sizeof(SBShipHeadState) == 0x10);

int SB_ShipHead_getExtraSize(void);
int SB_ShipHead_getObjectTypeId(void);
void SB_ShipHead_free(GameObject* obj);
void SB_ShipHead_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void SB_ShipHead_update(GameObject* obj);
void SB_ShipHead_init(GameObject* obj);

extern u8 gSbShipHeadHasFiredFireball;
extern int gSbShipHeadPrevGalleonPhase;
extern ObjectDescriptor gSB_ShipHeadObjDescriptor;

#endif /* DLLS_OBJECTS_490_SB_SHIPHEAD_H_ */
