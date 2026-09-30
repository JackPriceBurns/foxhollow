#include "foxhollow_cutscene_skip.h"

#include <aurora/imgui.h>
#include <SDL3/SDL.h>

namespace {

constexpr float kFadeInMs = 180.0f;
constexpr float kVisibleMs = 1300.0f;
constexpr float kFadeOutMs = 450.0f;
constexpr float kMargin = 22.0f;
constexpr float kBackgroundAlpha = 0.65f;
constexpr float kScale = 1.05f;

const char* sText = nullptr;
Uint64 sStartMs = 0;
int sStarted = 0;
int sHoldWhileSkipping = 0;

float notification_alpha(float elapsed) {
    if (elapsed < kFadeInMs) {
        return elapsed / kFadeInMs;
    }
    elapsed -= kFadeInMs;
    if (elapsed < kVisibleMs) {
        return 1.0f;
    }
    elapsed -= kVisibleMs;
    if (elapsed < kFadeOutMs) {
        return 1.0f - elapsed / kFadeOutMs;
    }
    return 0.0f;
}

}

extern "C" void fhCutsceneSkipNotify(const char* text, int holdWhileSkipping) {
    sText = text;
    sStarted = 0;
    sHoldWhileSkipping = holdWhileSkipping;
}

extern "C" void fhCutsceneSkipDrawNotification(void) {
    if (sText == nullptr) {
        return;
    }

    const Uint64 now = SDL_GetTicks();
    if (sStarted == 0) {
        sStartMs = now;
        sStarted = 1;
    }
    if (sHoldWhileSkipping != 0 && fhCutsceneSkipIsActive() != 0 &&
        now - sStartMs > static_cast<Uint64>(kFadeInMs)) {
        sStartMs = now - static_cast<Uint64>(kFadeInMs);
    }
    const float elapsed = static_cast<float>(now - sStartMs);
    if (elapsed >= kFadeInMs + kVisibleMs + kFadeOutMs) {
        sText = nullptr;
        return;
    }
    const float alpha = notification_alpha(elapsed);

    const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                                   ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav |
                                   ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoFocusOnAppearing;
    ImGui::SetNextWindowPos(ImVec2(kMargin, kMargin), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(kBackgroundAlpha);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f * kScale, 8.0f * kScale));
    ImGui::Begin("##FoxhollowCutsceneSkipNotification", nullptr, flags);
    ImGui::SetWindowFontScale(kScale);
    ImGui::TextUnformatted(sText);
    ImGui::End();
    ImGui::PopStyleVar(2);
}
