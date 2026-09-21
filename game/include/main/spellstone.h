#ifndef MAIN_SPELLSTONE_H_
#define MAIN_SPELLSTONE_H_

#include "types.h"
#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

typedef enum SpellStoneStateId {
    SPELLSTONE_STATE_HIDDEN,
    SPELLSTONE_STATE_IDLE,
    SPELLSTONE_STATE_ACTIVE
} SpellStoneStateId;

struct SpellStoneObjDescriptorType;
extern struct SpellStoneObjDescriptorType gSpellStoneObjDescriptor;

int spellstone_getState(GameObject* obj);
int spellstone_setState(GameObject* obj, int state);
int spellstone_getExtraSize(void);
void spellstone_free(GameObject* obj);
void spellstone_render(GameObject* obj, u32 p2, u32 p3, u32 p4, u32 p5, s8 visible);
void spellstone_hitDetect(void);
void spellstone_update(GameObject* obj);
void spellstone_init(GameObject* obj);
void spellstone_release(void);
void spellstone_initialise(void);

#endif /* MAIN_SPELLSTONE_H_ */
