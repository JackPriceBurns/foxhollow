#include "websocket.hpp"

#include <curl/curl.h>

#include <atomic>
#include <chrono>
#include <poll.h>
#include <string_view>
#include <thread>
#include <vector>

namespace fh::net {
namespace {

constexpr long kUpgradeTimeoutSeconds = 15;
constexpr int kUnsupportedCode = 4900;

bool curl_supports_websockets() {
  const curl_version_info_data* info = curl_version_info(CURLVERSION_NOW);
  if (info == nullptr || info->protocols == nullptr) {
    return false;
  }
  for (const char* const* protocol = info->protocols; *protocol != nullptr; ++protocol) {
    if (std::string_view{*protocol} == "wss" || std::string_view{*protocol} == "ws") {
      return true;
    }
  }
  return false;
}

int abort_when_stopping(void* clientp, curl_off_t, curl_off_t, curl_off_t, curl_off_t) {
  return static_cast<std::atomic<bool>*>(clientp)->load() ? 1 : 0;
}

struct Outgoing {
  bool binary;
  std::string data;
};

class CurlWebSocket final : public WebSocket {
public:
  CurlWebSocket(const std::string& url, const std::string& bearer) : mQueue(std::make_shared<EventQueue>()) {
    mThread = std::thread([this, url, bearer] { run(url, bearer); });
  }

  ~CurlWebSocket() override {
    mStopping = true;
    if (mThread.joinable()) {
      mThread.join();
    }
  }

  void send_binary(const void* data, size_t size) override {
    std::lock_guard lock{mOutgoingMutex};
    mOutgoing.push_back({true, std::string(static_cast<const char*>(data), size)});
  }

  void send_text(std::string_view text) override {
    std::lock_guard lock{mOutgoingMutex};
    mOutgoing.push_back({false, std::string(text)});
  }

  bool poll(SocketEvent& out) override { return mQueue->pop(out); }

private:
  void fail(int code) { mQueue->push({SocketEvent::Type::Closed, {}, code}); }

  bool flush(CURL* curl) {
    std::deque<Outgoing> pending;
    {
      std::lock_guard lock{mOutgoingMutex};
      pending.swap(mOutgoing);
    }
    for (const auto& message : pending) {
      size_t offset = 0;
      while (offset < message.data.size() || (offset == 0 && message.data.empty())) {
        size_t sent = 0;
        const CURLcode result = curl_ws_send(curl, message.data.data() + offset, message.data.size() - offset, &sent,
                                             0, message.binary ? CURLWS_BINARY : CURLWS_TEXT);
        if (result == CURLE_AGAIN) {
          if (mStopping) {
            return false;
          }
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
          continue;
        }
        if (result != CURLE_OK) {
          return false;
        }
        offset += sent;
        if (message.data.empty()) {
          break;
        }
      }
    }
    return true;
  }

  void run(const std::string& url, const std::string& bearer) {
    if (!curl_supports_websockets()) {
      fail(kUnsupportedCode);
      return;
    }
    CURL* curl = curl_easy_init();
    if (curl == nullptr) {
      fail(1006);
      return;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, kUpgradeTimeoutSeconds);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, abort_when_stopping);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &mStopping);
    curl_slist* headers = nullptr;
    if (!bearer.empty()) {
      const std::string authorization = "Authorization: Bearer " + bearer;
      headers = curl_slist_append(headers, authorization.c_str());
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }
    const CURLcode connected = curl_easy_perform(curl);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, nullptr);
    curl_slist_free_all(headers);
    if (connected != CURLE_OK) {
      long status = 0;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
      fail(status > 0 && status != 101 ? static_cast<int>(status) : 1006);
      curl_easy_cleanup(curl);
      return;
    }
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0L);
    curl_socket_t socket = CURL_SOCKET_BAD;
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket);
    mQueue->push({SocketEvent::Type::Open, {}, 0});

    std::vector<char> buffer(16384);
    std::string message;
    while (!mStopping) {
      if (!flush(curl)) {
        fail(1006);
        break;
      }
      pollfd descriptor{socket, POLLIN, 0};
      ::poll(&descriptor, 1, 10);
      bool closed = false;
      while (true) {
        size_t received = 0;
        const curl_ws_frame* meta = nullptr;
        const CURLcode result = curl_ws_recv(curl, buffer.data(), buffer.size(), &received, &meta);
        if (result == CURLE_AGAIN) {
          break;
        }
        if (result != CURLE_OK || meta == nullptr) {
          fail(1006);
          closed = true;
          break;
        }
        if (meta->flags & CURLWS_CLOSE) {
          int code = 1005;
          if (received >= 2) {
            code = (static_cast<unsigned char>(buffer[0]) << 8) | static_cast<unsigned char>(buffer[1]);
          }
          fail(code);
          closed = true;
          break;
        }
        message.append(buffer.data(), received);
        if (meta->bytesleft == 0 && !(meta->flags & CURLWS_CONT)) {
          const bool binary = (meta->flags & CURLWS_BINARY) != 0;
          mQueue->push({binary ? SocketEvent::Type::Binary : SocketEvent::Type::Text, std::move(message), 0});
          message.clear();
        }
      }
      if (closed) {
        break;
      }
    }
    if (mStopping) {
      size_t sent = 0;
      curl_ws_send(curl, "", 0, &sent, 0, CURLWS_CLOSE);
    }
    curl_easy_cleanup(curl);
  }

  std::shared_ptr<EventQueue> mQueue;
  std::thread mThread;
  std::atomic<bool> mStopping{false};
  std::mutex mOutgoingMutex;
  std::deque<Outgoing> mOutgoing;
};

} // namespace

std::unique_ptr<WebSocket> connect_websocket(const std::string& url, const std::string& bearer) {
  return std::make_unique<CurlWebSocket>(url, bearer);
}

} // namespace fh::net
