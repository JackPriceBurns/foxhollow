#include "main/gameloop_internal.h"
#include "main/gameloop_api.h"

int getButtonObjects(GameObject*** p) {
    *p = gGameLoopButtonObjects;
    return gGameLoopButtonObjectCount;
}

void removeButtonObject(GameObject* object) {
    int removeIndex = -1;
    GameObject** buttonObjects = gGameLoopButtonObjects;
    int buttonObjectCount = gGameLoopButtonObjectCount;
    for (int i = 0; i < buttonObjectCount; i++) {
        if (*buttonObjects == object) {
            removeIndex = i;
            break;
        }
        buttonObjects++;
    }
    for (int i = removeIndex; i < buttonObjectCount - 1; i++) {
        gGameLoopButtonObjects[i] = gGameLoopButtonObjects[i + 1];
    }
    gGameLoopButtonObjectCount--;
}
