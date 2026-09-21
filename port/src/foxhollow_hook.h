#ifndef FOXHOLLOW_HOOK_H_
#define FOXHOLLOW_HOOK_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FH_HOOK_PATCH_BYTES 16

void* fhHookResolveSymbol(const char* name);
int fhHookInstall(void* target, void* replacement, void** outOriginal);
int fhHookRemove(void* target);
int fhHookTargetIsPatchable(const void* target);

#ifdef __cplusplus
}
#endif

#endif /* FOXHOLLOW_HOOK_H_ */
