#ifndef FOXHOLLOW_UI_H_
#define FOXHOLLOW_UI_H_

#include "foxhollow_mod_api.h"

#ifdef __cplusplus
extern "C" {
#endif

void fhUiInit(void);
void fhUiShutdown(void);
void fhUiFrameBegin(void);
void fhUiDrawMod(FhMod* mod, const char* modId, FhModDrawFn draw);
void fhUiFrameEnd(void);

#ifdef __cplusplus
}
#endif

#endif /* FOXHOLLOW_UI_H_ */
