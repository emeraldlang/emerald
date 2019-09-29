# emerald cmake file
cmake_minimum_required(VERSION 3.6.0 FATAL_ERROR)

project(EMERALD VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(EMERALD_BUILD_TESTS "enable testing" ON)
option(EMERALD_BUILD_EXECUTABLE "build executable" ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(emerald_s SHARED
    src/ast_printer.cpp
    src/code.cpp
    src/code_cache.cpp
    src/compiler.cpp
    src/data_stack.cpp
    src/heap.cpp
    src/heap_managed.cpp
    src/module.cpp
    src/modules/core.cpp
    src/modules/init.cpp
    src/modules/io.cpp
    src/natives/array.cpp
    src/natives/boolean.cpp
    src/natives/number.cpp
    src/natives/object.cpp
    src/natives/string.cpp
    src/native_prototypes.cpp
    src/object.cpp
    src/opcode.cpp
    src/parser.cpp
    src/process.cpp
    src/reporter.cpp
    src/scanner.cpp
    src/scheduler.cpp
    src/source.cpp
    src/stack.cpp
    src/token.cpp
    src/vm.cpp)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(fmt REQUIRED)
find_package(Boost COMPONENTS serialization REQUIRED)

target_link_libraries(emerald_s
    pthread
    stdc++fs
    fmt::fmt
    ${Boost_LIBRARIES})

if (EMERALD_BUILD_EXECUTABLE)
    add_executable(emerald
        src/main.cpp)

    target_link_libraries(emerald
        PRIVATE emerald_s)
endif()

if (EMERALD_BUILD_TESTS)
    enable_testing()
endif()
