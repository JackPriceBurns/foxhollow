#include "dlls/objects/429_SH_thorntai.h"

#include "dolphin/math.h"
#include "dolphin/os.h"
#include "dolphin/os/OSReport.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/dll/curves_collision_state.h"
#include "main/dll/dll_002E_moveLib.h"
#include "main/dll/dll_00C9_enemy.h"
#include "main/dll/partfx_interface.h"
#include "main/dll/path_control_interface.h"
#include "main/frame_timing.h"
#include "main/frustum.h"
#include "main/gamebits.h"
#include "main/mapEventTypes.h"
#include "main/model.h"
#include "main/newshadows_audio.h"
#include "main/obj_path.h"
#include "main/obj_trigger.h"
#include "main/objHitReact.h"
#include "main/object_render.h"
#include "main/objprint_character.h"
#include "main/objseq.h"
#include "main/objtype.h"
#include "main/sky_interface.h"
#include "main/vecmath.h"
#include "sys/objects.h"

enum SHthorntailBehaviorState {
    SH_THORNTAIL_STATE_IDLE,
    SH_THORNTAIL_STATE_IDLE_COUNTDOWN,
    SH_THORNTAIL_STATE_MOVE_2,
    SH_THORNTAIL_STATE_MOVE_3,
    SH_THORNTAIL_STATE_MOVE_4,
    SH_THORNTAIL_STATE_MOVE_5,
    SH_THORNTAIL_STATE_TURN_HOME,
    SH_THORNTAIL_STATE_CLOSE_ATTACK,
    SH_THORNTAIL_STATE_CLOSE_ATTACK_WAIT,
    SH_THORNTAIL_STATE_CLOSE_ATTACK_REPEAT,
    SH_THORNTAIL_STATE_CLOSE_ATTACK_RECOVER,
    SH_THORNTAIL_STATE_TAIL_SWING_READY,
    SH_THORNTAIL_STATE_TAIL_SWING,
    SH_THORNTAIL_STATE_TAIL_SWING_RECOVER,
    SH_THORNTAIL_STATE_EVENT_PAUSE,
    SH_THORNTAIL_STATE_ROOT_MODE2_EVENT,
    SH_THORNTAIL_STATE_ROOT_MODE3_WAIT,
};

enum SHthorntailBehaviorFlag {
    SH_THORNTAIL_MOVE_COMPLETE = 1 << 0,
    SH_THORNTAIL_IMPACT_PENDING = 1 << 1,
    SH_THORNTAIL_TRIGGER_EVENT_PENDING = 1 << 2,
    SH_THORNTAIL_LEVEL_CONTROL_READY = 1 << 3,
    SH_THORNTAIL_FREEZE_MOTION = 1 << 4,
};

enum SHthorntailStateConfigFlag {
    SH_THORNTAIL_STATE_STATUS_ACTIVE = 1 << 0,
    SH_THORNTAIL_STATE_HEAVY_HIT_REACTION = 1 << 1,
    SH_THORNTAIL_STATE_ENABLE_LOOK_TRACKING = 1 << 2,
    SH_THORNTAIL_STATE_APPLY_ROOT_MOTION = 1 << 3,
};

enum SHthorntailTailSwingState {
    SH_THORNTAIL_TAIL_SWING_READY,
    SH_THORNTAIL_TAIL_SWING_WINDUP,
    SH_THORNTAIL_TAIL_SWING_ACTIVE,
};

enum SHthorntailLocomotionMode {
    SH_THORNTAIL_LOCOMOTION_1 = 1,
    SH_THORNTAIL_LOCOMOTION_2,
    SH_THORNTAIL_LOCOMOTION_3,
    SH_THORNTAIL_LOCOMOTION_4,
    SH_THORNTAIL_LOCOMOTION_5,
    SH_THORNTAIL_LOCOMOTION_6,
    SH_THORNTAIL_LOCOMOTION_7,
    SH_THORNTAIL_LOCOMOTION_8,
};

enum SHthorntailControlMode {
    SH_THORNTAIL_CONTROL_LEVEL_0,
    SH_THORNTAIL_CONTROL_LEVEL_1,
    SH_THORNTAIL_CONTROL_ROOT_2,
    SH_THORNTAIL_CONTROL_ROOT_3,
};

enum SHthorntailLinkedObjectGroup {
    SH_THORNTAIL_LINKED_EVENT_OBJECT_GROUP = 3,
};

enum SHthorntailObjectId {
    SH_THORNTAIL_OBJECT_ID = 0x4D7,
};

enum SHthorntailSfxId {
    SH_THORNTAIL_TAIL_SWING_ACTIVE_SFX = 0xA8,
    SH_THORNTAIL_TAIL_SWING_WINDUP_SFX = 0xA9,
    SH_THORNTAIL_EVENT_RESUME_SFX = 0x409,
    SH_THORNTAIL_ALERT_SFX = 0x410,
};

enum SHthorntailMapEvent {
    SH_THORNTAIL_ROOT_MODE3_TRIGGER_EVENT = 0x1D,
    SH_THORNTAIL_ROOT_MODE2_TRIGGER_EVENT = 3,
};

enum SHthorntailPathControl {
    SH_THORNTAIL_PATH_MODE = 3,
    SH_THORNTAIL_PATH_FLAGS = 0xA3,
    SH_THORNTAIL_PATH_CHANNEL = 4,
};

enum SHthorntailPartFx {
    SH_THORNTAIL_TAIL_SWING_EFFECT = 0x7F0,
};

enum SHthorntailAudioChannel {
    SH_THORNTAIL_LEVEL_CONTROL_AUDIO_CHANNEL = 0x7F,
};

typedef struct SHthorntailPlacement {
    ObjPlacement base;
    u8 controlMode;
    u8 initialFacing;
    u8 impactSequenceVariant;
    u8 leashRadius;
    u16 scale;
} SHthorntailPlacement;

typedef struct SHthorntailImpactSequence {
    u8 count;
    u8 sequenceIds[15];
} SHthorntailImpactSequence;

typedef struct SHthorntailLinkedConfigRow {
    s32 configToken;
    s32 linkedConfigTokens[3];
} SHthorntailLinkedConfigRow;

typedef struct SHthorntailStateConfig {
    s16 moveId;
    f32 stepScale;
    u8 flags;
    u16 trigger0SfxId;
} SHthorntailStateConfig;

