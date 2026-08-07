#ifndef MUSYX_SND3D_H_
#define MUSYX_SND3D_H_

#include "types.h"
#include "musyx/snd_types.h"

#define S3D_EMITTER_FLAG_POSITIONAL 0x00000001
#define S3D_EMITTER_FLAG_RESTART_ON_STOP 0x00000002
#define S3D_EMITTER_FLAG_STOP_AT_ORIGIN 0x00000004
#define S3D_EMITTER_FLAG_USE_AUX_STUDIO 0x00000010
#define S3D_EMITTER_FLAG_REMOVE_AT_ORIGIN 0x00000040
#define S3D_EMITTER_FLAG_PLAYING 0x00020000
#define S3D_EMITTER_FLAG_REMOVE 0x00040000
#define S3D_EMITTER_FLAG_WAITING_FOR_ROOM 0x00080000
#define S3D_EMITTER_FLAG_AGE_OUT 0x00100000
#define S3D_INVALID_FX_HANDLE 0xffffffff

#define S3D_UPDATE_SKIP_TICKS 3
#define S3D_DEFAULT_FX_VOLUME 0x7f
#define S3D_DEFAULT_FX_PAN 0x40
#define S3D_INIT_STEREO_FLAG 0x2
#define S3D_BASE_STUDIO 1
#define S3D_MAX_STUDIOS 3
#define SND_MAX_VOICES 0x40
#define SND_MAX_STUDIOS 8
#define SND_DEFAULT_SAMPLE_RATE 0x7d00

typedef struct SND_FVECTOR {
    f32 x;
    f32 y;
    f32 z;
} SND_FVECTOR;

typedef struct SND_ROOM {
    struct SND_ROOM *next;
    struct SND_ROOM *prev;
    u32 flags;
    SND_FVECTOR pos;
    f32 distance;
    u8 studio;
    u8 pad1d[3];
    void (*activateReverb)(u8 studio, u32 user);
    void (*deActivateReverb)(u8 studio);
    u32 user;
    u32 curMVol;
} SND_ROOM;

typedef struct SND_LISTENER {
    struct SND_LISTENER *next;
    struct SND_LISTENER *prev;
    SND_ROOM *room;
    u32 flags;
    SND_FVECTOR pos;
    f32 volPosOff;
    SND_FVECTOR dir;
    SND_FVECTOR heading;
    SND_FVECTOR right;
    SND_FVECTOR up;
    f32 mat[12];
    f32 surroundDisFront;
    f32 surroundDisBack;
    f32 soundSpeed;
    f32 vol;
} SND_LISTENER;

typedef struct SND_DOOR {
    struct SND_DOOR *next;
    struct SND_DOOR *prev;
    SND_FVECTOR pos;
    f32 open;
    f32 dampen;
    u8 fxVol;
    u8 destStudio;
    u8 pad1e[2];
    SND_ROOM *a;
    SND_ROOM *b;
    u32 flags;
    s16 filterCoef[4];
    SND_STUDIO_INPUT input;
} SND_DOOR;

typedef struct SND_PARAMETER {
    u8 ctrl;
    u8 pad01;
    union _paraData {
        u8 value7;
        u16 value14;
    } paraData;
} SND_PARAMETER;

typedef struct SND_PARAMETER_INFO {
    u8 numPara;
    u8 pad01[3];
    SND_PARAMETER *paraArray;
} SND_PARAMETER_INFO;

typedef struct SND_EMITTER {
    struct SND_EMITTER *next;
    struct SND_EMITTER *prev;
    SND_ROOM *room;
    SND_PARAMETER_INFO *paraInfo;
    u32 flags;
    SND_FVECTOR pos;
    SND_FVECTOR dir;
    f32 maxDis;
    f32 maxVol;
    f32 minVol;
    f32 volPush;
    u32 vid;
    u32 group;
    u16 fxid;
    u8 studio;
    u8 maxVoices;
    u16 VolLevelCnt;
    u8 pad4a[0x4c - 0x4a];
    f32 fade;
} SND_EMITTER;

void s3dHandle(void);
void s3dInit(u32 flags);
void s3dExit(void);
int sndInit(u8 voiceCount, u8 streamCount, u8 unk5, u8 stereo, u32 flags, u32 aramSize);

extern u8 snd_max_studios;
extern u8 snd_base_studio;
extern u32 snd_used_studios;
extern SND_DOOR* s3dDoorRoot;
extern SND_ROOM* s3dRoomRoot;
extern SND_LISTENER* s3dListenerRoot;
extern SND_EMITTER* s3dEmitterRoot;

extern u8 startGroupNum;
extern u8 startListNumnum;
extern u8 runListNum;
extern u8 sSnd3dStereo;

#endif /* MUSYX_SND3D_H_ */
