#ifndef DLLS_OBJECTS_275_H_
#define DLLS_OBJECTS_275_H_

#include "dlls/object_descriptor.h"
#include "dlls/objects/274.h"

#define SEQ_OBJ2_STATE_SIZE 0x1

typedef struct SeqObj2State {
    u8 flags;
} SeqObj2State;

STATIC_ASSERT(offsetof(SeqObj2State, flags) == 0x0);
STATIC_ASSERT(sizeof(SeqObj2State) == SEQ_OBJ2_STATE_SIZE);

int SeqObj2_getExtraSize(void);
int SeqObj2_getObjectTypeId(void);
void SeqObj2_free(GameObject* obj);
void SeqObj2_render(void);
void SeqObj2_hitDetect(void);
void SeqObj2_update(GameObject* obj);
void SeqObj2_init(GameObject* obj, SeqObjectPlacement* placement);
void SeqObj2_release(void);
void SeqObj2_initialise(void);

extern ObjectDescriptor gSeqObj2ObjDescriptor;

#endif /* DLLS_OBJECTS_275_H_ */
