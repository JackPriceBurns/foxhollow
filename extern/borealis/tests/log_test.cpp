#include "borealis/aurora_log.h"

#include <gtest/gtest.h>
#include "borealis/io.hpp"
#include "borealis/log.hpp"
#include "borealis/log_c.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using borealis::LogLevel;

namespace {

fs::path make_test_dir(const char* name) {
    const fs::path dir = fs::temp_directory_path() / "borealis-log-test" / name;
    fs::remove_all(dir);
    fs::create_directories(dir);
    return dir;
}

std::vector<std::string> ring_texts() {
    std::vector<std::string> texts;
    borealis::log::visit_ring(
        [&](const borealis::log::Message& message) { texts.emplace_back(message.text); });
    return texts;
}

size_t count_log_files(const fs::path& dir) {
    size_t count = 0;
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.path().extension() == ".log") {
            ++count;
        }
    }
    return count;
}

void write_file_bytes(const fs::path& path, size_t bytes) {
    std::ofstream out(path, std::ios::binary);
    const std::string chunk(bytes, 'x');
    out.write(chunk.data(), static_cast<std::streamsize>(chunk.size()));
}

TEST(Log, BootstrapReplay) {
    struct CountingSink : borealis::log::Sink {
        int writes = 0;
        void write(const borealis::log::Message&) override { ++writes; }
        void flush() override {}
    };

    const fs::path dir = make_test_dir("bootstrap");
    auto sink = std::make_shared<CountingSink>();
    borealis::log::add_sink(sink);
    constexpr borealis::Log Log{"test::bootstrap"};
    Log.trace("bootstrap trace filtered later");
    Log.debug("bootstrap debug replayed");
    Log.info("bootstrap info replayed");

    borealis::log::init({.level = LogLevel::Debug,
        .console = false,
        .fileDirectory = dir,
        .filePrefix = "bootstrap",
        .ringCapacity = 8});
    borealis::log::flush();

    const auto texts = ring_texts();
    ASSERT_EQ(texts.size(), 2);
    EXPECT_EQ(texts[0], "bootstrap debug replayed");
    EXPECT_EQ(texts[1], "bootstrap info replayed");
    EXPECT_EQ(sink->writes, 2);

    ASSERT_NE(borealis::log::file_path(), nullptr);
    std::ifstream in(borealis::io::fs_path_from_utf8(borealis::log::file_path()));
    const std::string contents(
        (std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    EXPECT_EQ(contents.find("bootstrap trace filtered later"), std::string::npos);
    EXPECT_NE(contents.find("bootstrap debug replayed"), std::string::npos);
    EXPECT_NE(contents.find("bootstrap info replayed"), std::string::npos);
}

TEST(Log, LevelFiltering) {
    borealis::log::init({.level = LogLevel::Warning, .console = false, .ringCapacity = 16});
    constexpr borealis::Log Log{"test::filter"};
    Log.info("filtered out");
    Log.warn("kept");
    const auto texts = ring_texts();
    ASSERT_EQ(texts.size(), 1);
    EXPECT_EQ(texts[0], "kept");
}

TEST(Log, ModulePrefixLevels) {
    borealis::log::init({.level = LogLevel::Info, .console = false, .ringCapacity = 16});
    borealis::log::set_module_level("noisy", LogLevel::Error);
    constexpr borealis::Log Noisy{"noisy::gfx"};
    constexpr borealis::Log Other{"other"};
    Noisy.warn("suppressed by prefix");
    Noisy.error("noisy error kept");
    Other.warn("other warn kept");

    // Longest matching prefix wins.
    borealis::log::set_module_level("noisy::gfx", LogLevel::Trace);
    Noisy.trace("trace enabled for noisy::gfx");

    const auto texts = ring_texts();
    ASSERT_EQ(texts.size(), 3);
    EXPECT_EQ(texts[0], "noisy error kept");
    EXPECT_EQ(texts[1], "other warn kept");
    EXPECT_EQ(texts[2], "trace enabled for noisy::gfx");
    borealis::log::clear_module_levels();
}

TEST(Log, RingCapacity) {
    borealis::log::init({.level = LogLevel::Info, .console = false, .ringCapacity = 4});
    constexpr borealis::Log Log{"test::ring"};
    for (int i = 0; i < 6; ++i) {
        Log.info("message {}", i);
    }
    const auto texts = ring_texts();
    ASSERT_EQ(texts.size(), 4);
    EXPECT_EQ(texts.front(), "message 2");
    EXPECT_EQ(texts.back(), "message 5");
}

TEST(Log, FileNamingAndContent) {
    const fs::path dir = make_test_dir("naming");
    borealis::log::init(
        {.console = false, .fileDirectory = dir, .filePrefix = "testapp", .ringCapacity = 4});
    constexpr borealis::Log Log{"test::file"};
    Log.info("hello file");
    borealis::log::flush();

    ASSERT_NE(borealis::log::file_path(), nullptr);
    EXPECT_GE(borealis::log::file_descriptor(), 0);
    const fs::path logPath = borealis::io::fs_path_from_utf8(borealis::log::file_path());
    const std::string filename = logPath.filename().string();
    // "testapp-" + 8 digits + "-" + 6 digits + ".log"
    EXPECT_EQ(filename.size(), std::string("testapp-").size() + 19);
    EXPECT_TRUE(filename.starts_with("testapp-"));
    EXPECT_TRUE(filename.ends_with(".log"));

    std::ifstream in(logPath);
    std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    EXPECT_NE(contents.find("[INFO | borealis::log] File logging initialized"), std::string::npos);
    EXPECT_NE(contents.find("[INFO | test::file] hello file"), std::string::npos);

    borealis::log::shutdown();
    EXPECT_EQ(borealis::log::file_path(), nullptr);
    EXPECT_EQ(borealis::log::file_descriptor(), -1);
}

TEST(Log, RetentionCountBound) {
    const fs::path dir = make_test_dir("retention-count");
    // Create 12 old logs plus legacy and unrelated files.
    for (int i = 0; i < 12; ++i) {
        write_file_bytes(dir / ("testapp-202401" + std::to_string(10 + i) + "-000000.log"), 10);
    }
    write_file_bytes(dir / "oldapp-20240101-000000.log", 10);
    write_file_bytes(dir / "unrelated-20240101-000000.log", 10);
    write_file_bytes(dir / "testapp-notatimestamp.log", 10);

    borealis::log::init({.console = false,
        .fileDirectory = dir,
        .filePrefix = "testapp",
        .legacyFilePrefixes = {"oldapp"},
        .maxRetainedFiles = 10});
    borealis::log::shutdown();

    // Keep the 9 newest old logs and prune the oldest legacy log.
    EXPECT_FALSE(fs::exists(dir / "oldapp-20240101-000000.log"));
    EXPECT_FALSE(fs::exists(dir / "testapp-20240110-000000.log"));
    EXPECT_FALSE(fs::exists(dir / "testapp-20240111-000000.log"));
    EXPECT_FALSE(fs::exists(dir / "testapp-20240112-000000.log"));
    EXPECT_TRUE(fs::exists(dir / "testapp-20240113-000000.log"));
    EXPECT_TRUE(fs::exists(dir / "testapp-20240121-000000.log"));
    // Keep non-matching files.
    EXPECT_TRUE(fs::exists(dir / "unrelated-20240101-000000.log"));
    EXPECT_TRUE(fs::exists(dir / "testapp-notatimestamp.log"));
    // 9 old + 2 non-matching + 1 new = 12
    EXPECT_EQ(count_log_files(dir), 12);
}

TEST(Log, RetentionByteBound) {
    const fs::path dir = make_test_dir("retention-bytes");
    write_file_bytes(dir / "testapp-20240101-000000.log", 1000);
    write_file_bytes(dir / "testapp-20240102-000000.log", 1000);
    write_file_bytes(dir / "testapp-20240103-000000.log", 1000);

    borealis::log::init({.console = false,
        .fileDirectory = dir,
        .filePrefix = "testapp",
        .maxRetainedFiles = 10,
        .maxRetainedBytes = 1500});
    borealis::log::shutdown();

    // Only the newest old file fits within 1500 bytes.
    EXPECT_TRUE(fs::exists(dir / "testapp-20240103-000000.log"));
    EXPECT_FALSE(fs::exists(dir / "testapp-20240102-000000.log"));
    EXPECT_FALSE(fs::exists(dir / "testapp-20240101-000000.log"));
}

TEST(Log, Divert) {
    static std::vector<std::string> diverted;
    diverted.clear();
    borealis::log::init({.level = LogLevel::Info,
        .console = false,
        .ringCapacity = 16,
        .divert = [](const borealis::log::Message& message) {
            if (message.text.find("stub") != std::string_view::npos) {
                diverted.emplace_back(message.text);
                return true;
            }
            return false;
        }});
    constexpr borealis::Log Log{"test::divert"};
    Log.info("foo is a stub");
    Log.info("regular message");
    const auto texts = ring_texts();
    ASSERT_EQ(diverted.size(), 1);
    EXPECT_EQ(diverted[0], "foo is a stub");
    ASSERT_EQ(texts.size(), 1);
    EXPECT_EQ(texts[0], "regular message");
}

TEST(Log, CBridge) {
    borealis::log::init({.level = LogLevel::Info, .console = false, .ringCapacity = 16});
    borealis_log_printf(BOREALIS_LOG_INFO, "test::c", "formatted %d %s", 42, "ok");
    borealis_log_printf(BOREALIS_LOG_DEBUG, "test::c", "filtered %d", 1);
    borealis_log_write(BOREALIS_LOG_WARNING, "test::c", "plain");
    const auto texts = ring_texts();
    ASSERT_EQ(texts.size(), 2);
    EXPECT_EQ(texts[0], "formatted 42 ok");
    EXPECT_EQ(texts[1], "plain");
}

TEST(Log, AuroraCallback) {
    borealis::log::init({.level = LogLevel::Info, .console = false, .ringCapacity = 16});
    const AuroraLogCallback callback = borealis::log::aurora_callback();
    const char message[] = "from aurora";
    callback(LOG_INFO, "aurora::gfx", message, sizeof(message) - 1);
    callback(LOG_DEBUG, "aurora::gfx", message, sizeof(message) - 1);
    const auto texts = ring_texts();
    ASSERT_EQ(texts.size(), 1);
    EXPECT_EQ(texts[0], "from aurora");
}

TEST(Log, ExtraSink) {
    struct CountingSink : borealis::log::Sink {
        int writes = 0;
        int flushes = 0;
        void write(const borealis::log::Message&) override { ++writes; }
        void flush() override { ++flushes; }
    };
    borealis::log::init({.level = LogLevel::Info, .console = false, .ringCapacity = 4});
    auto sink = std::make_shared<CountingSink>();
    borealis::log::add_sink(sink);
    constexpr borealis::Log Log{"test::sink"};
    Log.info("one");
    Log.error("two");  // >= flushOn (Warning default) forces a flush
    EXPECT_EQ(sink->writes, 2);
    EXPECT_TRUE(sink->flushes >= 1);
}

TEST(Log, LevelNames) {
    EXPECT_EQ(borealis::to_string(LogLevel::Trace), "TRACE");
    EXPECT_EQ(borealis::to_string(LogLevel::Warning), "WARNING");
    EXPECT_EQ(borealis::to_string(LogLevel::Fatal), "FATAL");

    EXPECT_EQ(borealis::level_from_string("trace"), LogLevel::Trace);
    EXPECT_EQ(borealis::level_from_string("Debug"), LogLevel::Debug);
    EXPECT_EQ(borealis::level_from_string("WARNING"), LogLevel::Warning);
    EXPECT_EQ(borealis::level_from_string("warn"), LogLevel::Warning);
    EXPECT_FALSE(borealis::level_from_string("0").has_value());
    EXPECT_FALSE(borealis::level_from_string("").has_value());
    EXPECT_FALSE(borealis::level_from_string("verbose").has_value());
}

TEST(Log, AuroraLevelMapping) {
    EXPECT_EQ(borealis::log::from_aurora_level(LOG_DEBUG), LogLevel::Debug);
    EXPECT_EQ(borealis::log::from_aurora_level(LOG_FATAL), LogLevel::Fatal);
    // Trace maps to Aurora's Debug level.
    EXPECT_EQ(borealis::log::to_aurora_level(LogLevel::Trace), LOG_DEBUG);
    EXPECT_EQ(borealis::log::to_aurora_level(LogLevel::Debug), LOG_DEBUG);
    EXPECT_EQ(borealis::log::to_aurora_level(LogLevel::Error), LOG_ERROR);
    // All representable levels round-trip.
    for (const AuroraLogLevel level : {LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_FATAL}) {
        EXPECT_EQ(borealis::log::to_aurora_level(borealis::log::from_aurora_level(level)), level);
    }
}

}  // namespace
