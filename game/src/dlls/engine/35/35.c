#include "game/objects/object.h"
#include "dolphin/MSL_C/PPCEABI/bare/H/math_trig_api.h"
#include "main/dll/partfxspawn_struct.h"
#include "main/dll_000A_expgfx.h"
#include "main/frame_timing.h"
#include "main/dll/dll_0023_effect10.h"
#include "main/vecmath.h"

f32 gEffect10SineValueA;
f32 gEffect10SineValueB;
s32 gEffect10SineAnglePhaseB;
s32 gEffect10SineAnglePhaseA;

f32 gEffect10ScrollPhaseA = 0.1f;
f32 gEffect10ScrollPhaseB = 0.3f;
f32 gEffect10TickScrollPhaseA = 0.1f;
f32 gEffect10TickScrollPhaseB = 0.3f;

#define EFFECT10_FLAG_USE_SRC       0x200000
#define EFFECT10_FLAGA_POS_RELATIVE 0x1
#define EFFECT10_FLAGA_UNK2         0x2

PartFxSpawnParams gEffect10DefaultSrcParams;

ObjectDescriptor6 Effect10_funcs = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_6_SLOTS,
    (ObjectDescriptorCallback)Effect10_initialise,
    (ObjectDescriptorCallback)Effect10_release,
    0,
    (ObjectDescriptorCallback)Effect10_func03_nop,
    (ObjectDescriptorCallback)Effect10_spawnObject,
    (ObjectDescriptorCallback)Effect10_updateFrameState,
};

