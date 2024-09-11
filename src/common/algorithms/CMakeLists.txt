

add_subdirectory(entropyminimizer)
add_subdirectory(examplealgorithm)
add_subdirectory(pitchraise)
add_subdirectory(resettorecording)

file(RELATIVE_PATH SUBDIRECTORY_RELATIVE_DIR "${PROJECT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Processing ${SUBDIRECTORY_RELATIVE_DIR}" )
string(REPLACE "/" "-" SUBMODULE_NAME "ept-${SUBDIRECTORY_RELATIVE_DIR}")
string(REGEX REPLACE "[^A-Za-z0-9\-]" "_" "SUBMODULE_NAME" "${SUBMODULE_NAME}")
message(STATUS "Submodule ${SUBMODULE_NAME}" )

add_library(
    ${SUBMODULE_NAME}
    INTERFACE
)

target_link_libraries(
    ${SUBMODULE_NAME}
    INTERFACE
    ${SUBMODULE_NAME}-entropyminimizer
    ${SUBMODULE_NAME}-examplealgorithm
    ${SUBMODULE_NAME}-pitchraise
    ${SUBMODULE_NAME}-resettorecording
)




# target_include_directories(
#     ${SUBMODULE_NAME}
#     INTERFACE
#     "${SUBDIRECTORY_PARENT}"
# )
