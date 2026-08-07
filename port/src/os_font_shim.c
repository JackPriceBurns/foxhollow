#include <dolphin/os.h>
#include "shim_log.h"

u16 OSGetFontEncode(void) { return 0; }

u32 OSLoadFont(OSFontHeader* fontData, void* tmp) {
  (void)fontData; (void)tmp;
  FH_STUB_ONCE("OSLoadFont");
  return 0;
}

char* OSGetFontTexel(const char* string, void* image, s32 pos, s32 stride, s32* width) {
  (void)image; (void)pos; (void)stride;
  if (width) {
    *width = 0;
  }
  FH_STUB_ONCE("OSGetFontTexel");
  return (char*)string;
}

char* OSGetFontWidth(const char* string, s32* width) {
  if (width) {
    *width = 0;
  }
  return (char*)string;
}
