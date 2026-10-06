cmake_minimum_required(VERSION 3.25)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
            "Build type options: Debug Release RelWithDebInfo MinSizeRel" FORCE)
endif ()

project(foxhollow LANGUAGES C CXX VERSION 0.5.8)

option(FOXHOLLOW_DEBUG_SHORTCUTS "Enable Foxhollow development shortcuts" ON)
option(FOXHOLLOW_DEBUG_INPUT_TIMING "Build the input timing overlay into the performance overlay" OFF)

if (APPLE)
    enable_language(OBJC OBJCXX)
    if (NOT CMAKE_OSX_SYSROOT)
        execute_process(COMMAND xcrun --sdk macosx --show-sdk-path
                OUTPUT_VARIABLE CMAKE_OSX_SYSROOT
                OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif ()
endif ()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (MSVC)
    set(FOXHOLLOW_C_DIALECT_FLAGS /clang:-std=gnu11 /clang:-fsigned-char /clang:-fcommon)
    set(FOXHOLLOW_CXX_DIALECT_FLAGS /clang:-fsigned-char)
    set(FOXHOLLOW_NO_WARN_FLAGS /w -Wno-error=incompatible-pointer-types)
    set(FOXHOLLOW_FORCE_INCLUDE_FLAGS /FI${CMAKE_CURRENT_SOURCE_DIR}/port/include/foxhollow_compat.h)
else ()
    set(FOXHOLLOW_C_DIALECT_FLAGS -std=gnu11 -fsigned-char -fcommon)
    set(FOXHOLLOW_CXX_DIALECT_FLAGS -fsigned-char)
    set(FOXHOLLOW_NO_WARN_FLAGS -w -Wno-error=incompatible-pointer-types)
    set(FOXHOLLOW_FORCE_INCLUDE_FLAGS -include ${CMAKE_CURRENT_SOURCE_DIR}/port/include/foxhollow_compat.h)
endif ()

if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm64|aarch64|ARM64)$")
    set(FOXHOLLOW_PATCH_ENTRY_NOPS 4)
else ()
    set(FOXHOLLOW_PATCH_ENTRY_NOPS 16)
endif ()
if (MSVC)
    set(FOXHOLLOW_PATCHABLE_ENTRY_FLAGS /clang:-fpatchable-function-entry=${FOXHOLLOW_PATCH_ENTRY_NOPS})
else ()
    set(FOXHOLLOW_PATCHABLE_ENTRY_FLAGS -fpatchable-function-entry=${FOXHOLLOW_PATCH_ENTRY_NOPS})
endif ()

function(foxhollow_link_whole_archive target archive)
    if (APPLE)
        target_link_options(${target} PRIVATE -Wl,-force_load,$<TARGET_FILE:${archive}>)
    elseif (MSVC)
        target_link_options(${target} PRIVATE /WHOLEARCHIVE:$<TARGET_FILE:${archive}> /FORCE:MULTIPLE)
    else ()
        target_link_options(${target} PRIVATE
                -Wl,--whole-archive $<TARGET_FILE:${archive}> -Wl,--no-whole-archive)
    endif ()
endfunction()

set(AURORA_ENABLE_DVD ON CACHE BOOL "" FORCE)
add_subdirectory(extern/aurora EXCLUDE_FROM_ALL)
find_package(nlohmann_json 3 CONFIG QUIET)
add_subdirectory(extern/borealis EXCLUDE_FROM_ALL)
set(MUSYX_ENABLE_SDL OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/musyx EXCLUDE_FROM_ALL)

find_package(ZLIB REQUIRED)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS port/compiling.txt)
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/port/compiling.txt FOXHOLLOW_GAME_SOURCES)
list(FILTER FOXHOLLOW_GAME_SOURCES EXCLUDE REGEX "^ *(#|$)")
if (FOXHOLLOW_GAME_SOURCES)
    list(TRANSFORM FOXHOLLOW_GAME_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/game/src/)
    add_library(game STATIC ${FOXHOLLOW_GAME_SOURCES})
    target_include_directories(game PRIVATE port/include extern/aurora/include game/include game)
    target_link_libraries(game PRIVATE ZLIB::ZLIB aurora::core musyx)
    target_compile_definitions(game PRIVATE BUILD_VERSION=0 VERSION_GSAE01 NDEBUG=1)
    target_compile_options(game PRIVATE
            ${FOXHOLLOW_C_DIALECT_FLAGS} ${FOXHOLLOW_NO_WARN_FLAGS} ${FOXHOLLOW_FORCE_INCLUDE_FLAGS}
            ${FOXHOLLOW_PATCHABLE_ENTRY_FLAGS})
