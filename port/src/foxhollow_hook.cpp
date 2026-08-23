#include "foxhollow_hook.h"

#include <cstdint>
#include <cstring>
#include <string>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

#if defined(__APPLE__)
#include <libkern/OSCacheControl.h>
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach/mach.h>
#elif defined(__linux__)
#include <elf.h>
#include <fcntl.h>
#include <link.h>
#include <sys/stat.h>
#endif

namespace {

#if defined(__aarch64__) || defined(_M_ARM64)
#define FH_HOOK_ARCH_ARM64 1
#elif defined(__x86_64__) || defined(_M_X64)
#define FH_HOOK_ARCH_X86_64 1
#endif

#if defined(FH_HOOK_ARCH_ARM64)

constexpr uint32_t kArm64Nop = 0xd503201fu;

bool is_patch_pad(const uint8_t* bytes) {
  for (size_t offset = 0; offset < FH_HOOK_PATCH_BYTES; offset += 4) {
    uint32_t word;
    std::memcpy(&word, bytes + offset, 4);
    if (word != kArm64Nop) {
      return false;
    }
  }
  return true;
}

void write_branch(uint8_t* target, const void* destination) {
  const uint32_t ldr = 0x58000050u;
  const uint32_t br = 0xd61f0200u;
  const uint64_t address = reinterpret_cast<uint64_t>(destination);
  std::memcpy(target + 0, &ldr, 4);
  std::memcpy(target + 4, &br, 4);
  std::memcpy(target + 8, &address, 8);
}

void write_pad(uint8_t* target) {
  const uint32_t nops[4] = {kArm64Nop, kArm64Nop, kArm64Nop, kArm64Nop};
  std::memcpy(target, nops, sizeof(nops));
}

#elif defined(FH_HOOK_ARCH_X86_64)

size_t decode_nop(const uint8_t* bytes, size_t available) {
  size_t index = 0;
  while (index < available && (bytes[index] == 0x66 || bytes[index] == 0x2e)) {
    ++index;
  }
  if (index >= available) {
    return 0;
  }
  if (bytes[index] == 0x90) {
    return index + 1;
  }
  if (index + 2 < available && bytes[index] == 0x0f && bytes[index + 1] == 0x1f) {
    const uint8_t modrm = bytes[index + 2];
    size_t length = index + 3;
    const uint8_t mod = static_cast<uint8_t>(modrm >> 6);
    const uint8_t rm = static_cast<uint8_t>(modrm & 0x07);
    if (rm == 0x04) {
      ++length;
    }
    if (mod == 0x01) {
      length += 1;
    } else if (mod == 0x02) {
      length += 4;
    } else if (mod == 0x00 && rm == 0x05) {
      length += 4;
    }
    return length <= available ? length : 0;
  }
  return 0;
}

bool is_patch_pad(const uint8_t* bytes) {
  size_t covered = 0;
  while (covered < FH_HOOK_PATCH_BYTES) {
    const size_t length = decode_nop(bytes + covered, FH_HOOK_PATCH_BYTES - covered);
    if (length == 0) {
      return false;
    }
    covered += length;
  }
  return covered == FH_HOOK_PATCH_BYTES;
}

void write_branch(uint8_t* target, const void* destination) {
  const uint8_t jump[6] = {0xff, 0x25, 0x00, 0x00, 0x00, 0x00};
  const uint64_t address = reinterpret_cast<uint64_t>(destination);
  std::memcpy(target, jump, sizeof(jump));
  std::memcpy(target + sizeof(jump), &address, sizeof(address));
  std::memset(target + sizeof(jump) + sizeof(address), 0x90,
              FH_HOOK_PATCH_BYTES - sizeof(jump) - sizeof(address));
}

void write_pad(uint8_t* target) {
  const uint8_t nop15[15] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x2e, 0x66, 0x0f,
                             0x1f, 0x84, 0x00, 0x00, 0x02, 0x00, 0x00};
  std::memcpy(target, nop15, sizeof(nop15));
  target[15] = 0x90;
}

