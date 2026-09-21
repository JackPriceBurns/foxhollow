#include "borealis/http.hpp"

#include <gtest/gtest.h>

#include <cstring>

namespace http = borealis::http;

namespace {

// Invalid requests keep these tests offline for every backend.

TEST(Http, BackendIdentity) {
    EXPECT_NE(http::backend_name(), nullptr);
    EXPECT_GT(std::strlen(http::backend_name()), 0);
    EXPECT_EQ(http::available(), (http::backend() != http::Backend::None));
}

TEST(Http, EmptyUrlRejected) {
    const http::Result result = http::get({});
    if (!http::available()) {
        EXPECT_EQ(result.error, http::Error::NoBackend);
        return;
    }
    EXPECT_EQ(result.error, http::Error::InvalidUrl);
    EXPECT_FALSE(result.message.empty());
    EXPECT_EQ(result.response.statusCode, 0);
}

TEST(Http, PlaintextSchemeRejected) {
    // HTTPS-only behavior is shared by every backend.
    for (const char* url : {"http://example.com/", "ftp://example.com/", "example.com"}) {
        const http::Result result = http::get({.url = url});
        if (!http::available()) {
            EXPECT_EQ(result.error, http::Error::NoBackend);
            continue;
        }
        EXPECT_TRUE(result.error == http::Error::UnsupportedScheme ||
                    result.error == http::Error::InvalidUrl);
        EXPECT_TRUE(result.response.body.empty());
    }
}

}  // namespace
