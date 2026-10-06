#include "websocket.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <winhttp.h>

#include <array>
#include <atomic>
#include <thread>
#include <vector>

namespace fh::net {
namespace {

constexpr int kResolveTimeoutMs = 10000;
constexpr int kConnectTimeoutMs = 10000;
constexpr int kSendTimeoutMs = 10000;
constexpr int kReceiveTimeoutMs = 60000;

std::wstring widen(const std::string& text) {
  if (text.empty()) {
    return {};
  }
  const int length = MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
  std::wstring wide(static_cast<size_t>(length), L'\0');
  MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), wide.data(), length);
  return wide;
}

class WinHttpWebSocket final : public WebSocket {
public:
  WinHttpWebSocket(const std::string& url, const std::string& bearer) : mQueue(std::make_shared<EventQueue>()) {
    mThread = std::thread([this, url, bearer] { run(url, bearer); });
  }

  ~WinHttpWebSocket() override {
    {
      std::lock_guard lock{mHandleMutex};
      mStopping = true;
      if (mSocket != nullptr) {
        WinHttpWebSocketShutdown(mSocket, WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS, nullptr, 0);
      }
      close_handles_locked();
    }
    if (mThread.joinable()) {
      mThread.join();
    }
  }

  void send_binary(const void* data, size_t size) override {
    send(WINHTTP_WEB_SOCKET_BINARY_MESSAGE_BUFFER_TYPE, data, size);
  }

  void send_text(std::string_view text) override {
    send(WINHTTP_WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE, text.data(), text.size());
  }

  bool poll(SocketEvent& out) override { return mQueue->pop(out); }

private:
  void send(WINHTTP_WEB_SOCKET_BUFFER_TYPE type, const void* data, size_t size) {
    std::lock_guard lock{mHandleMutex};
    if (mSocket == nullptr || mStopping) {
      return;
    }
    WinHttpWebSocketSend(mSocket, type, const_cast<void*>(data), static_cast<DWORD>(size));
  }

  void close_handles_locked() {
    for (HINTERNET* handle : {&mSocket, &mRequest, &mConnection, &mSession}) {
      if (*handle != nullptr) {
        WinHttpCloseHandle(*handle);
        *handle = nullptr;
      }
    }
  }

  bool adopt(HINTERNET handle, HINTERNET& slot) {
    std::lock_guard lock{mHandleMutex};
    if (handle == nullptr) {
      return false;
    }
    if (mStopping) {
      WinHttpCloseHandle(handle);
      return false;
    }
    slot = handle;
    return true;
  }

  HINTERNET current(HINTERNET& slot) {
    std::lock_guard lock{mHandleMutex};
    return mStopping ? nullptr : slot;
  }

  void fail(int code) { mQueue->push({SocketEvent::Type::Closed, {}, code}); }

