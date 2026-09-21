#include "borealis/sentry.hpp"

#include <gtest/gtest.h>

#include "sentry_config.hpp"

#include <optional>
#include <string_view>

namespace {

using borealis::sentry::detail::resolve_config;

TEST(Sentry, TruthyValues) {
    using borealis::sentry::detail::truthy;
    for (const std::string_view value : {"1", "true", "TRUE", "yes", "YES", "on", "ON"}) {
        EXPECT_TRUE(truthy(value));
    }
    for (const std::string_view value : {"", "0", "false", "False", "off", "no"}) {
        EXPECT_FALSE(truthy(value));
    }
}

TEST(Sentry, EnvironmentOverrides) {
    using OptionalView = std::optional<std::string_view>;

    auto config = resolve_config("build-dsn", {}, {}, {});
    EXPECT_TRUE(config.enabled);
    EXPECT_EQ(config.dsn, "build-dsn");
    EXPECT_FALSE(config.debug);

    config = resolve_config(
        "build-dsn", OptionalView{"0"}, OptionalView{"runtime-dsn"}, OptionalView{"yes"});
    EXPECT_FALSE(config.enabled);
    EXPECT_EQ(config.dsn, "runtime-dsn");
    EXPECT_TRUE(config.debug);

    // Empty overrides preserve defaults.
    config = resolve_config("build-dsn", OptionalView{""}, OptionalView{""}, OptionalView{""});
    EXPECT_TRUE(config.enabled);
    EXPECT_EQ(config.dsn, "build-dsn");
    EXPECT_FALSE(config.debug);
}

TEST(Sentry, UninitializedApi) {
    EXPECT_EQ(borealis::sentry::get_consent(), borealis::sentry::Consent::Unavailable);
    borealis::sentry::set_consent(true);
    borealis::sentry::shutdown();
#if BOREALIS_HAS_SENTRY
    EXPECT_TRUE(borealis::sentry::available());
#else
    EXPECT_FALSE(borealis::sentry::available());
    EXPECT_FALSE(borealis::sentry::initialize({}));
#endif
}

}  // namespace