typedef struct SHthorntailState {
    MoveLibState moveLib;
    s8 behaviorState;
    u8 behaviorFlags;
    u8 locomotionMode;
    u8 tailSwingState;
    f32 tailSwingTimer;
    const SHthorntailImpactSequence* impactSequence;
    f32 idleTimer;
    f32 comboTimer;
    f32 effectTimer;
    s16 storedFacingAngle;
    s8 comboRepeatCount;
    u8 freezeFrameCounter;
    u8 hitReactState;
    CurvesCollisionState pathState;
    f32 hitReactionStepScale;
    CharacterEyeAnimState eyeAnimState;
    u8 unusedAfterEyes[8];
    Vec3f renderPathPoints[4];
    f32 proximityAlertTimer;
} SHthorntailState;

STATIC_ASSERT(offsetof(SHthorntailPlacement, controlMode) == sizeof(ObjPlacement));
STATIC_ASSERT(offsetof(SHthorntailPlacement, scale) == sizeof(ObjPlacement) + 4);
STATIC_ASSERT(offsetof(SHthorntailState, behaviorState) == sizeof(MoveLibState));
STATIC_ASSERT(offsetof(SHthorntailState, pathState) >=
              offsetof(SHthorntailState, hitReactState) + sizeof(((SHthorntailState*)0)->hitReactState));
STATIC_ASSERT(offsetof(SHthorntailState, hitReactionStepScale) ==
              offsetof(SHthorntailState, pathState) + sizeof(CurvesCollisionState));
STATIC_ASSERT(offsetof(SHthorntailState, eyeAnimState) ==
              offsetof(SHthorntailState, hitReactionStepScale) + sizeof(f32));
STATIC_ASSERT(offsetof(SHthorntailState, renderPathPoints) ==
              offsetof(SHthorntailState, eyeAnimState) + sizeof(CharacterEyeAnimState) + 8);
STATIC_ASSERT(offsetof(SHthorntailState, proximityAlertTimer) ==
              offsetof(SHthorntailState, renderPathPoints) + sizeof(((SHthorntailState*)0)->renderPathPoints));
STATIC_ASSERT(sizeof(SHthorntailState) == offsetof(SHthorntailState, proximityAlertTimer) + sizeof(f32));

static s32 sSHthorntailActiveConfigToken = -1;

static const SHthorntailLinkedConfigRow sSHthorntailLinkedConfigRows[] = {
    {0x44318, {0x4467F, 0x44677, 0x4467B}}, {0x442FB, {0x44641, 0x4463F, 0x44640}},
    {0x44309, {0x44646, 0x44648, 0x44649}}, {0x44302, {0x4432F, 0x4431C, 0x4432E}},
    {0x442F4, {0x4463D, 0x4463C, 0x4463E}}, {0x44310, {0x44636, 0x44634, 0x44637}},
};

static Vec3f sSHthorntailPathPoints[] = {
    {-8.0f, 0.0f, -8.0f},
    {8.0f, 0.0f, -8.0f},
    {8.0f, 0.0f, 8.0f},
    {-8.0f, 0.0f, 8.0f},
};

static f32 sSHthorntailPathRadii[] = {0.0f, 0.0f, 0.0f, 0.0f};

static ObjHitReactEntry sSHthorntailNormalHitReactEntries[] = {
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 8,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
};

static ObjHitReactEntry sSHthorntailHeavyHitReactEntries[] = {
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
    {.primaryHitSfxId = 0x23F,
     .secondaryHitSfxId = 0x2C2,
     .reactionMoveId = 9,
     .unk06 = -1,
     .reactionStepScale = 0.01f},
};

static const SHthorntailStateConfig sSHthorntailStateConfigs[] = {
    [SH_THORNTAIL_STATE_IDLE] = {0, 0.006f, SH_THORNTAIL_STATE_ENABLE_LOOK_TRACKING, 0},
    [SH_THORNTAIL_STATE_IDLE_COUNTDOWN] = {0, 0.006f, 0, 0},
    [SH_THORNTAIL_STATE_MOVE_2] = {14, 0.012f, SH_THORNTAIL_STATE_APPLY_ROOT_MOTION, 0},
    [SH_THORNTAIL_STATE_MOVE_3] = {13, 0.006f, SH_THORNTAIL_STATE_APPLY_ROOT_MOTION, 0},
    [SH_THORNTAIL_STATE_MOVE_4] = {11, 0.006f, SH_THORNTAIL_STATE_APPLY_ROOT_MOTION, 0},
    [SH_THORNTAIL_STATE_MOVE_5] = {12, 0.006f, SH_THORNTAIL_STATE_APPLY_ROOT_MOTION, 0},
    [SH_THORNTAIL_STATE_TURN_HOME] = {15, 0.01f, SH_THORNTAIL_STATE_APPLY_ROOT_MOTION, 0},
    [SH_THORNTAIL_STATE_CLOSE_ATTACK] = {4, 0.004f, 0, 0},
    [SH_THORNTAIL_STATE_CLOSE_ATTACK_WAIT] = {5, 0.006f, 0, 0},
    [SH_THORNTAIL_STATE_CLOSE_ATTACK_REPEAT] = {6, 0.003f, 0, 0x2B0},
    [SH_THORNTAIL_STATE_CLOSE_ATTACK_RECOVER] = {7, 0.006f, 0, 0x2B0},
    [SH_THORNTAIL_STATE_TAIL_SWING_READY] = {1, 0.006f, SH_THORNTAIL_STATE_STATUS_ACTIVE, 0x2B0},
    [SH_THORNTAIL_STATE_TAIL_SWING] = {2, 0.0025f,
                                       SH_THORNTAIL_STATE_STATUS_ACTIVE | SH_THORNTAIL_STATE_HEAVY_HIT_REACTION, 0x2B1},
    [SH_THORNTAIL_STATE_TAIL_SWING_RECOVER] = {3, 0.006f, SH_THORNTAIL_STATE_STATUS_ACTIVE, 0},
    [SH_THORNTAIL_STATE_EVENT_PAUSE] = {10, 0.006f, SH_THORNTAIL_STATE_STATUS_ACTIVE, 0},
    [SH_THORNTAIL_STATE_ROOT_MODE2_EVENT] = {0, 0.006f, 0, 0},
    [SH_THORNTAIL_STATE_ROOT_MODE3_WAIT] = {16, 0.01f, 0, 0},
};

static const SHthorntailImpactSequence sSHthorntailLevelMode0DefaultSequence = {
    15, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xE}};
