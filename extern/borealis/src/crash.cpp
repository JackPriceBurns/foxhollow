#if !defined(_WIN32) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif

#include "borealis/crash.hpp"

#include "borealis/log.hpp"
#include "borealis/version.h"
#include "crash_report.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>

#if defined(_WIN32)

#include <windows.h>

#include <io.h>

#if defined(BOREALIS_CRASH_DBGHELP)
#include <dbghelp.h>
#endif

#else

#include <csignal>
#include <cstdlib>
#include <dlfcn.h>
#include <sys/ucontext.h>
#include <unistd.h>
#include <unwind.h>

#if defined(__APPLE__)
#include <TargetConditionals.h>
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#else
#include <elf.h>
#include <link.h>
#ifndef NT_GNU_BUILD_ID
#define NT_GNU_BUILD_ID 3
#endif
#endif

#endif

namespace borealis::crash {
namespace {

constexpr int kStderrFd = 2;
constexpr int kMaxFrames = 128;
constexpr char kHexDigits[] = "0123456789abcdef";

using detail::ModuleInfo;

ModuleInfo g_executable;

void raw_write(int fd, const char* data, size_t len) {
    if (fd < 0) {
        return;
    }
#if defined(_WIN32)
    _write(fd, data, static_cast<unsigned int>(len));
#else
    while (len > 0) {
        const ssize_t written = ::write(fd, data, len);
        if (written <= 0) {
            return;
        }
        data += written;
        len -= static_cast<size_t>(written);
    }
#endif
}

void write_str(int fd, const char* s) {
    if (s != nullptr) {
        raw_write(fd, s, std::strlen(s));
    }
}

void write_hex(int fd, unsigned long long value) {
    char buf[2 + 16];
    size_t o = sizeof(buf);
    do {
        buf[--o] = kHexDigits[value & 0xF];
        value >>= 4;
    } while (value != 0);
    buf[--o] = 'x';
    buf[--o] = '0';
    raw_write(fd, buf + o, sizeof(buf) - o);
}

void write_dec(int fd, unsigned int value) {
    char buf[10];
    size_t o = sizeof(buf);
    do {
        buf[--o] = static_cast<char>('0' + value % 10);
        value /= 10;
    } while (value != 0);
    raw_write(fd, buf + o, sizeof(buf) - o);
}

void write_hex_bytes(int fd, const uint8_t* data, unsigned len) {
    char buf[2];
    for (unsigned i = 0; i < len; ++i) {
        buf[0] = kHexDigits[data[i] >> 4];
        buf[1] = kHexDigits[data[i] & 0xF];
        raw_write(fd, buf, 2);
    }
}

void write_hex_byte(int fd, uint8_t value) {
    char buf[2];
    buf[0] = kHexDigits[value >> 4];
    buf[1] = kHexDigits[value & 0xF];
    raw_write(fd, buf, 2);
}

void write_quoted(int fd, const char* s) {
    write_str(fd, "\"");
    if (s != nullptr) {
        for (const char* p = s; *p != '\0'; ++p) {
            if (*p == '"' || *p == '\\') {
                raw_write(fd, "\\", 1);
            }
            raw_write(fd, p, 1);
        }
    }
    write_str(fd, "\"");
}

const char* base_name(const char* path) {
    const char* name = path;
    for (const char* p = path; p != nullptr && *p != '\0'; ++p) {
        if (*p == '/' || *p == '\\') {
            name = p + 1;
        }
    }
    return name[0] != '\0' ? name : "(unknown)";
}

void write_build_id(int fd, const uint8_t* buildId, unsigned buildIdLen, unsigned pdbAge) {
    if (buildIdLen == 0) {
        write_str(fd, "(unavailable)");
        return;
    }
#if defined(_WIN32)
    if (buildIdLen == 16) {
        write_hex_byte(fd, buildId[3]);
        write_hex_byte(fd, buildId[2]);
        write_hex_byte(fd, buildId[1]);
        write_hex_byte(fd, buildId[0]);
        write_str(fd, "-");
        write_hex_byte(fd, buildId[5]);
        write_hex_byte(fd, buildId[4]);
        write_str(fd, "-");
        write_hex_byte(fd, buildId[7]);
        write_hex_byte(fd, buildId[6]);
        write_str(fd, "-");
        write_hex_byte(fd, buildId[8]);
        write_hex_byte(fd, buildId[9]);
        write_str(fd, "-");
        write_hex_bytes(fd, buildId + 10, 6);
        if (pdbAge != 0) {
            write_str(fd, "-");
            write_dec(fd, pdbAge);
        }
        return;
    }
#else
    (void)pdbAge;
#endif
    write_hex_bytes(fd, buildId, buildIdLen);
}

const char* symbol_for(uintptr_t pc, unsigned long long* disp, void*) {
#if defined(_WIN32) && defined(BOREALIS_CRASH_DBGHELP)
    alignas(SYMBOL_INFO) static char storage[sizeof(SYMBOL_INFO) + 512];
    auto* sym = reinterpret_cast<SYMBOL_INFO*>(storage);
    sym->SizeOfStruct = sizeof(SYMBOL_INFO);
    sym->MaxNameLen = 511;
    DWORD64 d = 0;
    if (SymFromAddr(GetCurrentProcess(), pc, &d, sym)) {
        *disp = d;
        return sym->Name;
    }
    return nullptr;
#elif defined(_WIN32)
    (void)pc;
    (void)disp;
    return nullptr;
#else
    Dl_info info;
    if (dladdr(reinterpret_cast<void*>(pc), &info) != 0 && info.dli_sname != nullptr) {
        const auto base = reinterpret_cast<uintptr_t>(info.dli_saddr);
        *disp = pc >= base ? pc - base : 0;
        return info.dli_sname;
    }
    return nullptr;
#endif
}

bool find_module_info(uintptr_t pc, ModuleInfo& info, void*);

void resolve_module_info(uintptr_t pc, const ModuleInfo& executable,
    detail::ModuleResolver moduleResolver, void* userData, ModuleInfo& info) {
    info = executable;
    if (moduleResolver == nullptr) {
        return;
    }
    ModuleInfo resolved;
    if (moduleResolver(pc, resolved, userData)) {
        info = resolved;
    }
}

void emit_address_detail(int fd, uintptr_t pc, const ModuleInfo& executable,
    detail::ModuleResolver moduleResolver, void* userData) {
    ModuleInfo info;
    resolve_module_info(pc, executable, moduleResolver, userData, info);
    const uintptr_t rva = pc >= info.base ? pc - info.base : 0ull;
    write_hex(fd, pc);
    write_str(fd, " module_base=");
    write_hex(fd, info.base);
    if (info.size != 0) {
        write_str(fd, " image_size=");
        write_hex(fd, info.size);
    }
    write_str(fd, " rva=");
    write_hex(fd, rva);
    write_str(fd, " module=");
    write_quoted(fd, info.path[0] != '\0' ? info.path : base_name(executable.path));
    write_str(fd, " build_id=");
    write_build_id(fd, info.buildId, info.buildIdLen, info.pdbAge);
}

void emit_frame(int fd, int index, uintptr_t pc, const ModuleInfo& executable,
    detail::ModuleResolver moduleResolver, detail::SymbolResolver symbolResolver, void* userData) {
    ModuleInfo info;
    resolve_module_info(pc, executable, moduleResolver, userData, info);
    const uintptr_t rva = pc >= info.base ? pc - info.base : 0ull;

    write_str(fd, "#");
    if (index < 10) {
        write_str(fd, "0");
    }
    write_dec(fd, static_cast<unsigned int>(index));
    write_str(fd, " abs=");
    write_hex(fd, pc);
    write_str(fd, " module_base=");
    write_hex(fd, info.base);
    if (info.size != 0) {
        write_str(fd, " image_size=");
        write_hex(fd, info.size);
    }
    write_str(fd, " rva=");
    write_hex(fd, rva);
    write_str(fd, " module=");
    write_quoted(fd, info.path[0] != '\0' ? info.path : base_name(executable.path));
    write_str(fd, " build_id=");
    write_build_id(fd, info.buildId, info.buildIdLen, info.pdbAge);
    unsigned long long disp = 0;
    const char* sym = symbolResolver != nullptr ? symbolResolver(pc, &disp, userData) : nullptr;
    if (sym != nullptr && sym[0] != '\0') {
        write_str(fd, " ");
        write_str(fd, sym);
        write_str(fd, "+");
        write_hex(fd, disp);
    }
    write_str(fd, "\n");
}

void emit_header(int fd, const ModuleInfo& executable, const detail::Report& report,
    detail::ModuleResolver moduleResolver, void* userData) {
    write_str(fd, "\n==================== APPLICATION CRASHED ====================\n");
    write_str(fd, "Build:       " BOREALIS_APP_DESCRIBE " (" BOREALIS_APP_BRANCH ")\n");
    write_str(fd, "Revision:    " BOREALIS_APP_REVISION "  Date: " BOREALIS_APP_DATE
                  "  Type: " BOREALIS_BUILD_TYPE "\n");
    write_str(fd, "Platform:    " BOREALIS_PLATFORM_NAME " / " BOREALIS_ARCH "\n");
    write_str(fd, "Module:      ");
    write_str(fd, executable.path[0] != '\0' ? executable.path : "(unknown)");
    write_str(fd, "\nModule base: ");
    write_hex(fd, executable.base);
    write_str(fd, "\nBuild-ID:    ");
    write_build_id(fd, executable.buildId, executable.buildIdLen, executable.pdbAge);
    write_str(fd, "\nReason:      ");
    write_str(fd, report.reason);
    if (report.hasCode) {
        write_str(fd, " (");
        write_hex(fd, report.code);
        write_str(fd, ")");
    }
    write_str(fd, "\nFault addr:  ");
    write_hex(fd, report.faultAddress);
    write_str(fd, "\nCrash PC:    ");
    if (report.crashPcKnown) {
        emit_address_detail(fd, report.crashPc, executable, moduleResolver, userData);
    } else {
        write_str(fd, "(unavailable on this platform)");
    }
    write_str(fd, "\n");
    write_str(fd, "Backtrace:\n");
}

void emit_footer(int fd) {
    write_str(fd, "========================================================\n");
}

#if defined(_WIN32)

LONG g_inHandler = 0;
LPTOP_LEVEL_EXCEPTION_FILTER g_prevFilter = nullptr;

bool read_pe_module_info(uintptr_t moduleBase, ModuleInfo& info) {
    const auto* base = reinterpret_cast<const uint8_t*>(moduleBase);
    if (base == nullptr) {
        return false;
    }
    const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
        return false;
    }
    const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) {
        return false;
    }
    info.base = moduleBase;
    info.size = nt->OptionalHeader.SizeOfImage;
    const IMAGE_DATA_DIRECTORY& dir = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG];
    if (dir.VirtualAddress == 0 || dir.Size == 0) {
        return true;
    }
    const auto* dbg = reinterpret_cast<const IMAGE_DEBUG_DIRECTORY*>(base + dir.VirtualAddress);
    const unsigned count = dir.Size / sizeof(IMAGE_DEBUG_DIRECTORY);
    for (unsigned i = 0; i < count; ++i) {
        if (dbg[i].Type != IMAGE_DEBUG_TYPE_CODEVIEW) {
            continue;
        }
        const auto* cv = base + dbg[i].AddressOfRawData;
        if (std::memcmp(cv, "RSDS", 4) != 0) {
            continue;
        }
        std::memcpy(info.buildId, cv + 4, sizeof(GUID));
        info.buildIdLen = sizeof(GUID);
        std::memcpy(&info.pdbAge, cv + 4 + sizeof(GUID), sizeof(info.pdbAge));
        break;
    }
    return true;
}

