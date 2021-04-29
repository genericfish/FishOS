set(LIBC_SOURCES
    stdio.cpp
    stdlib.cpp
    string.cpp
)

add_library(LibK ${LIBC_SOURCES})
target_compile_options(LibK PUBLIC
    -ffreestanding
    -fno-exceptions
    -fno-rtti
    -nostdlib
    -nostdinc
    -nostdinc++
    -D__LIBK
)
target_include_directories(LibK
    PRIVATE .
    PRIVATE ../../
)
set_target_properties(LibK PROPERTIES PREFIX "")
