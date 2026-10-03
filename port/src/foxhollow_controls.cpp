#include <dolphin/pad.h>
#include <nlohmann/json.hpp>
#include <SDL3/SDL_gamepad.h>

#include <algorithm>
#include <cmath>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct NativeButtonName {
  std::string_view name;
  SDL_GamepadButton button;
};

struct NativeAxisName {
  std::string_view name;
  SDL_GamepadAxis axis;
};

struct GameButtonName {
  std::string_view name;
  PADButton button;
};

struct KeyAxisName {
  std::string_view name;
  PADAxis axis;
};

constexpr std::array kNativeButtons{
    NativeButtonName{"south", SDL_GAMEPAD_BUTTON_SOUTH},
    NativeButtonName{"east", SDL_GAMEPAD_BUTTON_EAST},
    NativeButtonName{"west", SDL_GAMEPAD_BUTTON_WEST},
    NativeButtonName{"north", SDL_GAMEPAD_BUTTON_NORTH},
    NativeButtonName{"back", SDL_GAMEPAD_BUTTON_BACK},
    NativeButtonName{"guide", SDL_GAMEPAD_BUTTON_GUIDE},
    NativeButtonName{"start", SDL_GAMEPAD_BUTTON_START},
    NativeButtonName{"leftstick", SDL_GAMEPAD_BUTTON_LEFT_STICK},
    NativeButtonName{"rightstick", SDL_GAMEPAD_BUTTON_RIGHT_STICK},
    NativeButtonName{"leftshoulder", SDL_GAMEPAD_BUTTON_LEFT_SHOULDER},
    NativeButtonName{"rightshoulder", SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER},
    NativeButtonName{"dpup", SDL_GAMEPAD_BUTTON_DPAD_UP},
    NativeButtonName{"dpdown", SDL_GAMEPAD_BUTTON_DPAD_DOWN},
    NativeButtonName{"dpleft", SDL_GAMEPAD_BUTTON_DPAD_LEFT},
    NativeButtonName{"dpright", SDL_GAMEPAD_BUTTON_DPAD_RIGHT},
    NativeButtonName{"misc1", SDL_GAMEPAD_BUTTON_MISC1},
};

constexpr std::array kNativeAxes{
    NativeAxisName{"lefttrigger", SDL_GAMEPAD_AXIS_LEFT_TRIGGER},
    NativeAxisName{"righttrigger", SDL_GAMEPAD_AXIS_RIGHT_TRIGGER},
};

constexpr std::array kGameButtons{
    GameButtonName{"A", PAD_BUTTON_A},       GameButtonName{"B", PAD_BUTTON_B},
    GameButtonName{"X", PAD_BUTTON_X},       GameButtonName{"Y", PAD_BUTTON_Y},
    GameButtonName{"Z", PAD_TRIGGER_Z},      GameButtonName{"Start", PAD_BUTTON_START},
    GameButtonName{"Up", PAD_BUTTON_UP},     GameButtonName{"Down", PAD_BUTTON_DOWN},
    GameButtonName{"Left", PAD_BUTTON_LEFT}, GameButtonName{"Right", PAD_BUTTON_RIGHT},
    GameButtonName{"L", PAD_TRIGGER_L},      GameButtonName{"R", PAD_TRIGGER_R},
};

constexpr std::array kKeyAxes{
    KeyAxisName{"MainUp", PAD_AXIS_LEFT_Y_POS},  KeyAxisName{"MainDown", PAD_AXIS_LEFT_Y_NEG},
    KeyAxisName{"MainLeft", PAD_AXIS_LEFT_X_NEG}, KeyAxisName{"MainRight", PAD_AXIS_LEFT_X_POS},
    KeyAxisName{"CUp", PAD_AXIS_RIGHT_Y_POS},     KeyAxisName{"CDown", PAD_AXIS_RIGHT_Y_NEG},
    KeyAxisName{"CLeft", PAD_AXIS_RIGHT_X_NEG},   KeyAxisName{"CRight", PAD_AXIS_RIGHT_X_POS},
    KeyAxisName{"L", PAD_AXIS_TRIGGER_L},         KeyAxisName{"R", PAD_AXIS_TRIGGER_R},
};

struct DeadZoneOverrides {
  std::optional<u16> main;
  std::optional<u16> c;
  std::optional<u16> triggerL;
  std::optional<u16> triggerR;
};

struct Controls {
  std::vector<PADButtonMapping> buttons;
  std::vector<PADAxisMapping> axes;
  DeadZoneOverrides deadZones;
  std::vector<PADKeyButtonBinding> keyButtons;
  std::vector<PADKeyAxisBinding> keyAxes;
};

struct AppliedController {
  s32 index = -1;
  u32 vid = 0;
  u32 pid = 0;
  u32 count = 0;
};

std::optional<Controls> sControls;
AppliedController sApplied;

void log_warning(const std::string& message) { std::fprintf(stderr, "[foxhollow] controls: %s\n", message.c_str()); }

