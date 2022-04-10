cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(watchdog LANGUAGES C CXX)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

add_executable(watchdog watchdog.c)

option(BUILD_EXAMPLE "build example" OFF)
if (BUILD_EXAMPLE)
    add_executable(example example.c)

    find_package(Threads REQUIRED)
    add_executable(example_mt example_mt.cpp)
    target_link_libraries(example_mt PRIVATE Threads::Threads)
endif()

install(TARGETS watchdog DESTINATION bin)
