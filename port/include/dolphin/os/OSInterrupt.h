#ifndef FOXHOLLOW_SHIM_OSINTERRUPT_H
#define FOXHOLLOW_SHIM_OSINTERRUPT_H

#include <dolphin/types.h>
#include_next <dolphin/os/OSInterrupt.h>

#ifdef __cplusplus
extern "C" {
#endif
BOOL OSEnableInterrupts(void);
BOOL OSDisableInterrupts(void);
BOOL OSRestoreInterrupts(BOOL level);
#ifdef __cplusplus
}
#endif

#endif
