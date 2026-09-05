cmake_minimum_required(VERSION 3.29)

project(itpld LANGUAGES C VERSION 0.1.0)

if(NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
  message(
    WARNING "itpld project stack is LLVM, detected: ${CMAKE_C_COMPILER_ID}.")
endif()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

option(ITPLD_BUILD_TESTS "Build unit tests for itpld" ON)

add_library(project_options INTERFACE)
target_compile_options(project_options INTERFACE -Wall -Wextra -Wpedantic
                                                 -Werror)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(project_options INTERFACE -fsanitize=address,undefined
                                                   -fno-omit-frame-pointer)
  target_link_options(project_options INTERFACE -fsanitize=address,undefined)
endif()

find_program(CLANG_TIDY_EXE NAMES clang-tidy REQUIRED)
if(CLANG_TIDY_EXE)
  set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

add_library(itpld_core STATIC src/itpld.c)

target_include_directories(
  itpld_core
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(itpld_core PRIVATE project_options)

add_executable(itpld src/main.c)

target_link_libraries(itpld PRIVATE itpld_core project_options)

if(ITPLD_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
