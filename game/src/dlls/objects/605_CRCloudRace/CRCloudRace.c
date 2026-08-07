/* CRCloudRace (DLL 605): CloudRunner race controller. */

#include "main/crcloudrace.h"
#include "main/audio/music_api.h"
#include "main/maketex_timer_api.h"
#include "main/map_load.h"
#include "main/pi_dolphin_api.h"
#include "main/rcp_dolphin_api.h"
#include "main/dll/player_api.h"
#include "dlls/objects/430_SH_LevelCon.h"
#include "main/mapEventTypes.h"
#include "main/gamebits.h"
#include "main/render_envfx_api.h"
#include "sys/objects.h"
#include "main/object_render.h"
#include "main/objtype.h"
#include "dlls/objects/440_SC_totempol.h"

ObjectDescriptor gCrCloudRaceObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)crcloudrace_initialise,
    (ObjectDescriptorCallback)crcloudrace_release,
    0,
    (ObjectDescriptorCallback)crcloudrace_init,
    (ObjectDescriptorCallback)crcloudrace_update,
    (ObjectDescriptorCallback)crcloudrace_hitDetect,
    (ObjectDescriptorCallback)crcloudrace_render,
    (ObjectDescriptorCallback)crcloudrace_free,
    (ObjectDescriptorCallback)crcloudrace_getObjectTypeId,
    crcloudrace_getExtraSize,
};

void crcloudrace_updateCompletionState(GameObject* obj, CrCloudRaceState* state)
{
    f32 dist;
    GameObject* player;
    GameObject* near;

    dist = 5e+07f;
    player = Obj_GetPlayerObject();
    if (mainGetBit(CRCLOUDRACE_GAMEBIT_IN_FINISH_VOLUME) == 0)
    {
        if (mainGetBit(CRCLOUDRACE_GAMEBIT_ABORT_TRIGGER) != 0)
        {
            state->phase = CRCLOUDRACE_PHASE_ABORT;
            setMotionBlur(0, 0.0f);
            mainSetBits(CRCLOUDRACE_GAMEBIT_RACE_ACTIVE, 0);
            mainSetBits(CRCLOUDRACE_GAMEBIT_RACE_STARTED, 0);
        }
    }
    else
    {
        mainSetBits(CRCLOUDRACE_GAMEBIT_IN_FINISH_VOLUME, 1);
        setMotionBlur(0, 0.0f);
        if (mainGetBit(CRCLOUDRACE_GAMEBIT_RACE_CAN_FINISH) != 0 &&
            playerGetFocusObject(player) == NULL)
        {
            near = objGetNearestTypeTo(CRCLOUDRACE_NEARBY_TOTEM_GROUP, obj, &dist);
            if (near != NULL)
            {
                ((ScTotemPoleInterfaceVTable*)*near->anim.dll)->handleEvent(near, 1);
            }
            state->phase = CRCLOUDRACE_PHASE_RESET_TO_START;
        }
    }
}

void crcloudrace_updateRaceState(GameObject* obj)
{
    CrCloudRaceState* inner;
    GameObject* player;

    inner = obj->extra;
    player = Obj_GetPlayerObject();
    switch (inner->phase)
    {
    case CRCLOUDRACE_PHASE_START:
        if (mainGetBit(CRCLOUDRACE_GAMEBIT_TOTEM_GATE) != 0)
        {
            mainSetBits(CRCLOUDRACE_GAMEBIT_TOTEM_LATCH, 1);
        }
        if (playerGetFocusObject(player) != NULL)
        {
            mainSetBits(CRCLOUDRACE_GAMEBIT_RACE_STARTED, 1);
            mainSetBits(CRCLOUDRACE_GAMEBIT_RACE_ACTIVE, 1);
            inner->phase = CRCLOUDRACE_PHASE_RACING;
            unlockLevel(0, 0, 1);
        }
        break;
    case CRCLOUDRACE_PHASE_RACING:
        crcloudrace_updateCompletionState(obj, inner);
        break;
    case CRCLOUDRACE_PHASE_ABORT:
        mainSetBits(CRCLOUDRACE_GAMEBIT_TOTEM_LATCH, 0);
        inner->phase = CRCLOUDRACE_PHASE_COUNTDOWN;
        s16toFloat(&inner->timer, CRCLOUDRACE_COUNTDOWN_FRAMES);
        break;
    case CRCLOUDRACE_PHASE_COUNTDOWN:
        if (timerCountDown(&inner->timer) != 0)
        {
            inner->phase = CRCLOUDRACE_PHASE_RELOAD_DRAG_ROCK;
        }
        break;
    case CRCLOUDRACE_PHASE_RELOAD_DRAG_ROCK:
        unlockLevel(0, 0, 1);
        loadMapAndParent(CRCLOUDRACE_DRAG_ROCK_MAP_ID);
        lockLevel(mapGetDirIdx(CRCLOUDRACE_DRAG_ROCK_MAP_ID), 0);
        mainSetBits(CRCLOUDRACE_RESET_BIT_D73, 0);
        mainSetBits(CRCLOUDRACE_RESET_BIT_983, 0);
        mainSetBits(CRCLOUDRACE_RESET_BIT_E23, 0);
        mainSetBits(CRCLOUDRACE_RESET_BIT_E1D, 0);
        mainSetBits(CRCLOUDRACE_RESET_BIT_DB8, 0);
        mainSetBits(CRCLOUDRACE_RESET_BIT_984, 0);
        mainSetBits(CRCLOUDRACE_GAMEBIT_DRAG_ROCK_CLEARED, 0);
        inner->phase = CRCLOUDRACE_PHASE_IDLE;
        break;
    case CRCLOUDRACE_PHASE_RESET_TO_START:
        inner->phase = CRCLOUDRACE_PHASE_START;
        break;
    case 1:
    case 6:
    default:
        inner->phase = CRCLOUDRACE_PHASE_START;
        break;
    case CRCLOUDRACE_PHASE_IDLE:
        break;
    }
}

