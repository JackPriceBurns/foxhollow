#pragma once

#include <xxhash.h>

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>

namespace borealis::disc {

enum class Status {
    Success,
    IOError,
    InvalidImage,
    UnknownGame,
    UnsupportedVersion,
    Canceled,
    HashMismatch,
    Failed,
};

enum class Platform {
    Unknown,
    GameCube,
    Wii,
};

/** Parses a canonical high-bits-first 32-character XXH3-128 value. */
constexpr XXH128_hash_t parse_xxh3_128(std::string_view value) {
    const auto parse_nibble = [](char character) constexpr -> std::uint8_t {
        if (character >= '0' && character <= '9') {
            return static_cast<std::uint8_t>(character - '0');
        }
        if (character >= 'a' && character <= 'f') {
            return static_cast<std::uint8_t>(character - 'a' + 10);
        }
        if (character >= 'A' && character <= 'F') {
            return static_cast<std::uint8_t>(character - 'A' + 10);
        }
        throw std::invalid_argument{"invalid hexadecimal character"};
    };
    const auto parse_half = [&parse_nibble](std::string_view half) constexpr {
        std::uint64_t result = 0;
        for (const char character : half) {
            result = (result << 4) | parse_nibble(character);
        }
        return result;
    };

    if (value.size() != 32) {
        throw std::invalid_argument{"expected 32 hexadecimal characters"};
    }
    return {
        .low64 = parse_half(value.substr(16, 16)),
        .high64 = parse_half(value.substr(0, 16)),
    };
}

struct AcceptedDisc {
    /** Six-character Nintendo game ID. */
    std::string_view gameId;
    std::uint8_t discNumber = 0;
    std::uint8_t revision = 0;
    XXH128_hash_t expectedHash{};
};

struct Catalog {
    std::span<const AcceptedDisc> acceptedDiscs;
    /** Game IDs that report UnsupportedVersion instead of UnknownGame. */
    std::span<const std::string_view> recognizedGameIds;
};

struct Metadata {
    std::string gameId;
    std::string title;
    Platform platform = Platform::Unknown;
    std::uint8_t discNumber = 0;
    std::uint8_t revision = 0;
    /** Reconstructed disc size used for verification. */
    std::uint64_t logicalSize = 0;
};

struct Progress {
    std::atomic<std::uint64_t> bytesRead = 0;
    std::atomic<std::uint64_t> bytesTotal = 0;
    std::atomic_bool cancelRequested = false;
};

struct Result {
    Status status = Status::Failed;
    Metadata metadata;
    std::optional<std::size_t> acceptedDiscIndex;
    /** Backend diagnostic for logging; ports provide user-facing text. */
    std::string message;
};

/** Synchronously inspects a disc header from a path or Android content URI. */
Result inspect(std::string_view location, Catalog catalog);

/**
 * Synchronously hashes and verifies a reconstructed disc image. Progress may be
 * polled or canceled from another thread; opening the image may not be interruptible.
 */
Result verify(std::string_view location, Catalog catalog, Progress* progress = nullptr);

}  // namespace borealis::disc
