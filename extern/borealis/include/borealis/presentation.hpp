#pragma once

namespace borealis::presentation {

/**
 * Sets the preferred frame rate. Pass 0 for the highest supported rate. The value
 * persists across Android surface recreation. Returns false if unsupported or invalid.
 */
bool set_preferred_frame_rate(float framesPerSecond) noexcept;

}  // namespace borealis::presentation
