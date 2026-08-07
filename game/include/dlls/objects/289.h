#ifndef DLLS_OBJECTS_289_H_
#define DLLS_OBJECTS_289_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define INFO_TEXT_PLACEMENT_SIZE 0x1C
#define INFO_TEXT_STATE_SIZE     0x4

/* Retail DLL 0x121 InfoText placements establish this fixed seven-word record. */
typedef struct InfoTextPlacement {
    ObjPlacement base; /* 0x00 */
    u8 rotationX;      /* 0x18: high byte of the initial X rotation */
    u8 hintTextIndex;  /* 0x19 */
    u8 pad1A[2];       /* 0x1A */
} InfoTextPlacement;

typedef struct InfoTextState {
    f32 displayTimer; /* 0x00 */
} InfoTextState;

STATIC_ASSERT(offsetof(InfoTextPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(InfoTextPlacement, rotationX) == 0x18);
STATIC_ASSERT(offsetof(InfoTextPlacement, hintTextIndex) == 0x19);
STATIC_ASSERT(offsetof(InfoTextPlacement, pad1A) == 0x1A);
STATIC_ASSERT(sizeof(InfoTextPlacement) == INFO_TEXT_PLACEMENT_SIZE);

STATIC_ASSERT(offsetof(InfoTextState, displayTimer) == 0x0);
STATIC_ASSERT(sizeof(InfoTextState) == INFO_TEXT_STATE_SIZE);

int infotext_getExtraSize(void);
void infotext_update(GameObject* obj);
void infotext_init(GameObject* obj, InfoTextPlacement* placement);

extern ObjectDescriptor gInfoTextObjDescriptor;

#endif /* DLLS_OBJECTS_289_H_ */
