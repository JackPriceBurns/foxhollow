#include "test_support.h"
#include "../game/src/dlls/engine/25/25.c"

__typeof__(gTitleMenuControlInterfaceCopy) gTitleMenuControlInterfaceCopy;
static void* expectedPath;
static RouteState* expectedRoute;
static int pathsReleased;
static int routesReleased;

void Sfx_StopObjectChannel(GameObject* obj, int channel) {
    FH_CHECK(channel == 127);
}

void voxmaps_freeRouteWork(RouteState* state) {
    FH_CHECK(state == expectedRoute);
    routesReleased++;
}

void mm_free(void* ptr) {
    FH_CHECK(ptr == expectedPath);
    pathsReleased++;
    free(ptr);
}

int main(void) {
    // Given an enemy whose path allocation lies above 4 GB.
    GameObject obj = {0};
    GroundBaddieState state = {0};
    expectedPath = fh_test_alloc_high(1, 1024);
    expectedRoute = &state.routeState;
    state.path = expectedPath;
    state.configFlags = 1;

    // When the enemy releases its state.
    dll_19_releaseState(&obj, &state, 1);

    // Then cleanup frees the original path and clears the reference.
    FH_CHECK(pathsReleased == 1);
    FH_CHECK(routesReleased == 1);
    FH_CHECK(state.path == NULL);

    // When cleanup runs again with the cleared path.
    dll_19_releaseState(&obj, &state, 1);

    // Then the path allocation is not freed a second time.
    FH_CHECK(pathsReleased == 1);
    FH_CHECK(state.path == NULL);
    return EXIT_SUCCESS;
}
