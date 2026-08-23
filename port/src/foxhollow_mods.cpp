#include "foxhollow_mods.h"

#include "foxhollow_mod_api.h"

#include <dolphin/gx/GXStruct.h>

#include <aurora/dvd.h>
#include <aurora/texture.hpp>

#include <nlohmann/json.hpp>

#if !defined(_WIN32)
#include <dlfcn.h>
#include <sys/mman.h>
#include <unistd.h>
#else
#include <windows.h>
#endif

#if defined(__APPLE__)
#include <libkern/OSCacheControl.h>
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach/mach.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <memory>
#include <string>
#include <system_error>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

namespace {

std::string path_to_utf8(const fs::path& path);

struct OverlayEntry {
  std::string discPath;
  std::string filePath;
  std::string ownerId;
  std::uintmax_t size = 0;
};

struct ModEntry {
  std::string id;
  std::string name;
  std::string version;
  std::string author;
  fs::path root;
};

struct ClassPatch {
  uint32_t classId;
  uint32_t slot;
  FhClassCallback original;
};

struct NativeMod {
  std::string id;
  std::string dir;
  void* handle = nullptr;
  FhModUpdateFn update = nullptr;
  FhModShutdownFn shutdown = nullptr;
  std::vector<ClassPatch> patches;
  std::vector<void*> hooks;
};

std::vector<std::unique_ptr<NativeMod>> sNativeMods;
uint64_t sFrameCount = 0;

extern "C" {
extern void* gResourceDescriptors[];
}

constexpr uint32_t kResourceDescriptorCount = 0x2c1;
constexpr size_t kInterfaceOffset = sizeof(uint32_t) * 4 + sizeof(void*) * 2;
constexpr uint32_t kInterfaceSlotCount = 8;

const char* platform_directory() {
#if defined(_WIN32)
#if defined(__aarch64__) || defined(_M_ARM64)
  return "windows-arm64";
#else
  return "windows-amd64";
#endif
#elif defined(__APPLE__)
#if defined(__aarch64__)
  return "macos-arm64";
#else
  return "macos-x86_64";
#endif
#else
#if defined(__aarch64__)
  return "linux-arm64";
#else
  return "linux-amd64";
#endif
#endif
}

const char* library_name() {
#if defined(_WIN32)
  return "mod.dll";
#else
  return "mod.so";
#endif
}

void* open_library(const std::string& path, std::string& outError) {
#if defined(_WIN32)
  void* handle = static_cast<void*>(LoadLibraryA(path.c_str()));
  if (handle == nullptr) {
    outError = "LoadLibrary failed";
  }
  return handle;
#else
  void* handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
  if (handle == nullptr) {
    const char* message = dlerror();
    outError = message != nullptr ? message : "dlopen failed";
  }
  return handle;
#endif
}

void* find_symbol(void* handle, const char* name) {
#if defined(_WIN32)
  return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(handle), name));
#else
  return dlsym(handle, name);
#endif
}

void close_library(void* handle) {
#if defined(_WIN32)
  FreeLibrary(static_cast<HMODULE>(handle));
#else
  dlclose(handle);
#endif
}

FhClassCallback* class_slots(uint32_t classId) {
  if (classId >= kResourceDescriptorCount || gResourceDescriptors[classId] == nullptr) {
    return nullptr;
  }
  auto* base = static_cast<uint8_t*>(gResourceDescriptors[classId]);
  return reinterpret_cast<FhClassCallback*>(base + kInterfaceOffset);
}

const char* host_mod_id(FhMod* mod) { return reinterpret_cast<NativeMod*>(mod)->id.c_str(); }

const char* host_mod_dir(FhMod* mod) { return reinterpret_cast<NativeMod*>(mod)->dir.c_str(); }

void host_log(FhMod* mod, FhLogLevel level, const char* message) {
  const char* tag = level == FH_LOG_ERROR ? "error" : (level == FH_LOG_WARN ? "warn" : "info");
  std::fprintf(stderr, "foxhollow: mods: [%s] %s: %s\n", reinterpret_cast<NativeMod*>(mod)->id.c_str(), tag,
               message != nullptr ? message : "");
}

