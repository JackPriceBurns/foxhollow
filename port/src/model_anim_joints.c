#include "foxhollow_compat.h"

#include <math.h>
#include <string.h>

#include <dolphin/types.h>
#include <dolphin/mtx.h>

#include "main/model.h"
#include "main/objanim_internal.h"
#include "main/render_internal.h"

extern s16 gModelRootRotX;
extern s16 gModelRootRotY;
extern s16 gModelRootRotZ;

#define FH_BJM_MODE_SRC_A_SCRATCH 0x01
#define FH_BJM_MODE_SRC_B_SCRATCH 0x02
#define FH_BJM_MODE_DST_SCRATCH_A 0x04
#define FH_BJM_MODE_DST_SCRATCH_B 0x08
#define FH_BJM_MODE_DST_SCRATCH (FH_BJM_MODE_DST_SCRATCH_A | FH_BJM_MODE_DST_SCRATCH_B)
#define FH_BJM_MODE_SCRATCH_ANY 0x0f
#define FH_BJM_MODE_NO_TRANS_LERP 0x10
#define FH_BJM_MODE_ROOT_ROT_OVERRIDE 0x20
#define FH_BJM_MODE_SUBFRAME_CHANNEL_A 0x40

#define FH_BJM_RECORD_STRIDE 0x40
#define FH_BJM_RECORD_CHANNEL_B 0x06
#define FH_BJM_REC_ROT 0x00
#define FH_BJM_REC_SCALE 0x0c
#define FH_BJM_REC_TRANS 0x18

#define FH_BJM_MTX_STRIDE 0x40
#define FH_BJM_MTX_QUAT_A 0x00
#define FH_BJM_MTX_QUAT_B 0x10
#define FH_BJM_MTX_SCRATCH_A 0x1c
#define FH_BJM_MTX_SCRATCH_B 0x22

#define FH_BJM_SCALE_ONE 0x400
#define FH_BJM_SUBFRAME_SCALE 16384.0f
#define FH_BJM_TRANS_SCALE (1.0f / 512.0f)
#define FH_BJM_SCALE_RECIP (1.0f / 1024.0f)
#define FH_BJM_OVERRIDE_END 0x1000

#define FH_BJM_RECORD_BYTES (0x10000 + FH_BJM_RECORD_STRIDE)

typedef union FhBjmConst {
    u32 u;
    f32 f;
} FhBjmConst;

static const FhBjmConst kFhBjmSinPoly[4] = {
    {0x83967accu}, {0x15a32d1eu}, {0xa7255dd4u}, {0x37c90fdau}
};

static const FhBjmConst kFhBjmCosPoly[5] = {
    {0x00000767u}, {0x8caae2bdu}, {0x1e81e0eeu}, {0xaf9de9e6u}, {0x3f800000u}
};

static u8 sFhBjmRecords[FH_BJM_RECORD_BYTES];

typedef struct FhBjmPass {
    u8* mtxBase;
    const ModelBone* joints;
    int jointCount;
    int flags;
} FhBjmPass;

typedef struct FhBjmBits {
    const u8* a;
    const u8* b;
    u32 wa;
    u32 wb;
    s32 bit;
} FhBjmBits;

static u32 fhBjmQuantizeU16(f32 v)
{
    if (!(v > 0.0f))
    {
        return 0u;
    }
    if (v >= 65535.0f)
    {
        return 65535u;
    }
    return (u32)v;
}

static s16 fhBjmToS16(u32 v)
{
    u32 lo = v & 0xffffu;

    return (s16)(lo < 0x8000u ? (s32)lo : (s32)lo - 0x10000);
}

static u16 fhBjmGetU16(const u8* p, int off)
{
    u16 v;

    memcpy(&v, p + off, sizeof(v));
    return v;
}

static s16 fhBjmGetS16(const u8* p, int off)
{
    s16 v;

    memcpy(&v, p + off, sizeof(v));
    return v;
}

static void fhBjmSetU16(u8* p, int off, u16 v)
{
    memcpy(p + off, &v, sizeof(v));
}

static f32 fhBjmGetF32(const u8* p, int off)
{
    f32 v;

    memcpy(&v, p + off, sizeof(v));
    return v;
}

static void fhBjmSetF32(u8* p, int off, f32 v)
{
    memcpy(p + off, &v, sizeof(v));
}

