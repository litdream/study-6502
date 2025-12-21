cmake_minimum_required(VERSION 3.13)
project(AppleIIEmulator CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find and link SDL2 and SDL2_ttf using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

# Main application
add_executable(apple_emulator main.cpp memory.cpp cpu.cpp)
target_include_directories(apple_emulator PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(apple_emulator PRIVATE ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})

# Copy TTF files to the build directory
file(GLOB FONT_FILES *.ttf)
foreach(FONT_FILE ${FONT_FILES})
    add_custom_command(
        TARGET apple_emulator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${FONT_FILE}
        ${CMAKE_CURRENT_BINARY_DIR}
    )
endforeach()

# Google Test with FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Unit tests for Memory
add_executable(memory_unit_tests Testing/memory_test.cpp memory.cpp cpu.cpp)
target_link_libraries(memory_unit_tests PRIVATE GTest::gtest_main)
target_include_directories(memory_unit_tests PUBLIC .)
add_test(NAME MemoryTests COMMAND memory_unit_tests)

# Unit tests for CPU
add_executable(cpu_unit_tests Testing/cpu_test.cpp     Testing/test_lda.cpp
    Testing/test_ldx.cpp
    Testing/test_ldy.cpp
    Testing/test_sta.cpp
    Testing/test_stx.cpp
    Testing/test_sty.cpp
    Testing/test_transfers.cpp
    Testing/test_pha.cpp
    Testing/test_php.cpp
    Testing/test_pla.cpp
    Testing/test_plp.cpp
    Testing/test_and.cpp
    Testing/test_eor.cpp
    Testing/test_ora.cpp
    Testing/test_adc.cpp
    Testing/test_sbc.cpp
    Testing/test_cmp.cpp
    Testing/test_cpx_cpy.cpp
    Testing/test_dec.cpp
    Testing/test_dex_dey.cpp
    Testing/test_inc.cpp
    Testing/test_inx_iny.cpp
    Testing/test_asl.cpp
    Testing/test_lsr.cpp
    Testing/test_rol.cpp
    Testing/test_ror.cpp
    Testing/test_jmp.cpp
    Testing/test_jsr_rts.cpp
    Testing/test_branches.cpp
    Testing/test_flag_control.cpp
    Testing/test_system_functions.cpp cpu.cpp memory.cpp)
target_link_libraries(cpu_unit_tests PRIVATE GTest::gtest_main)
target_include_directories(cpu_unit_tests PUBLIC .)
add_test(NAME CPUTests COMMAND cpu_unit_tests)
