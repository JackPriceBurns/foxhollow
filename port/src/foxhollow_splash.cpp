#include "foxhollow_splash.h"

#include <aurora/aurora.h>
#include <aurora/event.h>
#include <aurora/imgui.h>
#include <SDL3/SDL.h>

#include <algorithm>
#include <array>
#include <cfloat>
#include <cstddef>
#include <string_view>

#include "foxhollow_quit.h"

extern "C" const unsigned char gFoxhollowSplashFont[];
extern "C" const size_t gFoxhollowSplashFontSize;

namespace {

constexpr Uint64 kDurationNs = 10 * SDL_NS_PER_SECOND;
constexpr Uint64 kFadeNs = SDL_NS_PER_SECOND / 2;
constexpr Uint64 kFramePeriodNs = SDL_NS_PER_SECOND / 60;

constexpr std::string_view kName = "FOXHOLLOW";
constexpr std::string_view kGame = "STAR FOX ADVENTURES";

constexpr float kMarkHeight = 48.f;
constexpr float kMarkGap = 16.f;
constexpr float kNameSize = 30.f;
constexpr float kGameSize = 12.f;
constexpr float kGameGap = 4.f;
constexpr float kTracking = 0.1f;
constexpr float kBakeScale = 4.f;
constexpr float kPictureWidthShare = 0.5f;

constexpr std::array<ImVec2, 7> kFoxMark{{
    {16.f, 26.f},
    {7.f, 15.f},
    {9.f, 6.f},
    {13.5f, 11.f},
    {18.5f, 11.f},
    {23.f, 6.f},
    {25.f, 15.f},
}};
constexpr ImVec2 kFoxMarkOrigin{7.f, 6.f};
constexpr ImVec2 kFoxMarkSize{18.f, 20.f};

ImFont* load_font() {
  ImGuiIO& io = ImGui::GetIO();
  if (io.Fonts->Fonts.empty()) {
    io.Fonts->AddFontDefault();
  }
  ImFontGlyphRangesBuilder builder;
  builder.AddText(kName.data(), kName.data() + kName.size());
  builder.AddText(kGame.data(), kGame.data() + kGame.size());
  static ImVector<ImWchar> ranges;
  ranges.clear();
  builder.BuildRanges(&ranges);
  ImFontConfig config;
  config.FontDataOwnedByAtlas = false;
  return io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char*>(gFoxhollowSplashFont),
                                        static_cast<int>(gFoxhollowSplashFontSize), kNameSize * kBakeScale, &config,
                                        ranges.Data);
}

float tracked_width(ImFont* font, float size, std::string_view text) {
  float width = 0.f;
  for (const char c : text) {
    width += font->CalcTextSizeA(size, FLT_MAX, 0.f, &c, &c + 1).x + size * kTracking;
  }
  return width - size * kTracking;
}

void draw_tracked(ImDrawList* list, ImFont* font, float size, ImVec2 pos, ImU32 color, std::string_view text) {
  for (const char c : text) {
    list->AddText(font, size, pos, color, &c, &c + 1);
    pos.x += font->CalcTextSizeA(size, FLT_MAX, 0.f, &c, &c + 1).x + size * kTracking;
  }
}

ImU32 with_alpha(int r, int g, int b, float alpha) {
  return IM_COL32(r, g, b, static_cast<int>(std::clamp(alpha, 0.f, 1.f) * 255.f + 0.5f));
}

void draw(ImFont* font, float alpha) {
  fhDrawBlackScreen();
  ImDrawList* list = ImGui::GetForegroundDrawList();
  const ImVec2 display = ImGui::GetIO().DisplaySize;

  const float markWidth = kMarkHeight * kFoxMarkSize.x / kFoxMarkSize.y;
  const float textWidth = std::max(tracked_width(font, kNameSize, kName), tracked_width(font, kGameSize, kGame));
  const float unitWidth = markWidth + kMarkGap + textWidth;
  const float pictureWidth = std::min(display.x, display.y * 4.f / 3.f);
  const float scale = pictureWidth * kPictureWidthShare / unitWidth;

  const float textHeight = kNameSize + kGameGap + kGameSize;
  const float unitHeight = std::max(kMarkHeight, textHeight);
  const ImVec2 origin{(display.x - unitWidth * scale) * 0.5f, (display.y - unitHeight * scale) * 0.5f};

  const ImU32 orange = with_alpha(255, 137, 4, alpha);
  const ImU32 white = with_alpha(250, 250, 250, alpha);

  const float markScale = kMarkHeight * scale / kFoxMarkSize.y;
  const float markTop = origin.y + (unitHeight - kMarkHeight) * 0.5f * scale;
  std::array<ImVec2, kFoxMark.size()> mark{};
  for (std::size_t i = 0; i < kFoxMark.size(); ++i) {
    mark[i] = {origin.x + (kFoxMark[i].x - kFoxMarkOrigin.x) * markScale,
               markTop + (kFoxMark[i].y - kFoxMarkOrigin.y) * markScale};
  }
  list->AddConcavePolyFilled(mark.data(), static_cast<int>(mark.size()), orange);

  const float textLeft = origin.x + (markWidth + kMarkGap) * scale;
  const float textTop = origin.y + (unitHeight - textHeight) * 0.5f * scale;
  draw_tracked(list, font, kNameSize * scale, {textLeft, textTop}, white, kName);
  draw_tracked(list, font, kGameSize * scale, {textLeft, textTop + (kNameSize + kGameGap) * scale}, orange, kGame);
}

bool skip_requested(const SDL_Event& event) {
  switch (event.type) {
  case SDL_EVENT_KEY_DOWN:
    return !event.key.repeat;
  case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
  case SDL_EVENT_MOUSE_BUTTON_DOWN:
    return true;
  default:
    return false;
  }
}

float alpha_at(Uint64 elapsed, Uint64 end) {
  const float in = static_cast<float>(elapsed) / static_cast<float>(kFadeNs);
  const float out = static_cast<float>(end > elapsed ? end - elapsed : 0) / static_cast<float>(kFadeNs);
  return std::clamp(std::min(in, out), 0.f, 1.f);
}

} // namespace

extern "C" void fhDrawBlackScreen(void) {
  ImGui::GetForegroundDrawList()->AddRectFilled({0.f, 0.f}, ImGui::GetIO().DisplaySize, IM_COL32(0, 0, 0, 255));
}

extern "C" int fhSplashRun(void) {
  ImFont* font = load_font();
  if (font == nullptr) {
    return 1;
  }

  const Uint64 start = SDL_GetTicksNS();
  Uint64 end = kDurationNs;
  Uint64 nextFrame = start;
  while (true) {
    const Uint64 elapsed = SDL_GetTicksNS() - start;
    if (elapsed >= end) {
      return 1;
    }

    for (const AuroraEvent* event = aurora_update(); event != nullptr && event->type != AURORA_NONE; ++event) {
      if (event->type == AURORA_EXIT) {
        return 0;
      }
      if (event->type == AURORA_SDL_EVENT && skip_requested(event->sdl) && end > elapsed + kFadeNs) {
        end = elapsed + std::min(kFadeNs, elapsed);
      }
    }
    if (fhQuitRequested()) {
      return 0;
    }

    if (aurora_begin_frame()) {
      draw(font, alpha_at(elapsed, end));
      aurora_end_frame();
    }

    nextFrame += kFramePeriodNs;
    const Uint64 now = SDL_GetTicksNS();
    if (now < nextFrame) {
      SDL_DelayPrecise(nextFrame - now);
    } else {
      nextFrame = now;
    }
  }
}
