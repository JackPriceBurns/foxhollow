#include "game/objects/object.h"
#include "main/dll/partfxspawn_struct.h"
#include "main/dll_000A_expgfx.h"
#include "main/dll/dll_0024_effect11.h"
#include "main/vecmath.h"

PartFxSpawnParams gEffect11DefaultSrcParams;

ObjectDescriptor6 Effect11_funcs = {
    0,
    0,
    0,
    0x00050000,
    (ObjectDescriptorCallback)Effect11_initialise,
    (ObjectDescriptorCallback)Effect11_release,
    0,
    (ObjectDescriptorCallback)Effect11_func03_nop,
    (ObjectDescriptorCallback)Effect11_spawnObject,
    (ObjectDescriptorCallback)Effect11_func05_nop,
};

int Effect11_spawnObject(s16* obj, int id, PartFxSpawnParams* src, u32 flags, u8 srcByte)
{
    PartFxSpawn p;
    u32 hasOffset;

    if (obj == NULL)
    {
        return -1;
    }
    hasOffset = flags & 0x200000;
    if (hasOffset != 0)
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
    switch (id)
    {
    case 0x12c:
        p.scale = 0.0016f;
        p.count = 0xa;
        p.alpha = 0xff;
        p.flagsA = 0x40200;
        p.kind = 0xdb;
        break;
    case 0x12d:
        if (src == NULL)
        {
            gEffect11DefaultSrcParams.posX = 0.0f;
            gEffect11DefaultSrcParams.posY = 0.0f;
            gEffect11DefaultSrcParams.posZ = 0.0f;
            gEffect11DefaultSrcParams.scale = 1.0f;
            gEffect11DefaultSrcParams.rotX = 0;
            gEffect11DefaultSrcParams.rotY = 0;
            gEffect11DefaultSrcParams.rotZ = 0;
            src = &gEffect11DefaultSrcParams;
        }
        p.scale = 0.025f;
        p.count = randomGetRange(0, 0x1e) + 0x46;
        p.alpha = src->scale > 0.0f ? 0x50 : 0x41;
        p.flagsA = 0x80110;
        p.kind = src->scale > 0.0f ? 0x7b : 0xdb;
        break;
    case 0x12e:
        if (src == NULL)
        {
            gEffect11DefaultSrcParams.posX = 0.0f;
            gEffect11DefaultSrcParams.posY = 0.0f;
            gEffect11DefaultSrcParams.posZ = 0.0f;
            gEffect11DefaultSrcParams.scale = 1.0f;
            gEffect11DefaultSrcParams.rotX = 0;
            gEffect11DefaultSrcParams.rotY = 0;
            gEffect11DefaultSrcParams.rotZ = 0;
            src = &gEffect11DefaultSrcParams;
        }
        p.posX = 0.6f * (f32)randomGetRange(-10, 10);
        p.posY = 0.0f;
        p.posZ = 15.0f;
        p.velY = 0.1f * (f32)randomGetRange(1, 3);
        p.velX = 0.6f * src->posX;
        p.velZ = 0.6f * -src->posZ;
        p.scale = 0.0016f * (f32)randomGetRange(1, 3);
        p.count = 0x19;
        p.alpha = 0x55;
        p.flagsA = 0x80118;
        p.kind = 0x5f;
        break;
    case 0x12f:
        if (src == NULL)
        {
            gEffect11DefaultSrcParams.posX = 0.0f;
            gEffect11DefaultSrcParams.posY = 0.0f;
            gEffect11DefaultSrcParams.posZ = 0.0f;
            gEffect11DefaultSrcParams.scale = 1.0f;
            gEffect11DefaultSrcParams.rotX = 0;
            gEffect11DefaultSrcParams.rotY = 0;
            gEffect11DefaultSrcParams.rotZ = 0;
            src = &gEffect11DefaultSrcParams;
        }
        p.posX = 0.6f * (f32)randomGetRange(-10, 10);
        p.posY = 0.0f;
        p.posZ = 15.0f;
        p.velY = 0.1f * (f32)randomGetRange(1, 3);
        p.velX = 0.4f * src->posX;
        p.velZ = 0.4f * -src->posZ;
        p.scale = 0.0018f * (f32)randomGetRange(1, 3);
        p.count = 0x19;
        p.alpha = 0x55;
        p.flagsA = 0x80118;
        p.kind = 0x5f;
        break;
    case 0x130:
        if (src == NULL)
        {
            gEffect11DefaultSrcParams.posX = 0.0f;
            gEffect11DefaultSrcParams.posY = 0.0f;
            gEffect11DefaultSrcParams.posZ = 0.0f;
            gEffect11DefaultSrcParams.scale = 1.0f;
            gEffect11DefaultSrcParams.rotX = 0;
            gEffect11DefaultSrcParams.rotY = 0;
            gEffect11DefaultSrcParams.rotZ = 0;
            src = &gEffect11DefaultSrcParams;
        }
        p.posX = 0.6f * (f32)randomGetRange(-10, 10);
        p.posY = 0.0f;
        p.posZ = 15.0f;
        p.velY = 0.1f * (f32)randomGetRange(1, 3);
        p.velX = 0.2f * src->posX;
        p.velZ = 0.2f * -src->posZ;
        p.scale = 0.0022f * (f32)randomGetRange(1, 3);
        p.count = 0x19;
        p.alpha = 0x55;
        p.flagsA = 0x80118;
        p.kind = 0x5f;
        break;
    case 0x131:
        p.posX = 0.1f * (f32)randomGetRange(-0xc, 0xc);
        p.posY = 0.1f * (f32)randomGetRange(-0xc, 0xc) + 6.0f;
        p.posZ = 15.0f;
        p.velZ = 0.0045f * (f32)randomGetRange(5, 10);
        p.scale = 0.00165f;
        p.count = 100;
        p.alpha = 0xff;
        p.flagsA = 0x100;
        p.kind = 0x33;
        break;
    case 0x132:
        p.posX = 0.18f * (f32)randomGetRange(-10, 10);
        p.posY = 0.18f * (f32)randomGetRange(-10, 10);
        p.posZ = 0.18f * (f32)randomGetRange(-10, 10);
        p.scale = 0.02f;
        p.count = randomGetRange(0x78, 0x96);
        p.linkGroup = 0x1e;
        p.alpha = 0xff;
        p.flagsA = 0x11;
        p.kind = 0x5f;
        break;
    case 0x133:
        if (src == NULL)
        {
            gEffect11DefaultSrcParams.posX = 0.0f;
            gEffect11DefaultSrcParams.posY = 0.0f;
            gEffect11DefaultSrcParams.posZ = 0.0f;
            gEffect11DefaultSrcParams.scale = 1.0f;
            gEffect11DefaultSrcParams.rotX = 0;
            gEffect11DefaultSrcParams.rotY = 0;
            gEffect11DefaultSrcParams.rotZ = 0;
            src = &gEffect11DefaultSrcParams;
        }
        p.posX = src->posX;
        p.posY = src->posY;
        p.posZ = src->posZ;
        p.scale = 0.02f;
        p.count = 5;
        p.alpha = 0x80;
        p.flagsA |= 0x80210LL;
        p.kind = 0x26d;
        break;
    case 0x134:
        if (src == NULL)
        {
            gEffect11DefaultSrcParams.posX = 0.0f;
            gEffect11DefaultSrcParams.posY = 0.0f;
            gEffect11DefaultSrcParams.posZ = 0.0f;
            gEffect11DefaultSrcParams.scale = 1.0f;
            gEffect11DefaultSrcParams.rotX = 0;
            gEffect11DefaultSrcParams.rotY = 0;
            gEffect11DefaultSrcParams.rotZ = 0;
            src = &gEffect11DefaultSrcParams;
        }
        p.posX = 0.001f * (f32)randomGetRange(-200, 200) + src->posX;
        p.posY = src->posY;
        p.posZ = 0.001f * (f32)randomGetRange(-200, 200) + src->posZ;
        p.scale = 0.0001f * (f32)randomGetRange(5, 0xc);
        p.count = 0xc;
        p.alpha = randomGetRange(0x96, 0xfa);
        p.flagsA |= 0x80210LL;
        p.kind = 0xe0;
        break;
    case 0x135:
        if (src == NULL)
        {
            gEffect11DefaultSrcParams.posX = 0.0f;
            gEffect11DefaultSrcParams.posY = 0.0f;
            gEffect11DefaultSrcParams.posZ = 0.0f;
            gEffect11DefaultSrcParams.scale = 1.0f;
            gEffect11DefaultSrcParams.rotX = 0;
            gEffect11DefaultSrcParams.rotY = 0;
            gEffect11DefaultSrcParams.rotZ = 0;
            src = &gEffect11DefaultSrcParams;
        }
        p.posX = 0.18f * (f32)randomGetRange(-10, 10);
        p.posY = 0.18f * (f32)randomGetRange(-0x1e, 0);
        p.posZ = 0.18f * (f32)randomGetRange(-10, 10);
        p.velX = 0.02f * (f32)randomGetRange(-0xf, 0xf);
        p.velY = 0.0015f * (f32)randomGetRange(0xf, 0x23);
        p.velZ = 0.02f * (f32)randomGetRange(-0xf, 0xf);
        p.scale = 0.00012f * (f32)randomGetRange(0x64, 0x96);
        p.count = randomGetRange(0x32, 0x50);
        p.linkGroup = randomGetRange(0xa, 0x1e);
        p.flagsA = 0x218;
        p.kind = src->rotZ;
        break;
    case 0x136:
        if (src == NULL)
        {
            gEffect11DefaultSrcParams.posX = 0.0f;
            gEffect11DefaultSrcParams.posY = 0.0f;
            gEffect11DefaultSrcParams.posZ = 0.0f;
            gEffect11DefaultSrcParams.scale = 1.0f;
            gEffect11DefaultSrcParams.rotX = 0;
            gEffect11DefaultSrcParams.rotY = 0;
            gEffect11DefaultSrcParams.rotZ = 0;
            src = &gEffect11DefaultSrcParams;
        }
        p.posX = (f32)randomGetRange(-src->rotY, src->rotY) / 10.0f;
        p.posY = (f32)randomGetRange(-src->rotY, src->rotY) / 10.0f;
        p.posZ = (f32)randomGetRange(-src->rotY, src->rotY) / 10.0f;
        p.scale = 0.005f;
        p.count = randomGetRange(0x14, 0x1e);
        p.flagsA = 0x100200;
        p.kind = src->rotZ;
        break;
    case 0x137:
        if (src == NULL)
        {
            gEffect11DefaultSrcParams.posX = 0.0f;
            gEffect11DefaultSrcParams.posY = 0.0f;
            gEffect11DefaultSrcParams.posZ = 0.0f;
            gEffect11DefaultSrcParams.scale = 1.0f;
            gEffect11DefaultSrcParams.rotX = 0;
            gEffect11DefaultSrcParams.rotY = 0;
            gEffect11DefaultSrcParams.rotZ = 0;
            src = &gEffect11DefaultSrcParams;
        }
        if (src == NULL)
        {
            return -1;
        }
        p.velX = 0.003f * (f32)randomGetRange(0, 100) + 0.25f;
        p.velY = 0.002f * (f32)randomGetRange(0, 100) + 0.02f;
        p.velZ = 0.002f * (f32)randomGetRange(0, 100) + 0.02f;
        vecRotateZXY(&src->rotX, &p.velX);
        p.scale = 0.00004f * (f32)randomGetRange(0x14, 0x1e);
        p.alpha = 0xff;
        p.count = 0xf0;
        p.linkGroup = 0x10;
        p.unk04 = 0x138;
        p.flagsA = 0x480200;
        p.flagsB = 0x100000;
        p.kind = 0x167;
        break;
    case 0x138:
        p.scale = 0.0001f * (f32)randomGetRange(0x14, 0x1e);
        p.alpha = 0x37;
        p.count = 4;
        p.linkGroup = 0x10;
        p.flagsA = 0x80201;
        p.flagsB = 2;
        p.kind = 0x167;
        break;
    default:
        return -1;
    }
    p.flagsA = p.flagsA | flags;
    if (((p.flagsA & 1) != 0) && ((p.flagsA & 2) != 0))
    {
        p.flagsA ^= 2LL;
    }
    if ((p.flagsA & 1) != 0)
    {
        if (hasOffset != 0)
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

void Effect11_func05_nop(void)
{
}

void Effect11_func03_nop(void)
{
}

void Effect11_release(void)
{
}

void Effect11_initialise(void)
{
}
