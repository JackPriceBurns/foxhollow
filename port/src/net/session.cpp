#include "session.hpp"

#include "websocket.hpp"

#include "foxhollow_quit.h"

#include <nlohmann/json.hpp>

#include <SDL3/SDL_timer.h>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <chrono>
#include <filesystem>
#include <memory>
#include <optional>

namespace fh::net {
namespace {

namespace fs = std::filesystem;

constexpr uint64_t kConfigPollMs = 500;
constexpr uint64_t kInitialBackoffMs = 1000;
constexpr uint64_t kMaxBackoffMs = 30000;
constexpr uint64_t kPingIntervalMs = 20000;

struct Target {
  std::string modId;
  std::string room;
  std::string url;
  std::string ticket;
  uint64_t expiresAt = 0;

  bool operator==(const Target&) const = default;
};

std::string sConfigPath;
std::optional<fs::file_time_type> sConfigTime;
uint64_t sNextConfigPoll = 0;
std::optional<Target> sTarget;
std::string sModId;
std::string sRoom;

std::unique_ptr<WebSocket> sSocket;
bool sOpen = false;
bool sWelcomed = false;
uint64_t sNextAttempt = 0;
uint64_t sBackoff = kInitialBackoffMs;
uint64_t sNextPing = 0;
bool sFatal = false;

int32_t sLocal = 0;
int32_t sHost = 0;
std::vector<Player> sPlayers;

void log_line(const char* message) { std::fprintf(stderr, "[foxhollow] multiplayer: %s\n", message); }

uint64_t now_ms() { return SDL_GetTicks(); }

fs::path utf8_path(const std::string& text) { return fs::path(std::u8string(text.begin(), text.end())); }

uint64_t unix_seconds() {
  return static_cast<uint64_t>(
      std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

bool relay_url_allowed(const std::string& url) {
  if (url.rfind("wss://", 0) == 0) {
    return true;
  }
  if (url.rfind("ws://", 0) != 0) {
    return false;
  }
  const std::string rest = url.substr(5);
  const std::string host = rest.substr(0, rest.find_first_of(":/"));
  if (rest.rfind("[::1]", 0) == 0) {
    return true;
  }
  return host == "127.0.0.1" || host == "localhost";
}

std::optional<Target> read_target() {
  std::error_code error;
  const fs::path path = utf8_path(sConfigPath);
  if (!fs::exists(path, error)) {
    return std::nullopt;
  }
  std::FILE* file = std::fopen(sConfigPath.c_str(), "rb");
  if (file == nullptr) {
    return sTarget;
  }
  std::string contents;
  char buffer[4096];
  size_t read = 0;
  while ((read = std::fread(buffer, 1, sizeof(buffer), file)) > 0) {
    contents.append(buffer, read);
  }
  std::fclose(file);
  const auto json = nlohmann::json::parse(contents, nullptr, false);
  if (json.is_discarded() || !json.is_object()) {
    return sTarget;
  }
  const auto relayUrl = json.value("relayUrl", std::string{});
  const auto ticket = json.value("ticket", std::string{});
  const auto modId = json.value("modId", std::string{});
  const auto room = json.value("room", std::string{});
  if (relayUrl.empty() || ticket.empty() || modId.empty()) {
    return std::nullopt;
  }
  if (!relay_url_allowed(relayUrl)) {
    log_line("refusing a relay that is not wss:// (plain ws:// is only allowed for localhost)");
    return std::nullopt;
  }
  std::string base = relayUrl;
  while (!base.empty() && base.back() == '/') {
    base.pop_back();
  }
  return Target{modId, room, base + "/v1/connect", ticket, json.value("expiresAt", uint64_t{0})};
}

void reset_room_state() {
  sOpen = false;
  sWelcomed = false;
  sLocal = 0;
  sHost = 0;
  sPlayers.clear();
}

void disconnect(const SessionCallbacks& callbacks) {
  const bool wasWelcomed = sWelcomed;
  sSocket.reset();
  reset_room_state();
  if (wasWelcomed && callbacks.disconnected) {
    callbacks.disconnected();
  }
}

void poll_config(const SessionCallbacks& callbacks) {
  if (sConfigPath.empty() || now_ms() < sNextConfigPoll) {
    return;
  }
  sNextConfigPoll = now_ms() + kConfigPollMs;
  std::error_code error;
  const auto time = fs::last_write_time(utf8_path(sConfigPath), error);
  const std::optional<fs::file_time_type> stamp = error ? std::nullopt : std::optional{time};
  if (stamp == sConfigTime) {
    return;
  }
  sConfigTime = stamp;
  auto target = read_target();
  if (target == sTarget) {
    return;
  }
  if (target && sTarget && target->url == sTarget->url && target->room == sTarget->room &&
      target->modId == sTarget->modId) {
    sTarget->ticket = target->ticket;
    sTarget->expiresAt = target->expiresAt;
    if (sFatal && !sSocket) {
      sFatal = false;
      sBackoff = kInitialBackoffMs;
      sNextAttempt = 0;
    }
    return;
  }
  disconnect(callbacks);
  sTarget = std::move(target);
  sModId = sTarget ? sTarget->modId : std::string{};
  sRoom = sTarget ? sTarget->room : std::string{};
  sBackoff = kInitialBackoffMs;
  sNextAttempt = 0;
  sFatal = false;
  log_line(sTarget ? "room changed" : "left the room");
}

void handle_text(const std::string& text, const SessionCallbacks& callbacks) {
  if (text == "pong") {
    return;
  }
  const auto message = nlohmann::json::parse(text, nullptr, false);
  if (message.is_discarded() || !message.is_object()) {
    return;
  }
  const auto type = message.value("t", std::string{});
  if (type == "welcome") {
    sLocal = message.value("you", 0);
    sHost = message["host"].is_number() ? message["host"].get<int32_t>() : 0;
    sPlayers.clear();
    for (const auto& player : message.value("players", nlohmann::json::array())) {
      if (!player.is_object() || sPlayers.size() >= kMaxPlayers) {
        continue;
      }
      const int32_t id = player.value("id", 0);
      if (id >= 1 && id <= 255) {
        sPlayers.push_back({id, player.value("name", std::string{})});
      }
    }
    sWelcomed = true;
    sBackoff = kInitialBackoffMs;
    log_line("connected to the room");
    if (callbacks.connected) {
      callbacks.connected();
    }
    for (const auto& player : sPlayers) {
      if (player.id != sLocal && callbacks.joined) {
        callbacks.joined(player);
      }
    }
  } else if (type == "join") {
    const auto& data = message["player"];
    if (!data.is_object()) {
      return;
    }
    Player player{data.value("id", 0), data.value("name", std::string{})};
    if (player.id < 1 || player.id > 255 || (sPlayers.size() >= kMaxPlayers &&
                                              std::ranges::none_of(sPlayers, [&](const Player& p) { return p.id == player.id; }))) {
      return;
    }
    std::erase_if(sPlayers, [&](const Player& existing) { return existing.id == player.id; });
    sPlayers.push_back(player);
    std::ranges::sort(sPlayers, {}, &Player::id);
    if (callbacks.joined) {
      callbacks.joined(player);
    }
  } else if (type == "leave") {
    const int32_t id = message.value("id", 0);
    std::erase_if(sPlayers, [&](const Player& existing) { return existing.id == id; });
    if (callbacks.left) {
      callbacks.left(id);
    }
  } else if (type == "host") {
    sHost = message["id"].is_number() ? message["id"].get<int32_t>() : 0;
    if (callbacks.hostChanged) {
      callbacks.hostChanged(sHost);
    }
  } else if (type == "kicked") {
    sFatal = true;
    log_line("the room's owner removed you; closing the game");
    disconnect(callbacks);
    fhRequestQuit();
  } else if (type == "error") {
    std::fprintf(stderr, "[foxhollow] multiplayer: relay error: %s\n", message.value("message", std::string{}).c_str());
  }
}

void handle_closed(int code, const SessionCallbacks& callbacks) {
  std::fprintf(stderr, "[foxhollow] multiplayer: connection closed (%d)\n", code);
  disconnect(callbacks);
  if (code == 4001) {
    sFatal = true;
    log_line("the room's owner removed you; closing the game");
    fhRequestQuit();
    return;
  }
  if (code == 4900) {
    sFatal = true;
    log_line("this system's libcurl was built without WebSocket support; multiplayer is unavailable");
    return;
  }
  if (code == 401 || code == 403 || code == 409) {
    sFatal = true;
    log_line(code == 403 ? "the room is full, or its owner removed you"
             : code == 409 ? "the room is for a different mod version"
                           : "the room ticket was refused");
    return;
  }
  sNextAttempt = now_ms() + sBackoff;
  sBackoff = std::min(sBackoff * 2, kMaxBackoffMs);
}

} // namespace

void session_init(const char* configPath) {
  sConfigPath = configPath != nullptr ? configPath : "";
}

void session_update(const SessionCallbacks& callbacks) {
  poll_config(callbacks);
  if (!sTarget || sFatal) {
    return;
  }
  if (sTarget->expiresAt != 0 && unix_seconds() >= sTarget->expiresAt) {
    if (sSocket) {
      disconnect(callbacks);
      log_line("the room ticket has expired");
    }
    return;
  }
  if (!sSocket) {
    if (now_ms() < sNextAttempt) {
      return;
    }
    sSocket = connect_websocket(sTarget->url, sTarget->ticket);
    reset_room_state();
    sNextPing = now_ms() + kPingIntervalMs;
  }
  SocketEvent event;
  uint32_t delivered = 0;
  while (sSocket && delivered < kMaxMessagesPerUpdate && sSocket->poll(event)) {
    ++delivered;
    switch (event.type) {
    case SocketEvent::Type::Open:
      sOpen = true;
      sSocket->send_text("ping");
      sNextPing = now_ms() + kPingIntervalMs;
      break;
    case SocketEvent::Type::Text:
      if (event.data.size() <= kMaxControlBytes) {
        handle_text(event.data, callbacks);
      }
      if (!sSocket) {
        return;
      }
      break;
    case SocketEvent::Type::Binary:
      if (!event.data.empty() && event.data.size() <= kMaxPayload + 1 && callbacks.message) {
        callbacks.message(static_cast<uint8_t>(event.data[0]), reinterpret_cast<const uint8_t*>(event.data.data()) + 1,
                          static_cast<uint32_t>(event.data.size() - 1));
      }
      break;
    case SocketEvent::Type::Closed:
      handle_closed(event.code, callbacks);
      return;
    }
  }
  if (sSocket && sOpen && now_ms() >= sNextPing) {
    sSocket->send_text("ping");
    sNextPing = now_ms() + kPingIntervalMs;
  }
}

void session_shutdown() {
  sSocket.reset();
  reset_room_state();
}

const std::string& session_mod_id() { return sModId; }
const std::string& session_room_code() { return sRoom; }
bool session_connected() { return sWelcomed; }
int32_t session_local_player() { return sWelcomed ? sLocal : 0; }
int32_t session_host_player() { return sWelcomed ? sHost : 0; }
const std::vector<Player>& session_players() { return sPlayers; }

bool session_send(int32_t toPlayer, const void* data, uint32_t size) {
  if (!sWelcomed || !sSocket || toPlayer < 0 || toPlayer > 255 || size > kMaxPayload || (size > 0 && data == nullptr)) {
    return false;
  }
  std::string frame(size + 1, '\0');
  frame[0] = static_cast<char>(toPlayer);
  if (size > 0) {
    std::memcpy(frame.data() + 1, data, size);
  }
  sSocket->send_binary(frame.data(), frame.size());
  return true;
}

} // namespace fh::net
