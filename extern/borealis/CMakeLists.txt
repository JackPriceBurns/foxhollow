cmake_minimum_required(VERSION 3.25)
project(borealis VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BOREALIS_BUILD_TESTS "Build borealis tests" ${PROJECT_IS_TOP_LEVEL})
option(BOREALIS_ENABLE_CLI "Build borealis::cli (requires cxxopts)" ON)
option(BOREALIS_ENABLE_HTTP "Build borealis::http" ON)
option(BOREALIS_ENABLE_UPDATE "Build borealis::update (requires borealis::http and nlohmann_json)" ON)
option(BOREALIS_ENABLE_CRASH "Build borealis::crash" ON)
option(BOREALIS_ENABLE_DATA "Build borealis::data (requires SDL3 and nlohmann_json)" ON)
option(BOREALIS_ENABLE_FILE_SELECT "Build borealis::file_select (requires SDL3)" ON)
option(BOREALIS_ENABLE_DISC "Build borealis::disc (requires SDL3, nod, and xxhash)" ON)
option(BOREALIS_ENABLE_SENTRY "Build borealis::sentry with sentry-native/crashpad" OFF)
set(_borealis_discord_default ON)
if (ANDROID OR IOS OR TVOS OR CMAKE_SYSTEM_NAME MATCHES "^(iOS|tvOS)$")
    set(_borealis_discord_default OFF)
endif ()
option(BOREALIS_ENABLE_DISCORD "Build borealis::discord" ${_borealis_discord_default})

set(BOREALIS_SENTRY_DSN "" CACHE STRING "Sentry DSN for borealis::sentry")
set(BOREALIS_SENTRY_ENVIRONMENT "development" CACHE STRING "Sentry environment for borealis::sentry")
set(BOREALIS_HTTP_BACKEND "auto" CACHE STRING "HTTP backend for borealis::http")
set_property(CACHE BOREALIS_HTTP_BACKEND PROPERTY STRINGS auto winhttp urlsession curl android none)
set(BOREALIS_AURORA_DIR "" CACHE PATH "Path to an aurora source tree (if no aurora::core target)")

if (NOT TARGET fmt::fmt)
    find_package(fmt 11 CONFIG QUIET)
    if (NOT fmt_FOUND)
        include(FetchContent)
        FetchContent_Declare(fmt
                URL https://github.com/fmtlib/fmt/archive/refs/tags/12.1.0.tar.gz
                URL_HASH SHA256=ea7de4299689e12b6dddd392f9896f08fb0777ac7168897a244a6d6085043fea
                DOWNLOAD_EXTRACT_TIMESTAMP FALSE
        )
        FetchContent_MakeAvailable(fmt)
    endif ()
endif ()

# Version detection
set(BOREALIS_APP_SOURCE_DIR "${CMAKE_SOURCE_DIR}" CACHE PATH "Source directory of the application")
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/DetectVersion.cmake)
if (NOT DEFINED BOREALIS_APP_DESCRIBE)
    borealis_detect_version()
endif ()
set_property(GLOBAL PROPERTY BOREALIS_ANDROID_VERSION_NAME "${BOREALIS_APP_DESCRIBE}")
set_property(GLOBAL PROPERTY BOREALIS_ANDROID_VERSION_CODE "${BOREALIS_APP_VERSION_CODE}")
borealis_detect_platform_name()
borealis_git_describe(${CMAKE_CURRENT_SOURCE_DIR} BOREALIS_LIB_DESCRIBE)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/BorealisAndroid.cmake)
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/generated/borealis/version.h
        @ONLY
)

add_library(borealis_core INTERFACE)
add_library(borealis::core ALIAS borealis_core)
target_include_directories(borealis_core INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/generated
)
target_compile_features(borealis_core INTERFACE cxx_std_20)
target_compile_definitions(borealis_core INTERFACE BOREALIS_BUILD_TYPE="$<IF:$<BOOL:$<CONFIG>>,$<CONFIG>,None>")

