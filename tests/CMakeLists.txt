

# Find Check using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(CHECK REQUIRED check)

include(CheckCSourceCompiles)
include(CheckCSourceRuns)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)

add_executable(test_cpu test_cpu.c)
target_link_libraries(test_cpu cpu ${CHECK_LIBRARIES} pthread)

