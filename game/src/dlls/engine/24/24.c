#include "dlls/object_descriptor.h"
#include "main/dll/partfx_interface.h"
#include "main/shader_api.h"
#include "sys/objects.h"
#include "main/gamebits.h"
#include "main/mm.h"
#include "main/texture.h"
#include "main/model.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/frame_timing.h"
#include "main/lightmap_api.h"
#include "main/lightmap_text_color_api.h"
#include "main/dll/dll_0018_boneparticleeffect.h"
#include "main/vecmath.h"
#include "main/camera.h"
#include "dolphin/gx/GXCull.h"
#include "dolphin/mtx.h"
#include "main/rcp_dolphin_api.h"
#include "track/intersect_api.h"
#include "track/intersect_geom_api.h"
#include "track/intersect_render_setup_api.h"

s16 gBoneParticleEffectTimer;
s32 gBoneParticleScrollOffset;
s16 gBoneParticleStageIndex;
s32 gBoneParticleJointIndex;
f32 gBoneParticleDrift;
void* gBoneParticleTextureB;
void* gBoneParticleTextureA;
s32 gBoneParticleBufferFlip;

#define BONE_PARTICLE_EFFECT_PARTFX       0x28c
#define BONE_PARTICLE_EFFECT_BUFFER_COUNT 7
#define BONE_PARTICLE_EFFECT_BUFFER_BYTES 0x140
#define BONE_PARTICLE_EFFECT_SLOT_COUNT   20

void* gBoneParticleEffectBuffers[8];
f32 gBoneParticleDriftVelocity[2] = {10.0f, 0.0f};

/* the two bone-particle texture assets loaded at init (gBoneParticleTextureA/B) */
#define BONE_PARTICLE_TEXTURE_A_ID 0x16b
#define BONE_PARTICLE_TEXTURE_B_ID 0x201



static void boneParticleEffect_resetDrift(void) {
    gBoneParticleDrift = 0.0f;
}

void boneParticleEffect_func08_nop(void) {
}

f32 gBoneParticleConfigTable[108] = {
    -1500.0f, 0.0f,     -1500.0f, -1500.0f, 0.0f,     1500.0f, 1500.0f, 0.0f,    1500.0f, 1500.0f,  0.0f,    -1500.0f,
    -1500.0f, 0.0f,     -1500.0f, -1500.0f, 0.0f,     1500.0f, 1500.0f, 0.0f,    1500.0f, 1500.0f,  0.0f,    -1500.0f,
    -1500.0f, 0.0f,     -1500.0f, -1500.0f, 0.0f,     1500.0f, 1500.0f, 0.0f,    1500.0f, 1500.0f,  0.0f,    -1500.0f,
    0.0f,     -1500.0f, -1500.0f, 0.0f,     -1500.0f, 1500.0f, 0.0f,    1500.0f, 1500.0f, 0.0f,     1500.0f, -1500.0f,
    0.0f,     -1500.0f, -1500.0f, 0.0f,     -1500.0f, 1500.0f, 0.0f,    1500.0f, 1500.0f, 0.0f,     1500.0f, -1500.0f,
    0.0f,     -1500.0f, -1500.0f, 0.0f,     -1500.0f, 1500.0f, 0.0f,    1500.0f, 1500.0f, 0.0f,     1500.0f, -1500.0f,
    -1500.0f, -1500.0f, 0.0f,     1500.0f,  -1500.0f, 0.0f,    1500.0f, 1500.0f, 0.0f,    -1500.0f, 1500.0f, 0.0f,
    -1500.0f, -1500.0f, 0.0f,     1500.0f,  -1500.0f, 0.0f,    1500.0f, 1500.0f, 0.0f,    -1500.0f, 1500.0f, 0.0f,
    -1500.0f, -1500.0f, 0.0f,     1500.0f,  -1500.0f, 0.0f,    1500.0f, 1500.0f, 0.0f,    -1500.0f, 1500.0f, 0.0f,
};

