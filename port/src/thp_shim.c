#include <dolphin/types.h>
#include "shim_log.h"

BOOL THPInit(void) { return 1; }

s32 THPVideoDecode(void* file, void* tileY, void* tileU, void* tileV, void* work) {
  (void)file; (void)tileY; (void)tileU; (void)tileV; (void)work;
  FH_STUB_ONCE("THPVideoDecode (movies not implemented; skipped)");
  return 0;
}

s32 THPAudioDecode(s16* buffer, u8* audioFrame, s32 flag, s32 track) {
  (void)buffer; (void)audioFrame; (void)flag; (void)track;
  FH_STUB_ONCE("THPAudioDecode (movies not implemented; skipped)");
  return 0;
}
