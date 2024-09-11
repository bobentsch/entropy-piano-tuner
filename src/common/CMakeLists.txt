


add_subdirectory(algorithms)
add_subdirectory(core)
add_subdirectory(config)


add_library(
    ept-common
    STATIC
)


target_link_libraries(
    ept-common
    PRIVATE
    ept-common-core
    ept-common-algorithms
    ept-common-config
)

target_include_directories(
    ept-common
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)