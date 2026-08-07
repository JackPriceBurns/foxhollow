#ifndef DLLS_OBJECTS_518_LIGHTSOURCE_H_
#define DLLS_OBJECTS_518_LIGHTSOURCE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct ModelLightStruct ModelLightStruct;

/*
 * Only the placement prefix consumed by this DLL is modeled. The active-target
 * retail placement width is not yet evidenced.
 */
typedef struct LightSourcePlacementView {
    ObjPlacement base; /* 0x00 */
    s8 yaw;            /* 0x18 */
    u8 mode;           /* 0x19 */
    s16 range;         /* 0x1A */
    s16 flags;         /* 0x1C */
    s16 gameBit;       /* 0x1E */
    u8 unknown20[2];   /* 0x20 */
    u8 options;        /* 0x22 */
} LightSourcePlacementView;

typedef struct LightSourceLoopFlags {
    u8 loopedSound : 1;
} LightSourceLoopFlags;

/* lightsource_getExtraSize() allocates this complete 0x1C-byte state. */
typedef struct LightSourceState {
    ModelLightStruct* light;        /* 0x00 */
    f32 fxTimer;                    /* 0x04 */
    f32 unknown08;                  /* 0x08 */
    f32 sparkSpawnTimer;            /* 0x0C */
    int gameBit;                    /* 0x10 */
    u8 mode;                        /* 0x14 */
    u8 fxType;                      /* 0x15 */
    u8 fxArg;                       /* 0x16 */
    u8 lit;                         /* 0x17 */
    u8 litPrev;                     /* 0x18 */
    u8 sparks;                      /* 0x19 */
    LightSourceLoopFlags loopFlags; /* 0x1A */
    u8 unknown1B;                   /* 0x1B */
} LightSourceState;

typedef enum LightSourceMode {
    LIGHTSOURCE_MODE_STATIC = 0,
    LIGHTSOURCE_MODE_INTERACTIVE = 1,
} LightSourceMode;

#define LIGHTSOURCE_FLAG_FX_ARG_6        0x01
#define LIGHTSOURCE_FLAG_DISABLE_FX_TYPE 0x02
#define LIGHTSOURCE_FLAG_FX_TYPE_4       0x04
#define LIGHTSOURCE_FLAG_FX_TYPE_8       0x08
#define LIGHTSOURCE_FLAG_FX_TYPE_6       0x10
#define LIGHTSOURCE_FLAG_FX_ARG_ZERO     0x20
#define LIGHTSOURCE_FLAG_CREATE_LIGHT    0x40
#define LIGHTSOURCE_FLAG_CREATE_GLOW     0x80
#define LIGHTSOURCE_OPTION_SPARKS        0x01

STATIC_ASSERT(offsetof(LightSourcePlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(LightSourcePlacementView, yaw) == 0x18);
STATIC_ASSERT(offsetof(LightSourcePlacementView, mode) == 0x19);
STATIC_ASSERT(offsetof(LightSourcePlacementView, range) == 0x1A);
STATIC_ASSERT(offsetof(LightSourcePlacementView, flags) == 0x1C);
STATIC_ASSERT(offsetof(LightSourcePlacementView, gameBit) == 0x1E);
STATIC_ASSERT(offsetof(LightSourcePlacementView, unknown20) == 0x20);
STATIC_ASSERT(offsetof(LightSourcePlacementView, options) == 0x22);

STATIC_ASSERT(sizeof(LightSourceLoopFlags) == 0x01);
STATIC_ASSERT(offsetof(LightSourceState, light) == 0x00);
STATIC_ASSERT(offsetof(LightSourceState, fxTimer) == 0x04);
STATIC_ASSERT(offsetof(LightSourceState, unknown08) == 0x08);
STATIC_ASSERT(offsetof(LightSourceState, sparkSpawnTimer) == 0x0C);
STATIC_ASSERT(offsetof(LightSourceState, gameBit) == 0x10);
STATIC_ASSERT(offsetof(LightSourceState, mode) == 0x14);
STATIC_ASSERT(offsetof(LightSourceState, fxType) == 0x15);
STATIC_ASSERT(offsetof(LightSourceState, fxArg) == 0x16);
STATIC_ASSERT(offsetof(LightSourceState, lit) == 0x17);
STATIC_ASSERT(offsetof(LightSourceState, litPrev) == 0x18);
STATIC_ASSERT(offsetof(LightSourceState, sparks) == 0x19);
STATIC_ASSERT(offsetof(LightSourceState, loopFlags) == 0x1A);
STATIC_ASSERT(offsetof(LightSourceState, unknown1B) == 0x1B);
STATIC_ASSERT(sizeof(LightSourceState) == 0x1C);

int lightsource_getExtraSize(void);
int lightsource_getObjectTypeId(void);
void lightsource_free(GameObject* obj);
void lightsource_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void lightsource_hitDetect(void);
void lightsource_update(GameObject* obj);
void lightsource_init(GameObject* obj, const LightSourcePlacementView* placement);
void lightsource_release(void);
void lightsource_initialise(void);

extern const u8 gLightSourceColorTable[16][3];
extern ObjectDescriptor gLightSourceObjDescriptor;

#endif /* DLLS_OBJECTS_518_LIGHTSOURCE_H_ */