#else

bool is_patch_pad(const uint8_t*) { return false; }
void write_branch(uint8_t*, const void*) {}
void write_pad(uint8_t*) {}

#endif

#if defined(_WIN32)

bool unprotect_range(void* address, size_t size, DWORD* outPrevious) {
  return VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, outPrevious) != 0;
}

void reprotect_range(void* address, size_t size, DWORD previous) {
  DWORD ignored = 0;
  VirtualProtect(address, size, previous, &ignored);
  FlushInstructionCache(GetCurrentProcess(), address, size);
}

#else

size_t page_size() { return static_cast<size_t>(getpagesize()); }

uintptr_t page_floor(void* address) {
  return reinterpret_cast<uintptr_t>(address) & ~static_cast<uintptr_t>(page_size() - 1);
}

uintptr_t page_ceil(void* address, size_t size) {
  const size_t granularity = page_size();
  return (reinterpret_cast<uintptr_t>(address) + size + granularity - 1) & ~static_cast<uintptr_t>(granularity - 1);
}

bool unprotect_range(void* address, size_t size) {
  const uintptr_t start = page_floor(address);
  const size_t span = static_cast<size_t>(page_ceil(address, size) - start);
#if defined(__APPLE__)
  if (vm_protect(mach_task_self(), static_cast<vm_address_t>(start), static_cast<vm_size_t>(span), FALSE,
                 VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY) == KERN_SUCCESS) {
    return true;
  }
#endif
  if (mprotect(reinterpret_cast<void*>(start), span, PROT_READ | PROT_WRITE | PROT_EXEC) == 0) {
    return true;
  }
  return mprotect(reinterpret_cast<void*>(start), span, PROT_READ | PROT_WRITE) == 0;
}

void reprotect_range(void* address, size_t size) {
  const uintptr_t start = page_floor(address);
  const size_t span = static_cast<size_t>(page_ceil(address, size) - start);
  mprotect(reinterpret_cast<void*>(start), span, PROT_READ | PROT_EXEC);
#if defined(__APPLE__)
  sys_icache_invalidate(address, size);
#else
  __builtin___clear_cache(static_cast<char*>(address), static_cast<char*>(address) + size);
#endif
}

#endif

#if defined(__APPLE__)

