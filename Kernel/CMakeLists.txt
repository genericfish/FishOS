add_compile_options(-Os)

set(KERNEL_SOURCES
    Kernel.cpp
    StdLib.cpp
    TTY/TTY.cpp
)

set(KERNEL_ASM
    Arch/${FISH_ARCH}/boot.S
)

set(KERNEL_LD
    Arch/${FISH_ARCH}/linker.ld
)

set(KERNEL_CXX_FLAGS
    -ffreestanding
    -fno-exceptions
    -fno-rtti
    -nostdlib
    -nostdinc
    -nostdinc++
)

add_library(Kernel ${KERNEL_SOURCES})
add_dependencies(Kernel LibK)

add_custom_command(
    TARGET Kernel
    PRE_BUILD
    COMMAND
        ${CMAKE_ASM_COMPILER} ${CMAKE_CURRENT_SOURCE_DIR}/${KERNEL_ASM} -o ${CMAKE_BINARY_DIR}/Boot/boot.o
)

target_compile_options(Kernel PUBLIC ${KERNEL_CXX_FLAGS})
target_include_directories(Kernel
    PRIVATE .
    PRIVATE ../
    PRIVATE ../Libraries/LibC
)
target_link_libraries(Kernel "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/LibK.a")
set_target_properties(Kernel PROPERTIES
    PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Boot
)

add_custom_command(
    TARGET Kernel
    POST_BUILD
    COMMAND ${CMAKE_CXX_COMPILER} ${KERNEL_CXX_FLAGS} -T ${CMAKE_CURRENT_SOURCE_DIR}/${KERNEL_LD} -o ${CMAKE_BINARY_DIR}/Boot/fish.kernel ${CMAKE_BINARY_DIR}/Boot/boot.o ${CMAKE_BINARY_DIR}/Boot/Kernel.a ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/LibK.a
)