void capture_build_id() {
    ModuleInfo info;
    if (!read_pe_module_info(g_executable.base, info)) {
        return;
    }
    g_executable.buildIdLen = info.buildIdLen;
    if (g_executable.buildIdLen != 0) {
        std::memcpy(g_executable.buildId, info.buildId, g_executable.buildIdLen);
    }
    g_executable.pdbAge = info.pdbAge;
}

bool find_module_info(uintptr_t pc, ModuleInfo& info, void*) {
    info = {};
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(reinterpret_cast<LPCVOID>(pc), &mbi, sizeof(mbi)) == 0 ||
        mbi.AllocationBase == nullptr)
    {
        return false;
    }
    const auto moduleBase = reinterpret_cast<uintptr_t>(mbi.AllocationBase);
    info = {};
    info.base = moduleBase;
    GetModuleFileNameA(reinterpret_cast<HMODULE>(moduleBase), info.path,
        static_cast<DWORD>(sizeof(info.path) - 1));
    read_pe_module_info(moduleBase, info);
    return true;
}

const char* exception_name(DWORD code) {
    switch (code) {
    case EXCEPTION_ACCESS_VIOLATION:
        return "EXCEPTION_ACCESS_VIOLATION";
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        return "EXCEPTION_ILLEGAL_INSTRUCTION";
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        return "EXCEPTION_INT_DIVIDE_BY_ZERO";
    case EXCEPTION_STACK_OVERFLOW:
        return "EXCEPTION_STACK_OVERFLOW";
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        return "EXCEPTION_DATATYPE_MISALIGNMENT";
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
    default:
        return "EXCEPTION";
    }
}