int Effect10_spawnObject(s16* obj, int id, PartFxSpawnParams* src, u32 flags, u8 srcByte, f32* extraParam)
{
    PartFxSpawn p;
    u32 hasSrc;

    gEffect10ScrollPhaseA += 0.001f;
    if (gEffect10ScrollPhaseA > 1.0f)
    {
        gEffect10ScrollPhaseA = 0.1f;
    }
    gEffect10ScrollPhaseB += 0.0003f;
    if (gEffect10ScrollPhaseB > 1.0f)
    {
        gEffect10ScrollPhaseB = 0.3f;
    }
    if (obj == NULL)
    {
        return -1;
    }
    hasSrc = flags & EFFECT10_FLAG_USE_SRC;
    if (hasSrc != 0)
    {
        if (src == NULL)
        {
            return -1;
        }
        p.sourcePosX = src->posX;
        p.sourcePosY = src->posY;
        p.sourcePosZ = src->posZ;
        p.sourceScale = src->scale;
        p.rot2 = src->rotZ;
        p.rot1 = src->rotY;
        p.rot0 = src->rotX;
        p.srcFlag = srcByte;
    }
    p.flagsA = 0;
    p.flagsB = 0;
    p.idByte = id;
    p.model = obj;
    p.posX = 0.0f;
    p.posY = 0.0f;
    p.posZ = 0.0f;
    p.velX = 0.0f;
    p.velY = 0.0f;
    p.velZ = 0.0f;
    p.scale = 0.0f;
    p.count = 0;
    p.unk04 = -1;
    p.alpha = 0xff;
    p.linkGroup = 0;
    p.kind = 0;
    p.colD = 0xffff;
    p.colE = 0xffff;
    p.colF = 0xffff;
    p.colA = 0xffff;
    p.colB = 0xffff;
    p.colC = 0xffff;
    p.unk40 = 0;
    if (src == NULL)
    {
        gEffect10DefaultSrcParams.posX = 0.0f;
        gEffect10DefaultSrcParams.posY = 0.0f;
        gEffect10DefaultSrcParams.posZ = 0.0f;
        gEffect10DefaultSrcParams.scale = 1.0f;
        gEffect10DefaultSrcParams.rotX = 0;
        gEffect10DefaultSrcParams.rotY = 0;
        gEffect10DefaultSrcParams.rotZ = 0;
        src = &gEffect10DefaultSrcParams;
    }
    switch (id)
    {
    case 0x32a:
        p.count = (int)(50.0f * src->scale + 20.0f);
        p.scale = 0.0008f * (f32)(int)p.count;
        p.flagsA = 0x8100200;
        p.kind = 0x57;
        p.sourcePosX = src->posX;
        p.sourcePosY = src->posY;
        p.sourcePosZ = src->posZ;
        p.sourceScale = 1.0f;
        p.rot2 = 0;
        p.rot1 = 0;
        p.rot0 = src->rotX;
        p.alpha = 0xff;
        break;
    case 0x32b:
        p.count = (int)(src->scale * (f32)randomGetRange(0x96, 0xc8) + 50.0f);
        p.scale = 0.00014f * (f32)(int)p.count;
        p.flagsA = 0x8100200;
        p.kind = 0x56;
        p.sourcePosX = src->posX;
        p.sourcePosY = src->posY;
        p.sourcePosZ = src->posZ;
        p.sourceScale = 1.0f;
        p.rot2 = 0;
        p.rot1 = 0;
        p.rot0 = 0;
        p.alpha = 0xff;
        break;
    case 0x32c:
        p.scale = 0.02f * (f32)randomGetRange(2, 4);
        p.count = 200;
        p.flagsA = 0x8100200;
        p.kind = 0x56;
        p.sourcePosX = src->posX;
        p.sourcePosY = src->posY;
        p.sourcePosZ = src->posZ;
        p.sourceScale = 1.0f;
        p.rot2 = 0;
        p.rot1 = 0;
        p.rot0 = 0;
        p.alpha = 0xff;
        break;
    case 0x32d:
        p.scale = 0.025f;
        p.count = 0x32;
        p.flagsA = 0x180200;
        p.flagsB = 0x1000000;
        p.kind = 0x60;
        p.alpha = 0xff;
        break;
    case 0x32e:
    {
        u16 color;

        p.velX = 0.004f * (f32)randomGetRange(-0x28, 0x28);
        p.velY = 0.002f * (f32)randomGetRange(0xa, 0x50);
        p.velZ = 0.004f * (f32)randomGetRange(-0x28, 0x28);
        p.scale = 0.0001f * (f32)randomGetRange(5, 0x19);
        p.count = randomGetRange(0x64, 0x78);
        p.rot0 = randomGetRange(0, 0xffff);
        p.rot1 = randomGetRange(0, 0xffff);
        p.rot0 = randomGetRange(0, 0xffff);
        p.sourcePosX = (f32)randomGetRange(0xe6, 0x320);
        p.sourcePosY = (f32)randomGetRange(0xe6, 0x320);
        p.sourcePosZ = (f32)randomGetRange(0xe6, 0x320);
        p.flagsB = 0x1000020;
        p.flagsA = 0x86000008;
        color = randomGetRange(0x8000, 0xffff);
        p.colD = color;
        p.colA = color;
        p.colB = p.colE = 0xffff;
        p.colC = p.colF = 0xffff;
        p.kind = 0x3a3;
        break;
    }
    case 0x32f:
        p.posX = src->posX;
        p.posY = src->posY;
        p.posZ = src->posZ;
        p.velX = 0.0035f * (f32)randomGetRange(-100, 100);
        p.velY = 0.0035f * (f32)randomGetRange(-100, 100);
        p.velZ = 0.0035f * (f32)randomGetRange(-100, 100);
        p.scale = src->scale * (0.00065f * (f32)randomGetRange(4, 5));
        p.count = randomGetRange(0xf, 0x23);
        p.alpha = 0xff;
        p.flagsA = 0x80110;
        p.flagsB = 0x8400c00;
        p.kind = 0xc79;
        break;
    case 0x330:
        p.posX = 0.001f * (f32)randomGetRange(-100, 100) + src->posX;
        p.posY = 0.001f * (f32)randomGetRange(-100, 100) + src->posY;
        p.posZ = 0.001f * (f32)randomGetRange(-100, 100) + src->posZ;
        p.velX = 0.005f * (f32)randomGetRange(-100, 100);
        p.velY = 0.005f * (f32)randomGetRange(-100, 100);
        p.velZ = 0.005f * (f32)randomGetRange(-100, 100);
        p.scale = 0.004f * src->scale;
        p.count = randomGetRange(0xf, 0x23);
        p.alpha = 0xff;
        p.flagsA = 0x80100;
        p.flagsB = 0x4400c00;
        p.kind = 0xc74;
        break;
    case 0x332:
        p.velX = 0.01f * (f32)randomGetRange(-0x14, 0x14);
        p.velY = 0.01f;
        p.velZ = 0.01f * (f32)randomGetRange(-0x14, 0x14);
        p.scale = 0.0052f;
        p.count = 0x96;
        p.flagsA = 0xa100100;
        p.kind = 0x62;
        break;
    case 0x336:
    {
        f32 scale;

        if (extraParam != NULL)
        {
            scale = *extraParam;
        }
        else
        {
            scale = 1.0f;
        }
        p.posX = scale * (f32)randomGetRange(-10, 10);
        p.posY = scale * (f32)randomGetRange(-10, 10);
        p.posZ = scale * (f32)randomGetRange(-10, 10);
        p.velX = scale * (0.02f * (f32)randomGetRange(-0xf, 0xf));
        p.velY = scale * (0.02f * (f32)randomGetRange(-0xf, 0xf));
        p.velZ = scale * (0.02f * (f32)randomGetRange(-0xf, 0xf));
        p.scale = 0.0004f * (f32)randomGetRange(8, 10);
        p.count = 0x50;
        p.flagsA = 0x80480404;
        p.flagsB = 0x20;
        p.colF = 0;
        p.colE = 0;
        p.colD = 0;
        p.colC = 0;
        p.colB = 0;
        p.colA = 0;
        p.kind = 0xc9d;
        break;
    }
    case 0x337:
    {
        int mode;

        if (extraParam != NULL)
        {
            mode = *(int*)extraParam;
        }
        else
        {
            mode = 0;
        }
        if (mode == 0)
        {
            p.scale = 0.02f;
            p.count = 1;
            p.flagsA = 0x480000;
        }
        else if (mode == 1)
        {
            p.scale = 0.04f;
            p.count = 1;
            p.flagsA = 0x480000;
            p.alpha = 0x32;
        }
        else if (mode == 2)
        {
            p.velX = 0.02f * (f32)randomGetRange(-0xf, 0xf);
            p.velY = 0.02f * (f32)randomGetRange(-0xf, 0xf);
            p.velZ = 0.02f * (f32)randomGetRange(-10, 10);
            p.scale = 0.005f;
            p.count = randomGetRange(0x1e, 0x28);
            p.flagsA = 0x3000000;
            p.flagsB = 0x600000;
        }
        else if (mode == 3)
        {
            p.posX = (f32)randomGetRange(-10, 10);
            p.posY = (f32)randomGetRange(-10, 10);
            p.posZ = (f32)randomGetRange(-10, 10);
            p.velX = 0.02f * (f32)randomGetRange(-0xf, 0xf);
            p.velY = 0.02f * (f32)randomGetRange(-0xf, 0xf);
            p.velZ = 0.02f * (f32)randomGetRange(-0xf, 0xf);
            p.scale = 0.0004f * (f32)randomGetRange(8, 10);
            p.count = 0x1e;
            p.alpha = 0xb4;
            p.flagsA = 0x80480404;
        }
        else
        {
            p.posX = (f32)randomGetRange(-3, 3);
            p.posY = (f32)randomGetRange(-3, 3);
            p.posZ = (f32)randomGetRange(-3, 3);
            p.scale = 0.003f;
            p.count = 100;
            p.flagsA = 0x80480000;
            p.flagsB = 0x400000;
            p.alpha = 0x7f;
        }
        p.kind = 0xc7e;
        break;
    }
    case 0x338:
    {
        int mode;

        if (extraParam != NULL)
        {
            mode = *(int*)extraParam;
        }
        else
        {
            mode = 0;
        }
        if (mode == 0)
        {
            p.scale = 0.02f;
            p.count = 1;
            p.flagsA = 0x480000;
        }
        else if (mode == 1)
        {
            p.scale = 0.04f;
            p.count = 1;
            p.flagsA = 0x480000;
            p.alpha = 0x32;
        }
        else if (mode == 2)
        {
            p.velX = 0.02f * (f32)randomGetRange(-0xf, 0xf);
            p.velY = 0.02f * (f32)randomGetRange(-0xf, 0xf);
            p.velZ = 0.02f * (f32)randomGetRange(-10, 10);
            p.scale = 0.005f;
            p.count = randomGetRange(0x1e, 0x28);
            p.flagsA = 0x3000000;
            p.flagsB = 0x600000;
        }
        else if (mode == 3)
        {
            p.posX = (f32)randomGetRange(-10, 10);
            p.posY = (f32)randomGetRange(-10, 10);
            p.posZ = (f32)randomGetRange(-10, 10);
            p.velX = 0.02f * (f32)randomGetRange(-0xf, 0xf);
            p.velY = 0.02f * (f32)randomGetRange(-0xf, 0xf);
            p.velZ = 0.02f * (f32)randomGetRange(-0xf, 0xf);
            p.scale = 0.0004f * (f32)randomGetRange(8, 10);
            p.count = 0x1e;
            p.alpha = 0xb4;
            p.flagsA = 0x80480404;
        }
        else
        {
            p.posX = (f32)randomGetRange(-3, 3);
            p.posY = (f32)randomGetRange(-3, 3);
            p.posZ = (f32)randomGetRange(-3, 3);
            p.scale = 0.003f;
            p.count = 100;
            p.flagsA = 0x80480000;
            p.flagsB = 0x400000;
            p.alpha = 0x7f;
        }
        p.kind = 0x4f9;
        break;
    }
    case 0x340:
        p.velX = 0.02f * (f32)randomGetRange(-100, 100);
        p.velY = 0.02f * (f32)randomGetRange(10, 200);
        p.velZ = 0.02f * (f32)randomGetRange(-100, 100);
        p.scale = 0.0001f * (f32)randomGetRange(8, 0xb);
        p.count = 0x4b;
        p.flagsA = 0x1080000;
        p.kind = 0xc0f;
        break;
    case 0x342:
        p.velX = 0.02f * (f32)randomGetRange(-100, 100);
        p.velY = 0.012f * (f32)randomGetRange(0x14, 100);
        p.velZ = 0.02f * (f32)randomGetRange(-100, 100);
        p.scale = 0.0015f;
        p.count = 0x28;
        p.flagsA = 0x1080200;
        p.kind = 0xc0f;
        break;
    case 0x343:
        p.velX = 0.02f * (f32)randomGetRange(-100, 100);
        p.velY = 0.02f * (f32)randomGetRange(10, 200);
        p.velZ = 0.02f * (f32)randomGetRange(-100, 100);
        p.scale = 0.00015f * (f32)randomGetRange(8, 0xb);
        p.count = randomGetRange(0x41, 0x4b);
        p.flagsA = 0x1080000;
        p.flagsB = 0x5000000;
        p.kind = 0x77;
        p.alpha = randomGetRange(0x46, 100);
        break;
    case 0x344:
        p.velX = 0.02f * (f32)randomGetRange(-100, 100);
        p.velY = 0.012f * (f32)randomGetRange(0x14, 100);
        p.velZ = 0.02f * (f32)randomGetRange(-100, 100);
        p.scale = 0.00015f * (f32)randomGetRange(5, 10);
        p.count = 0x28;
        p.flagsA = 0x1080200;
        p.kind = 0x77;
        p.alpha = 0x7f;
        break;
    case 0x345:
        p.velX = 0.02f * (f32)randomGetRange(-10, 10);
        p.velY = 0.02f * (f32)randomGetRange(0x14, 0x28);
        p.velZ = 0.02f * (f32)randomGetRange(-10, 10);
        p.posX = (f32)randomGetRange(-10, 10);
        p.posY = -4.0f;
        p.posZ = (f32)randomGetRange(-10, 10);
        p.scale = 0.009f;
        p.count = randomGetRange(0x14, 0x23);
        p.flagsA = 0x1080200;
        p.flagsB = 0x5000000;
        p.kind = 0x60;
        p.alpha = randomGetRange(0x96, 200);
        break;
    case 0x346:
        p.posX = src->posX;
        p.posY = src->posY;
        p.posZ = src->posZ;
        p.scale = 0.001f * (f32)randomGetRange(5, 0x19) + src->scale;
        p.count = 0x1e0;
        p.linkGroup = 0;
        p.flagsA = 0x480014;
        p.kind = 0xdf;
        break;
    case 0x347:
        p.velX = 0.02f * (f32)randomGetRange(-0x1e, 0x1e);
        p.velY = 0.02f * (f32)randomGetRange(-5, 10);
        p.velZ = 0.02f * (f32)randomGetRange(-0x1e, 0x1e);
        p.posX = 0.0f;
        p.posY = (f32)randomGetRange(10, 0x1e);
        p.posZ = 0.0f;
        p.scale = 0.01f;
        p.count = 0x32;
        p.flagsA = 0x8a000208;
        p.kind = 0x60;
        p.colD = 0x7f00;
        p.colE = 0x6400;
        p.colF = 0;
        p.colA = 0x5a00;
        p.colB = 0;
        p.colC = 0;
        p.flagsB = 0x20;
        p.alpha = 0x7f;
        break;
    case 0x34c:
        p.scale = 0.025f;
        p.count = 0x32;
        p.flagsA = 0x180200;
        p.flagsB = 0x1000000;
        p.kind = 0x2b;
        p.alpha = 0x9d;
        break;
    case 0x34d:
    {
        u16 color;

        p.velX = 0.004f * (f32)randomGetRange(-0x28, 0x28);
        p.velY = 0.002f * (f32)randomGetRange(10, 0x50);
        p.velZ = 0.004f * (f32)randomGetRange(-0x28, 0x28);
        p.scale = 0.0001f * (f32)randomGetRange(5, 0x19);
        p.count = randomGetRange(0x64, 0x78);
        p.rot0 = randomGetRange(0, 0xffff);
        p.rot1 = randomGetRange(0, 0xffff);
        p.rot0 = randomGetRange(0, 0xffff);
        p.sourcePosX = (f32)randomGetRange(0xe6, 0x320);
        p.sourcePosY = (f32)randomGetRange(0xe6, 0x320);
        p.sourcePosZ = (f32)randomGetRange(0xe6, 0x320);
        p.flagsB = 0x1000020;
        p.flagsA = 0x86000008;
        color = randomGetRange(0, 0x2ee0) + 0x3caf;
        p.colD = color;
        p.colA = color;
        color = p.colA - randomGetRange(0, 0x2710);
        p.colE = color;
        p.colB = color;
        color = p.colA - randomGetRange(0x2710, 0x3caf);
        p.colF = color;
        p.colC = color;
        p.kind = 0x3a3;
        break;
    }
    case 0x34e:
    {
        u16 color;

        p.velX = 0.004f * (f32)randomGetRange(-0x28, 0x28);
        p.velY = 0.002f * (f32)randomGetRange(10, 0x50);
        p.velZ = 0.004f * (f32)randomGetRange(-0x28, 0x28);
        p.posY = (f32)randomGetRange(5, 0x1e);
        p.scale = 0.0001f * (f32)randomGetRange(5, 0x19);
        p.count = randomGetRange(0x64, 0x78);
        p.rot0 = randomGetRange(0, 0xffff);
        p.rot1 = randomGetRange(0, 0xffff);
        p.rot0 = randomGetRange(0, 0xffff);
        p.sourcePosX = (f32)randomGetRange(0xe6, 0x320);
        p.sourcePosY = (f32)randomGetRange(0xe6, 0x320);
        p.sourcePosZ = (f32)randomGetRange(0xe6, 0x320);
        p.flagsB = 0x1000020;
        p.flagsA = 0x86000008;
        color = randomGetRange(0, 0x2ee0) + 0x3caf;
        p.colD = color;
        p.colA = color;
        p.colE = 0x7530;
        p.colB = 0x7530;
        color = p.colA - randomGetRange(0x2710, 0x3caf);
        p.colF = color;
        p.colC = color;
        p.kind = 0x3a3;
        break;
    }
    case 0x331:
    case 0x333:
    case 0x334:
    case 0x335:
    case 0x339:
        break;
    default:
        return -1;
    }
    p.flagsA = p.flagsA | flags;
    if (((p.flagsA & EFFECT10_FLAGA_POS_RELATIVE) != 0) && ((p.flagsA & EFFECT10_FLAGA_UNK2) != 0))
    {
        p.flagsA ^= 2LL;
    }
    if ((p.flagsA & EFFECT10_FLAGA_POS_RELATIVE) != 0)
    {
        if (hasSrc != 0)
        {
            p.posX = p.posX + p.sourcePosX;
            p.posY = p.posY + p.sourcePosY;
            p.posZ = p.posZ + p.sourcePosZ;
        }
        else if (p.model != NULL)
        {
            p.posX = p.posX + ((GameObject*)p.model)->anim.worldPosX;
            p.posY = p.posY + ((GameObject*)p.model)->anim.worldPosY;
            p.posZ = p.posZ + ((GameObject*)p.model)->anim.worldPosZ;
        }
    }
    return (*gExpgfxInterface)->spawnEffect(&p, -1, id, 0);
}

