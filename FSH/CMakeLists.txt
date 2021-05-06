file(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")
foreach(header ${headers})
    get_filename_component(subdirectory ${header} DIRECTORY)
    install(
        FILES ${header}
        DESTINATION ${CMAKE_BINARY_DIR}/Root/usr/include/FSH/${subdirectory}
    )
endforeach()

file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h" "*.cpp")
foreach(source ${sources})
    get_filename_component(subdirectory ${source} DIRECTORY)
    install(
        FILES ${source}
        DESTINATION ${CMAKE_BINARY_DIR}/Root/usr/src/fish/${subdirectory}
    )
endforeach()
