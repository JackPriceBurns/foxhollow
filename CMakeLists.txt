cmake_minimum_required(VERSION 3.25)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
            "Build type options: Debug Release RelWithDebInfo MinSizeRel" FORCE)
endif ()

project(foxhollow LANGUAGES C CXX VERSION 0.0.1)

option(FOXHOLLOW_DEBUG_SHORTCUTS "Enable Foxhollow development shortcuts" ON)

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
    set(FOXHOLLOW_NO_WARN_FLAGS -w)
    set(FOXHOLLOW_FORCE_INCLUDE_FLAGS -include ${CMAKE_CURRENT_SOURCE_DIR}/port/include/foxhollow_compat.h)
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
add_subdirectory(extern/borealis EXCLUDE_FROM_ALL)

find_package(ZLIB REQUIRED)
find_package(JPEG REQUIRED)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS port/compiling.txt)
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/port/compiling.txt FOXHOLLOW_GAME_SOURCES)
list(FILTER FOXHOLLOW_GAME_SOURCES EXCLUDE REGEX "^ *(#|$)")
if (FOXHOLLOW_GAME_SOURCES)
    list(TRANSFORM FOXHOLLOW_GAME_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/game/src/)
    add_library(game STATIC ${FOXHOLLOW_GAME_SOURCES})
    target_include_directories(game PRIVATE port/include extern/aurora/include game/include game)
    target_link_libraries(game PRIVATE ZLIB::ZLIB)
    target_compile_definitions(game PRIVATE BUILD_VERSION=0 VERSION_GSAE01 NDEBUG=1 TARGET_PC=1)
    target_compile_options(game PRIVATE
            ${FOXHOLLOW_C_DIALECT_FLAGS} ${FOXHOLLOW_NO_WARN_FLAGS} ${FOXHOLLOW_FORCE_INCLUDE_FLAGS})
endif ()

add_executable(foxhollow src/main.c)
target_include_directories(foxhollow PRIVATE port/include)
target_link_libraries(foxhollow PRIVATE
        ZLIB::ZLIB
        port_shims
        aurora::core aurora::gx aurora::main aurora::vi
        aurora::os aurora::pad aurora::dvd aurora::card
        aurora::mtx aurora::si aurora::gd aurora::ms)
if (TARGET game)
    foxhollow_link_whole_archive(foxhollow game)
    add_dependencies(foxhollow game)
    set_target_properties(foxhollow PROPERTIES LINK_DEPENDS $<TARGET_FILE:game>)
endif ()

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
if (FOXHOLLOW_DEBUG_SHORTCUTS)
    list(APPEND FOXHOLLOW_SHIM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/port/src/debug_overlay.cpp)
endif ()
add_library(port_shims STATIC ${FOXHOLLOW_SHIM_SOURCES})
target_include_directories(port_shims PRIVATE port/src port/include extern/aurora/include game/include game)
target_compile_definitions(port_shims PRIVATE TARGET_PC=1)
if (FOXHOLLOW_DEBUG_SHORTCUTS)
    target_compile_definitions(port_shims PRIVATE FOXHOLLOW_DEBUG_SHORTCUTS=1)
endif ()
target_compile_options(port_shims PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${FOXHOLLOW_C_DIALECT_FLAGS}>
        $<$<COMPILE_LANGUAGE:CXX>:${FOXHOLLOW_CXX_DIALECT_FLAGS}>)
target_link_libraries(port_shims PRIVATE aurora::core JPEG::JPEG borealis::crash)


if (TARGET game)
    add_executable(linkcheck EXCLUDE_FROM_ALL src/main.c)
    target_link_libraries(linkcheck PRIVATE
            ZLIB::ZLIB
            port_shims
            aurora::core aurora::gx aurora::main aurora::vi
            aurora::os aurora::pad aurora::dvd aurora::card
            aurora::mtx aurora::si aurora::gd aurora::ms)
    foxhollow_link_whole_archive(linkcheck game)
    add_dependencies(linkcheck game)
    set_target_properties(linkcheck PROPERTIES LINK_DEPENDS $<TARGET_FILE:game>)
endif ()
