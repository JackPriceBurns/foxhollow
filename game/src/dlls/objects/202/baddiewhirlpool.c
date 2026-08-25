#include "main/dll/baddie_frozen.h"
#include "main/dll/baddiewhirlpool.h"
#include "main/frame_timing.h"
#include "main/model.h"
#include "main/objhits.h"
#include "main/objtype.h"
#include "sys/objects.h"
#include "track/intersect_whirlpool.h"

typedef enum IceBaddieObjectGroup {
    ICE_BADDIE_WHIRLPOOL_OBJECT_GROUP = 80
} IceBaddieObjectGroup;

void whirlpool_updateWhileFrozen(GameObject* wpad0, u8* wpad1, GameObject* attacker, int wpad3, int wpad4, int wpad5,
                                 Vec* wpad6, int wpad7)
{
}

void iceBaddie_enterWhirlpoolGroup(GameObject* obj, EnemyState* state)
{
    ObjHitsPriorityState* hitState;

    if (state->userData2 == 0)
    {
        objAddObjectType(obj, ICE_BADDIE_WHIRLPOOL_OBJECT_GROUP);
        state->userData2 = 1;
    }
    ObjHits_SetHitVolumeSlot(&obj->anim, 10, 1, 0);
    hitState = obj->anim.hitReactState;
    hitState->suppressOutgoingHits = 0;
    obj->anim.rotX -= 256;
}

void iceBaddie_leaveWhirlpoolGroup(GameObject* obj, EnemyState* state)
{
    if (state->userData2 != 0)
    {
        objFreeObjectType(obj, ICE_BADDIE_WHIRLPOOL_OBJECT_GROUP);
        state->userData2 = 0;
    }
    obj->anim.rotX = (s16)((f32)obj->anim.rotX - 256.0f * timeDelta);
}

void baddie_initWhirlpoolState(GameObject* obj, EnemyState* state)
{
    state->sightRange = 60.0f;
    state->userData2 = (u8)state->aggroRange;
    state->aggroRange = 160.0f;
    state->flags2E4 = 0x42001;
    state->animPlaySpeed = 0.01f;
    state->gravity = 0.006f;
    state->drag = 0.95f;
    state->moveId0 = 0;
    state->moveSpeedScale0 = 1.0f;
    state->moveId1 = 5;
    state->moveSpeedScale1 = 1.0f;
    state->moveId2 = 7;
    state->moveSpeedScale2 = 1.0f;
    state->userData1 = 1;
    state->userData2 = 0;
    ObjModel_SetRenderCallback((u8*)Obj_GetActiveModel(obj), renderWhirlpool);
}
