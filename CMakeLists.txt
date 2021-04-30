cmake_minimum_required(VERSION 3.20)

project(FishOS ASM CXX C)

# FIXME: Do not hardcode the compiler path
set(CMAKE_C_COMPILER "${PROJECT_SOURCE_DIR}/Toolchain/Local/bin/i686-elf-gcc")
set(CMAKE_CXX_COMPILER "${PROJECT_SOURCE_DIR}/Toolchain/Local/bin/i686-elf-g++")
set(CMAKE_ASM_COMPILER "${PROJECT_SOURCE_DIR}/Toolchain/Local/bin/i686-elf-as")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# FIXME: Wait until g++-11 to get GNU++23
#   set(CMAKE_CXX_STANDARD 23)
#   set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS -std=gnu++23)

set(CMAKE_INSTALL_PREFIX ./Root)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX}/usr/lib)
set(CMAKE_CROSSCOMPILING True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(FISH_ARCH i686)

#add_subdirectory(FSH)
add_subdirectory(Kernel)
add_subdirectory(Libraries)

add_custom_target(run
    COMMAND qemu-system-i386 -kernel ${CMAKE_BINARY_DIR}/Boot/fish.kernel
)

add_dependencies(run Kernel)