  void run(const std::string& url, const std::string& bearer) {
    std::wstring httpUrl = widen(url);
    const bool secure = httpUrl.rfind(L"wss://", 0) == 0;
    if (secure) {
      httpUrl.replace(0, 3, L"https");
    } else if (httpUrl.rfind(L"ws://", 0) == 0) {
      httpUrl.replace(0, 2, L"http");
    }
    URL_COMPONENTS parts{};
    parts.dwStructSize = sizeof(parts);
    std::array<wchar_t, 256> host{};
    std::array<wchar_t, 4096> path{};
    parts.lpszHostName = host.data();
    parts.dwHostNameLength = static_cast<DWORD>(host.size());
    parts.lpszUrlPath = path.data();
    parts.dwUrlPathLength = static_cast<DWORD>(path.size());
    parts.dwExtraInfoLength = 1;
    if (!WinHttpCrackUrl(httpUrl.c_str(), 0, 0, &parts)) {
      fail(1015);
      return;
    }
    std::wstring target(parts.lpszUrlPath, parts.dwUrlPathLength);
    if (parts.lpszExtraInfo != nullptr) {
      target.append(parts.lpszExtraInfo, parts.dwExtraInfoLength);
    }
    const std::wstring headers = bearer.empty() ? std::wstring{} : L"Authorization: Bearer " + widen(bearer);

    if (!adopt(WinHttpOpen(L"Foxhollow", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME,
                           WINHTTP_NO_PROXY_BYPASS, 0),
               mSession)) {
      fail(1006);
      return;
    }
    if (HINTERNET session = current(mSession)) {
      WinHttpSetTimeouts(session, kResolveTimeoutMs, kConnectTimeoutMs, kSendTimeoutMs, kReceiveTimeoutMs);
    }
    HINTERNET session = current(mSession);
    if (session == nullptr || !adopt(WinHttpConnect(session, host.data(), parts.nPort, 0), mConnection)) {
      fail(1006);
      return;
    }
    HINTERNET connection = current(mConnection);
    if (connection == nullptr ||
        !adopt(WinHttpOpenRequest(connection, L"GET", target.c_str(), nullptr, WINHTTP_NO_REFERER,
                                  WINHTTP_DEFAULT_ACCEPT_TYPES, secure ? WINHTTP_FLAG_SECURE : 0),
               mRequest)) {
      fail(1006);
      return;
    }
    HINTERNET request = current(mRequest);
    if (request == nullptr || !WinHttpSetOption(request, WINHTTP_OPTION_UPGRADE_TO_WEB_SOCKET, nullptr, 0) ||
        !WinHttpSendRequest(request, headers.empty() ? WINHTTP_NO_ADDITIONAL_HEADERS : headers.c_str(),
                            headers.empty() ? 0 : static_cast<DWORD>(-1L), nullptr, 0, 0, 0) ||
        !WinHttpReceiveResponse(request, nullptr)) {
      fail(1006);
      return;
    }
    DWORD status = 0;
    DWORD statusSize = sizeof(status);
    WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX,
                        &status, &statusSize, WINHTTP_NO_HEADER_INDEX);
    if (status != 101) {
      fail(static_cast<int>(status));
      return;
    }
    if (!adopt(WinHttpWebSocketCompleteUpgrade(request, 0), mSocket)) {
      fail(1006);
      return;
    }
    {
      std::lock_guard lock{mHandleMutex};
      if (mRequest != nullptr) {
        WinHttpCloseHandle(mRequest);
        mRequest = nullptr;
      }
    }
    mQueue->push({SocketEvent::Type::Open, {}, 0});

    std::vector<char> buffer(16384);
    std::string message;
    while (true) {
      HINTERNET socket = current(mSocket);
      if (socket == nullptr) {
        fail(1000);
        return;
      }
      DWORD read = 0;
      WINHTTP_WEB_SOCKET_BUFFER_TYPE type{};
      const DWORD result =
          WinHttpWebSocketReceive(socket, buffer.data(), static_cast<DWORD>(buffer.size()), &read, &type);
      if (result != ERROR_SUCCESS) {
        fail(1006);
        return;
      }
      if (type == WINHTTP_WEB_SOCKET_CLOSE_BUFFER_TYPE) {
        USHORT closeStatus = 1005;
        std::array<BYTE, 123> reason{};
        DWORD reasonLength = 0;
        WinHttpWebSocketQueryCloseStatus(socket, &closeStatus, reason.data(), static_cast<DWORD>(reason.size()),
                                         &reasonLength);
        fail(closeStatus);
        return;
      }
      message.append(buffer.data(), read);
      if (type == WINHTTP_WEB_SOCKET_BINARY_MESSAGE_BUFFER_TYPE) {
        mQueue->push({SocketEvent::Type::Binary, std::move(message), 0});
        message.clear();
      } else if (type == WINHTTP_WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE) {
        mQueue->push({SocketEvent::Type::Text, std::move(message), 0});
        message.clear();
      }
    }
  }

  std::shared_ptr<EventQueue> mQueue;
  std::thread mThread;
  std::mutex mHandleMutex;
  bool mStopping = false;
  HINTERNET mSession = nullptr;
  HINTERNET mConnection = nullptr;
  HINTERNET mRequest = nullptr;
  HINTERNET mSocket = nullptr;
};

} // namespace

std::unique_ptr<WebSocket> connect_websocket(const std::string& url, const std::string& bearer) {
  return std::make_unique<WinHttpWebSocket>(url, bearer);
}

} // namespace fh::net