int capture_backtrace_win(CONTEXT ctx, uintptr_t* out, int cap) {
    int n = 0;
    while (n < cap) {
#if defined(_M_X64)
        const DWORD64 ip = ctx.Rip;
#elif defined(_M_ARM64)
        const DWORD64 ip = ctx.Pc;
#else
        const DWORD64 ip = 0;
#endif
        if (ip == 0) {
            break;
        }
        out[n++] = static_cast<uintptr_t>(ip);
#if defined(_M_X64) || defined(_M_ARM64)
        DWORD64 imageBase = 0;
        PRUNTIME_FUNCTION fn = RtlLookupFunctionEntry(ip, &imageBase, nullptr);
        if (fn != nullptr) {
            PVOID handlerData = nullptr;
            DWORD64 establisherFrame = 0;
            RtlVirtualUnwind(UNW_FLAG_NHANDLER, imageBase, ip, fn, &ctx, &handlerData,
                &establisherFrame, nullptr);
            continue;
        }
#if defined(_M_X64)
        if (ctx.Rsp == 0) {
            break;
        }
        ctx.Rip = *reinterpret_cast<const DWORD64*>(ctx.Rsp);
        ctx.Rsp += sizeof(DWORD64);
#else
        if (ctx.Lr == 0 || ctx.Lr == ip) {
            break;
        }
        ctx.Pc = ctx.Lr;
        ctx.Lr = 0;
#endif
#else
        break;
#endif
    }
    return n;
}