static void fhBjmBitsInit(FhBjmBits* s, const u8* a, const u8* b)
{
    s->a = a;
    s->b = b;
    s->wa = fhReadBE32(a);
    s->wb = fhReadBE32(b);
    s->bit = 0;
}

static void fhBjmBitsRead(FhBjmBits* s, s32 count, u32* outA, u32* outB)
{
    u32 shift;

    s->bit += count;
    if (s->bit > 32)
    {
        s32 prev = s->bit - count;
        s32 bytes = prev >> 3;

        s->a += bytes;
        s->b += bytes;
        prev &= 7;
        s->wa = fhReadBE32(s->a) << prev;
        s->wb = fhReadBE32(s->b) << prev;
        s->bit = prev + count;
    }
    shift = (u32)(32 - count);
    *outA = s->wa >> shift;
    *outB = s->wb >> shift;
    s->wa <<= (u32)count;
    s->wb <<= (u32)count;
}

static void fhBjmApplyOverrides(u8* recBase, const s16* overrides, int alsoChannelB)
{
    if (overrides == NULL)
    {
        return;
    }
    for (;;)
    {
        u16 offset = (u16)overrides[0];
        s16 value;
        u8* slot;

        if (offset == FH_BJM_OVERRIDE_END)
        {
            return;
        }
        value = overrides[2];
        slot = recBase + offset;
        fhBjmSetU16(slot, 0, (u16)(fhBjmGetS16(slot, 0) + value));
        if (alsoChannelB)
        {
            fhBjmSetU16(slot, FH_BJM_RECORD_CHANNEL_B,
                        (u16)(fhBjmGetS16(slot, FH_BJM_RECORD_CHANNEL_B) + value));
        }
        overrides += 4;
    }
}

static void fhBjmDecodeChannel(u8* recBase, const u8* frameData, const u8* cursor, s32 stride,
                               f32 phase, const s16* overrides)
{
    FhBjmBits bits;
    const u8* desc;
    s32 remaining;
    s32 comp;
    u8* out;
    u32 frac;
    f32 fracF;

    if (frameData == NULL || cursor == NULL)
    {
        return;
    }

    desc = frameData + 4;
    remaining = (s32)frameData[0] * 3;

    fracF = phase - (f32)fhBjmQuantizeU16(phase);
    frac = fhBjmQuantizeU16(fracF * FH_BJM_SUBFRAME_SCALE);

    fhBjmBitsInit(&bits, cursor, cursor + stride);

    out = recBase;
    comp = 3;
    while (remaining > 0)
    {
        u16 d = fhReadBE16(desc);
        u16 value = (u16)(d & 0xfff0u);
        s32 nb = (s32)(d & 0xfu);

        if (nb != 0)
        {
            u32 va;
            u32 vb;
            s32 delta;

            fhBjmBitsRead(&bits, nb, &va, &vb);
            delta = (s32)(vb - va);
            delta = (s32)((u32)delta << 18) >> 18;
            delta = (s32)((u32)delta * (u32)frac);
            delta >>= 14;
            value = (u16)(value + (u16)(((s32)va + delta) << 2));
        }
        fhBjmSetU16(out, FH_BJM_REC_ROT, value);
        desc += 2;
        fhBjmSetU16(out, FH_BJM_REC_SCALE, 0);
        fhBjmSetU16(out, FH_BJM_REC_TRANS, 0);

        if ((d & 0x10u) != 0)
        {
            u16 e = fhReadBE16(desc);
            int wantTrans = 1;

            if ((e & 0x10u) != 0)
            {
                u16 sv = (u16)(e & 0xffc0u);
                u32 more = (u32)(e & 0x20u);
                s32 nb2 = (s32)(e & 0xfu);

                if (nb2 != 0)
                {
                    u32 va;
                    u32 vb;
                    s32 delta;

                    fhBjmBitsRead(&bits, nb2, &va, &vb);
                    delta = (s32)(vb - va);
                    delta = (s32)((u32)delta * (u32)frac);
                    delta >>= 14;
                    sv = (u16)(sv + (u16)(((s32)va + delta) << 1));
                }
                fhBjmSetU16(out, FH_BJM_REC_SCALE, sv);
                desc += 2;
                e = fhReadBE16(desc);
                if (more == 0)
                {
                    wantTrans = 0;
                }
            }
            if (wantTrans)
            {
                u16 tv = (u16)(e & 0xfff0u);
                s32 nb3 = (s32)(e & 0xfu);

                if (nb3 != 0)
                {
                    u32 va;
                    u32 vb;
                    s32 delta;

                    fhBjmBitsRead(&bits, nb3, &va, &vb);
                    delta = (s32)(vb - va);
                    delta = (s32)((u32)delta << 16) >> 16;
                    delta = (s32)((u32)delta * (u32)frac);
                    delta >>= 14;
                    tv = (u16)(tv + (u16)((s32)va + delta));
                }
                fhBjmSetU16(out, FH_BJM_REC_TRANS, tv);
                desc += 2;
            }
        }

        comp--;
        if (comp == 0)
        {
            comp = 3;
            out += 0x3a;
        }
        out += 2;
        remaining--;
    }

    fhBjmApplyOverrides(recBase, overrides, 0);
}

