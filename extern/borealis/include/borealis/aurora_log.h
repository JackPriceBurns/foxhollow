#pragma once

#include "borealis/log.hpp"

#include <aurora/aurora.h>

namespace borealis::log {

/** Pass as AuroraConfig::logCallback to route Aurora logging into borealis::log. */
AuroraLogCallback aurora_callback() noexcept;

LogLevel from_aurora_level(AuroraLogLevel level) noexcept;
AuroraLogLevel to_aurora_level(LogLevel level) noexcept;

}  // namespace borealis::log
