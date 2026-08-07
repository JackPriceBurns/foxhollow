
#include "main/gameloop_internal.h"
#include "main/gameloop_api.h"

int getButtonObjects(GameObject*** p)
{
    *p = gGameLoopButtonObjects;
    return gGameLoopButtonObjectCount;
}
void removeButtonObject(GameObject* object)
{
    GameObject** buttonObjects;
    int buttonObjectCount;
    int objectIndex;
    int removeIndex;

    removeIndex = -1;
    objectIndex = 0;
    buttonObjects = gGameLoopButtonObjects;
    buttonObjectCount = gGameLoopButtonObjectCount;
    for (; objectIndex < buttonObjectCount; objectIndex++)
    {
        if (*buttonObjects == object)
        {
            removeIndex = objectIndex;
            break;
        }
        buttonObjects++;
    }
    for (objectIndex = removeIndex; objectIndex < buttonObjectCount - 1; objectIndex++)
    {
        gGameLoopButtonObjects[objectIndex] = gGameLoopButtonObjects[objectIndex + 1];
    }
    gGameLoopButtonObjectCount--;
}
