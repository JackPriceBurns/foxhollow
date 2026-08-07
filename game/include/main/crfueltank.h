#ifndef MAIN_CRFUELTANK_H_
#define MAIN_CRFUELTANK_H_

#include "types.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "dlls/object_descriptor.h"

typedef struct CrFuelTankState {
  u8 unk0[0xc];
  f32 timer;
} CrFuelTankState;

typedef struct CrFuelTankDef {
  ObjPlacement base;
  u8 unk18[0x1a - 0x18];
  s16 idleFrameCount;
  u8 unk1C[2];
  s16 hitEvent;
} CrFuelTankDef;

STATIC_ASSERT(offsetof(CrFuelTankDef, idleFrameCount) == 0x1A);
STATIC_ASSERT(sizeof(CrFuelTankDef) == 0x20);

extern ObjectDescriptor gCrFuelTankObjDescriptor;

int crfueltank_getExtraSize(void);
void crfueltank_free(void);
void crfueltank_render(void);
void crfueltank_hitDetect(GameObject *obj);
void crfueltank_update(GameObject *obj);
void crfueltank_init(GameObject *obj,CrFuelTankDef *def);
void crfueltank_release(void);
void crfueltank_initialise(void);

#endif /* MAIN_CRFUELTANK_H_ */
