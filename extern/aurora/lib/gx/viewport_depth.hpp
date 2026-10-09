#pragma once

namespace aurora::gx {

struct ViewportDepth {
  float min;
  float max;
  float scale;
  float offset;
};

constexpr ViewportDepth viewport_depth(float nearZ, float farZ, bool reversed) noexcept {
  const float min = reversed ? 1.f - farZ : nearZ;
  const float max = reversed ? 1.f - nearZ : farZ;
  if (min < 0.f || max > 1.f || min > max) {
    return {0.f, 1.f, max - min, min};
  }
  return {min, max, 1.f, 0.f};
}

} // namespace aurora::gx
