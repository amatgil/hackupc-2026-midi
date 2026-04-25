cmake_minimum_required(VERSION 3.28)
project(HackUPC-2026-midi)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB SRC ./src/*.cc)

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC ./src/*.cc)


# Try system-installed raylib first
find_package(raylib QUIET)

# If not found, fetch and build it
if (NOT raylib_FOUND)
  include(FetchContent)

  FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
  )


  FetchContent_MakeAvailable(raylib)
endif()
FetchContent_Declare(
fftw
URL https://www.fftw.org/fftw-3.3.11.tar.gz
)
FetchContent_MakeAvailable(fftw)

add_executable(${CMAKE_PROJECT_NAME} ${SRC})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC src ${fftw3_SOURCE_DIR}/api)
target_link_libraries(${CMAKE_PROJECT_NAME} "raylib;fftw3")

file(CREATE_LINK ${CMAKE_SOURCE_DIR}/src/shaders ${CMAKE_BINARY_DIR}/shaders SYMBOLIC)
file(CREATE_LINK ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets SYMBOLIC)
