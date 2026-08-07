#include <dolphin/os.h>
#include "shim_log.h"

static OSThread* sCurrentThread;

BOOL OSCreateThread(OSThread* thread, void* (*func)(void*), void* param, void* stack, u32 stackSize, s32 priority, u16 attributes) {
  (void)func; (void)param; (void)stack; (void)stackSize; (void)priority; (void)attributes;
  FH_STUB_ONCE("OSCreateThread (thread not started; single-threaded port)");
  if (thread) {
    sCurrentThread = thread;
  }
  return 1;
}

s32 OSResumeThread(OSThread* thread) {
  (void)thread;
  FH_STUB_ONCE("OSResumeThread");
  return 0;
}

s32 OSSuspendThread(OSThread* thread) {
  (void)thread;
  FH_STUB_ONCE("OSSuspendThread");
  return 0;
}

void OSCancelThread(OSThread* thread) { (void)thread; }
void OSSleepThread(OSThreadQueue* queue) { (void)queue; FH_STUB_ONCE("OSSleepThread"); }
void OSInitThreadQueue(OSThreadQueue* queue) { (void)queue; }
void OSWakeupThread(OSThreadQueue* queue) { (void)queue; }
OSThread* OSGetCurrentThread(void) { return sCurrentThread; }
