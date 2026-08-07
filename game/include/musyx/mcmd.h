#ifndef MUSYX_MCMD_H_
#define MUSYX_MCMD_H_

#include "musyx/adsr.h"

#define MAC_STATE_RUNNABLE 0
#define MAC_STATE_YIELDED 1
#define MAC_STATE_STOPPED 2

#define MCMD_LOOP_RANDOM_DELAY_FLAG 0x00010000
#define MCMD_LOOP_WAIT_FOR_KEYOFF_FLAG 0x00000100
#define MCMD_LOOP_WAIT_FOR_INACTIVE_FLAG 0x01000000
#define MCMD_LOOP_COUNTER_FOREVER 0xFFFF
#define MCMD_WAIT_ABSOLUTE_TIME_FLAG 0x00000001
#define MCMD_WAIT_TIME_UNIT_MS_FLAG 0x00000100

#define MCMD_VOICE_KEYOFF_INPUT_FLAG 0x100
#define MCMD_VOICE_PITCH_ADSR_INPUT_FLAG 0x200
#define MCMD_VOICE_DEFERRED_KEYOFF_INPUT_FLAG 0x400
#define MCMD_VOICE_START_OFFSET_INPUT_FLAG 0x800
#define MCMD_VOICE_PARAM_RAMP_INPUT_FLAG 0x2000
#define MCMD_VOICE_KEYOFF_WAIT_OUTPUT_FLAG 0x4
#define MCMD_VOICE_KEYOFF_OUTPUT_FLAG 0x8
#define MCMD_VOICE_ACTIVE_OUTPUT_FLAG 0x20
#define MCMD_VOICE_KEY_SYNC_OUTPUT_FLAG 0x100
#define MCMD_VOICE_PORTAMENTO_OUTPUT_FLAG 0x400
#define MCMD_VOICE_ALLOCATED_OUTPUT_FLAG 0x2
#define MCMD_VOICE_VOLUME_RAMP_OUTPUT_FLAG 0x8000
#define MCMD_VOICE_INACTIVE_WAIT_OUTPUT_FLAG 0x40000
#define MCMD_VOICE_VIBRATO_RAMP_OUTPUT_FLAG 0x2000
#define MCMD_VOICE_VIBRATO_CURVE_OUTPUT_FLAG 0x4000

#define MCMD_INPUT_DIRTY_VOLUME 0x0001
#define MCMD_INPUT_DIRTY_PANNING 0x0002
#define MCMD_INPUT_DIRTY_SUR_PANNING 0x0004
#define MCMD_INPUT_DIRTY_PITCH_BEND 0x0008
#define MCMD_INPUT_DIRTY_DOPPLER 0x0010
#define MCMD_INPUT_DIRTY_MODULATION 0x0020
#define MCMD_INPUT_DIRTY_PEDAL 0x0040
#define MCMD_INPUT_DIRTY_PORTAMENTO 0x0080
#define MCMD_INPUT_DIRTY_PRE_AUX_A 0x0100
#define MCMD_INPUT_DIRTY_REVERB 0x0200
#define MCMD_INPUT_DIRTY_PRE_AUX_B 0x0400
#define MCMD_INPUT_DIRTY_POST_AUX_B 0x0800
#define MCMD_INPUT_DIRTY_TREMOLO 0x1000
#define MCMD_INPUT_DIRTY_ALL 0x1FFF

#define MCMD_INPUT_ENTRY_COMBINE_MASK 0x0F
#define MCMD_INPUT_ENTRY_USE_VAR_FLAG 0x10
#define MCMD_INPUT_COMBINE_SET 0
#define MCMD_INPUT_COMBINE_ADD 1
#define MCMD_INPUT_COMBINE_MUL 2
#define MCMD_INPUT_COMBINE_SUB 3

#define MCMD_CTRL_MODULATION 0x01
#define MCMD_CTRL_VOLUME 0x07
#define MCMD_CTRL_EXPRESSION 0x0B
#define MCMD_CTRL_PANNING 0x0A
#define MCMD_CTRL_PEDAL 0x40
#define MCMD_CTRL_PORTAMENTO 0x41
#define MCMD_CTRL_REVERB 0x5B
#define MCMD_CTRL_POST_AUX_B 0x5D
#define MCMD_CTRL_PITCH_BEND 0x80
#define MCMD_CTRL_SUR_PANNING 0x83
#define MCMD_CTRL_DOPPLER 0x84
#define MCMD_CTRL_EX_A0 0xA0
#define MCMD_CTRL_EX_A1 0xA1
#define MCMD_CTRL_MIDI_LAYER 0xA2
#define MCMD_CTRL_VOICE_AGE 0xA3
#define MCMD_CTRL_VOICE_TIME 0xA4

typedef struct McmdCommandArgs {
    u32 flags;
    u32 value;
} McmdCommandArgs;

typedef struct McmdInputEntry {
    u8 midiCtrl;
    u8 combine;
    u8 unk2[2];
    s32 scale;
} McmdInputEntry;

typedef struct McmdInputSlot {
    McmdInputEntry source[4];
    s16 oldValue;
    u8 numSource;
    u8 unk23;
} McmdInputSlot;

typedef struct SYNTH_LFO {
    u32 time;
    u32 period;
    s16 value;
    s16 lastValue;
} SYNTH_LFO;

typedef struct VID_LIST {
    struct VID_LIST *next;
    struct VID_LIST *prev;
    u32 vid;
    u32 root;
} VID_LIST;

