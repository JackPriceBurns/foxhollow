#ifndef MAIN_PROXIMITYMINE_H_
#define MAIN_PROXIMITYMINE_H_

#include "types.h"
#include "main/model_light.h"
#include "game/objects/object_setup.h"
#include "dlls/object_descriptor.h"

#define PROXIMITYMINE_HIT_VOLUME_SLOT 13

struct GameObject;

/* Runtime state of a proximity mine (ProximityMineState.mode). */
typedef enum ProximityMineMode {
  PROXIMITYMINE_MODE_LAUNCHING = -1, /* compute launch velocity then fall through to flight */
  PROXIMITYMINE_MODE_EXPIRED = 0,    /* stopped/destroyed: count render timer then free */
  PROXIMITYMINE_MODE_FLIGHT = 1,     /* integrate launch velocity, then fall through to armed */
  PROXIMITYMINE_MODE_ARMED = 2,      /* live: spawn fx, enable hit detection */
  PROXIMITYMINE_MODE_WAITING = 3     /* idle until player enters trigger range, then arm */
} ProximityMineMode;

/* Placement-config spawn variant (ProximityMineDef.mode). */
typedef enum ProximityMineSpawnMode {
  PROXIMITYMINE_SPAWN_TIMED = 0,     /* stationary mine armed after a parameter delay */
  PROXIMITYMINE_SPAWN_LAUNCHED = 1,  /* launched/thrown mine */
  PROXIMITYMINE_SPAWN_PROXIMITY = 2  /* immediately-armed proximity mine */
} ProximityMineSpawnMode;

typedef struct ProximityMineState {
  struct GameObject *targetObj;
  ModelLightStruct *effectHandle;
  f32 triggerDistance;
  f32 scaleStep;
  u8 unk10[4];
  f32 renderTimer;
  f32 launchTimer;
  f32 resetTimer;
  f32 bounceTimer;
  f32 initTimer;
  f32 lifespanTimer;
  s8 mode;
  u8 unk2D;
  u8 flashMode;
  u8 unk2F;
  u8 effectVisible;
  u8 unk31[3];
} ProximityMineState;

STATIC_ASSERT(offsetof(ProximityMineState, effectHandle) == 0x4);
STATIC_ASSERT(sizeof(ProximityMineState) == 0x34);

typedef struct ProximityMineDef {
  ObjPlacement base;
  s8 angleSeed;
  s8 mode;
  s16 parameter;
} ProximityMineDef;

STATIC_ASSERT(offsetof(ProximityMineDef, angleSeed) == 0x18);
STATIC_ASSERT(sizeof(ProximityMineDef) == 0x1C);

extern ObjectDescriptor gProximityMineObjDescriptor;

void ProximityMine_expire(struct GameObject *obj);
int ProximityMine_getExtraSize(void);
int ProximityMine_getObjectTypeId(void);
void ProximityMine_free(struct GameObject *obj);
void ProximityMine_render(struct GameObject *obj,u32 p2,u32 p3,
                          u32 p4,u32 p5);
void ProximityMine_hitDetect(struct GameObject *obj);
void ProximityMine_update(struct GameObject *obj);
void ProximityMine_init(struct GameObject *obj,ProximityMineDef *def);
void ProximityMine_release(void);
void ProximityMine_initialise(void);

#endif /* MAIN_PROXIMITYMINE_H_ */