uint64_t host_frame_count(FhMod*) { return sFrameCount; }

uint32_t host_class_count(FhMod*) { return kResourceDescriptorCount; }

int host_class_replace_callback(FhMod* mod, uint32_t classId, FhClassSlot slot, FhClassCallback replacement,
                                FhClassCallback* outOriginal) {
  if (slot >= kInterfaceSlotCount || replacement == nullptr) {
    return FH_MOD_ERROR;
  }
  FhClassCallback* slots = class_slots(classId);
  if (slots == nullptr) {
    return FH_MOD_ERROR;
  }

  auto* native = reinterpret_cast<NativeMod*>(mod);
  const FhClassCallback original = slots[slot];
  native->patches.push_back(ClassPatch{classId, static_cast<uint32_t>(slot), original});
  slots[slot] = replacement;
  if (outOriginal != nullptr) {
    *outOriginal = original;
  }
  return FH_MOD_OK;
}

constexpr uint32_t kArm64Nop = 0xd503201fu;
constexpr size_t kPatchBytes = 16;

#if defined(__APPLE__) && defined(__aarch64__)

void* resolve_symbol(const char* name) {
  if (name == nullptr || name[0] == '\0') {
    return nullptr;
  }
  const std::string decorated = std::string("_") + name;

  const struct mach_header_64* header = reinterpret_cast<const struct mach_header_64*>(_dyld_get_image_header(0));
  if (header == nullptr) {
    return nullptr;
  }
  const intptr_t slide = _dyld_get_image_vmaddr_slide(0);

  const symtab_command* symtab = nullptr;
  const segment_command_64* linkedit = nullptr;
  const uint8_t* cursor = reinterpret_cast<const uint8_t*>(header) + sizeof(struct mach_header_64);
  for (uint32_t i = 0; i < header->ncmds; ++i) {
    const load_command* command = reinterpret_cast<const load_command*>(cursor);
    if (command->cmd == LC_SYMTAB) {
      symtab = reinterpret_cast<const symtab_command*>(command);
    } else if (command->cmd == LC_SEGMENT_64) {
      const auto* segment = reinterpret_cast<const segment_command_64*>(command);
      if (std::strcmp(segment->segname, SEG_LINKEDIT) == 0) {
        linkedit = segment;
      }
    }
    cursor += command->cmdsize;
  }
  if (symtab == nullptr || linkedit == nullptr) {
    return nullptr;
  }

  const uintptr_t base = static_cast<uintptr_t>(slide) + linkedit->vmaddr - linkedit->fileoff;
  const auto* symbols = reinterpret_cast<const struct nlist_64*>(base + symtab->symoff);
  const char* strings = reinterpret_cast<const char*>(base + symtab->stroff);

  for (uint32_t i = 0; i < symtab->nsyms; ++i) {
    const struct nlist_64& symbol = symbols[i];
    if ((symbol.n_type & N_STAB) != 0 || (symbol.n_type & N_TYPE) != N_SECT || symbol.n_value == 0) {
      continue;
    }
    if (decorated == (strings + symbol.n_un.n_strx)) {
      return reinterpret_cast<void*>(static_cast<uintptr_t>(symbol.n_value) + static_cast<uintptr_t>(slide));
    }
  }
  return nullptr;
}

bool unprotect_range(void* address, size_t size) {
  const size_t pageSize = static_cast<size_t>(getpagesize());
  const uintptr_t start = reinterpret_cast<uintptr_t>(address) & ~static_cast<uintptr_t>(pageSize - 1);
  const uintptr_t end = (reinterpret_cast<uintptr_t>(address) + size + pageSize - 1) & ~static_cast<uintptr_t>(pageSize - 1);
  const kern_return_t result = vm_protect(mach_task_self(), static_cast<vm_address_t>(start),
                                          static_cast<vm_size_t>(end - start), FALSE,
                                          VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY);
  if (result == KERN_SUCCESS) {
    return true;
  }
  return mprotect(reinterpret_cast<void*>(start), end - start, PROT_READ | PROT_WRITE) == 0;
}

