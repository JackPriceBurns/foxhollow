#ifndef MAIN_SPELLSTONE_H_
#define MAIN_SPELLSTONE_H_

#include "types.h"
#include "game/objects/object.h"
#include "dlls/object_descriptor.h"

/* SpellStoneState.state */
typedef enum SpellStoneStateId {
  SPELLSTONE_STATE_HIDDEN = 0, /* not rendered, hits off, snaps to follow target */
  SPELLSTONE_STATE_IDLE = 1,   /* placed and visible, hits enabled, awaiting activation */
  SPELLSTONE_STATE_ACTIVE = 2  /* raised and spinning, proximity-completes the map event */
} SpellStoneStateId;

typedef struct SpellStoneState {
  u8 state;
} SpellStoneState;

typedef struct SpellStoneDef {
  u8 unk0[0x19];
  s8 eventIndex;
  u8 unk1A[4];
  s16 completeEvent;
  s16 activeEvent;
} SpellStoneDef;

extern ObjectDescriptor12 gSpellStoneObjDescriptor;

int spellstone_getState(GameObject *obj);
int spellstone_setState(GameObject *obj,int state);
int spellstone_getExtraSize(void);
void spellstone_free(GameObject *obj);
void spellstone_render(GameObject *obj,u32 p2,u32 p3,
                       u32 p4,u32 p5,char visible);
void spellstone_hitDetect(void);
void spellstone_update(GameObject *obj);
void spellstone_init(GameObject *obj);
void spellstone_release(void);
void spellstone_initialise(void);

#endif /* MAIN_SPELLSTONE_H_ */
