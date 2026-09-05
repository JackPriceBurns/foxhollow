#include "test_support.h"
#include "../game/src/dlls/objects/325_CloudPrison/CloudPrison.c"

RomCurveInterface** gRomCurveInterface;
static GameObject* senders;
static int popped;

int ObjMsg_Pop(GameObject* obj, u32* message, uintptr_t* sender, u32* param) {
    FH_CHECK(obj == &senders[0]);
    if (popped == 10) {
        return 0;
    }
    *message = 0xabc00 + popped;
    *sender = (uintptr_t)&senders[popped];
    *param = 100 + popped;
    popped++;
    return 1;
}

u32 ObjMsg_SendToObject(GameObject* obj, u32 message, GameObject* sender, uintptr_t param) {
    FH_UNEXPECTED_CALL();
    return 0;
}

int main(void) {
    // Given a full queue of ten distinct messages from objects allocated above 4 GB.
    senders = fh_test_alloc_high(10, sizeof(*senders));
    gCloudPrisonControlNeedsCurveLookup = 0;

    // When CloudPrison defers the messages during its update.
    CloudPrisonControl_update(&senders[0]);

    // Then each record retains its message, full sender pointer, and parameter without overlapping its neighbours.
    FH_CHECK(popped == 10);
    FH_CHECK(gCloudPrisonDeferredMessageCount == 10);
    CloudPrisonDeferredMessage* messages = (CloudPrisonDeferredMessage*)gCloudPrisonDeferredMessageStorage;
    for (int i = 0; i < 10; i++) {
        FH_CHECK(messages[i].messageId == 0xabc00 + i);
        FH_CHECK(messages[i].sender == &senders[i]);
        FH_CHECK(messages[i].data == 100 + i);
    }
    free(senders);
    return EXIT_SUCCESS;
}
