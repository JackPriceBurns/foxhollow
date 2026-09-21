#include "dlls/object_descriptor.h"
#include "main/dll/dll_003A_dummy3a.h"
#include "main/model_engine.h"

void Dummy3A_render(void) {
}

void Dummy3A_frameEnd(void) {
}

int Dummy3A_frameStart(void) {
    return 0;
}

void Dummy3A_release(void) {
}

void Dummy3A_initialise(void) {
}

UI_RESOURCE_ADAPTERS(gDummy3AUiResource, Dummy3A_initialise, Dummy3A_frameStart, Dummy3A_render)

UiResourceDescriptor Dummy3A_funcs = {
    {
        {0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_6_SLOTS},
        gDummy3AUiResourceAcquire,
        Dummy3A_release,
    },
    {
        NULL,
        gDummy3AUiResourceFrameStart,
        Dummy3A_frameEnd,
        gDummy3AUiResourceDraw,
    },
};