void reprotect_range(void* address, size_t size) {
  const size_t pageSize = static_cast<size_t>(getpagesize());
  const uintptr_t start = reinterpret_cast<uintptr_t>(address) & ~static_cast<uintptr_t>(pageSize - 1);
  const uintptr_t end = (reinterpret_cast<uintptr_t>(address) + size + pageSize - 1) & ~static_cast<uintptr_t>(pageSize - 1);
  mprotect(reinterpret_cast<void*>(start), end - start, PROT_READ | PROT_EXEC);
  sys_icache_invalidate(address, size);
}

bool has_patch_pad(const void* target) {
  uint32_t words[4];
  std::memcpy(words, target, sizeof(words));
  for (uint32_t word : words) {
    if (word != kArm64Nop) {
      return false;
    }
  }
  return true;
}

int install_hook(void* target, void* replacement, void** outOriginal) {
  if (target == nullptr || replacement == nullptr) {
    return FH_MOD_ERROR;
  }
  if (!has_patch_pad(target)) {
    return FH_MOD_ERROR;
  }
  if (!unprotect_range(target, kPatchBytes)) {
    return FH_MOD_ERROR;
  }

  uint8_t patch[kPatchBytes];
  const uint32_t ldr = 0x58000050u;
  const uint32_t br = 0xd61f0200u;
  const uint64_t destination = reinterpret_cast<uint64_t>(replacement);
  std::memcpy(patch + 0, &ldr, 4);
  std::memcpy(patch + 4, &br, 4);
  std::memcpy(patch + 8, &destination, 8);
  std::memcpy(target, patch, kPatchBytes);

  reprotect_range(target, kPatchBytes);
  if (outOriginal != nullptr) {
    *outOriginal = static_cast<uint8_t*>(target) + kPatchBytes;
  }
  return FH_MOD_OK;
}

int remove_hook(void* target) {
  if (target == nullptr || !unprotect_range(target, kPatchBytes)) {
    return FH_MOD_ERROR;
  }
  uint32_t nops[4] = {kArm64Nop, kArm64Nop, kArm64Nop, kArm64Nop};
  std::memcpy(target, nops, sizeof(nops));
  reprotect_range(target, kPatchBytes);
  return FH_MOD_OK;
}

#else

void* resolve_symbol(const char*) { return nullptr; }
int install_hook(void*, void*, void**) { return FH_MOD_ERROR; }
int remove_hook(void*) { return FH_MOD_ERROR; }

#endif

void* host_symbol_address(FhMod*, const char* name) { return resolve_symbol(name); }

int host_hook_install(FhMod* mod, void* target, void* replacement, void** outOriginal) {
  const int result = install_hook(target, replacement, outOriginal);
  if (result == FH_MOD_OK) {
    reinterpret_cast<NativeMod*>(mod)->hooks.push_back(target);
  }
  return result;
}

int host_hook_remove(FhMod* mod, void* target) {
  const int result = remove_hook(target);
  if (result == FH_MOD_OK) {
    auto& hooks = reinterpret_cast<NativeMod*>(mod)->hooks;
    hooks.erase(std::remove(hooks.begin(), hooks.end(), target), hooks.end());
  }
  return result;
}

const FhModHost sHost{
    .structSize = static_cast<uint32_t>(sizeof(FhModHost)),
    .abiVersion = FH_MOD_ABI_VERSION,
    .modId = host_mod_id,
    .modDir = host_mod_dir,
    .log = host_log,
    .frameCount = host_frame_count,
    .classCount = host_class_count,
    .classReplaceCallback = host_class_replace_callback,
    .symbolAddress = host_symbol_address,
    .hookInstall = host_hook_install,
    .hookRemove = host_hook_remove,
};

