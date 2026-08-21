#include <aurora/aurora.h>
#include <aurora/dvd.h>
#include <aurora/event.h>
#include <aurora/main.h>
#include <dolphin/gx/GXAurora.h>

#include "foxhollow_config.h"

#include <stdio.h>
#include <stdlib.h>

int gameMain(int argc, char** argv);
void foxhollowFramePumpInit(void);

static void log_callback(AuroraLogLevel level, const char* module, const char* message, unsigned int len) {
  const char* levelStr;
  FILE* out = stdout;
  switch (level) {
  case LOG_DEBUG:
    levelStr = "DEBUG";
    break;
  case LOG_INFO:
    levelStr = "INFO";
    break;
  case LOG_WARNING:
    levelStr = "WARNING";
    break;
  case LOG_ERROR:
    levelStr = "ERROR";
    out = stderr;
    break;
  case LOG_FATAL:
    levelStr = "FATAL";
    out = stderr;
    break;
  }
  fprintf(out, "[%s] %s: %s\n", levelStr, module, message);
  if (level == LOG_FATAL) {
    fflush(out);
    abort();
  }
}

static const char* disc_path(int argc, char* argv[]) {
  if (argc > 1) {
    return argv[1];
  }
  return getenv("FOXHOLLOW_DISC");
}

int main(int argc, char* argv[]) {
  const char* disc = disc_path(argc, argv);
  if (!disc) {
    fprintf(stderr, "usage: foxhollow <path-to-disc.iso|rvz>\n(or set FOXHOLLOW_DISC)\n");
    return 1;
  }

  const AuroraConfig config = {
      .appName = "Foxhollow",
      .logCallback = &log_callback,
      .vsync = fhConfigVsync() != 0,
      .startFullscreen = fhConfigFullscreen() != 0,
      .mem1Size = 128 * 1024 * 1024,
      .mem2Size = ARAM_DEFAULT_SIZE,
  };
  aurora_initialize(argc, argv, &config);

  AuroraSetViewportPolicy(AURORA_VIEWPORT_FIT);
  AuroraSetDisplayAspect(fhConfigDisplayAspect());

  if (!aurora_dvd_open(disc)) {
    fprintf(stderr, "foxhollow: failed to open disc image: %s\n", disc);
    aurora_shutdown();
    return 1;
  }

  foxhollowFramePumpInit();
  return gameMain(argc, argv);
}
