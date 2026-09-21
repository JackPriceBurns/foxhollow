#include "foxhollow_config.h"

#include <stdlib.h>
#include <string.h>

#define FH_ASPECT_NARROW (4.0f / 3.0f)
#define FH_ASPECT_WIDE (16.0f / 9.0f)
#define FH_DEFAULT_FRAME_LIMIT 60

static int sLoaded;
static FhScreenStyle sScreenStyle;
static int sFullscreen;
static int sVsync = 1;
static int sFrameLimit = FH_DEFAULT_FRAME_LIMIT;
static int sRevision;
static char sMemoryCardPath[1024];
static int sHasMemoryCardPath;
static char sAutosavePath[1024];
static int sHasAutosavePath;

static int read_flag(const char* name, int fallback) {
  const char* value = getenv(name);
  if (value == NULL || value[0] == '\0') {
    return fallback;
  }
  if (strcmp(value, "0") == 0 || strcmp(value, "false") == 0 || strcmp(value, "off") == 0) {
    return 0;
  }
  return 1;
}

static int read_path(const char* name, char* out, size_t capacity) {
  const char* value = getenv(name);
  size_t length;
  if (value == NULL || value[0] == '\0') {
    return 0;
  }
  length = strlen(value);
  if (length + 1 >= capacity) {
    return 0;
  }
  memcpy(out, value, length);
  out[length] = '\0';
  return 1;
}

static void load(void) {
  const char* style;
  const char* frameLimit;
  const char* revision;

  if (sLoaded) {
    return;
  }
  sLoaded = 1;

  style = getenv("FOXHOLLOW_SCREEN_STYLE");
  if (style != NULL) {
    if (strcmp(style, "wide") == 0) {
      sScreenStyle = FH_SCREEN_STYLE_WIDE;
    } else if (strcmp(style, "narrow") == 0) {
      sScreenStyle = FH_SCREEN_STYLE_NARROW;
    }
  }

  sFullscreen = read_flag("FOXHOLLOW_FULLSCREEN", 0);
  sVsync = read_flag("FOXHOLLOW_VSYNC", 1);

  revision = getenv("FOXHOLLOW_REV");
  if (revision != NULL && revision[0] != '\0') {
    if (strcmp(revision, "0") == 0) {
      sRevision = 0;
    } else if (strcmp(revision, "1") == 0) {
      sRevision = 1;
    } else {
      sRevision = -1;
    }
  }

  sHasMemoryCardPath = read_path("FOXHOLLOW_MEMORY_CARD", sMemoryCardPath, sizeof(sMemoryCardPath));
  sHasAutosavePath = read_path("FOXHOLLOW_AUTOSAVE", sAutosavePath, sizeof(sAutosavePath));

  frameLimit = getenv("FOXHOLLOW_FRAME_LIMIT");
  if (frameLimit != NULL && frameLimit[0] != '\0') {
    sFrameLimit = atoi(frameLimit);
    if (sFrameLimit < 0) {
      sFrameLimit = FH_DEFAULT_FRAME_LIMIT;
    }
  }
}

FhScreenStyle fhConfigScreenStyle(void) {
  load();
  return sScreenStyle;
}

int fhConfigScreenStyleIsForced(void) { return fhConfigScreenStyle() != FH_SCREEN_STYLE_AUTO; }

int fhConfigScreenStyleIsWide(void) { return fhConfigScreenStyle() == FH_SCREEN_STYLE_WIDE; }

f32 fhConfigDisplayAspect(void) {
  switch (fhConfigScreenStyle()) {
  case FH_SCREEN_STYLE_WIDE:
    return FH_ASPECT_WIDE;
  case FH_SCREEN_STYLE_NARROW:
    return FH_ASPECT_NARROW;
  default:
    return FH_ASPECT_NARROW;
  }
}

int fhConfigFullscreen(void) {
  load();
  return sFullscreen;
}

int fhConfigVsync(void) {
  load();
  return sVsync;
}

int fhConfigFrameLimit(void) {
  load();
  return sFrameLimit;
}

int fhConfigRevision(void) {
  load();
  return sRevision;
}

const char* fhConfigMemoryCardPath(void) {
  load();
  return sHasMemoryCardPath ? sMemoryCardPath : NULL;
}

const char* fhConfigAutosavePath(void) {
  load();
  return sHasAutosavePath ? sAutosavePath : NULL;
}
