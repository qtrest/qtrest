QT += network

HEADERS += $$PWD/src/apibase.h \
 $$PWD/src/models/baserestlistmodel.h \
 $$PWD/src/models/detailsmodel.h \
 $$PWD/src/models/jsonrestlistmodel.h \
 $$PWD/src/models/xmlrestlistmodel.h \
    $$PWD/src/models/restitem.h \
    $$PWD/src/usingleton.h \
    $$PWD/src/models/pagination.h
SOURCES += $$PWD/src/apibase.cpp \
 $$PWD/src/models/baserestlistmodel.cpp \
 $$PWD/src/models/detailsmodel.cpp \
 $$PWD/src/models/jsonrestlistmodel.cpp \
 $$PWD/src/models/xmlrestlistmodel.cpp \
    $$PWD/src/models/restitem.cpp \
    $$PWD/src/models/pagination.cpp

INCLUDEPATH += $$PWD/src/
INCLUDEPATH += $$PWD/src/models/

OTHER_FILES += $$PWD/README.md $$PWD/LICENSE $$PWD/QtMicroRestFramework.qmodel
