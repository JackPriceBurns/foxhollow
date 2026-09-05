#include "test_support.h"
#include "../game/src/dlls/objects/448_DIMLogFire/DIMLogFire.c"

static GameObject* fire;
static GameObject* child;
static ModelLightStruct* light;
static int effectsFreed;
static int childrenFreed;
static int lightsFreed;
static int groupsFreed;

static void freeEffects(uintptr_t source) {
    FH_CHECK(source == (uintptr_t)fire);
    effectsFreed++;
}

static ExpgfxInterface effects = {.freeSource2 = freeEffects};
static ExpgfxInterface* effectsPointer = &effects;
ExpgfxInterface** gExpgfxInterface = &effectsPointer;

void Obj_FreeObject(GameObject* obj) {
    FH_CHECK(obj == child);
    childrenFreed++;
}

void ModelLightStruct_free(ModelLightStruct* value) {
    FH_CHECK(value == light);
    lightsFreed++;
}

void objFreeObjectType(GameObject* obj, int group) {
    FH_CHECK(obj == fire && group == 0x31);
    groupsFreed++;
}

int main(void) {
    // Given a log fire with separately allocated child and light objects above 4 GB.
    fire = fh_test_alloc_high(1, sizeof(*fire));
    child = fh_test_alloc_high(1, sizeof(*child));
    light = fh_test_alloc_high(1, sizeof(*light));
    DimLogFireState* state = fh_test_alloc_high(1, DIMLogFire_getExtraSize());
    fire->extra = state;
    state->subObject = child;
    state->light = light;

    // When normal cleanup runs.
    DIMLogFire_free(fire, 0);

    // Then the original child, light, effect source and group membership are released once.
    FH_CHECK(childrenFreed == 1 && lightsFreed == 1 && effectsFreed == 1 && groupsFreed == 1);

    // Given another cleanup scenario using the mode that preserves the child.
    childrenFreed = lightsFreed = effectsFreed = groupsFreed = 0;

    // When cleanup runs with a nonzero free mode.
    DIMLogFire_free(fire, 1);

    // Then effects, group membership and light are released while the child is preserved.
    FH_CHECK(childrenFreed == 0 && lightsFreed == 1 && effectsFreed == 1 && groupsFreed == 1);

    // Given a fire without a child or light.
    state->subObject = NULL;
    state->light = NULL;
    childrenFreed = lightsFreed = effectsFreed = groupsFreed = 0;

    // When normal cleanup runs.
    DIMLogFire_free(fire, 0);

    // Then only effects and group membership are released.
    FH_CHECK(childrenFreed == 0 && lightsFreed == 0 && effectsFreed == 1 && groupsFreed == 1);

    // Given five units of remaining strength.
    state->remainingStrength = 5;

    // When successive commands consume less than, exactly, and more than the remaining strength.
    int pending = dimlogfire_countdownCallback(fire, 2);
    int exhausted = dimlogfire_countdownCallback(fire, 3);
    int overshot = dimlogfire_countdownCallback(fire, 1);

    // Then zero and negative strength both report completion, and the negative value remains stored.
    FH_CHECK(pending == 0 && exhausted == 1 && overshot == 1);
    FH_CHECK(state->remainingStrength == -1);
    free(state);
    free(light);
    free(child);
    free(fire);
    return EXIT_SUCCESS;
}
