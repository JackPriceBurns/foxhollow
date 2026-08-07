#ifndef DLLS_OBJECTS_440_SC_TOTEMPOL_H_
#define DLLS_OBJECTS_440_SC_TOTEMPOL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* Exact anim.romDefNo value used by peer scans. */
#define SC_TOTEM_POLE_SEQUENCE_ID                0x282
#define SC_TOTEM_POLE_HANDLE_EVENT_VTABLE_OFFSET 0x20

#define SC_TOTEM_POLE_GAMEBIT_FRONT 0x81
#define SC_TOTEM_POLE_GAMEBIT_LEFT  0x82
#define SC_TOTEM_POLE_GAMEBIT_RIGHT 0x83
#define SC_TOTEM_POLE_GAMEBIT_REAR  0x84

typedef struct ScTotemPoleInterfaceVTable {
    void* unknown00[8];
    void (*handleEvent)(GameObject* totemPole, int eventId);
} ScTotemPoleInterfaceVTable;

typedef struct ScTotemPolePlacement {
    ObjPlacement base;
    u8 unknown18[2];
    u8 rotXByte;
    u8 unknown1B[5];
} ScTotemPolePlacement;

typedef struct ScTotemPoleState {
    u16 litGameBit;
    u8 lit;
    u8 wasLit;
    f32 animationSpeed;
} ScTotemPoleState;

STATIC_ASSERT(offsetof(ScTotemPoleInterfaceVTable, handleEvent) == SC_TOTEM_POLE_HANDLE_EVENT_VTABLE_OFFSET);

STATIC_ASSERT(offsetof(ScTotemPolePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(ScTotemPolePlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(ScTotemPolePlacement, rotXByte) == 0x1A);
STATIC_ASSERT(offsetof(ScTotemPolePlacement, unknown1B) == 0x1B);
STATIC_ASSERT(sizeof(ScTotemPolePlacement) == 0x20);

STATIC_ASSERT(offsetof(ScTotemPoleState, litGameBit) == 0x00);
STATIC_ASSERT(offsetof(ScTotemPoleState, lit) == 0x02);
STATIC_ASSERT(offsetof(ScTotemPoleState, wasLit) == 0x03);
STATIC_ASSERT(offsetof(ScTotemPoleState, animationSpeed) == 0x04);
STATIC_ASSERT(sizeof(ScTotemPoleState) == 0x08);

int sc_totempole_sortCompletionGameBits(const u16* recordGameBits, int completionTime);
int sc_totempole_getExtraSize(void);
int sc_totempole_getObjectTypeId(void);
void sc_totempole_free(void);
void sc_totempole_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void sc_totempole_hitDetect(void);
void sc_totempole_update(GameObject* obj);
void sc_totempole_init(GameObject* obj, const ScTotemPolePlacement* placement);
void sc_totempole_release(void);
void sc_totempole_initialise(void);

extern u16 gScTotemPoleRecordGameBits[4];
extern f32 gScTotemPoleHitEffectCooldown;
extern ObjectDescriptor gSC_totempoleObjDescriptor;

#endif /* DLLS_OBJECTS_440_SC_TOTEMPOL_H_ */
