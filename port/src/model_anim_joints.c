#include "foxhollow_compat.h"

#include <math.h>
#include <string.h>

#include <dolphin/types.h>
#include <dolphin/mtx.h>

#include "main/model.h"
#include "main/objanim_internal.h"

#define FH_ANIM_MAX_JOINT_RECORDS 256
#define FH_ANIM_SUBFRAME_SCALE 16384
#define FH_ANIM_SCALE_ONE 0x400
#define FH_ANIM_TRANS_SCALE (1.0f / 512.0f)
#define FH_ANIM_SCALE_RECIP (1.0f / 1024.0f)
#define FH_ANIM_ANGLE_TO_HALF_RAD (3.14159265358979323846f / 65536.0f)

#define FH_ANIM_MODE_QUAT_SRC_A 0x01
#define FH_ANIM_MODE_QUAT_SRC_B 0x02
#define FH_ANIM_MODE_QUAT_DST_A 0x04
#define FH_ANIM_MODE_QUAT_DST_B 0x08
#define FH_ANIM_MODE_SCRATCH (FH_ANIM_MODE_QUAT_SRC_A | FH_ANIM_MODE_QUAT_SRC_B | \
                              FH_ANIM_MODE_QUAT_DST_A | FH_ANIM_MODE_QUAT_DST_B)
#define FH_ANIM_MODE_NO_TRANS_LERP 0x10
#define FH_ANIM_MODE_ROOT_ROT_OVERRIDE 0x20
#define FH_ANIM_MODE_SINGLE_PASS 0x40

#define FH_SCRATCH_QUAT_A 0x00
#define FH_SCRATCH_QUAT_B 0x10
#define FH_SCRATCH_SCALE_A 0x28
#define FH_SCRATCH_SCALE_B 0x2e
#define FH_SCRATCH_TRANS_A 0x34
#define FH_SCRATCH_TRANS_B 0x3a

#define FH_JOINT_MTX_STRIDE 0x40

extern s16 gModelRootRotX;
extern s16 gModelRootRotY;
extern s16 gModelRootRotZ;

typedef struct FhAnimJointRecord {
    s16 rot[3];
    s16 scale[3];
    s16 trans[3];
} FhAnimJointRecord;

typedef struct FhAnimBitReader {
    const u8* base;
    u32 bit;
} FhAnimBitReader;

static FhAnimJointRecord sFhAnimRecords[2][FH_ANIM_MAX_JOINT_RECORDS];
static int sFhAnimRecordCount[2];

static u32 fhAnimReadBits(FhAnimBitReader* r, int count)
{
    u32 value = 0;
    int i;

    for (i = 0; i < count; i++)
    {
        u32 bit = (r->base[r->bit >> 3] >> (7 - (r->bit & 7))) & 1;
        value = (value << 1) | bit;
        r->bit++;
    }
    return value;
}

static s32 fhAnimSample(FhAnimBitReader* a, FhAnimBitReader* b, int width, s32 frac, int wrapBits)
{
    s32 va;
    s32 vb;
    s32 delta;

    if (width == 0)
    {
        return 0;
    }
    va = (s32)fhAnimReadBits(a, width);
    vb = (s32)fhAnimReadBits(b, width);
    delta = vb - va;
    if (wrapBits != 0)
    {
        int shift = 32 - wrapBits;

        delta = (s32)((u32)delta << shift) >> shift;
    }
    return va + (s32)(((s64)delta * frac) >> 14);
}

