#include "test_support.h"
#include "../game/src/dlls/objects/241_InvHit/InvHit.c"

f32 timeDelta;
u8 framesThisStep;
EffectInterface** gPartfxInterface;
static GameObject* player;
static GameObject* tricky;
static GameObject* target;
static GameObject* helper;
static int frees;

GameObject* Obj_GetPlayerObject(void) {
    return player;
}

GameObject* getTrickyObject(void) {
    return tricky;
}

GameObject* playerGetTargetObject(GameObject* obj) {
    FH_CHECK(obj == player);
    return target;
}

int ObjList_ContainsObject(GameObject* obj) {
    FH_UNEXPECTED_CALL();
    return 0;
}

int trackGetHeight(GameObject* obj, f32 x, f32 y, f32 z, TrackGroundHit*** hits, int mode, int mask) {
    FH_UNEXPECTED_CALL();
    return 0;
}

void Obj_FreeObject(GameObject* obj) {
    FH_CHECK(obj == helper);
    frees++;
}

int main(void) {
    // Given a proximity helper, a player exactly on its radius, and Tricky just inside it.
    GameObject obj = {0};
    InvHitState state = {0};
    ObjHitsPriorityState hits = {0}, playerHits = {0}, trickyHits = {0};
    helper = &obj;
    player = fh_test_alloc_high(1, sizeof(*player));
    tricky = fh_test_alloc_high(1, sizeof(*tricky));
    obj.extra = &state;
    obj.anim.hitReactState = (ObjHitReactState*)&hits;
    obj.userData2 = 10;
    state.mode = 0;
    player->anim.classId = 1;
    player->anim.localPosX = 10;
    player->anim.hitReactState = (ObjHitReactState*)&playerHits;
    tricky->anim.localPosZ = 9;
    tricky->anim.hitReactState = (ObjHitReactState*)&trickyHits;
    playerHits.flags = trickyHits.flags = 5;

    // When proximity damage is evaluated.
    InvHit_update(&obj);

    // Then the radius is exclusive, only Tricky is hit, and unrelated flags survive.
    FH_CHECK(playerHits.priorityHitCount == 0 && playerHits.flags == 5);
    FH_CHECK(trickyHits.priorityHitCount == 1 && trickyHits.flags == 4);
    FH_CHECK(hits.priorityHitCount == 1);

    // Given an owner hit list containing only another object.
    state.mode = 7;
    obj.userData1 = (intptr_t)player;
    playerHits.priorityHitCount = 1;
    playerHits.hitObjects[0] = (uintptr_t)tricky;
    hits.flags = 5;

    // When the self-free helper checks that list.
    InvHit_update(&obj);

    // Then absence from the list leaves it alive, matching retail despite the source-file description.
    FH_CHECK(frees == 0 && hits.flags == 5);

    // When the owner reports the helper as its second hit.
    playerHits.priorityHitCount = 2;
    playerHits.hitObjects[1] = (uintptr_t)&obj;
    InvHit_update(&obj);

    // Then the matching entry disables and frees the helper exactly once.
    FH_CHECK(frees == 1 && hits.flags == 4);

    // Given a lock-on publisher with different local and world positions and no target.
    state.mode = 5;
    obj.anim.localPosX = 1;
    obj.anim.localPosY = 2;
    obj.anim.localPosZ = 3;
    obj.anim.worldPosX = 11;
    obj.anim.worldPosY = 22;
    obj.anim.worldPosZ = 33;
    gInvHitPublishedPos[0] = gInvHitPublishedPos[1] = gInvHitPublishedPos[2] = -1;

    // When the publisher updates without a lock-on target.
    InvHit_update(&obj);

    // Then it saves local position history but leaves the published world position untouched.
    FH_CHECK(obj.anim.previousLocalPosX == 1 && obj.anim.previousLocalPosY == 2 && obj.anim.previousLocalPosZ == 3);
    FH_CHECK(gInvHitPublishedPos[0] == -1 && gInvHitPublishedPos[1] == -1 && gInvHitPublishedPos[2] == -1);

    // When a high-address target becomes available.
    target = tricky;
    InvHit_update(&obj);

    // Then all three world coordinates are published.
    FH_CHECK(gInvHitPublishedPos[0] == 11 && gInvHitPublishedPos[1] == 22 && gInvHitPublishedPos[2] == 33);
    free(tricky);
    free(player);
    return EXIT_SUCCESS;
}