void load_native(const ModEntry& mod) {
  const fs::path libraryPath = mod.root / "lib" / platform_directory() / library_name();
  std::error_code error;
  if (!fs::is_regular_file(libraryPath, error)) {
    return;
  }

  auto native = std::make_unique<NativeMod>();
  native->id = mod.id;
  native->dir = path_to_utf8(mod.root);

  std::string openError;
  native->handle = open_library(path_to_utf8(libraryPath), openError);
  if (native->handle == nullptr) {
    std::fprintf(stderr, "foxhollow: mods: %s: %s\n", mod.id.c_str(), openError.c_str());
    return;
  }

  auto initialize = reinterpret_cast<FhModInitializeFn>(find_symbol(native->handle, "fh_mod_initialize"));
  native->update = reinterpret_cast<FhModUpdateFn>(find_symbol(native->handle, "fh_mod_update"));
  native->shutdown = reinterpret_cast<FhModShutdownFn>(find_symbol(native->handle, "fh_mod_shutdown"));
  if (initialize == nullptr) {
    std::fprintf(stderr, "foxhollow: mods: %s: no fh_mod_initialize export\n", mod.id.c_str());
    close_library(native->handle);
    return;
  }

  NativeMod* raw = native.get();
  sNativeMods.push_back(std::move(native));
  if (initialize(reinterpret_cast<FhMod*>(raw), &sHost) != FH_MOD_OK) {
    std::fprintf(stderr, "foxhollow: mods: %s: fh_mod_initialize failed\n", mod.id.c_str());
    raw->update = nullptr;
    return;
  }
  std::fprintf(stderr, "foxhollow: mods: %s: code loaded from lib/%s\n", mod.id.c_str(), platform_directory());
}

std::vector<std::unique_ptr<OverlayEntry>> sOverlayEntries;
std::vector<AuroraOverlayFile> sOverlayFiles;
std::vector<aurora::texture::ReplacementGroup> sTextureGroups;
bool sOverlayRegistered = false;

int64_t seek_file(std::FILE* file, int64_t offset, int32_t whence) {
#if defined(_WIN32)
  if (_fseeki64(file, offset, whence) != 0) {
    return -1;
  }
  return _ftelli64(file);
#else
  if (fseeko(file, static_cast<off_t>(offset), whence) != 0) {
    return -1;
  }
  return static_cast<int64_t>(ftello(file));
#endif
}

void* overlay_open(void* userData) {
  const auto* entry = static_cast<const OverlayEntry*>(userData);
  if (entry == nullptr) {
    return nullptr;
  }
  return std::fopen(entry->filePath.c_str(), "rb");
}

void overlay_close(void* handle) {
  if (handle != nullptr) {
    std::fclose(static_cast<std::FILE*>(handle));
  }
}

int64_t overlay_read(void* handle, uint8_t* buf, size_t len) {
  if (handle == nullptr) {
    return -1;
  }
  return static_cast<int64_t>(std::fread(buf, 1, len, static_cast<std::FILE*>(handle)));
}

int64_t overlay_seek(void* handle, int64_t offset, int32_t whence) {
  if (handle == nullptr) {
    return -1;
  }
  return seek_file(static_cast<std::FILE*>(handle), offset, whence);
}

std::string path_to_utf8(const fs::path& path) {
  const auto generic = path.generic_u8string();
  return std::string(reinterpret_cast<const char*>(generic.data()), generic.size());
}

