cmake_minimum_required(VERSION 3.28)
project(HackUPC-2026-midi)

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC ./src/*.cc)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        6.0 # release-1.10.0
)

FetchContent_MakeAvailable(raylib)

add_executable(${CMAKE_PROJECT_NAME} ${SRC})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC src)

target_link_libraries(${CMAKE_PROJECT_NAME} raylib)
