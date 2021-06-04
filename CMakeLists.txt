cmake_minimum_required(VERSION 3.20)

project(FishOS ASM CXX C)

set(CMAKE_C_COMPILER "${PROJECT_SOURCE_DIR}/Toolchain/Local/bin/x86_64-elf-gcc")
set(CMAKE_CXX_COMPILER "${PROJECT_SOURCE_DIR}/Toolchain/Local/bin/x86_64-elf-g++")
set(CMAKE_ASM_COMPILER "${PROJECT_SOURCE_DIR}/Toolchain/Local/bin/x86_64-elf-as")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INSTALL_PREFIX ./Root)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX}/usr/lib)
set(CMAKE_CROSSCOMPILING True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

set(FISH_ARCH x86_64)

add_subdirectory(FSH)
add_subdirectory(Kernel)
add_subdirectory(Libraries)

add_custom_target(run
    COMMAND qemu-system-x86_64 -kernel ${CMAKE_BINARY_DIR}/Root/boot/fish.kernel
)

add_custom_target(debug
    COMMAND qemu-system-x86_64 -kernel ${CMAKE_BINARY_DIR}/Root/boot/fish.kernel -d int,cpu_reset -D log.txt -no-reboot
)

add_dependencies(run Kernel)
add_dependencies(run debug)
