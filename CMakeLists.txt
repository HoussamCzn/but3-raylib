cmake_minimum_required(VERSION 3.14)

project(rayons)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES "source/*.cxx")

add_executable(main ${SOURCES})

target_include_directories(main PUBLIC include)