static const SHthorntailImpactSequence sSHthorntailLevelMode1Sequence = {1, {0x10}};
static const SHthorntailImpactSequence sSHthorntailRootMode2DefaultSequence = {1, {0x14}};
static const SHthorntailImpactSequence sSHthorntailLevelMode0Locomotion6Sequence = {3, {0x2D, 0x2E, 0x2F}};
static const SHthorntailImpactSequence sSHthorntailRootMode3DefaultSequence = {4, {0x33, 0x34, 0x35, 0x36}};
static const SHthorntailImpactSequence sSHthorntailRootMode3Locomotion1Sequence = {1, {0x37}};
static const SHthorntailImpactSequence sSHthorntailRootMode3Locomotion2Sequence = {1, {0x38}};
static const SHthorntailImpactSequence sSHthorntailRootMode3Locomotion3Sequence = {1, {0x39}};
static const SHthorntailImpactSequence sSHthorntailRootMode3Locomotion4Sequence = {1, {0x3A}};
static const SHthorntailImpactSequence sSHthorntailRootMode3Locomotion5IdleSequence = {1, {0x3B}};
static const SHthorntailImpactSequence sSHthorntailRootMode3Locomotion5PlayerSequence = {1, {0x3C}};
static const SHthorntailImpactSequence sSHthorntailRootMode3Locomotion5EventSequence = {1, {0x3D}};
static const SHthorntailImpactSequence sSHthorntailRootMode3Locomotion6Sequence = {1, {0x3E}};
static const SHthorntailImpactSequence sSHthorntailRootMode3Locomotion7Sequence = {1, {0x3F}};
static const SHthorntailImpactSequence sSHthorntailRootMode3Locomotion8Sequence = {1, {0x40}};

static const SHthorntailImpactSequence sSHthorntailLevelMode0Locomotion1Sequences[] = {
    {1, {0x0F}}, {1, {0x10}}, {1, {0x11}}, {1, {0x12}}, {1, {0x13}}, {0, {0}},
};
static const SHthorntailImpactSequence sSHthorntailLevelMode0Locomotion2ClearSequences[] = {
    {1, {0x16}}, {1, {0x18}}, {1, {0x1A}}, {1, {0x1C}}, {1, {0x1E}}, {0, {0}},
};
static const SHthorntailImpactSequence sSHthorntailLevelMode0Locomotion2SetSequences[] = {
    {1, {0x17}}, {1, {0x19}}, {1, {0x1B}}, {1, {0x1D}}, {1, {0x1F}}, {0, {0}},
};
static const SHthorntailImpactSequence sSHthorntailLevelMode0Locomotion3ClearSequences[] = {
    {1, {0x20}}, {1, {0x22}}, {1, {0x24}}, {1, {0x26}}, {1, {0x28}}, {0, {0}},
};
static const SHthorntailImpactSequence sSHthorntailLevelMode0Locomotion3SetSequences[] = {
    {1, {0x21}}, {1, {0x23}}, {1, {0x25}}, {1, {0x27}}, {1, {0x29}}, {0, {0}},
};
static const SHthorntailImpactSequence sSHthorntailLevelMode0Locomotion5ClearSequences[] = {
    {1, {0x2B}}, {1, {0x2C}}, {1, {0x2C}}, {1, {0x2A}}, {1, {0x2B}}, {0, {0}},
};
static const SHthorntailImpactSequence sSHthorntailLevelMode0Locomotion8Sequences[] = {
    {1, {0x31}}, {1, {0x30}}, {1, {0x30}}, {1, {0x32}}, {1, {0x31}}, {0, {0}},
};

static const char sSHthorntailAngleYawDebug[] = "angle %d, obj-yaw %d\n";
static const char sSHthorntailSourceFile[] = "SHthorntail.c";
static const char sSHthorntailInvalidState[] = "Thorntail entered an invalid state\n";

static const SHthorntailPlacement* shThorntail_getPlacement(const GameObject* obj) {
    return (const SHthorntailPlacement*)obj->anim.placementData;
}

static s16 shThorntail_getLinkedGameBit(const GameObject* obj) {
    const SHthorntailPlacement* placement = shThorntail_getPlacement(obj);

    return ObjAnim_ReadPlacementS16(&obj->anim, &placement->controlMode);
}

static f32 shThorntail_xzDistanceSquared(const Vec3f* a, const Vec3f* b) {
    f32 dx = a->x - b->x;
    f32 dz = a->z - b->z;

    return dx * dx + dz * dz;
}

static f32 shThorntail_homeDistanceSquared(const GameObject* obj, const SHthorntailPlacement* placement) {
    f32 dx = obj->anim.worldPosX - placement->base.posX;
    f32 dz = obj->anim.worldPosZ - placement->base.posZ;

    return dx * dx + dz * dz;
}

static int shThorntail_hasNearbyPendingEventObject(GameObject* obj) {
    const SHthorntailPlacement* placement = shThorntail_getPlacement(obj);
    GameObject** objects;
    int count;
    int groupIndex = -1;
    int linkedEventPending = 0;
    s8 matchCount = 0;

    for (int i = 0; i < ARRAY_COUNT(sSHthorntailLinkedConfigRows); i++) {
        if (placement->base.ident == sSHthorntailLinkedConfigRows[i].configToken) {
            groupIndex = i;
            break;
        }
    }

    objects = (GameObject**)objGetAllOfType(SH_THORNTAIL_LINKED_EVENT_OBJECT_GROUP, &count);
    for (int i = 0; i < count; i++) {
        const SHthorntailPlacement* linkedPlacement = shThorntail_getPlacement(objects[i]);

        if (objects[i]->anim.romDefNo == SH_THORNTAIL_OBJECT_ID) {
            const SHthorntailLinkedConfigRow* linkedConfigRow = &sSHthorntailLinkedConfigRows[groupIndex];

            if (linkedPlacement->base.ident != linkedConfigRow->linkedConfigTokens[0] &&
                linkedPlacement->base.ident != linkedConfigRow->linkedConfigTokens[1] &&
                linkedPlacement->base.ident != linkedConfigRow->linkedConfigTokens[2]) {
                continue;
            }
            enemy_setTrackedObj(objects[i], obj);
            if (shThorntail_xzDistanceSquared((Vec3f*)&objects[i]->anim.worldPosX, (Vec3f*)&obj->anim.worldPosX) < 40000.0f &&
                mainGetBit(shThorntail_getLinkedGameBit(objects[i])) == 0) {
                linkedEventPending = 1;
            }
            matchCount++;
            if (matchCount == ARRAY_COUNT(linkedConfigRow->linkedConfigTokens)) {
                break;
            }
        }
    }

    return linkedEventPending;
}

