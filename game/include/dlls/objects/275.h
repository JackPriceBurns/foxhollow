#ifndef DLLS_OBJECTS_275_H_
#define DLLS_OBJECTS_275_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define SEQ_OBJ2_STATE_SIZE 0x1

typedef struct SeqObj2Placement {
    ObjPlacement base;
    s16 usedGameBit;
    s16 requiredGameBit;
    u8 initialYaw;
    u8 flags;
    s8 sequenceId;
    u8 modelBankIndex;
    s16 preemptSequenceId;
    u16 sequenceParam;
} SeqObj2Placement;

typedef struct SeqObj2State {
    u8 flags;
} SeqObj2State;

STATIC_ASSERT(offsetof(SeqObj2State, flags) == 0x0);
STATIC_ASSERT(sizeof(SeqObj2State) == SEQ_OBJ2_STATE_SIZE);
STATIC_ASSERT(offsetof(SeqObj2Placement, base) == 0x0);
STATIC_ASSERT(offsetof(SeqObj2Placement, usedGameBit) == 0x18);
STATIC_ASSERT(offsetof(SeqObj2Placement, requiredGameBit) == 0x1A);
STATIC_ASSERT(offsetof(SeqObj2Placement, initialYaw) == 0x1C);
STATIC_ASSERT(offsetof(SeqObj2Placement, flags) == 0x1D);
STATIC_ASSERT(offsetof(SeqObj2Placement, sequenceId) == 0x1E);
STATIC_ASSERT(offsetof(SeqObj2Placement, modelBankIndex) == 0x1F);
STATIC_ASSERT(offsetof(SeqObj2Placement, preemptSequenceId) == 0x20);
STATIC_ASSERT(offsetof(SeqObj2Placement, sequenceParam) == 0x22);

int SeqObj2_getExtraSize(void);
int SeqObj2_getObjectTypeId(void);
void SeqObj2_free(GameObject* obj);
void SeqObj2_render(void);
void SeqObj2_hitDetect(void);
void SeqObj2_update(GameObject* obj);
void SeqObj2_init(GameObject* obj, SeqObj2Placement* placement);
void SeqObj2_release(void);
void SeqObj2_initialise(void);

extern ObjectDescriptor gSeqObj2ObjDescriptor;

#endif /* DLLS_OBJECTS_275_H_ */
