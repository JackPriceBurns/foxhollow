#pragma once

#include <cstdint>

namespace borealis::crash::detail {

struct ModuleInfo {
    uintptr_t base = 0;
    uintptr_t size = 0;
    char path[1024] = {};
    uint8_t buildId[64] = {};
    unsigned buildIdLen = 0;
    unsigned pdbAge = 0;
};

struct Report {
    const char* reason = nullptr;
    unsigned long long code = 0;
    bool hasCode = false;
    uintptr_t faultAddress = 0;
    uintptr_t crashPc = 0;
    bool crashPcKnown = false;
    const uintptr_t* frames = nullptr;
    int frameCount = 0;
};

using ModuleResolver = bool (*)(uintptr_t pc, ModuleInfo& info, void* userData);
using SymbolResolver = const char* (*)(uintptr_t pc, unsigned long long* displacement,
    void* userData);

void emit_report(int fd, const ModuleInfo& executable, const Report& report,
    ModuleResolver moduleResolver, SymbolResolver symbolResolver, void* userData);

}  // namespace borealis::crash::detail
