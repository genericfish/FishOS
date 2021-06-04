add_compile_options(-Os)

set(KERNEL_SOURCES
    Interrupts/IDT.cpp
    TTY/TTY.cpp
    Kernel.cpp
    StdLib.cpp
)
set(KERNEL_ASM ${CMAKE_CURRENT_SOURCE_DIR}/Arch/${FISH_ARCH}/boot.S)
set(KERNEL_LD ${CMAKE_CURRENT_SOURCE_DIR}/Arch/${FISH_ARCH}/linker.ld)

set(KERNEL_CXX_FLAGS
    -march=native
    -fno-PIC
    -pedantic
    -ffreestanding
    -fno-exceptions
    -fno-rtti
    -fuse-cxa-atexit
    -fno-threadsafe-statics
    -mcmodel=kernel
    -mno-red-zone
    -nostdlib
    -nostdinc
    -nostdinc++
    -z max-page-size=0x1000
)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/Boot/boot.o
    COMMAND
        ${CMAKE_ASM_COMPILER} ${KERNEL_ASM}
        -o ${CMAKE_BINARY_DIR}/Boot/boot.o
    DEPENDS
        ${KERNEL_ASM}
)

add_custom_target(boot.o
    ALL
    DEPENDS
        ${CMAKE_BINARY_DIR}/Boot/boot.o
)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/Root/boot/fish.kernel
    COMMAND
        ${CMAKE_CXX_COMPILER} ${KERNEL_CXX_FLAGS}
            -T ${KERNEL_LD}
            -o ${CMAKE_BINARY_DIR}/Root/boot/fish.kernel
            ${CMAKE_BINARY_DIR}/Boot/boot.o
            ${CMAKE_BINARY_DIR}/Boot/KernelObj.a
            ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/LibK.a
    DEPENDS
        KernelObj
        LibK
        boot.o
        ${KERNEL_LD}
        ${KERNEL_ASM}
)

add_custom_target(Kernel
    ALL
    DEPENDS
        ${CMAKE_BINARY_DIR}/Root/boot/fish.kernel
)

add_library(KernelObj ${KERNEL_SOURCES})
add_dependencies(KernelObj LibK boot.o)

target_compile_options(KernelObj PUBLIC ${KERNEL_CXX_FLAGS})
target_include_directories(KernelObj
    PRIVATE .
    PRIVATE ../
    PRIVATE ../Libraries/LibC
)
target_link_libraries(KernelObj "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/LibK.a")
set_target_properties(KernelObj PROPERTIES
    PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Boot
)
