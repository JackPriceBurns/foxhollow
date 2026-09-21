#pragma once

#include "borealis/log.hpp"

#include <cxxopts.hpp>

#include <filesystem>
#include <optional>

namespace borealis::cli {

struct StandardOptionSet {
    bool logLevel = true;
    bool logDir = true;
    bool userDir = true;
    bool console = true;
};

struct StandardOptions {
    std::optional<LogLevel> level;
    std::filesystem::path logDir;
    std::filesystem::path userDir;
    bool console = false;

    void apply_to(log::Options& options) const;
};

/** Registers borealis' standard options into a cxxopts instance. */
void add_standard_options(cxxopts::Options& options, const StandardOptionSet& set = {});

/** Reads the standard options out of a parse result. */
StandardOptions parse(const cxxopts::ParseResult& parsed);

}  // namespace borealis::cli