void emit(int fd, EXCEPTION_POINTERS* ep) {
    if (fd < 0) {
        return;
    }

    const DWORD code = ep->ExceptionRecord->ExceptionCode;
    const uintptr_t pc = reinterpret_cast<uintptr_t>(ep->ExceptionRecord->ExceptionAddress);
    uintptr_t faultAddr = 0;
    if (code == EXCEPTION_ACCESS_VIOLATION && ep->ExceptionRecord->NumberParameters >= 2) {
        faultAddr = static_cast<uintptr_t>(ep->ExceptionRecord->ExceptionInformation[1]);
    }

    uintptr_t frames[kMaxFrames];
    const int frameCount = capture_backtrace_win(*ep->ContextRecord, frames, kMaxFrames);
    const detail::Report report{
        .reason = exception_name(code),
        .code = code,
        .hasCode = true,
        .faultAddress = faultAddr,
        .crashPc = pc,
        .crashPcKnown = true,
        .frames = frames,
        .frameCount = frameCount,
    };
    detail::emit_report(fd, g_executable, report, &find_module_info, &symbol_for, nullptr);
}

LONG WINAPI windows_handler(EXCEPTION_POINTERS* ep) {
    if (InterlockedCompareExchange(&g_inHandler, 1, 0) != 0) {
        return EXCEPTION_CONTINUE_SEARCH;
    }
    emit(kStderrFd, ep);
    const int logFd = borealis::log::file_descriptor();
    if (logFd >= 0) {
        emit(logFd, ep);
    }
    if (g_prevFilter != nullptr) {
        return g_prevFilter(ep);
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

#else

constexpr int kSignals[] = {SIGSEGV, SIGBUS, SIGABRT, SIGILL, SIGFPE};
constexpr int kSignalCount = static_cast<int>(sizeof(kSignals) / sizeof(kSignals[0]));
constexpr int kAltStackSize = 128 * 1024;

volatile std::sig_atomic_t g_inHandler = 0;
char g_altStack[kAltStackSize];
struct sigaction g_prev[kSignalCount];
std::terminate_handler g_prevTerminate = nullptr;

void crash_regs(void* ucv, uintptr_t& pc, uintptr_t& lr, uintptr_t& fp) {
    pc = 0;
    lr = 0;
    fp = 0;
    if (ucv == nullptr) {
        return;
    }
    auto* uc = static_cast<ucontext_t*>(ucv);
#if defined(__APPLE__)
#if defined(__aarch64__) || defined(__arm64__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext->__ss.__pc);
    lr = static_cast<uintptr_t>(uc->uc_mcontext->__ss.__lr);
    fp = static_cast<uintptr_t>(uc->uc_mcontext->__ss.__fp);
#elif defined(__x86_64__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext->__ss.__rip);
    fp = static_cast<uintptr_t>(uc->uc_mcontext->__ss.__rbp);
#endif
#elif defined(__ANDROID__)
#if defined(__aarch64__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext.pc);
    lr = static_cast<uintptr_t>(uc->uc_mcontext.regs[30]);
    fp = static_cast<uintptr_t>(uc->uc_mcontext.regs[29]);
#elif defined(__x86_64__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_RIP]);
    fp = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_RBP]);
#elif defined(__arm__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext.arm_pc);
    lr = static_cast<uintptr_t>(uc->uc_mcontext.arm_lr);
    fp = static_cast<uintptr_t>(uc->uc_mcontext.arm_fp);
#elif defined(__i386__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_EIP]);
    fp = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_EBP]);
