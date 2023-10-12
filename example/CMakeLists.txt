project(qtrest-example VERSION ${PROJECT_VERSION} LANGUAGES CXX)

qt5_add_resources(QT_RESOURCES main.qrc)

set(HEADERS_FILES
    jsonplaceholderapi.h
)

set(SOURCE_FILES
    main.cpp
    jsonplaceholderapi.cpp
)

set(INCLUDE_DIRS
    .
)

set(ALL_FILES ${HEADERS_FILES} ${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${ALL_FILES} ${QT_RESOURCES})

set(ADDITIONAL_LIBRARIES)
if ("${WITH_QML_SUPPORT}" STREQUAL "1")
  set(ADDITIONAL_LIBRARIES Qt5::Quick Qt5::Qml)
  add_definitions(-DWITH_QML_SUPPORT)
endif()

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network ${ADDITIONAL_LIBRARIES} qtrest)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

set(BINARY_NAME ${PROJECT_NAME})
if (MSVC)
    set(BINARY_NAME ${PROJECT_NAME}.exe)
endif()

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${BINARY_NAME} DESTINATION ${QTREST_INSTALL_TOOLS_DIR})
