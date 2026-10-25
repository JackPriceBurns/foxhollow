#include "foxhollow_ui.h"

#include <aurora/aurora.h>
#include <aurora/imgui.h>
#include <aurora/input.hpp>
#include <dolphin/gx/GXAurora.h>
#include <imgui_internal.h>

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

extern "C" const unsigned char gFoxhollowSplashFont[];
extern "C" const size_t gFoxhollowSplashFontSize;

namespace {

namespace input = aurora::input;

constexpr float kFontSize = 18.f;
constexpr float kAnchorMargin = 12.f;
constexpr float kScrollMinimum = 40.f;
constexpr int32_t kLayerPriority = 150;

enum class Scope {
  Window,
  Section,
  Scroll,
};

struct Entry {
  Scope scope;
  ImGuiID scroll;
};

struct ScrollMark {
  ImGuiID id;
  float afterY;
};

struct WindowState {
  bool wrap;
  std::vector<ScrollMark> scrolls;
};

struct ColorSpan {
  size_t offset;
  ImU32 color;
};

struct ParsedText {
  std::string plain;
  std::vector<ColorSpan> spans;
};

FhMod* sMod = nullptr;
std::string sModId;
std::vector<Entry> sScopes;
std::vector<WindowState> sWindows;
std::unordered_map<ImGuiID, float> sFooters;
ImFont* sFont = nullptr;
input::LayerId sLayer = input::kInvalidLayerId;
bool sModal = false;
bool sModalThisFrame = false;
ImGuiID sActiveInput = 0;
std::string sActiveInputText;
ImGuiID sRefocusInput = 0;

bool inside() { return sMod != nullptr && !sScopes.empty(); }

ImU32 from_rgba(uint32_t rgba) {
  return IM_COL32((rgba >> 24) & 0xFF, (rgba >> 16) & 0xFF, (rgba >> 8) & 0xFF, rgba & 0xFF);
}

int hex_digit(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  }
  if (c >= 'a' && c <= 'f') {
    return c - 'a' + 10;
  }
  if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  }
  return -1;
}

const char* parse_escape(const char* cursor, uint32_t& outRgb) {
  if (cursor[1] != '#') {
    return nullptr;
  }
  uint32_t value = 0;
  for (int i = 0; i < 6; ++i) {
    const int digit = hex_digit(cursor[2 + i]);
    if (digit < 0) {
      return nullptr;
    }
    value = (value << 4) | static_cast<uint32_t>(digit);
  }
  if (cursor[8] != ';') {
    return nullptr;
  }
  outRgb = value;
  return cursor + 9;
}

ParsedText parse_text(const char* text, ImU32 color) {
  const uint32_t alpha = (color >> IM_COL32_A_SHIFT) & 0xFF;
  ParsedText parsed;
  parsed.spans.push_back({0, color});
  for (const char* cursor = text; *cursor != '\0';) {
    if (*cursor != '\x1b') {
      parsed.plain.push_back(*cursor++);
      continue;
    }
    uint32_t rgb = 0;
    if (const char* after = parse_escape(cursor, rgb)) {
      parsed.spans.push_back({parsed.plain.size(), from_rgba((rgb << 8) | alpha)});
      cursor = after;
    } else {
      ++cursor;
    }
  }
  return parsed;
}

ImVec2 render_text(ImDrawList* list, ImFont* font, float size, ImVec2 origin, float wrap, const ParsedText& parsed) {
  const float scale = size / font->FontSize;
  const char* const begin = parsed.plain.data();
  const char* const end = begin + parsed.plain.size();
  const std::vector<ColorSpan>& spans = parsed.spans;
  size_t span = 0;
  float y = 0.f;
  float widest = 0.f;
  const char* line = begin;
  for (;;) {
    const char* newline = static_cast<const char*>(std::memchr(line, '\n', static_cast<size_t>(end - line)));
    const char* paragraphEnd = newline != nullptr ? newline : end;
    do {
      const char* lineEnd = wrap > 0.f ? font->CalcWordWrapPositionA(scale, line, paragraphEnd, wrap) : paragraphEnd;
      float x = 0.f;
      for (const char* piece = line; piece < lineEnd;) {
        while (span + 1 < spans.size() && begin + spans[span + 1].offset <= piece) {
          ++span;
        }
        const char* pieceEnd = lineEnd;
        if (span + 1 < spans.size()) {
          pieceEnd = std::min(lineEnd, begin + spans[span + 1].offset);
        }
        if (list != nullptr) {
          list->AddText(font, size, ImVec2(origin.x + x, origin.y + y), spans[span].color, piece, pieceEnd);
        }
        x += font->CalcTextSizeA(size, FLT_MAX, 0.f, piece, pieceEnd).x;
        piece = pieceEnd;
      }
      widest = std::max(widest, x);
      y += size;
      line = lineEnd;
      while (line < paragraphEnd && (*line == ' ' || *line == '\t')) {
        ++line;
      }
    } while (line < paragraphEnd);
    if (newline == nullptr) {
      break;
    }
    line = newline + 1;
  }
  return ImVec2(widest, y);
}

