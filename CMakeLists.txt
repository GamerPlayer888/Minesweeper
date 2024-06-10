cmake_minimum_required(VERSION 3.7.2)
project(minesweeper)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

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