static void shThorntail_updateTailSwing(GameObject* obj, SHthorntailState* state) {
    switch (state->tailSwingState) {
    case SH_THORNTAIL_TAIL_SWING_READY:
        state->tailSwingTimer -= timeDelta;
        if (state->tailSwingTimer <= 0.0f) {
            Sfx_PlayFromObject(obj, SH_THORNTAIL_TAIL_SWING_WINDUP_SFX);
            state->tailSwingState = SH_THORNTAIL_TAIL_SWING_WINDUP;
            state->tailSwingTimer = 180.0f;
        }
        break;
    case SH_THORNTAIL_TAIL_SWING_WINDUP:
        state->tailSwingTimer -= timeDelta;
        if (state->tailSwingTimer <= 0.0f) {
            Sfx_PlayFromObject(obj, SH_THORNTAIL_TAIL_SWING_ACTIVE_SFX);
            state->tailSwingState = SH_THORNTAIL_TAIL_SWING_ACTIVE;
        }
        break;
    case SH_THORNTAIL_TAIL_SWING_ACTIVE:
        if (state->behaviorFlags & SH_THORNTAIL_MOVE_COMPLETE) {
            state->tailSwingState = SH_THORNTAIL_TAIL_SWING_READY;
            state->tailSwingTimer = 45.0f;
        }
        break;
    }
}

static u32 shThorntail_chooseNextState(GameObject* obj, SHthorntailState* state,
                                       const SHthorntailPlacement* placement) {
    if (placement->leashRadius != 0) {
        GameObject* player = Obj_GetPlayerObject();

        if (shThorntail_xzDistanceSquared((Vec3f*)&obj->anim.worldPosX, (Vec3f*)&player->anim.worldPosX) < 10000.0f) {
            if (state->behaviorState >= SH_THORNTAIL_STATE_MOVE_2 &&
                state->behaviorState <= SH_THORNTAIL_STATE_MOVE_5) {
                return SH_THORNTAIL_STATE_TURN_HOME;
            }
            return SH_THORNTAIL_STATE_CLOSE_ATTACK;
        }

        if (shThorntail_homeDistanceSquared(obj, placement) >
            (f32)((s32)placement->leashRadius * placement->leashRadius)) {
            s16 homeAngle =
                (s16)getAngle(obj->anim.localPosX - placement->base.posX, obj->anim.localPosZ - placement->base.posZ);
            s16 angleDelta = homeAngle - (u16)obj->anim.rotX;
            int absoluteAngleDelta = angleDelta >= 0 ? angleDelta : -angleDelta;

            if (absoluteAngleDelta > 0x20) {
                OSReport(sSHthorntailAngleYawDebug,
                         (u16)getAngle(obj->anim.localPosX - placement->base.posX,
                                       obj->anim.localPosZ - placement->base.posZ),
                         obj->anim.rotX);
                if (state->behaviorState >= SH_THORNTAIL_STATE_MOVE_2 &&
                    state->behaviorState <= SH_THORNTAIL_STATE_MOVE_5) {
                    return SH_THORNTAIL_STATE_TURN_HOME;
                }
                return SH_THORNTAIL_STATE_CLOSE_ATTACK;
            }
        }
    } else {
        return SH_THORNTAIL_STATE_CLOSE_ATTACK;
    }

    if (ViewFrustum_IsSphereVisible(&obj->anim.localPosX, obj->anim.hitboxScale * obj->anim.rootMotionScale) == 0) {
        return SH_THORNTAIL_STATE_CLOSE_ATTACK;
    }
    if (state->behaviorState >= SH_THORNTAIL_STATE_MOVE_2 && state->behaviorState <= SH_THORNTAIL_STATE_MOVE_5) {
        return randomGetRange(SH_THORNTAIL_STATE_MOVE_3, SH_THORNTAIL_STATE_MOVE_5) & 0xFF;
    }
    return SH_THORNTAIL_STATE_MOVE_2;
}

static void shThorntail_updateState(GameObject* obj, SHthorntailState* state) {
    switch (state->behaviorState) {
    case SH_THORNTAIL_STATE_IDLE:
        if (RandomTimer_UpdateRangeTrigger(&state->proximityAlertTimer, 2.0f, 5.0f) != 0) {
            Sfx_PlayFromObject(obj, SH_THORNTAIL_ALERT_SFX);
        }
        state->idleTimer -= timeDelta;
        if (state->idleTimer <= 120.0f) {
            state->behaviorState = SH_THORNTAIL_STATE_IDLE_COUNTDOWN;
        }
        break;
    case SH_THORNTAIL_STATE_IDLE_COUNTDOWN:
        state->idleTimer -= timeDelta;
        if (state->idleTimer <= 0.0f) {
            if ((*gSkyInterface)->getSunPosition(0) != 0) {
                state->behaviorState = SH_THORNTAIL_STATE_TAIL_SWING_READY;
            } else {
                state->behaviorState = shThorntail_chooseNextState(obj, state, shThorntail_getPlacement(obj));
            }
        }
        break;
    case SH_THORNTAIL_STATE_MOVE_2:
    case SH_THORNTAIL_STATE_MOVE_3:
    case SH_THORNTAIL_STATE_MOVE_4:
    case SH_THORNTAIL_STATE_MOVE_5:
    case SH_THORNTAIL_STATE_TURN_HOME:
        if (state->behaviorFlags & SH_THORNTAIL_MOVE_COMPLETE) {
            if ((*gSkyInterface)->getSunPosition(0) != 0) {
                state->behaviorState = SH_THORNTAIL_STATE_TAIL_SWING_READY;
            } else {
                state->behaviorState = shThorntail_chooseNextState(obj, state, shThorntail_getPlacement(obj));
            }
        }
        break;
    case SH_THORNTAIL_STATE_CLOSE_ATTACK:
        if (state->behaviorFlags & SH_THORNTAIL_MOVE_COMPLETE) {
            state->behaviorState = SH_THORNTAIL_STATE_CLOSE_ATTACK_WAIT;
            state->comboTimer = (f32)randomGetRange(500, 800);
            state->comboRepeatCount = randomGetRange(1, 3);
        }
        break;
    case SH_THORNTAIL_STATE_CLOSE_ATTACK_WAIT:
        state->comboTimer -= (f32)framesThisStep;
        if (state->comboTimer <= 0.0f) {
            state->behaviorState = state->comboRepeatCount <= 0 ? SH_THORNTAIL_STATE_CLOSE_ATTACK_RECOVER
                                                                : SH_THORNTAIL_STATE_CLOSE_ATTACK_REPEAT;
        }
        break;
    case SH_THORNTAIL_STATE_CLOSE_ATTACK_REPEAT:
        if (state->behaviorFlags & SH_THORNTAIL_MOVE_COMPLETE) {
            state->behaviorState = SH_THORNTAIL_STATE_CLOSE_ATTACK_WAIT;
            state->comboTimer = (f32)randomGetRange(500, 800);
            state->comboRepeatCount--;
        }
        break;
    case SH_THORNTAIL_STATE_CLOSE_ATTACK_RECOVER:
        if (state->behaviorFlags & SH_THORNTAIL_MOVE_COMPLETE) {
            state->behaviorState = SH_THORNTAIL_STATE_IDLE;
            state->idleTimer = (f32)randomGetRange(1000, 2000);
        }
        break;
    case SH_THORNTAIL_STATE_TAIL_SWING_READY:
        if (state->behaviorFlags & SH_THORNTAIL_MOVE_COMPLETE) {
            state->tailSwingState = SH_THORNTAIL_TAIL_SWING_ACTIVE;
            state->behaviorState = SH_THORNTAIL_STATE_TAIL_SWING;
        }
        break;
    case SH_THORNTAIL_STATE_TAIL_SWING:
        shThorntail_updateTailSwing(obj, state);
        if ((state->behaviorFlags & SH_THORNTAIL_MOVE_COMPLETE) && (*gSkyInterface)->getSunPosition(0) == 0) {
            state->behaviorState = SH_THORNTAIL_STATE_TAIL_SWING_RECOVER;
        }
        break;
    case SH_THORNTAIL_STATE_TAIL_SWING_RECOVER:
        if (state->behaviorFlags & SH_THORNTAIL_MOVE_COMPLETE) {
            state->behaviorState = SH_THORNTAIL_STATE_IDLE;
            state->idleTimer = (f32)randomGetRange(1000, 2000);
        }
        break;
    default:
        OSPanic(sSHthorntailSourceFile, 0x6CD, sSHthorntailInvalidState);
    }
}

