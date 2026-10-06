#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>

namespace fh::net {

struct SocketEvent {
  enum class Type { Open, Text, Binary, Closed };
  Type type = Type::Closed;
  std::string data;
  int code = 0;
};

class EventQueue {
public:
  void push(SocketEvent event) {
    std::lock_guard lock{mMutex};
    if (mClosed) {
      return;
    }
    if (event.type == SocketEvent::Type::Closed) {
      mClosed = true;
    }
    mEvents.push_back(std::move(event));
  }

  bool pop(SocketEvent& out) {
    std::lock_guard lock{mMutex};
    if (mEvents.empty()) {
      return false;
    }
    out = std::move(mEvents.front());
    mEvents.pop_front();
    return true;
  }

private:
  std::mutex mMutex;
  std::deque<SocketEvent> mEvents;
  bool mClosed = false;
};

class WebSocket {
public:
  virtual ~WebSocket() = default;
  virtual void send_binary(const void* data, size_t size) = 0;
  virtual void send_text(std::string_view text) = 0;
  virtual bool poll(SocketEvent& out) = 0;
};

std::unique_ptr<WebSocket> connect_websocket(const std::string& url, const std::string& bearer);

} // namespace fh::net