# For standalone builds, fetch Aurora main branch to use its CMake modules + includes
set(_borealis_aurora_cmake_dir "")
if (NOT TARGET aurora::core)
    if (NOT BOREALIS_AURORA_DIR)
        message(STATUS "borealis: fetching aurora sources")
        include(FetchContent)
        FetchContent_Declare(borealis_aurora
                GIT_REPOSITORY https://github.com/encounter/aurora.git
                GIT_TAG main
                GIT_SHALLOW TRUE
                SOURCE_SUBDIR include # avoid configuring cmake
        )
        FetchContent_MakeAvailable(borealis_aurora)
        set(BOREALIS_AURORA_DIR "${borealis_aurora_SOURCE_DIR}")
    endif ()
    if (NOT EXISTS "${BOREALIS_AURORA_DIR}/include/aurora/aurora.h")
        message(FATAL_ERROR "BOREALIS_AURORA_DIR is not an aurora source tree: ${BOREALIS_AURORA_DIR}")
    endif ()
    if (EXISTS "${BOREALIS_AURORA_DIR}/cmake/AuroraDependencyVersions.cmake")
        set(_borealis_aurora_cmake_dir "${BOREALIS_AURORA_DIR}/cmake")
        include("${_borealis_aurora_cmake_dir}/AuroraDependencyVersions.cmake")
        if (WIN32)
            set(_borealis_sdl3_provider_default vendor)
        else ()
            set(_borealis_sdl3_provider_default auto)
        endif ()
        set(AURORA_SDL3_PROVIDER "${_borealis_sdl3_provider_default}" CACHE STRING
                "How to provide SDL3: auto, vendor (build from source), system (find_package/imported), or package (download prebuilt)")
        set(AURORA_SDL3_LINKAGE "static" CACHE STRING "SDL3 linkage type preference")
        set(AURORA_NOD_PROVIDER "auto" CACHE STRING
                "How to provide nod: auto, vendor (build from source), system (find_package/imported), package (download prebuilt)")
        set(AURORA_NOD_LINKAGE "static" CACHE STRING "nod linkage type preference")
    endif ()
endif ()

add_library(borealis_log STATIC src/log.cpp)
add_library(borealis::log ALIAS borealis_log)
target_link_libraries(borealis_log PUBLIC borealis_core fmt::fmt)
if (TARGET aurora::core)
    target_link_libraries(borealis_log PUBLIC aurora::core)
else ()
    target_include_directories(borealis_log PUBLIC ${BOREALIS_AURORA_DIR}/include)
endif ()
if (ANDROID)
    target_link_libraries(borealis_log PRIVATE log)
endif ()

if (BOREALIS_ENABLE_CRASH)
    add_library(borealis_crash STATIC src/crash.cpp)
    add_library(borealis::crash ALIAS borealis_crash)
    target_link_libraries(borealis_crash PUBLIC borealis_log PRIVATE ${CMAKE_DL_LIBS})
    if (WIN32)
        target_compile_definitions(borealis_crash PRIVATE
                $<$<CONFIG:Debug>:BOREALIS_CRASH_DBGHELP=1>)
        target_link_libraries(borealis_crash PRIVATE $<$<CONFIG:Debug>:dbghelp>)
    endif ()
endif ()

