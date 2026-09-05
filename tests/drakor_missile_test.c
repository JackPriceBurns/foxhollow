#include "test_support.h"
#include "../game/src/dlls/objects/610/610.c"

static GameObject* missile;
static int frees;

void Obj_FreeObject(GameObject* obj) {
    FH_CHECK(obj == missile);
    frees++;
}

int main(void) {
    missile = fh_test_alloc_high(1, sizeof(*missile));
    DrakorMissileState state = {0};
    missile->extra = &state;
    for (int mode = 0; mode <= 4; mode++) {
        // Given a missile in each retail flight state with another flag already set.
        state.state = mode;
        state.flags = 0x80;
        frees = 0;

        // When destruction is requested.
        drakormissile_requestFree(missile);

        // Then the request flag is added without losing other flags, and only fadeout frees immediately.
        FH_CHECK(state.flags == 0x81 && state.state == mode);
        FH_CHECK(frees == (mode == 1));
        FH_CHECK(drakormissile_isFadingOut(missile) == (mode == 1));

        // When straight-flight cancellation is requested in that state.
        drakormissile_abortStraightFlight(missile);

        // Then only a straight missile enters the explosion state, with no additional immediate free.
        FH_CHECK(state.state == (mode == 3 ? 2 : mode));
        FH_CHECK(frees == (mode == 1));
    }
    free(missile);
    return EXIT_SUCCESS;
}
