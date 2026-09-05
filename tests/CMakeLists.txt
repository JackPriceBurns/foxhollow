cmake_minimum_required(VERSION 3.25)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(foxhollow_tests LANGUAGES C)
    enable_testing()
endif ()

if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "The native pointer regression tests require a 64-bit build")
endif ()

get_filename_component(FOXHOLLOW_TEST_SOURCE_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

add_custom_target(foxhollow_tests)

function(foxhollow_add_regression name source)
    set(target foxhollow_test_${name})
    add_executable(${target} ${source})
    set_target_properties(${target} PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON C_EXTENSIONS ON)
    target_include_directories(${target} PRIVATE
            ${FOXHOLLOW_TEST_SOURCE_ROOT}/port/include
            ${FOXHOLLOW_TEST_SOURCE_ROOT}/extern/aurora/include
            ${FOXHOLLOW_TEST_SOURCE_ROOT}/game/include
            ${FOXHOLLOW_TEST_SOURCE_ROOT}/game)
    target_compile_definitions(${target} PRIVATE TARGET_PC=1 VERSION_GSAE01 BUILD_VERSION=0 NDEBUG=1)
    if (MSVC)
        target_compile_options(${target} PRIVATE
                /clang:-std=gnu11 /clang:-fsigned-char /clang:-fcommon /Gy /Gw
                /FI${FOXHOLLOW_TEST_SOURCE_ROOT}/port/include/foxhollow_compat.h
                -Wno-error=incompatible-pointer-types)
        target_link_options(${target} PRIVATE /OPT:REF)
    else ()
        target_compile_options(${target} PRIVATE
                -fsigned-char -fcommon -ffunction-sections -fdata-sections
                -include ${FOXHOLLOW_TEST_SOURCE_ROOT}/port/include/foxhollow_compat.h
                -Wno-error=incompatible-pointer-types)
        if (APPLE)
            target_link_options(${target} PRIVATE -Wl,-dead_strip)
        else ()
            target_link_options(${target} PRIVATE -Wl,--gc-sections)
            target_link_libraries(${target} PRIVATE m)
        endif ()
    endif ()
    add_dependencies(foxhollow_tests ${target})
    add_test(NAME foxhollow.${name} COMMAND ${target})
    set_tests_properties(foxhollow.${name} PROPERTIES TIMEOUT 10 LABELS "game;regression;${ARGN}")
endfunction()

foxhollow_add_regression(pressure_switch pressure_switch_test.c object pointer)
foxhollow_add_regression(grimble_path grimble_path_test.c object pointer)
foxhollow_add_regression(sequence_messages sequence_messages_test.c object pointer)
foxhollow_add_regression(effect_cleanup effect_cleanup_test.c object pointer)
foxhollow_add_regression(deferred_messages deferred_messages_test.c object pointer)
foxhollow_add_regression(enemy_path_cleanup enemy_path_cleanup_test.c pointer)
foxhollow_add_regression(thornbush_hits thornbush_hits_test.c object pointer)
foxhollow_add_regression(retail_pressure_reset retail_pressure_reset_test.c object retail quirk)
foxhollow_add_regression(retail_animation_fallback retail_animation_fallback_test.c retail quirk)
foxhollow_add_regression(sidekick_ball sidekick_ball_test.c object pointer)
foxhollow_add_regression(log_fire log_fire_test.c object pointer)
foxhollow_add_regression(drakor_missile drakor_missile_test.c object)
foxhollow_add_regression(rolling_barrel rolling_barrel_test.c object pointer)
foxhollow_add_regression(falling_ladder falling_ladder_test.c object endian)
foxhollow_add_regression(moving_platform moving_platform_test.c object pointer endian)
foxhollow_add_regression(dust_mote_source dust_mote_source_test.c object endian)
foxhollow_add_regression(firefly_wander firefly_wander_test.c object)
foxhollow_add_regression(invisible_hit invisible_hit_test.c object pointer)
foxhollow_add_regression(ice_ball ice_ball_test.c object pointer)