/* Per-bone particle vertex update + draw. */
void boneParticleEffect_update(void* ctx, int renderParam, u8* obj) {
    MatrixTransform vtx;
    s16 j;
    s16 k;
    int row;
    ObjModel* model;
    u32 id;
    u32 cls;
    u8* mtx;
    GameObject* gobj = (GameObject*)obj;
    const u8* effectDataBytes;
    const f32* scaleA;
    const f32* scaleB;
    const f32* scaleC;
    const u8* jointIdCursor;
    void** updateBufferCursor;
    void** drawBufferCursor;
    int bufferIndex;
    f32 jointPositionScale;
    f32 one;
    f32 zero;
    f32 dx;
    f32 dy;
    f32 dz;

    effectDataBytes = (const u8*)gBoneParticleConfigTable;
    if (mainGetBit(GAMEBIT_TRICKYCURVE_PLAYER_HIT) != 0) {
        mainSetBits(GAMEBIT_TRICKYCURVE_PLAYER_HIT, 0);
        gBoneParticleEffectTimer = 0xf;
        Sfx_PlayFromObject(gobj, SFXTRIG_id_281);
    }
    model = (ObjModel*)gobj->anim.banks[gobj->anim.bankIndex];
    if (gBoneParticleStageIndex > 6) {
        gBoneParticleStageIndex = 0;
    }
    if (gBoneParticleJointIndex > model->file->jointCount - 1) {
        gBoneParticleJointIndex = 0;
    }
    gBoneParticleScrollOffset = gBoneParticleScrollOffset + framesThisStep;
    if (gBoneParticleScrollOffset > 0x1f) {
        gBoneParticleScrollOffset = gBoneParticleScrollOffset - 0x1f;
    }
    gBoneParticleDrift = gBoneParticleDriftVelocity[0] * timeDelta + gBoneParticleDrift;
    if (gBoneParticleDrift > 500.0f) {
        gBoneParticleDriftVelocity[0] *= -1.0f;
        gBoneParticleDrift = 500.0f;
        Sfx_PlayFromObject(gobj, SFXTRIG_id_282);
    } else if (gBoneParticleDrift < -500.0f) {
        gBoneParticleDriftVelocity[0] *= -1.0f;
        gBoneParticleDrift = -500.0f;
        Sfx_PlayFromObject(gobj, SFXTRIG_id_282);
    }
    bufferIndex = 0;
    drawBufferCursor = gBoneParticleEffectBuffers;
    updateBufferCursor = gBoneParticleEffectBuffers;
    for (; bufferIndex < BONE_PARTICLE_EFFECT_BUFFER_COUNT; bufferIndex++) {
        if (bufferIndex != 5) {
            gBoneParticleStageIndex = bufferIndex;
            row = 0;
            j = 0;
            jointIdCursor = effectDataBytes + 0x5b4;
            zero = (0.0f);
            one = (1.0f);
            jointPositionScale = 20.02f;
            while (j < 5) {
                vtx.x = zero;
                vtx.y = zero;
                vtx.z = zero;
                vtx.scale = one;
                vtx.rotZ = 0;
                vtx.rotY = 0;
                vtx.rotX = 0;
                mtx = model->jointMatrices[model->bufferFlags & 1];
                {
                    const u8* bp;
                    bp = effectDataBytes + gBoneParticleStageIndex * 5;
                    bp = bp + j;
                    id = bp[0x5b4];
                }
                mtx = (u8*)((BoneFxJRow*)mtx + (id << 4));
                dx = (*(Mtx44*)mtx)[3][0] + playerMapOffsetX;
                dy = (*(Mtx44*)mtx)[3][1];
                dz = (*(Mtx44*)mtx)[3][2] + playerMapOffsetZ;
                dx = dx - gobj->anim.localPosX;
                dy = dy - gobj->anim.localPosY;
                dz = dz - gobj->anim.localPosZ;
                dx = dx * jointPositionScale;
                if (id == 0x1d || id == 0x1d) {
                    dy = 20.02f * (8.0f + dy);
                } else {
                    dy = dy * jointPositionScale;
                }
                dz = dz * jointPositionScale;
                Matrix_TransformPoint((f32*)mtx, vtx.x, vtx.y, vtx.z, &vtx.x, &vtx.y, &vtx.z);
                k = 0;
                scaleA = (const f32*)(effectDataBytes + 0x90);
                scaleB = (const f32*)effectDataBytes;
                scaleC = (const f32*)(effectDataBytes + 0x120);
                while (k < 4) {
                    const u8* idr;
                    f32 sc;
                    id = *(const u8*)(jointIdCursor + gBoneParticleStageIndex * 5);
                    idr = effectDataBytes;
                    idr = idr + id;
                    cls = idr[0x590];
                    if (cls == 0) {
                        vtx.x = scaleA[0] * (sc = *(const f32*)(effectDataBytes + id * 4 + 0x5d8));
                        vtx.y = scaleA[1] * sc;
                        vtx.z = scaleA[2] * *(const f32*)(effectDataBytes + id * 4 + 0x664);
                    } else if (cls == 1) {
                        vtx.x = scaleB[0] * (sc = *(const f32*)(effectDataBytes + id * 4 + 0x5d8));
                        vtx.y = scaleB[1] * sc;
                        vtx.z = scaleB[2] * *(const f32*)(effectDataBytes + id * 4 + 0x664);
                    } else if (cls == 2) {
                        vtx.x = scaleC[0] * (sc = *(const f32*)(effectDataBytes + id * 4 + 0x5d8));
                        vtx.y = scaleC[1] * sc;
                        vtx.z = scaleC[2] * *(const f32*)(effectDataBytes + id * 4 + 0x664);
                    }
                    Matrix_TransformPoint((f32*)mtx, vtx.x, vtx.y, vtx.z, &vtx.x, &vtx.y, &vtx.z);
                    vtx.x = vtx.x + playerMapOffsetX;
                    vtx.z = vtx.z + playerMapOffsetZ;
                    ((ParticleSlot*)*updateBufferCursor)[k + row].posX = dx + (vtx.x - gobj->anim.localPosX);
                    ((ParticleSlot*)*updateBufferCursor)[k + row].posY = dy + (vtx.y - gobj->anim.localPosY);
                    ((ParticleSlot*)*updateBufferCursor)[k + row].posZ = dz + (vtx.z - gobj->anim.localPosZ);
                    ((ParticleSlot*)*updateBufferCursor)[k + row].alpha = 0x9b;
                    ((ParticleSlot*)*updateBufferCursor)[k + row].texV =
                        (s16)(((const ParticleSlot*)(effectDataBytes + 0x1b0))[k + row].texV -
                              (gBoneParticleScrollOffset << 2));
                    scaleA += 3;
                    scaleB += 3;
                    scaleC += 3;
                    k += 1;
                }
                row += 4;
                jointIdCursor += 1;
                j += 1;
            }
        }
        updateBufferCursor += 1;
    }
    vtx.x = gobj->anim.localPosX;
    vtx.y = gobj->anim.localPosY;
    vtx.z = gobj->anim.localPosZ;
    vtx.scale = 0.0495f;
    setTextColor(ctx, 0xff, 0xff, 0xff, 0xff);
    if (gBoneParticleEffectTimer != 0) {
        (*gPartfxInterface)->spawnObject((u8*)gobj, BONE_PARTICLE_EFFECT_PARTFX, NULL, 1, -1, NULL);
        (*gPartfxInterface)->spawnObject((u8*)gobj, BONE_PARTICLE_EFFECT_PARTFX, NULL, 1, -1, NULL);
        (*gPartfxInterface)->spawnObject((u8*)gobj, BONE_PARTICLE_EFFECT_PARTFX, NULL, 1, -1, NULL);
        if (randomGetRange(0, 1) != 0) {
            textureSelectAnimationFramePair(ctx, gBoneParticleTextureA, 0, 0, 0, 0, 0);
        } else {
            textureSelectAnimationFramePair(ctx, gBoneParticleTextureB, 0, 0, 0, 0, 0);
        }
        gBoneParticleEffectTimer -= framesThisStep;
        if (gBoneParticleEffectTimer < 0) {
            gBoneParticleEffectTimer = 0;
        }
    } else {
        textureSelectAnimationFramePair(ctx, gBoneParticleTextureA, 0, 0, 0, 0, 0);
    }
    Camera_LoadModelViewMatrix((int)ctx, renderParam, &vtx, 1.0f, 0.0f, NULL);
    GXSetCullMode(GX_CULL_NONE);
    _textSetColor(ctx, 0xff, 0xff, 0xff, 0xff);
    gxTevResetStages();
    gxTevTextureTimesRasStage();
    gxTevModulateColor1Stage();
    gxTevCommitStages();
    gxSetAlphaBlendZTest();
    {
        int i;
        i = 0;
        do {
            lightmapDrawTriangleList(*drawBufferCursor, (u8*)(effectDataBytes + 0x2f0), 0x20);
            drawBufferCursor += 1;
            i += 1;
        } while (i < BONE_PARTICLE_EFFECT_BUFFER_COUNT);
    }
    gBoneParticleBufferFlip = 1 - gBoneParticleBufferFlip;
}