std::string to_lower(std::string value) {
  std::transform(value.begin(), value.end(), value.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return value;
}

bool is_hidden(const fs::path& path) {
  const auto name = path_to_utf8(path.filename());
  return !name.empty() && name.front() == '.';
}

bool has_hidden_component(const fs::path& path) {
  for (const auto& component : path) {
    if (is_hidden(component)) {
      return true;
    }
  }
  return false;
}

fs::path resolve_mods_root(int argc, char** argv, const char* userPath, bool& outExplicit) {
  outExplicit = true;

  for (int i = 1; i + 1 < argc; ++i) {
    if (std::strcmp(argv[i], "--mods") == 0) {
      return fs::path(argv[i + 1]);
    }
  }

  const char* env = std::getenv("FOXHOLLOW_MODS");
  if (env != nullptr && env[0] != '\0') {
    return fs::path(env);
  }

  outExplicit = false;
  if (userPath != nullptr && userPath[0] != '\0') {
    return fs::path(userPath) / "mods";
  }

  return {};
}

bool read_manifest(const fs::path& manifestPath, ModEntry& outMod) {
  std::FILE* file = std::fopen(path_to_utf8(manifestPath).c_str(), "rb");
  if (file == nullptr) {
    return false;
  }

  std::string contents;
  char buffer[4096];
  size_t read = 0;
  while ((read = std::fread(buffer, 1, sizeof(buffer), file)) > 0) {
    contents.append(buffer, read);
  }
  std::fclose(file);

  nlohmann::json manifest = nlohmann::json::parse(contents, nullptr, false);
  if (manifest.is_discarded() || !manifest.is_object()) {
    std::fprintf(stderr, "foxhollow: mods: %s is not valid JSON\n", path_to_utf8(manifestPath).c_str());
    return false;
  }

  if (!manifest.contains("id") || !manifest["id"].is_string()) {
    std::fprintf(stderr, "foxhollow: mods: %s has no \"id\"\n", path_to_utf8(manifestPath).c_str());
    return false;
  }

  if (manifest.contains("enabled") && manifest["enabled"].is_boolean() && !manifest["enabled"].get<bool>()) {
    return false;
  }

  outMod.id = manifest["id"].get<std::string>();
  outMod.name = manifest.value("name", outMod.id);
  outMod.version = manifest.value("version", std::string{});
  outMod.author = manifest.value("author", std::string{});
  return true;
}

std::vector<ModEntry> discover(const fs::path& root) {
  std::vector<ModEntry> mods;
  std::error_code error;

  fs::directory_iterator it(root, fs::directory_options::skip_permission_denied, error);
  if (error) {
    std::fprintf(stderr, "foxhollow: mods: cannot read %s: %s\n", path_to_utf8(root).c_str(), error.message().c_str());
    return mods;
  }

  std::vector<fs::path> directories;
  for (const auto& entry : it) {
    if (!entry.is_directory(error) || is_hidden(entry.path())) {
      continue;
    }
    if (!fs::exists(entry.path() / "mod.json", error)) {
      continue;
    }
    directories.push_back(entry.path());
  }

  std::sort(directories.begin(), directories.end(), [](const fs::path& a, const fs::path& b) {
    return to_lower(path_to_utf8(a.filename())) < to_lower(path_to_utf8(b.filename()));
  });

  for (const auto& directory : directories) {
    ModEntry mod;
    mod.root = directory;
    if (read_manifest(directory / "mod.json", mod)) {
      mods.push_back(std::move(mod));
    }
  }

  return mods;
}

void collect_overlay(const ModEntry& mod, std::vector<OverlayEntry>& collected) {
  const fs::path overlayRoot = mod.root / "overlay";
  std::error_code error;
  if (!fs::is_directory(overlayRoot, error)) {
    return;
  }

  fs::recursive_directory_iterator it(overlayRoot, fs::directory_options::skip_permission_denied, error);
  if (error) {
    return;
  }

  for (const auto& entry : it) {
    if (!entry.is_regular_file(error)) {
      continue;
    }

    const fs::path relative = fs::relative(entry.path(), overlayRoot, error);
    if (error || relative.empty() || has_hidden_component(relative)) {
      continue;
    }

    OverlayEntry overlay;
    overlay.discPath = "/" + path_to_utf8(relative);
    overlay.filePath = path_to_utf8(entry.path());
    overlay.ownerId = mod.id;
    overlay.size = entry.file_size(error);
    if (error) {
      std::fprintf(stderr, "foxhollow: mods: %s: cannot size %s\n", mod.id.c_str(), overlay.filePath.c_str());
      continue;
    }
    collected.push_back(std::move(overlay));
  }
}

void register_overlays(std::vector<OverlayEntry>& collected) {
  if (collected.empty()) {
    return;
  }

  std::vector<size_t> winners;
  std::unordered_map<std::string, size_t> byDiscPath;
  for (size_t i = 0; i < collected.size(); ++i) {
    const std::string key = to_lower(collected[i].discPath);
    const auto existing = byDiscPath.find(key);
    if (existing == byDiscPath.end()) {
      byDiscPath.emplace(key, winners.size());
      winners.push_back(i);
      continue;
    }
    std::fprintf(stderr, "foxhollow: mods: %s overrides %s for %s\n", collected[i].ownerId.c_str(),
                 collected[winners[existing->second]].ownerId.c_str(), collected[i].discPath.c_str());
    winners[existing->second] = i;
  }

  sOverlayEntries.reserve(winners.size());
  sOverlayFiles.reserve(winners.size());
  for (const size_t index : winners) {
    auto entry = std::make_unique<OverlayEntry>(std::move(collected[index]));
    AuroraOverlayFile file{};
    file.fileName = entry->discPath.c_str();
    file.userData = entry.get();
    file.size = static_cast<size_t>(entry->size);
    sOverlayFiles.push_back(file);
    sOverlayEntries.push_back(std::move(entry));
  }

  static const AuroraOverlayCallbacks callbacks{
      .open = overlay_open,
      .close = overlay_close,
      .read = overlay_read,
      .seek = overlay_seek,
  };
  aurora_dvd_overlay_callbacks(&callbacks);
  aurora_dvd_overlay_files(sOverlayFiles.data(), sOverlayFiles.size(), nullptr);
  sOverlayRegistered = true;
}

size_t register_textures(const ModEntry& mod, int32_t priority) {
  const fs::path texturesRoot = mod.root / "textures";
  std::error_code error;
  if (!fs::is_directory(texturesRoot, error)) {
    return 0;
  }

  aurora::texture::ReplacementOptions options{};
  options.priority = priority;
  auto group = aurora::texture::load_replacement_directory(texturesRoot, options);
  const size_t count = group.registrations.size();
  if (count > 0) {
    sTextureGroups.push_back(std::move(group));
  }
  return count;
}

} // namespace

