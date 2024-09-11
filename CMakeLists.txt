cmake_minimum_required(VERSION 3.30)

project(helloworld VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(EptConfig)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia Gui Network)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(PkgConfig REQUIRED)


pkg_check_modules(libuv REQUIRED IMPORTED_TARGET libuv)
add_library(libuv::libuv ALIAS PkgConfig::libuv)

pkg_check_modules(fftw REQUIRED IMPORTED_TARGET fftw3)
add_library(fftw::fftw ALIAS PkgConfig::fftw)

pkg_check_modules(QWT REQUIRED IMPORTED_TARGET Qt6Qwt6)
add_library(QWT::QWT ALIAS PkgConfig::QWT)


qt_standard_project_setup()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()


add_subdirectory(docs)
add_subdirectory(src)
