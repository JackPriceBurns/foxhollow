#ifndef MAIN_DLL_ARW_DLL_02A6_ARWSQUADRON_H
#define MAIN_DLL_ARW_DLL_02A6_ARWSQUADRON_H

#include "global.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/dll/curve_walker.h"

typedef struct SquadFlags
{
    u8 attackWindowOpen : 1;
    u8 followingCurve : 1;
    u8 volleyInProgress : 1;
    u8 acceptsDamage : 1;
} SquadFlags;

typedef struct SquadCmdFlags
{
    u8 attackWindowOpen : 1;
    u8 followingCurve : 1;
    u8 volleyInProgress : 1;
    u8 acceptsDamage : 1;
    u8 rollCmdActive : 1;
    u8 : 3;
} SquadCmdFlags;

typedef struct ArwSquadronSetup
{
    ObjPlacement base;
    u8 rotXByte;
    u8 rotYByte;
    u8 rotZByte;
    u8 rotXSpeed;
    u8 rotYSpeed;
    u8 rotZSpeed;
    s8 leaderOffsetZ;
    u8 pad1F;
    int leaderObjectId;
    u16 exitDistance;
    s8 leaderOffsetX;
    s8 leaderOffsetY;
    u8 pad28[2];
    u16 volleyAngleSpread;
    u8 volleyCooldown;
    u8 shotInterval;
    u8 shotsPerVolley;
    u8 pathMode;
    u8 pathSpeed;
    u8 dialogueVariant;
    s16 gameBit;
} ArwSquadronSetup;

STATIC_ASSERT(offsetof(ArwSquadronSetup, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(ArwSquadronSetup, leaderObjectId) == 0x20);
STATIC_ASSERT(sizeof(ArwSquadronSetup) == 0x34);

typedef struct ArwSquadronPathCommand
{
    u8 pad00[0x18];
    u8 primaryCommand;
    s8 signature;
    s8 primaryValue;
    u8 pad1B[0x14];
    u8 secondaryCommand;
    u8 secondaryValue;
} ArwSquadronPathCommand;

typedef struct ArwSquadronState
{
    RomCurveWalker curve;
    f32 pathSpeed;
    f32 targetPathSpeed;
    f32 hitFlashTimer;
    f32 muzzleLightRadius;
    f32 muzzleLightIntensity;
    f32 damageSmokeScale;
    f32 fireFxScale;
    f32 volleyCooldownTimer;
    f32 shotIntervalTimer;
    f32 deathTimer;
    f32 activationDistance;
    f32 exitDistance;
    f32 rollAmplitude;
    GameObject* leaderObj;
    s16 rotXSpeed;
    s16 rotYSpeed;
    s16 rotZSpeed;
    u16 swayPhaseX;
    u16 swayPhaseY;
    u16 swaySpeedX;
    u16 swaySpeedY;
    s16 volleyAngle;
    s16 hitFadeRed;
    s16 hitFadeGreen;
    u8 hitFlashActive;
    s8 volleyShotsRemaining;
    u8 hitVolumeMode;
    u8 deathScore;
    u8 hitScore;
    u8 phase;
    u8 muzzleCount;
    u8 projectilePathCount;
    u8 variant;
    u8 dialogueVariant;
    u8 health;
    u8 fxFrameCounter;
    union
    {
        SquadFlags init;
        SquadCmdFlags cmd;
    } flags;
    u8 pad161[3];
} ArwSquadronState;

STATIC_ASSERT(sizeof(ArwSquadronState) == 0x164);
STATIC_ASSERT(offsetof(ArwSquadronState, curve) == 0x00);
STATIC_ASSERT(offsetof(ArwSquadronState, pathSpeed) == 0x108);
STATIC_ASSERT(offsetof(ArwSquadronState, volleyCooldownTimer) == 0x124);
STATIC_ASSERT(offsetof(ArwSquadronState, leaderObj) == 0x13c);
STATIC_ASSERT(offsetof(ArwSquadronState, flags) == 0x160);

int ARWSquadron_getExtraSize(void);
int ARWSquadron_getObjectTypeId(void);
void ARWSquadron_free(void);
void ARWSquadron_render(GameObject* obj, int p2, int p3, int p4, int p5);
void ARWSquadron_hitDetect(void);
void ARWSquadron_init(GameObject* obj, ArwSquadronSetup* setup);
void ARWSquadron_update(GameObject* obj);

void arwsquadron_spawnProjectile(GameObject* obj, int pathIdx, int angle, int flag);
void arwsquadron_applyCommandParams(GameObject* obj, ArwSquadronState* state);
void arwsquadron_followPath(GameObject* obj, ArwSquadronState* state);
void arwsquadron_updateVolley(GameObject* obj, ArwSquadronState* state, ArwSquadronSetup* setup);
void arwsquadron_emitEffects(GameObject* obj, ArwSquadronState* state);
void arwsquadron_handleDamage(GameObject* obj, ArwSquadronState* state);
void arwsquadron_followLeader(GameObject* obj, ArwSquadronState* state);

#endif