void* resolve_in_image(const std::string& decorated) {
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

#elif defined(__linux__)

int load_bias_callback(struct dl_phdr_info* info, size_t, void* data) {
  if (info->dlpi_name == nullptr || info->dlpi_name[0] == '\0') {
    *static_cast<uintptr_t*>(data) = static_cast<uintptr_t>(info->dlpi_addr);
    return 1;
  }
  return 0;
}

uintptr_t executable_load_bias() {
  uintptr_t bias = 0;
  dl_iterate_phdr(load_bias_callback, &bias);
  return bias;
}

void* resolve_in_image(const std::string& name) {
  const int fd = open("/proc/self/exe", O_RDONLY);
  if (fd < 0) {
    return nullptr;
  }
  struct stat status;
  if (fstat(fd, &status) != 0 || status.st_size <= 0) {
    close(fd);
    return nullptr;
  }
  const size_t size = static_cast<size_t>(status.st_size);
  void* mapping = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
  close(fd);
  if (mapping == MAP_FAILED) {
    return nullptr;
  }

  void* result = nullptr;
  const auto* base = static_cast<const uint8_t*>(mapping);
  const auto* header = reinterpret_cast<const ElfW(Ehdr)*>(base);
  if (std::memcmp(header->e_ident, ELFMAG, SELFMAG) == 0 && header->e_shoff != 0 && header->e_shentsize != 0) {
    const auto* sections = reinterpret_cast<const ElfW(Shdr)*>(base + header->e_shoff);
    const uintptr_t bias = executable_load_bias();
    for (uint16_t i = 0; i < header->e_shnum && result == nullptr; ++i) {
      if (sections[i].sh_type != SHT_SYMTAB && sections[i].sh_type != SHT_DYNSYM) {
        continue;
      }
      if (sections[i].sh_link >= header->e_shnum || sections[i].sh_entsize == 0) {
        continue;
      }
      const auto* symbols = reinterpret_cast<const ElfW(Sym)*>(base + sections[i].sh_offset);
      const char* strings = reinterpret_cast<const char*>(base + sections[sections[i].sh_link].sh_offset);
      const size_t count = static_cast<size_t>(sections[i].sh_size / sections[i].sh_entsize);
      for (size_t index = 0; index < count; ++index) {
        const ElfW(Sym)& symbol = symbols[index];
        if (symbol.st_value == 0 || ELF32_ST_TYPE(symbol.st_info) != STT_FUNC) {
          continue;
        }
        if (name == (strings + symbol.st_name)) {
          result = reinterpret_cast<void*>(static_cast<uintptr_t>(symbol.st_value) + bias);
          break;
        }
      }
    }
  }
  munmap(mapping, size);
  return result;
}

#elif defined(_WIN32)

void* resolve_in_image(const std::string& name) {
  HMODULE module = GetModuleHandleA(nullptr);
  if (module == nullptr) {
    return nullptr;
  }
  return reinterpret_cast<void*>(GetProcAddress(module, name.c_str()));
}

#else

void* resolve_in_image(const std::string&) { return nullptr; }

#endif

} // namespace

extern "C" int fhHookTargetIsPatchable(const void* target) {
  if (target == nullptr) {
    return 0;
  }
  uint8_t bytes[FH_HOOK_PATCH_BYTES];
  std::memcpy(bytes, target, sizeof(bytes));
  return is_patch_pad(bytes) ? 1 : 0;
}

extern "C" int fhHookInstall(void* target, void* replacement, void** outOriginal) {
  if (target == nullptr || replacement == nullptr || !fhHookTargetIsPatchable(target)) {
    return 1;
  }
#if defined(_WIN32)
  DWORD previous = 0;
  if (!unprotect_range(target, FH_HOOK_PATCH_BYTES, &previous)) {
    return 1;
  }
  write_branch(static_cast<uint8_t*>(target), replacement);
  reprotect_range(target, FH_HOOK_PATCH_BYTES, previous);
#else
  if (!unprotect_range(target, FH_HOOK_PATCH_BYTES)) {
    return 1;
  }
  write_branch(static_cast<uint8_t*>(target), replacement);
  reprotect_range(target, FH_HOOK_PATCH_BYTES);
#endif
  if (outOriginal != nullptr) {
    *outOriginal = static_cast<uint8_t*>(target) + FH_HOOK_PATCH_BYTES;
  }
  return 0;
}

extern "C" int fhHookRemove(void* target) {
  if (target == nullptr) {
    return 1;
  }
#if defined(_WIN32)
  DWORD previous = 0;
  if (!unprotect_range(target, FH_HOOK_PATCH_BYTES, &previous)) {
    return 1;
  }
  write_pad(static_cast<uint8_t*>(target));
  reprotect_range(target, FH_HOOK_PATCH_BYTES, previous);
#else
  if (!unprotect_range(target, FH_HOOK_PATCH_BYTES)) {
    return 1;
  }
  write_pad(static_cast<uint8_t*>(target));
  reprotect_range(target, FH_HOOK_PATCH_BYTES);
#endif
  return 0;
}

extern "C" void* fhHookResolveSymbol(const char* name) {
  if (name == nullptr || name[0] == '\0') {
    return nullptr;
  }
#if defined(__APPLE__)
  return resolve_in_image(std::string("_") + name);
#else
  return resolve_in_image(std::string(name));
#endif
}
