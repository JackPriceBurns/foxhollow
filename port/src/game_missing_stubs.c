#include <dolphin/types.h>
#include "shim_log.h"

typedef struct AXFX_REVSTD_WORK AXFX_REVSTD_WORK;

int ReverbSTDCreate(AXFX_REVSTD_WORK* rv, f32 coloration, f32 time, f32 mix, f32 damping, f32 predelay) {
    (void)rv;
    (void)coloration;
    (void)time;
    (void)mix;
    (void)damping;
    (void)predelay;
    FH_STUB_ONCE("ReverbSTDCreate (aux reverb disabled until Phase 5 audio)");
    return 0;
}

void ReverbSTDCallback(void* left, void* right, void* surround, void* context) {
    (void)left;
    (void)right;
    (void)surround;
    (void)context;
}
