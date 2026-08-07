#ifndef MAIN_DLL_DF_DLL_0234_DFPERCHWITCH_H_
#define MAIN_DLL_DF_DLL_0234_DFPERCHWITCH_H_

#include "types.h"
#include "dlls/object_descriptor.h"

extern char sDfperchwitchInitNoLongerSupported[];
extern ObjectDescriptor gDfperchwitchObjDescriptor;

int dfperchwitch_getExtraSize(void);
int dfperchwitch_getObjectTypeId(void);
void dfperchwitch_free(void);
void dfperchwitch_render(void);
void dfperchwitch_hitDetect(void);
void dfperchwitch_update(void);
void dfperchwitch_init(void);
void dfperchwitch_release(void);
void dfperchwitch_initialise(void);

#endif /* MAIN_DLL_DF_DLL_0234_DFPERCHWITCH_H_ */
