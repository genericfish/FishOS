execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtbegin.o
    OUTPUT_VARIABLE CRTBEGIN
)

execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtend.o
    OUTPUT_VARIABLE CRTEND
)

string(
    REGEX REPLACE "\n$" ""
    CRTBEGIN "${CRTBEGIN}"
)

string(
    REGEX REPLACE "\n$" ""
    CRTEND "${CRTEND}"
)

set(LIBC_SOURCES
    stdio.cpp
    stdlib.cpp
    string.cpp
)

add_library(LibK ${LIBC_SOURCES})
target_compile_options(LibK PUBLIC
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
    -D__LIBK
    -lgcc
)
target_include_directories(LibK
    PRIVATE .
    PRIVATE ../../
)
target_link_libraries(LibK
    ${CRTBEGIN}
    ${CRTEND}
)

set_target_properties(LibK PROPERTIES PREFIX "")
