#include <dolphin/types.h>
#include "shim_log.h"

void modelAnimBuildJointMatrices(int* out, u8* dst, void* animState, u8* jointData, int jointCount,
                                 u8* scratch, int flags, int mode) {
  (void)out; (void)dst; (void)animState; (void)jointData; (void)jointCount;
  (void)scratch; (void)flags; (void)mode;
  FH_STUB_ONCE("modelAnimBuildJointMatrices (NOT DECOMPILED UPSTREAM; skeletal animation broken until implemented)");
}

typedef struct AXFX_REVSTD_WORK AXFX_REVSTD_WORK;

int ReverbSTDCreate(AXFX_REVSTD_WORK* rv, f32 coloration, f32 time, f32 mix, f32 damping, f32 predelay) {
  (void)rv; (void)coloration; (void)time; (void)mix; (void)damping; (void)predelay;
  FH_STUB_ONCE("ReverbSTDCreate (aux reverb disabled until Phase 5 audio)");
  return 0;
}

void ReverbSTDCallback(void* left, void* right, void* surround, void* context) {
  (void)left; (void)right; (void)surround; (void)context;
}
