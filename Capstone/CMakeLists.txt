cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-std=c++17)

project(CppND-Capstone)

find_package(SDL2 REQUIRED)
include_directories(include ${SDL2_INCLUDE_DIRS})

file(GLOB project_SRCS src/*.cpp)

add_executable(capstone ${project_SRCS})
target_link_libraries(capstone ${SDL2_LIBRARIES} SDL2_image)