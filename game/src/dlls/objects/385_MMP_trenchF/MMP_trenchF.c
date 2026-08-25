#include "dlls/objects/385_MMP_trenchF.h"

#include "game/objects/object.h"
#include "main/dll/expgfx_interface.h"
#include "main/dll/partfx_interface.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/vecmath.h"

typedef enum MMPTrenchFxParticleId {
    MMP_TRENCH_FX_PARTICLE_BURST = 0x71F,
    MMP_TRENCH_FX_PARTICLE_AMBIENT
} MMPTrenchFxParticleId;

typedef struct MMPTrenchFxState {
    s16 enableGameBit;
    u16 extentX;
    u16 extentZ;
    u16 extentY;
    Vec3s emitAngles;
    u8 unknown0E[2];
    PartFxSpawnParams burstSpawnParams;
    f32 burstCooldown;
    f32 burstTimer;
} MMPTrenchFxState;

static PartFxSpawnParams gMMPTrenchFxAmbientSpawnParams;

int mmpTrenchFx_getExtraSize(void) {
    return sizeof(MMPTrenchFxState);
}

int mmpTrenchFx_getObjectTypeId(void) {
    return 0;
}

void mmpTrenchFx_free(GameObject* obj) {
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

void mmpTrenchFx_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }
}

void mmpTrenchFx_hitDetect(void) {
}

void mmpTrenchFx_update(GameObject* obj) {
    MMPTrenchFxState* state = obj->extra;

    if (state->enableGameBit == -1 || mainGetBit(state->enableGameBit) != 0) {
        state->burstCooldown -= timeDelta;
        if (state->burstCooldown < 0.0f) {
            state->burstSpawnParams.scale = 1.0f;
            state->burstSpawnParams.posX = (f32)randomGetRange(-state->extentX, state->extentX);
            state->burstSpawnParams.posY = (f32)randomGetRange(-state->extentY, state->extentY);
            state->burstSpawnParams.posZ = (f32)randomGetRange(-state->extentZ, state->extentZ);
            vecRotateZXY(&state->emitAngles.x, &state->burstSpawnParams.posX);
            state->burstSpawnParams.posX += obj->anim.localPosX;
            state->burstSpawnParams.posY += obj->anim.localPosY;
            state->burstSpawnParams.posZ += obj->anim.localPosZ;
            state->burstCooldown = (f32)randomGetRange(0x64, 0xC8);
            state->burstTimer = (f32)randomGetRange(0x32, 0x64);
        }
        state->burstTimer -= timeDelta;
        if (state->burstTimer > 0.0f) {
            (*gPartfxInterface)
                ->spawnObject(obj, MMP_TRENCH_FX_PARTICLE_BURST, &state->burstSpawnParams, 0x200001, -1, NULL);
        }
        gMMPTrenchFxAmbientSpawnParams.scale = 1.0f;
        gMMPTrenchFxAmbientSpawnParams.posX = (f32)randomGetRange(-state->extentX, state->extentX);
        gMMPTrenchFxAmbientSpawnParams.posY = (f32)randomGetRange(-state->extentY, state->extentY);
        gMMPTrenchFxAmbientSpawnParams.posZ = (f32)randomGetRange(-state->extentZ, state->extentZ);
        vecRotateZXY(&state->emitAngles.x, &gMMPTrenchFxAmbientSpawnParams.posX);
        gMMPTrenchFxAmbientSpawnParams.posX += obj->anim.localPosX;
        gMMPTrenchFxAmbientSpawnParams.posY += obj->anim.localPosY;
        gMMPTrenchFxAmbientSpawnParams.posZ += obj->anim.localPosZ;
        (*gPartfxInterface)
            ->spawnObject(obj, MMP_TRENCH_FX_PARTICLE_AMBIENT, &gMMPTrenchFxAmbientSpawnParams, 0x200001, -1, NULL);
    }
}

void mmpTrenchFx_init(GameObject* obj, const MMPTrenchFxPlacement* placement) {
    MMPTrenchFxState* state = obj->extra;
    s16 angle;

    state->enableGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->enableGameBit);
    state->extentX = (u16)(placement->extentX << 2);
    state->extentZ = (u16)(placement->extentZ << 2);
    state->extentY = (u16)(placement->extentY << 2);
    angle = (s16)((s32)placement->emitAngleZ * 256);
    state->emitAngles.z = angle;
    obj->anim.rotZ = angle;
    angle = (s16)((s32)placement->emitAngleY * 256);
    state->emitAngles.y = angle;
    obj->anim.rotY = angle;
    angle = (s16)((s32)placement->emitAngleX * 256);
    state->emitAngles.x = angle;
    obj->anim.rotX = angle;
    obj->anim.rootMotionScale = 0.1f;
}

void mmpTrenchFx_release(void) {
}

void mmpTrenchFx_initialise(void) {
}

OBJECT_INIT_ADAPTER(gMMPTrenchFxObjDescriptorInitAdapter, mmpTrenchFx_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gMMPTrenchFxObjDescriptorHitDetectAdapter, mmpTrenchFx_hitDetect)
OBJECT_FREE_ADAPTER(gMMPTrenchFxObjDescriptorFreeAdapter, mmpTrenchFx_free, obj)
OBJECT_TYPE_ID_ADAPTER(gMMPTrenchFxObjDescriptorTypeIdAdapter, mmpTrenchFx_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gMMPTrenchFxObjDescriptorExtraSizeAdapter, mmpTrenchFx_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gMMPTrenchFxObjDescriptorAcquire, mmpTrenchFx_initialise)

ObjectDescriptor gMMPTrenchFxObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gMMPTrenchFxObjDescriptorAcquire,
        mmpTrenchFx_release,
    },
    0,
    gMMPTrenchFxObjDescriptorInitAdapter,
    mmpTrenchFx_update,
    gMMPTrenchFxObjDescriptorHitDetectAdapter,
    mmpTrenchFx_render,
    gMMPTrenchFxObjDescriptorFreeAdapter,
    gMMPTrenchFxObjDescriptorTypeIdAdapter,
    gMMPTrenchFxObjDescriptorExtraSizeAdapter,
};
