cmake_minimum_required(VERSION 3.12.0)

project(qtrest VERSION 0.4.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# params
# available params:
# - BUILD_EXAMPLE=1
# - WITH_QML_SUPPORT=1

if("${BUILD_TYPE}" STREQUAL "SHARED")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    message("Build type: ${BUILD_TYPE}")
endif()

# dependencies
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

if ("${WITH_QML_SUPPORT}" STREQUAL "1")
    find_package(Qt5 COMPONENTS Widgets Qml Quick REQUIRED)
    message("Build with QML support")
else()
    message("Build without QML support")
endif()

# install
set(QTREST_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include/qtrest)
set(QTREST_INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)
set(QTREST_INSTALL_TOOLS_DIR ${CMAKE_INSTALL_PREFIX}/tools/qtrest)
set(QTREST_INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)

set(PTYQTLIB_HEADERS_DIR ${PROJECT_SOURCE_DIR}/core)

include_directories(${PTYQT_INSTALL_INCLUDE_DIR})
include_directories(${PTYQTLIB_HEADERS_DIR})

# add library
add_subdirectory(core)

# add example
if ("${BUILD_EXAMPLE}" STREQUAL "1")
    add_subdirectory(example)
endif()