endif ()

add_executable(foxhollow src/main.c)
target_include_directories(foxhollow PRIVATE port/include)
target_link_libraries(foxhollow PRIVATE
        ZLIB::ZLIB
        port_shims
        musyx
        aurora::core aurora::gx aurora::main aurora::vi
        aurora::os aurora::pad aurora::dvd aurora::card
        aurora::mtx aurora::si aurora::gd aurora::ms aurora::thp)
if (TARGET game)
    foxhollow_link_whole_archive(foxhollow game)
    add_dependencies(foxhollow game)
    set_target_properties(foxhollow PROPERTIES LINK_DEPENDS $<TARGET_FILE:game>)
endif ()

set_target_properties(foxhollow PROPERTIES ENABLE_EXPORTS ON)

if (APPLE)
    add_custom_command(TARGET foxhollow POST_BUILD
            COMMAND codesign -s - --entitlements
            ${CMAKE_CURRENT_SOURCE_DIR}/debug.entitlements -f $<TARGET_FILE:foxhollow>
            COMMENT "Re-signing with get-task-allow so lldb can attach")
endif ()

include(${CMAKE_CURRENT_SOURCE_DIR}/extern/aurora/cmake/AuroraCopyRuntimeDLLs.cmake)
aurora_copy_runtime_dlls(foxhollow)

