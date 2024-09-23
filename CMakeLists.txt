cmake_minimum_required(VERSION 3.29)
project(cproj)
include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
fetchcontent_declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.5.1)
fetchcontent_makeavailable(SFML)


set(CMAKE_CXX_STANDARD 14)

add_executable(cproj main.cpp)

target_link_libraries(cproj sfml-graphics)