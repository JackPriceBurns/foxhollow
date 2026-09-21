#ifndef DLLS_OBJECTS_291_FUELCELL_H_
#define DLLS_OBJECTS_291_FUELCELL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/lightningeffect.h"

#define FUEL_CELL_STATE_SIZE             0x60
#define FUEL_CELL_LIGHTNING_EFFECT_COUNT 10

/* Shared by world fuel cells, the shop's lightning display, and the minimap radar. */
#define FUEL_CELL_OBJECT_GROUP 0x4F

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct FuelCellPlacement {
    ObjPlacement base; /* 0x00 */
    u8 pad18[6];       /* 0x18 */
    s16 offBit;        /* 0x1E: set after collection; -1 disables the object */
    s16 onBit;         /* 0x20: must be set before the object appears; -1 = no gate */
} FuelCellPlacement;

typedef struct FuelCellStateFlags {
    u8 active : 1;           /* Sound, rendering, and object-group membership are active. */
    u8 pickupPending : 1;    /* The player has been offered this cell; awaits completion. */
    u8 alternateEffects : 1; /* Uses isolated lightning and the alternate burst scale. */
    u8 resetPosition : 1;    /* Restores the placement position on the next active update. */
    u8 : 4;
} FuelCellStateFlags;

typedef struct FuelCellState {
    s16 triggerGameBit;     /* 0x00: head of the player pickup-message payload */
    s16 pickupMessageValue; /* 0x02 */
    f32 pickupMessageDelay; /* 0x04 */
    LightningEffect* lightningEffects[FUEL_CELL_LIGHTNING_EFFECT_COUNT]; /* 0x08 */
    u8 pad30[4];                                                         /* 0x30 */
    f32 lightningAges[FUEL_CELL_LIGHTNING_EFFECT_COUNT];                 /* 0x34 */
    FuelCellStateFlags flags;                                            /* 0x5C */
    u8 pad5D[3];                                                         /* 0x5D */
} FuelCellState;

STATIC_ASSERT(offsetof(FuelCellPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(FuelCellPlacement, pad18) == 0x18);
STATIC_ASSERT(offsetof(FuelCellPlacement, offBit) == 0x1E);
STATIC_ASSERT(offsetof(FuelCellPlacement, onBit) == 0x20);

STATIC_ASSERT(sizeof(FuelCellStateFlags) == 0x1);
STATIC_ASSERT(offsetof(FuelCellState, triggerGameBit) == 0x0);
STATIC_ASSERT(offsetof(FuelCellState, pickupMessageValue) == 0x2);
STATIC_ASSERT(offsetof(FuelCellState, pickupMessageDelay) == 0x4);
STATIC_ASSERT(offsetof(FuelCellState, lightningEffects) == 0x8);
STATIC_ASSERT(offsetof(FuelCellState, pad30) == 0x30);
STATIC_ASSERT(offsetof(FuelCellState, lightningAges) == 0x34);
STATIC_ASSERT(offsetof(FuelCellState, flags) == 0x5C);
STATIC_ASSERT(offsetof(FuelCellState, pad5D) == 0x5D);
STATIC_ASSERT(sizeof(FuelCellState) == FUEL_CELL_STATE_SIZE);

int FuelCell_SeqFn(GameObject* obj);
void FuelCell_setupModelRenderState(GameObject* obj);
int FuelCell_getExtraSize(void);
void FuelCell_free(GameObject* obj);
void FuelCell_render(GameObject* obj, int p2, int p3, int p4, int p5);
void FuelCell_update(GameObject* obj);
void FuelCell_init(GameObject* obj);

extern ObjectDescriptor gFuelCellObjDescriptor;

#endif /* DLLS_OBJECTS_291_FUELCELL_H_ */
