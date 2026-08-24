#include "foxhollow_mod_api.h"
#include <stdio.h>

extern float gCameraProjectionMatrix[4][4];
extern float gCameraLightPerspectiveMatrix[3][4];
extern float gCameraLightPerspectiveFlipYMatrix[3][4];
extern float gCameraLightPerspectiveScaledMatrix[3][4];
extern unsigned int gCameraProjectionMode;
extern void GXSetProjection(float (*matrix)[4], unsigned int mode);
extern void fhGXSetCullSwap(int enabled);
extern float gPauseMenuOpenAmount;

static FhMod* sMod;
static const FhModHost* sHost;

static int sSuppressMirror;
static void (*sOrigPauseMenuDraw)(int, int, int);
static void (*sOrigUpdateProjection)(void*, int);
static void (*sOrigRebuild)(void);
static signed char (*sOrigStickX)(int);

static void say(const char* m) { sHost->log(sMod, FH_LOG_INFO, m); }

static void negate_row0(void) {
  if (sSuppressMirror) {
    return;
  }
  for (int i = 0; i < 4; i++) {
    gCameraProjectionMatrix[0][i] = -gCameraProjectionMatrix[0][i];
    gCameraLightPerspectiveMatrix[0][i] = -gCameraLightPerspectiveMatrix[0][i];
    gCameraLightPerspectiveFlipYMatrix[0][i] = -gCameraLightPerspectiveFlipYMatrix[0][i];
    gCameraLightPerspectiveScaledMatrix[0][i] = -gCameraLightPerspectiveScaledMatrix[0][i];
  }
  GXSetProjection(gCameraProjectionMatrix, gCameraProjectionMode);
}

static void update_projection_hook(void* viewport, int unused) {
  sOrigUpdateProjection(viewport, unused);
  negate_row0();
}

static void rebuild_hook(void) {
  sOrigRebuild();
  negate_row0();
}

static void pause_menu_draw_hook(int a, int b, int c) {
  sSuppressMirror = 1;
  fhGXSetCullSwap(0);
  sOrigPauseMenuDraw(a, b, c);
  fhGXSetCullSwap(1);
  sSuppressMirror = 0;
}

static signed char stick_x_hook(int port) {
  if (gPauseMenuOpenAmount > 0.5f) {
    return sOrigStickX(port);
  }
  const int value = -(int)sOrigStickX(port);
  return (signed char)(value < -128 ? -128 : (value > 127 ? 127 : value));
}

static int hook(const char* name, void* replacement, void** original) {
  void* target = sHost->symbolAddress(sMod, name);
  char buf[160];
  if (target == NULL) {
    snprintf(buf, sizeof(buf), "could not resolve %s", name);
    say(buf);
    return 0;
  }
  if (sHost->hookInstall(sMod, target, replacement, original) != FH_MOD_OK) {
    snprintf(buf, sizeof(buf), "could not hook %s (no patch pad?)", name);
    say(buf);
    return 0;
  }
  snprintf(buf, sizeof(buf), "hooked %s", name);
  say(buf);
  return 1;
}

FH_MOD_EXPORT int fh_mod_initialize(FhMod* mod, const FhModHost* host) {
  sMod = mod;
  sHost = host;
  if (host->abiVersion < 2) { return FH_MOD_ERROR; }

  int ok = 1;
  ok &= hook("Camera_UpdateProjection", (void*)update_projection_hook, (void**)&sOrigUpdateProjection);
  ok &= hook("Camera_RebuildProjectionMatrix", (void*)rebuild_hook, (void**)&sOrigRebuild);
  ok &= hook("padGetStickX", (void*)stick_x_hook, (void**)&sOrigStickX);
  ok &= hook("pauseMenuDraw", (void*)pause_menu_draw_hook, (void**)&sOrigPauseMenuDraw);
  if (!ok) { return FH_MOD_ERROR; }

  fhGXSetCullSwap(1);
  say("mirrored");
  return FH_MOD_OK;
}

FH_MOD_EXPORT void fh_mod_update(FhMod* mod) { (void)mod; }
FH_MOD_EXPORT void fh_mod_shutdown(FhMod* mod) {
  (void)mod;
  fhGXSetCullSwap(0);
}