#endif
#elif defined(__linux__)
#if defined(__x86_64__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_RIP]);
    fp = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_RBP]);
#elif defined(__aarch64__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext.pc);
    lr = static_cast<uintptr_t>(uc->uc_mcontext.regs[30]);
    fp = static_cast<uintptr_t>(uc->uc_mcontext.regs[29]);
#elif defined(__i386__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_EIP]);
    fp = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_EBP]);
#endif
#endif
}

bool pc_near_function_entry(uintptr_t pc) {
    constexpr uintptr_t kPrologueWindow = 20;
    Dl_info info;
    if (dladdr(reinterpret_cast<void*>(pc), &info) == 0 || info.dli_saddr == nullptr) {
        return false;
    }
    const auto start = reinterpret_cast<uintptr_t>(info.dli_saddr);
    return pc >= start && pc - start <= kPrologueWindow;
}

int capture_backtrace_fp(uintptr_t pc, uintptr_t lr, uintptr_t fp, uintptr_t* out, int cap) {
    int n = 0;
    if (pc != 0 && n < cap) {
        out[n++] = pc;
    }
    bool dedupeLr = false;
    if (lr != 0 && lr != pc && n < cap && pc_near_function_entry(pc)) {
        out[n++] = lr;
        dedupeLr = true;
    }
    uintptr_t cur = fp;
    uintptr_t prev = 0;
    constexpr uintptr_t kMaxFrameSpan = 16u << 20;
    while (n < cap) {
        if (cur == 0 || (cur & (sizeof(uintptr_t) - 1)) != 0 || cur <= prev) {
            break;
        }
        const auto* slot = reinterpret_cast<const uintptr_t*>(cur);
        const uintptr_t next = slot[0];
        const uintptr_t ret = slot[1];
        if (ret == 0) {
            break;
        }
        const bool skip = dedupeLr && ret == lr;
        dedupeLr = false;
        if (!skip) {
            out[n++] = ret;
        }
        if (next != 0 && next > cur && next - cur > kMaxFrameSpan) {
            break;
        }
        prev = cur;
        cur = next;
    }
    return n;
}

struct UnwindState {
    uintptr_t* pcs;
    int count;
    int cap;
    int skip;
};

