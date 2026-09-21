#include "borealis/disc.hpp"

#include "disc_internal.hpp"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_iostream.h>
#include <nod.h>
#include <xxhash.h>

#include <algorithm>
#include <array>
#include <iterator>
#include <limits>
#include <memory>
#include <new>
#include <string>
#include <utility>

namespace borealis::disc::detail {
namespace {

constexpr std::size_t HashBufferSize = 1024 * 1024;

std::string copy_message(const char* message, std::string fallback) {
    if (message == nullptr || message[0] == '\0') {
        return fallback;
    }
    return message;
}

std::string nod_message(const NodApi& api, std::string fallback) {
    return copy_message(
        api.errorMessage == nullptr ? nullptr : api.errorMessage(), std::move(fallback));
}

std::int64_t stream_read_at(
    void* userdata, std::uint64_t offset, void* output, std::size_t length) {
    if (length == 0) {
        return 0;
    }
    if (userdata == nullptr || output == nullptr ||
        offset > static_cast<std::uint64_t>(std::numeric_limits<Sint64>::max()))
    {
        return -1;
    }

    auto* stream = static_cast<SDL_IOStream*>(userdata);
    if (SDL_SeekIO(stream, static_cast<Sint64>(offset), SDL_IO_SEEK_SET) < 0) {
        return -1;
    }
    const std::size_t read = SDL_ReadIO(stream, output, length);
    if (read != 0) {
        return static_cast<std::int64_t>(read);
    }
    return SDL_GetIOStatus(stream) == SDL_IO_STATUS_EOF ? 0 : -1;
}

std::int64_t stream_length(void* userdata) {
    if (userdata == nullptr) {
        return -1;
    }
    return SDL_GetIOSize(static_cast<SDL_IOStream*>(userdata));
}

void stream_close(void* userdata) {
    if (userdata != nullptr) {
        SDL_CloseIO(static_cast<SDL_IOStream*>(userdata));
    }
}

class NodHandleOwner {
public:
    explicit NodHandleOwner(const NodApi& api) : mApi{api} {}

    NodHandleOwner(const NodHandleOwner&) = delete;
    NodHandleOwner& operator=(const NodHandleOwner&) = delete;

    NodHandleOwner(NodHandleOwner&& other) noexcept
        : mApi{other.mApi}, mHandle{std::exchange(other.mHandle, nullptr)} {}

    ~NodHandleOwner() {
        if (mHandle != nullptr) {
            mApi.freeHandle(mHandle);
        }
    }

    NodHandle** out() noexcept { return &mHandle; }
    NodHandle* get() const noexcept { return mHandle; }

private:
    const NodApi& mApi;
    NodHandle* mHandle = nullptr;
};

struct OpenedDisc {
    explicit OpenedDisc(const NodApi& api) : disc{api} {}