static void fhBjmDecodeKeyframePair(u8* recBase, const u8* frameData, const u8* cursor, s32 stride,
                                    const s16* overrides)
{
    FhBjmBits bits;
    const u8* desc;
    s32 remaining;
    s32 comp;
    u8* out;

    if (frameData == NULL || cursor == NULL)
    {
        return;
    }

    desc = frameData + 4;
    remaining = (s32)frameData[0] * 3;

    fhBjmBitsInit(&bits, cursor, cursor + stride);

    out = recBase;
    comp = 3;
    while (remaining > 0)
    {
        u16 d = fhReadBE16(desc);
        s32 nb = (s32)(d & 0xfu);
        u16 gate = d;

        if (nb == 0)
        {
            fhBjmSetU16(out, FH_BJM_REC_ROT, d);
            fhBjmSetU16(out, FH_BJM_REC_ROT + FH_BJM_RECORD_CHANNEL_B, d);
        }
        else
        {
            u16 base = (u16)(d & 0xfff0u);
            u32 va;
            u32 vb;

            gate = base;
            fhBjmBitsRead(&bits, nb, &va, &vb);
            fhBjmSetU16(out, FH_BJM_REC_ROT, (u16)((va << 2) + base));
            fhBjmSetU16(out, FH_BJM_REC_ROT + FH_BJM_RECORD_CHANNEL_B, (u16)((vb << 2) + base));
        }
        desc += 2;
        fhBjmSetU16(out, FH_BJM_REC_SCALE, 0);
        fhBjmSetU16(out, FH_BJM_REC_SCALE + FH_BJM_RECORD_CHANNEL_B, 0);
        fhBjmSetU16(out, FH_BJM_REC_TRANS, 0);
        fhBjmSetU16(out, FH_BJM_REC_TRANS + FH_BJM_RECORD_CHANNEL_B, 0);

        if ((gate & 0x10u) != 0)
        {
            u16 e = fhReadBE16(desc);
            int wantTrans = 1;

            if ((e & 0x10u) != 0)
            {
                u32 more = (u32)(e & 0x20u);
                s32 nb2 = (s32)(e & 0xfu);

                if (nb2 == 0)
                {
                    fhBjmSetU16(out, FH_BJM_REC_SCALE, e);
                    fhBjmSetU16(out, FH_BJM_REC_SCALE + FH_BJM_RECORD_CHANNEL_B, e);
                }
                else
                {
                    u16 base = (u16)(e & 0xffc0u);
                    u32 va;
                    u32 vb;

                    fhBjmBitsRead(&bits, nb2, &va, &vb);
                    fhBjmSetU16(out, FH_BJM_REC_SCALE, (u16)((va << 1) + base));
                    fhBjmSetU16(out, FH_BJM_REC_SCALE + FH_BJM_RECORD_CHANNEL_B,
                                (u16)((vb << 1) + base));
                }
                desc += 2;
                e = fhReadBE16(desc);
                if (more == 0)
                {
                    wantTrans = 0;
                }
            }
            if (wantTrans)
            {
                s32 nb3 = (s32)(e & 0xfu);

                if (nb3 == 0)
                {
                    fhBjmSetU16(out, FH_BJM_REC_TRANS, e);
                    fhBjmSetU16(out, FH_BJM_REC_TRANS + FH_BJM_RECORD_CHANNEL_B, e);
                }
                else
                {
                    u16 base = (u16)(e & 0xfff0u);
                    u32 va;
                    u32 vb;

                    fhBjmBitsRead(&bits, nb3, &va, &vb);
                    fhBjmSetU16(out, FH_BJM_REC_TRANS, (u16)(va + base));
                    fhBjmSetU16(out, FH_BJM_REC_TRANS + FH_BJM_RECORD_CHANNEL_B, (u16)(vb + base));
                }
                desc += 2;
            }
        }

        comp--;
        if (comp == 0)
        {
            comp = 3;
            out += 0x3a;
        }
        out += 2;
        remaining--;
    }

    fhBjmApplyOverrides(recBase, overrides, 1);
}

