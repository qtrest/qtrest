project(qtrest-core)

set(HEADERS_FILES
    apibase.h
    abstractjsonrestlistmodel.h
    abstractxmlrestlistmodel.h
    baserestlistmodel.h
    detailsmodel.h
    jsonrestlistmodel.h
    pagination.h
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
  set(ADDITIONAL_LIBRARIES Qt5::Quick Qt5::Qml)
  add_definitions(-DWITH_QML_SUPPORT)
endif()

target_link_libraries(qtrest PUBLIC Qt5::Core Qt5::Network ${ADDITIONAL_LIBRARIES})

# install libs and headers
if("${BUILD_TYPE}" STREQUAL "STATIC")
    install(TARGETS qtrest DESTINATION ${QTREST_INSTALL_LIB_DIR})
else()
    message("CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} ${QTREST_INSTALL_BIN_DIR} ${QTREST_INSTALL_LIB_DIR}")
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qtrest.dll DESTINATION ${QTREST_INSTALL_BIN_DIR})
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qtrest.lib DESTINATION ${QTREST_INSTALL_LIB_DIR})
endif()
install(FILES ${HEADERS_FILES} DESTINATION ${QTREST_INSTALL_INCLUDE_DIR})