    NodHandleOwner disc;
    Result result;
};

Platform platform_from_header(const NodDiscHeader& header) noexcept {
    constexpr std::array<std::uint8_t, 4> GameCubeMagic{0xC2, 0x33, 0x9F, 0x3D};
    constexpr std::array<std::uint8_t, 4> WiiMagic{0x5D, 0x1C, 0x9E, 0xA3};
    if (std::equal(std::begin(header.gcn_magic), std::end(header.gcn_magic), GameCubeMagic.begin()))
    {
        return Platform::GameCube;
    }
    if (std::equal(std::begin(header.wii_magic), std::end(header.wii_magic), WiiMagic.begin())) {
        return Platform::Wii;
    }
    return Platform::Unknown;
}

std::string header_title(const NodDiscHeader& header) {
    const auto end = std::find(std::begin(header.game_title), std::end(header.game_title), '\0');
    return std::string{std::begin(header.game_title), end};
}

std::optional<std::size_t> find_accepted_disc(
    const Metadata& metadata, std::span<const AcceptedDisc> records) noexcept {
    for (std::size_t index = 0; index < records.size(); ++index) {
        const auto& record = records[index];
        if (record.gameId == metadata.gameId && record.discNumber == metadata.discNumber &&
            record.revision == metadata.revision)
        {
            return index;
        }
    }
    return std::nullopt;
}

bool is_recognized_game(const Metadata& metadata, Catalog catalog) noexcept {
    const auto accepted = std::find_if(catalog.acceptedDiscs.begin(), catalog.acceptedDiscs.end(),
        [&metadata](const AcceptedDisc& record) { return record.gameId == metadata.gameId; });
    if (accepted != catalog.acceptedDiscs.end()) {
        return true;
    }
    return std::find(catalog.recognizedGameIds.begin(), catalog.recognizedGameIds.end(),
               metadata.gameId) != catalog.recognizedGameIds.end();
}

OpenedDisc open_and_inspect(SDL_IOStream* stream, Catalog catalog, const NodApi& api) {
    OpenedDisc opened{api};
    if (stream == nullptr) {
        opened.result = {
            .status = Status::IOError,
            .message = "Disc stream is null",
        };
        return opened;
    }

    const NodDiscStream callbacks{
        .user_data = stream,
        .read_at = stream_read_at,
        .stream_len = stream_length,
        .close = stream_close,
    };
    const NodResult openResult = api.openStream(&callbacks, nullptr, opened.disc.out());
    if (openResult != NOD_RESULT_OK || opened.disc.get() == nullptr) {
        opened.result = {
            .status =
                openResult == NOD_RESULT_OK ? Status::Failed : status_from_nod_result(openResult),
            .message = nod_message(api, "Failed to open disc image"),
        };
        return opened;
    }

    opened.result.metadata.logicalSize = api.discSize(opened.disc.get());
    NodDiscHeader header{};
    const NodResult headerResult = api.header(opened.disc.get(), &header);
    if (headerResult != NOD_RESULT_OK) {
        opened.result.status = status_from_nod_result(headerResult);
        opened.result.message = nod_message(api, "Failed to read disc header");
        return opened;
    }

    opened.result.metadata.gameId.assign(std::begin(header.game_id), std::end(header.game_id));
    opened.result.metadata.title = header_title(header);
    opened.result.metadata.platform = platform_from_header(header);
    opened.result.metadata.discNumber = header.disc_num;
    opened.result.metadata.revision = header.disc_version;

    if (opened.result.metadata.platform == Platform::Unknown) {
        opened.result.status = Status::InvalidImage;
        opened.result.message = "Disc header contains no recognized platform magic";
        return opened;
    }

    opened.result.acceptedDiscIndex =
        find_accepted_disc(opened.result.metadata, catalog.acceptedDiscs);
    if (opened.result.acceptedDiscIndex.has_value()) {
        opened.result.status = Status::Success;
    } else if (is_recognized_game(opened.result.metadata, catalog)) {
        opened.result.status = Status::UnsupportedVersion;
    } else {
        opened.result.status = Status::UnknownGame;
    }
    return opened;
}

Result canceled_result() {
    return {
        .status = Status::Canceled,
    };
}

}  // namespace

const NodApi& default_nod_api() noexcept {
    static const NodApi api{
        .openStream = nod_disc_open_stream,
        .freeHandle = nod_free,
        .read = nod_read,
        .header = nod_disc_header,
        .discSize = nod_disc_size,
        .errorMessage = nod_error_message,
    };
    return api;
}

Status status_from_nod_result(NodResult result) noexcept {
    switch (result) {
    case NOD_RESULT_OK:
        return Status::Success;
    case NOD_RESULT_ERR_IO:
        return Status::IOError;
    case NOD_RESULT_ERR_FORMAT:
        return Status::InvalidImage;
    default:
        return Status::Failed;
    }
}

Result inspect_stream(SDL_IOStream* stream, Catalog catalog, const NodApi& api) {
    auto opened = open_and_inspect(stream, catalog, api);
    return std::move(opened.result);
}

Result verify_stream(SDL_IOStream* stream, Catalog catalog, Progress* progress, const NodApi& api) {
    if (progress != nullptr) {
        progress->bytesRead.store(0, std::memory_order_relaxed);
        progress->bytesTotal.store(0, std::memory_order_relaxed);
        if (progress->cancelRequested.load(std::memory_order_relaxed)) {
            stream_close(stream);
            return canceled_result();
        }
    }

    auto opened = open_and_inspect(stream, catalog, api);
    if (opened.result.status != Status::Success) {
        return std::move(opened.result);
    }

    if (progress != nullptr) {
        progress->bytesTotal.store(opened.result.metadata.logicalSize, std::memory_order_relaxed);
    }

    std::unique_ptr<XXH3_state_t, decltype(&XXH3_freeState)> hashState{
        XXH3_createState(), XXH3_freeState};
    if (hashState == nullptr || XXH3_128bits_reset(hashState.get()) == XXH_ERROR) {
        opened.result.status = Status::Failed;
        opened.result.message = "Failed to initialize XXH3-128 state";
        return std::move(opened.result);
    }

    auto buffer = std::unique_ptr<std::uint8_t[]>{new (std::nothrow) std::uint8_t[HashBufferSize]};
    if (buffer == nullptr) {
        opened.result.status = Status::Failed;
        opened.result.message = "Failed to allocate disc verification buffer";
        return std::move(opened.result);
    }

    std::uint64_t bytesRead = 0;
    while (true) {
        if (progress != nullptr && progress->cancelRequested.load(std::memory_order_relaxed)) {
            opened.result.status = Status::Canceled;
            return std::move(opened.result);
        }

        const std::int64_t read = api.read(opened.disc.get(), buffer.get(), HashBufferSize);
        if (read < 0) {
            opened.result.status = Status::IOError;
            opened.result.message = nod_message(api, "Failed while reading disc image");
            return std::move(opened.result);
        }
        if (read == 0) {
            break;
        }
        if (static_cast<std::uint64_t>(read) > HashBufferSize) {
            opened.result.status = Status::Failed;
            opened.result.message = "Disc backend returned an oversized read";
            return std::move(opened.result);
        }
        if (XXH3_128bits_update(hashState.get(), buffer.get(), static_cast<std::size_t>(read)) ==
            XXH_ERROR)
        {
            opened.result.status = Status::Failed;
            opened.result.message = "Failed to update XXH3-128 state";
            return std::move(opened.result);
        }

        bytesRead += static_cast<std::uint64_t>(read);
        if (progress != nullptr) {
            progress->bytesRead.store(bytesRead, std::memory_order_relaxed);
        }
    }

    if (bytesRead != opened.result.metadata.logicalSize) {
        opened.result.status = Status::IOError;
        opened.result.message = "Disc image ended before its reported logical size";
        return std::move(opened.result);
    }

    const auto& record = catalog.acceptedDiscs[*opened.result.acceptedDiscIndex];
    const XXH128_hash_t actualHash = XXH3_128bits_digest(hashState.get());
    opened.result.status =
        XXH128_isEqual(actualHash, record.expectedHash) ? Status::Success : Status::HashMismatch;
    return std::move(opened.result);
}

}  // namespace borealis::disc::detail

