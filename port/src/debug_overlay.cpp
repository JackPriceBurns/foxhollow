#include <aurora/imgui.h>
#include <SDL3/SDL.h>

namespace {
bool sVisible = true;
bool sToggleKeyWasDown;
Uint64 sPreviousFrameNs;
Uint64 sSampleStartNs;
float sWorstFrameMs;
float sDisplayedWorstFrameMs;
}

extern "C" void fhDebugOverlayDraw(void) {
  const Uint64 now = SDL_GetTicksNS();
  const float frameMs = sPreviousFrameNs != 0 ? static_cast<float>(now - sPreviousFrameNs) / 1000000.0f : 0.0f;
  sPreviousFrameNs = now;

  if (sSampleStartNs == 0) {
    sSampleStartNs = now;
  }
  if (frameMs > sWorstFrameMs) {
    sWorstFrameMs = frameMs;
  }
  if (now - sSampleStartNs >= SDL_NS_PER_SECOND) {
    sDisplayedWorstFrameMs = sWorstFrameMs;
    sWorstFrameMs = 0.0f;
    sSampleStartNs = now;
  }

  int keyCount;
  const bool* keys = SDL_GetKeyboardState(&keyCount);
  const bool toggleKeyDown = keyCount > SDL_SCANCODE_COMMA && keys[SDL_SCANCODE_COMMA];
  if (toggleKeyDown && !sToggleKeyWasDown) {
    sVisible = !sVisible;
  }
  sToggleKeyWasDown = toggleKeyDown;

  if (!sVisible) {
    return;
  }

  const float displayedWorstFrameMs = sDisplayedWorstFrameMs != 0.0f ? sDisplayedWorstFrameMs : sWorstFrameMs;
  const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                                 ImGuiWindowFlags_NoInputs;
  ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Always);
  ImGui::SetNextWindowBgAlpha(0.65f);
  ImGui::Begin("Foxhollow Performance", nullptr, flags);
  ImGui::Text("%.1f FPS  %.2f ms  worst %.2f ms", ImGui::GetIO().Framerate, frameMs, displayedWorstFrameMs);
  ImGui::End();
}
