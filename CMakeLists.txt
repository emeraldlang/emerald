# emerald cmake file
cmake_minimum_required(VERSION 3.6.0 FATAL_ERROR)

project(EMERALD VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(EMERALD_BUILD_TESTS "enable testing" ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(Boost COMPONENTS serialization REQUIRED)
find_package(CLI11 REQUIRED)
find_package(fmt REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${Boost_INCLUDE_DIRS})

add_library(emerald_s SHARED
    src/ast_printer.cpp
    src/code.cpp
    src/code_cache.cpp
    src/compiler.cpp
    src/heap.cpp
    src/heap_managed.cpp
    src/interpreter.cpp
    src/module.cpp
    src/module_registry.cpp
    src/modules/collections.cpp
    src/modules/core.cpp
    src/modules/gc.cpp
    src/modules/init.cpp
    src/modules/io.cpp
    src/modules/net.cpp
    src/natives/array.cpp
    src/natives/boolean.cpp
    src/natives/number.cpp
    src/natives/object.cpp
    src/natives/string.cpp
    src/native_frame.cpp
    src/native_objects.cpp
    src/object.cpp
    src/opcode.cpp
    src/parser.cpp
    src/reporter.cpp
    src/scanner.cpp
    src/source.cpp
    src/stack.cpp
    src/token.cpp)

target_link_libraries(emerald_s
    pthread
    stdc++fs
    CLI11::CLI11
    CONAN_PKG::fmt
    ${Boost_LIBRARIES})

add_executable(emerald
    src/main.cpp)

target_link_libraries(emerald
    PRIVATE emerald_s)

file(GLOB LIB_FILES ${PROJECT_SOURCE_DIR}/lib/*.em)
add_custom_target(emerald_lib
    ALL
    COMMAND emerald compile ${LIB_FILES} -o ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    DEPENDS ${LIB_FILES})

if (EMERALD_BUILD_TESTS)
    enable_testing()
endif()