static void fhBjmSinCos(s32 units, f32* outCos, f32* outSin)
{
    f32 x = (f32)fhBjmToS16((u32)units << 2);
    f32 x2 = x * x;
    f32 sp;
    f32 cp;
    u32 quadrant;

    sp = x2 * kFhBjmSinPoly[0].f + kFhBjmSinPoly[1].f;
    sp = x2 * sp + kFhBjmSinPoly[2].f;
    sp = x2 * sp + kFhBjmSinPoly[3].f;
    sp = x * sp;

    cp = x2 * kFhBjmCosPoly[0].f + kFhBjmCosPoly[1].f;
    cp = x2 * cp + kFhBjmCosPoly[2].f;
    cp = x2 * cp + kFhBjmCosPoly[3].f;
    cp = x2 * cp + kFhBjmCosPoly[4].f;

    quadrant = (u32)(units + 0x2000) & 0xc000u;
    if (quadrant == 0u)
    {
        *outSin = sp;
        *outCos = cp;
    }
    else if (quadrant == 0x4000u)
    {
        *outSin = cp;
        *outCos = -sp;
    }
    else if (quadrant == 0x8000u)
    {
        *outSin = -sp;
        *outCos = -cp;
    }
    else
    {
        *outSin = -cp;
        *outCos = sp;
    }
}

static void fhBjmQuatFromEuler(const u8* rec, f32* q)
{
    f32 cx;
    f32 sx;
    f32 cy;
    f32 sy;
    f32 cz;
    f32 sz;
    f32 cxcy;
    f32 cxsy;
    f32 sxcy;
    f32 sxsy;

    fhBjmSinCos(((s32)fhBjmGetS16(rec, 0)) >> 1, &cx, &sx);
    fhBjmSinCos(((s32)fhBjmGetS16(rec, 2)) >> 1, &cy, &sy);
    fhBjmSinCos(((s32)fhBjmGetS16(rec, 4)) >> 1, &cz, &sz);

    cxcy = cx * cy;
    cxsy = cx * sy;
    sxcy = sx * cy;
    sxsy = sx * sy;

    q[0] = (cxcy * cz) + (sxsy * sz);
    q[1] = (sxcy * cz) - (cxsy * sz);
    q[2] = (cxsy * cz) + (sxcy * sz);
    q[3] = (cxcy * sz) - (sxsy * cz);
}

