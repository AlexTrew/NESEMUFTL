cmake_minimum_required(VERSION 3.25.1)

set(CMAKE_BUILD_TYPE Debug)

project(ya-nes-emu)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Show all warning messages
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

file(GLOB SOURCES
  main.c
     )

add_library(cpu cpu.h cpu.c)
add_executable(ya-nes-emu ${SOURCES})

set_target_properties(ya-nes-emu PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(ya-nes-emu cpu)

enable_testing()
add_subdirectory(tests)
