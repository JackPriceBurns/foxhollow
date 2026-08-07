#ifndef MAIN_DLL_VF_PLATFORM1_H_
#define MAIN_DLL_VF_PLATFORM1_H_

#include "dlls/object_descriptor.h"

extern char sPlatform1DrawNoLongerSupported[];
extern char sPlatform1ControlNoLongerSupported[];
extern char sPlatform1InitNoLongerSupported[];
extern ObjectDescriptor gPlatform1ObjDescriptor;

int platform1_getExtraSize(void);
int platform1_getObjectTypeId(void);
void platform1_free(void);
void platform1_drawUnsupported(void);
void platform1_hitDetect(void);
void platform1_controlUnsupported(void);
void platform1_init(void);
void platform1_release(void);
void platform1_initialise(void);

#endif /* MAIN_DLL_VF_PLATFORM1_H_ */
