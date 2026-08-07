#ifndef DLLS_OBJECTS_367_IMSPACETHRU_H_
#define DLLS_OBJECTS_367_IMSPACETHRU_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef enum IMSpaceThrusterPhase {
    IM_SPACE_THRUSTER_PHASE_OFF = 0,
    IM_SPACE_THRUSTER_PHASE_ON = 1,
    IM_SPACE_THRUSTER_PHASE_FADE_OUT = 2,
} IMSpaceThrusterPhase;

typedef struct IMSpaceThrusterPlacement {
    ObjPlacement base;
    s8 initialRotX;
    u8 kind;
    s16 initialRotY;
    s16 bankIndex;
} IMSpaceThrusterPlacement;

typedef struct IMSpaceThrusterState {
    u8 kind;
    u8 phase;
    s16 blendTimer;
    void* keyframesA;
    void* keyframesB;
} IMSpaceThrusterState;

typedef s16 (*IMSpaceThrusterGetModeCallback)(GameObject* parent, int kind);
typedef void (*IMSpaceThrusterSetWeightCallback)(GameObject* parent, f32 weight, int kind);

typedef struct IMSpaceThrusterParentInterface {
    void* standardSlots[8];
    IMSpaceThrusterGetModeCallback getThrusterMode;
    void* slot09;
    IMSpaceThrusterSetWeightCallback setThrusterWeight;
} IMSpaceThrusterParentInterface;

STATIC_ASSERT(offsetof(IMSpaceThrusterPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(IMSpaceThrusterPlacement, initialRotX) == 0x18);
STATIC_ASSERT(offsetof(IMSpaceThrusterPlacement, kind) == 0x19);
STATIC_ASSERT(offsetof(IMSpaceThrusterPlacement, initialRotY) == 0x1A);
STATIC_ASSERT(offsetof(IMSpaceThrusterPlacement, bankIndex) == 0x1C);

STATIC_ASSERT(offsetof(IMSpaceThrusterState, kind) == 0x00);
STATIC_ASSERT(offsetof(IMSpaceThrusterState, phase) == 0x01);
STATIC_ASSERT(offsetof(IMSpaceThrusterState, blendTimer) == 0x02);
STATIC_ASSERT(offsetof(IMSpaceThrusterState, keyframesA) == 0x04);
STATIC_ASSERT(offsetof(IMSpaceThrusterState, keyframesB) == 0x08);
STATIC_ASSERT(sizeof(IMSpaceThrusterState) == 0x0C);

STATIC_ASSERT(offsetof(IMSpaceThrusterParentInterface, standardSlots) == 0x00);
STATIC_ASSERT(offsetof(IMSpaceThrusterParentInterface, getThrusterMode) == 0x20);
STATIC_ASSERT(offsetof(IMSpaceThrusterParentInterface, slot09) == 0x24);
STATIC_ASSERT(offsetof(IMSpaceThrusterParentInterface, setThrusterWeight) == 0x28);
STATIC_ASSERT(sizeof(IMSpaceThrusterParentInterface) == 0x2C);

#define IM_SPACE_THRUSTER_PARENT_INTERFACE(parent) ((IMSpaceThrusterParentInterface*)*((GameObject*)(parent))->anim.dll)

int imSpaceThruster_getExtraSize(void);
int imSpaceThruster_getObjectTypeId(void);
void imSpaceThruster_free(GameObject* obj);
void imSpaceThruster_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                            s8 visible);
void imSpaceThruster_hitDetect(void);
void imSpaceThruster_update(GameObject* obj);
void imSpaceThruster_init(GameObject* obj, const IMSpaceThrusterPlacement* placement);
void imSpaceThruster_release(void);
void imSpaceThruster_initialise(void);

extern s16 gIMSpaceThrusterKeyframeIndicesA[6];
extern s16 gIMSpaceThrusterKeyframeIndicesB[6];
extern ObjectDescriptor gIMSpaceThrusterObjDescriptor;

#endif /* DLLS_OBJECTS_367_IMSPACETHRU_H_ */
