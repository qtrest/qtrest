cmake_minimum_required(VERSION 3.12.0)

project(qtrest VERSION ${PROJECT_VERSION} LANGUAGES CXX)

find_package(Qt5Network REQUIRED)
find_package(Qt5Quick REQUIRED)

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

target_link_libraries(qtrest Qt5::Core Qt5::Network Qt5::Quick)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
