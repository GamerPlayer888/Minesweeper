cmake_minimum_required(VERSION 3.7.2)
project(minesweeper)

if (APPLE)
    message(STATUS "Configuring for macOS - Removing -static-libgcc and -static-libstdc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
else()
    message(STATUS "Configuring for non-macOS - Keeping -static-libgcc and -static-libstdc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FETCHCONTENT_DECLARE(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.1)
FETCHCONTENT_MAKEAVAILABLE(SFML)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC src/*.cpp)
file(GLOB HEADER src/*.h)

add_executable(minesweeper ${SRC})
include_directories(minesweeper ${HEADER})

target_link_libraries(minesweeper sfml-graphics)
