







file(RELATIVE_PATH SUBDIRECTORY_RELATIVE_DIR "${PROJECT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Processing ${SUBDIRECTORY_RELATIVE_DIR}" )
string(REPLACE "/" "-" SUBMODULE_NAME "ept-${SUBDIRECTORY_RELATIVE_DIR}")
string(REGEX REPLACE "[^A-Za-z0-9\-]" "_" "SUBMODULE_NAME" "${SUBMODULE_NAME}")
message(STATUS "Submodule ${SUBMODULE_NAME}" )

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS *.cpp *.c *.h *.hpp)
add_library(
    ${SUBMODULE_NAME}
    STATIC
    ${SRCS}
)


target_link_libraries(
    ${SUBMODULE_NAME}
    PRIVATE
    Qt6::Core
    Qt6::Widgets
)


file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/.." SUBDIRECTORY_PARENT)

target_include_directories(
    ${SUBMODULE_NAME}
    INTERFACE
    "${SUBDIRECTORY_PARENT}"
    PRIVATE
    "${SUBDIRECTORY_PARENT}"
)


# file(GLOB SUBDIR_LIST LIST_DIRECTORIES true RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")
# foreach(dir ${SUBDIR_LIST})
#     if(IS_DIRECTORY ${dir})
#         add_subdirectory(${dir})
#         target_include_directories(${SUBMODULE_NAME}-${dir} PRIVATE ${SUBDIRECTORY_PARENT})
#         target_link_libraries(${SUBMODULE_NAME} PRIVATE ${SUBMODULE_NAME}-${dir})
#     endif()
# endforeach()

file(GLOB SUBDIR_LIST LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR} "*")
get_filename_component(CURDIR_ABS ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)
foreach(dir ${SUBDIR_LIST})
    get_filename_component(dir_abs ${dir} ABSOLUTE)
    if(IS_DIRECTORY ${dir} AND NOT ${dir_abs} STREQUAL ${CURDIR_ABS})
        get_filename_component(reldir ${dir} NAME)
        add_subdirectory(${dir})
        if(TARGET ${SUBMODULE_NAME}-${reldir})
            message(STATUS "common-core: subdir ${reldir}, target ${SUBMODULE_NAME}-${reldir}")
            target_include_directories(${SUBMODULE_NAME}-${reldir} PRIVATE ${SUBDIRECTORY_PARENT})
            target_link_libraries(${SUBMODULE_NAME} PRIVATE ${SUBMODULE_NAME}-${reldir})
        endif()
    endif()
endforeach()
