#include <dolphin/os.h>
#include "shim_log.h"

void OSInitMessageQueue(OSMessageQueue* mq, OSMessage* msgArray, s32 msgCount) {
  mq->msgArray = msgArray;
  mq->msgCount = msgCount;
  mq->firstIndex = 0;
  mq->usedCount = 0;
}

BOOL OSSendMessage(OSMessageQueue* mq, OSMessage msg, s32 flags) {
  s32 lastIndex;
  if (mq->usedCount >= mq->msgCount) {
    if (flags & OS_MESSAGE_BLOCK) {
      FH_STUB_ONCE("OSSendMessage blocking on full queue (single-threaded port; dropping)");
    }
    return 0;
  }
  lastIndex = (mq->firstIndex + mq->usedCount) % mq->msgCount;
  mq->msgArray[lastIndex] = msg;
  mq->usedCount++;
  return 1;
}

BOOL OSReceiveMessage(OSMessageQueue* mq, OSMessage* msg, s32 flags) {
  if (mq->usedCount == 0) {
    if (flags & OS_MESSAGE_BLOCK) {
      FH_STUB_ONCE("OSReceiveMessage blocking on empty queue (single-threaded port; returning empty)");
    }
    return 0;
  }
  if (msg) {
    *msg = mq->msgArray[mq->firstIndex];
  }
  mq->firstIndex = (mq->firstIndex + 1) % mq->msgCount;
  mq->usedCount--;
  return 1;
}
