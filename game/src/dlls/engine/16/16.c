#include "dlls/object_descriptor.h"
#include "main/game_ui_interface.h"
#include "main/model_engine.h"
#include "main/dll/dll_0010_uicontroller.h"

void UIController_render(void* context, int arg1, int arg2) {
    if (gameTimerIsRunning() != 0) {
        gameTimerRun(context);
    }

    hudNumberRender(context);
    (*gGameUIInterface)->render(context, arg1, arg2);
}

void UIController_frameEnd(void) {
    (*gGameUIInterface)->frameEnd();
}

void UIController_frameStart(void) {
    (*gGameUIInterface)->frameStart();
}

void UIController_release(void) {
}

void UIController_initialise(void) {
}

UI_RESOURCE_VOID_FRAME_ADAPTERS(gUIControllerUiResource, UIController_initialise, UIController_frameStart, UIController_render, (void*)(intptr_t)arg0, arg1, arg2)

UiResourceDescriptor UIController_funcs = {
    {
        {0, 0, 0, 0x00050000},
        gUIControllerUiResourceAcquire,
        UIController_release,
    },
    {
        NULL,
        gUIControllerUiResourceFrameStart,
        UIController_frameEnd,
        gUIControllerUiResourceDraw,
    },
};