void Effect10_updateFrameState(void)
{
    f32 sum;
    f32 step;
    sum = gEffect10TickScrollPhaseA + (step = 0.001f * timeDelta);
    gEffect10TickScrollPhaseA = sum;
    if (sum > 1.0f)
    {
        gEffect10TickScrollPhaseA = 0.1f;
    }
    sum = gEffect10TickScrollPhaseB + step;
    gEffect10TickScrollPhaseB = sum;
    if (sum > 1.0f)
    {
        gEffect10TickScrollPhaseB = 0.3f;
    }
    gEffect10SineAnglePhaseA = gEffect10SineAnglePhaseA + framesThisStep * 0x64;
    if (gEffect10SineAnglePhaseA > 0x7fff)
    {
        gEffect10SineAnglePhaseA = 0;
    }
    gEffect10SineValueA = mathSinf(3.1415927f * (f32)(s16)gEffect10SineAnglePhaseA / 32768.0f);
    gEffect10SineAnglePhaseB = gEffect10SineAnglePhaseB + framesThisStep * 0x32;
    if (gEffect10SineAnglePhaseB > 0x7fff)
    {
        gEffect10SineAnglePhaseB = 0;
    }
    gEffect10SineValueB = mathSinf(3.1415927f * (f32)(s16)gEffect10SineAnglePhaseB / 32768.0f);
}

void Effect10_func03_nop(void)
{
}

void Effect10_release(void)
{
}

void Effect10_initialise(void)
{
}