void boneParticleEffect_func06_nop(void) {
}

ParticleSlot gBoneParticleInitData[20] = {
{-500, -900, -500, 0, 0, 0, 255, 255, 255, 255},
    {500, -900, -500, 0, 63, 0, 255, 255, 255, 255},
    {500, -900, 500, 0, 127, 0, 255, 255, 255, 255},
    {-500, -900, 500, 0, 191, 0, 255, 255, 255, 255},
    {-500, -900, -500, 0, 0, 127, 255, 255, 255, 255},
    {500, -900, -500, 0, 63, 127, 255, 255, 255, 255},
    {500, -900, 500, 0, 127, 127, 255, 255, 255, 255},
    {-500, -900, 500, 0, 191, 127, 255, 255, 255, 255},
    {-500, -900, -500, 0, 0, 255, 255, 255, 255, 255},
    {500, -900, -500, 0, 63, 255, 255, 255, 255, 255},
    {500, -900, 500, 0, 127, 255, 255, 255, 255, 255},
    {-500, -900, 500, 0, 191, 255, 255, 255, 255, 255},
    {-500, -900, -500, 0, 0, 383, 255, 255, 255, 255},
    {500, -900, -500, 0, 63, 383, 255, 255, 255, 255},
    {500, -900, 500, 0, 127, 383, 255, 255, 255, 255},
    {-500, -900, 500, 0, 191, 383, 255, 255, 255, 255},
    {-500, -900, -500, 0, 0, 511, 255, 255, 255, 255},
    {500, -900, -500, 0, 63, 511, 255, 255, 255, 255},
    {500, -900, 500, 0, 127, 511, 255, 255, 255, 255},
    {-500, -900, 500, 0, 191, 511, 255, 255, 255, 255},
};

