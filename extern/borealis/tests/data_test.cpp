#include "data_internal.hpp"

#include <gtest/gtest.h>

#include "borealis/log.hpp"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;
using borealis::data::ErrorCode;
using borealis::data::LocationMode;
using borealis::data::Options;
using borealis::data::detail::Environment;
using borealis::data::detail::ManagerBackend;

namespace {

fs::path make_test_dir(const char* name) {
    const fs::path path = fs::temp_directory_path() / "borealis-data-test" / name;
    std::error_code ec;
    fs::remove_all(path, ec);
    fs::create_directories(path);
    return path;
}

void write_text(const fs::path& path, std::string_view text) {
    fs::create_directories(path.parent_path());
    std::ofstream output{path, std::ios::binary | std::ios::trunc};
    output << text;
    EXPECT_TRUE(output.good());
}

std::string read_text(const fs::path& path) {
    std::ifstream input{path, std::ios::binary};
    return {std::istreambuf_iterator<char>{input}, std::istreambuf_iterator<char>{}};
}

void write_descriptor(const fs::path& path, LocationMode mode, const fs::path& customPath = {},
    const fs::path& previousPath = {}) {
    const char* modeName = mode == LocationMode::Default  ? "default" :
                           mode == LocationMode::Portable ? "portable" :
                                                            "custom";
    nlohmann::json json{{"version", 1}, {"mode", modeName}};
    if (!customPath.empty()) {
        json["customPath"] = customPath.string();
    }
    if (!previousPath.empty()) {
        json["previousPath"] = previousPath.string();
    }
    write_text(path, json.dump(4));
}

nlohmann::json read_json(const fs::path& path) {
    return nlohmann::json::parse(read_text(path));
}

Environment make_environment(const fs::path& root) {
    return {
        .preferencePath = root / "prefs" / "TestApp",
        .basePath = root / "base",
        .homePath = root / "home",
        .documentsPath = root / "documents",
        .legacyPreferencePaths = {root / "prefs" / "OldApp"},
        .canOpenFolder = true,
    };
}

Options make_options() {
    return {
        .portableRelativePath = "data",
        .legacyApps = {{.orgName = "TestOrg", .appName = "OldApp"}},
        .migration =
            {
                .directories = {"saves", "textures"},
                .files = {"config.json", "controller_ports.dat"},
                .extensions = {".controller", ".gci"},
                .filenamePatterns = {{.prefix = "MemoryCard", .suffix = ".raw"}},
            },
    };
}

class DataTest : public testing::Test {
protected:
    static void SetUpTestSuite() { borealis::log::init({.console = false}); }
};

TEST_F(DataTest, DescriptorPrecedence) {
    const auto root = make_test_dir("descriptor-precedence");
    auto environment = make_environment(root);
    const auto baseTarget = root / "base-target";
    const auto prefTarget = root / "pref-target";
    write_descriptor(environment.basePath / "data_location.json", LocationMode::Custom, baseTarget);
    write_descriptor(
        environment.preferencePath / "data_location.json", LocationMode::Custom, prefTarget);

    ManagerBackend manager{make_options(), environment};
    EXPECT_TRUE(manager.initialize({}));
    EXPECT_EQ(manager.active_data_path(), baseTarget);
    EXPECT_EQ(manager.paths().cachePath, environment.preferencePath);
    EXPECT_EQ(manager.configured_mode(), LocationMode::Custom);

    const auto replacementTarget = root / "replacement-target";
    EXPECT_TRUE(manager.set_custom_data_path(replacementTarget));
    EXPECT_TRUE(read_json(environment.basePath / "data_location.json")
                    .at("customPath")
                    .get<std::string>() == fs::absolute(replacementTarget).string());
    EXPECT_TRUE(read_json(environment.preferencePath / "data_location.json")
                    .at("customPath")
                    .get<std::string>() == prefTarget.string());
}

TEST_F(DataTest, NewDescriptorPlacementFollowsMode) {
    const auto customRoot = make_test_dir("custom-descriptor-placement");
    auto customEnvironment = make_environment(customRoot);
    customEnvironment.isWindows = true;
    ManagerBackend customManager{make_options(), customEnvironment};
    EXPECT_TRUE(customManager.initialize({}));
    EXPECT_TRUE(customManager.set_custom_data_path(customRoot / "custom"));
    EXPECT_TRUE(fs::exists(customEnvironment.preferencePath / "data_location.json"));
    EXPECT_FALSE(fs::exists(customEnvironment.basePath / "data_location.json"));

    const auto portableRoot = make_test_dir("portable-descriptor-placement");
    auto portableEnvironment = make_environment(portableRoot);
    portableEnvironment.isWindows = false;
    ManagerBackend portableManager{make_options(), portableEnvironment};
    EXPECT_TRUE(portableManager.initialize({}));
    EXPECT_TRUE(portableManager.set_portable_data_path());
    EXPECT_TRUE(fs::exists(portableEnvironment.basePath / "data_location.json"));
    EXPECT_FALSE(fs::exists(portableEnvironment.preferencePath / "data_location.json"));
    EXPECT_EQ(
        read_json(portableEnvironment.basePath / "data_location.json").at("mode"), "portable");
}

TEST_F(DataTest, UserOverrideIsolated) {
    const auto root = make_test_dir("user-override");
    auto environment = make_environment(root);
    const auto descriptorTarget = root / "descriptor-target";
    const auto overrideTarget = root / "override-target";
    write_descriptor(environment.basePath / "data_location.json", LocationMode::Custom,
        descriptorTarget, environment.preferencePath);
    write_text(environment.preferencePath / "config.json", "old");

    ManagerBackend manager{make_options(), environment};
    EXPECT_TRUE(manager.initialize(overrideTarget));
    EXPECT_EQ(manager.active_data_path(), overrideTarget);
    EXPECT_TRUE(manager.has_user_directory_override());
    EXPECT_TRUE(fs::exists(environment.preferencePath / "config.json"));
    EXPECT_FALSE(fs::exists(overrideTarget / "config.json"));
    EXPECT_EQ(manager.set_custom_data_path(root / "another").code, ErrorCode::OverrideActive);
    EXPECT_FALSE(manager.capabilities().canChangeLocation);
}

TEST_F(DataTest, ChainedLocationChangeKeepsActiveSource) {
    const auto root = make_test_dir("chained-change");
    auto environment = make_environment(root);
    ManagerBackend manager{make_options(), environment};
    EXPECT_TRUE(manager.initialize({}));
    const auto active = manager.active_data_path();
    const auto second = root / "second";
    const auto third = root / "third";

    EXPECT_TRUE(manager.set_custom_data_path(second));
    EXPECT_TRUE(manager.set_custom_data_path(third));
    EXPECT_EQ(manager.configured_data_path(), third);
    EXPECT_TRUE(manager.is_data_path_restart_pending());

    const auto json = read_json(environment.preferencePath / "data_location.json");
    EXPECT_EQ(json.at("customPath").get<std::string>(), fs::absolute(third).string());
    EXPECT_EQ(json.at("previousPath").get<std::string>(), active.string());
}

TEST_F(DataTest, SuccessfulMigrationClearsPreviousPath) {
    const auto root = make_test_dir("migration-success");
    auto environment = make_environment(root);
    const auto source = root / "source";
    const auto target = root / "target";
    write_descriptor(
        environment.preferencePath / "data_location.json", LocationMode::Custom, target, source);
    write_text(source / "config.json", "config");
    write_text(source / "profile.controller", "controller");
    write_text(source / "MemoryCardA.raw", "card");
    write_text(source / "unrelated.bin", "leave");
    write_text(source / "data_location.json", "leave descriptor");
    write_text(source / "controller_ports.dat" / "not-a-file", "leave directory");
    write_text(source / "saves" / "slot.dat", "save");

    std::error_code symlinkError;
    fs::create_symlink("unrelated.bin", source / "linked.gci", symlinkError);

    ManagerBackend manager{make_options(), environment};
    EXPECT_TRUE(manager.initialize({}));
    EXPECT_EQ(read_text(target / "config.json"), "config");
    EXPECT_EQ(read_text(target / "profile.controller"), "controller");
    EXPECT_EQ(read_text(target / "MemoryCardA.raw"), "card");
    EXPECT_EQ(read_text(target / "saves" / "slot.dat"), "save");
    EXPECT_TRUE(fs::exists(source / "unrelated.bin"));
    EXPECT_TRUE(fs::exists(source / "data_location.json"));
    EXPECT_TRUE(fs::exists(source / "controller_ports.dat" / "not-a-file"));
    EXPECT_FALSE(fs::exists(target / "controller_ports.dat"));
    if (!symlinkError) {
        EXPECT_TRUE(fs::is_symlink(target / "linked.gci"));
        EXPECT_FALSE(fs::is_symlink(source / "linked.gci"));
    }
    EXPECT_FALSE(
        read_json(environment.preferencePath / "data_location.json").contains("previousPath"));
}

TEST_F(DataTest, CollisionRetainsPreviousPath) {
    const auto root = make_test_dir("migration-collision");
    auto environment = make_environment(root);
    const auto source = root / "source";
    const auto target = root / "target";
    write_descriptor(
        environment.preferencePath / "data_location.json", LocationMode::Custom, target, source);
    write_text(source / "config.json", "source");
    write_text(target / "config.json", "target");

    ManagerBackend manager{make_options(), environment};
    const auto status = manager.initialize({});
    EXPECT_EQ(status.code, ErrorCode::MigrationIncomplete);
    EXPECT_EQ(read_text(source / "config.json"), "source");
    EXPECT_EQ(read_text(target / "config.json"), "target");
    EXPECT_TRUE(
        read_json(environment.preferencePath / "data_location.json").contains("previousPath"));
}

TEST_F(DataTest, NestedDestinationIsNotRevisited) {
    const auto root = make_test_dir("nested-destination");
    auto environment = make_environment(root);
    const auto source = root / "source";
    const auto target = source / "nested-target";
    write_descriptor(
        environment.preferencePath / "data_location.json", LocationMode::Custom, target, source);
    write_text(source / "config.json", "config");

    ManagerBackend manager{make_options(), environment};
    EXPECT_TRUE(manager.initialize({}));
    EXPECT_EQ(read_text(target / "config.json"), "config");
    EXPECT_FALSE(fs::exists(target / "nested-target"));
}

TEST_F(DataTest, LegacyDirectoryButNotTextDescriptor) {
    const auto root = make_test_dir("legacy-directory");
    auto environment = make_environment(root);
    const auto legacy = root / "old-organization" / "OldApp";
    environment.legacyPreferencePaths = {legacy};
    write_text(legacy / "config.json", "legacy config");
    write_text(legacy / "data-location.txt", "/never/supported");

    ManagerBackend manager{make_options(), environment};
    EXPECT_TRUE(manager.initialize({}));
    EXPECT_EQ(read_text(environment.preferencePath / "config.json"), "legacy config");
    EXPECT_TRUE(fs::exists(legacy / "data-location.txt"));
}

TEST_F(DataTest, PlatformDefaultPolicies) {
    const auto windowsRoot = make_test_dir("windows-default");
    auto windowsEnvironment = make_environment(windowsRoot);
    windowsEnvironment.isWindows = true;
    auto windowsOptions = make_options();
    windowsOptions.defaultPath.useExecutableDirectoryOnWindows = true;
    ManagerBackend windowsManager{windowsOptions, windowsEnvironment};
    EXPECT_TRUE(windowsManager.initialize({}));
    EXPECT_EQ(windowsManager.active_data_path(), windowsEnvironment.basePath);
    EXPECT_TRUE(windowsManager.set_portable_data_path());
    EXPECT_EQ(windowsManager.configured_data_path(), windowsEnvironment.basePath / "data");

    const auto iosRoot = make_test_dir("ios-default");
    auto iosEnvironment = make_environment(iosRoot);
    iosEnvironment.isIOS = true;
    iosEnvironment.canChangeLocation = false;
    auto iosOptions = make_options();
    iosOptions.defaultPath.useDocumentsOnIOS = true;
    ManagerBackend iosManager{iosOptions, iosEnvironment};
    EXPECT_TRUE(iosManager.initialize({}));
    EXPECT_EQ(iosManager.active_data_path(), iosEnvironment.documentsPath);
    EXPECT_EQ(iosManager.set_custom_data_path(iosRoot / "custom").code, ErrorCode::Unsupported);
}

}  // namespace
