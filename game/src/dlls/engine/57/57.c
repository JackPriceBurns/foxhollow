#include "main/frame_timing.h"
#include "main/texture.h"
#include "dlls/object_descriptor.h"
#include "main/model_engine.h"
#include "main/rcp_dolphin_api.h"
#include "sys/objects.h"
#include "main/dll/dll_0039_dummy39.h"
#include "main/pause_menu_api.h"

#define DUMMY39_COUNTDOWN_FRAMES 0x28
#define DUMMY39_WARP_MAP         0x60
#define DUMMY39_MAX_STEP_FRAMES  3

void Dummy39_render(void)
{
}

void Dummy39_frameEnd(void)
{
}

int Dummy39_run(void)
{
    s32 step;
    u8 cur;
    s8 next;
    Obj_GetPlayerObject();
    step = framesThisStep;
    if (step > DUMMY39_MAX_STEP_FRAMES)
        step = DUMMY39_MAX_STEP_FRAMES;
    cur = gDummy39Countdown;
    if ((s8)cur > 0)
    {
        next = cur - step;
        *(s8*)&gDummy39Countdown = next;
        if ((s8)(u8)next <= 0)
        {
            loadUiDll(1);
            warpToMap(DUMMY39_WARP_MAP, 1);
        }
    }
    return 0;
}

void Dummy39_release(void)
{
    textureFree((Texture*)(gDummy39Texture));
}

void Dummy39_initialise(void)
{
    gDummy39Countdown = DUMMY39_COUNTDOWN_FRAMES;
}

ObjectDescriptor6 Dummy39_funcs = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_6_SLOTS,
    (ObjectDescriptorCallback)Dummy39_initialise,
    (ObjectDescriptorCallback)Dummy39_release,
    0,
    (ObjectDescriptorCallback)Dummy39_run,
    (ObjectDescriptorCallback)Dummy39_frameEnd,
    (ObjectDescriptorCallback)Dummy39_render,
};
