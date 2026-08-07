#ifndef DLLS_OBJECTS_422_SH_TRICKY_H_
#define DLLS_OBJECTS_422_SH_TRICKY_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

typedef enum ShTrickyPhase {
    SH_TRICKY_PHASE_WAIT_TRIGGER = 0,
    SH_TRICKY_PHASE_REQUEST_DELAY = 1,
    SH_TRICKY_PHASE_REQUEST_MOVE = 2,
    SH_TRICKY_PHASE_WAIT_RETURN_TO_QUEEN = 3,
    SH_TRICKY_PHASE_COMPLETE = 4,
} ShTrickyPhase;

typedef struct ShTrickyState {
    u8 phase;
} ShTrickyState;

STATIC_ASSERT(sizeof(ShTrickyState) == 0x01);
STATIC_ASSERT(offsetof(ShTrickyState, phase) == 0x00);

extern ObjectDescriptor gSHTrickyObjDescriptor;

int shTricky_getExtraSize(void);
void shTricky_update(GameObject* obj);
void shTricky_init(GameObject* obj);

#endif /* DLLS_OBJECTS_422_SH_TRICKY_H_ */