_Unwind_Reason_Code unwind_cb(struct _Unwind_Context* ctx, void* arg) {
    auto* s = static_cast<UnwindState*>(arg);
    const uintptr_t ip = static_cast<uintptr_t>(_Unwind_GetIP(ctx));
    if (ip == 0) {
        return _URC_END_OF_STACK;
    }
    if (s->skip > 0) {
        --s->skip;
        return _URC_NO_REASON;
    }
    if (s->count >= s->cap) {
        return _URC_END_OF_STACK;
    }
    s->pcs[s->count++] = ip;
    return _URC_NO_REASON;
}

int capture_backtrace(uintptr_t* pcs, int cap, int skip) {
    UnwindState s{pcs, 0, cap, skip};
    _Unwind_Backtrace(&unwind_cb, &s);
    return s.count;
}

void prewarm_unwinder() {
    uintptr_t warm[4];
    capture_backtrace(warm, 4, 0);
}

#if defined(__APPLE__)

bool read_mach_build_id(uintptr_t moduleBase, ModuleInfo& info) {
    const auto* header = reinterpret_cast<const struct mach_header_64*>(moduleBase);
    if (header == nullptr || header->magic != MH_MAGIC_64) {
        return false;
    }
    const auto* lc = reinterpret_cast<const struct load_command*>(
        reinterpret_cast<const char*>(header) + sizeof(struct mach_header_64));
    for (uint32_t i = 0; i < header->ncmds; ++i) {
        if (lc->cmd == LC_UUID) {
            const auto* uuid = reinterpret_cast<const struct uuid_command*>(lc);
            std::memcpy(info.buildId, uuid->uuid, sizeof(uuid->uuid));
            info.buildIdLen = sizeof(uuid->uuid);
            return true;
        }
        lc = reinterpret_cast<const struct load_command*>(
            reinterpret_cast<const char*>(lc) + lc->cmdsize);
    }
    return true;
}

void capture_build_id() {
    ModuleInfo info;
    if (!read_mach_build_id(g_executable.base, info)) {
        return;
    }
    g_executable.buildIdLen = info.buildIdLen;
    if (g_executable.buildIdLen != 0) {
        std::memcpy(g_executable.buildId, info.buildId, g_executable.buildIdLen);
    }
}

#else

bool segment_contains(const dl_phdr_info* info, uintptr_t addr) {
    for (int i = 0; i < info->dlpi_phnum; ++i) {
        const ElfW(Phdr) & ph = info->dlpi_phdr[i];
        if (ph.p_type != PT_LOAD) {
            continue;
        }
        const uintptr_t start = info->dlpi_addr + ph.p_vaddr;
        if (addr >= start && addr < start + ph.p_memsz) {
            return true;
        }
    }
    return false;
}

void read_elf_module_info(const dl_phdr_info* info, ModuleInfo& module) {
    uintptr_t minAddr = ~static_cast<uintptr_t>(0);
    uintptr_t maxAddr = 0;
    for (int i = 0; i < info->dlpi_phnum; ++i) {
        const ElfW(Phdr) & ph = info->dlpi_phdr[i];
        if (ph.p_type != PT_LOAD) {
            continue;
        }
        const uintptr_t start = info->dlpi_addr + ph.p_vaddr;
        const uintptr_t end = start + ph.p_memsz;
        if (start < minAddr) {
            minAddr = start;
        }
        if (end > maxAddr) {
            maxAddr = end;
        }
    }
    if (minAddr <= maxAddr && maxAddr != 0) {
        module.base = minAddr;
        module.size = maxAddr - minAddr;
    }

    for (int i = 0; i < info->dlpi_phnum; ++i) {
        const ElfW(Phdr) & ph = info->dlpi_phdr[i];
        if (ph.p_type != PT_NOTE) {
            continue;
        }
        const auto* p = reinterpret_cast<const uint8_t*>(info->dlpi_addr + ph.p_vaddr);
        const uint8_t* end = p + ph.p_memsz;
        while (p + sizeof(ElfW(Nhdr)) <= end) {
            const auto* nh = reinterpret_cast<const ElfW(Nhdr)*>(p);
            const char* name = reinterpret_cast<const char*>(nh + 1);
            const uint8_t* desc =
                reinterpret_cast<const uint8_t*>(name + ((nh->n_namesz + 3) & ~3u));
            if (nh->n_type == NT_GNU_BUILD_ID && nh->n_namesz == 4 &&
                std::memcmp(name, "GNU", 4) == 0)
            {
                unsigned n = nh->n_descsz;
                if (n > sizeof(module.buildId)) {
                    n = sizeof(module.buildId);
                }
                std::memcpy(module.buildId, desc, n);
                module.buildIdLen = n;
                return;
            }
            p = desc + ((nh->n_descsz + 3) & ~3u);
        }
    }
}

