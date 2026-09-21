/*
 * ARWArwingGu (DLL 669) - the Arwing's attached "gear" models in the
 * on-rails flight sections: the twin laser guns (def 0x610 / 0x615), the
 * bomb model (def 0x611) and the engine/escort model (def 0x606). One DLL
 * drives all of them, branching on the object's romDefNo. getExtraSize and
 * update therefore return / interpret a different state shape per romDefNo:
 *   0x606  engine - scrolls a texture animation (8-byte texture state)
 *   0x610/0x615 guns - count down a "just fired" visible timer, then hide
 *   0x611  bomb - fades alpha in or out toward a target (1-byte fadeIn flag)
 * arwarwinggu_setActiveVisible shows/hides a gun and selects its model
 * index; the arwarwing TU calls it when a shot is fired.
 */
#include "main/dll/ARW/dll_029D_arwarwinggu.h"
#include "main/frame_timing.h"
#include "main/rcp_dolphin.h"
#include "sys/objects.h"
#include "main/model.h"
#include "main/objtexture.h"

/* object def numbers (== romDefNo) of the Arwing's attached models */
enum
{
    ARWGU_DEF_ENGINE = 0x606, /* escort / engine, animated texture */
    ARWGU_DEF_GUN_L = 0x610,
    ARWGU_DEF_BOMB = 0x611,
    ARWGU_DEF_GUN_R = 0x615
};



void arwarwinggu_setActiveVisible(GameObject* obj, u8 active, u8 visible)
{
    ObjAnimComponent* objAnim = &(obj)->anim;
    ArwingGuGunState* state = (obj)->extra;

    if (active != 0)
    {
        Obj_SetActiveModelIndex(obj, visible != 0 ? 1 : 0);
        (obj)->anim.flags &= ~OBJANIM_FLAG_HIDDEN;
        objAnim->alpha = 0xff;
        state->visibleTimer = 5.0f;
    }
    else
    {
        (obj)->anim.flags |= OBJANIM_FLAG_HIDDEN;
        objAnim->alpha = 0;
    }
}

void arwarwinggu_setTextureFrame(GameObject* obj, int textureFrame)
{
    ArwingGuTextureState* state = obj->extra;
    state->textureFrame = textureFrame;
}

void arwarwinggu_applyTextureFrame(GameObject* obj)
{
    ObjModel* model;
    ObjTextureRuntimeSlot* texture;
    ArwingGuTextureState* state = (obj)->extra;
    Texture* anim;
    model = Obj_GetActiveModel(obj);
    texture = objFindTexture(obj, 0, 0);
    anim = ObjModel_GetTexture(model->file, 0);
    textureSetAnimationFrameStep(anim, (u16)state->textureFrame);
    textureUpdateAnimationFrame(anim, &state->textureAnimFlags, &texture->textureId);
}

int ARWArwingGu_getExtraSize(GameObject* obj)
{
    switch (obj->anim.romDefNo)
    {
    case ARWGU_DEF_ENGINE:
        return sizeof(ArwingGuTextureState);
    case ARWGU_DEF_GUN_L:
    case ARWGU_DEF_GUN_R:
        return sizeof(ArwingGuGunState);
    case ARWGU_DEF_BOMB:
        return sizeof(ArwingGuBombState);
    default:
        return 0;
    }
}

int ARWArwingGu_getObjectTypeId(void)
{
    return 0;
}

void ARWArwingGu_free(void)
{
}

void ARWArwingGu_render(void)
{
}

void ARWArwingGu_hitDetect(void)
{
}

static const f32 gArwingGuAlphaMax[1] = {255.0f};

void ARWArwingGu_update(GameObject* obj)
{
    ObjAnimComponent* objAnim = &(obj)->anim;

    switch ((obj)->anim.romDefNo)
    {
    case ARWGU_DEF_ENGINE:
    {
        ArwingGuTextureState* state = (obj)->extra;
        ObjModel* model = Obj_GetActiveModel(obj);
        ObjTextureRuntimeSlot* texture = objFindTexture(obj, 0, 0);
        Texture* anim = ObjModel_GetTexture(model->file, 0);
        textureSetAnimationFrameStep(anim, (u16)state->textureFrame);
        textureUpdateAnimationFrame(anim, &state->textureAnimFlags, &texture->textureId);
        break;
    }
    case ARWGU_DEF_GUN_L:
    case ARWGU_DEF_GUN_R:
    {
        ArwingGuGunState* state = (obj)->extra;
        f32 minTimer;
        f32 vt = state->visibleTimer;
        if (vt > (minTimer = 0.0f))
        {
            state->visibleTimer = vt - timeDelta;
            if (state->visibleTimer <= minTimer)
            {
                state->visibleTimer = minTimer;
                objAnim->alpha = 0;
            }
        }
        break;
    }
    case ARWGU_DEF_BOMB:
    {
        ArwingGuBombState* state = (obj)->extra;
        f32 alpha;
        if (state->fadeIn != 0)
        {
            alpha = gArwingGuAlphaMax[0] * timeDelta + (f32)(u32)objAnim->alpha;
        }
        else
        {
            alpha = (f32)(u32)objAnim->alpha - gArwingGuAlphaMax[0] * timeDelta;
        }
        if (alpha < 0.0f)
        {
            alpha = 0.0f;
        }
        else if (alpha > gArwingGuAlphaMax[0])
        {
            alpha = gArwingGuAlphaMax[0];
        }
        objAnim->alpha = alpha;
        break;
    }
    }
}

void ARWArwingGu_init(GameObject* obj)
{
    if (obj->anim.romDefNo == ARWGU_DEF_ENGINE)
    {
        return;
    }
    obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
    obj->anim.alpha = 0;
}

void ARWArwingGu_release(void)
{
}

void ARWArwingGu_initialise(void)
{
}

OBJECT_INIT_ADAPTER(gARWArwingGuObjDescriptorInitAdapter, ARWArwingGu_init, obj)
OBJECT_HIT_DETECT_ADAPTER(gARWArwingGuObjDescriptorHitDetectAdapter, ARWArwingGu_hitDetect)
OBJECT_RENDER_ADAPTER(gARWArwingGuObjDescriptorRenderAdapter, ARWArwingGu_render)
OBJECT_FREE_ADAPTER(gARWArwingGuObjDescriptorFreeAdapter, ARWArwingGu_free)
OBJECT_TYPE_ID_ADAPTER(gARWArwingGuObjDescriptorTypeIdAdapter, ARWArwingGu_getObjectTypeId)

RESOURCE_ACQUIRE_ADAPTER(gARWArwingGuObjDescriptorAcquire, ARWArwingGu_initialise)

ObjectDescriptor gARWArwingGuObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gARWArwingGuObjDescriptorAcquire,
        ARWArwingGu_release,
    },
    NULL,
    gARWArwingGuObjDescriptorInitAdapter,
    ARWArwingGu_update,
    gARWArwingGuObjDescriptorHitDetectAdapter,
    gARWArwingGuObjDescriptorRenderAdapter,
    gARWArwingGuObjDescriptorFreeAdapter,
    gARWArwingGuObjDescriptorTypeIdAdapter,
    ARWArwingGu_getExtraSize,
};
