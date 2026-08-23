#include "foxhollow_mod_api.h"

#include <stdio.h>

extern float Camera_GetFovY(void);
extern void Camera_SetFovY(float fovY);
extern void* Obj_GetPlayerObject(void);

#define CAMCONTROL_CLASS_ID 1
#define FOV_TARGET 90.0f

static FhMod* sMod;
static const FhModHost* sHost;
static void (*sOriginalCameraUpdate)(unsigned char framesThisStep);
static int sReported;

static void say(FhLogLevel level, const char* message) {
  sHost->log(sMod, level, message);
}

static void camera_update_hook(unsigned char framesThisStep) {
  sOriginalCameraUpdate(framesThisStep);

  const float vanillaFovY = Camera_GetFovY();
  Camera_SetFovY(FOV_TARGET);

  if (!sReported) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "interposed Camera_update; vanilla fovY %.2f -> %.2f", vanillaFovY, FOV_TARGET);
    say(FH_LOG_INFO, buffer);
    sReported = 1;
  }
}

FH_MOD_EXPORT int fh_mod_initialize(FhMod* mod, const FhModHost* host) {
  sMod = mod;
  sHost = host;

  if (host->abiVersion < FH_MOD_ABI_VERSION) {
    return FH_MOD_ERROR;
  }

  char buffer[256];
  snprintf(buffer, sizeof(buffer), "hello from %s (abi %u, %u classes)", host->modId(mod), host->abiVersion,
           host->classCount(mod));
  say(FH_LOG_INFO, buffer);

  snprintf(buffer, sizeof(buffer), "called into game code: Camera_GetFovY() = %.2f, Obj_GetPlayerObject() = %p",
           Camera_GetFovY(), Obj_GetPlayerObject());
  say(FH_LOG_INFO, buffer);

  FhClassCallback original = 0;
  if (host->classReplaceCallback(mod, CAMCONTROL_CLASS_ID, FH_SLOT_UPDATE, (FhClassCallback)camera_update_hook,
                                 &original) != FH_MOD_OK) {
    say(FH_LOG_ERROR, "could not interpose the camcontrol update slot");
    return FH_MOD_ERROR;
  }
  sOriginalCameraUpdate = (void (*)(unsigned char))original;
  return FH_MOD_OK;
}

FH_MOD_EXPORT void fh_mod_update(FhMod* mod) {
  const uint64_t frame = sHost->frameCount(mod);
  if (frame % 600 != 0) {
    return;
  }
  char buffer[128];
  snprintf(buffer, sizeof(buffer), "frame %llu, fovY %.2f", (unsigned long long)frame, Camera_GetFovY());
  say(FH_LOG_INFO, buffer);
}

FH_MOD_EXPORT void fh_mod_shutdown(FhMod* mod) {
  (void)mod;
  say(FH_LOG_INFO, "goodbye");
}
