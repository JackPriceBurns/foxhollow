#ifndef DLLS_OBJECTS_345_H_
#define DLLS_OBJECTS_345_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct BlastedTargetPlacement {
    ObjPlacement base;
    s8 rotXByte;
    u8 pad19;
    s16 pieceCount;
    s16 mapLayerId;
    s16 completedGameBit;
    s16 progressGameBit;
    u8 pad22[0x02];
} BlastedTargetPlacement;

typedef struct BlastedTargetState {
    u32 destroyedHitObjects[3];
    int mapLayerActivated;
    u8 pieceCount;
    u8 damageStage;
    u8 pad12[0x02];
} BlastedTargetState;

STATIC_ASSERT(offsetof(BlastedTargetPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(BlastedTargetPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(BlastedTargetPlacement, pad19) == 0x19);
STATIC_ASSERT(offsetof(BlastedTargetPlacement, pieceCount) == 0x1A);
STATIC_ASSERT(offsetof(BlastedTargetPlacement, mapLayerId) == 0x1C);
STATIC_ASSERT(offsetof(BlastedTargetPlacement, completedGameBit) == 0x1E);
STATIC_ASSERT(offsetof(BlastedTargetPlacement, progressGameBit) == 0x20);
STATIC_ASSERT(offsetof(BlastedTargetPlacement, pad22) == 0x22);
STATIC_ASSERT(sizeof(BlastedTargetPlacement) == 0x24);

STATIC_ASSERT(offsetof(BlastedTargetState, destroyedHitObjects) == 0x00);
STATIC_ASSERT(offsetof(BlastedTargetState, mapLayerActivated) == 0x0C);
STATIC_ASSERT(offsetof(BlastedTargetState, pieceCount) == 0x10);
STATIC_ASSERT(offsetof(BlastedTargetState, damageStage) == 0x11);
STATIC_ASSERT(offsetof(BlastedTargetState, pad12) == 0x12);
STATIC_ASSERT(sizeof(BlastedTargetState) == 0x14);

int blasted_activateMapLayer(GameObject* obj, int mapLayerId);
int blasted_getExtraSize(void);
int blasted_getObjectTypeId(void);
void blasted_free(void);
void blasted_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void blasted_hitDetect(void);
void blasted_update(GameObject* obj);
void blasted_init(GameObject* obj, BlastedTargetPlacement* placement);
void blasted_release(void);
void blasted_initialise(void);

extern int gBlastedDamageTimer;
extern ObjectDescriptor gBlastedObjDescriptor;

#endif /* DLLS_OBJECTS_345_H_ */
