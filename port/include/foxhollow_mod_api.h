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

typedef enum FhUiWindowFlags {
  FH_UI_WINDOW_MODAL = 1u << 0,
  FH_UI_WINDOW_PASSIVE = 1u << 1,
  FH_UI_WINDOW_NO_TITLE = 1u << 2,
  FH_UI_WINDOW_TRANSPARENT = 1u << 3,
  FH_UI_WINDOW_FIXED = 1u << 4,
  FH_UI_WINDOW_AUTO_HEIGHT = 1u << 5,
  FH_UI_WINDOW_RELATIVE = 1u << 6,
  FH_UI_WINDOW_ANCHOR_BOTTOM = 1u << 7
} FhUiWindowFlags;

typedef enum FhUiInputFlags {
  FH_UI_INPUT_FOCUS = 1u << 0
} FhUiInputFlags;

typedef enum FhUiInputResult {
  FH_UI_INPUT_NONE = 0,
  FH_UI_INPUT_EDITED = 1,
  FH_UI_INPUT_SUBMITTED = 2,
  FH_UI_INPUT_HISTORY_PREVIOUS = 3,
  FH_UI_INPUT_HISTORY_NEXT = 4
} FhUiInputResult;

typedef enum FhUiKey {
  FH_UI_KEY_F1 = 0x101,
  FH_UI_KEY_F2 = 0x102,
  FH_UI_KEY_F3 = 0x103,
  FH_UI_KEY_F4 = 0x104,
  FH_UI_KEY_F5 = 0x105,
  FH_UI_KEY_F6 = 0x106,
  FH_UI_KEY_F7 = 0x107,
  FH_UI_KEY_F8 = 0x108,
  FH_UI_KEY_F9 = 0x109,
  FH_UI_KEY_F10 = 0x10A,
  FH_UI_KEY_F11 = 0x10B,
  FH_UI_KEY_F12 = 0x10C
} FhUiKey;

typedef struct FhUiWindow {
  uint32_t structSize;
  const char* title;
  float x;
  float y;
  float width;
  float height;
  uint32_t flags;
} FhUiWindow;

typedef struct FhUi FhUi;

struct FhUi {
  uint32_t structSize;
  float displayWidth;
  float displayHeight;
  float gameX;
  float gameY;
  float gameWidth;
  float gameHeight;

  int (*begin)(const FhUi* ui, const char* id, const FhUiWindow* window, int* open);
  void (*end)(const FhUi* ui);

  void (*text)(const FhUi* ui, const char* text);
  int (*button)(const FhUi* ui, const char* label);
  int (*checkbox)(const FhUi* ui, const char* label, int* value);
  int (*slider)(const FhUi* ui, const char* label, float* value, float minimum, float maximum);
  int (*input)(const FhUi* ui, const char* label, char* buffer, uint32_t capacity, uint32_t flags);
  int (*beginSection)(const FhUi* ui, const char* label);
  void (*endSection)(const FhUi* ui);
  int (*beginScroll)(const FhUi* ui, const char* id);
  void (*endScroll)(const FhUi* ui);
  void (*sameLine)(const FhUi* ui);
  void (*separator)(const FhUi* ui);

  int (*keyPressed)(const FhUi* ui, uint32_t key);

  void (*drawText)(const FhUi* ui, float x, float y, uint32_t rgba, const char* text);
  void (*drawRect)(const FhUi* ui, float x0, float y0, float x1, float y1, uint32_t rgba, float rounding);
  void (*measureText)(const FhUi* ui, const char* text, float* outWidth, float* outHeight);
};

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
typedef void (*FhModDrawFn)(FhMod* mod, const FhUi* ui);

#ifdef __cplusplus
}
#endif

#endif /* FOXHOLLOW_MOD_API_H_ */
