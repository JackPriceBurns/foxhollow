#ifndef DLLS_OBJECTS_236_INFOPOINT_H_
#define DLLS_OBJECTS_236_INFOPOINT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

struct ObjSeqState;

typedef struct InfoPointPlacement {
    ObjPlacement base;
    u16 textId;
    u8 unknown1A;
    u8 unknown1B;
    u8 rotationXByte;
    u8 unknown1D[3];
} InfoPointPlacement;

STATIC_ASSERT(offsetof(InfoPointPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(InfoPointPlacement, textId) == 0x18);
STATIC_ASSERT(offsetof(InfoPointPlacement, unknown1B) == 0x1B);
STATIC_ASSERT(offsetof(InfoPointPlacement, rotationXByte) == 0x1C);
STATIC_ASSERT(sizeof(InfoPointPlacement) == 0x20);

int InfoPoint_SeqFn(GameObject* obj, int unused, struct ObjSeqState* animUpdate);
int InfoPoint_getExtraSize(void);
int InfoPoint_getObjectTypeId(void);
void InfoPoint_free(GameObject* obj);
void InfoPoint_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void InfoPoint_hitDetect(GameObject* obj);
void InfoPoint_update(GameObject* obj);
void InfoPoint_init(GameObject* obj, const InfoPointPlacement* placement);
void InfoPoint_release(void);
void InfoPoint_initialise(void);

extern ObjectDescriptor gInfoPointObjDescriptor;

#endif /* DLLS_OBJECTS_236_INFOPOINT_H_ */