ImFont* overlay_font() { return sFont != nullptr ? sFont : ImGui::GetFont(); }

void mark_scroll(ImGuiID id) { sWindows.back().scrolls.push_back({id, ImGui::GetCursorPosY()}); }

Scope pop_scope() {
  const Entry entry = sScopes.back();
  sScopes.pop_back();
  switch (entry.scope) {
  case Scope::Window: {
    const float endY = ImGui::GetCursorPosY();
    for (const ScrollMark& mark : sWindows.back().scrolls) {
      sFooters[mark.id] = endY - mark.afterY;
    }
    sWindows.pop_back();
    ImGui::End();
    break;
  }
  case Scope::Section:
    ImGui::TreePop();
    break;
  case Scope::Scroll:
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
      ImGui::SetScrollHereY(1.f);
    }
    ImGui::EndChild();
    mark_scroll(entry.scroll);
    break;
  }
  return entry.scope;
}

int ui_begin(const FhUi*, const char* id, const FhUiWindow* window, int* open) {
  if (sMod == nullptr || id == nullptr || window == nullptr || window->structSize < sizeof(FhUiWindow)) {
    return 0;
  }
  if (open != nullptr && *open == 0) {
    return 0;
  }

  const uint32_t flags = window->flags;
  const bool passive = (flags & FH_UI_WINDOW_PASSIVE) != 0;
  const bool modal = (flags & FH_UI_WINDOW_MODAL) != 0 && !passive;
  const bool fixed = (flags & FH_UI_WINDOW_FIXED) != 0;
  const bool autoHeight = (flags & FH_UI_WINDOW_AUTO_HEIGHT) != 0;
  const ImVec2 display = ImGui::GetIO().DisplaySize;

  ImVec2 position(window->x, window->y);
  if ((flags & FH_UI_WINDOW_RELATIVE) != 0) {
    position.x *= display.x;
    position.y *= display.y;
  }
  ImVec2 pivot(0.f, 0.f);
  if ((flags & FH_UI_WINDOW_ANCHOR_BOTTOM) != 0) {
    position.y = display.y - kAnchorMargin;
    pivot.y = 1.f;
  }
  const ImGuiCond cond = fixed ? ImGuiCond_Always : ImGuiCond_FirstUseEver;
  ImGui::SetNextWindowPos(position, cond, pivot);
  if (autoHeight) {
    if (window->width > 0.f) {
      ImGui::SetNextWindowSizeConstraints(ImVec2(window->width, 0.f), ImVec2(window->width, FLT_MAX));
    }
  } else if (window->width > 0.f && window->height > 0.f) {
    ImGui::SetNextWindowSize(ImVec2(window->width, window->height), cond);
  }

  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
  if ((flags & FH_UI_WINDOW_NO_TITLE) != 0) {
    windowFlags |= ImGuiWindowFlags_NoTitleBar;
  }
  if ((flags & FH_UI_WINDOW_TRANSPARENT) != 0) {
    windowFlags |= ImGuiWindowFlags_NoBackground;
  }
  if (fixed) {
    windowFlags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
  }
  if (autoHeight) {
    windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
  }
  if (passive) {
    windowFlags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav |
                   ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoSavedSettings;
  }

  const std::string name =
      std::string(window->title != nullptr ? window->title : "") + "###" + sModId + "/" + id;
  bool keep = true;
  bool* close = open != nullptr && (windowFlags & ImGuiWindowFlags_NoTitleBar) == 0 ? &keep : nullptr;
  const bool visible = ImGui::Begin(name.c_str(), close, windowFlags);

  bool closed = !keep;
  if (modal) {
    if (ImGui::IsWindowAppearing()) {
      ImGui::SetWindowFocus();
    }
    if (open != nullptr && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
        ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
      closed = true;
    }
  }
  if (closed) {
    *open = 0;
  } else if (modal) {
    sModalThisFrame = true;
  }
  if (closed || !visible) {
    ImGui::End();
    return 0;
  }

  sScopes.push_back({Scope::Window, 0});
  sWindows.push_back({!autoHeight || window->width > 0.f, {}});
  return 1;
}

