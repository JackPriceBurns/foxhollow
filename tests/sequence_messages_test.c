#include "test_support.h"
#include "../game/src/dlls/objects/323_FEseqobject/FEseqobject.c"

EffectInterface** gPartfxInterface;
static GameObject* receivers[2];
static GameObject* source;
static int popped;
static int sent;

void mainSetBits(int bit, int value) {
    FH_UNEXPECTED_CALL();
}

GameObject** objGetAllOfType(int group, int* count) {
    FH_CHECK(group == 3);
    *count = 2;
    return receivers;
}

int ObjMsg_Pop(GameObject* obj, u32* message, uintptr_t* sender, u32* param) {
    FH_CHECK(obj == source);
    if (popped == 3) {
        return 0;
    }
    *message = FESEQOBJECT_MESSAGE_IN_1 + popped;
    *sender = (uintptr_t)receivers[0];
    *param = 0;
    popped++;
    return 1;
}

u32 ObjMsg_SendToObject(GameObject* obj, u32 message, GameObject* sender, uintptr_t param) {
    FH_CHECK(obj == receivers[1]);
    FH_CHECK(sender == source && param == 0);
    FH_CHECK(message == FESEQOBJECT_MESSAGE_OUT_1 + sent);
    sent++;
    return 1;
}

int main(void) {
    // Given three queued sequence messages and a matching receiver second in a high-address object list.
    source = fh_test_alloc_high(1, sizeof(*source));
    receivers[0] = fh_test_alloc_high(1, sizeof(GameObject));
    receivers[1] = fh_test_alloc_high(1, sizeof(GameObject));
    receivers[1]->anim.romDefNo = 0xf7;
    ObjSeqState sequence = {0};

    // When the sequence callback processes the queue.
    FEseqobject_SeqFn(source, 0, &sequence);

    // Then all three messages reach the matching receiver with the sequence object as sender.
    FH_CHECK(popped == 3 && sent == 3);
    free(receivers[0]);
    free(receivers[1]);
    free(source);
    return EXIT_SUCCESS;
}