static int fhAnimDecodeChannel(int slot, const u8* frameData, const u8* cursor, int stride, f32 framePhase)
{
    const u8* record;
    const u8* descriptors;
    const u8* descriptorEnd;
    FhAnimBitReader a;
    FhAnimBitReader b;
    FhAnimJointRecord* out;
    int recordCount;
    int i;
    int axis;
    s32 frac;
    f32 phase;

    if (frameData == NULL || cursor == NULL)
    {
        sFhAnimRecordCount[slot] = 0;
        return 0;
    }
    record = frameData - 6;
    recordCount = record[6];
    if (recordCount > FH_ANIM_MAX_JOINT_RECORDS)
    {
        recordCount = FH_ANIM_MAX_JOINT_RECORDS;
    }
    descriptors = frameData + 4;
    descriptorEnd = record + (s16)fhSwap16(*(const u16*)(record + 2));
    if (descriptorEnd < descriptors)
    {
        sFhAnimRecordCount[slot] = 0;
        return 0;
    }

    phase = framePhase - floorf(framePhase);
    frac = (s32)(phase * (f32)FH_ANIM_SUBFRAME_SCALE);
    if (frac < 0)
    {
        frac = 0;
    }

    a.base = cursor;
    a.bit = 0;
    b.base = cursor + stride;
    b.bit = 0;

    out = sFhAnimRecords[slot];
    memset(out, 0, (size_t)recordCount * sizeof(FhAnimJointRecord));

    for (i = 0; i < recordCount; i++)
    {
        for (axis = 0; axis < 3; axis++)
        {
            u16 d;
            s32 sample;

            if (descriptors + 2 > descriptorEnd)
            {
                recordCount = i;
                break;
            }
            d = fhSwap16(*(const u16*)descriptors);
            descriptors += 2;
            sample = fhAnimSample(&a, &b, d & 0xf, frac, 14);
            sample = (s32)(d & 0xfff0) + (sample << 2);
            out[i].rot[axis] = (s16)sample;

            if ((d & 0x10) == 0 || descriptors + 2 > descriptorEnd)
            {
                continue;
            }
            d = fhSwap16(*(const u16*)descriptors);
            if ((d & 0x10) != 0)
            {
                sample = fhAnimSample(&a, &b, d & 0xf, frac, 0);
                if ((d & 0xf) != 0)
                {
                    sample = (s32)(d & 0xffc0) + (sample << 1);
                }
                else
                {
                    sample = (s32)(d & 0xffc0);
                }
                out[i].scale[axis] = (s16)sample;
                descriptors += 2;
                if ((d & 0x20) == 0 || descriptors + 2 > descriptorEnd)
                {
                    continue;
                }
                d = fhSwap16(*(const u16*)descriptors);
            }
            sample = fhAnimSample(&a, &b, d & 0xf, frac, 16);
            sample = (s32)(d & 0xfff0) + sample;
            out[i].trans[axis] = (s16)sample;
            descriptors += 2;
        }
    }
    sFhAnimRecordCount[slot] = recordCount;
    return recordCount;
}

static void fhAnimApplyJointOverrides(int slot, const u8* scratch)
{
    const s16* entry;

    if (scratch == NULL)
    {
        return;
    }
    entry = (const s16*)scratch + slot;
    while ((u16)entry[0] != 0x1000)
    {
        u16 offset = (u16)entry[0];
        int recordIndex = offset >> 6;
        int componentOffset = offset & 0x3f;
        s16 value = entry[2];

        if (recordIndex >= 0 && recordIndex < sFhAnimRecordCount[slot])
        {
            FhAnimJointRecord* record = &sFhAnimRecords[slot][recordIndex];

            switch (componentOffset)
            {
                case 0x00:
                case 0x02:
                case 0x04:
                    record->rot[componentOffset >> 1] =
                        (s16)(record->rot[componentOffset >> 1] + value);
                    break;
                case 0x0c:
                case 0x0e:
                case 0x10:
                    record->scale[(componentOffset - 0x0c) >> 1] =
                        (s16)(record->scale[(componentOffset - 0x0c) >> 1] + value);
                    break;
                case 0x18:
                case 0x1a:
                case 0x1c:
                    record->trans[(componentOffset - 0x18) >> 1] =
                        (s16)(record->trans[(componentOffset - 0x18) >> 1] + value);
                    break;
            }
        }
        entry += 4;
    }
}

static void fhAnimEulerToQuat(const s16* rot, f32* q)
{
    f32 hx = (f32)rot[0] * FH_ANIM_ANGLE_TO_HALF_RAD;
    f32 hy = (f32)rot[1] * FH_ANIM_ANGLE_TO_HALF_RAD;
    f32 hz = (f32)rot[2] * FH_ANIM_ANGLE_TO_HALF_RAD;
    f32 cx = cosf(hx);
    f32 sx = sinf(hx);
    f32 cy = cosf(hy);
    f32 sy = sinf(hy);
    f32 cz = cosf(hz);
    f32 sz = sinf(hz);

    q[0] = cx * cy * cz + sx * sy * sz;
    q[1] = sx * cy * cz - cx * sy * sz;
    q[2] = cx * sy * cz + sx * cy * sz;
    q[3] = cx * cy * sz - sx * sy * cz;
}

static const FhAnimJointRecord* fhAnimRecordAt(int slot, int index)
{
    static const FhAnimJointRecord kIdentity = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    if (index < 0 || index >= sFhAnimRecordCount[slot])
    {
        return &kIdentity;
    }
    return &sFhAnimRecords[slot][index];
}

static s32 fhAnimLerpS32(s32 from, s32 to, s32 frac)
{
    return from + (((to - from) * frac) >> 14);
}

