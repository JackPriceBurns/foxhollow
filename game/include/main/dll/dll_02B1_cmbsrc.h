#ifndef MAIN_DLL_CMBSRC_H_
#define MAIN_DLL_CMBSRC_H_

#include "global.h"
#include "game/objects/object.h"
#include "dlls/object_descriptor.h"
#include "game/objects/object_setup.h"
#include "main/objanim_internal.h"

#define CMBSRC_DLL_ID 0x02B1
#define CMBSRC_DUSTMOTESOU_DLL_ID 0x02B2
#define CMBSRC_CLASS_ID 0x007E
#define CMBSRC_DEF_ID 0x059C
#define CMBSRCTPOLE_DEF_ID 0x059D
#define CMBSRCTWALL_DEF_ID 0x059E
#define CMBSRC_THUSTERSOUR_DEF_ID 0x059F
#define DUSTMOTESOU_DEF_ID 0x05A0
#define CMBSRC_OBJECT_DEF_BYTES 0xA0
#define CMBSRC_PLACEMENT_BYTES 0x30
#define CMBSRC_EXTRA_STATE_BYTES 0x28

#define CMBSRC_SEQ_DEFAULT 0x06E8
#define CMBSRC_SEQ_THUSTER_SOURCE 0x0758
#define CMBSRC_SEQ_TWALL 0x0853

#define CMBSRC_STATE_RENDERED 0x01
#define CMBSRC_STATE_EXTERNAL_ACTIVE 0x02
#define CMBSRC_STATE_THORNTAIL_GATE 0x04
#define CMBSRC_STATE_SUPPRESS_IDLE_EFFECT 0x08

#define CMBSRC_MAP_START_ACTIVE 0x01
#define CMBSRC_MAP_LOOP_SOUND 0x02
#define CMBSRC_MAP_ENABLE_HIT_VOLUME 0x04
#define CMBSRC_MAP_RENDER_MODEL 0x08
#define CMBSRC_MAP_CREATE_LIGHT 0x10
#define CMBSRC_MAP_AFFECTS_AABB_LIGHT 0x20
#define CMBSRC_MAP_GLOW 0x40
#define CMBSRC_MAP_GLOW_LARGE 0x80

#define CMBSRC_BEHAVIOR_THORNTAIL_GATE 0x01
#define CMBSRC_BEHAVIOR_ACTIVE_PARTICLES 0x02
#define CMBSRC_BEHAVIOR_DISABLE_FIELD4D 0x04
#define CMBSRC_BEHAVIOR_WIDE_ATTENUATION 0x08
#define CMBSRC_BEHAVIOR_HIT_MODE_MASK 0x30
#define CMBSRC_BEHAVIOR_SYNC_HIT_POSITION 0x40
#define CMBSRC_BEHAVIOR_SUPPRESS_IDLE_EFFECT 0x80

#define CMBSRC_HIT_TYPE_DAMAGE 0x10
#define CMBSRC_MAX_HIT_CHARGE 0x0F
#define CMBSRC_COLOR_CYCLE_COUNT 3
#define CMBSRC_MODE_COLOR_CYCLE 0x0F
#define CMBSRC_EFFECT_MODE_COUNT 9
#define CMBSRC_SUBMODE_COUNT 4
#define CMBSRC_LOOP_SOUND_CHANNEL 0x40
#define CMBSRC_HIT_VOLUME_SLOT 0x1F
#define CMBSRC_PARTICLE_EFFECT_ID 0x07CB
#define CMBSRC_DEFAULT_INACTIVE_FRAMES 0x0258

struct ModelLightStruct;

typedef struct CmbSrcMapData {
  ObjPlacement base;
  s8 rotZ;
  s8 rotY;
  s8 rotX;
  u8 colorIndex;
  u8 effectMode;
  u8 pulseSubMode;
  u8 pad1E[0x20 - 0x1E];
  f32 radius;
  s16 gameBit;
  u8 colorDistance;
  u8 effectDistance;
  u8 pulseDistance;
  u8 flags;
  u8 behaviorFlags;
  u8 inactiveSeconds;
  u8 glowProjectionMode;
  u8 pad2D[CMBSRC_PLACEMENT_BYTES - 0x2D];
} CmbSrcMapData;