u8 lbl_80310128[744] = {
    0, 0, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 7, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 7, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 9, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 5, 9, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 5, 10, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 6, 10, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 6, 11, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 7, 11, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 7, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 8, 12, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 8, 13, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 9, 13, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 9, 14, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 10, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 10, 15, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 11, 15, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 11, 12, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 12, 16, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 12, 17, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 13, 17, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 13, 18, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 14, 18, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 14, 19, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 15, 19, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 15, 16, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 6, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 8, 10, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 8, 10, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 12, 14, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 12, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 16, 18, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 16, 18, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 1, 2, 3, 4, 0, 5, 6, 7, 8, 9, 10,
    11, 13, 13, 16, 17, 18, 19, 20, 22, 23, 24, 25, 26, 15, 29, 32,
    15, 15, 28, 28, 33, 0, 0, 0,
};

f32 lbl_80310410[70] = {
    0.94f, 0.66f, 0.493f, 0.568f, 0.518f, 0.5f, 0.463f, 0.618f, 0.518f, 0.45f,
    0.638f, 0.842f, 0.6f, 0.413f, 0.9f, 0.862f, 0.518f, 0.585f, 0.285f, 0.465f,
    0.375f, 0.75f, 0.54f, 0.615f, 0.15f, 0.458f, 0.397f, 0.75f, 0.937f, 0.945f,
    0.75f, 0.75f, 0.862f, 0.947f, 0.75f, 0.75f, 1.0f, 0.772f, 0.967f, 0.967f,
    1.13f, 0.712f, 1.037f, 0.757f, 0.45f, 0.178f, 0.463f, 0.6f, 0.413f, 0.9f,
    0.862f, 0.518f, 0.585f, 0.555f, 0.465f, 0.375f, 0.75f, 0.54f, 0.615f, 0.51f,
    0.458f, 0.397f, 0.75f, 0.488f, 0.945f, 0.75f, 0.75f, 0.862f, 0.558f, 0.75f,
};