static void shThorntail_updateRootControlMode3(GameObject* obj, SHthorntailState* state) {
    state->impactSequence = &sSHthorntailRootMode3DefaultSequence;
    switch (state->locomotionMode) {
    case SH_THORNTAIL_LOCOMOTION_1:
        state->impactSequence = &sSHthorntailRootMode3Locomotion1Sequence;
        break;
    case SH_THORNTAIL_LOCOMOTION_2:
        if (mainGetBit(GAMEBIT_ITEM_WhiteGrubTub_Used) != 6) {
            state->impactSequence = &sSHthorntailRootMode3Locomotion2Sequence;
        }
        break;
    case SH_THORNTAIL_LOCOMOTION_3:
        if (mainGetBit(GAMEBIT_ITEM_MoonPassKey_Got) == 0) {
            state->impactSequence = &sSHthorntailRootMode3Locomotion3Sequence;
        }
        break;
    case SH_THORNTAIL_LOCOMOTION_4:
        state->impactSequence = &sSHthorntailRootMode3Locomotion4Sequence;
        break;
    case SH_THORNTAIL_LOCOMOTION_5:
        if (mainGetBit(GAMEBIT_SH_Related023C) == 0) {
            if (mainGetBit(GAMEBIT_ITEM_OpenPortal_Got) != 0) {
                (*gMapEventInterface)->setMapAct(SH_THORNTAIL_ROOT_MODE3_TRIGGER_EVENT, 3);
                state->impactSequence = &sSHthorntailRootMode3Locomotion5EventSequence;
            } else if (mainGetBit(GAMEBIT_SH_ThornTailRelated023D) != 0) {
                if (state->behaviorState == SH_THORNTAIL_STATE_ROOT_MODE3_WAIT) {
                    state->behaviorState = SH_THORNTAIL_STATE_IDLE;
                    state->idleTimer = (f32)randomGetRange(1000, 2000);
                }
                state->impactSequence = &sSHthorntailRootMode3Locomotion5PlayerSequence;
            } else {
                state->impactSequence = &sSHthorntailRootMode3Locomotion5IdleSequence;
                state->behaviorState = SH_THORNTAIL_STATE_ROOT_MODE3_WAIT;
                return;
            }
        }
        break;
    case SH_THORNTAIL_LOCOMOTION_6:
        if (mainGetBit(GAMEBIT_ITEM_BigScarabBag_Got) == 0) {
            state->impactSequence = &sSHthorntailRootMode3Locomotion6Sequence;
        }
        break;
    case SH_THORNTAIL_LOCOMOTION_7:
        if (mainGetBit(GAMEBIT_SH_ThornTailRelated0199) == 0) {
            state->impactSequence = &sSHthorntailRootMode3Locomotion7Sequence;
        }
        break;
    case SH_THORNTAIL_LOCOMOTION_8:
        state->impactSequence = &sSHthorntailRootMode3Locomotion8Sequence;
        break;
    }
    shThorntail_updateState(obj, state);
}

static void shThorntail_updateRootControlMode2(GameObject* obj, SHthorntailState* state) {
    state->impactSequence = &sSHthorntailLevelMode0DefaultSequence;
    switch (state->locomotionMode) {
    case SH_THORNTAIL_LOCOMOTION_1:
    case SH_THORNTAIL_LOCOMOTION_2:
    case SH_THORNTAIL_LOCOMOTION_3:
    case SH_THORNTAIL_LOCOMOTION_4:
    case SH_THORNTAIL_LOCOMOTION_5:
        state->impactSequence = &sSHthorntailRootMode2DefaultSequence;
        break;
    case SH_THORNTAIL_LOCOMOTION_6:
        if (shThorntail_hasNearbyPendingEventObject(obj) != 0) {
            state->behaviorState = SH_THORNTAIL_STATE_EVENT_PAUSE;
            return;
        }
        if (state->behaviorState == SH_THORNTAIL_STATE_EVENT_PAUSE) {
            Sfx_PlayFromObject(NULL, SH_THORNTAIL_EVENT_RESUME_SFX);
            state->behaviorState = SH_THORNTAIL_STATE_IDLE;
            state->idleTimer = (f32)randomGetRange(1000, 2000);
        }
        state->impactSequence = &sSHthorntailRootMode2DefaultSequence;
        break;
    case SH_THORNTAIL_LOCOMOTION_7:
        if (state->behaviorState == SH_THORNTAIL_STATE_ROOT_MODE2_EVENT) {
            u32 triggerEventId = mainGetBit(GAMEBIT_SH_ThornTailRelated01A0);

            if (mainGetBit(triggerEventId) != 0) {
                (*gMapEventInterface)
                    ->setObjGroupStatus((int)obj->anim.mapEventSlot, SH_THORNTAIL_ROOT_MODE2_TRIGGER_EVENT, 0);
                state->behaviorState = SH_THORNTAIL_STATE_IDLE;
                state->idleTimer = (f32)randomGetRange(1000, 2000);
            } else {
                return;
            }
        } else if (mainGetBit(GAMEBIT_SH_ThornTailRelated01A0) == 0 && ObjTrigger_IsSet(obj) != 0) {
            state->behaviorFlags |= SH_THORNTAIL_TRIGGER_EVENT_PENDING;
            state->behaviorState = SH_THORNTAIL_STATE_ROOT_MODE2_EVENT;
            (*gMapEventInterface)
                ->setObjGroupStatus((int)obj->anim.mapEventSlot, SH_THORNTAIL_ROOT_MODE2_TRIGGER_EVENT, 1);
            mainSetBits(GAMEBIT_SH_ThornTailRelated0199, 1);
            return;
        }
        break;
    case SH_THORNTAIL_LOCOMOTION_8:
        state->impactSequence = &sSHthorntailLevelMode0Locomotion8Sequences[3];
        break;
    }
    shThorntail_updateState(obj, state);
}

