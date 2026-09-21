#include "borealis/disc.hpp"

#include <gtest/gtest.h>
#include "disc_internal.hpp"

#include <SDL3/SDL_iostream.h>
#include <nod.h>
#include <xxhash.h>

#include <algorithm>
#include <array>
#include <cstring>
#include <limits>
#include <span>
#include <string_view>
#include <vector>

namespace disc = borealis::disc;

namespace {

struct TestStream {
    std::vector<std::uint8_t> data{0x11, 0x22, 0x33};
    std::size_t position = 0;
    int closeCount = 0;
};

Sint64 SDLCALL test_stream_size(void* userdata) {
    return static_cast<Sint64>(static_cast<TestStream*>(userdata)->data.size());
}

Sint64 SDLCALL test_stream_seek(void* userdata, Sint64 offset, SDL_IOWhence whence) {
    auto& stream = *static_cast<TestStream*>(userdata);
    Sint64 base = 0;
    switch (whence) {
    case SDL_IO_SEEK_SET:
        break;
    case SDL_IO_SEEK_CUR:
        base = static_cast<Sint64>(stream.position);
        break;
    case SDL_IO_SEEK_END:
        base = static_cast<Sint64>(stream.data.size());
        break;
    default:
        return -1;
    }
    if (offset < -base || offset > std::numeric_limits<Sint64>::max() - base) {
        return -1;
    }
    const Sint64 position = base + offset;
    if (position < 0 || static_cast<std::uint64_t>(position) > stream.data.size()) {
        return -1;
    }
    stream.position = static_cast<std::size_t>(position);
    return position;
}

std::size_t SDLCALL test_stream_read(
    void* userdata, void* output, std::size_t size, SDL_IOStatus* status) {
    auto& stream = *static_cast<TestStream*>(userdata);
    const std::size_t available = stream.data.size() - stream.position;
    const std::size_t read = std::min(size, available);
    std::memcpy(output, stream.data.data() + stream.position, read);
    stream.position += read;
    if (read < size) {
        *status = SDL_IO_STATUS_EOF;
    }
    return read;
}

bool SDLCALL test_stream_close(void* userdata) {
    ++static_cast<TestStream*>(userdata)->closeCount;
    return true;
}

SDL_IOStream* make_stream(TestStream& stream) {
    SDL_IOStreamInterface interface{};
    SDL_INIT_INTERFACE(&interface);
    interface.size = test_stream_size;
    interface.seek = test_stream_seek;
    interface.read = test_stream_read;
    interface.close = test_stream_close;
    return SDL_OpenIO(&interface, &stream);
}

struct FakeBackend {
    NodResult openResult = NOD_RESULT_OK;
    NodResult headerResult = NOD_RESULT_OK;
    NodDiscHeader header{};
    std::vector<std::uint8_t> data;
    std::size_t offset = 0;
    std::size_t maxRead = std::numeric_limits<std::size_t>::max();
    std::uint64_t reportedSize = std::numeric_limits<std::uint64_t>::max();
    int failReadCall = -1;
    int readCalls = 0;
    int openCalls = 0;
    int freeCalls = 0;
    std::int64_t observedStreamLength = -1;
    std::int64_t observedStreamRead = -1;
    std::uint8_t observedFirstByte = 0;
    const char* message = "fake nod error";
    disc::Progress* cancelAfterFirstRead = nullptr;
    NodDiscStream stream{};
};

FakeBackend* ActiveBackend = nullptr;

NodResult fake_open_stream(const NodDiscStream* stream, const NodDiscOptions*, NodHandle** output) {
    if (ActiveBackend == nullptr) {
        ADD_FAILURE() << "No active fake backend";
        *output = nullptr;
        return NOD_RESULT_ERR_OTHER;
    }
    auto& backend = *ActiveBackend;
    ++backend.openCalls;
    backend.stream = *stream;
    backend.observedStreamLength = stream->stream_len(stream->user_data);
    backend.observedStreamRead =
        stream->read_at(stream->user_data, 0, &backend.observedFirstByte, 1);
    if (backend.openResult != NOD_RESULT_OK) {
        stream->close(stream->user_data);
        *output = nullptr;
        return backend.openResult;
    }
    *output = reinterpret_cast<NodHandle*>(&backend);
    return NOD_RESULT_OK;
}

void fake_free(NodHandle* handle) {
    auto& backend = *reinterpret_cast<FakeBackend*>(handle);
    ++backend.freeCalls;
    backend.stream.close(backend.stream.user_data);
}

std::int64_t fake_read(NodHandle* handle, std::uint8_t* output, std::size_t size) {
    auto& backend = *reinterpret_cast<FakeBackend*>(handle);
    const int call = backend.readCalls++;
    if (call == backend.failReadCall) {
        return -1;
    }
    const std::size_t available = backend.data.size() - backend.offset;
    const std::size_t read = std::min({size, available, backend.maxRead});
    std::memcpy(output, backend.data.data() + backend.offset, read);
    backend.offset += read;
    if (call == 0 && backend.cancelAfterFirstRead != nullptr) {
        backend.cancelAfterFirstRead->cancelRequested.store(true, std::memory_order_relaxed);
    }
    return static_cast<std::int64_t>(read);
}

NodResult fake_header(const NodHandle* handle, NodDiscHeader* output) {
    const auto& backend = *reinterpret_cast<const FakeBackend*>(handle);
    if (backend.headerResult == NOD_RESULT_OK) {
        *output = backend.header;
    }
    return backend.headerResult;
}

std::uint64_t fake_disc_size(const NodHandle* handle) {
    const auto& backend = *reinterpret_cast<const FakeBackend*>(handle);
    return backend.reportedSize == std::numeric_limits<std::uint64_t>::max() ? backend.data.size() :
                                                                               backend.reportedSize;
}

const char* fake_error_message() {
    return ActiveBackend == nullptr ? nullptr : ActiveBackend->message;
}

const disc::detail::NodApi FakeApi{
    .openStream = fake_open_stream,
    .freeHandle = fake_free,
    .read = fake_read,
    .header = fake_header,
    .discSize = fake_disc_size,
    .errorMessage = fake_error_message,
};

struct ActiveBackendScope {
    explicit ActiveBackendScope(FakeBackend& backend) { ActiveBackend = &backend; }
    ~ActiveBackendScope() { ActiveBackend = nullptr; }
};

void set_header(FakeBackend& backend, std::string_view gameId, std::uint8_t revision = 0,
    std::uint8_t discNumber = 0) {
    if (gameId.size() != 6) {
        ADD_FAILURE() << "Game ID must contain six characters";
        return;
    }
    std::copy(gameId.begin(), gameId.end(), std::begin(backend.header.game_id));
    constexpr std::string_view Title = "Test Disc";
    std::copy(Title.begin(), Title.end(), std::begin(backend.header.game_title));
    backend.header.gcn_magic[0] = 0xC2;
    backend.header.gcn_magic[1] = 0x33;
    backend.header.gcn_magic[2] = 0x9F;
    backend.header.gcn_magic[3] = 0x3D;
    backend.header.disc_num = discNumber;
    backend.header.disc_version = revision;
}

disc::Result inspect(FakeBackend& backend, TestStream& stream, disc::Catalog catalog) {
    ActiveBackendScope active{backend};
    SDL_IOStream* io = make_stream(stream);
    if (io == nullptr) {
        ADD_FAILURE() << "Failed to create SDL IO stream";
        return {};
    }
    return disc::detail::inspect_stream(io, catalog, FakeApi);
}

disc::Result verify(FakeBackend& backend, TestStream& stream, disc::Catalog catalog,
    disc::Progress* progress = nullptr) {
    ActiveBackendScope active{backend};
    SDL_IOStream* io = make_stream(stream);
    if (io == nullptr) {
        ADD_FAILURE() << "Failed to create SDL IO stream";
        return {};
    }
    return disc::detail::verify_stream(io, catalog, progress, FakeApi);
}

TEST(Disc, HashParser) {
    constexpr auto hash = disc::parse_xxh3_128("0123456789abcdeffedcba9876543210");
    static_assert(hash.high64 == 0x0123456789abcdefULL);
    static_assert(hash.low64 == 0xfedcba9876543210ULL);

    EXPECT_THROW(disc::parse_xxh3_128("not-a-hash"), std::invalid_argument);
}

TEST(Disc, PublicPreflight) {
    EXPECT_EQ(disc::inspect("", {}).status, disc::Status::IOError);

    disc::Progress progress;
    progress.bytesRead.store(99, std::memory_order_relaxed);
    progress.bytesTotal.store(100, std::memory_order_relaxed);
    EXPECT_EQ(disc::verify("", {}, &progress).status, disc::Status::IOError);
    EXPECT_EQ(progress.bytesRead.load(std::memory_order_relaxed), 0);
    EXPECT_EQ(progress.bytesTotal.load(std::memory_order_relaxed), 0);

    progress.cancelRequested.store(true, std::memory_order_relaxed);
    EXPECT_EQ(disc::verify("not-opened.iso", {}, &progress).status, disc::Status::Canceled);
}

TEST(Disc, MetadataAndRecordMatching) {
    constexpr std::array accepted{
        disc::AcceptedDisc{
            .gameId = "GAME01",
            .discNumber = 0,
            .revision = 1,
        },
    };
    constexpr std::array<std::string_view, 1> recognized{"OTHER1"};
    const disc::Catalog catalog{accepted, recognized};

    {
        FakeBackend backend;
        set_header(backend, "GAME01", 1);
        TestStream stream;
        const auto result = inspect(backend, stream, catalog);
        EXPECT_EQ(result.status, disc::Status::Success);
        EXPECT_EQ(result.acceptedDiscIndex, 0);
        EXPECT_EQ(result.metadata.gameId, "GAME01");
        EXPECT_EQ(result.metadata.title, "Test Disc");
        EXPECT_EQ(result.metadata.platform, disc::Platform::GameCube);
        EXPECT_EQ(result.metadata.revision, 1);
        EXPECT_EQ(backend.observedStreamLength, 3);
        EXPECT_EQ(backend.observedStreamRead, 1);
        EXPECT_EQ(backend.observedFirstByte, 0x11);
        EXPECT_EQ(backend.freeCalls, 1);
        EXPECT_EQ(stream.closeCount, 1);
    }
    {
        FakeBackend backend;
        set_header(backend, "GAME01", 2);
        TestStream stream;
        const auto result = inspect(backend, stream, catalog);
        EXPECT_EQ(result.status, disc::Status::UnsupportedVersion);
        EXPECT_FALSE(result.acceptedDiscIndex.has_value());
        EXPECT_EQ(stream.closeCount, 1);
    }
    {
        FakeBackend backend;
        set_header(backend, "OTHER1");
        TestStream stream;
        EXPECT_EQ(inspect(backend, stream, catalog).status, disc::Status::UnsupportedVersion);
        EXPECT_EQ(stream.closeCount, 1);
    }
    {
        FakeBackend backend;
        set_header(backend, "UNKN01");
        TestStream stream;
        EXPECT_EQ(inspect(backend, stream, catalog).status, disc::Status::UnknownGame);
        EXPECT_EQ(stream.closeCount, 1);
    }
    {
        FakeBackend backend;
        set_header(backend, "GAME01", 1);
        std::fill(std::begin(backend.header.gcn_magic), std::end(backend.header.gcn_magic), 0);
        TestStream stream;
        EXPECT_EQ(inspect(backend, stream, catalog).status, disc::Status::InvalidImage);
        EXPECT_EQ(stream.closeCount, 1);
    }
}

TEST(Disc, ErrorMappingAndOwnership) {
    constexpr disc::Catalog EmptyCatalog{};
    EXPECT_EQ(disc::detail::status_from_nod_result(NOD_RESULT_ERR_IO), disc::Status::IOError);
    EXPECT_TRUE(
        disc::detail::status_from_nod_result(NOD_RESULT_ERR_FORMAT) == disc::Status::InvalidImage);
    EXPECT_EQ(disc::detail::status_from_nod_result(NOD_RESULT_ERR_OTHER), disc::Status::Failed);

    {
        FakeBackend backend;
        backend.openResult = NOD_RESULT_ERR_IO;
        TestStream stream;
        const auto result = inspect(backend, stream, EmptyCatalog);
        EXPECT_EQ(result.status, disc::Status::IOError);
        EXPECT_EQ(result.message, "fake nod error");
        EXPECT_EQ(backend.freeCalls, 0);
        EXPECT_EQ(stream.closeCount, 1);
    }
    {
        FakeBackend backend;
        backend.openResult = NOD_RESULT_ERR_FORMAT;
        TestStream stream;
        EXPECT_EQ(inspect(backend, stream, EmptyCatalog).status, disc::Status::InvalidImage);
        EXPECT_EQ(stream.closeCount, 1);
    }
    {
        FakeBackend backend;
        set_header(backend, "GAME01");
        backend.headerResult = NOD_RESULT_ERR_IO;
        TestStream stream;
        EXPECT_EQ(inspect(backend, stream, EmptyCatalog).status, disc::Status::IOError);
        EXPECT_EQ(backend.freeCalls, 1);
        EXPECT_EQ(stream.closeCount, 1);
    }
}

TEST(Disc, VerificationResultsAndProgress) {
    const std::vector<std::uint8_t> data{'d', 'i', 's', 'c', '-', 'd', 'a', 't', 'a'};
    const XXH128_hash_t expected = XXH3_128bits(data.data(), data.size());
    std::array accepted{
        disc::AcceptedDisc{
            .gameId = "GAME01",
            .expectedHash = expected,
        },
    };
    const disc::Catalog catalog{accepted, {}};

    {
        FakeBackend backend;
        set_header(backend, "GAME01");
        backend.data = data;
        backend.maxRead = 2;
        TestStream stream;
        disc::Progress progress;
        const auto result = verify(backend, stream, catalog, &progress);
        EXPECT_EQ(result.status, disc::Status::Success);
        EXPECT_EQ(progress.bytesRead.load(std::memory_order_relaxed), data.size());
        EXPECT_EQ(progress.bytesTotal.load(std::memory_order_relaxed), data.size());
        EXPECT_GT(backend.readCalls, 1);
        EXPECT_EQ(stream.closeCount, 1);
    }
    {
        FakeBackend backend;
        set_header(backend, "GAME01");
        backend.data = data;
        TestStream stream;
        accepted[0].expectedHash.low64 ^= 1;
        const auto result = verify(backend, stream, catalog);
        EXPECT_EQ(result.status, disc::Status::HashMismatch);
        EXPECT_EQ(result.acceptedDiscIndex, 0);
        EXPECT_EQ(stream.closeCount, 1);
        accepted[0].expectedHash = expected;
    }
    {
        FakeBackend backend;
        set_header(backend, "GAME01");
        backend.data = data;
        backend.reportedSize = data.size() + 1;
        TestStream stream;
        EXPECT_EQ(verify(backend, stream, catalog).status, disc::Status::IOError);
        EXPECT_EQ(stream.closeCount, 1);
    }
    {
        FakeBackend backend;
        set_header(backend, "GAME01");
        backend.data = data;
        backend.maxRead = 2;
        backend.failReadCall = 1;
        TestStream stream;
        disc::Progress progress;
        const auto result = verify(backend, stream, catalog, &progress);
        EXPECT_EQ(result.status, disc::Status::IOError);
        EXPECT_EQ(progress.bytesRead.load(std::memory_order_relaxed), 2);
        EXPECT_EQ(stream.closeCount, 1);
    }
}

TEST(Disc, Cancellation) {
    const std::vector<std::uint8_t> data{'c', 'a', 'n', 'c', 'e', 'l'};
    const std::array accepted{
        disc::AcceptedDisc{
            .gameId = "GAME01",
            .expectedHash = XXH3_128bits(data.data(), data.size()),
        },
    };
    const disc::Catalog catalog{accepted, {}};

    {
        FakeBackend backend;
        set_header(backend, "GAME01");
        backend.data = data;
        TestStream stream;
        disc::Progress progress;
        progress.bytesRead.store(99, std::memory_order_relaxed);
        progress.bytesTotal.store(100, std::memory_order_relaxed);
        progress.cancelRequested.store(true, std::memory_order_relaxed);
        const auto result = verify(backend, stream, catalog, &progress);
        EXPECT_EQ(result.status, disc::Status::Canceled);
        EXPECT_EQ(progress.bytesRead.load(std::memory_order_relaxed), 0);
        EXPECT_EQ(progress.bytesTotal.load(std::memory_order_relaxed), 0);
        EXPECT_EQ(backend.openCalls, 0);
        EXPECT_EQ(stream.closeCount, 1);
    }
    {
        FakeBackend backend;
        set_header(backend, "GAME01");
        backend.data = data;
        backend.maxRead = 2;
        TestStream stream;
        disc::Progress progress;
        backend.cancelAfterFirstRead = &progress;
        const auto result = verify(backend, stream, catalog, &progress);
        EXPECT_EQ(result.status, disc::Status::Canceled);
        EXPECT_EQ(progress.bytesRead.load(std::memory_order_relaxed), 2);
        EXPECT_EQ(progress.bytesTotal.load(std::memory_order_relaxed), data.size());
        EXPECT_EQ(stream.closeCount, 1);
    }
}

}  // namespace
