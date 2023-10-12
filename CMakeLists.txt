cmake_minimum_required(VERSION 3.12.0)

project(qtrest VERSION 0.3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#params
#available params:
# - BUILD_EXAMPLE=1
# - WITH_QML_SUPPORT=1

if("${BUILD_TYPE}" STREQUAL "SHARED")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    message("Build type: ${BUILD_TYPE}")
endif()

# find required packages
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

if ("${WITH_QML_SUPPORT}" STREQUAL "1")
    find_package(Qt5 COMPONENTS Widgets Qml Quick REQUIRED)
    message("Build with QML support")
else()
    message("Build without QML support")
endif()

set(HEADERS_FILES
    src/apibase.h
    src/models/abstractjsonrestlistmodel.h
    src/models/abstractxmlrestlistmodel.h
    src/models/baserestlistmodel.h
    src/models/detailsmodel.h
    src/models/jsonrestlistmodel.h
    src/models/pagination.h
    src/models/requests.h
    src/models/restitem.h
    src/models/xmlrestlistmodel.h
)

set(SOURCE_FILES
    src/apibase.cpp
    src/models/abstractjsonrestlistmodel.cpp
    src/models/abstractxmlrestlistmodel.cpp
    src/models/baserestlistmodel.cpp
    src/models/detailsmodel.cpp
    src/models/jsonrestlistmodel.cpp
    src/models/pagination.cpp
    src/models/requests.cpp
    src/models/restitem.cpp
    src/models/xmlrestlistmodel.cpp
)

set(INCLUDE_DIRS
    src
    src/models
)

set(ALL_FILES ${HEADERS_FILES} ${SOURCE_FILES})

#install
set(QTREST_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include/qtrest)
set(QTREST_INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)
set(QTREST_INSTALL_TOOLS_DIR ${CMAKE_INSTALL_PREFIX}/tools/qtrest)
set(QTREST_INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)

# add library
if("${BUILD_TYPE}" STREQUAL "STATIC")
    add_library(${PROJECT_NAME} STATIC ${ALL_FILES})
    add_definitions(-DQTREST_BUILD_STATIC)
else()
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
    add_definitions(-DQTREST_BUILD_DYNAMIC)
endif()

set(ADDITIONAL_LIBRARIES )
if ("${WITH_QML_SUPPORT}" STREQUAL "1")
  set(ADDITIONAL_LIBRARIES Qt5::Quick Qt5::Qml)
  add_definitions(-DWITH_QML_SUPPORT)
endif()

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network ${ADDITIONAL_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

# install libs and headers
if("${BUILD_TYPE}" STREQUAL "STATIC")
    install(TARGETS qtrest DESTINATION ${QTREST_INSTALL_LIB_DIR})
else()
    message("CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} ${QTREST_INSTALL_BIN_DIR} ${QTREST_INSTALL_LIB_DIR}")
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qtrest.dll DESTINATION ${QTREST_INSTALL_BIN_DIR})
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qtrest.lib DESTINATION ${QTREST_INSTALL_LIB_DIR})
endif()
install(FILES ${HEADERS_FILES} DESTINATION ${QTREST_INSTALL_INCLUDE_DIR})

# add example
if ("${BUILD_EXAMPLE}" STREQUAL "1")
    add_subdirectory(example)
endif()

