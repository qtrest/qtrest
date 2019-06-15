cmake_minimum_required(VERSION 3.12.0)

set(PROJECT_VERSION 0.2.0)

add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

project(qtrest VERSION ${PROJECT_VERSION} LANGUAGES CXX)

find_package(Qt5 COMPONENTS Widgets Qml Quick REQUIRED)

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

add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Quick Qt5::Qml)

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

add_subdirectory(example)

