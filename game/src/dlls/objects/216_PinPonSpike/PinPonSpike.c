/*
 * PinPonSpike object (DLL slot 216).
 *
 * Simulates a thrown spike until it hits a character or surface. An impact
 * hides the projectile, emits particles, and starts its despawn countdown.
 * The launch-angle helper is also used by the duster object family.
 */
#include "dlls/objects/216_PinPonSpike.h"
#include "dolphin/MSL_C/PPCEABI/bare/H/math_api.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/partfx_interface.h"
#include "main/dll_000A_expgfx.h"
#include "main/frame_timing.h"
#include "main/objhits.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

int pinponspike_calculateLaunchAngle(const f32* source, const f32* target, f32 speed, u8 useHighArc, f32 gravity) {
    f32 deltaX = source[0] - target[0];
    f32 deltaZ = source[2] - target[2];
    f32 horizontalDistance = sqrtf(deltaX * deltaX + deltaZ * deltaZ) * 1.05f;
    f32 deltaY = source[1] - target[1];
    f32 gravityQuarter = 0.25f * gravity;
    f32 coefficient = gravityQuarter * gravity;
    f32 speedSquared = speed * speed;
    f32 linearTerm = -(gravity * deltaY) - speedSquared;
    f32 discriminant = linearTerm * linearTerm -
                       (4.0f * coefficient) * (deltaY * deltaY + horizontalDistance * horizontalDistance);

    if (!(discriminant >= 0.0f)) {
        return 0x2000;
    }

    f32 root = sqrtf(discriminant);
    f32 flightTimeSquared;
    if (useHighArc != 0) {
        flightTimeSquared = (0.5f * (-linearTerm + root)) / coefficient;
    } else {
        flightTimeSquared = (0.5f * (-linearTerm - root)) / coefficient;
    }
    f32 flightTime = sqrtf(flightTimeSquared);
    f32 horizontalVelocity = horizontalDistance / flightTime;
    f32 verticalVelocity = sqrtf(-(horizontalVelocity * horizontalVelocity - speedSquared));
    return getAngle(verticalVelocity, horizontalVelocity);
}

static void pinponspike_impact(GameObject* obj, ObjHitsPriorityState* hitState) {
    obj->anim.alpha = 0;
    obj->userData1 = 120;
    hitState->flags &= ~OBJHITS_PRIORITY_STATE_ENABLED;
    for (int particleIndex = 0; particleIndex < 25; particleIndex++) {
        (*gPartfxInterface)->spawnObject(obj, 0x715, NULL, 1, -1, &particleIndex);
    }
    Sfx_PlayFromObject(obj, SFXTRIG_lummy311);
}

int pinponspike_getExtraSize(void) {
    return 0;
}

int pinponspike_getObjectTypeId(void) {
    return 0;
}

void pinponspike_free(GameObject* obj) {
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

void pinponspike_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible) {
    (void)obj;
    (void)fwdArg2;
    (void)fwdArg3;
    (void)fwdArg4;
    (void)fwdArg5;
    (void)visible;
}

void pinponspike_hitDetect(GameObject* obj) {
    (void)obj;
}

void pinponspike_update(GameObject* obj) {
    ObjHitsPriorityState* hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;

    if (obj->userData1 > 0) {
        obj->userData1 = (s32)((f32)obj->userData1 - timeDelta);
        if (obj->userData1 <= 0) {
            Obj_FreeObject(obj);
            return;
        }
    }
    if (obj->anim.alpha != 0) {
        f32 moveX = obj->anim.velocityX * timeDelta;
        f32 moveY = obj->anim.velocityY * timeDelta;
        f32 moveZ = obj->anim.velocityZ * timeDelta;
        objMove(obj, moveX, moveY, moveZ);
        obj->anim.velocityY += -0.2f * timeDelta;
        if (obj->anim.velocityY < -20.0f) {
            obj->anim.velocityY = -20.0f;
        }
        obj->anim.rotX = getAngle(moveX, moveZ) - 0x8000;
        obj->anim.rotY = 0x4000 - getAngle(sqrtf(moveX * moveX + moveZ * moveZ), moveY);
        ObjHits_SetHitVolumeSlot(&obj->anim, 10, 1, 0);
        ObjHits_EnableObject(obj);
        GameObject* lastHitObject = (GameObject*)hitState->lastHitObject;
        if ((lastHitObject != NULL &&
             (lastHitObject == Obj_GetPlayerObject() || lastHitObject == getTrickyObject())) ||
            hitState->contactFlags != 0) {
            pinponspike_impact(obj, hitState);
        } else if (obj->anim.localPosY < -2000.0f) {
            Obj_FreeObject(obj);
        }
    }
}

void pinponspike_init(GameObject* obj) {
    obj->userData1 = 0;
    ObjHits_DisableObject(obj);
    obj->anim.alpha = 0xff;
    Sfx_PlayFromObject(obj, SFXTRIG_whiz3_c);
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void pinponspike_release(void) {
}

void pinponspike_initialise(void) {
}

ObjectDescriptor gPinPonSpikeObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)pinponspike_initialise,
    (ObjectDescriptorCallback)pinponspike_release,
    0,
    (ObjectDescriptorCallback)pinponspike_init,
    (ObjectDescriptorCallback)pinponspike_update,
    (ObjectDescriptorCallback)pinponspike_hitDetect,
    (ObjectDescriptorCallback)pinponspike_render,
    (ObjectDescriptorCallback)pinponspike_free,
    (ObjectDescriptorCallback)pinponspike_getObjectTypeId,
    pinponspike_getExtraSize,
};
