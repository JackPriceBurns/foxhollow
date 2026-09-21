#include "sentry_config.hpp"

namespace borealis::sentry::detail {

bool truthy(std::string_view value) {
    return value == "1" || value == "true" || value == "TRUE" || value == "yes" || value == "YES" ||
           value == "on" || value == "ON";
}

EffectiveConfig resolve_config(std::string_view buildDsn,
    const std::optional<std::string_view>& enabledOverride,
    const std::optional<std::string_view>& dsnOverride,
    const std::optional<std::string_view>& debugOverride) {
    EffectiveConfig config{
        .dsn = std::string(buildDsn),
    };
    if (enabledOverride && !enabledOverride->empty()) {
        config.enabled = truthy(*enabledOverride);
    }
    if (dsnOverride && !dsnOverride->empty()) {
        config.dsn = *dsnOverride;
    }
    if (debugOverride && !debugOverride->empty()) {
        config.debug = truthy(*debugOverride);
    }
    return config;
}

}  // namespace borealis::sentry::detail
