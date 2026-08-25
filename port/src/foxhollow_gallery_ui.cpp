#include <aurora/imgui.h>
#include <SDL3/SDL.h>

#include <dolphin/gx/GXAurora.h>

#include "foxhollow_gallery.h"

#include <cmath>
#include <cstdio>
#include <cstring>

extern "C" {
extern float gCameraModelViewMatrix[3][4];
extern float gCameraFovY;
extern float gCameraAspectRatio;
extern float playerMapOffsetX;
extern float playerMapOffsetZ;
}

namespace {

constexpr float kPi = 3.14159265358979323846f;

struct ContentRect {
  float left;
  float top;
  float width;
  float height;
};

char sFilter[64];
bool sVisible = true;
bool sToggleKeyWasDown;

ContentRect content_rect() {
  const ImVec2 display = ImGui::GetIO().DisplaySize;
  uint32_t renderWidth = 0;
  uint32_t renderHeight = 0;
  AuroraGetRenderSize(&renderWidth, &renderHeight);
  if (renderWidth == 0 || renderHeight == 0 || display.x <= 0.0f || display.y <= 0.0f) {
    return {0.0f, 0.0f, display.x, display.y};
  }

  const float contentAspect = static_cast<float>(renderWidth) / static_cast<float>(renderHeight);
  float width = display.x;
  float height = width / contentAspect;
  if (height > display.y) {
    height = display.y;
    width = height * contentAspect;
  }
  return {(display.x - width) * 0.5f, (display.y - height) * 0.5f, width, height};
}

bool project(float worldX, float worldY, float worldZ, ImVec2& out) {
  const float x = worldX - playerMapOffsetX;
  const float y = worldY;
  const float z = worldZ - playerMapOffsetZ;
  const float(*m)[4] = gCameraModelViewMatrix;
  const float vx = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3];
  const float vy = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3];
  const float vz = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3];
  const float depth = -vz;

  if (!(depth > 1.0f)) {
    return false;
  }
  const float tanHalf = std::tan(gCameraFovY * 0.5f * kPi / 180.0f);
  if (!(tanHalf > 0.0f) || !(gCameraAspectRatio > 0.0f)) {
    return false;
  }

  const float ndcX = vx / (depth * tanHalf * gCameraAspectRatio);
  const float ndcY = vy / (depth * tanHalf);
  if (ndcX < -1.6f || ndcX > 1.6f || ndcY < -1.6f || ndcY > 1.6f) {
    return false;
  }

  const ContentRect rect = content_rect();
  out.x = rect.left + (ndcX * 0.5f + 0.5f) * rect.width;
  out.y = rect.top + (1.0f - (ndcY * 0.5f + 0.5f)) * rect.height;
  return true;
}

void draw_labels() {
  int count = 0;
  const FhGallerySpawn* spawns = fhGallerySpawns(&count);
  if (spawns == nullptr || count == 0) {
    return;
  }

  ImDrawList* draw = ImGui::GetBackgroundDrawList();
  for (int i = 0; i < count; i++) {
    if (!spawns[i].alive) {
      continue;
    }
    ImVec2 screen;
    if (!project(spawns[i].x, spawns[i].y + 80.0f, spawns[i].z, screen)) {
      continue;
    }

    char text[96];
    std::snprintf(text, sizeof(text), "%s\n#%d  dll %d", spawns[i].name != nullptr ? spawns[i].name : "?",
                  spawns[i].defId, spawns[i].dllId);

    const ImVec2 size = ImGui::CalcTextSize(text);
    const ImVec2 topLeft(screen.x - size.x * 0.5f - 4.0f, screen.y - size.y - 4.0f);
    const ImVec2 bottomRight(topLeft.x + size.x + 8.0f, topLeft.y + size.y + 4.0f);
    draw->AddRectFilled(topLeft, bottomRight, IM_COL32(0, 0, 0, 150), 3.0f);
    draw->AddText(ImVec2(topLeft.x + 4.0f, topLeft.y + 2.0f), IM_COL32(255, 235, 140, 255), text);
    draw->AddLine(ImVec2(screen.x, bottomRight.y), ImVec2(screen.x, screen.y + 10.0f), IM_COL32(255, 235, 140, 120));
  }
}

bool matches_filter(const char* name, int defId) {
  if (sFilter[0] == '\0') {
    return true;
  }
  char needle[64];
  char haystack[64];
  std::snprintf(haystack, sizeof(haystack), "%s %d", name != nullptr ? name : "", defId);
  std::snprintf(needle, sizeof(needle), "%s", sFilter);
  for (char* p = haystack; *p != '\0'; p++) {
    if (*p >= 'A' && *p <= 'Z') {
      *p = static_cast<char>(*p - 'A' + 'a');
    }
  }
  for (char* p = needle; *p != '\0'; p++) {
    if (*p >= 'A' && *p <= 'Z') {
      *p = static_cast<char>(*p - 'A' + 'a');
    }
  }
  return std::strstr(haystack, needle) != nullptr;
}

