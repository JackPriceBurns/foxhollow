#ifndef DLLS_OBJECTS_300_TRANSPORTER_H_
#define DLLS_OBJECTS_300_TRANSPORTER_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef enum TransporterFlag {
    TRANSPORTER_FLAG_INTERACTIVE = 0x01,
    TRANSPORTER_FLAG_PULSE_LATCH = 0x02,
    TRANSPORTER_FLAG_PULSE_FX = 0x04,
    TRANSPORTER_FLAG_WARP_B = 0x08,
    TRANSPORTER_FLAG_WARP_C = 0x10,
    TRANSPORTER_FLAG_DISABLED = 0x20,
    TRANSPORTER_FLAG_WARP_A = 0x40,
    TRANSPORTER_FLAG_ENABLE_GAMEBIT_OFF = 0x80,
} TransporterFlag;

#define TRANSPORTER_GAME_BIT_NONE -1
#define TRANSPORTER_WARP_ID_NONE  -1

typedef enum TransporterTriggerMode {
    TRANSPORTER_TRIGGER_PROXIMITY = 0,
    TRANSPORTER_TRIGGER_INTERACTION = 1,
} TransporterTriggerMode;

/*
 * Retail EN romlists prove that all 35 Transporter placements use this
 * complete 0x24-byte layout. The object's identity is stored in base.ident.
 */
typedef struct TransporterPlacement {
    ObjPlacement base; /* 0x00 */
    u8 rotXHigh;       /* 0x18: shifted left by eight */
    u8 pad19;          /* 0x19 */
    s8 warpId;         /* 0x1A: TRANSPORTER_WARP_ID_NONE disables warping */
    u8 pad1B[5];       /* 0x1B */
    s16 enableGameBit; /* 0x20: TRANSPORTER_GAME_BIT_NONE means no gate */
    u8 pad22[2];       /* 0x22 */
} TransporterPlacement;

/* Transporter_getExtraSize proves the complete 0x10-byte runtime allocation. */
typedef struct TransporterState {
    f32 pulseTimer;     /* 0x00 */
    f32 cooldownTimer;  /* 0x04 */
    s16 activateDelay;  /* 0x08 */
    s16 unk0A;          /* 0x0A */
    u8 countdownActive; /* 0x0C */
    u8 triggerMode;     /* 0x0D: TransporterTriggerMode */
    u8 flags;           /* 0x0E: TransporterFlag */
    u8 pad0F;           /* 0x0F */
} TransporterState;

STATIC_ASSERT(offsetof(TransporterPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(TransporterPlacement, rotXHigh) == 0x18);
STATIC_ASSERT(offsetof(TransporterPlacement, pad19) == 0x19);
STATIC_ASSERT(offsetof(TransporterPlacement, warpId) == 0x1A);
STATIC_ASSERT(offsetof(TransporterPlacement, pad1B) == 0x1B);
STATIC_ASSERT(offsetof(TransporterPlacement, enableGameBit) == 0x20);
STATIC_ASSERT(offsetof(TransporterPlacement, pad22) == 0x22);
STATIC_ASSERT(sizeof(TransporterPlacement) == 0x24);

STATIC_ASSERT(offsetof(TransporterState, pulseTimer) == 0x00);
STATIC_ASSERT(offsetof(TransporterState, cooldownTimer) == 0x04);
STATIC_ASSERT(offsetof(TransporterState, activateDelay) == 0x08);
STATIC_ASSERT(offsetof(TransporterState, unk0A) == 0x0A);
STATIC_ASSERT(offsetof(TransporterState, countdownActive) == 0x0C);
STATIC_ASSERT(offsetof(TransporterState, triggerMode) == 0x0D);
STATIC_ASSERT(offsetof(TransporterState, flags) == 0x0E);
STATIC_ASSERT(offsetof(TransporterState, pad0F) == 0x0F);
STATIC_ASSERT(sizeof(TransporterState) == 0x10);

void Transporter_updateEffects(GameObject* obj);
void Transporter_updateInteraction(GameObject* obj);

int Transporter_sequenceCallback(GameObject* obj, int unused, ObjSeqState* animUpdate);
int Transporter_getExtraSize(void);
void Transporter_render(void);
void Transporter_hitDetect(int obj);
void Transporter_update(GameObject* obj);
void Transporter_init(GameObject* obj, TransporterPlacement* placement);

extern ObjectDescriptor gTransporterObjDescriptor;

#endif /* DLLS_OBJECTS_300_TRANSPORTER_H_ */
