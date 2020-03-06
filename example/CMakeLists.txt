project(qtrest-example VERSION ${PROJECT_VERSION} LANGUAGES CXX)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

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

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Quick Qt5::Qml Qt5::Widgets qtrest)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
