project(qtrest-core)

set(HEADERS_FILES
    apibase.h
    abstractjsonrestlistmodel.h
    abstractxmlrestlistmodel.h
    baserestlistmodel.h
    detailsmodel.h
    jsonrestlistmodel.h
    pagination.h
    qtrest_global.h
    requests.h
    restitem.h
    xmlrestlistmodel.h
)

set(SOURCE_FILES
    apibase.cpp
    abstractjsonrestlistmodel.cpp
    abstractxmlrestlistmodel.cpp
    baserestlistmodel.cpp
    detailsmodel.cpp
    jsonrestlistmodel.cpp
    pagination.cpp
    requests.cpp
    restitem.cpp
    xmlrestlistmodel.cpp
)

set(ALL_FILES ${HEADERS_FILES} ${SOURCE_FILES})

# add library
if("${BUILD_TYPE}" STREQUAL "STATIC")
    add_library(qtrest STATIC ${ALL_FILES})
    add_definitions(-DQTREST_BUILD_STATIC)
else()
    add_library(qtrest SHARED ${SOURCE_FILES})
    add_definitions(-DQTREST_BUILD_DYNAMIC)
endif()

set(ADDITIONAL_LIBRARIES "")
if ("${WITH_QML_SUPPORT}" STREQUAL "1")
    set(ADDITIONAL_LIBRARIES Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Qml)
  add_definitions(-DWITH_QML_SUPPORT)
endif()

target_link_libraries(qtrest PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network ${ADDITIONAL_LIBRARIES})

# install libs and headers
if("${BUILD_TYPE}" STREQUAL "STATIC")
    install(TARGETS qtrest DESTINATION ${QTREST_INSTALL_LIB_DIR})
else()
    # Use install(TARGETS) for proper cross-platform support
    install(TARGETS qtrest
        RUNTIME DESTINATION ${QTREST_INSTALL_BIN_DIR}   # Windows DLLs
        LIBRARY DESTINATION ${QTREST_INSTALL_LIB_DIR}   # Linux/macOS .so/.dylib
        ARCHIVE DESTINATION ${QTREST_INSTALL_LIB_DIR}   # Windows import libs
    )
endif()
install(FILES ${HEADERS_FILES} DESTINATION ${QTREST_INSTALL_INCLUDE_DIR})