void boneParticleEffect_spawnAtBones(GameObject* obj, int effectId, void* extraArg, u8 prob, short* src) {
    ObjModel* model;
    int i;
    PartFxSpawnParams data;

    model = Obj_GetActiveModel(obj);
    for (i = 0; i < model->file->jointCount; i++) {
        if (randomGetRange(1, 0x64) <= prob) {
            MtxPtr mtx;
            data.posX = (0.0f);
            data.posY = (0.0f);
            data.posZ = (0.0f);
            data.scale = (1.0f);
            data.unk4 = 0;
            data.unk2 = 0;
            data.unk0 = 0;
            mtx = (MtxPtr)ObjModel_GetJointMatrix((u8*)model, i);
            PSMTXMultVec(mtx, &data.pos, &data.pos);
            data.posX = data.posX - (obj)->anim.worldPosX;
            data.posY = data.posY - (obj)->anim.worldPosY;
            data.posZ = data.posZ - (obj)->anim.worldPosZ;
            data.posX = data.posX + playerMapOffsetX;
            data.posZ = data.posZ + playerMapOffsetZ;
            if (src != NULL) {
                data.scale = *(f32*)((char*)src + 0x8);
                data.unk0 = src[0];
                data.unk4 = src[2];
                data.unk2 = src[1];
                data.effectParam = src[3];
            } else {
                data.scale = (1.0f);
                data.unk0 = 0;
                data.unk4 = 0;
                data.unk2 = 0;
                data.effectParam = 0;
            }
            (*gPartfxInterface)->spawnObject(obj, effectId, &data, 2, -1, extraArg);
        }
    }
}

void boneParticleEffect_func04_nop(void) {
}

void boneParticleEffect_func03_nop(void) {
}

void boneParticleEffect_release(void) {
    int i;
    for (i = 0; i < BONE_PARTICLE_EFFECT_BUFFER_COUNT; i++) {
        if (gBoneParticleEffectBuffers[i] != NULL) {
            mm_free(gBoneParticleEffectBuffers[i]);
        }
        gBoneParticleEffectBuffers[i] = NULL;
    }
    if (gBoneParticleTextureA != NULL) {
        textureFree((Texture*)(gBoneParticleTextureA));
    }
    if (gBoneParticleTextureB != NULL) {
        textureFree((Texture*)(gBoneParticleTextureB));
    }
}