static void fhBjmBlendPass(const FhBjmPass* ctx, u8* srcABase, u8* srcBBase, f32 t, s32 tFixed,
                           int mode)
{
    u8* dstBase;
    f32 oneMinusT;
    int j;

    if ((mode & FH_BJM_MODE_ROOT_ROT_OVERRIDE) != 0)
    {
        fhBjmSetU16(srcBBase, 0, (u16)gModelRootRotX);
        fhBjmSetU16(srcBBase, 2, (u16)gModelRootRotY);
        fhBjmSetU16(srcBBase, 4, (u16)gModelRootRotZ);
    }

    dstBase = srcABase;
    if ((mode & FH_BJM_MODE_DST_SCRATCH) != 0)
    {
        dstBase = ctx->mtxBase + FH_BJM_MTX_SCRATCH_A;
        if ((mode & FH_BJM_MODE_DST_SCRATCH_B) != 0)
        {
            dstBase += FH_BJM_RECORD_CHANNEL_B;
        }
    }

    oneMinusT = 1.0f - t;

    for (j = 0; j < ctx->jointCount; j++)
    {
        const ModelBone* bone = &ctx->joints[j];
        size_t idxA = (size_t)bone->idx[1] * FH_BJM_RECORD_STRIDE;
        size_t idxB = (size_t)bone->idx[2] * FH_BJM_RECORD_STRIDE;
        const u8* srcA = srcABase + idxA;
        const u8* srcB = srcBBase + idxB;
        u8* dst = dstBase + idxA;
        int k;

        if ((mode & FH_BJM_MODE_SCRATCH_ANY) != 0)
        {
            size_t outOff = (size_t)(bone->idx[0] & 0x7f) * FH_BJM_RECORD_STRIDE;

            dst = dstBase + outOff;
            if ((mode & (FH_BJM_MODE_SRC_A_SCRATCH | FH_BJM_MODE_SRC_B_SCRATCH)) != 0)
            {
                if ((mode & FH_BJM_MODE_SRC_A_SCRATCH) != 0)
                {
                    srcA = srcABase + outOff;
                }
                else
                {
                    srcB = srcBBase + outOff;
                }
            }
        }

        for (k = 0; k < 6; k += 2)
        {
            s32 scaleB = (s32)fhBjmGetU16(srcB, FH_BJM_REC_SCALE + k);
            s32 scaleA = (s32)fhBjmGetU16(srcA, FH_BJM_REC_SCALE + k);
            s32 trans = (s32)fhBjmGetS16(srcA, FH_BJM_REC_TRANS + k);
            s32 blended;

            if (scaleB == 0)
            {
                scaleB = FH_BJM_SCALE_ONE;
            }
            if (scaleA == 0)
            {
                scaleA = FH_BJM_SCALE_ONE;
            }
            blended = (s32)((u32)(scaleB - scaleA) * (u32)tFixed);
            blended >>= 14;
            fhBjmSetU16(dst, FH_BJM_REC_SCALE + k, (u16)(blended + scaleA));

            if ((mode & FH_BJM_MODE_NO_TRANS_LERP) == 0)
            {
                s32 transB = (s32)fhBjmGetS16(srcB, FH_BJM_REC_TRANS + k);
                s32 d = (s32)((u32)(transB - trans) * (u32)tFixed);

                d >>= 14;
                trans += d;
            }
            fhBjmSetU16(dst, FH_BJM_REC_TRANS + k, (u16)trans);
        }
    }

    for (j = 0; j < ctx->jointCount; j++)
    {
        const ModelBone* bone = &ctx->joints[j];
        size_t outOff = (size_t)(bone->idx[0] & 0x7f) * FH_BJM_RECORD_STRIDE;
        const u8* rec = dstBase + (size_t)bone->idx[1] * FH_BJM_RECORD_STRIDE;
        f32 qa[4];
        f32 qb[4];
        f32 q[4];
        f32 dot;
        s32 tag;
        s32 outIdx;
        u8* mtx;
        f32 tw2;
        f32 tx2;
        f32 ty2;
        f32 wx;
        f32 wy;
        f32 wz;
        f32 xx;
        f32 xy;
        f32 xz;
        f32 yy;
        f32 yz;
        f32 zz;
        f32 col0[3];
        f32 col1[3];
        f32 col2[3];
        f32 trans[3];
        u32 scaleX;
        u32 scaleY;
        u32 scaleZ;
        int i;

        if ((mode & FH_BJM_MODE_SRC_A_SCRATCH) != 0)
        {
            const u8* slot = ctx->mtxBase + outOff;

            rec = dstBase + outOff;
            qa[0] = fhBjmGetF32(slot, FH_BJM_MTX_QUAT_A + 0);
            qa[1] = fhBjmGetF32(slot, FH_BJM_MTX_QUAT_A + 4);
            qa[2] = fhBjmGetF32(slot, FH_BJM_MTX_QUAT_A + 8);
            qa[3] = fhBjmGetF32(slot, FH_BJM_MTX_QUAT_A + 12);
        }
        else
        {
            fhBjmQuatFromEuler(srcABase + (size_t)bone->idx[1] * FH_BJM_RECORD_STRIDE, qa);
        }

        if ((mode & FH_BJM_MODE_SRC_B_SCRATCH) != 0)
        {
            const u8* slot = ctx->mtxBase + outOff;

            qb[0] = fhBjmGetF32(slot, FH_BJM_MTX_QUAT_B + 0);
            qb[1] = fhBjmGetF32(slot, FH_BJM_MTX_QUAT_B + 4);
            qb[2] = fhBjmGetF32(slot, FH_BJM_MTX_QUAT_B + 8);
            qb[3] = fhBjmGetF32(slot, FH_BJM_MTX_QUAT_B + 12);
        }
        else
        {
            fhBjmQuatFromEuler(srcBBase + (size_t)bone->idx[2] * FH_BJM_RECORD_STRIDE, qb);
        }

        dot = ((qa[0] * qb[0]) + (qa[1] * qb[1])) + (qa[2] * qb[2]);
        dot = dot + (qa[3] * qb[3]);

        qa[0] *= oneMinusT;
        qa[1] *= oneMinusT;
        qa[2] *= oneMinusT;
        if (!(dot >= 0.0f))
        {
            qb[0] = 0.0f - qb[0];
            qb[1] = 0.0f - qb[1];
            qb[2] = 0.0f - qb[2];
            qb[3] = 0.0f - qb[3];
        }
        qa[3] *= oneMinusT;

        tag = (s32)(s8)bone->idx[0];
        if ((tag & ctx->flags) < 0)
        {
            continue;
        }
        outIdx = (tag & ctx->flags) & 0x7f;

        q[0] = qa[0] + (qb[0] * t);
        q[1] = qa[1] + (qb[1] * t);
        q[2] = qa[2] + (qb[2] * t);
        q[3] = qa[3] + (qb[3] * t);

        if ((mode & FH_BJM_MODE_DST_SCRATCH) != 0)
        {
            u8* slot = ctx->mtxBase + outOff;
            int off = ((mode & FH_BJM_MODE_DST_SCRATCH_B) != 0) ? FH_BJM_MTX_QUAT_B
                                                                : FH_BJM_MTX_QUAT_A;

            fhBjmSetF32(slot, off + 0, q[0]);
            fhBjmSetF32(slot, off + 4, q[1]);
            fhBjmSetF32(slot, off + 8, q[2]);
            fhBjmSetF32(slot, off + 12, q[3]);
            continue;
        }

        mtx = ctx->mtxBase + (size_t)outIdx * FH_BJM_MTX_STRIDE;

        for (i = 0; i < 3; i++)
        {
            trans[i] = (f32)fhBjmGetS16(rec, FH_BJM_REC_TRANS + i * 2) * FH_BJM_TRANS_SCALE
                       + bone->head[i];
        }
        scaleX = fhBjmGetU16(rec, FH_BJM_REC_SCALE + 0);
        scaleY = fhBjmGetU16(rec, FH_BJM_REC_SCALE + 2);
        scaleZ = fhBjmGetU16(rec, FH_BJM_REC_SCALE + 4);

        for (i = 0; i < 3; i++)
        {
            fhBjmSetF32(mtx, 0x0c + i * 0x10, trans[i]);
        }

        tw2 = q[1] + q[1];
        tx2 = q[2] + q[2];
        ty2 = q[3] + q[3];
        wx = q[0] * tw2;
        wy = q[0] * tx2;
        wz = q[0] * ty2;
        xx = q[1] * tw2;
        xy = q[1] * tx2;
        xz = q[1] * ty2;
        yy = q[2] * tx2;
        yz = q[2] * ty2;
        zz = q[3] * ty2;

        col0[0] = 1.0f - (yy + zz);
        col0[1] = xy + wz;
        col0[2] = xz - wy;
        col1[0] = xy - wz;
        col1[1] = 1.0f - (xx + zz);
        col1[2] = yz + wx;
        col2[0] = xz + wy;
        col2[1] = yz - wx;
        col2[2] = 1.0f - (xx + yy);

        if (scaleX != 0u)
        {
            f32 s = (f32)scaleX * FH_BJM_SCALE_RECIP;

            fhBjmSetF32(mtx, 0x00, col0[0] * s);
            fhBjmSetF32(mtx, 0x10, col0[1] * s);
            fhBjmSetF32(mtx, 0x20, col0[2] * s);
        }
        else
        {
            fhBjmSetF32(mtx, 0x00, col0[0]);
            fhBjmSetF32(mtx, 0x04, col0[1]);
            fhBjmSetF32(mtx, 0x08, col0[2]);
        }

        if (scaleY != 0u)
        {
            f32 s = (f32)scaleY * FH_BJM_SCALE_RECIP;

            fhBjmSetF32(mtx, 0x04, col1[0] * s);
            fhBjmSetF32(mtx, 0x14, col1[1] * s);
            fhBjmSetF32(mtx, 0x24, col1[2] * s);
        }
        else
        {
            fhBjmSetF32(mtx, 0x04, col1[0]);
            fhBjmSetF32(mtx, 0x14, col1[1]);
            fhBjmSetF32(mtx, 0x24, col1[2]);
        }

        if (scaleZ != 0u)
        {
            f32 s = (f32)scaleZ * FH_BJM_SCALE_RECIP;

            fhBjmSetF32(mtx, 0x08, col2[0] * s);
            fhBjmSetF32(mtx, 0x18, col2[1] * s);
            fhBjmSetF32(mtx, 0x28, col2[2] * s);
        }
        else
        {
            fhBjmSetF32(mtx, 0x08, col2[0]);
            fhBjmSetF32(mtx, 0x18, col2[1]);
            fhBjmSetF32(mtx, 0x28, col2[2]);
        }
    }
}