typedef struct CmbSrcHitFlags {
  u8 disabled : 1;
} CmbSrcHitFlags;

typedef struct CmbSrcState {
  struct ModelLightStruct *light;
  f32 effectTimer;
  f32 pulseTimer;
  f32 particleTimer;
  f32 colorCycleTimer;
  f32 inactiveTimer;
  f32 radius;
  f32 hitRecoverTimer;
  u16 inactiveFrameCount;
  u8 flags;
  u8 colorCycleIndex;
  u8 priorityHitType;
  u8 active;
  s8 hitCharge;
  CmbSrcHitFlags hitFlags;
} CmbSrcState;

STATIC_ASSERT(sizeof(CmbSrcMapData) == CMBSRC_PLACEMENT_BYTES);
STATIC_ASSERT(offsetof(CmbSrcMapData, rotZ) == 0x18);
STATIC_ASSERT(offsetof(CmbSrcMapData, colorIndex) == 0x1B);
STATIC_ASSERT(offsetof(CmbSrcMapData, effectMode) == 0x1C);
STATIC_ASSERT(offsetof(CmbSrcMapData, radius) == 0x20);
STATIC_ASSERT(offsetof(CmbSrcMapData, gameBit) == 0x24);
STATIC_ASSERT(offsetof(CmbSrcMapData, flags) == 0x29);
STATIC_ASSERT(offsetof(CmbSrcMapData, behaviorFlags) == 0x2A);
STATIC_ASSERT(offsetof(CmbSrcMapData, inactiveSeconds) == 0x2B);
STATIC_ASSERT(offsetof(CmbSrcMapData, glowProjectionMode) == 0x2C);

STATIC_ASSERT(sizeof(CmbSrcState) == CMBSRC_EXTRA_STATE_BYTES);
STATIC_ASSERT(offsetof(CmbSrcState, light) == 0x00);
STATIC_ASSERT(offsetof(CmbSrcState, colorCycleTimer) == 0x10);
STATIC_ASSERT(offsetof(CmbSrcState, inactiveTimer) == 0x14);
STATIC_ASSERT(offsetof(CmbSrcState, radius) == 0x18);
STATIC_ASSERT(offsetof(CmbSrcState, inactiveFrameCount) == 0x20);
STATIC_ASSERT(offsetof(CmbSrcState, flags) == 0x22);
STATIC_ASSERT(offsetof(CmbSrcState, colorCycleIndex) == 0x23);
STATIC_ASSERT(offsetof(CmbSrcState, priorityHitType) == 0x24);
STATIC_ASSERT(offsetof(CmbSrcState, active) == 0x25);
STATIC_ASSERT(offsetof(CmbSrcState, hitCharge) == 0x26);
STATIC_ASSERT(offsetof(CmbSrcState, hitFlags) == 0x27);

extern ObjectDescriptor gCmbSrcObjDescriptor;
extern u8 gCmbsrcColorCycleIndexTable[8];
extern u8 gCmbsrcColorSoundIdTable[];
extern u8 gCmbsrcColorRgbTable[];
extern f32 gCmbsrcColorRadiusScaleTable[];

int cmbsrc_getExtraSize(void);
int cmbsrc_getObjectTypeId(void);
void cmbsrc_initialise(void);
void cmbsrc_release(void);
int cmbsrc_updateAndReturnZero(GameObject* obj);
int cmbsrc_getColorIndex(GameObject* obj);
void cmbsrc_setExternalActive(GameObject* obj, u8 active);
void cmbsrc_free(GameObject* cmbsrc);
void cmbsrc_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible);
u8 cmbsrc_shouldActivate(GameObject* obj, CmbSrcState* state, CmbSrcMapData* setup);
u8 cmbsrc_shouldDeactivate(GameObject* obj, CmbSrcState* state, CmbSrcMapData* setup);
void cmbsrc_hitDetect(GameObject* obj);
u8 cmbsrc_cycleColor(GameObject* obj, CmbSrcState* state);
void cmbsrc_updateVisuals(GameObject* obj, CmbSrcState* state);
void cmbsrc_update(GameObject* obj);
void cmbsrc_init(GameObject* obj, CmbSrcMapData* setup);

#endif /* MAIN_DLL_CMBSRC_H_ */
