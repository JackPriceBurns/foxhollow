#ifndef DLLS_OBJECTS_214_KALDACHOMME_H_
#define DLLS_OBJECTS_214_KALDACHOMME_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct KaldachomMePlacement {
    ObjPlacement base; /* 0x00 */
    u8 rotZByte;       /* 0x18 */
    u8 rotYByte;       /* 0x19 */
    u8 rotXByte;       /* 0x1A */
    u8 pad1B;          /* 0x1B */
} KaldachomMePlacement;

typedef enum KaldachomMeLinkedMode {
    KALDACHOMME_LINKED_MODE_MOVE_0 = 1,
    KALDACHOMME_LINKED_MODE_MOVE_1 = 2,
} KaldachomMeLinkedMode;

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