if (BOREALIS_ENABLE_SENTRY)
    include(FetchContent)
    if (NOT TARGET sentry)
        message(STATUS "borealis::sentry: fetching sentry-native")
        set(SENTRY_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
        set(SENTRY_BACKEND crashpad CACHE STRING "" FORCE)
        if (WIN32)
            set(SENTRY_TRANSPORT winhttp CACHE STRING "" FORCE)
        endif ()
        set(SENTRY_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(SENTRY_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(SENTRY_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
        FetchContent_Declare(sentry_native
                GIT_REPOSITORY https://github.com/getsentry/sentry-native.git
                GIT_TAG 0.13.6
                GIT_SHALLOW TRUE
                GIT_PROGRESS TRUE
                GIT_SUBMODULES_RECURSE TRUE
        )
        set(_borealis_skip_install_rules ${CMAKE_SKIP_INSTALL_RULES})
        set(CMAKE_SKIP_INSTALL_RULES ON)

        if (APPLE AND NOT TARGET CURL::libcurl)
            find_package(CURL REQUIRED)
        endif ()
        if (APPLE AND TARGET CURL::libcurl)
            get_target_property(_borealis_curl_includes CURL::libcurl INTERFACE_INCLUDE_DIRECTORIES)
            if (_borealis_curl_includes)
                list(FILTER _borealis_curl_includes EXCLUDE REGEX "/MacOSX[^/]*\\.sdk/usr/include$")
                set_property(TARGET CURL::libcurl PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${_borealis_curl_includes}")
            endif ()
        endif ()

        FetchContent_MakeAvailable(sentry_native)
        set(CMAKE_SKIP_INSTALL_RULES ${_borealis_skip_install_rules})
    endif ()
    if (NOT TARGET sentry)
        message(FATAL_ERROR "borealis::sentry requested, but sentry-native did not provide target 'sentry'")
    endif ()
endif ()

add_library(borealis_sentry STATIC
        src/sentry.cpp
        src/sentry_config.cpp
)
add_library(borealis::sentry ALIAS borealis_sentry)
target_link_libraries(borealis_sentry PUBLIC borealis_log)
if (BOREALIS_ENABLE_SENTRY)
    string(REPLACE "\\" "\\\\" _borealis_sentry_dsn "${BOREALIS_SENTRY_DSN}")
    string(REPLACE "\"" "\\\"" _borealis_sentry_dsn "${_borealis_sentry_dsn}")
    string(REPLACE "\\" "\\\\" _borealis_sentry_environment "${BOREALIS_SENTRY_ENVIRONMENT}")
    string(REPLACE "\"" "\\\"" _borealis_sentry_environment "${_borealis_sentry_environment}")
    target_compile_definitions(borealis_sentry
            PUBLIC BOREALIS_HAS_SENTRY=1
            PRIVATE
            BOREALIS_SENTRY_DSN="${_borealis_sentry_dsn}"
            BOREALIS_SENTRY_ENVIRONMENT="${_borealis_sentry_environment}"
            SENTRY_BUILD_STATIC=1
    )
    target_link_libraries(borealis_sentry PRIVATE sentry)
else ()
    target_compile_definitions(borealis_sentry PUBLIC BOREALIS_HAS_SENTRY=0)
endif ()

if (BOREALIS_ENABLE_CLI)
    if (NOT TARGET cxxopts::cxxopts)
        find_package(cxxopts 3 CONFIG QUIET)
        if (NOT cxxopts_FOUND)
            include(FetchContent)
            FetchContent_Declare(cxxopts
                    URL https://github.com/jarro2783/cxxopts/archive/refs/tags/v3.3.1.tar.gz
                    URL_HASH SHA256=3bfc70542c521d4b55a46429d808178916a579b28d048bd8c727ee76c39e2072
                    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
            )
            FetchContent_MakeAvailable(cxxopts)
        endif ()
    endif ()

    add_library(borealis_cli STATIC src/cli.cpp)
    add_library(borealis::cli ALIAS borealis_cli)
    target_link_libraries(borealis_cli PUBLIC borealis_log cxxopts::cxxopts)
endif ()

set(_borealis_sdl_needed OFF)
set(_borealis_json_needed OFF)
add_library(borealis_sdl_dep INTERFACE)
add_library(borealis_json_dep INTERFACE)

add_library(borealis_presentation STATIC src/presentation.cpp)
add_library(borealis::presentation ALIAS borealis_presentation)
target_link_libraries(borealis_presentation PUBLIC borealis_core)
if (ANDROID)
    set(_borealis_sdl_needed ON)
    target_link_libraries(borealis_presentation PRIVATE borealis_sdl_dep)
endif ()

if (BOREALIS_ENABLE_DISC)
    set(_borealis_sdl_needed ON)
    if (NOT TARGET aurora::dvd AND NOT TARGET nod::nod AND _borealis_aurora_cmake_dir)
        include("${_borealis_aurora_cmake_dir}/AuroraNodProvider.cmake")
    endif ()
    if (TARGET aurora::dvd)
        set(_borealis_nod_target aurora::dvd)
    elseif (TARGET nod::nod)
        set(_borealis_nod_target nod::nod)
    else ()
        message(FATAL_ERROR "borealis::disc requires aurora::dvd, nod::nod, or aurora sources "
                "providing AuroraNodProvider.cmake")
    endif ()

    if (NOT TARGET xxhash AND NOT TARGET xxHash::xxhash)
        message(STATUS "borealis: fetching xxhash")
        include(FetchContent)
        FetchContent_Declare(xxhash
                URL https://github.com/Cyan4973/xxHash/archive/refs/tags/v0.8.3.tar.gz
                URL_HASH SHA256=aae608dfe8213dfd05d909a57718ef82f30722c392344583d3f39050c7f29a80
                SOURCE_SUBDIR cmake_unofficial
                DOWNLOAD_EXTRACT_TIMESTAMP FALSE
                EXCLUDE_FROM_ALL
        )
        set(XXHASH_BUILD_XXHSUM OFF CACHE INTERNAL "Build the xxhsum binary")
        set(_borealis_had_bsl_cache OFF)
        if (DEFINED CACHE{BUILD_SHARED_LIBS})
            set(_borealis_had_bsl_cache ON)
        endif ()
        FetchContent_MakeAvailable(xxhash)
        if (NOT _borealis_had_bsl_cache)
            unset(BUILD_SHARED_LIBS CACHE)
        endif ()
    endif ()
    if (TARGET xxhash)
        set(_borealis_xxhash_target xxhash)
    elseif (TARGET xxHash::xxhash)
        set(_borealis_xxhash_target xxHash::xxhash)
    else ()
        message(FATAL_ERROR "borealis::disc requires an xxhash or xxHash::xxhash target")
    endif ()

    add_library(borealis_disc STATIC src/disc.cpp)
    add_library(borealis::disc ALIAS borealis_disc)
    target_link_libraries(borealis_disc
            PUBLIC borealis_core ${_borealis_xxhash_target}
            PRIVATE ${_borealis_nod_target} borealis_sdl_dep)
endif ()

if (BOREALIS_ENABLE_DATA)
    set(_borealis_sdl_needed ON)
    set(_borealis_json_needed ON)
    add_library(borealis_data STATIC src/data.cpp)
    add_library(borealis::data ALIAS borealis_data)
    target_link_libraries(borealis_data
            PUBLIC borealis_log
            PRIVATE borealis_json_dep borealis_sdl_dep)
endif ()

if (BOREALIS_ENABLE_FILE_SELECT)
    set(_borealis_sdl_needed ON)
    add_library(borealis_file_select STATIC src/file_select/file_select.cpp)
    add_library(borealis::file_select ALIAS borealis_file_select)
    target_link_libraries(borealis_file_select
            PUBLIC borealis_core
            PRIVATE borealis_sdl_dep)

    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        find_library(BOREALIS_APPKIT_FRAMEWORK AppKit REQUIRED)
        target_sources(borealis_file_select PRIVATE src/file_select/macos.mm)
        set_source_files_properties(src/file_select/macos.mm PROPERTIES COMPILE_FLAGS -fobjc-arc)
        target_link_libraries(borealis_file_select PRIVATE ${BOREALIS_APPKIT_FRAMEWORK})
    elseif (IOS OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
        find_library(BOREALIS_UIKIT_FRAMEWORK UIKit REQUIRED)
        find_library(BOREALIS_UNIFORM_TYPE_IDENTIFIERS_FRAMEWORK
                UniformTypeIdentifiers REQUIRED)
        target_sources(borealis_file_select PRIVATE src/file_select/ios.mm)
        set_source_files_properties(src/file_select/ios.mm PROPERTIES COMPILE_FLAGS -fobjc-arc)
        target_link_libraries(borealis_file_select PRIVATE
                ${BOREALIS_UIKIT_FRAMEWORK}
                ${BOREALIS_UNIFORM_TYPE_IDENTIFIERS_FRAMEWORK})
    elseif (ANDROID)
        target_sources(borealis_file_select PRIVATE src/file_select/android.cpp)
    endif ()
endif ()

if (BOREALIS_ENABLE_DISCORD)
    set(_borealis_json_needed ON)
    if (ANDROID OR IOS OR TVOS OR CMAKE_SYSTEM_NAME MATCHES "^(iOS|tvOS)$")
        message(FATAL_ERROR "borealis::discord is not supported on this platform")
    endif ()
    find_package(Threads REQUIRED)
    add_library(borealis_discord STATIC src/discord/discord.cpp src/discord/discord_protocol.cpp)
    target_compile_definitions(borealis_discord PUBLIC BOREALIS_HAS_DISCORD=1)
    target_link_libraries(borealis_discord PUBLIC borealis_log PRIVATE borealis_json_dep Threads::Threads)
else ()
    add_library(borealis_discord STATIC src/discord/stub.cpp)
    target_compile_definitions(borealis_discord PUBLIC BOREALIS_HAS_DISCORD=0)
    target_link_libraries(borealis_discord PUBLIC borealis_core)
endif ()
add_library(borealis::discord ALIAS borealis_discord)

if (BOREALIS_ENABLE_HTTP)
    set(_borealis_http_backend "${BOREALIS_HTTP_BACKEND}")
    set(_borealis_http_explicit TRUE)
    if (_borealis_http_backend STREQUAL "auto")
        set(_borealis_http_explicit FALSE)
        if (WIN32)
            set(_borealis_http_backend winhttp)
        elseif (ANDROID)
            set(_borealis_http_backend android)
        elseif (APPLE)
            set(_borealis_http_backend urlsession)
        else ()
            set(_borealis_http_backend curl)
        endif ()
    endif ()

    if (_borealis_http_backend STREQUAL "curl")
        find_package(CURL QUIET OPTIONAL_COMPONENTS HTTPS SSL)
        if (NOT (CURL_FOUND AND CURL_HTTPS_FOUND AND CURL_SSL_FOUND))
            if (_borealis_http_explicit)
                message(FATAL_ERROR "borealis::http: BOREALIS_HTTP_BACKEND=curl, but libcurl with HTTPS/SSL was not found")
            endif ()
            message(WARNING "borealis::http: libcurl with HTTPS/SSL not found; building with no HTTP backend")
            set(_borealis_http_backend none)
        endif ()
    endif ()

    if (_borealis_http_backend STREQUAL "winhttp")
        set(_borealis_http_source src/http/winhttp.cpp)
    elseif (_borealis_http_backend STREQUAL "urlsession")
        set(_borealis_http_source src/http/url_session.mm)
        set_source_files_properties(src/http/url_session.mm PROPERTIES COMPILE_FLAGS -fobjc-arc)
        find_library(BOREALIS_FOUNDATION_FRAMEWORK Foundation REQUIRED)
    elseif (_borealis_http_backend STREQUAL "android")
        set(_borealis_http_source src/http/android.cpp)
    elseif (_borealis_http_backend STREQUAL "curl")
        set(_borealis_http_source src/http/curl.cpp)
    elseif (_borealis_http_backend STREQUAL "none")
        set(_borealis_http_source src/http/no_backend.cpp)
    else ()
        message(FATAL_ERROR "borealis::http: unknown BOREALIS_HTTP_BACKEND '${BOREALIS_HTTP_BACKEND}'")
    endif ()

    add_library(borealis_http STATIC ${_borealis_http_source})
    add_library(borealis::http ALIAS borealis_http)
    target_link_libraries(borealis_http PUBLIC borealis_core)

    if (_borealis_http_backend STREQUAL "winhttp")
        target_link_libraries(borealis_http PRIVATE winhttp)
    elseif (_borealis_http_backend STREQUAL "urlsession")
        target_link_libraries(borealis_http PRIVATE ${BOREALIS_FOUNDATION_FRAMEWORK})
    elseif (_borealis_http_backend STREQUAL "curl")
        target_link_libraries(borealis_http PRIVATE CURL::libcurl)
    elseif (_borealis_http_backend STREQUAL "android")
        set(_borealis_sdl_needed ON)
        target_link_libraries(borealis_http PRIVATE borealis_sdl_dep)
    endif ()
    message(STATUS "borealis::http: backend ${_borealis_http_backend}")
endif ()

if (BOREALIS_ENABLE_UPDATE)
    set(_borealis_json_needed ON)
    if (NOT TARGET borealis_http)
        message(FATAL_ERROR "borealis::update requires borealis::http")
    endif ()
    add_library(borealis_update STATIC src/update/github.cpp src/update/version.cpp)
    add_library(borealis::update ALIAS borealis_update)
    target_link_libraries(borealis_update
            PUBLIC borealis_http
            PRIVATE fmt::fmt borealis_json_dep
    )
endif ()

if (_borealis_sdl_needed)
    if (NOT TARGET SDL3::SDL3 AND NOT TARGET SDL3::SDL3-static)
        if (_borealis_aurora_cmake_dir)
            include("${_borealis_aurora_cmake_dir}/AuroraSDL3Provider.cmake")
        else ()
            find_package(SDL3 CONFIG REQUIRED)
        endif ()
    endif ()
    unset(_borealis_sdl_target)
    if (DEFINED AURORA_SDL3_TARGET AND NOT AURORA_SDL3_TARGET STREQUAL "")
        if (TARGET ${AURORA_SDL3_TARGET})
            set(_borealis_sdl_target ${AURORA_SDL3_TARGET})
        endif ()
    endif ()
    if (_borealis_sdl_target)
        # pass
    elseif (TARGET SDL3::SDL3)
        set(_borealis_sdl_target SDL3::SDL3)
    elseif (TARGET SDL3::SDL3-static)
        set(_borealis_sdl_target SDL3::SDL3-static)
    else ()
        message(FATAL_ERROR "SDL3 is required by an enabled Borealis module")
    endif ()
    target_link_libraries(borealis_sdl_dep INTERFACE ${_borealis_sdl_target})
endif ()

if (_borealis_json_needed)
    if (NOT TARGET nlohmann_json::nlohmann_json)
        find_package(nlohmann_json 3 CONFIG QUIET)
        if (NOT nlohmann_json_FOUND)
            include(FetchContent)
            FetchContent_Declare(nlohmann_json
                    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
                    URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
                    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
            )
            FetchContent_MakeAvailable(nlohmann_json)
        endif ()
    endif ()
    target_link_libraries(borealis_json_dep INTERFACE nlohmann_json::nlohmann_json)
endif ()

if (BOREALIS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()
