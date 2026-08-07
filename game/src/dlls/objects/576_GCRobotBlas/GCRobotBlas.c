/*
 * DLL 0x0240 - GC robot-blast object [0x801FF884-0x801FF9B0).
 *
 * A passive blast-effect object. Its sequence callback (GCRobotBlast_SeqFn)
 * latches the latest anim event id into the blast-fired flag (flags04 bit
 * 0x80); when that flag is set and mode is 0 or 1 it spawns a pair of
 * directional energy bursts each tick. init seeds mode from the placement
 * def byte (def+0x19), clears the fired flag and installs the sequence
 * callback. The remaining descriptor leaves (render/hitDetect/update/
 * free/release/initialise) are no-ops; getExtraSize reports
 * sizeof(GCRobotBlastState).
 */
#include "dlls/object_descriptor.h"
#include "main/dll/blastflags4_types.h"
#include "main/objfx.h"
#include "main/dll/dll_0240_gcrobotblast.h"
#include "main/objseq.h"

STATIC_ASSERT(sizeof(GCRobotBlastState) == 0x8);
STATIC_ASSERT(offsetof(GCRobotBlastState, mode) == 0x0);
STATIC_ASSERT(offsetof(GCRobotBlastState, flags04) == 0x4);
STATIC_ASSERT(offsetof(GCRobotBlastPlacement, mode) == 0x19);

int GCRobotBlast_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate)
{
    GCRobotBlastState* state = obj->extra;
    int i;

    for (i = 0; i < animUpdate->eventCount; i++)
    {
        state->flags04.b80 = animUpdate->eventIds[i];
    }
    if (state->flags04.b80 != 0)
    {
        switch (state->mode)
        {
        case 0:
        case 1:
            objfx_spawnDirectionalBurst(obj, 7, 1.0f, 5, 6, 0x64, 8.0f, NULL, 0x200000);
            objfx_spawnDirectionalBurst(obj, 6, 1.0f, 1, 6, 0x64, 8.0f, NULL, 0x200000);
            break;
        }
    }
    return 0;
}

int GCRobotBlast_getExtraSize(void)
{
    return sizeof(GCRobotBlastState);
}
int GCRobotBlast_getObjectTypeId(void)
{
    return 0x0;
}

void GCRobotBlast_free(void)
{
}

void GCRobotBlast_render(void)
{
}

void GCRobotBlast_hitDetect(void)
{
}

void GCRobotBlast_update(void)
{
}

void GCRobotBlast_init(GameObject* obj, GCRobotBlastPlacement* placement)
{
    GCRobotBlastState* state = obj->extra;
    state->mode = placement->mode;
    state->flags04.b80 = 0;
    obj->animEventCallback = GCRobotBlast_SeqFn;
}

void GCRobotBlast_release(void)
{
}

void GCRobotBlast_initialise(void)
{
}

ObjectDescriptor gGCRobotBlastObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_14_SLOTS,
    GCRobotBlast_initialise,
    GCRobotBlast_release,
    0,
    (ObjectDescriptorCallback)GCRobotBlast_init,
    GCRobotBlast_update,
    GCRobotBlast_hitDetect,
    GCRobotBlast_render,
    GCRobotBlast_free,
    (ObjectDescriptorCallback)GCRobotBlast_getObjectTypeId,
    GCRobotBlast_getExtraSize,
};
