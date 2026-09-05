#include "test_support.h"
#include "../game/src/main/pi_dolphin.c"

int main(int argc, char** argv) {
    // Given synthetic animation tables with only a secondary unmarked placeholder available.
    int primary[3000] = {0};
    int secondary[3000] = {0};
    const int animation = 7;
    secondary[animation] = 0x2220;
    gResourceFileBuffers[0x2f] = 0;
    gResourceFileBuffers[0x49] = (uintptr_t)secondary;

    // When the resident animation banks are merged.
    FH_CHECK(mergeTableFiles(MLDF_MERGE_ANIM, 0x2f, 0x49, 3000) == 1);

    // Then the placeholder survives, preserving the selection that can cause retail's disguise T-pose.
    FH_CHECK(MLDF_MERGE_ANIM[animation] == 0x2220);
    FH_CHECK(MLDF_MERGE_ANIM[2999] == UINT32_MAX);

    // Given a full primary animation becomes resident alongside that placeholder.
    primary[animation] = 0x10001110;
    gResourceFileBuffers[0x2f] = (uintptr_t)primary;

    // When the animation banks are merged again.
    FH_CHECK(mergeTableFiles(MLDF_MERGE_ANIM, 0x2f, 0x49, 3000) == 1);

    // Then the full primary animation takes priority.
    FH_CHECK(MLDF_MERGE_ANIM[animation] == 0x10001110);

    // Given the primary bank has a placeholder and the secondary bank has a full animation.
    primary[animation] = 0x1110;
    secondary[animation] = 0x10002220;

    // When those animation banks are merged.
    FH_CHECK(mergeTableFiles(MLDF_MERGE_ANIM, 0x2f, 0x49, 3000) == 1);

    // Then the full secondary animation wins and retains the secondary source selection.
    FH_CHECK(MLDF_MERGE_ANIM[animation] == 0x20002220);

    // Given both banks now offer full animations.
    primary[animation] = 0x10001110;

    // When those animation banks are merged.
    FH_CHECK(mergeTableFiles(MLDF_MERGE_ANIM, 0x2f, 0x49, 3000) == 1);

    // Then retail resolves the tie in favour of the primary bank.
    FH_CHECK(MLDF_MERGE_ANIM[animation] == 0x10001110);

    // Given the full primary bank unloads and only the secondary placeholder remains again.
    gResourceFileBuffers[0x2f] = 0;
    secondary[animation] = 0x2220;

    // When the animation table is rebuilt after unloading.
    FH_CHECK(mergeTableFiles(MLDF_MERGE_ANIM, 0x2f, 0x49, 3000) == 1);

    // Then it returns to the placeholder instead of retaining an animation from the unloaded bank.
    FH_CHECK(MLDF_MERGE_ANIM[animation] == 0x2220);
    return EXIT_SUCCESS;
}