file(GLOB FOXHOLLOW_SHIM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/port/src/*.c)
list(APPEND FOXHOLLOW_SHIM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/port/src/crash_handler.cpp)
list(APPEND FOXHOLLOW_SHIM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/port/src/foxhollow_mods.cpp)
list(APPEND FOXHOLLOW_SHIM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/port/src/foxhollow_hook.cpp)
list(APPEND FOXHOLLOW_SHIM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/port/src/foxhollow_controls.cpp)
list(APPEND FOXHOLLOW_SHIM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/port/src/foxhollow_splash.cpp)
list(APPEND FOXHOLLOW_SHIM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/port/src/net/session.cpp)
if (WIN32)
    list(APPEND FOXHOLLOW_SHIM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/port/src/net/websocket_winhttp.cpp)
elseif (APPLE)
    enable_language(OBJCXX)
    list(APPEND FOXHOLLOW_SHIM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/port/src/net/websocket_url_session.mm)
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/port/src/net/websocket_url_session.mm
            PROPERTIES COMPILE_FLAGS -fobjc-arc)
else ()
    find_package(CURL 7.86 REQUIRED)
    list(APPEND FOXHOLLOW_SHIM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/port/src/net/websocket_curl.cpp)
endif ()

include(FetchContent)
FetchContent_GetProperties(imgui)
set(FOXHOLLOW_SPLASH_FONT ${imgui_SOURCE_DIR}/misc/fonts/Roboto-Medium.ttf)
if (NOT EXISTS ${FOXHOLLOW_SPLASH_FONT})
    message(FATAL_ERROR "Splash font not found at ${FOXHOLLOW_SPLASH_FONT}")
endif ()
set(FOXHOLLOW_SPLASH_FONT_SOURCE ${CMAKE_BINARY_DIR}/generated/foxhollow_splash_font.c)
file(READ ${FOXHOLLOW_SPLASH_FONT} FOXHOLLOW_SPLASH_FONT_HEX HEX)
file(SIZE ${FOXHOLLOW_SPLASH_FONT} FOXHOLLOW_SPLASH_FONT_SIZE)
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," FOXHOLLOW_SPLASH_FONT_BYTES ${FOXHOLLOW_SPLASH_FONT_HEX})
file(WRITE ${FOXHOLLOW_SPLASH_FONT_SOURCE}.tmp
        "#include <stddef.h>\nconst unsigned char gFoxhollowSplashFont[] = {${FOXHOLLOW_SPLASH_FONT_BYTES}};\nconst size_t gFoxhollowSplashFontSize = ${FOXHOLLOW_SPLASH_FONT_SIZE};\n")
configure_file(${FOXHOLLOW_SPLASH_FONT_SOURCE}.tmp ${FOXHOLLOW_SPLASH_FONT_SOURCE} COPYONLY)
list(APPEND FOXHOLLOW_SHIM_SOURCES ${FOXHOLLOW_SPLASH_FONT_SOURCE})
if (FOXHOLLOW_DEBUG_SHORTCUTS)
    list(APPEND FOXHOLLOW_SHIM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/port/src/debug_overlay.cpp)
    list(APPEND FOXHOLLOW_SHIM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/port/src/foxhollow_gallery_ui.cpp)
endif ()
add_library(port_shims STATIC ${FOXHOLLOW_SHIM_SOURCES})
if (NOT FOXHOLLOW_VERSION)
    set(FOXHOLLOW_VERSION ${PROJECT_VERSION})
endif ()
target_compile_definitions(port_shims PRIVATE FOXHOLLOW_VERSION="${FOXHOLLOW_VERSION}")
target_include_directories(port_shims PRIVATE port/src port/include extern/aurora/include game/include game)
if (FOXHOLLOW_DEBUG_SHORTCUTS)
    target_compile_definitions(port_shims PRIVATE FOXHOLLOW_DEBUG_SHORTCUTS=1)
    if (FOXHOLLOW_DEBUG_INPUT_TIMING)
        target_compile_definitions(port_shims PRIVATE FOXHOLLOW_DEBUG_INPUT_TIMING=1)
    endif ()
endif ()
target_compile_options(port_shims PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${FOXHOLLOW_C_DIALECT_FLAGS}>
        $<$<COMPILE_LANGUAGE:CXX>:${FOXHOLLOW_CXX_DIALECT_FLAGS}>)
target_link_libraries(port_shims PRIVATE aurora::core borealis::crash nlohmann_json::nlohmann_json musyx)
if (WIN32)
    target_link_libraries(port_shims PRIVATE winhttp)
elseif (APPLE)
    find_library(FOXHOLLOW_FOUNDATION_FRAMEWORK Foundation REQUIRED)
    target_link_libraries(port_shims PRIVATE ${FOXHOLLOW_FOUNDATION_FRAMEWORK})
else ()
    target_link_libraries(port_shims PRIVATE CURL::libcurl)
endif ()

if (WIN32 AND TARGET game)
    find_program(FOXHOLLOW_LLVM_NM llvm-nm REQUIRED)
    set(FOXHOLLOW_EXPORTS_DEF ${CMAKE_BINARY_DIR}/foxhollow_exports.def)
    set(FOXHOLLOW_EXPORT_TARGETS game port_shims musyx
            aurora::gx aurora::os aurora::pad aurora::mtx aurora::vi
            aurora::si aurora::dvd aurora::card aurora::gd aurora::ms aurora::thp)
    set(FOXHOLLOW_EXPORT_LIB_FILES "")
    foreach (FOXHOLLOW_EXPORT_TARGET IN LISTS FOXHOLLOW_EXPORT_TARGETS)
        if (TARGET ${FOXHOLLOW_EXPORT_TARGET})
            list(APPEND FOXHOLLOW_EXPORT_LIB_FILES $<TARGET_FILE:${FOXHOLLOW_EXPORT_TARGET}>)
        endif ()
    endforeach ()
    list(JOIN FOXHOLLOW_EXPORT_LIB_FILES "," FOXHOLLOW_EXPORT_LIBS)
    add_custom_command(TARGET foxhollow PRE_LINK
            COMMAND ${CMAKE_COMMAND} -DNM=${FOXHOLLOW_LLVM_NM} "-DLIBS=${FOXHOLLOW_EXPORT_LIBS}"
                    -DOUT=${FOXHOLLOW_EXPORTS_DEF}
                    -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateWindowsExports.cmake
            COMMENT "Generating the game export list for mods"
            VERBATIM)
    target_link_options(foxhollow PRIVATE /DEF:${FOXHOLLOW_EXPORTS_DEF})
endif ()

if (TARGET game)
    add_executable(linkcheck EXCLUDE_FROM_ALL src/main.c)
    target_link_libraries(linkcheck PRIVATE
            ZLIB::ZLIB
            port_shims
            musyx
            aurora::core aurora::gx aurora::main aurora::vi
            aurora::os aurora::pad aurora::dvd aurora::card
            aurora::mtx aurora::si aurora::gd aurora::ms aurora::thp)
    foxhollow_link_whole_archive(linkcheck game)
    add_dependencies(linkcheck game)
    set_target_properties(linkcheck PROPERTIES LINK_DEPENDS $<TARGET_FILE:game>)
endif ()