std::optional<SDL_GamepadButton> native_button(std::string_view name) {
  const auto it = std::ranges::find(kNativeButtons, name, &NativeButtonName::name);
  return it == kNativeButtons.end() ? std::nullopt : std::optional{it->button};
}

std::optional<SDL_GamepadAxis> native_trigger(std::string_view name) {
  const auto it = std::ranges::find(kNativeAxes, name, &NativeAxisName::name);
  return it == kNativeAxes.end() ? std::nullopt : std::optional{it->axis};
}

std::optional<PADButton> game_button(std::string_view name) {
  const auto it = std::ranges::find(kGameButtons, name, &GameButtonName::name);
  return it == kGameButtons.end() ? std::nullopt : std::optional{it->button};
}

std::optional<PADAxis> key_axis(std::string_view name) {
  const auto it = std::ranges::find(kKeyAxes, name, &KeyAxisName::name);
  return it == kKeyAxes.end() ? std::nullopt : std::optional{it->axis};
}

PADAxisMapping axis_mapping(SDL_GamepadAxis axis, PADAxisSign sign, PADAxis padAxis) {
  return PADAxisMapping{{static_cast<s32>(axis), sign}, static_cast<s32>(SDL_GAMEPAD_BUTTON_INVALID), padAxis};
}

void add_stick(Controls& controls, std::string_view side, bool c) {
  SDL_GamepadAxis x;
  SDL_GamepadAxis y;
  if (side == "left") {
    x = SDL_GAMEPAD_AXIS_LEFTX;
    y = SDL_GAMEPAD_AXIS_LEFTY;
  } else if (side == "right") {
    x = SDL_GAMEPAD_AXIS_RIGHTX;
    y = SDL_GAMEPAD_AXIS_RIGHTY;
  } else {
    log_warning("unknown stick \"" + std::string(side) + "\"");
    return;
  }
  controls.axes.push_back(axis_mapping(x, AXIS_SIGN_POSITIVE, c ? PAD_AXIS_RIGHT_X_POS : PAD_AXIS_LEFT_X_POS));
  controls.axes.push_back(axis_mapping(x, AXIS_SIGN_NEGATIVE, c ? PAD_AXIS_RIGHT_X_NEG : PAD_AXIS_LEFT_X_NEG));
  controls.axes.push_back(axis_mapping(y, AXIS_SIGN_NEGATIVE, c ? PAD_AXIS_RIGHT_Y_POS : PAD_AXIS_LEFT_Y_POS));
  controls.axes.push_back(axis_mapping(y, AXIS_SIGN_POSITIVE, c ? PAD_AXIS_RIGHT_Y_NEG : PAD_AXIS_LEFT_Y_NEG));
}

void add_trigger(Controls& controls, std::string_view source, PADButton button, PADAxis padAxis) {
  if (const auto axis = native_trigger(source)) {
    controls.axes.push_back(axis_mapping(*axis, AXIS_SIGN_POSITIVE, padAxis));
    controls.buttons.push_back(PADButtonMapping{PAD_NATIVE_BUTTON_INVALID, button});
    return;
  }
  if (const auto native = native_button(source)) {
    controls.axes.push_back(PADAxisMapping{{static_cast<s32>(SDL_GAMEPAD_AXIS_INVALID), AXIS_SIGN_POSITIVE},
                                           static_cast<s32>(*native), padAxis});
    controls.buttons.push_back(PADButtonMapping{static_cast<u32>(*native), button});
    return;
  }
  log_warning("unknown trigger source \"" + std::string(source) + "\"");
}

std::optional<u16> dead_zone(const nlohmann::json& value) {
  if (!value.is_number()) {
    return std::nullopt;
  }
  return static_cast<u16>(std::clamp<long>(std::lround(value.get<double>()), 0, 32767));
}

void parse_gamepad(const nlohmann::json& gamepad, Controls& controls) {
  if (const auto buttons = gamepad.find("buttons"); buttons != gamepad.end() && buttons->is_object()) {
    for (const auto& [name, value] : buttons->items()) {
      const auto padButton = game_button(name);
      if (!padButton || *padButton == PAD_TRIGGER_L || *padButton == PAD_TRIGGER_R) {
        log_warning("unknown gamepad button \"" + name + "\"");
        continue;
      }
      if (value.is_null()) {
        controls.buttons.push_back(PADButtonMapping{PAD_NATIVE_BUTTON_INVALID, *padButton});
        continue;
      }
      const auto native = value.is_string() ? native_button(value.get<std::string>()) : std::nullopt;
      if (!native) {
        log_warning("unknown native button for \"" + name + "\"");
        continue;
      }
      controls.buttons.push_back(PADButtonMapping{static_cast<u32>(*native), *padButton});
    }
  }

  if (const auto triggers = gamepad.find("triggers"); triggers != gamepad.end() && triggers->is_object()) {
    if (const auto left = triggers->find("L"); left != triggers->end() && left->is_string()) {
      add_trigger(controls, left->get<std::string>(), PAD_TRIGGER_L, PAD_AXIS_TRIGGER_L);
    }
    if (const auto right = triggers->find("R"); right != triggers->end() && right->is_string()) {
      add_trigger(controls, right->get<std::string>(), PAD_TRIGGER_R, PAD_AXIS_TRIGGER_R);
    }
  }

  if (const auto sticks = gamepad.find("sticks"); sticks != gamepad.end() && sticks->is_object()) {
    if (const auto main = sticks->find("main"); main != sticks->end() && main->is_string()) {
      add_stick(controls, main->get<std::string>(), false);
    }
    if (const auto c = sticks->find("c"); c != sticks->end() && c->is_string()) {
      add_stick(controls, c->get<std::string>(), true);
    }
  }

  if (const auto zones = gamepad.find("deadzones"); zones != gamepad.end() && zones->is_object()) {
    controls.deadZones.main = dead_zone(zones->value("main", nlohmann::json{}));
    controls.deadZones.c = dead_zone(zones->value("c", nlohmann::json{}));
    controls.deadZones.triggerL = dead_zone(zones->value("triggerL", nlohmann::json{}));
    controls.deadZones.triggerR = dead_zone(zones->value("triggerR", nlohmann::json{}));
  }
}

