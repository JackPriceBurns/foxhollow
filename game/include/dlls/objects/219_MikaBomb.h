#ifndef DLLS_OBJECTS_219_MIKABOMB_H_
#define DLLS_OBJECTS_219_MIKABOMB_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/dll/dll_005B_modgfx.h"

typedef struct MikaBombState {
    GameObject* shadowObj;            /* 0x00 */
    f32 groundY;                      /* 0x04 */
    Dll5BInterface** resource;        /* 0x08 */
    u8 exploded;                      /* 0x0C */
    u8 pad0D[3];                      /* 0x0D */
} MikaBombState;

STATIC_ASSERT(offsetof(MikaBombState, shadowObj) == 0x0);
STATIC_ASSERT(offsetof(MikaBombState, groundY) == 0x4);
STATIC_ASSERT(offsetof(MikaBombState, resource) == 0x8);
STATIC_ASSERT(offsetof(MikaBombState, exploded) == 0xC);
STATIC_ASSERT(sizeof(MikaBombState) == 0x10);

int MikaBomb_getExtraSize(void);
int MikaBomb_getObjectTypeId(void);
void MikaBomb_free(GameObject* obj, int mode);
void MikaBomb_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void MikaBomb_hitDetect(GameObject* obj);
void MikaBomb_update(GameObject* obj);
void MikaBomb_init(GameObject* obj);
void MikaBomb_release(void);
void MikaBomb_initialise(void);

extern ObjectDescriptor gMikaBombObjDescriptor;
extern const Dll5BSpawnCountRange gMikaBombExplosionSpawnCountRange;

#endif /* DLLS_OBJECTS_219_MIKABOMB_H_ */
