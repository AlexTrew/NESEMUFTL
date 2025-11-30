cmake_minimum_required(VERSION 3.25.1)

set(CMAKE_BUILD_TYPE Debug)

project(nesemuftl)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)

# Show all warning messages
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

file(GLOB SOURCES
  main.c
     )

add_library(cpu cpu_state.h cpu.h cpu.c cpu_addr_mode.h assembler.h shared.h shared.c assembler.c cpu_addr_mode.c instructions.c instructions.h)
add_executable(nesemuftl ${SOURCES})

set_target_properties(nesemuftl PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(nesemuftl cpu ${GLIB2_LIBRARIES})
target_include_directories(nesemuftl PUBLIC ${GLIB2_INCLUDE_DIRS})
target_compile_options(nesemuftl PUBLIC {GLIB2_CFLAGS_OTHER})

enable_testing()
add_subdirectory(tests)
