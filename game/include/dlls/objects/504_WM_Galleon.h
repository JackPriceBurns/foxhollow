#ifndef DLLS_OBJECTS_504_WM_GALLEON_H_
#define DLLS_OBJECTS_504_WM_GALLEON_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

#define WM_GALLEON_OBJECT_ID 0x139

typedef struct WMGalleonSetup {
    ObjPlacement base;
    s8 rotationXByte;
    u8 unknown19;
    s16 unknown1A;
    u8 unknown1C[2];
    s16 unknown1E;
    u8 unknown20[4];
} WMGalleonSetup;

STATIC_ASSERT(offsetof(WMGalleonSetup, base) == 0x00);
STATIC_ASSERT(offsetof(WMGalleonSetup, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(WMGalleonSetup, unknown19) == 0x19);
STATIC_ASSERT(offsetof(WMGalleonSetup, unknown1A) == 0x1A);
STATIC_ASSERT(offsetof(WMGalleonSetup, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(WMGalleonSetup, unknown1E) == 0x1E);
STATIC_ASSERT(offsetof(WMGalleonSetup, unknown20) == 0x20);
STATIC_ASSERT(sizeof(WMGalleonSetup) == 0x24);

typedef struct WMGalleonState {
    f32 savedX;
    f32 savedY;
    f32 savedZ;
    u8 mapEventsLatched;
    u8 unknown0D;
    s16 savedRotationX;
} WMGalleonState;

STATIC_ASSERT(offsetof(WMGalleonState, savedX) == 0x00);
STATIC_ASSERT(offsetof(WMGalleonState, savedY) == 0x04);
STATIC_ASSERT(offsetof(WMGalleonState, savedZ) == 0x08);
STATIC_ASSERT(offsetof(WMGalleonState, mapEventsLatched) == 0x0C);
STATIC_ASSERT(offsetof(WMGalleonState, unknown0D) == 0x0D);
STATIC_ASSERT(offsetof(WMGalleonState, savedRotationX) == 0x0E);
STATIC_ASSERT(sizeof(WMGalleonState) == 0x10);

int WM_Galleon_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int WM_Galleon_getExtraSize(void);
int WM_Galleon_getObjectTypeId(void);
void WM_Galleon_free(GameObject* obj, int leavingMap);
void WM_Galleon_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void WM_Galleon_hitDetect(void);
void WM_Galleon_update(GameObject* obj);
void WM_Galleon_init(GameObject* obj, const WMGalleonSetup* setup);
void WM_Galleon_release(void);
void WM_Galleon_initialise(void);

extern ObjectDescriptor gWM_GalleonObjDescriptor;

#endif /* DLLS_OBJECTS_504_WM_GALLEON_H_ */
