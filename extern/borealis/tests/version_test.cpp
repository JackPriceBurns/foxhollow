#include "borealis/version.h"

#include <gtest/gtest.h>

#include <string_view>

namespace {

TEST(Version, MacrosAreLiterals) {
    constexpr std::string_view describe = BOREALIS_APP_DESCRIBE;
    constexpr std::string_view version = BOREALIS_APP_VERSION;
    constexpr std::string_view shortVersion = BOREALIS_APP_SHORT_VERSION;
    constexpr std::string_view platform = BOREALIS_PLATFORM_NAME;
    constexpr std::string_view arch = BOREALIS_ARCH;
    constexpr std::string_view lib = BOREALIS_LIB_DESCRIBE;

    EXPECT_FALSE(describe.empty());
    EXPECT_FALSE(version.empty());
    EXPECT_FALSE(shortVersion.empty());
    EXPECT_FALSE(platform.empty());
    EXPECT_FALSE(arch.empty());
    EXPECT_NE(arch, "unknown");
    // Git checkouts always provide a library description.
    EXPECT_FALSE(lib.empty());

    // BOREALIS_BUILD_TYPE comes from borealis::core.
    constexpr std::string_view buildTypeFromCore = BOREALIS_BUILD_TYPE;
    EXPECT_FALSE(buildTypeFromCore.empty());

    // Branch, revision, and date may be empty.
    constexpr std::string_view branch = BOREALIS_APP_BRANCH;
    constexpr std::string_view revision = BOREALIS_APP_REVISION;
    constexpr std::string_view date = BOREALIS_APP_DATE;
    (void)branch;
    (void)revision;
    (void)date;
}

TEST(Version, Shape) {
    constexpr std::string_view version = BOREALIS_APP_VERSION;
    int parts = 1;
    for (const char c : version) {
        if (c == '.') {
            ++parts;
        } else {
            EXPECT_TRUE(c >= '0' && c <= '9');
        }
    }
    EXPECT_EQ(parts, 4);

    constexpr std::string_view shortVersion = BOREALIS_APP_SHORT_VERSION;
    EXPECT_TRUE(version.starts_with(shortVersion));
    EXPECT_EQ(version[shortVersion.size()], '.');

    static_assert(BOREALIS_APP_VERSION_CODE >= 1, "version code is a positive integer constant");
}

}  // namespace