int elf_build_id_callback(dl_phdr_info* info, size_t, void* arg) {
    const auto self = *static_cast<const uintptr_t*>(arg);
    if (!segment_contains(info, self)) {
        return 0;
    }
    ModuleInfo module;
    read_elf_module_info(info, module);
    g_executable.buildIdLen = module.buildIdLen;
    if (g_executable.buildIdLen != 0) {
        std::memcpy(g_executable.buildId, module.buildId, g_executable.buildIdLen);
    }
    return 1;
}

void capture_build_id() {
    uintptr_t self = reinterpret_cast<uintptr_t>(&install);
    dl_iterate_phdr(&elf_build_id_callback, &self);
}

#endif

#if !defined(__APPLE__)
struct ElfModuleSearch {
    uintptr_t pc;
    ModuleInfo* module;
};

int elf_module_info_callback(dl_phdr_info* info, size_t, void* arg) {
    auto* search = static_cast<ElfModuleSearch*>(arg);
    if (!segment_contains(info, search->pc)) {
        return 0;
    }
    if (info->dlpi_name != nullptr && info->dlpi_name[0] != '\0') {
        std::strncpy(search->module->path, info->dlpi_name, sizeof(search->module->path) - 1);
    }
    read_elf_module_info(info, *search->module);
    return 1;
}
#endif

bool find_module_info(uintptr_t pc, ModuleInfo& info, void*) {
    info = {};
    Dl_info moduleInfo;
    if (dladdr(reinterpret_cast<void*>(pc), &moduleInfo) == 0) {
        return false;
    }
    if (moduleInfo.dli_fbase != nullptr) {
        info.base = reinterpret_cast<uintptr_t>(moduleInfo.dli_fbase);
    }
    if (moduleInfo.dli_fname != nullptr && moduleInfo.dli_fname[0] != '\0') {
        info.path[0] = '\0';
        std::strncpy(info.path, moduleInfo.dli_fname, sizeof(info.path) - 1);
    }
    info.buildIdLen = 0;
    info.pdbAge = 0;
#if defined(__APPLE__)
    read_mach_build_id(info.base, info);
#else
    ElfModuleSearch search{pc, &info};
    dl_iterate_phdr(&elf_module_info_callback, &search);
#endif
    return true;
}

const char* signal_name(int sig) {
    switch (sig) {
    case SIGSEGV:
        return "SIGSEGV (segmentation fault)";
    case SIGBUS:
        return "SIGBUS (bus error)";
    case SIGABRT:
        return "SIGABRT (abort)";
    case SIGILL:
        return "SIGILL (illegal instruction)";
    case SIGFPE:
        return "SIGFPE (floating point exception)";
    default:
        return "unknown signal";
    }
}

void emit(int fd, int sig, siginfo_t* info, const uintptr_t* frames, int frameCount, uintptr_t pc) {
    if (fd < 0) {
        return;
    }
    const uintptr_t faultAddr = info != nullptr ? reinterpret_cast<uintptr_t>(info->si_addr) : 0;
    const detail::Report report{
        .reason = signal_name(sig),
        .faultAddress = faultAddr,
        .crashPc = pc,
        .crashPcKnown = pc != 0,
        .frames = frames,
        .frameCount = frameCount,
    };
    detail::emit_report(fd, g_executable, report, &find_module_info, &symbol_for, nullptr);
}