void boneParticleEffect_initialise(void) {
    int i;
    int j;

    gBoneParticleTextureA = textureLoadAsset(BONE_PARTICLE_TEXTURE_A_ID);
    gBoneParticleTextureB = textureLoadAsset(BONE_PARTICLE_TEXTURE_B_ID);
    gBoneParticleEffectBuffers[0] = mmAlloc(BONE_PARTICLE_EFFECT_BUFFER_BYTES, 0x15, 0);
    gBoneParticleEffectBuffers[1] = mmAlloc(BONE_PARTICLE_EFFECT_BUFFER_BYTES, 0x15, 0);
    gBoneParticleEffectBuffers[2] = mmAlloc(BONE_PARTICLE_EFFECT_BUFFER_BYTES, 0x15, 0);
    gBoneParticleEffectBuffers[3] = mmAlloc(BONE_PARTICLE_EFFECT_BUFFER_BYTES, 0x15, 0);
    gBoneParticleEffectBuffers[4] = mmAlloc(BONE_PARTICLE_EFFECT_BUFFER_BYTES, 0x15, 0);
    gBoneParticleEffectBuffers[5] = mmAlloc(BONE_PARTICLE_EFFECT_BUFFER_BYTES, 0x15, 0);
    gBoneParticleEffectBuffers[6] = mmAlloc(BONE_PARTICLE_EFFECT_BUFFER_BYTES, 0x15, 0);
    for (i = 0; i < BONE_PARTICLE_EFFECT_BUFFER_COUNT; i++) {
        for (j = 0; j < BONE_PARTICLE_EFFECT_SLOT_COUNT; j++) {
            ((ParticleSlot*)gBoneParticleEffectBuffers[i])[j].posX = gBoneParticleInitData[j].posX;
            ((ParticleSlot*)gBoneParticleEffectBuffers[i])[j].posY = gBoneParticleInitData[j].posY;
            ((ParticleSlot*)gBoneParticleEffectBuffers[i])[j].posZ = gBoneParticleInitData[j].posZ;
            ((ParticleSlot*)gBoneParticleEffectBuffers[i])[j].texU = gBoneParticleInitData[j].texU;
            ((ParticleSlot*)gBoneParticleEffectBuffers[i])[j].texV = gBoneParticleInitData[j].texV;
            ((ParticleSlot*)gBoneParticleEffectBuffers[i])[j].red = gBoneParticleInitData[j].red;
            ((ParticleSlot*)gBoneParticleEffectBuffers[i])[j].green = gBoneParticleInitData[j].green;
            ((ParticleSlot*)gBoneParticleEffectBuffers[i])[j].blue = gBoneParticleInitData[j].blue;
            ((ParticleSlot*)gBoneParticleEffectBuffers[i])[j].alpha = 0xff;
        }
    }
}
typedef struct BoneParticleEffectDllInterface {
    u32 reserved0;
    u32 reserved1;
    u32 reserved2;
    u32 slotCountAndFlags;
    ObjectDescriptorCallback initialise;
    ObjectDescriptorCallback release;
    ObjectDescriptorCallback slot02;
    ObjectDescriptorCallback slot03;
    ObjectDescriptorCallback slot04;
    ObjectDescriptorCallback spawnAtBones;
    ObjectDescriptorCallback slot06;
    ObjectDescriptorCallback update;
    ObjectDescriptorCallback slot08;
    ObjectDescriptorCallback slot09;
} BoneParticleEffectDllInterface;

BoneParticleEffectDllInterface boneParticleEffect_funcs = {
    0,
    0,
    0,
    0x00080000,
    (ObjectDescriptorCallback)boneParticleEffect_initialise,
    (ObjectDescriptorCallback)boneParticleEffect_release,
    0,
    (ObjectDescriptorCallback)boneParticleEffect_func03_nop,
    (ObjectDescriptorCallback)boneParticleEffect_func04_nop,
    (ObjectDescriptorCallback)boneParticleEffect_spawnAtBones,
    (ObjectDescriptorCallback)boneParticleEffect_func06_nop,
    (ObjectDescriptorCallback)boneParticleEffect_update,
    (ObjectDescriptorCallback)boneParticleEffect_func08_nop,
    0,
};
