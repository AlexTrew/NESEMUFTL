cmake_minimum_required(VERSION 3.25.1)

set(CMAKE_BUILD_TYPE Debug)

project(ya-nes-emu)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(SDL3_DIR ./SDL)
find_package(SDL3 REQUIRED)


file(GLOB SOURCES
  cpu.h
  ppu.h
  cpu.c
  ppu.c
  main.c
     )

add_executable(ya-nes-emu ${SOURCES})

set_target_properties(ya-nes-emu PROPERTIES LINKER_LANGUAGE C)

INCLUDE_DIRECTORIES(${SDL3_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL3_LIBRARIES})