extern "C" void fhModsInit(int argc, char** argv, const char* userPath) {
  bool explicitRoot = false;
  const fs::path root = resolve_mods_root(argc, argv, userPath, explicitRoot);
  if (root.empty()) {
    return;
  }

  std::error_code error;
  if (!fs::is_directory(root, error)) {
    if (explicitRoot) {
      std::fprintf(stderr, "foxhollow: mods: %s is not a directory\n", path_to_utf8(root).c_str());
    }
    return;
  }

  const std::vector<ModEntry> mods = discover(root);
  if (mods.empty()) {
    return;
  }

  std::vector<OverlayEntry> collected;
  size_t textureCount = 0;
  for (size_t i = 0; i < mods.size(); ++i) {
    const ModEntry& mod = mods[i];
    collect_overlay(mod, collected);
    const size_t textures = register_textures(mod, static_cast<int32_t>(i + 1));
    textureCount += textures;
    load_native(mod);
    std::fprintf(stderr, "foxhollow: mods: loaded %s%s%s\n", mod.name.c_str(), mod.version.empty() ? "" : " ",
                 mod.version.c_str());
  }

  register_overlays(collected);
  std::fprintf(stderr, "foxhollow: mods: %zu mod(s), %zu overlay file(s), %zu texture replacement(s) from %s\n",
               mods.size(), sOverlayFiles.size(), textureCount, path_to_utf8(root).c_str());
}

extern "C" void fhModsUpdate(void) {
  ++sFrameCount;
  for (const auto& native : sNativeMods) {
    if (native->update != nullptr) {
      native->update(reinterpret_cast<FhMod*>(native.get()));
    }
  }
}

extern "C" void fhModsShutdown(void) {
  for (auto it = sNativeMods.rbegin(); it != sNativeMods.rend(); ++it) {
    NativeMod* native = it->get();
    if (native->shutdown != nullptr) {
      native->shutdown(reinterpret_cast<FhMod*>(native));
    }
    for (void* target : native->hooks) {
      remove_hook(target);
    }
    native->hooks.clear();
    for (auto patch = native->patches.rbegin(); patch != native->patches.rend(); ++patch) {
      FhClassCallback* slots = class_slots(patch->classId);
      if (slots != nullptr) {
        slots[patch->slot] = patch->original;
      }
    }
    if (native->handle != nullptr) {
      close_library(native->handle);
    }
  }
  sNativeMods.clear();

  for (const auto& group : sTextureGroups) {
    aurora::texture::unregister_replacements(group);
  }
  sTextureGroups.clear();

  if (sOverlayRegistered) {
    aurora_dvd_overlay_files(nullptr, 0, nullptr);
    sOverlayRegistered = false;
  }
  sOverlayFiles.clear();
  sOverlayEntries.clear();
}
