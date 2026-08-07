#ifndef DLLS_OBJECTS_347_CFFORCEFIEL_H_
#define DLLS_OBJECTS_347_CFFORCEFIEL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define CFFORCEFIELD_EMITTER_COUNT 3

typedef struct CfForceFieldStatusFlags {
    u8 disabled : 1;
    u8 unused : 7;
} CfForceFieldStatusFlags;

typedef struct CfForceFieldState {
    CfForceFieldStatusFlags statusFlags;
    u8 pad01[0x03];
    f32 collapseTimer;
} CfForceFieldState;

typedef struct CfForceFieldPlacement {
    ObjPlacement base;
    s8 rotXByte;
    s8 effectStyle;
    s16 unknown1A;
    u8 pad1C[0x02];
    s16 activeGameBit;
    s16 collapseGameBit;
    u8 pad22[0x02];
} CfForceFieldPlacement;

typedef struct CfForceFieldEmitterConfig {
    int spiralEffectId;
    int burstEffectId;
    int spiralAngleStep;
    int burstCount;
    int spinStep;
    f32 spiralPhaseSpeed;
} CfForceFieldEmitterConfig;

STATIC_ASSERT(sizeof(CfForceFieldStatusFlags) == 0x01);

STATIC_ASSERT(offsetof(CfForceFieldState, statusFlags) == 0x00);
STATIC_ASSERT(offsetof(CfForceFieldState, pad01) == 0x01);
STATIC_ASSERT(offsetof(CfForceFieldState, collapseTimer) == 0x04);
STATIC_ASSERT(sizeof(CfForceFieldState) == 0x08);

STATIC_ASSERT(offsetof(CfForceFieldPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CfForceFieldPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(CfForceFieldPlacement, effectStyle) == 0x19);
STATIC_ASSERT(offsetof(CfForceFieldPlacement, unknown1A) == 0x1A);
STATIC_ASSERT(offsetof(CfForceFieldPlacement, pad1C) == 0x1C);
STATIC_ASSERT(offsetof(CfForceFieldPlacement, activeGameBit) == 0x1E);
STATIC_ASSERT(offsetof(CfForceFieldPlacement, collapseGameBit) == 0x20);
STATIC_ASSERT(offsetof(CfForceFieldPlacement, pad22) == 0x22);
STATIC_ASSERT(sizeof(CfForceFieldPlacement) == 0x24);

STATIC_ASSERT(offsetof(CfForceFieldEmitterConfig, spiralEffectId) == 0x00);
STATIC_ASSERT(offsetof(CfForceFieldEmitterConfig, burstEffectId) == 0x04);
STATIC_ASSERT(offsetof(CfForceFieldEmitterConfig, spiralAngleStep) == 0x08);
STATIC_ASSERT(offsetof(CfForceFieldEmitterConfig, burstCount) == 0x0C);
STATIC_ASSERT(offsetof(CfForceFieldEmitterConfig, spinStep) == 0x10);
STATIC_ASSERT(offsetof(CfForceFieldEmitterConfig, spiralPhaseSpeed) == 0x14);
STATIC_ASSERT(sizeof(CfForceFieldEmitterConfig) == 0x18);

int cfforcefield_getExtraSize(void);
int cfforcefield_getObjectTypeId(void);
void cfforcefield_free(void);
void cfforcefield_render(void);
void cfforcefield_hitDetect(void);
void cfforcefield_update(GameObject* obj);
void cfforcefield_init(GameObject* obj, CfForceFieldPlacement* placement);
void cfforcefield_release(void);
void cfforcefield_initialise(void);

extern f32 gCfForceFieldRingRadiusScale;
extern int gCfForceFieldRingJitter;
extern int gCfForceFieldCollapseSpinStep;
extern CfForceFieldEmitterConfig gCfForceFieldEmitters[CFFORCEFIELD_EMITTER_COUNT];
extern ObjectDescriptor gCFForceFieldObjDescriptor;

#endif /* DLLS_OBJECTS_347_CFFORCEFIEL_H_ */
