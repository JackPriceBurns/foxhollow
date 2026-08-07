#ifndef DLLS_OBJECTS_351_H_
#define DLLS_OBJECTS_351_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef enum AttractorMode {
    ATTRACTOR_MODE_NONE = 0,
    ATTRACTOR_MODE_RETURN_SELF = 1,
    ATTRACTOR_MODE_FACE_PLAYER = 2,
} AttractorMode;

typedef struct AttractorPlacement {
    ObjPlacement base;
    s8 rotXByte;
    s8 mode; /* AttractorMode */
    s16 unknown1A;
} AttractorPlacement;

STATIC_ASSERT(offsetof(AttractorPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(AttractorPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(AttractorPlacement, mode) == 0x19);
STATIC_ASSERT(offsetof(AttractorPlacement, unknown1A) == 0x1A);

void attractor_getTarget(GameObject* obj, GameObject** outTarget);
int attractor_getScale(GameObject* obj);
int attractor_getExtraSize(void);
int attractor_getObjectTypeId(void);
void attractor_free(GameObject* obj);
void attractor_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void attractor_hitDetect(void);
void attractor_update(void);
void attractor_init(GameObject* obj, AttractorPlacement* placement);
void attractor_release(void);
void attractor_initialise(void);

extern ObjectDescriptor12 gAttractorObjDescriptor;

#endif /* DLLS_OBJECTS_351_H_ */
