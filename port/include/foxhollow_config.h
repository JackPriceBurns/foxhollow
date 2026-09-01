#ifndef FOXHOLLOW_CONFIG_H_
#define FOXHOLLOW_CONFIG_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  FH_SCREEN_STYLE_AUTO = 0,
  FH_SCREEN_STYLE_NARROW,
  FH_SCREEN_STYLE_WIDE
} FhScreenStyle;

FhScreenStyle fhConfigScreenStyle(void);
int fhConfigScreenStyleIsForced(void);
int fhConfigScreenStyleIsWide(void);
f32 fhConfigDisplayAspect(void);
int fhConfigFullscreen(void);
int fhConfigVsync(void);
int fhConfigFrameLimit(void);
int fhConfigRevision(void);
const char* fhConfigMemoryCardPath(void);
const char* fhConfigAutosavePath(void);

#ifdef __cplusplus
}
#endif

#endif /* FOXHOLLOW_CONFIG_H_ */
