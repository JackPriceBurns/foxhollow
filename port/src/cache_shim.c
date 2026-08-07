#include <dolphin/types.h>
#include "shim_log.h"

void DCFlushRange(void* addr, u32 nBytes) { (void)addr; (void)nBytes; }
void DCFlushRangeNoSync(void* addr, u32 nBytes) { (void)addr; (void)nBytes; }
void DCInvalidateRange(void* addr, u32 nBytes) { (void)addr; (void)nBytes; }
void DCStoreRange(void* addr, u32 nBytes) { (void)addr; (void)nBytes; }
void DCStoreRangeNoSync(void* addr, u32 nBytes) { (void)addr; (void)nBytes; }
void LCEnable(void) {}
void LCDisable(void) {}
void LCLoadBlocks(void* destTag, void* srcAddr, u32 numBlocks) {
  (void)destTag; (void)srcAddr; (void)numBlocks;
  FH_STUB_ONCE("LCLoadBlocks");
}
void LCStoreBlocks(void* destAddr, void* srcTag, u32 numBlocks) {
  (void)destAddr; (void)srcTag; (void)numBlocks;
  FH_STUB_ONCE("LCStoreBlocks");
}
u32 LCQueueWait(u32 len) { return len; }