namespace borealis::disc {
namespace {

Result open_error(std::string message) {
    return {
        .status = Status::IOError,
        .message = std::move(message),
    };
}

SDL_IOStream* open_location(std::string_view location, Result& error) {
    if (location.empty()) {
        error = open_error("Disc location is empty");
        return nullptr;
    }

    const std::string path{location};
    SDL_IOStream* stream = SDL_IOFromFile(path.c_str(), "rb");
    if (stream == nullptr) {
        error = open_error(detail::copy_message(SDL_GetError(), "Failed to open disc image"));
    }
    return stream;
}

}  // namespace

Result inspect(std::string_view location, Catalog catalog) {
    Result error;
    SDL_IOStream* stream = open_location(location, error);
    if (stream == nullptr) {
        return error;
    }
    return detail::inspect_stream(stream, catalog, detail::default_nod_api());
}

Result verify(std::string_view location, Catalog catalog, Progress* progress) {
    if (progress != nullptr) {
        progress->bytesRead.store(0, std::memory_order_relaxed);
        progress->bytesTotal.store(0, std::memory_order_relaxed);
        if (progress->cancelRequested.load(std::memory_order_relaxed)) {
            return detail::canceled_result();
        }
    }

    Result error;
    SDL_IOStream* stream = open_location(location, error);
    if (stream == nullptr) {
        return error;
    }
    return detail::verify_stream(stream, catalog, progress, detail::default_nod_api());
}

}  // namespace borealis::disc
