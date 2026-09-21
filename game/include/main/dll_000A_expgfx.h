#ifndef MAIN_EXPGFX_H_
#define MAIN_EXPGFX_H_

#include "types.h"
#include "main/dll/expgfx_interface.h"
#include "dlls/object_descriptor.h"
#include "main/dll/expgfx_resource.h"

RESOURCE_DESCRIPTOR_TYPE(ExpgfxDllInterface, ExpgfxInterface);

extern ExpgfxDllInterface expgfx_funcs;

void expgfxRemove(uintptr_t slotPoolBase, int poolIndex, int slotIndex, int skipTextureFree, int flushSlot);
void expgfxRemoveAll(void);
int expgfxGetSlot(short* poolIndexOut, short* slotIndexOut, short slotType, int preferredPoolIndex, uintptr_t sourceId);
void expgfx_initSlotQuad(void* slot);
void expgfx_updateActivePools(u8 sourceMode, uintptr_t sourceId, int resetSourceFrameState);
int expgfx_addToTable(void* resourceHandle, uintptr_t sourceId, uintptr_t attachedTableKey, s16 resourceId);
int expgfx_updateSourceFrameFlags(void* sourceObject);
void expgfx_ownerFree3(uintptr_t sourceId);
void expgfx_func0B_nop(void);
void expgfx_func0A_nop(void);
int expgfx_func09(void);
void expgfx_renderSourcePools(uintptr_t sourceId, int sourceMode);
void drawGlow(uintptr_t slotPoolBase, int poolIndex);
void renderParticles(void);
void expgfx_free2(uintptr_t sourceId);
void expgfx_free(uintptr_t sourceId);
void expgfx_resetAllPools(void);
void expgfx_updateFrameState(int sourceMode, uintptr_t sourceId);
struct ExpgfxSpawnConfig;
int expgfx_addremove(struct ExpgfxSpawnConfig* config, int preferredPoolIndex, int slotType, int planeOffsetSetId);
void expgfx_onMapSetup(void);
void expgfx_release(void);
void expgfx_initialise(void);

#endif /* MAIN_EXPGFX_H_ */