void ui_end(const FhUi*) {
  if (sMod == nullptr || std::none_of(sScopes.begin(), sScopes.end(),
                                      [](const Entry& entry) { return entry.scope == Scope::Window; })) {
    return;
  }
  while (pop_scope() != Scope::Window) {
  }
}

void ui_text(const FhUi*, const char* text) {
  if (!inside() || text == nullptr) {
    return;
  }
  const ParsedText parsed = parse_text(text, ImGui::GetColorU32(ImGuiCol_Text));
  const float wrap = sWindows.back().wrap ? ImGui::GetContentRegionAvail().x : 0.f;
  const ImVec2 size = render_text(ImGui::GetWindowDrawList(), ImGui::GetFont(), ImGui::GetFontSize(),
                                  ImGui::GetCursorScreenPos(), wrap, parsed);
  ImGui::Dummy(size);
}

int ui_button(const FhUi*, const char* label) {
  if (!inside() || label == nullptr) {
    return 0;
  }
  return ImGui::Button(label) ? 1 : 0;
}

int ui_checkbox(const FhUi*, const char* label, int* value) {
  if (!inside() || label == nullptr || value == nullptr) {
    return 0;
  }
  bool checked = *value != 0;
  if (!ImGui::Checkbox(label, &checked)) {
    return 0;
  }
  *value = checked ? 1 : 0;
  return 1;
}

int ui_slider(const FhUi*, const char* label, float* value, float minimum, float maximum) {
  if (!inside() || label == nullptr || value == nullptr || !std::isfinite(*value) || !std::isfinite(minimum) ||
      !std::isfinite(maximum) || minimum >= maximum) {
    return 0;
  }
  return ImGui::SliderFloat(label, value, minimum, maximum) ? 1 : 0;
}

int input_history(ImGuiInputTextCallbackData* data) {
  if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory) {
    *static_cast<ImGuiKey*>(data->UserData) = data->EventKey;
  }
  return 0;
}

int ui_input(const FhUi*, const char* label, char* buffer, uint32_t capacity, uint32_t flags) {
  if (!inside() || label == nullptr || buffer == nullptr || capacity == 0) {
    return FH_UI_INPUT_NONE;
  }
  buffer[capacity - 1] = '\0';

  const ImGuiID id = ImGui::GetID(label);
  if (id == sActiveInput && sActiveInputText != buffer) {
    if (ImGuiInputTextState* state = ImGui::GetInputTextState(id)) {
      state->ReloadUserBufAndMoveToEnd();
    }
  }
  const bool focus = (flags & FH_UI_INPUT_FOCUS) != 0;
  if (focus && (ImGui::IsWindowAppearing() || sRefocusInput == id)) {
    ImGui::SetKeyboardFocusHere();
    sRefocusInput = 0;
  }
  if (std::strncmp(label, "##", 2) == 0) {
    ImGui::SetNextItemWidth(-FLT_MIN);
  }

  ImGuiKey history = ImGuiKey_None;
  const bool submitted =
      ImGui::InputText(label, buffer, capacity, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory,
                       input_history, &history);
  const bool edited = ImGui::IsItemEdited();
  if (ImGui::IsItemActive()) {
    sActiveInput = id;
    sActiveInputText = buffer;
  } else if (sActiveInput == id) {
    sActiveInput = 0;
    sActiveInputText.clear();
  }

  if (submitted) {
    if (focus) {
      sRefocusInput = id;
    }
    return FH_UI_INPUT_SUBMITTED;
  }
  if (history == ImGuiKey_UpArrow) {
    return FH_UI_INPUT_HISTORY_PREVIOUS;
  }
  if (history == ImGuiKey_DownArrow) {
    return FH_UI_INPUT_HISTORY_NEXT;
  }
  return edited ? FH_UI_INPUT_EDITED : FH_UI_INPUT_NONE;
}

