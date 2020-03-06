cmake_minimum_required(VERSION 3.12.0)

set(API_VERSION "0.2")
set(PATCH_VERSION "0")
project(qtrest VERSION "${API_VERSION}.${PATCH_VERSION}" LANGUAGES CXX)

add_definitions(${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Qml Quick REQUIRED)

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(SOURCE_FILES
    src/base/pagination.cpp
    src/base/apibase.cpp
    src/base/requests.cpp
    src/base/restitem.cpp
    src/base/detailsmodel.cpp
    src/base/baserestlistmodel.cpp
    src/abstractjsonrestlistmodel.cpp
    src/abstractxmlrestlistmodel.cpp
    src/jsonrestlistmodel.cpp
    src/xmlrestlistmodel.cpp
)

set(ALL_FILES ${HEADERS_FILES} ${SOURCE_FILES})

add_library(${PROJECT_NAME} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION "${PROJECT_VERSION}"
    SOVERSION ${API_VERSION}
    INTERFACE_${PROJECT_NAME}_MAJOR_VERSION ${API_VERSION}
)
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY
             COMPATIBLE_INTERFACE_STRING ${PROJECT_NAME}_MAJOR_VERSION)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Quick Qt5::Qml)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/base>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_definitions(${PROJECT_NAME} PRIVATE QTREST_LIBRARY)

add_subdirectory(example)

# Installation

if (NOT CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR ".")
endif()

if (NOT CMAKE_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR ".")
endif()

if (NOT CMAKE_INSTALL_INCLUDEDIR)
    set(CMAKE_INSTALL_INCLUDEDIR "include")
endif()

configure_file(${PROJECT_NAME}.pc.in ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc @ONLY NEWLINE_STYLE UNIX)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)
install(DIRECTORY src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        FILES_MATCHING PATTERN "*.h")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

export(PACKAGE ${PROJECT_NAME})
export(EXPORT ${PROJECT_NAME}Targets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Targets.cmake")
configure_file(cmake/${PROJECT_NAME}Config.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake"
    COPYONLY
)

set(ConfigFilesLocation "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake DESTINATION ${ConfigFilesLocation})

install(FILES cmake/${PROJECT_NAME}Config.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${ConfigFilesLocation}
)

if (UNIX AND NOT APPLE)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()
