#include "crash_report.hpp"

#include <gtest/gtest.h>

#include "borealis/version.h"

#include <cstdio>
#include <cstring>
#include <string>

#if defined(_WIN32)
#include <io.h>
#define BOREALIS_TEST_FILENO _fileno
#else
#include <unistd.h>
#define BOREALIS_TEST_FILENO fileno
#endif

namespace {

using borealis::crash::detail::ModuleInfo;

bool resolve_module(uintptr_t, ModuleInfo& info, void*) {
    info.base = 0x2000;
    info.size = 0x1000;
    std::strcpy(info.path, "module\"quoted\\name.so");
    info.buildId[0] = 0xde;
    info.buildId[1] = 0xad;
    info.buildId[2] = 0xbe;
    info.buildId[3] = 0xef;
    info.buildIdLen = 4;
    return true;
}

const char* resolve_symbol(uintptr_t, unsigned long long* displacement, void*) {
    *displacement = 0x2a;
    return "test_symbol";
}

std::string read_file(std::FILE* file) {
    EXPECT_EQ(std::fseek(file, 0, SEEK_END), 0);
    const long size = std::ftell(file);
    EXPECT_GE(size, 0);
    EXPECT_EQ(std::fseek(file, 0, SEEK_SET), 0);
    std::string text(static_cast<size_t>(size), '\0');
    EXPECT_EQ(std::fread(text.data(), 1, text.size(), file), text.size());
    return text;
}

TEST(CrashReport, Format) {
    ModuleInfo executable;
    executable.base = 0x1000;
    executable.size = 0x4000;
    std::strcpy(executable.path, "/tmp/test-app");
    executable.buildId[0] = 0x01;
    executable.buildId[1] = 0xab;
    executable.buildId[2] = 0x00;
    executable.buildId[3] = 0xff;
    executable.buildIdLen = 4;

    const uintptr_t frames[] = {0x2020, 0x2070};
    const borealis::crash::detail::Report report{
        .reason = "TEST_EXCEPTION",
        .code = 0xc0de,
        .hasCode = true,
        .faultAddress = 0x2345,
        .crashPc = 0x2010,
        .crashPcKnown = true,
        .frames = frames,
        .frameCount = 2,
    };

    std::FILE* file = std::tmpfile();
    ASSERT_NE(file, nullptr);
    borealis::crash::detail::emit_report(
        BOREALIS_TEST_FILENO(file), executable, report, &resolve_module, &resolve_symbol, nullptr);
    const std::string actual = read_file(file);
    std::fclose(file);

    const std::string expected =
        "\n==================== APPLICATION CRASHED ====================\n"
        "Build:       " BOREALIS_APP_DESCRIBE " (" BOREALIS_APP_BRANCH ")\n"
        "Revision:    " BOREALIS_APP_REVISION "  Date: " BOREALIS_APP_DATE
        "  Type: " BOREALIS_BUILD_TYPE "\n"
        "Platform:    " BOREALIS_PLATFORM_NAME " / " BOREALIS_ARCH "\n"
        "Module:      /tmp/test-app\n"
        "Module base: 0x1000\n"
        "Build-ID:    01ab00ff\n"
        "Reason:      TEST_EXCEPTION (0xc0de)\n"
        "Fault addr:  0x2345\n"
        "Crash PC:    0x2010 module_base=0x2000 image_size=0x1000 rva=0x10 "
        "module=\"module\\\"quoted\\\\name.so\" build_id=deadbeef\n"
        "Backtrace:\n"
        "#00 abs=0x2020 module_base=0x2000 image_size=0x1000 rva=0x20 "
        "module=\"module\\\"quoted\\\\name.so\" build_id=deadbeef test_symbol+0x2a\n"
        "#01 abs=0x2070 module_base=0x2000 image_size=0x1000 rva=0x70 "
        "module=\"module\\\"quoted\\\\name.so\" build_id=deadbeef test_symbol+0x2a\n"
        "========================================================\n";
    EXPECT_EQ(actual, expected);
}

TEST(CrashReport, MissingModuleAndCrashPc) {
    ModuleInfo executable;
    executable.base = 0x4000;
    std::strcpy(executable.path, "fallback-app");

    const uintptr_t frame = 0x4010;
    const borealis::crash::detail::Report report{
        .reason = "SIGTEST",
        .frames = &frame,
        .frameCount = 1,
    };

    std::FILE* file = std::tmpfile();
    ASSERT_NE(file, nullptr);
    borealis::crash::detail::emit_report(
        BOREALIS_TEST_FILENO(file), executable, report, nullptr, nullptr, nullptr);
    const std::string actual = read_file(file);
    std::fclose(file);

    EXPECT_NE(actual.find("Build-ID:    (unavailable)"), std::string::npos);
    EXPECT_NE(actual.find("Crash PC:    (unavailable on this platform)"), std::string::npos);
    EXPECT_TRUE(
        actual.find("#00 abs=0x4010 module_base=0x4000 rva=0x10 "
                    "module=\"fallback-app\" build_id=(unavailable)\n") != std::string::npos);
}

}  // namespace