s32 scancode(const nlohmann::json& value) {
  return value.is_number_integer() ? value.get<s32>() : PAD_KEY_INVALID;
}

void parse_keyboard(const nlohmann::json& keyboard, Controls& controls) {
  if (const auto buttons = keyboard.find("buttons"); buttons != keyboard.end() && buttons->is_object()) {
    for (const auto& [name, value] : buttons->items()) {
      if (const auto padButton = game_button(name)) {
        controls.keyButtons.push_back(PADKeyButtonBinding{scancode(value), *padButton});
      } else {
        log_warning("unknown keyboard button \"" + name + "\"");
      }
    }
  }
  if (const auto axes = keyboard.find("axes"); axes != keyboard.end() && axes->is_object()) {
    for (const auto& [name, value] : axes->items()) {
      if (const auto padAxis = key_axis(name)) {
        controls.keyAxes.push_back(PADKeyAxisBinding{scancode(value), *padAxis, 0});
      } else {
        log_warning("unknown keyboard axis \"" + name + "\"");
      }
    }
  }
}

std::optional<Controls> load_controls(const char* path) {
  std::ifstream file(path);
  if (!file) {
    log_warning(std::string("could not open ") + path);
    return std::nullopt;
  }
  const auto json = nlohmann::json::parse(file, nullptr, false);
  if (json.is_discarded() || !json.is_object()) {
    log_warning(std::string(path) + " is not valid JSON");
    return std::nullopt;
  }
  if (json.value("version", 0) != 1) {
    log_warning(std::string(path) + " has an unsupported version");
    return std::nullopt;
  }
  Controls controls;
  if (const auto gamepad = json.find("gamepad"); gamepad != json.end() && gamepad->is_object()) {
    parse_gamepad(*gamepad, controls);
  }
  if (const auto keyboard = json.find("keyboard"); keyboard != json.end() && keyboard->is_object()) {
    parse_keyboard(*keyboard, controls);
  }
  return controls;
}

void apply_gamepad(const Controls& controls) {
  for (const auto& mapping : controls.buttons) {
    PADSetButtonMapping(PAD_CHAN0, mapping);
  }
  for (const auto& mapping : controls.axes) {
    PADSetAxisMapping(PAD_CHAN0, mapping);
  }
  if (auto* zones = PADGetDeadZones(PAD_CHAN0)) {
    zones->stickDeadZone = controls.deadZones.main.value_or(zones->stickDeadZone);
    zones->substickDeadZone = controls.deadZones.c.value_or(zones->substickDeadZone);
    zones->leftTriggerActivationZone = controls.deadZones.triggerL.value_or(zones->leftTriggerActivationZone);
    zones->rightTriggerActivationZone = controls.deadZones.triggerR.value_or(zones->rightTriggerActivationZone);
  }
}

} // namespace

extern "C" void fhControlsInit(void) {
  const char* path = std::getenv("FOXHOLLOW_CONTROLS");
  if (path == nullptr || *path == '\0') {
    return;
  }
  sControls = load_controls(path);
  if (!sControls) {
    return;
  }
  for (const auto& binding : sControls->keyButtons) {
    PADSetKeyButtonBinding(PAD_CHAN0, binding);
  }
  for (const auto& binding : sControls->keyAxes) {
    PADSetKeyAxisBinding(PAD_CHAN0, binding);
  }
  std::fprintf(stderr, "[foxhollow] controls: loaded %s\n", path);
}

extern "C" void fhControlsUpdate(void) {
  if (!sControls) {
    return;
  }
  AppliedController current{.index = PADGetIndexForPort(PAD_CHAN0), .count = PADCount()};
  if (current.index >= 0) {
    PADGetVidPid(PAD_CHAN0, &current.vid, &current.pid);
  }
  if (current.index == sApplied.index && current.vid == sApplied.vid && current.pid == sApplied.pid &&
      current.count == sApplied.count) {
    return;
  }
  sApplied = current;
  if (current.index >= 0 && !PADIsGCAdapter(PAD_CHAN0)) {
    apply_gamepad(*sControls);
  }
}
