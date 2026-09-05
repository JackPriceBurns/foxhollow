#include "test_support.h"
#include "../game/src/dlls/objects/381/381.c"

static GameObject* barrel;
static GameObject* members[3];
static int memberCount;
static int removals;
static int renders;

GameObject** objGetAllOfType(int group, int* count) {
    FH_CHECK(group == 0x2f);
    *count = memberCount;
    return members;
}

void objFreeObjectType(GameObject* obj, int group) {
    FH_CHECK(obj == barrel && group == 0x2f);
    removals++;
}

void objRenderModelAndHitVolumes(GameObject* obj, int a, int b, int c, int d, f32 scale) {
    FH_CHECK(obj == barrel && a == 2 && b == 3 && c == 4 && d == 5 && scale == 1);
    renders++;
}

int main(void) {
    // Given an exploded barrel second in a native group list, with two explosions outstanding.
    barrel = fh_test_alloc_high(1, sizeof(*barrel));
    GameObject other = {0};
    RollingBarrelState state = {0};
    barrel->extra = &state;
    state.mode = 1;
    members[0] = &other;
    members[1] = barrel;
    members[2] = barrel;
    memberCount = 3;
    gRollingBarrelExplodingCount = 2;

    // When the barrel is freed.
    rollingBarrel_free(barrel);

    // Then group removal occurs once even with a duplicate, and one outstanding explosion is removed.
    FH_CHECK(removals == 1 && gRollingBarrelExplodingCount == 1);

    // Given a rolling barrel absent from the group.
    state.mode = 0;
    memberCount = 1;
    removals = 0;

    // When cleanup runs for that barrel.
    rollingBarrel_free(barrel);

    // Then no membership is removed and the explosion count is unchanged.
    FH_CHECK(removals == 0 && gRollingBarrelExplodingCount == 1);

    for (int mode = 0; mode <= 3; mode++) {
        for (int visible = 0; visible <= 1; visible++) {
            // Given each barrel lifecycle state, both visible and culled.
            state.mode = mode;
            renders = 0;

            // When the renderer visits the barrel.
            rollingBarrel_render(barrel, 2, 3, 4, 5, visible);

            // Then only a visible rolling barrel submits its model.
            FH_CHECK(renders == (visible && mode == 0));
        }
    }
    free(barrel);
    return EXIT_SUCCESS;
}
