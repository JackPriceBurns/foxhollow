#include "test_support.h"
#include "../game/src/dlls/objects/279_AppleOnTree/AppleOnTree.c"

static uintptr_t received;
static int releases;

static void record(uintptr_t source) {
    received = source;
    releases++;
}

static ExpgfxInterface callbacks = {.freeSource = record};
static ExpgfxInterface* interface = &callbacks;
ExpgfxInterface** gExpgfxInterface = &interface;

int main(void) {
    // Given an effect owner allocated above the 32-bit address range.
    GameObject* obj = fh_test_alloc_high(1, sizeof(*obj));

    // When the object releases its effects.
    AppleOnTree_free(obj);

    // Then the effect system receives the complete owner address exactly once.
    FH_CHECK(received == (uintptr_t)obj);
    FH_CHECK(releases == 1);
    free(obj);
    return EXIT_SUCCESS;
}
