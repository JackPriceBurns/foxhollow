#ifndef DLLS_OBJECTS_396_MMSH_SHRINE_H_
#define DLLS_OBJECTS_396_MMSH_SHRINE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "dlls/objects/430_SH_LevelCon.h"
#include "main/model_light.h"
#include "main/objseq.h"

typedef struct MMSHShrinePlacement {
    ObjPlacement base;
    u8 unknown18[0x1A - 0x18];
    s16 initialValue;
    u8 unknown1C[0x24 - 0x1C];
} MMSHShrinePlacement;

STATIC_ASSERT(sizeof(MMSHShrinePlacement) == 0x24);
STATIC_ASSERT(offsetof(MMSHShrinePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(MMSHShrinePlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(MMSHShrinePlacement, initialValue) == 0x1A);
STATIC_ASSERT(offsetof(MMSHShrinePlacement, unknown1C) == 0x1C);

typedef struct MMSHShrineState {
    ModelLightStruct* light;
    f32 swayPhase;
    f32 stickVelocity;
    f32 targetVelocity;
    f32 swayTarget;
    f32 idleSfxTimer;
    GameBitLatchState latch;
    s16 unknown1C;
    s16 orbitPhaseA;
    s16 orbitPhaseB;
    s16 orbitPhaseC;
    u8 phase;
    u8 unknown25[3];
} MMSHShrineState;

STATIC_ASSERT(sizeof(MMSHShrineState) == 0x28);
STATIC_ASSERT(offsetof(MMSHShrineState, light) == 0x00);
STATIC_ASSERT(offsetof(MMSHShrineState, swayPhase) == 0x04);
STATIC_ASSERT(offsetof(MMSHShrineState, stickVelocity) == 0x08);
STATIC_ASSERT(offsetof(MMSHShrineState, targetVelocity) == 0x0C);
STATIC_ASSERT(offsetof(MMSHShrineState, swayTarget) == 0x10);
STATIC_ASSERT(offsetof(MMSHShrineState, idleSfxTimer) == 0x14);
STATIC_ASSERT(offsetof(MMSHShrineState, latch) == 0x18);
STATIC_ASSERT(offsetof(MMSHShrineState, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(MMSHShrineState, orbitPhaseA) == 0x1E);
STATIC_ASSERT(offsetof(MMSHShrineState, orbitPhaseB) == 0x20);
STATIC_ASSERT(offsetof(MMSHShrineState, orbitPhaseC) == 0x22);
STATIC_ASSERT(offsetof(MMSHShrineState, phase) == 0x24);
STATIC_ASSERT(offsetof(MMSHShrineState, unknown25) == 0x25);

extern ObjectDescriptor gMMSHShrineObjDescriptor;

void mmshShrine_updateHoverMotion(GameObject* obj);
int mmshShrine_updateFearSway(GameObject* obj);
int mmshShrine_processAnimEvents(GameObject* obj, int unusedArg, ObjSeqState* animUpdate);
int mmshShrine_getExtraSize(void);
int mmshShrine_getObjectTypeId(void);
void mmshShrine_free(GameObject* obj);
void mmshShrine_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void mmshShrine_hitDetect(void);
void mmshShrine_update(GameObject* obj);
void mmshShrine_init(GameObject* obj, const MMSHShrinePlacement* placement);
void mmshShrine_release(void);
void mmshShrine_initialise(void);

#endif /* DLLS_OBJECTS_396_MMSH_SHRINE_H_ */
