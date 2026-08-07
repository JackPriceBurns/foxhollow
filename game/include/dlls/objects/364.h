#ifndef DLLS_OBJECTS_364_H_
#define DLLS_OBJECTS_364_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

#define IM_SNOW_CLAW_DROP_OBJECT_COUNT 5

typedef struct IMSnowClawPlacement {
    ObjPlacement base;
    u8 pad18[0x0F];
    s8 dropObjectIndex;
    u8 pad28[0x10];
} IMSnowClawPlacement;

typedef struct IMSnowClawState {
    GameObject* mount;
    f32 unknown04;
    f32 mountSnapX;
    f32 mountSnapY;
    f32 mountSnapZ;
    f32 pathPointX;
    f32 pathPointY;
    f32 pathPointZ;
    u8 mountAlpha;
    s8 dropObjectIndex;
    s8 appliedDropObjectIndex;
    u8 pad23;
} IMSnowClawState;

typedef struct IMSnowClawMountInterface {
    void* pad00[4];
    void (*render)(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, int visible);
    void* pad14[5];
    void (*getPosition)(GameObject* obj, f32* x, f32* y, f32* z);
    void* pad2C[3];
    int (*getRiderMode)(GameObject* obj);
    void (*setRiderMode)(GameObject* obj, int mode);
    void (*func12)(GameObject* obj, f32* out, int* outNegative);
    f32 (*getNormalizedSpeed)(GameObject* obj, f32* out);
} IMSnowClawMountInterface;

typedef struct IMSnowClawDropObjectTable {
    s16 objectIds[IM_SNOW_CLAW_DROP_OBJECT_COUNT];
} IMSnowClawDropObjectTable;

STATIC_ASSERT(offsetof(IMSnowClawPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(IMSnowClawPlacement, pad18) == 0x18);
STATIC_ASSERT(offsetof(IMSnowClawPlacement, dropObjectIndex) == 0x27);
STATIC_ASSERT(offsetof(IMSnowClawPlacement, pad28) == 0x28);
STATIC_ASSERT(sizeof(IMSnowClawPlacement) == 0x38);

STATIC_ASSERT(offsetof(IMSnowClawState, mount) == 0x00);
STATIC_ASSERT(offsetof(IMSnowClawState, unknown04) == 0x04);
STATIC_ASSERT(offsetof(IMSnowClawState, mountSnapX) == 0x08);
STATIC_ASSERT(offsetof(IMSnowClawState, mountSnapY) == 0x0C);
STATIC_ASSERT(offsetof(IMSnowClawState, mountSnapZ) == 0x10);
STATIC_ASSERT(offsetof(IMSnowClawState, pathPointX) == 0x14);
STATIC_ASSERT(offsetof(IMSnowClawState, pathPointY) == 0x18);
STATIC_ASSERT(offsetof(IMSnowClawState, pathPointZ) == 0x1C);
STATIC_ASSERT(offsetof(IMSnowClawState, mountAlpha) == 0x20);
STATIC_ASSERT(offsetof(IMSnowClawState, dropObjectIndex) == 0x21);
STATIC_ASSERT(offsetof(IMSnowClawState, appliedDropObjectIndex) == 0x22);
STATIC_ASSERT(offsetof(IMSnowClawState, pad23) == 0x23);
STATIC_ASSERT(sizeof(IMSnowClawState) == 0x24);

STATIC_ASSERT(offsetof(IMSnowClawMountInterface, pad00) == 0x00);
STATIC_ASSERT(offsetof(IMSnowClawMountInterface, render) == 0x10);
STATIC_ASSERT(offsetof(IMSnowClawMountInterface, pad14) == 0x14);
STATIC_ASSERT(offsetof(IMSnowClawMountInterface, getPosition) == 0x28);
STATIC_ASSERT(offsetof(IMSnowClawMountInterface, pad2C) == 0x2C);
STATIC_ASSERT(offsetof(IMSnowClawMountInterface, getRiderMode) == 0x38);
STATIC_ASSERT(offsetof(IMSnowClawMountInterface, setRiderMode) == 0x3C);
STATIC_ASSERT(offsetof(IMSnowClawMountInterface, func12) == 0x40);
STATIC_ASSERT(offsetof(IMSnowClawMountInterface, getNormalizedSpeed) == 0x44);
STATIC_ASSERT(sizeof(IMSnowClawMountInterface) == 0x48);

STATIC_ASSERT(offsetof(IMSnowClawDropObjectTable, objectIds) == 0x00);
STATIC_ASSERT(sizeof(IMSnowClawDropObjectTable) == 0x0A);

void imSnowClaw_syncMountTransform(GameObject* obj, GameObject* mount, int renderArg2, int renderArg3, int renderArg4,
                                   int renderArg5, int visible, int mountAlpha, int renderMount);
int imSnowClaw_sequenceCallback(GameObject* obj, int unusedArg2, ObjSeqState* animUpdate);
int imSnowClaw_getExtraSize(void);
int imSnowClaw_getObjectTypeId(void);
void imSnowClaw_free(GameObject* obj);
void imSnowClaw_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void imSnowClaw_hitDetect(GameObject* obj);
void imSnowClaw_update(GameObject* obj);
void imSnowClaw_init(GameObject* obj, IMSnowClawPlacement* placement);
void imSnowClaw_release(void);
void imSnowClaw_initialise(void);

extern const IMSnowClawDropObjectTable gIMSnowClawDropObjectTable;
extern ObjectDescriptor gIMSnowClawObjDescriptor;

#endif /* DLLS_OBJECTS_364_H_ */
