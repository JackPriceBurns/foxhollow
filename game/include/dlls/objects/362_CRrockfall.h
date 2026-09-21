#ifndef DLLS_OBJECTS_362_CRROCKFALL_H_
#define DLLS_OBJECTS_362_CRROCKFALL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define CR_ROCKFALL_CONFIG_COUNT 2

typedef enum CrRockfallMode {
    CR_ROCKFALL_MODE_ARMED = 0,
    CR_ROCKFALL_MODE_FALLING = 1,
    CR_ROCKFALL_MODE_RESTING = 2,
    CR_ROCKFALL_MODE_SHATTERED = 3,
} CrRockfallMode;

typedef struct CrRockfallConfig {
    s32 seqId;
    s32 landSfx;
    f32 restOffsetY;
} CrRockfallConfig;

typedef struct CrRockfallPlacement {
    ObjPlacement base;
    u8 pad18[0x02];
    u8 triggerRange;
    u8 scaleByte;
    s16 gameBitId;
    s16 fallDelay;
} CrRockfallPlacement;

typedef struct CrRockfallState {
    const CrRockfallConfig* config;
    f32 floorY;
    f32 startY;
    u8 mode; /* CrRockfallMode */
    u8 fallStarted;
    u8 floorFound;
    u8 pad0F;
    s16 fallDelay;
    u8 pad12[0x02];
} CrRockfallState;

STATIC_ASSERT(offsetof(CrRockfallConfig, seqId) == 0x00);
STATIC_ASSERT(offsetof(CrRockfallConfig, landSfx) == 0x04);
STATIC_ASSERT(offsetof(CrRockfallConfig, restOffsetY) == 0x08);
STATIC_ASSERT(sizeof(CrRockfallConfig) == 0x0C);

STATIC_ASSERT(offsetof(CrRockfallPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CrRockfallPlacement, pad18) == 0x18);
STATIC_ASSERT(offsetof(CrRockfallPlacement, triggerRange) == 0x1A);
STATIC_ASSERT(offsetof(CrRockfallPlacement, scaleByte) == 0x1B);
STATIC_ASSERT(offsetof(CrRockfallPlacement, gameBitId) == 0x1C);
STATIC_ASSERT(offsetof(CrRockfallPlacement, fallDelay) == 0x1E);
STATIC_ASSERT(sizeof(CrRockfallPlacement) == 0x20);

STATIC_ASSERT(offsetof(CrRockfallState, config) == 0x00);
STATIC_ASSERT(offsetof(CrRockfallState, floorY) == 0x04);
STATIC_ASSERT(offsetof(CrRockfallState, startY) == 0x08);
STATIC_ASSERT(offsetof(CrRockfallState, mode) == 0x0C);
STATIC_ASSERT(offsetof(CrRockfallState, fallStarted) == 0x0D);
STATIC_ASSERT(offsetof(CrRockfallState, floorFound) == 0x0E);
STATIC_ASSERT(offsetof(CrRockfallState, pad0F) == 0x0F);
STATIC_ASSERT(offsetof(CrRockfallState, fallDelay) == 0x10);
STATIC_ASSERT(offsetof(CrRockfallState, pad12) == 0x12);
STATIC_ASSERT(sizeof(CrRockfallState) == 0x14);

f32 crrockfall_findFloorY(GameObject* obj);
int crrockfall_getExtraSize(void);
int crrockfall_getObjectTypeId(void);
void crrockfall_free(void);
void crrockfall_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void crrockfall_hitDetect(void);
void crrockfall_update(GameObject* obj);
void crrockfall_init(GameObject* obj, const CrRockfallPlacement* placement);
void crrockfall_release(void);
void crrockfall_initialise(void);

extern void* gCrRockfallResource;
extern CrRockfallConfig gCrRockfallConfigTable[CR_ROCKFALL_CONFIG_COUNT];
extern ObjectDescriptor gCRrockfallObjDescriptor;

#endif /* DLLS_OBJECTS_362_CRROCKFALL_H_ */
