#ifndef DLLS_OBJECTS_258_STAYPOINT_H_
#define DLLS_OBJECTS_258_STAYPOINT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* Known setup-record prefix; the complete retail record width is not proven. */
typedef struct StayPointPlacement {
    ObjPlacement base;   /* 0x00 */
    u8 pad18[6];         /* 0x18 */
    s16 activeGameBit;   /* 0x1E: set while Tricky stays here; -1 = unused */
    s16 requiredGameBit; /* 0x20: enables this stay point; -1 = always */
} StayPointPlacement;

STATIC_ASSERT(offsetof(StayPointPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(StayPointPlacement, pad18) == 0x18);
STATIC_ASSERT(offsetof(StayPointPlacement, activeGameBit) == 0x1E);
STATIC_ASSERT(offsetof(StayPointPlacement, requiredGameBit) == 0x20);

void StayPoint_update(GameObject* obj);
void StayPoint_init(GameObject* obj);

extern ObjectDescriptor gStayPointObjDescriptor;

#endif /* DLLS_OBJECTS_258_STAYPOINT_H_ */