int ui_begin_section(const FhUi*, const char* label) {
  if (!inside() || label == nullptr) {
    return 0;
  }
  if (!ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
    return 0;
  }
  sScopes.push_back({Scope::Section, 0});
  return 1;
}

void ui_end_section(const FhUi*) {
  if (inside() && sScopes.back().scope == Scope::Section) {
    pop_scope();
  }
}

int ui_begin_scroll(const FhUi*, const char* id) {
  if (!inside() || id == nullptr) {
    return 0;
  }
  const ImGuiID scroll = ImGui::GetID(id);
  const auto footer = sFooters.find(scroll);
  const float reserve = footer != sFooters.end() ? footer->second : 0.f;
  const float height = std::max(ImGui::GetContentRegionAvail().y - reserve, kScrollMinimum);
  if (!ImGui::BeginChild(scroll, ImVec2(0.f, height))) {
    ImGui::EndChild();
    mark_scroll(scroll);
    return 0;
  }
  sScopes.push_back({Scope::Scroll, scroll});
  return 1;
}

void ui_end_scroll(const FhUi*) {
  if (inside() && sScopes.back().scope == Scope::Scroll) {
    pop_scope();
  }
}

void ui_same_line(const FhUi*) {
  if (inside()) {
    ImGui::SameLine();
  }
}

void ui_separator(const FhUi*) {
  if (inside()) {
    ImGui::Separator();
  }
}

ImGuiKey to_imgui_key(uint32_t key) {
  if (key >= 'a' && key <= 'z') {
    key -= 'a' - 'A';
  }
  if (key >= 'A' && key <= 'Z') {
    return static_cast<ImGuiKey>(ImGuiKey_A + static_cast<int>(key - 'A'));
  }
  if (key >= '0' && key <= '9') {
    return static_cast<ImGuiKey>(ImGuiKey_0 + static_cast<int>(key - '0'));
  }
  if (key >= FH_UI_KEY_F1 && key <= FH_UI_KEY_F12) {
    return static_cast<ImGuiKey>(ImGuiKey_F1 + static_cast<int>(key - FH_UI_KEY_F1));
  }
  return ImGuiKey_None;
}

int ui_key_pressed(const FhUi*, uint32_t key) {
  if (sMod == nullptr || ImGui::GetIO().WantTextInput) {
    return 0;
  }
  const ImGuiKey imguiKey = to_imgui_key(key);
  return imguiKey != ImGuiKey_None && ImGui::IsKeyPressed(imguiKey, false) ? 1 : 0;
}

void ui_draw_text(const FhUi*, float x, float y, uint32_t rgba, const char* text) {
  if (sMod == nullptr || text == nullptr || !std::isfinite(x) || !std::isfinite(y)) {
    return;
  }
  ImFont* font = overlay_font();
  render_text(ImGui::GetBackgroundDrawList(), font, font->FontSize, ImVec2(x, y), 0.f,
              parse_text(text, from_rgba(rgba)));
}

void ui_draw_rect(const FhUi*, float x0, float y0, float x1, float y1, uint32_t rgba, float rounding) {
  if (sMod == nullptr || !std::isfinite(x0) || !std::isfinite(y0) || !std::isfinite(x1) || !std::isfinite(y1)) {
    return;
  }
  const float radius = std::isfinite(rounding) ? std::max(rounding, 0.f) : 0.f;
  ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), from_rgba(rgba), radius);
}

void ui_measure_text(const FhUi*, const char* text, float* outWidth, float* outHeight) {
  ImVec2 size(0.f, 0.f);
  if (sMod != nullptr && text != nullptr) {
    ImFont* font = overlay_font();
    size = render_text(nullptr, font, font->FontSize, ImVec2(0.f, 0.f), 0.f, parse_text(text, 0));
  }
  if (outWidth != nullptr) {
    *outWidth = size.x;
  }
  if (outHeight != nullptr) {
    *outHeight = size.y;
  }
}

