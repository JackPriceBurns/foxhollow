#pragma once

#include <filesystem>
#include <string>
#include <string_view>

namespace borealis::io {

/** Converts a filesystem path to UTF-8 without using the Windows ANSI code page. */
inline std::string fs_path_to_string(const std::filesystem::path& path) {
#if defined(_WIN32)
    const std::u8string u8 = path.u8string();
    return std::string(u8.begin(), u8.end());
#else
    return path.string();
#endif
}

/** Converts a filesystem path to UTF-8 with '/' separators. */
inline std::string fs_path_to_generic_string(const std::filesystem::path& path) {
#if defined(_WIN32)
    const std::u8string u8 = path.generic_u8string();
    return std::string(u8.begin(), u8.end());
#else
    return path.generic_string();
#endif
}

/** Constructs a filesystem path from UTF-8. */
inline std::filesystem::path fs_path_from_utf8(std::string_view utf8) {
#if defined(_WIN32)
    return std::filesystem::path(std::u8string(utf8.begin(), utf8.end()));
#else
    return std::filesystem::path(std::string(utf8));
#endif
}

}  // namespace borealis::io
