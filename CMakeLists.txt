cmake_minimum_required(VERSION 3.25.1)

set(CMAKE_BUILD_TYPE Debug)

project(nesemuftl)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Show all warning messages
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

file(GLOB SOURCES
  main.c
     )

add_library(cpu cpu.h cpu.c)
add_executable(nesemuftl ${SOURCES})

set_target_properties(nesemuftl PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(nesemuftl cpu)

enable_testing()
add_subdirectory(tests)