FhUi sUi{
    .structSize = static_cast<uint32_t>(sizeof(FhUi)),
    .begin = ui_begin,
    .end = ui_end,
    .text = ui_text,
    .button = ui_button,
    .checkbox = ui_checkbox,
    .slider = ui_slider,
    .input = ui_input,
    .beginSection = ui_begin_section,
    .endSection = ui_end_section,
    .beginScroll = ui_begin_scroll,
    .endScroll = ui_end_scroll,
    .sameLine = ui_same_line,
    .separator = ui_separator,
    .keyPressed = ui_key_pressed,
    .drawText = ui_draw_text,
    .drawRect = ui_draw_rect,
    .measureText = ui_measure_text,
};

void update_frame() {
  const ImVec2 display = ImGui::GetIO().DisplaySize;
  sUi.displayWidth = display.x;
  sUi.displayHeight = display.y;
  sUi.gameX = 0.f;
  sUi.gameY = 0.f;
  sUi.gameWidth = display.x;
  sUi.gameHeight = display.y;

  u32 renderWidth = 0;
  u32 renderHeight = 0;
  AuroraGetRenderSize(&renderWidth, &renderHeight);
  if (renderWidth == 0 || renderHeight == 0 || display.x <= 0.f || display.y <= 0.f) {
    return;
  }
  const float aspect = static_cast<float>(renderWidth) / static_cast<float>(renderHeight);
  float width = display.x;
  float height = width / aspect;
  if (height > display.y) {
    height = display.y;
    width = height * aspect;
  }
  sUi.gameX = (display.x - width) * 0.5f;
  sUi.gameY = (display.y - height) * 0.5f;
  sUi.gameWidth = width;
  sUi.gameHeight = height;
}

input::EventResult layer_event(const input::InputEvent&, void*) {
  return sModal ? input::EventResult::Consume : input::EventResult::Pass;
}

bool layer_captures(const input::InputSource&, void*) { return sModal; }

input::PointerMode layer_pointer(void*) { return sModal ? input::PointerMode::Visible : input::PointerMode::None; }

} // namespace

extern "C" void fhUiInit(void) {
  if (sLayer != input::kInvalidLayerId) {
    return;
  }
  if (ImGui::GetCurrentContext() != nullptr) {
    ImGuiIO& io = ImGui::GetIO();
    if (io.Fonts->Fonts.empty()) {
      io.Fonts->AddFontDefault();
    }
    const AuroraWindowSize size = aurora_get_window_size();
    ImFontConfig config;
    config.FontDataOwnedByAtlas = false;
    if (size.width > 0) {
      config.RasterizerDensity =
          std::max(1.f, static_cast<float>(size.native_fb_width) / static_cast<float>(size.width));
    }
    sFont = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char*>(gFoxhollowSplashFont),
                                           static_cast<int>(gFoxhollowSplashFontSize), kFontSize, &config);
  }
  sLayer = input::register_layer({
      .label = "foxhollow.mods.ui",
      .priority = kLayerPriority,
      .onEvent = layer_event,
      .capturesSource = layer_captures,
      .pointerMode = layer_pointer,
  });
}

extern "C" void fhUiShutdown(void) {
  if (sLayer != input::kInvalidLayerId) {
    input::unregister_layer(sLayer);
    sLayer = input::kInvalidLayerId;
  }
  sModal = false;
  sModalThisFrame = false;
  sFont = nullptr;
  sFooters.clear();
  sActiveInput = 0;
  sActiveInputText.clear();
  sRefocusInput = 0;
}

extern "C" void fhUiFrameBegin(void) {
  sModalThisFrame = false;
  update_frame();
}

extern "C" void fhUiDrawMod(FhMod* mod, const char* modId, FhModDrawFn draw) {
  if (draw == nullptr) {
    return;
  }
  sMod = mod;
  sModId = modId != nullptr ? modId : "";
  if (sFont != nullptr) {
    ImGui::PushFont(sFont);
  }
  draw(mod, &sUi);
  while (!sScopes.empty()) {
    pop_scope();
  }
  if (sFont != nullptr) {
    ImGui::PopFont();
  }
  sMod = nullptr;
}

extern "C" void fhUiFrameEnd(void) {
  if (sModalThisFrame != sModal) {
    sModal = sModalThisFrame;
    input::reconcile();
  }
}
