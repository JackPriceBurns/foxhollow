#include <aurora/imgui.h>
#include <SDL3/SDL.h>

#include <dolphin/pad.h>

#include <cfloat>

#include "foxhollow_cheats.h"

namespace {

constexpr float kMenuWidth = 380.0f;
constexpr float kMenuMargin = 24.0f;
constexpr float kMenuFontScale = 1.2f;

bool sOpen;
bool sToggleKeyWasDown;
bool sPadBlocked;

void set_pad_blocked(bool blocked) {
  if (blocked == sPadBlocked) {
    return;
  }
  sPadBlocked = blocked;
  PADBlockInput(blocked);
}

void draw_menu_contents() {
  ImGui::SetWindowFontScale(kMenuFontScale);

  if (!fhCheatsArwingActive()) {
    ImGui::SeparatorText("PLAYER");
    bool godMode = fhCheatsGodModeEnabled() != 0;
    if (ImGui::Checkbox("God Mode", &godMode)) {
      fhCheatsSetGodMode(godMode ? 1 : 0);
    }
    bool infiniteMagic = fhCheatsInfiniteMagicEnabled() != 0;
    if (ImGui::Checkbox("Infinite Magic", &infiniteMagic)) {
      fhCheatsSetInfiniteMagic(infiniteMagic ? 1 : 0);
    }
    bool fastRun = fhCheatsFastRunEnabled() != 0;
    if (ImGui::Checkbox("Fast Movement (2x)", &fastRun)) {
      fhCheatsSetFastRun(fastRun ? 1 : 0);
    }
    if (fhCheatsPlayerIsFox() && ImGui::Button("Give All Abilities", ImVec2(-FLT_MIN, 0.0f))) {
      fhCheatsGiveAllStaffAbilities();
    }

    if (fhCheatsPlayerIsFox()) {
      ImGui::SeparatorText("ITEMS");
      bool infiniteItems = fhCheatsInfiniteItemsEnabled() != 0;
      if (ImGui::Checkbox("Infinite Items", &infiniteItems)) {
        fhCheatsSetInfiniteItems(infiniteItems ? 1 : 0);
      }
      if (fhCheatsTrickyPresent()) {
        bool infiniteTrickyEnergy = fhCheatsInfiniteTrickyEnergyEnabled() != 0;
        if (ImGui::Checkbox("Infinite Tricky Energy", &infiniteTrickyEnergy)) {
          fhCheatsSetInfiniteTrickyEnergy(infiniteTrickyEnergy ? 1 : 0);
        }
      }
      if (ImGui::Button("Give All Items", ImVec2(-FLT_MIN, 0.0f))) {
        fhCheatsGiveAllItems();
      }
    }
  }

  if (fhCheatsArwingActive()) {
    ImGui::SeparatorText("ARWING");
    bool arwingGodMode = fhCheatsArwingGodModeEnabled() != 0;
    if (ImGui::Checkbox("God Mode##arwing", &arwingGodMode)) {
      fhCheatsSetArwingGodMode(arwingGodMode ? 1 : 0);
    }
    bool infiniteBombs = fhCheatsArwingInfiniteBombsEnabled() != 0;
    if (ImGui::Checkbox("Infinite Bombs", &infiniteBombs)) {
      fhCheatsSetArwingInfiniteBombs(infiniteBombs ? 1 : 0);
    }
    bool rapidFire = fhCheatsArwingRapidFireEnabled() != 0;
    if (ImGui::Checkbox("Rapid Fire", &rapidFire)) {
      fhCheatsSetArwingRapidFire(rapidFire ? 1 : 0);
    }
    bool maxRings = fhCheatsArwingMaxRingsEnabled() != 0;
    if (ImGui::Checkbox("Max Rings", &maxRings)) {
      fhCheatsSetArwingMaxRings(maxRings ? 1 : 0);
    }
  }

  if (fhCheatsCloudRunnerActive()) {
    ImGui::SeparatorText("CLOUDRUNNER");
    bool cloudRunnerRapidFire = fhCheatsCloudRunnerRapidFireEnabled() != 0;
    if (ImGui::Checkbox("Rapid Fire##cloudrunner", &cloudRunnerRapidFire)) {
      fhCheatsSetCloudRunnerRapidFire(cloudRunnerRapidFire ? 1 : 0);
    }
  }

  ImGui::Separator();
  ImGui::TextDisabled("F11 - Close");
}

void draw_menu() {
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  const ImVec2 anchor(viewport->WorkPos.x + viewport->WorkSize.x - kMenuMargin, viewport->WorkPos.y + kMenuMargin);
  bool open = true;

  ImGui::SetNextWindowPos(anchor, ImGuiCond_Appearing, ImVec2(1.0f, 0.0f));
  ImGui::SetNextWindowSizeConstraints(ImVec2(kMenuWidth, 0.0f), ImVec2(kMenuWidth, FLT_MAX));
  ImGui::SetNextWindowBgAlpha(0.9f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.0f, 12.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
  if (ImGui::Begin("FoxHollow Cheats", &open,
                   ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                       ImGuiWindowFlags_NoCollapse)) {
    draw_menu_contents();
  }
  ImGui::End();
  ImGui::PopStyleVar(2);
  sOpen = open;
}

}  // namespace

extern "C" void fhCheatsDrawOverlay(void) {
  int keyCount = 0;
  const bool* keys = SDL_GetKeyboardState(&keyCount);
  const bool toggleKeyDown = keys != nullptr && keyCount > SDL_SCANCODE_F11 && keys[SDL_SCANCODE_F11];
  const bool toggled = toggleKeyDown && !sToggleKeyWasDown;
  sToggleKeyWasDown = toggleKeyDown;
  const bool gameplay = fhCheatsGameplayActive() != 0;
  if (gameplay) {
    fhCheatsUpdate();
  }

  if (!gameplay) {
    sOpen = false;
  } else if (toggled) {
    sOpen = !sOpen;
  }

  if (sOpen) {
    draw_menu();
  }
  set_pad_blocked(sOpen || ImGui::GetIO().WantTextInput);
}