int crcloudrace_completionCallback(GameObject* obj, int unused, ObjSeqState* animUpdate)
{
    CrCloudRaceState* state = obj->extra;
    int i;

    state->flags |= CRCLOUDRACE_STATE_FLAG_COMPLETION_CALLBACK;
    for (i = 0; i < animUpdate->eventCount; i++)
    {
        switch (animUpdate->eventIds[i])
        {
        case CRCLOUDRACE_COMPLETION_ANIM_EVENT:
            mainSetBits(CRCLOUDRACE_GAMEBIT_COMPLETION_EVENT, 1);
            mainSetBits(CRCLOUDRACE_GAMEBIT_DRAG_ROCK_CLEARED, 0);
            loadMapAndParent(CRCLOUDRACE_DRAG_ROCK_MAP_ID);
            unlockLevel(0, 0, 1);
            lockLevel(mapGetDirIdx(CRCLOUDRACE_DRAG_ROCK_MAP_ID), 0);
            (*gMapEventInterface)->setObjGroupStatus(CRCLOUDRACE_DRAG_ROCK_MAP_ID, 1, 1);
            break;
        }
    }
    return 0;
}

int crcloudrace_getExtraSize(void)
{
    return sizeof(CrCloudRaceState);
}

int crcloudrace_getObjectTypeId(void)
{
    return 0;
}

void crcloudrace_free(void)
{
    return;
}

void crcloudrace_render(GameObject* obj, u32 p2, u32 p3, u32 p4, u32 p5, char visible)
{
    int draw;

    draw = visible;
    if (draw != 0)
    {
        objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
    }
    return;
}

void crcloudrace_hitDetect(void)
{
    return;
}

void crcloudrace_update(GameObject* obj)
{
    u32 eventActive;
    CrCloudRaceState* state;

    state = obj->extra;
    if (obj->userData2 == 0)
    {
        eventActive = mainGetBit(CRCLOUDRACE_GAMEBIT_EFFECT_CLEAR);
        if (eventActive != 0)
        {
            getEnvfxActImmediately(obj, obj, CRCLOUDRACE_ENVFX_CLEAR_A, 0);
            getEnvfxActImmediately(obj, obj, CRCLOUDRACE_ENVFX_CLEAR_B, 0);
            mainSetBits(CRCLOUDRACE_GAMEBIT_EFFECT_CLEAR, 0);
            unlockLevel(0, 0, 1);
        }
        obj->userData1 = 1;
    }
    crcloudrace_updateRaceState(obj);
    state->flags &= ~CRCLOUDRACE_STATE_FLAG_COMPLETION_CALLBACK;
    GameBitLatch_Update((GameBitLatchState*)state->effect, 1, -1, -1, CRCLOUDRACE_GAMEBIT_START_LATCH_A,
                          CRCLOUDRACE_GAMEBIT_START_LATCH_B);
    GameBitLatch_Update((GameBitLatchState*)state->effect, 2, -1, -1, CRCLOUDRACE_GAMEBIT_START_LATCH_A,
                          CRCLOUDRACE_GAMEBIT_START_LATCH_C);
    return;
}

void crcloudrace_init(GameObject* obj)
{
    CrCloudRaceState* state;

    state = obj->extra;
    obj->animEventCallback = crcloudrace_completionCallback;
    state->phase = CRCLOUDRACE_PHASE_START;
    storeZeroToFloatParam(&state->timer);
    mainSetBits(CRCLOUDRACE_GAMEBIT_START_LATCH_A, 1);
    Music_StopChannelsByPriorityGroup(3, MUSIC_CHANNEL_STOP_FADE, 1000);
    return;
}

void crcloudrace_release(void)
{
    return;
}

void crcloudrace_initialise(void)
{
    return;
}