typedef struct CALLSTACK {
    u8 *addr;
    u8 *curAddr;
} CALLSTACK;

typedef struct McmdVoiceState {
    u8 unk00[0x24];
    u32 lastLowCallTimeHi;
    u32 lastLowCallTimeLo;
    u32 lastZeroCallTimeHi;
    u32 lastZeroCallTimeLo;
    u8 *addr;
    u8 *curAddr;
    struct McmdVoiceState *nextMacActive;
    struct McmdVoiceState *prevMacActive;
    struct McmdVoiceState *nextTimeQueueMacro;
    struct McmdVoiceState *prevTimeQueueMacro;
    s32 macState;
    u8 *trapEventAddr[3];
    u8 *trapEventCurAddr[3];
    u8 trapEventAny;
    u8 unk69[3];
    CALLSTACK callStack[4];
    u8 callStackEntryNum;
    u8 callStackIndex;
    u8 unk8E[2];
    u32 macStartTimeHi;
    u32 macStartTimeLo;
    u32 waitHi;
    u32 waitLo;
    u32 waitTimeHi;
    u32 waitTimeLo;
    u8 timeUsedByInput;
    u8 unkA9;
    u16 loop;
    u32 local_vars[16];
    u32 child;
    u32 parent;
    u32 id;
    VID_LIST *vidList;
    VID_LIST *vidMasterList;
    u16 allocId;
    u16 macroId;
    u8 keyGroup;
    u8 unk105[3];
    u32 lastVID;
    u8 prio;
    u8 unk10D;
    u16 ageSpeed;
    u32 age;
    u32 cFlagsHi;
    u32 cFlagsLo;
    u8 block;
    u8 fxFlag;
    u8 vGroup;
    u8 studio;
    u8 track;
    u8 midi;
    u8 midiSet;
    u8 section;
    u32 sInfo;
    u32 playFrq;
    u16 curNote;
    s8 curDetune;
    u8 orgNote;
    u8 lastNote;
    u8 portType;
    u16 portLastCtrlState;
    u32 portDuration;
    u32 portCurPitch; /* 0x138: current portamento pitch, key<<16 + bend */
    u32 portTime;
    u8 vibKeyRange;
    u8 vibCentRange;
    u8 unk142[2];
    u32 vibPeriod;
    u32 vibCurTime;
    s32 vibCurOffset;
    s16 vibModAddScale;
    u8 unk152[2];
    u32 volume;
    u32 orgVolume;
    f32 lastVolFaderScale;
    u32 lastPan;
    u32 lastSPan;
    f32 treCurScale;
    u16 treScale;
    u16 treModAddScale;
    u32 panning[2];
    s32 panDelta[2];
    u32 panTarget[2];
    u32 panTime[2];
    u8 revVolScale;
    u8 revVolOffset;
    u8 volTable;
    u8 itdMode;
    s32 envDelta;
    s32 envTarget;
    s32 envCurrent;
    s32 sweepOff[2];
    s32 sweepAdd[2];
    s32 sweepCnt[2];
    u8 sweepNum[2];
    u8 unk1BA[2];
    SYNTH_LFO lfo[2];
    u8 lfoUsedByInput[2];
    u8 pbLowerKeyRange;
    u8 pbUpperKeyRange;
    u16 pbLast;
    u8 unk1DA[0x1DC - 0x1DA];
    ADSR_VARS pitchADSR;
    s16 pitchADSRRange;
    u16 curPitch;
    struct {
        u8 vol;
        u8 pan;
        u8 midi;
        u8 midiSet;
        u8 section;
        u8 track;
        u8 vGroup;
        u8 studio;
        u8 itdMode;
    } setup;
    u8 unk211[3];
    u32 midiDirtyFlags;
    McmdInputSlot inpVolume;
    McmdInputSlot inpPanning;
    McmdInputSlot inpSurroundPanning;
    McmdInputSlot inpPitchBend;
    McmdInputSlot inpDoppler;
    McmdInputSlot inpModulation;
    McmdInputSlot inpPedal;
    McmdInputSlot inpPortamento;
    McmdInputSlot inpPreAuxA;
    McmdInputSlot inpReverb;
    McmdInputSlot inpPreAuxB;
    McmdInputSlot inpPostAuxB;
    McmdInputSlot inpTremolo;

    u8 mesgNum;
    u8 mesgRead;
    u8 mesgWrite;
    u8 unk3EF;
    u32 mesgQueue[4];
    u16 curOutputVolume;
    u8 unk402[2];
} McmdVoiceState;

extern McmdVoiceState* synthVoice;

extern McmdInputSlot inpAuxA[8][4];
extern McmdInputSlot inpAuxB[8][4];

#ifdef STATIC_ASSERT
STATIC_ASSERT(offsetof(McmdVoiceState, child) == 0xEC);
STATIC_ASSERT(offsetof(McmdVoiceState, id) == 0xF4);
STATIC_ASSERT(offsetof(McmdVoiceState, prio) == 0x10C);
STATIC_ASSERT(offsetof(McmdVoiceState, cFlagsHi) == 0x114);
STATIC_ASSERT(offsetof(McmdVoiceState, curNote) == 0x12C);
STATIC_ASSERT(offsetof(McmdVoiceState, portCurPitch) == 0x138);
STATIC_ASSERT(offsetof(McmdVoiceState, midiDirtyFlags) == 0x214);
STATIC_ASSERT(sizeof(McmdVoiceState) == 0x404);
#endif

#endif /* MUSYX_MCMD_H_ */
