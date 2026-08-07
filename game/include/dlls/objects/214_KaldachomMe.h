#ifndef DLLS_OBJECTS_214_KALDACHOMME_H_
#define DLLS_OBJECTS_214_KALDACHOMME_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct KaldachomMeState {
    f32 progress;       /* 0x00 */
    f32 step;           /* 0x04 */
    f32 targetProgress; /* 0x08 */
    u8 moveId;          /* 0x0C */
    u8 pad0D[3];        /* 0x0D */
} KaldachomMeState;

typedef struct KaldachomMePlacement {
    ObjPlacement base; /* 0x00 */
    u8 rotZByte;       /* 0x18 */
    u8 rotYByte;       /* 0x19 */
    u8 rotXByte;       /* 0x1A */
    u8 pad1B;          /* 0x1B */
} KaldachomMePlacement;

typedef u8 KaldachomMeLinkedMode;

enum {
    KALDACHOMME_LINKED_MODE_MOVE_0 = 1,
    KALDACHOMME_LINKED_MODE_MOVE_1 = 2
};

STATIC_ASSERT(offsetof(KaldachomMeState, progress) == 0x0);
STATIC_ASSERT(offsetof(KaldachomMeState, step) == 0x4);
STATIC_ASSERT(offsetof(KaldachomMeState, targetProgress) == 0x8);
STATIC_ASSERT(offsetof(KaldachomMeState, moveId) == 0xC);
STATIC_ASSERT(sizeof(KaldachomMeState) == 0x10);

STATIC_ASSERT(offsetof(KaldachomMePlacement, base) == 0x0);
STATIC_ASSERT(offsetof(KaldachomMePlacement, rotZByte) == 0x18);
STATIC_ASSERT(offsetof(KaldachomMePlacement, rotYByte) == 0x19);
STATIC_ASSERT(offsetof(KaldachomMePlacement, rotXByte) == 0x1A);
STATIC_ASSERT(sizeof(KaldachomMePlacement) == 0x1C);

void kaldachomme_setLinkedMouthMode(GameObject* obj, KaldachomMeLinkedMode mode);
int KaldachomMe_getExtraSize(void);
int KaldachomMe_getObjectTypeId(void);
void KaldachomMe_free(GameObject* obj);
void KaldachomMe_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void KaldachomMe_hitDetect(GameObject* obj);
void KaldachomMe_update(GameObject* obj);
void KaldachomMe_init(GameObject* obj, KaldachomMePlacement* placement);
void KaldachomMe_release(void);
void KaldachomMe_initialise(void);

extern ObjectDescriptor gKaldachomMeObjDescriptor;

#endif /* DLLS_OBJECTS_214_KALDACHOMME_H_ */
