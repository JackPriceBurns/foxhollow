#include "main/texture.h"
#include "sys/objects.h"
#include "dlls/object_descriptor.h"
#include "main/model_engine.h"
#include "main/objtype.h"
#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/printf.h"
#include "main/dll/dll_003F_dll3f.h"
#include "main/dll/SP/dll_0285_spshop.h"

#define DLL3F_TEXTURE_ID 0x47A

void* gDll3FTexture;

void dll_3F_updateTimerReadout(void* obj) {
    char buf[12];
    f32 maxDist = 1e+04f;
    int start = 0;
    int elapsed = 0;
    int total = 0;

    if (gameTimerIsRunning()) {
        gameTimerRun(obj);
    }
    GameObject* player = Obj_GetPlayerObject();
    GameObject* nearest = objGetNearestTypeTo(9, player, &maxDist);
    if (nearest != NULL) {
        SHOP_INTERFACE(nearest)->func17(nearest, &start, &elapsed, &total);
    }
    elapsed = total - (elapsed - start);
    if (elapsed < 0) {
        elapsed = 0;
    }
    sprintf(buf, "%2d", elapsed);
}

void dll_3F_frameEnd_nop(void) {
}

int dll_3F_frameStart_ret_0(void) {
    return 0;
}

void dll_3F_release(void) {
    textureFree(gDll3FTexture);
}

void dll_3F_initialise(void) {
    gDll3FTexture = textureLoadAsset(DLL3F_TEXTURE_ID);
}

UI_RESOURCE_ADAPTERS(gdll_3FUiResource, dll_3F_initialise, dll_3F_frameStart_ret_0, dll_3F_updateTimerReadout, (void*)(intptr_t)arg0)

UiResourceDescriptor dll_3F_funcs = {
    {
        {0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_6_SLOTS},
        gdll_3FUiResourceAcquire,
        dll_3F_release,
    },
    {
        NULL,
        gdll_3FUiResourceFrameStart,
        dll_3F_frameEnd_nop,
        gdll_3FUiResourceDraw,
    },
};