static void shThorntail_updateLevelControlMode1(GameObject* obj, SHthorntailState* state,
                                                const SHthorntailPlacement* placement) {
    GameObject* player = Obj_GetPlayerObject();
    u8 closeToPlayer = shThorntail_xzDistanceSquared((Vec3f*)&obj->anim.worldPosX, (Vec3f*)&player->anim.worldPosX) < 10000.0f;

    state->impactSequence = &sSHthorntailLevelMode1Sequence;
    if (placement->impactSequenceVariant == 0) {
        if (mainGetBit(GAMEBIT_ITEM_FireflyLantern_Got) != 0) {
            if (mainGetBit(GAMEBIT_SH_ThornTailRelated0168) != 0) {
                state->behaviorFlags |= SH_THORNTAIL_FREEZE_MOTION;
                state->freezeFrameCounter = 0;
                closeToPlayer = FALSE;
            } else if (ObjTrigger_IsSet(obj) != 0) {
                state->behaviorFlags |= SH_THORNTAIL_TRIGGER_EVENT_PENDING;
                mainSetBits(GAMEBIT_SH_ThornTailRelated0CD6, 1);
            }
        } else if (ObjTrigger_IsSet(obj) != 0) {
            state->behaviorFlags |= SH_THORNTAIL_TRIGGER_EVENT_PENDING;
            mainSetBits(GAMEBIT_SH_ThornTailRelated0CD5, 1);
        }
    } else if (mainGetBit(GAMEBIT_SH_MetQueen) != 0) {
        closeToPlayer = FALSE;
    }

    switch (state->behaviorState) {
    case SH_THORNTAIL_STATE_IDLE:
        if (!closeToPlayer) {
            state->idleTimer = 120.0f;
            state->behaviorState = SH_THORNTAIL_STATE_IDLE_COUNTDOWN;
        }
        break;
    case SH_THORNTAIL_STATE_IDLE_COUNTDOWN:
        if (closeToPlayer) {
            state->behaviorState = SH_THORNTAIL_STATE_IDLE;
        } else {
            state->idleTimer -= timeDelta;
            if (state->idleTimer <= 0.0f) {
                state->behaviorState = SH_THORNTAIL_STATE_TAIL_SWING_READY;
            }
        }
        break;
    case SH_THORNTAIL_STATE_TAIL_SWING_READY:
        if (state->behaviorFlags & SH_THORNTAIL_MOVE_COMPLETE) {
            if (closeToPlayer) {
                state->behaviorState = SH_THORNTAIL_STATE_TAIL_SWING_RECOVER;
            } else {
                state->tailSwingState = SH_THORNTAIL_TAIL_SWING_ACTIVE;
                state->behaviorState = SH_THORNTAIL_STATE_TAIL_SWING;
            }
        }
        break;
    case SH_THORNTAIL_STATE_TAIL_SWING:
        if (closeToPlayer) {
            state->behaviorState = SH_THORNTAIL_STATE_TAIL_SWING_RECOVER;
        } else {
            shThorntail_updateTailSwing(obj, state);
        }
        break;
    case SH_THORNTAIL_STATE_TAIL_SWING_RECOVER:
        if (state->behaviorFlags & SH_THORNTAIL_MOVE_COMPLETE) {
            state->behaviorState = SH_THORNTAIL_STATE_IDLE;
            state->idleTimer = (f32)randomGetRange(1000, 2000);
        }
        break;
    }
}

static void shThorntail_updateLevelControlMode0(GameObject* obj, SHthorntailState* state,
                                                const SHthorntailPlacement* placement) {
    state->impactSequence = &sSHthorntailLevelMode0DefaultSequence;
    switch (state->locomotionMode) {
    case SH_THORNTAIL_LOCOMOTION_1:
        state->impactSequence = &sSHthorntailLevelMode0Locomotion1Sequences[placement->impactSequenceVariant];
        break;
    case SH_THORNTAIL_LOCOMOTION_2:
        if (mainGetBit(GAMEBIT_SH_QueenFed) != 0) {
            state->impactSequence = &sSHthorntailLevelMode0Locomotion2SetSequences[placement->impactSequenceVariant];
        } else {
            state->impactSequence = &sSHthorntailLevelMode0Locomotion2ClearSequences[placement->impactSequenceVariant];
        }
        break;
    case SH_THORNTAIL_LOCOMOTION_3:
        if (mainGetBit(GAMEBIT_ITEM_MoonPassKey_Got) != 0) {
            state->impactSequence = &sSHthorntailLevelMode0Locomotion3SetSequences[placement->impactSequenceVariant];
        } else {
            state->impactSequence = &sSHthorntailLevelMode0Locomotion3ClearSequences[placement->impactSequenceVariant];
        }
        break;
    case SH_THORNTAIL_LOCOMOTION_5:
        if (mainGetBit(GAMEBIT_SH_ThornTailRelated023D) == 0) {
            state->impactSequence = &sSHthorntailLevelMode0Locomotion5ClearSequences[placement->impactSequenceVariant];
        }
        break;
    case SH_THORNTAIL_LOCOMOTION_6:
        if (shThorntail_hasNearbyPendingEventObject(obj) != 0) {
            state->behaviorState = SH_THORNTAIL_STATE_EVENT_PAUSE;
            return;
        }
        if (state->behaviorState == SH_THORNTAIL_STATE_EVENT_PAUSE) {
            Sfx_PlayFromObject(NULL, SH_THORNTAIL_EVENT_RESUME_SFX);
            state->behaviorState = SH_THORNTAIL_STATE_IDLE;
            state->idleTimer = (f32)randomGetRange(1000, 2000);
        }
        if (mainGetBit(GAMEBIT_ITEM_BigScarabBag_Got) == 0) {
            state->impactSequence = &sSHthorntailLevelMode0Locomotion6Sequence;
        }
        break;
    case SH_THORNTAIL_LOCOMOTION_8:
        state->impactSequence = &sSHthorntailLevelMode0Locomotion8Sequences[placement->impactSequenceVariant];
        break;
    }
    shThorntail_updateState(obj, state);
}

