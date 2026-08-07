#include "dlls/object_descriptor.h"
#include "main/game_ui_interface.h"
#include "main/model_engine.h"
#include "main/dll/dll_0010_uicontroller.h"

void UIController_render(void* context, int arg1, int arg2)
{
    if (gameTimerIsRunning() != 0)
    {
        gameTimerRun(context);
    }
    hudNumberRender(context);
    (*gGameUIInterface)->render(context, arg1, arg2);
}

void UIController_frameEnd(void)
{
    (*gGameUIInterface)->frameEnd();
}

void UIController_frameStart(void)
{
    (*gGameUIInterface)->frameStart();
}

void UIController_release(void)
{
}

void UIController_initialise(void)
{
}
typedef struct UIControllerDllInterface {
    u32 reserved0;
    u32 reserved1;
    u32 reserved2;
    u32 slotCountAndFlags;
    ObjectDescriptorCallback initialise;
    ObjectDescriptorCallback release;
    ObjectDescriptorCallback slot02;
    ObjectDescriptorCallback frameStart;
    ObjectDescriptorCallback frameEnd;
    ObjectDescriptorCallback render;
} UIControllerDllInterface;

UIControllerDllInterface UIController_funcs = {
    0,
    0,
    0,
    0x00050000,
    (ObjectDescriptorCallback)UIController_initialise,
    (ObjectDescriptorCallback)UIController_release,
    0,
    (ObjectDescriptorCallback)UIController_frameStart,
    (ObjectDescriptorCallback)UIController_frameEnd,
    (ObjectDescriptorCallback)UIController_render,
};
