file(RELATIVE_PATH SUBDIRECTORY_RELATIVE_DIR "${PROJECT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Processing ${SUBDIRECTORY_RELATIVE_DIR}" )
string(REPLACE "/" "-" SUBMODULE_NAME "ept-${SUBDIRECTORY_RELATIVE_DIR}")
string(REGEX REPLACE "[^A-Za-z0-9\-]" "_" "SUBMODULE_NAME" "${SUBMODULE_NAME}")
message(STATUS "Submodule ${SUBMODULE_NAME}" )

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS *.cpp *.c *.h *.hpp)
add_library(
    ${SUBMODULE_NAME}
    OBJECT
    ${SRCS}
)

target_link_libraries(${SUBMODULE_NAME} PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Multimedia ept-common ept-app-implementations)