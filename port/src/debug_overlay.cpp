#include <aurora/gfx.h>
#include <aurora/imgui.h>
#include <SDL3/SDL.h>

namespace {
constexpr uint32_t kTextureUploadBudget = 25165824;

bool sVisible = true;
bool sToggleKeyWasDown;
Uint64 sPreviousFrameNs;
Uint64 sSampleStartNs;
float sWorstFrameMs;
float sDisplayedWorstFrameMs;
uint32_t sFrameCount;
float sDisplayedFps;
uint32_t sPreviousCreatedPipelines;
uint32_t sDisplayedPipelinesPerSecond;
uint32_t sPeakTextureUploadSize;
uint32_t sDisplayedPeakTextureUploadSize;
uint32_t sBudgetOverflowFrames;
uint32_t sDisplayedBudgetOverflowFrames;
}

extern "C" void fhDebugOverlayDraw(void) {
  const Uint64 now = SDL_GetTicksNS();
  const float frameMs = sPreviousFrameNs != 0 ? static_cast<float>(now - sPreviousFrameNs) / 1000000.0f : 0.0f;
  sPreviousFrameNs = now;

  const AuroraStats* stats = aurora_get_stats();

  if (sSampleStartNs == 0) {
    sSampleStartNs = now;
    if (stats != nullptr) {
      sPreviousCreatedPipelines = stats->createdPipelines;
    }
  }
  if (frameMs > sWorstFrameMs) {
    sWorstFrameMs = frameMs;
  }
  sFrameCount++;
  if (stats != nullptr) {
    if (stats->lastTextureUploadSize > sPeakTextureUploadSize) {
      sPeakTextureUploadSize = stats->lastTextureUploadSize;
    }
    if (stats->lastTextureUploadSize >= kTextureUploadBudget) {
      sBudgetOverflowFrames++;
    }
  }

  if (now - sSampleStartNs >= SDL_NS_PER_SECOND) {
    const float elapsedSeconds = static_cast<float>(now - sSampleStartNs) / 1000000000.0f;
    sDisplayedWorstFrameMs = sWorstFrameMs;
    sDisplayedFps = elapsedSeconds > 0.0f ? static_cast<float>(sFrameCount) / elapsedSeconds : 0.0f;
    sDisplayedPeakTextureUploadSize = sPeakTextureUploadSize;
    sDisplayedBudgetOverflowFrames = sBudgetOverflowFrames;
    if (stats != nullptr) {
      sDisplayedPipelinesPerSecond = stats->createdPipelines - sPreviousCreatedPipelines;
      sPreviousCreatedPipelines = stats->createdPipelines;
    }
    sWorstFrameMs = 0.0f;
    sFrameCount = 0;
    sPeakTextureUploadSize = 0;
    sBudgetOverflowFrames = 0;
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
  ImGui::Text("%.1f FPS  %.2f ms  worst %.2f ms", sDisplayedFps, frameMs, displayedWorstFrameMs);

  if (stats != nullptr) {
    const ImVec4 hot = ImVec4(1.0f, 0.45f, 0.35f, 1.0f);
    const ImVec4 warm = ImVec4(1.0f, 0.85f, 0.35f, 1.0f);

    if (stats->queuedPipelines > 0 || sDisplayedPipelinesPerSecond > 0) {
      ImGui::TextColored(warm, "pipelines  queued %u  built/s %u", stats->queuedPipelines,
                         sDisplayedPipelinesPerSecond);
    } else {
      ImGui::Text("pipelines  queued 0  built/s 0");
    }

    const float peakMiB = static_cast<float>(sDisplayedPeakTextureUploadSize) / 1048576.0f;
    const float budgetMiB = static_cast<float>(kTextureUploadBudget) / 1048576.0f;
    if (sDisplayedBudgetOverflowFrames > 0) {
      ImGui::TextColored(hot, "texupload  peak %.2f / %.1f MiB  OVERFLOW %u frames", peakMiB, budgetMiB,
                         sDisplayedBudgetOverflowFrames);
    } else {
      ImGui::Text("texupload  peak %.2f / %.1f MiB", peakMiB, budgetMiB);
    }

    ImGui::Text("draws  %u  merged %u", stats->drawCallCount, stats->mergedDrawCallCount);
  }

  ImGui::End();
}