void draw_panel() {
  FhGalleryStatus status;
  fhGalleryGetStatus(&status);

  ImGui::SetNextWindowPos(ImVec2(10.0f, 120.0f), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(340.0f, 470.0f), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowBgAlpha(0.88f);
  if (!ImGui::Begin("Object Gallery", nullptr, ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::End();
    return;
  }

  if (!status.ready) {
    ImGui::TextWrapped("Waiting for gameplay. Start a game, then the gallery can spawn objects.");
    ImGui::End();
    return;
  }

  ImGui::Text("%d of %d definitions available here", status.candidateCount, status.catalogueCount);
  if (status.skippedCount > 0) {
    ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.4f, 1.0f), "%d on the crash skip list", status.skippedCount);
  }
  ImGui::Separator();

  if (ImGui::Button("Spawn page")) {
    fhGalleryRespawn();
  }
  ImGui::SameLine();
  if (ImGui::Button("Clear")) {
    fhGalleryClear();
  }
  ImGui::SameLine();
  if (ImGui::Button("Open area")) {
    fhGalleryWarpToFlatMap();
  }

  int page = status.page;
  const int pageCount = status.pageCount > 0 ? status.pageCount : 1;
  ImGui::Text("Page %d / %d", page + 1, pageCount);
  ImGui::SameLine();
  if (ImGui::ArrowButton("##prev", ImGuiDir_Left) && page > 0) {
    fhGallerySetPage(page - 1);
  }
  ImGui::SameLine();
  if (ImGui::ArrowButton("##next", ImGuiDir_Right) && page + 1 < pageCount) {
    fhGallerySetPage(page + 1);
  }

  int pageSize = status.pageSize;
  if (ImGui::SliderInt("Per page", &pageSize, 1, 120)) {
    fhGallerySetPageSize(pageSize);
  }
  float spacing = status.spacing;
  if (ImGui::SliderFloat("Spacing", &spacing, 60.0f, 900.0f, "%.0f")) {
    fhGallerySetSpacing(spacing);
  }

  bool freeze = status.freeze != 0;
  if (ImGui::Checkbox("Freeze object logic", &freeze)) {
    fhGallerySetFreeze(freeze ? 1 : 0);
  }
  bool labels = status.showLabels != 0;
  if (ImGui::Checkbox("Show labels", &labels)) {
    fhGallerySetShowLabels(labels ? 1 : 0);
  }

  ImGui::Separator();
  ImGui::InputText("Filter", sFilter, sizeof(sFilter));

  int spawnCount = 0;
  const FhGallerySpawn* spawns = fhGallerySpawns(&spawnCount);
  if (ImGui::BeginChild("##spawned", ImVec2(0.0f, 150.0f), true)) {
    for (int i = 0; i < spawnCount; i++) {
      if (!spawns[i].alive) {
        continue;
      }
      char label[96];
      std::snprintf(label, sizeof(label), "%-12s #%-5d seq %-5d dll %-4d##s%d", spawns[i].name, spawns[i].defId,
                    spawns[i].seqId, spawns[i].dllId, i);
      if (ImGui::Selectable(label)) {
        fhGalleryTeleportToSlot(spawns[i].slot);
      }
    }
  }
  ImGui::EndChild();

  ImGui::Text("All available objects");
  if (ImGui::BeginChild("##catalogue", ImVec2(0.0f, 0.0f), true)) {
    const int total = fhGalleryCandidateCount();
    for (int i = 0; i < total; i++) {
      int defId = 0;
      int seqId = 0;
      int dllId = 0;
      const char* name = nullptr;
      if (!fhGalleryCandidateInfo(i, &defId, &seqId, &dllId, &name)) {
        continue;
      }
      if (!matches_filter(name, defId)) {
        continue;
      }
      char label[96];
      std::snprintf(label, sizeof(label), "%-12s #%-5d seq %-5d dll %-4d##c%d", name, defId, seqId, dllId, i);
      if (ImGui::Selectable(label)) {
        fhGallerySetPage(fhGalleryFindCandidatePage(i));
      }
    }
  }
  ImGui::EndChild();

  ImGui::End();
}

}  // namespace

extern "C" void fhGalleryDrawOverlay(void) {
  if (!fhGalleryEnabled()) {
    return;
  }

  int keyCount = 0;
  const bool* keys = SDL_GetKeyboardState(&keyCount);
  const bool toggleKeyDown = keys != nullptr && keyCount > SDL_SCANCODE_G && keys[SDL_SCANCODE_G];
  if (toggleKeyDown && !sToggleKeyWasDown) {
    sVisible = !sVisible;
  }
  sToggleKeyWasDown = toggleKeyDown;

  FhGalleryStatus status;
  fhGalleryGetStatus(&status);
  if (status.showLabels) {
    draw_labels();
  }
  if (sVisible) {
    draw_panel();
  }
}