void modelAnimBuildJointMatrices(int* out, u8* dst, void* animState, u8* jointData, int jointCount,
                                 u8* scratch, int flags, int mode)
{
    ObjAnimState* work = (ObjAnimState*)animState;
    u8* mtxBase;
    int quatSrcA = (mode & FH_ANIM_MODE_QUAT_SRC_A) != 0;
    int quatSrcB = (mode & FH_ANIM_MODE_QUAT_SRC_B) != 0;
    int useScratchRecords = (mode & FH_ANIM_MODE_SCRATCH) != 0;
    s32 blendFrac;
    int j;

    if (out == NULL || animState == NULL || jointData == NULL || jointCount <= 0)
    {
        return;
    }
    mtxBase = *(u8**)out;
    if (mtxBase == NULL)
    {
        return;
    }

    blendFrac = (s32)work->eventCountdown;
    if (blendFrac < 0)
    {
        blendFrac = 0;
    }
    if (blendFrac > FH_ANIM_SUBFRAME_SCALE)
    {
        blendFrac = FH_ANIM_SUBFRAME_SCALE;
    }

    if (!quatSrcA)
    {
        fhAnimDecodeChannel(0, (const u8*)work->frameData[0], work->frameStreamCursors[0],
                            work->frameStreamStrides[0], work->framePhases[0]);
        fhAnimApplyJointOverrides(0, scratch);
    }
    else
    {
        sFhAnimRecordCount[0] = 0;
    }
    if (!quatSrcB)
    {
        fhAnimDecodeChannel(1, (const u8*)work->frameData[1], work->frameStreamCursors[1],
                            work->frameStreamStrides[1], work->framePhases[1]);
        fhAnimApplyJointOverrides(1, scratch);
        if ((mode & FH_ANIM_MODE_ROOT_ROT_OVERRIDE) && sFhAnimRecordCount[1] > 0)
        {
            sFhAnimRecords[1][0].rot[0] = gModelRootRotX;
            sFhAnimRecords[1][0].rot[1] = gModelRootRotY;
            sFhAnimRecords[1][0].rot[2] = gModelRootRotZ;
        }
    }
    else
    {
        sFhAnimRecordCount[1] = 0;
    }

    for (j = 0; j < jointCount; j++)
    {
        const ModelBone* bone = (const ModelBone*)(jointData + (size_t)j * sizeof(ModelBone));
        s32 tagged = (s32)(s8)bone->idx[0];
        int dstIndex;
        int indexA;
        int indexB;
        const FhAnimJointRecord* recA;
        const FhAnimJointRecord* recB;
        u8* mtxSlot;
        u8* scratchSlot;
        f32 qa[4];
        f32 qb[4];
        f32 q[4];
        f32 dot;
        f32 len;
        s16 scaleOut[3];
        s16 transOut[3];
        MtxPtr m;
        int axis;

        if ((tagged & flags) < 0)
        {
            continue;
        }
        dstIndex = (int)(tagged & 0x7f);
        indexA = bone->idx[1];
        indexB = bone->idx[2];
        recA = fhAnimRecordAt(0, indexA);
        recB = fhAnimRecordAt(1, indexB);
        scratchSlot = mtxBase + (size_t)dstIndex * FH_JOINT_MTX_STRIDE;

        if (quatSrcA)
        {
            memcpy(qa, scratchSlot + FH_SCRATCH_QUAT_A, sizeof(qa));
        }
        else
        {
            fhAnimEulerToQuat(recA->rot, qa);
        }
        if (quatSrcB)
        {
            memcpy(qb, scratchSlot + FH_SCRATCH_QUAT_B, sizeof(qb));
        }
        else
        {
            fhAnimEulerToQuat(recB->rot, qb);
        }

        dot = qa[0] * qb[0] + qa[1] * qb[1] + qa[2] * qb[2] + qa[3] * qb[3];
        if (dot < 0.0f)
        {
            qb[0] = -qb[0];
            qb[1] = -qb[1];
            qb[2] = -qb[2];
            qb[3] = -qb[3];
        }
        {
            f32 t = (f32)blendFrac * (1.0f / (f32)FH_ANIM_SUBFRAME_SCALE);
            f32 s = 1.0f - t;
            q[0] = qa[0] * s + qb[0] * t;
            q[1] = qa[1] * s + qb[1] * t;
            q[2] = qa[2] * s + qb[2] * t;
            q[3] = qa[3] * s + qb[3] * t;
        }

        for (axis = 0; axis < 3; axis++)
        {
            s32 sa;
            s32 sb;
            s32 ta;
            s32 tb;

            if (quatSrcA)
            {
                sa = *(const s16*)(scratchSlot + FH_SCRATCH_SCALE_A + axis * 2);
                ta = *(const s16*)(scratchSlot + FH_SCRATCH_TRANS_A + axis * 2);
            }
            else
            {
                sa = recA->scale[axis];
                ta = recA->trans[axis];
            }
            if (quatSrcB)
            {
                sb = *(const s16*)(scratchSlot + FH_SCRATCH_SCALE_B + axis * 2);
                tb = *(const s16*)(scratchSlot + FH_SCRATCH_TRANS_B + axis * 2);
            }
            else
            {
                sb = recB->scale[axis];
                tb = recB->trans[axis];
            }
            if (sa == 0)
            {
                sa = FH_ANIM_SCALE_ONE;
            }
            if (sb == 0)
            {
                sb = FH_ANIM_SCALE_ONE;
            }
            scaleOut[axis] = (s16)fhAnimLerpS32(sa, sb, blendFrac);
            if (mode & FH_ANIM_MODE_NO_TRANS_LERP)
            {
                transOut[axis] = (s16)ta;
            }
            else
            {
                transOut[axis] = (s16)fhAnimLerpS32(ta, tb, blendFrac);
            }
        }

        if (useScratchRecords)
        {
            int scaleOff = (mode & FH_ANIM_MODE_QUAT_DST_B) ? FH_SCRATCH_SCALE_B : FH_SCRATCH_SCALE_A;
            int transOff = (mode & FH_ANIM_MODE_QUAT_DST_B) ? FH_SCRATCH_TRANS_B : FH_SCRATCH_TRANS_A;

            memcpy(scratchSlot + scaleOff, scaleOut, sizeof(scaleOut));
            memcpy(scratchSlot + transOff, transOut, sizeof(transOut));
        }
        if (mode & (FH_ANIM_MODE_QUAT_DST_A | FH_ANIM_MODE_QUAT_DST_B))
        {
            int quatOff = (mode & FH_ANIM_MODE_QUAT_DST_B) ? FH_SCRATCH_QUAT_B : FH_SCRATCH_QUAT_A;

            memcpy(scratchSlot + quatOff, q, sizeof(q));
            continue;
        }

        len = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
        if (len > 1.0e-8f)
        {
            len = 1.0f / sqrtf(len);
            q[0] *= len;
            q[1] *= len;
            q[2] *= len;
            q[3] *= len;
        }
        else
        {
            q[0] = 1.0f;
            q[1] = 0.0f;
            q[2] = 0.0f;
            q[3] = 0.0f;
        }

        mtxSlot = mtxBase + (size_t)dstIndex * FH_JOINT_MTX_STRIDE;
        m = (MtxPtr)mtxSlot;
        {
            f32 w = q[0];
            f32 x = q[1];
            f32 y = q[2];
            f32 z = q[3];
            f32 x2 = x + x;
            f32 y2 = y + y;
            f32 z2 = z + z;
            f32 wx = w * x2;
            f32 wy = w * y2;
            f32 wz = w * z2;
            f32 xx = x * x2;
            f32 xy = x * y2;
            f32 xz = x * z2;
            f32 yy = y * y2;
            f32 yz = y * z2;
            f32 zz = z * z2;
            f32 s0 = (f32)scaleOut[0] * FH_ANIM_SCALE_RECIP;
            f32 s1 = (f32)scaleOut[1] * FH_ANIM_SCALE_RECIP;
            f32 s2 = (f32)scaleOut[2] * FH_ANIM_SCALE_RECIP;

            m[0][0] = (1.0f - (yy + zz)) * s0;
            m[1][0] = (xy + wz) * s0;
            m[2][0] = (xz - wy) * s0;
            m[0][1] = (xy - wz) * s1;
            m[1][1] = (1.0f - (xx + zz)) * s1;
            m[2][1] = (yz + wx) * s1;
            m[0][2] = (xz + wy) * s2;
            m[1][2] = (yz - wx) * s2;
            m[2][2] = (1.0f - (xx + yy)) * s2;
            m[0][3] = (f32)transOut[0] * FH_ANIM_TRANS_SCALE + bone->head[0];
            m[1][3] = (f32)transOut[1] * FH_ANIM_TRANS_SCALE + bone->head[1];
            m[2][3] = (f32)transOut[2] * FH_ANIM_TRANS_SCALE + bone->head[2];
        }
    }

    if (mode & (FH_ANIM_MODE_QUAT_DST_A | FH_ANIM_MODE_QUAT_DST_B))
    {
        return;
    }

    for (j = 0; j < jointCount; j++)
    {
        const ModelBone* bone = (const ModelBone*)(jointData + (size_t)j * sizeof(ModelBone));
        s32 tagged = (s32)(s8)bone->idx[0];
        int dstIndex;
        MtxPtr parent;
        MtxPtr child;
        Mtx result;

        if ((tagged & flags) < 0)
        {
            continue;
        }
        dstIndex = (int)(tagged & 0x7f);
        child = (MtxPtr)(mtxBase + (size_t)dstIndex * FH_JOINT_MTX_STRIDE);
        if (j == 0 || bone->parent < 0)
        {
            parent = (MtxPtr)dst;
        }
        else
        {
            parent = (MtxPtr)(mtxBase + (size_t)bone->parent * FH_JOINT_MTX_STRIDE);
        }
        if (parent == NULL)
        {
            continue;
        }
        PSMTXConcat(parent, child, result);
        memcpy(child, result, sizeof(Mtx));
    }

}
