cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 20)

project(Exception)

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)

if (${FORCE_COLORED_OUTPUT})
  if     ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options (-fdiagnostics-color=always)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options (-fcolor-diagnostics)
  endif ()
endif ()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -lfmt -mavx2 -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,leak,undefined -Wall -Wextra -Wpedantic -g -O0 -lfmt")

set(MAIN main.cpp)

add_executable(a.out ${MAIN})
