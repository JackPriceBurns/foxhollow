#ifndef FOXHOLLOW_MOD_API_H_
#define FOXHOLLOW_MOD_API_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FH_MOD_ABI_VERSION 2u

#define FH_MOD_OK 0
#define FH_MOD_ERROR 1

#if defined(_WIN32)
#define FH_MOD_EXPORT __declspec(dllexport)
#else
#define FH_MOD_EXPORT __attribute__((visibility("default")))
#endif

/* Declares a symbol that lives in the host executable. Windows reaches exported
   data only through an import thunk, so the declaration has to say so; the ELF
   and Mach-O linkers resolve it from the host image without any marker. */
#if defined(_WIN32)
#define FH_MOD_IMPORT __declspec(dllimport)
#else
#define FH_MOD_IMPORT
#endif

typedef struct FhMod FhMod;

#define FH_NET_BROADCAST 0
#define FH_NET_MAX_PAYLOAD 1024
#define FH_NET_NAME_CAPACITY 32

typedef struct FhNetPlayer {
  int32_t id;
  char name[FH_NET_NAME_CAPACITY];
} FhNetPlayer;

typedef enum FhLogLevel {
  FH_LOG_INFO = 0,
  FH_LOG_WARN = 1,
  FH_LOG_ERROR = 2
} FhLogLevel;

typedef enum FhClassSlot {
  FH_SLOT_02 = 0,
  FH_SLOT_INIT = 1,
  FH_SLOT_UPDATE = 2,
  FH_SLOT_HIT_DETECT = 3,
  FH_SLOT_RENDER = 4,
  FH_SLOT_FREE = 5,
  FH_SLOT_GET_TYPE_ID = 6,
  FH_SLOT_GET_EXTRA_SIZE = 7
} FhClassSlot;

typedef void (*FhClassCallback)(void);

typedef struct FhModHost {
  uint32_t structSize;
  uint32_t abiVersion;

  const char* (*modId)(FhMod* mod);
  const char* (*modDir)(FhMod* mod);
  void (*log)(FhMod* mod, FhLogLevel level, const char* message);
  uint64_t (*frameCount)(FhMod* mod);

  uint32_t (*classCount)(FhMod* mod);
  int (*classReplaceCallback)(FhMod* mod, uint32_t classId, FhClassSlot slot, FhClassCallback replacement,
                              FhClassCallback* outOriginal);

  void* (*symbolAddress)(FhMod* mod, const char* name);
  int (*hookInstall)(FhMod* mod, void* target, void* replacement, void** outOriginal);
  int (*hookRemove)(FhMod* mod, void* target);

  int (*configBool)(FhMod* mod, const char* key, int fallback);
  int32_t (*configInt)(FhMod* mod, const char* key, int32_t fallback);
  float (*configFloat)(FhMod* mod, const char* key, float fallback);
  const char* (*configString)(FhMod* mod, const char* key, const char* fallback);

  int32_t (*netLocalPlayer)(FhMod* mod);
  int32_t (*netHostPlayer)(FhMod* mod);
  uint32_t (*netPlayers)(FhMod* mod, FhNetPlayer* out, uint32_t capacity);
  int (*netSend)(FhMod* mod, int32_t toPlayer, const void* data, uint32_t size);
  const char* (*netRoomCode)(FhMod* mod);
} FhModHost;

typedef int (*FhModInitializeFn)(FhMod* mod, const FhModHost* host);
typedef void (*FhModUpdateFn)(FhMod* mod);
typedef void (*FhModShutdownFn)(FhMod* mod);
typedef void (*FhModConfigChangedFn)(FhMod* mod);
typedef void (*FhModNetConnectedFn)(FhMod* mod);
typedef void (*FhModNetDisconnectedFn)(FhMod* mod);
typedef void (*FhModNetMessageFn)(FhMod* mod, int32_t fromPlayer, const void* data, uint32_t size);
typedef void (*FhModNetPlayerJoinedFn)(FhMod* mod, int32_t player, const char* name);
typedef void (*FhModNetPlayerLeftFn)(FhMod* mod, int32_t player);
typedef void (*FhModNetHostChangedFn)(FhMod* mod, int32_t player);

#ifdef __cplusplus
}
#endif

#endif /* FOXHOLLOW_MOD_API_H_ */
