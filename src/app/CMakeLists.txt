



file(GLOB SRCS CONFIGURE_DEPENDS *.cpp *.c *.h *.hpp)

add_executable(
    entropy-piano-tuner
    ${SRCS}
)




target_link_libraries(
    entropy-piano-tuner
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    Qt6::Multimedia

    fmt::fmt
    libuv::libuv
    fftw::fftw

    ept-common
)

# file(GLOB SUBDIR_LIST LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR} "*")
# foreach(dir ${SUBDIR_LIST})
#     get_filename_component(reldir ${dir} NAME)
#     if(IS_DIRECTORY ${dir} AND TARGET "ept-app-${reldir}")
#         message(STATUS "Subdir ${dir}, lib ept-app-${reldir}")
#         add_subdirectory(${reldir})
#         target_include_directories(ept-app-${reldir} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#         target_link_libraries(entropy-piano-tuner PRIVATE ept-app-${reldir})
#     endif()
# endforeach()

file(GLOB SUBDIR_LIST LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR} "*")
get_filename_component(CURDIR_ABS ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)
foreach(dir ${SUBDIR_LIST})
    get_filename_component(dir_abs ${dir} ABSOLUTE)
    if(IS_DIRECTORY ${dir} AND NOT ${dir_abs} STREQUAL ${CURDIR_ABS})
        get_filename_component(reldir ${dir} NAME)
        add_subdirectory(${dir})
        if(TARGET ept-app-${reldir})
            message(STATUS "ept-app: subdir ${reldir}, target ept-app-${reldir}")
            target_include_directories(ept-app-${reldir} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
            target_link_libraries(entropy-piano-tuner PRIVATE ept-app-${reldir})
        endif()
    endif()
endforeach()


target_include_directories(entropy-piano-tuner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})