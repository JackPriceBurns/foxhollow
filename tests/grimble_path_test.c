#include "test_support.h"
#include "../game/src/dlls/objects/208_Grimble/Grimble.c"

static GameObject* paths[2];
static int queries;
static int initialises;
static int samples;
static int rotations;

GameObject** objGetAllOfType(int group, int* count) {
    FH_CHECK(group == 0x17);
    *count = 2;
    return paths;
}

int randomGetRange(int min, int max) {
    FH_CHECK(min == 10 && max == 60);
    return 10;
}

static int nearest(GameObject* obj, f32 x, f32 y, f32 z, f32* distance, f32* progress, f32* auxiliary) {
    FH_CHECK(queries < 2);
    FH_CHECK(obj == paths[queries]);
    FH_CHECK(x == 15 && y == 25 && z == 35);
    *distance = obj == paths[0] ? 120 : 40;
    *progress = obj == paths[0] ? 5 : 3;
    *auxiliary = 0;
    queries++;
    return 1;
}

static void initialise(GameObject* obj, void* state) {
    FH_CHECK(obj == paths[1]);
    FH_CHECK(state != NULL);
    initialises++;
}

static void sample(GameObject* obj, f32 progress, f32* x, f32* y, f32* z) {
    FH_CHECK(obj == paths[1] && progress == 3);
    *x = 50;
    *y = 60;
    *z = 70;
    samples++;
}

static s16 rotation(GameObject* obj) {
    FH_CHECK(obj == paths[1]);
    rotations++;
    return 123;
}

int main(void) {
    // Given Grimble and two high-address paths, with the nearer path second in the list.
    GameObject* obj = fh_test_alloc_high(1, sizeof(*obj));
    GroundBaddieState* state = fh_test_alloc_high(1, grimble_getExtraSize());
    GrimbleControl* control = (GrimbleControl*)(state + 1);
    void* slots[14] = {0};
    ObjectInterface* interface = (ObjectInterface*)slots;
    paths[0] = fh_test_alloc_high(1, sizeof(GameObject));
    paths[1] = fh_test_alloc_high(1, sizeof(GameObject));
    slots[8] = initialise;
    slots[9] = sample;
    slots[12] = nearest;
    slots[13] = rotation;
    paths[0]->anim.dll = &interface;
    paths[1]->anim.dll = &interface;
    state->control = control;
    obj->extra = state;
    obj->anim.localPosX = 15;
    obj->anim.localPosY = 25;
    obj->anim.localPosZ = 35;

    // When Grimble attaches to the nearest path using a deterministic random choice.
    grimble_attachNearestPath(obj);

    // Then it selects that path and obtains its position and rotation through the correct callbacks.
    FH_CHECK(control->pathObj == paths[1]);
    FH_CHECK(control->candidatePathObj == paths[1]);
    FH_CHECK(queries == 2 && initialises == 1 && samples == 1 && rotations == 1);
    FH_CHECK(control->nearestDist == 40 && control->pathProgress == 3);
    FH_CHECK(control->baseRotX == 123);
    FH_CHECK(control->pathPosX == 50 && control->pathPosY == 60 && control->pathPosZ == 70);
    FH_CHECK(control->posYDelta == 35 && control->targetProgress == 2);
    free(paths[0]);
    free(paths[1]);
    free(state);
    free(obj);
    return EXIT_SUCCESS;
}
