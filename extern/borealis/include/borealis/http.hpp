#pragma once

#include <chrono>
#include <cstddef>
#include <string>
#include <vector>

namespace borealis::http {

/** HTTP backend selected at configure time. */
enum class Backend {
    None,
    WinHttp,
    UrlSession,
    LibCurl,
    Android,
};

enum class Error {
    None,
    NoBackend,
    InvalidUrl,
    UnsupportedScheme,
    Timeout,
    TooLarge,
    Network,
};

struct Header {
    std::string name;
    std::string value;
};

struct Request {
    std::string url;
    std::vector<Header> headers;
    std::chrono::milliseconds timeout{10000};
    size_t maxBodyBytes = 1024 * 1024;
};

struct Response {
    int statusCode = 0;
    std::vector<Header> headers;
    std::string body;
};

struct Result {
    Error error = Error::None;
    std::string message;
    Response response;
};

/** Returns false when no HTTP backend is available. */
bool available() noexcept;
Backend backend() noexcept;
const char* backend_name() noexcept;

/** Performs a blocking HTTPS GET. Errors are returned in Result::error. */
Result get(const Request& request);

}  // namespace borealis::http
