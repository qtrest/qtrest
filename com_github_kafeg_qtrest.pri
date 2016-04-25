QT += network

HEADERS += $$PWD/src/apibase.h \
 $$PWD/src/models/baserestlistmodel.h \
 $$PWD/src/models/detailsmodel.h \
    $$PWD/src/models/restitem.h \
    $$PWD/src/usingleton.h \
    $$PWD/src/models/pagination.h \
    $$PWD/src/models/abstractjsonrestlistmodel.h \
    $$PWD/src/models/abstractxmlrestlistmodel.h \
    $$PWD/src/models/jsonrestlistmodel.h \
    $$PWD/src/models/requests.h \
    $$PWD/src/models/xmlrestlistmodel.h
SOURCES += $$PWD/src/apibase.cpp \
 $$PWD/src/models/baserestlistmodel.cpp \
 $$PWD/src/models/detailsmodel.cpp \
    $$PWD/src/models/restitem.cpp \
    $$PWD/src/models/pagination.cpp \
    $$PWD/src/models/abstractjsonrestlistmodel.cpp \
    $$PWD/src/models/abstractxmlrestlistmodel.cpp \
    $$PWD/src/models/jsonrestlistmodel.cpp \
    $$PWD/src/models/requests.cpp \
    $$PWD/src/models/xmlrestlistmodel.cpp

INCLUDEPATH += $$PWD/src/
INCLUDEPATH += $$PWD/src/models/

OTHER_FILES += $$PWD/README.md $$PWD/LICENSE $$PWD/docs/QtMicroRestFramework.qmodel

RESOURCES += \
    $$PWD/com_github_kafeg_qtrest.qrc
	
