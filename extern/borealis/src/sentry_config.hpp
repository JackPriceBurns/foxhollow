#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace borealis::sentry::detail {

struct EffectiveConfig {
    bool enabled = true;
    std::string dsn;
    bool debug = false;
};

bool truthy(std::string_view value);
EffectiveConfig resolve_config(std::string_view buildDsn,
    const std::optional<std::string_view>& enabledOverride,
    const std::optional<std::string_view>& dsnOverride,
    const std::optional<std::string_view>& debugOverride);

}  // namespace borealis::sentry::detail
