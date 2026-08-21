#include <aurora/aurora.h>
#include <aurora/dvd.h>
#include <aurora/event.h>
#include <aurora/main.h>
#include <dolphin/dvd.h>
#include <dolphin/gx/GXAurora.h>

#include "foxhollow_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gameMain(int argc, char** argv);
void foxhollowFramePumpInit(void);

typedef struct FoxhollowDiscMapping {
  char gameName[4];
  unsigned char revision;
  const char* label;
} FoxhollowDiscMapping;

static const FoxhollowDiscMapping sDiscMappings[] = {
    {{'G', 'S', 'A', 'E'}, 0, "USA 1.0"},
    {{'G', 'S', 'A', 'E'}, 1, "USA 1.1"},
    {{'G', 'S', 'A', 'J'}, 0, "Japan 1.0"},
    {{'G', 'S', 'A', 'P'}, 0, "Europe 1.0"},
    {{'G', 'S', 'A', 'P'}, 1, "Europe 1.1"},
};

static const FoxhollowDiscMapping* disc_mapping(const DVDDiskID* id) {
  if (id == NULL || memcmp(id->company, "01", sizeof(id->company)) != 0 || id->diskNumber != 0) {
    return NULL;
  }

  for (size_t i = 0; i < sizeof(sDiscMappings) / sizeof(sDiscMappings[0]); i++) {
    const FoxhollowDiscMapping* mapping = &sDiscMappings[i];
    if (memcmp(id->gameName, mapping->gameName, sizeof(id->gameName)) == 0 && id->gameVersion == mapping->revision) {
      return mapping;
    }
  }

  return NULL;
}

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
  const FoxhollowDiscMapping* mapping;
  const int revision = fhConfigRevision();
  if (revision < 0) {
    fprintf(stderr, "foxhollow: FOXHOLLOW_REV must be 0 or 1\n");
    return 1;
  }
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

  mapping = disc_mapping(DVDGetCurrentDiskID());
  if (mapping == NULL) {
    const DVDDiskID* id = DVDGetCurrentDiskID();
    fprintf(stderr, "foxhollow: unsupported disc %.4s%.2s revision %u, disc %u\n", id->gameName, id->company,
            id->gameVersion, id->diskNumber);
    aurora_dvd_close();
    aurora_shutdown();
    return 1;
  }
  fprintf(stdout, "foxhollow: using %s disc with the GSAE01 asset mapping and USA 1.%d code\n", mapping->label,
          revision);

  foxhollowFramePumpInit();
  return gameMain(argc, argv);
}