static int shThorntail_updateLevelControlState(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    SHthorntailState* state = obj->extra;

    (void)unused;

    if (!(state->behaviorFlags & SH_THORNTAIL_LEVEL_CONTROL_READY)) {
        Sfx_StopObjectChannel(obj, SH_THORNTAIL_LEVEL_CONTROL_AUDIO_CHANNEL);
        state->behaviorState = SH_THORNTAIL_STATE_IDLE;
        state->idleTimer = (f32)randomGetRange(1000, 2000);
        state->behaviorFlags &= ~SH_THORNTAIL_TRIGGER_EVENT_PENDING;
        state->behaviorFlags |= SH_THORNTAIL_LEVEL_CONTROL_READY | SH_THORNTAIL_FREEZE_MOTION;
        state->freezeFrameCounter = 0;
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    }
    if (state->behaviorFlags & SH_THORNTAIL_IMPACT_PENDING) {
        if (dll_2E_updateSequenceTurn(obj, animUpdate, &state->moveLib, 0, 0) != 0) {
            return 0;
        }
        animUpdate->flags &= ~OBJSEQ_APPLY_TEXTURE_SCROLL_TRACK;
        characterDoEyeAnims(obj, &state->eyeAnimState);
    }
    state->pathState.subtype = 0;
    objAudioDispatchAnimEvents(obj, &animUpdate->animEvents, 8, state->renderPathPoints, &state->pathState, 1.0f, 1.0f);
    return 0;
}

static int shThorntail_getExtraSize(void) {
    return sizeof(SHthorntailState);
}

static void shThorntail_free(GameObject* obj) {
    const SHthorntailPlacement* placement = shThorntail_getPlacement(obj);

    if (sSHthorntailActiveConfigToken == placement->base.ident) {
        sSHthorntailActiveConfigToken = -1;
    }
    objFreeObjectType(obj, SH_THORNTAIL_OBJECT_GROUP);
}

static void shThorntail_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                               s8 visible) {
    SHthorntailState* state = obj->extra;

    (void)visible;

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    dll_2E_setTargetFromPathPoint(obj, &state->moveLib, 0);
    for (int i = 0; i < ARRAY_COUNT(state->renderPathPoints); i++) {
        ObjPath_GetPointWorldPosition(obj, i, &state->renderPathPoints[i].x, &state->renderPathPoints[i].y,
                                      &state->renderPathPoints[i].z, 0);
    }
}

static void shThorntail_applyGravity(GameObject* obj) {
    obj->anim.velocityY = -(0.17f * timeDelta - obj->anim.velocityY);
}

static void shThorntail_update(GameObject* obj) {
    SHthorntailState* state = obj->extra;
    const SHthorntailPlacement* placement = shThorntail_getPlacement(obj);
    ObjAnimEventList animEvents;
    PartFxSpawnParams effectParams;
    ObjHitReactEntry* hitReactEntries;
    int moveComplete;

    if (state->behaviorState == SH_THORNTAIL_STATE_TAIL_SWING) {
        if (state->effectTimer <= 0.0f) {
            if (obj->objectFlags & OBJECT_OBJFLAG_RENDERED) {
                ObjPath_GetPointWorldPosition(obj, 4, &effectParams.pos.x, &effectParams.pos.y, &effectParams.pos.z, 0);
                (*gPartfxInterface)
                    ->spawnObject(obj, SH_THORNTAIL_TAIL_SWING_EFFECT, &effectParams, 0x200001, -1, NULL);
            }
            state->effectTimer = 30.0f;
        }
        state->effectTimer -= timeDelta;
    }

    state->behaviorFlags &= ~SH_THORNTAIL_LEVEL_CONTROL_READY;
    if (sSHthorntailStateConfigs[state->behaviorState].flags & SH_THORNTAIL_STATE_HEAVY_HIT_REACTION) {
        hitReactEntries = sSHthorntailHeavyHitReactEntries;
    } else {
        hitReactEntries = sSHthorntailNormalHitReactEntries;
    }
    state->hitReactState = ObjHitReact_Update(obj, hitReactEntries, ARRAY_COUNT(sSHthorntailNormalHitReactEntries),
                                              state->hitReactState, &state->hitReactionStepScale);
    if (state->hitReactState != 0) {
        return;
    }

    state->locomotionMode = (*gMapEventInterface)->getMapAct((int)obj->anim.mapEventSlot);
    switch (placement->controlMode) {
    case SH_THORNTAIL_CONTROL_LEVEL_0:
        shThorntail_updateLevelControlMode0(obj, state, placement);
        break;
    case SH_THORNTAIL_CONTROL_LEVEL_1:
        shThorntail_updateLevelControlMode1(obj, state, placement);
        break;
    case SH_THORNTAIL_CONTROL_ROOT_2:
        shThorntail_updateRootControlMode2(obj, state);
        break;
    case SH_THORNTAIL_CONTROL_ROOT_3:
        shThorntail_updateRootControlMode3(obj, state);
        break;
    }

    const SHthorntailStateConfig* stateConfig = &sSHthorntailStateConfigs[state->behaviorState];
    if (stateConfig->flags & SH_THORNTAIL_STATE_STATUS_ACTIVE) {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_PROMPT_SUPPRESSED;
    } else {
        obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_PROMPT_SUPPRESSED;
        obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
    }
    if (state->behaviorFlags & SH_THORNTAIL_FREEZE_MOTION) {
        if (++state->freezeFrameCounter > 10) {
            state->behaviorFlags &= ~SH_THORNTAIL_FREEZE_MOTION;
        } else {
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
        }
    }

    if ((int)obj->anim.currentMove != stateConfig->moveId) {
        ObjAnim_SetCurrentMove(obj, stateConfig->moveId, 0.0f, 0);
        state->storedFacingAngle = obj->anim.rotX;
    }
    moveComplete = ObjAnim_AdvanceCurrentMove(obj, stateConfig->stepScale, timeDelta, &animEvents);
    if (moveComplete != 0) {
        state->behaviorFlags |= SH_THORNTAIL_MOVE_COMPLETE;
    } else {
        state->behaviorFlags &= ~SH_THORNTAIL_MOVE_COMPLETE;
    }

    if (stateConfig->flags & SH_THORNTAIL_STATE_APPLY_ROOT_MOTION) {
        f32 negSinFacing;
        f32 negCosFacing;

        if (state->behaviorFlags & SH_THORNTAIL_MOVE_COMPLETE) {
            state->storedFacingAngle = obj->anim.rotX;
        }
        negSinFacing = -mathSinf((3.1415927f * (f32)(s32)state->storedFacingAngle) / 32768.0f);
        negCosFacing = -mathCosf((3.1415927f * (f32)(s32)state->storedFacingAngle) / 32768.0f);
        obj->anim.localPosX += negSinFacing * -animEvents.rootDeltaZ;
        obj->anim.localPosZ += negCosFacing * -animEvents.rootDeltaZ;
        obj->anim.localPosX += negCosFacing * -animEvents.rootDeltaX;
        obj->anim.localPosZ += negSinFacing * animEvents.rootDeltaX;
        obj->anim.rotX += animEvents.rootPitch;
    }

    for (int i = 0; i < animEvents.triggerCount; i++) {
        if (animEvents.triggeredIds[i] == 0 && stateConfig->trigger0SfxId != 0) {
            Sfx_PlayFromObject(obj, stateConfig->trigger0SfxId);
        }
    }
    objAudioDispatchAnimEvents(obj, &animEvents, 8, state->renderPathPoints, &state->pathState, 1.0f, 1.0f);

    if (stateConfig->flags & SH_THORNTAIL_STATE_ENABLE_LOOK_TRACKING) {
        state->moveLib.modeBits &= ~MOVELIB_HOLD_LOOK_TRACKING;
    } else {
        state->moveLib.modeBits |= MOVELIB_HOLD_LOOK_TRACKING;
    }
    dll_2E_updateLookAt(obj, &state->moveLib);
    if (stateConfig->flags & SH_THORNTAIL_STATE_HEAVY_HIT_REACTION) {
        characterCloseEyes(obj, &state->eyeAnimState);
    } else {
        characterDoEyeAnims(obj, &state->eyeAnimState);
    }

    state->behaviorFlags &= ~SH_THORNTAIL_IMPACT_PENDING;
    if (!(state->behaviorFlags & SH_THORNTAIL_TRIGGER_EVENT_PENDING) && ObjTrigger_IsSet(obj) != 0) {
        u32 sequenceIndex = randomGetRange(1, state->impactSequence->count);

        state->behaviorFlags |= SH_THORNTAIL_IMPACT_PENDING;
        (*gObjectTriggerInterface)->runSequence(state->impactSequence->sequenceIds[sequenceIndex - 1], obj, -1);
    }

    if (placement->leashRadius != 0 &&
        shThorntail_homeDistanceSquared(obj, placement) >
            (f32)((u32)placement->leashRadius * (u32)placement->leashRadius) &&
        ViewFrustum_IsSphereVisible(&obj->anim.localPosX, obj->anim.hitboxScale * obj->anim.rootMotionScale) == 0) {
        obj->anim.rotX =
            getAngle(obj->anim.localPosX - placement->base.posX, obj->anim.localPosZ - placement->base.posZ);
    }

    state->pathState.subtype = 1;
    if (sSHthorntailActiveConfigToken == -1) {
        sSHthorntailActiveConfigToken = placement->base.ident;
        shThorntail_applyGravity(obj);
        (*gPathControlInterface)->update(obj, &state->pathState, timeDelta);
        (*gPathControlInterface)->apply(obj, &state->pathState);
        (*gPathControlInterface)->advance(obj, &state->pathState, timeDelta);
        obj->anim.rotY = state->pathState.tiltPitch;
        obj->anim.rotZ = state->pathState.tiltRoll;
    } else {
        if (sSHthorntailActiveConfigToken == placement->base.ident) {
            sSHthorntailActiveConfigToken = -1;
        }
        if (state->behaviorState >= SH_THORNTAIL_STATE_MOVE_2 && state->behaviorState <= SH_THORNTAIL_STATE_TURN_HOME) {
            shThorntail_applyGravity(obj);
            (*gPathControlInterface)->update(obj, &state->pathState, timeDelta);
            (*gPathControlInterface)->apply(obj, &state->pathState);
            (*gPathControlInterface)->advance(obj, &state->pathState, timeDelta);
            obj->anim.rotY = state->pathState.tiltPitch;
            obj->anim.rotZ = state->pathState.tiltRoll;
        } else {
            (*gPathControlInterface)->attachObject(obj, &state->pathState);
        }
    }
}

