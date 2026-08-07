/* DLL 0x0228 */
#include "main/dll/CF/laser.h"
#include "main/game_ui_interface.h"
#include "main/gamebits.h"
#include "main/mapEventTypes.h"
#include "main/objprint_render_api.h"

int VFP_SpellPlace_getExtraSize(void);
int VFP_SpellPlace_getObjectTypeId(void);
void VFP_SpellPlace_free(void);
void VFP_SpellPlace_render(void);
void VFP_SpellPlace_hitDetect(void);
void VFP_SpellPlace_update(GameObject* spellPlace);
void VFP_SpellPlace_init(GameObject* spellPlace, s8* def);
void VFP_SpellPlace_release(void);
void VFP_SpellPlace_initialise(void);

int VFP_SpellPlace_getExtraSize(void)
{
    return sizeof(LaserState);
}

int VFP_SpellPlace_getObjectTypeId(void)
{
    return 0x0;
}

void VFP_SpellPlace_free(void)
{
}

void VFP_SpellPlace_render(void)
{
}

void VFP_SpellPlace_hitDetect(void)
{
}

void VFP_SpellPlace_update(GameObject* spellPlace)
{
    LaserState* state;
    u8 mode;

    if (((LaserState*)spellPlace->extra)->completionLatched == 0 &&
        mainGetBit((int)((LaserState*)spellPlace->extra)->activationGameBit) != 0)
    {
        spellPlace->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
    }
    else
    {
        spellPlace->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    }
    objUpdateHitVolumeTransforms(spellPlace);
    if (spellPlace->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED)
    {
        mode = (*gMapEventInterface)->getMapAct((int)spellPlace->anim.mapEventSlot);
        switch (mode)
        {
        case LASEROBJ_MODE_SEQUENCE_A:
            state = spellPlace->extra;
            if ((*gGameUIInterface)->isItemBeingUsed(LASEROBJ_MAIN_SEQUENCE_A_EVENT) != 0)
            {
                mainSetBits(state->completionGameBit, 1);
                mainSetBits(state->activationGameBit, 0);
                state->completionLatched = 1;
                spellPlace->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
            }
            break;
        case LASEROBJ_MODE_SEQUENCE_B:
            state = spellPlace->extra;
            if ((*gGameUIInterface)->isItemBeingUsed(LASEROBJ_MAIN_SEQUENCE_B_EVENT) != 0)
            {
                mainSetBits(state->completionGameBit, 1);
                mainSetBits(state->activationGameBit, 0);
                state->completionLatched = 1;
                spellPlace->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
            }
            break;
        }
    }
}

void VFP_SpellPlace_init(GameObject* spellPlace, s8* def)
{
    LaserObjectMapData* mapData;
    LaserState* state;

    mapData = (LaserObjectMapData*)def;
    state = spellPlace->extra;
    state->completionGameBit = mapData->completionGameBit;
    state->activationGameBit = mapData->activationGameBit;
    state->completionLatched = 0;
    spellPlace->anim.rotX = (s16)(mapData->yawByte << LASEROBJ_YAW_BYTE_SHIFT);
    if (mainGetBit(state->completionGameBit) != 0)
    {
        state->completionLatched = 1;
        spellPlace->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    }
    spellPlace->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED | OBJECT_OBJFLAG_HIDDEN;
}

void VFP_SpellPlace_release(void)
{
}

void VFP_SpellPlace_initialise(void)
{
}

ObjectDescriptor gVFP_SpellPlaceObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)VFP_SpellPlace_initialise,
    (ObjectDescriptorCallback)VFP_SpellPlace_release,
    0,
    (ObjectDescriptorCallback)VFP_SpellPlace_init,
    (ObjectDescriptorCallback)VFP_SpellPlace_update,
    (ObjectDescriptorCallback)VFP_SpellPlace_hitDetect,
    (ObjectDescriptorCallback)VFP_SpellPlace_render,
    (ObjectDescriptorCallback)VFP_SpellPlace_free,
    (ObjectDescriptorCallback)VFP_SpellPlace_getObjectTypeId,
    (ObjectDescriptorExtraSizeCallback)VFP_SpellPlace_getExtraSize,
};