static void fhBjmEulerPass(const FhBjmPass* ctx, const u8* recBase)
{
    int j;

    for (j = 0; j < ctx->jointCount; j++)
    {
        const ModelBone* bone = &ctx->joints[j];
        s32 tag = (s32)(s8)bone->idx[0];
        s32 outIdx = tag & ctx->flags;
        const u8* rec;
        u8* mtx;
        f32 cx;
        f32 sx;
        f32 cy;
        f32 sy;
        f32 cz;
        f32 sz;
        f32 cxsz;
        f32 sxsz;
        f32 sxcz;
        f32 cxcz;
        f32 col0[3];
        f32 col1[3];
        f32 col2[3];
        u32 scaleX;
        u32 scaleY;
        u32 scaleZ;
        int i;

        if (outIdx < 0)
        {
            continue;
        }

        mtx = ctx->mtxBase + (size_t)outIdx * FH_BJM_MTX_STRIDE;
        rec = recBase + (size_t)bone->idx[1] * FH_BJM_RECORD_STRIDE;

        fhBjmSinCos((s32)fhBjmGetU16(rec, 0), &cx, &sx);
        fhBjmSinCos((s32)fhBjmGetU16(rec, 2), &cy, &sy);
        fhBjmSinCos((s32)fhBjmGetU16(rec, 4), &cz, &sz);

        for (i = 0; i < 3; i++)
        {
            f32 trans = (f32)fhBjmGetS16(rec, FH_BJM_REC_TRANS + i * 2) * FH_BJM_TRANS_SCALE;

            fhBjmSetF32(mtx, 0x0c + i * 0x10, bone->head[i] + trans);
        }

        cxsz = cx * sz;
        sxsz = sx * sz;
        sxcz = sx * cz;
        cxcz = cx * cz;

        col0[0] = cy * cz;
        col0[1] = cy * sz;
        col0[2] = 0.0f - sy;
        col1[0] = (sxcz * sy) - cxsz;
        col1[1] = (sxsz * sy) + cxcz;
        col1[2] = sx * cy;
        col2[0] = (cxcz * sy) + sxsz;
        col2[1] = (cxsz * sy) - sxcz;
        col2[2] = cx * cy;

        scaleX = fhBjmGetU16(rec, FH_BJM_REC_SCALE + 0);
        scaleY = fhBjmGetU16(rec, FH_BJM_REC_SCALE + 2);
        scaleZ = fhBjmGetU16(rec, FH_BJM_REC_SCALE + 4);

        if (scaleX != 0u)
        {
            f32 s = (f32)scaleX * FH_BJM_SCALE_RECIP;

            col0[0] *= s;
            col0[1] *= s;
            col0[2] *= s;
        }
        fhBjmSetF32(mtx, 0x00, col0[0]);
        fhBjmSetF32(mtx, 0x10, col0[1]);
        fhBjmSetF32(mtx, 0x20, col0[2]);

        if (scaleY != 0u)
        {
            f32 s = (f32)scaleY * FH_BJM_SCALE_RECIP;

            col1[0] *= s;
            col1[1] *= s;
            col1[2] *= s;
        }
        fhBjmSetF32(mtx, 0x04, col1[0]);
        fhBjmSetF32(mtx, 0x14, col1[1]);
        fhBjmSetF32(mtx, 0x24, col1[2]);

        if (scaleZ != 0u)
        {
            f32 s = (f32)scaleZ * FH_BJM_SCALE_RECIP;

            col2[0] *= s;
            col2[1] *= s;
            col2[2] *= s;
        }
        fhBjmSetF32(mtx, 0x08, col2[0]);
        fhBjmSetF32(mtx, 0x18, col2[1]);
        fhBjmSetF32(mtx, 0x28, col2[2]);
    }
}