static void shThorntail_init(GameObject* obj, const SHthorntailPlacement* placement) {
    SHthorntailState* state = obj->extra;
    ObjModel* model;
    u8 pathSourceTypes[] = {1, 1, 1, 1};

    obj->anim.rotX = (s16)((u16)placement->initialFacing * 0x100);
    switch (placement->controlMode) {
    case SH_THORNTAIL_CONTROL_LEVEL_0:
    case SH_THORNTAIL_CONTROL_ROOT_2:
    case SH_THORNTAIL_CONTROL_ROOT_3:
        state->behaviorState = SH_THORNTAIL_STATE_IDLE;
        state->idleTimer = (f32)randomGetRange(1000, 2000);
        break;
    case SH_THORNTAIL_CONTROL_LEVEL_1:
        state->tailSwingState = SH_THORNTAIL_TAIL_SWING_ACTIVE;
        state->behaviorState = SH_THORNTAIL_STATE_TAIL_SWING;
        break;
    }

    obj->anim.rootMotionScale = obj->anim.modelInstance->rootMotionScaleBase *
                                ((f32)ObjAnim_ReadPlacementU16(&obj->anim, &placement->scale) / 1000.0f);
    model = Obj_GetActiveModel(obj);
    modelInitBones(obj->anim.rootMotionScale, model);
    (*gPathControlInterface)->init(&state->pathState, SH_THORNTAIL_PATH_MODE, SH_THORNTAIL_PATH_FLAGS, 0);
    (*gPathControlInterface)
        ->setup(&state->pathState, SH_THORNTAIL_PATH_CHANNEL, sSHthorntailPathPoints, sSHthorntailPathRadii,
                pathSourceTypes);
    (*gPathControlInterface)->attachObject(obj, &state->pathState);
    obj->animEventCallback = shThorntail_updateLevelControlState;
    dll_2E_initState(obj, &state->moveLib, (s16)-9102, 0x2AAA, 3);
    dll_2E_setReattackDelay(&state->moveLib, 400, 0x78);
    objAddObjectType(obj, SH_THORNTAIL_OBJECT_GROUP);
}

OBJECT_INIT_ADAPTER(gSH_thorntailObjDescriptorInitAdapter, shThorntail_init, obj, placement)
OBJECT_FREE_ADAPTER(gSH_thorntailObjDescriptorFreeAdapter, shThorntail_free, obj)
OBJECT_EXTRA_SIZE_ADAPTER(gSH_thorntailObjDescriptorExtraSizeAdapter, shThorntail_getExtraSize)

ObjectDescriptor gSH_thorntailObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = NULL,
        .release = NULL,
    },
    .init = gSH_thorntailObjDescriptorInitAdapter,
    .update = shThorntail_update,
    .render = shThorntail_render,
    .free = gSH_thorntailObjDescriptorFreeAdapter,
    .getExtraSize = gSH_thorntailObjDescriptorExtraSizeAdapter,
};;