void chain_previous(int sig, siginfo_t* info, void* uc) {
    for (int i = 0; i < kSignalCount; ++i) {
        if (kSignals[i] != sig) {
            continue;
        }
        const struct sigaction& o = g_prev[i];
        if ((o.sa_flags & SA_SIGINFO) != 0) {
            if (o.sa_sigaction != nullptr) {
                o.sa_sigaction(sig, info, uc);
                return;
            }
        } else {
            if (o.sa_handler == SIG_IGN) {
                return;
            }
            if (o.sa_handler != SIG_DFL && o.sa_handler != nullptr) {
                o.sa_handler(sig);
                return;
            }
        }
        break;
    }
    ::signal(sig, SIG_DFL);
    ::raise(sig);
}

void handler(int sig, siginfo_t* info, void* ucv) {
    if (g_inHandler != 0) {
        _exit(128 + sig);
    }
    g_inHandler = 1;

    uintptr_t pc = 0;
    uintptr_t lr = 0;
    uintptr_t fp = 0;
    crash_regs(ucv, pc, lr, fp);
    uintptr_t frames[kMaxFrames];
    int frameCount = capture_backtrace_fp(pc, lr, fp, frames, kMaxFrames);
    if (frameCount < 2) {
        frameCount = capture_backtrace(frames, kMaxFrames, 2);
    }

    emit(kStderrFd, sig, info, frames, frameCount, pc);
    const int logFd = borealis::log::file_descriptor();
    if (logFd >= 0) {
        emit(logFd, sig, info, frames, frameCount, pc);
        ::fsync(logFd);
    }

    chain_previous(sig, info, ucv);
}

void write_terminate_message(int fd, const char* body, const char* what) {
    write_str(fd, "\nterminate: ");
    write_str(fd, body);
    write_str(fd, what);
    write_str(fd, "\n");
}

void on_terminate() {
    const char* body = "unknown reason";
    const char* what = nullptr;
    if (std::exception_ptr ep = std::current_exception()) {
        try {
            std::rethrow_exception(ep);
        } catch (const std::exception& e) {
            body = "uncaught exception: ";
            what = e.what();
        } catch (...) {
            body = "uncaught non-std exception";
        }
    } else {
        body = "no active exception";
    }
    write_terminate_message(kStderrFd, body, what);
    write_terminate_message(borealis::log::file_descriptor(), body, what);
    if (g_prevTerminate != nullptr) {
        g_prevTerminate();
    }
    std::abort();
}

#endif

}  // namespace

void detail::emit_report(int fd, const ModuleInfo& executable, const Report& report,
    ModuleResolver moduleResolver, SymbolResolver symbolResolver, void* userData) {
    if (fd < 0) {
        return;
    }
    emit_header(fd, executable, report, moduleResolver, userData);
    for (int i = 0; i < report.frameCount; ++i) {
        emit_frame(fd, i, report.frames[i], executable, moduleResolver, symbolResolver, userData);
    }
    emit_footer(fd);
}

void install() {
#if defined(_WIN32)
    g_executable.base = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    GetModuleFileNameA(nullptr, g_executable.path, sizeof(g_executable.path) - 1);
    capture_build_id();
#if defined(BOREALIS_CRASH_DBGHELP)
    SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
    SymInitialize(GetCurrentProcess(), nullptr, TRUE);
#endif
    g_prevFilter = SetUnhandledExceptionFilter(&windows_handler);
#elif !defined(__APPLE__) || !TARGET_OS_TV
    Dl_info moduleInfo;
    if (dladdr(reinterpret_cast<void*>(&install), &moduleInfo) != 0) {
        g_executable.base = reinterpret_cast<uintptr_t>(moduleInfo.dli_fbase);
        if (moduleInfo.dli_fname != nullptr) {
            std::strncpy(g_executable.path, moduleInfo.dli_fname, sizeof(g_executable.path) - 1);
        }
    }
    capture_build_id();
    prewarm_unwinder();

    static stack_t altStack;
    altStack.ss_sp = g_altStack;
    altStack.ss_size = sizeof(g_altStack);
    altStack.ss_flags = 0;
    sigaltstack(&altStack, nullptr);

    struct sigaction sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = &handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;

    for (int i = 0; i < kSignalCount; ++i) {
        sigaction(kSignals[i], &sa, &g_prev[i]);
    }

    g_prevTerminate = std::set_terminate(&on_terminate);
#endif
}

}  // namespace borealis::crash
