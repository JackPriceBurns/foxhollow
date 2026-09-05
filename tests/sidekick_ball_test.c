#include "test_support.h"
#include "../game/src/dlls/objects/245_SidekickBal/SidekickBal.c"

PathControlInterface** gPathControlInterface;
static GameObject* ball;
static int enables;
static int syncs;
static int disables;

int fhConfigRevision(void) {
    return 0;
}

void ObjHits_EnableObject(GameObject* obj) {
    FH_CHECK(obj == ball);
    enables++;
}

void ObjHits_SyncObjectPositionIfDirty(GameObject* obj) {
    FH_CHECK(obj == ball && enables == 1);
    syncs++;
}

void ObjHits_DisableObject(GameObject* obj) {
    FH_CHECK(obj == ball);
    disables++;
}

int main(void) {
    // Given Tricky's ball above 4 GB with an old fade timer and a distinct launch position.
    ball = fh_test_alloc_high(1, sizeof(*ball));
    SidekickBallState* state = fh_test_alloc_high(1, SidekickBall_getExtraSize());
    ball->extra = state;
    ball->anim.localPosX = 11;
    ball->anim.localPosY = 22;
    ball->anim.localPosZ = 33;
    state->fadeTimer = 59;

    // When a throw supplies independent velocities on each axis.
    sidekickBall_launch(ball, NULL, 4, -5, 6);

    // Then the thrown ball uses those velocities, refreshes its lifetime, and starts collision tracking there.
    FH_CHECK(state->ballMode == 3 && state->fadeTimer == 0);
    FH_CHECK(ball->anim.velocityX == 4 && ball->anim.velocityY == -5 && ball->anim.velocityZ == 6);
    FH_CHECK(state->previousPosX == 11 && state->previousPosY == 22 && state->previousPosZ == 33);
    FH_CHECK(state->path.subtype == 1 && enables == 1 && syncs == 1);

    // Given each retail ball mode and a running fade timer.
    const int refreshes[] = {0, 0, 1, 1, 0};
    const int heldOrMoving[] = {0, 1, 1, 0, 0};
    for (int mode = 0; mode < 5; mode++) {
        state->ballMode = mode;
        state->fadeTimer = 17;

        // When its activity queries and keep-alive run.
        int active = sidekickBall_isHeldOrMoving(ball);
        sidekickBall_keepAlive(ball);

        // Then only held or thrown balls refresh, while the activity query includes held and moving modes.
        FH_CHECK(active == heldOrMoving[mode]);
        FH_CHECK(state->fadeTimer == (refreshes[mode] ? 0 : 17));
    }

    // When the ball returns to idle.
    sidekickBall_setIdle(ball, NULL);

    // Then collision is disabled and its fade and path state reset.
    FH_CHECK(sidekickBall_isIdle(ball) == 1 && state->ballMode == 0);
    FH_CHECK(state->fadeTimer == 0 && state->path.subtype == 0 && disables == 1);
    free(state);
    free(ball);
    return EXIT_SUCCESS;
}
