#ifndef DLLS_OBJECTS_445_H_
#define DLLS_OBJECTS_445_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

/*
 * Shared by the SC_paypoint and SPWell object definitions. Both retail
 * placement families use fixed nine-word (0x24-byte) records.
 */
typedef struct PaymentKioskPlacement {
    ObjPlacement base;
    s8 rotXByte;
    u8 unknown19;
    s16 price;
    u8 unknown1C[2];
    s16 gameBit;
    u8 unknown20[4];
} PaymentKioskPlacement;

typedef struct PaymentKioskState {
    u8 payState;    /* resolve, active, or paid */
    u8 textVariant; /* SC_paypoint or SPWell text-table row */
    u8 promptState; /* none, approach, or cannot afford */
} PaymentKioskState;

STATIC_ASSERT(offsetof(PaymentKioskPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(PaymentKioskPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(PaymentKioskPlacement, unknown19) == 0x19);
STATIC_ASSERT(offsetof(PaymentKioskPlacement, price) == 0x1A);
STATIC_ASSERT(offsetof(PaymentKioskPlacement, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(PaymentKioskPlacement, gameBit) == 0x1E);
STATIC_ASSERT(offsetof(PaymentKioskPlacement, unknown20) == 0x20);
STATIC_ASSERT(sizeof(PaymentKioskPlacement) == 0x24);

STATIC_ASSERT(offsetof(PaymentKioskState, payState) == 0x00);
STATIC_ASSERT(offsetof(PaymentKioskState, textVariant) == 0x01);
STATIC_ASSERT(offsetof(PaymentKioskState, promptState) == 0x02);
STATIC_ASSERT(sizeof(PaymentKioskState) == 0x03);

u32 PaymentKiosk_testEvent(GameObject* obj, int unused, int eventId);
int PaymentKiosk_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int PaymentKiosk_getExtraSize(void);
int PaymentKiosk_getObjectTypeId(void);
void PaymentKiosk_free(void);
void PaymentKiosk_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void PaymentKiosk_hitDetect(void);
void PaymentKiosk_update(GameObject* obj);
void PaymentKiosk_init(GameObject* obj, const PaymentKioskPlacement* placement);
void PaymentKiosk_release(void);
void PaymentKiosk_initialise(void);

extern ObjectDescriptor gPaymentKioskObjDescriptor;

#endif /* DLLS_OBJECTS_445_H_ */
