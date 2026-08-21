#ifndef DLLS_OBJECTS_438_SC_LEVELCON_H_
#define DLLS_OBJECTS_438_SC_LEVELCON_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

enum ScLevelControlRomDefNo {
    SC_LEVEL_CONTROL_ROM_DEF_NO = 0x282,
};

enum ScLevelControlMapId {
    SC_LEVEL_CONTROL_MAP_SWAPCIRCLE = 0xE,
};

enum ScLevelControlMapAct {
    SC_LEVEL_CONTROL_MAP_ACT_SPELLSTONE_INSERTED = 1,
    SC_LEVEL_CONTROL_MAP_ACT_SPELLSTONE_USED = 2,
    SC_LEVEL_CONTROL_MAP_ACT_ESCAPE_COMPLETE = 6,
};

enum ScLevelControlAnimState {
    SC_LEVEL_CONTROL_ANIM_STATE_IDLE = 0,
    SC_LEVEL_CONTROL_ANIM_STATE_1 = 1,
    SC_LEVEL_CONTROL_ANIM_STATE_CLEAR = 2,
    SC_LEVEL_CONTROL_ANIM_STATE_START_TREX_CHALLENGE = 3,
    SC_LEVEL_CONTROL_ANIM_STATE_STOP_TREX_CHALLENGE = 4,
    SC_LEVEL_CONTROL_ANIM_STATE_TIMED_CHALLENGE = 5,
    SC_LEVEL_CONTROL_ANIM_STATE_FINISH_TIMED_CHALLENGE = 6,
    SC_LEVEL_CONTROL_ANIM_STATE_EVENT_1 = 7,
};

void sc_levelcontrol_setAnimEventState(GameObject* obj, enum ScLevelControlAnimState animEventState);

extern ObjectDescriptor12 gSC_levelcontrolObjDescriptor;

#endif /* DLLS_OBJECTS_438_SC_LEVELCON_H_ */
