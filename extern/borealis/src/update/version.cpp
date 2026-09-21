#include "borealis/update.hpp"

#include <algorithm>
#include <charconv>

namespace borealis::update {
namespace {

std::optional<int> parse_component(std::string_view& value) {
    if (value.empty() || value.front() < '0' || value.front() > '9') {
        return std::nullopt;
    }

    int parsed = 0;
    const char* begin = value.data();
    const char* end = value.data() + value.size();
    const auto [ptr, ec] = std::from_chars(begin, end, parsed);
    if (ec != std::errc()) {
        return std::nullopt;
    }

    value.remove_prefix(static_cast<size_t>(ptr - begin));
    return parsed;
}

bool consume(std::string_view& value, char expected) {
    if (value.empty() || value.front() != expected) {
        return false;
    }
    value.remove_prefix(1);
    return true;
}

bool is_digit(char value) {
    return value >= '0' && value <= '9';
}

bool is_identifier_char(char value) {
    return is_digit(value) || (value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z') ||
           value == '-';
}

bool is_numeric_identifier(std::string_view value) {
    if (value.empty()) {
        return false;
    }
    for (const char c : value) {
        if (!is_digit(c)) {
            return false;
        }
    }
    return true;
}

bool is_identifier_list(std::string_view value) {
    if (value.empty()) {
        return false;
    }

    bool expectingIdentifier = true;
    for (const char c : value) {
        if (c == '.') {
            if (expectingIdentifier) {
                return false;
            }
            expectingIdentifier = true;
            continue;
        }
        if (!is_identifier_char(c)) {
            return false;
        }
        expectingIdentifier = false;
    }

    return !expectingIdentifier;
}

// Removes git-describe distance: "4" becomes empty and "rc.1-4" becomes "rc.1".
std::string_view trim_git_describe_suffix(std::string_view value) {
    if (value.ends_with("-dirty")) {
        value.remove_suffix(6);
    }
    if (is_numeric_identifier(value)) {
        return {};
    }

    const size_t suffixStart = value.rfind('-');
    if (suffixStart != std::string_view::npos &&
        value.substr(0, suffixStart).find('.') != std::string_view::npos &&
        is_numeric_identifier(value.substr(suffixStart + 1)))
    {
        value.remove_suffix(value.size() - suffixStart);
    }
    return value;
}

void split_identifiers(std::string_view value, std::vector<std::string>& identifiers) {
    while (!value.empty()) {
        const size_t separator = value.find('.');
        if (separator == std::string_view::npos) {
            identifiers.emplace_back(value);
            return;
        }
        identifiers.emplace_back(value.substr(0, separator));
        value.remove_prefix(separator + 1);
    }
}

std::string_view trim_leading_zeroes(std::string_view value) {
    while (value.size() > 1 && value.front() == '0') {
        value.remove_prefix(1);
    }
    return value;
}

int compare_identifier(std::string_view lhs, std::string_view rhs) {
    const bool lhsNumeric = is_numeric_identifier(lhs);
    const bool rhsNumeric = is_numeric_identifier(rhs);
    if (lhsNumeric && rhsNumeric) {
        lhs = trim_leading_zeroes(lhs);
        rhs = trim_leading_zeroes(rhs);
        if (lhs.size() != rhs.size()) {
            return lhs.size() < rhs.size() ? -1 : 1;
        }
    } else if (lhsNumeric != rhsNumeric) {
        // Numeric identifiers precede alphanumeric identifiers.
        return lhsNumeric ? -1 : 1;
    }

    const int result = lhs.compare(rhs);
    if (result < 0) {
        return -1;
    }
    if (result > 0) {
        return 1;
    }
    return 0;
}

}  // namespace

int compare_version(const Version& lhs, const Version& rhs) {
    if (lhs.major != rhs.major) {
        return lhs.major < rhs.major ? -1 : 1;
    }
    if (lhs.minor != rhs.minor) {
        return lhs.minor < rhs.minor ? -1 : 1;
    }
    if (lhs.patch != rhs.patch) {
        return lhs.patch < rhs.patch ? -1 : 1;
    }
    if (lhs.prerelease.empty() != rhs.prerelease.empty()) {
        return lhs.prerelease.empty() ? 1 : -1;
    }

    const size_t commonSize = std::min(lhs.prerelease.size(), rhs.prerelease.size());
    for (size_t i = 0; i < commonSize; ++i) {
        const int result = compare_identifier(lhs.prerelease[i], rhs.prerelease[i]);
        if (result != 0) {
            return result;
        }
    }
    if (lhs.prerelease.size() != rhs.prerelease.size()) {
        return lhs.prerelease.size() < rhs.prerelease.size() ? -1 : 1;
    }
    return 0;
}

std::optional<Version> parse_version(std::string_view value) {
    if (!value.empty() && value.front() == 'v') {
        value.remove_prefix(1);
    }

    Version version;
    auto major = parse_component(value);
    if (!major || !consume(value, '.')) {
        return std::nullopt;
    }
    auto minor = parse_component(value);
    if (!minor || !consume(value, '.')) {
        return std::nullopt;
    }
    auto patch = parse_component(value);
    if (!patch) {
        return std::nullopt;
    }

    version.major = *major;
    version.minor = *minor;
    version.patch = *patch;

    if (value.empty()) {
        return version;
    }
    if (value.front() == '+') {
        value.remove_prefix(1);
        if (!is_identifier_list(value)) {
            return std::nullopt;
        }
        return version;
    }
    if (!consume(value, '-')) {
        return std::nullopt;
    }

    const size_t buildStart = value.find('+');
    std::string_view prerelease = value.substr(0, buildStart);
    if (!is_identifier_list(prerelease)) {
        return std::nullopt;
    }
    if (buildStart != std::string_view::npos && !is_identifier_list(value.substr(buildStart + 1))) {
        return std::nullopt;
    }

    prerelease = trim_git_describe_suffix(prerelease);
    if (!prerelease.empty()) {
        split_identifiers(prerelease, version.prerelease);
    }
    return version;
}

}  // namespace borealis::update
