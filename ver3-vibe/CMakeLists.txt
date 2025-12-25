cmake_minimum_required(VERSION 3.13)
project(AppleIIEmulator_v3 CXX)

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

# Copy ROM files to the build directory
file(GLOB ROM_FILES *.rom)
foreach(ROM_FILE ${ROM_FILES})
    add_custom_command(
        TARGET apple_emulator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${ROM_FILE}
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

# Unit tests for CPU
add_executable(cpu_unit_tests Testing/cpu_test.cpp cpu.cpp memory.cpp)
target_link_libraries(cpu_unit_tests PRIVATE GTest::gtest_main)
add_test(NAME CPUTests COMMAND cpu_unit_tests)
