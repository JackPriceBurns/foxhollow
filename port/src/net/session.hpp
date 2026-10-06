#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace fh::net {

constexpr int32_t kBroadcast = 0;
constexpr uint32_t kMaxPayload = 1024;
constexpr uint32_t kMaxPlayers = 16;
constexpr uint32_t kMaxMessagesPerUpdate = 256;
constexpr size_t kMaxControlBytes = 16384;

struct Player {
  int32_t id = 0;
  std::string name;
};

struct SessionCallbacks {
  std::function<void()> connected;
  std::function<void()> disconnected;
  std::function<void(int32_t, const uint8_t*, uint32_t)> message;
  std::function<void(const Player&)> joined;
  std::function<void(int32_t)> left;
  std::function<void(int32_t)> hostChanged;
};

void session_init(const char* configPath);
void session_update(const SessionCallbacks& callbacks);
void session_shutdown();

const std::string& session_mod_id();
const std::string& session_room_code();
bool session_connected();
int32_t session_local_player();
int32_t session_host_player();
const std::vector<Player>& session_players();
bool session_send(int32_t toPlayer, const void* data, uint32_t size);

} // namespace fh::net