static void fhBjmConcatPass(const FhBjmPass* ctx, u8* dst, int mode)
{
    int j;

    if ((mode & FH_BJM_MODE_DST_SCRATCH) != 0)
    {
        return;
    }

    for (j = 0; j < ctx->jointCount; j++)
    {
        const ModelBone* bone = &ctx->joints[j];
        s32 tag = (s32)(s8)bone->idx[0];
        s32 outIdx = tag & ctx->flags;
        MtxPtr parent;
        MtxPtr child;

        if (outIdx < 0)
        {
            continue;
        }

        if (j == 0)
        {
            child = (MtxPtr)(ctx->mtxBase + (size_t)(tag & 0x7f) * FH_BJM_MTX_STRIDE);
            parent = (MtxPtr)dst;
        }
        else
        {
            child = (MtxPtr)(ctx->mtxBase + (size_t)outIdx * FH_BJM_MTX_STRIDE);
            parent = (MtxPtr)(ctx->mtxBase + (size_t)(u8)bone->parent * FH_BJM_MTX_STRIDE);
        }
        if (parent == NULL)
        {
            continue;
        }
        PSMTXConcat(parent, child, child);
    }
}

void modelAnimBuildJointMatrices(int* out, u8* dst, void* animState, u8* jointData, int jointCount,
                                 u8* scratch, int flags, int mode)
{
    ObjAnimState* work = (ObjAnimState*)animState;
    FhBjmPass ctx;
    const s16* overrides = (const s16*)scratch;
    u8* recA = sFhBjmRecords;
    u8* recB = sFhBjmRecords + FH_BJM_RECORD_CHANNEL_B;

    if (out == NULL || animState == NULL || jointData == NULL || jointCount <= 0)
    {
        return;
    }

    ctx.mtxBase = *(u8**)out;
    ctx.joints = (const ModelBone*)jointData;
    ctx.jointCount = jointCount;
    ctx.flags = flags;

    if (ctx.mtxBase == NULL)
    {
        return;
    }

    if ((mode & FH_BJM_MODE_SUBFRAME_CHANNEL_A) != 0)
    {
        f32 phase = work->framePhases[0];
        f32 frac;
        s16 fracFixed;
        s16 blend;

        fhBjmDecodeKeyframePair(recA, (const u8*)work->frameData[0], work->frameStreamCursors[0],
                                work->frameStreamStrides[0], overrides);

        frac = phase - (f32)fhBjmQuantizeU16(phase);
        fracFixed = fhBjmToS16(fhBjmQuantizeU16(frac * FH_BJM_SUBFRAME_SCALE));
        fhBjmBlendPass(&ctx, recA, recB, frac, fracFixed, FH_BJM_MODE_DST_SCRATCH_A);

        fhBjmDecodeChannel(recB, (const u8*)work->frameData[1], work->frameStreamCursors[1],
                           work->frameStreamStrides[1], work->framePhases[1], overrides);

        blend = (s16)work->eventCountdown;
        fhBjmBlendPass(&ctx, ctx.mtxBase + FH_BJM_MTX_SCRATCH_A, recB,
                       (f32)blend / FH_BJM_SUBFRAME_SCALE, blend, FH_BJM_MODE_SRC_A_SCRATCH);
        fhBjmConcatPass(&ctx, dst, FH_BJM_MODE_SRC_A_SCRATCH);
        return;
    }

    if ((mode & FH_BJM_MODE_SRC_A_SCRATCH) != 0)
    {
        recA = ctx.mtxBase + FH_BJM_MTX_SCRATCH_A;
    }
    else
    {
        fhBjmDecodeChannel(recA, (const u8*)work->frameData[0], work->frameStreamCursors[0],
                           work->frameStreamStrides[0], work->framePhases[0], overrides);
        if ((s16)work->eventCountdown <= 0)
        {
            fhBjmEulerPass(&ctx, sFhBjmRecords);
            fhBjmConcatPass(&ctx, dst, mode);
            return;
        }
    }

    if ((mode & FH_BJM_MODE_SRC_B_SCRATCH) != 0)
    {
        recB = ctx.mtxBase + FH_BJM_MTX_SCRATCH_B;
    }
    else
    {
        fhBjmDecodeChannel(recB, (const u8*)work->frameData[1], work->frameStreamCursors[1],
                           work->frameStreamStrides[1], work->framePhases[1], overrides + 1);
    }

    {
        s16 blend = (s16)work->eventCountdown;

        fhBjmBlendPass(&ctx, recA, recB, (f32)blend / FH_BJM_SUBFRAME_SCALE, blend, mode);
    }
    fhBjmConcatPass(&ctx, dst, mode);
}
