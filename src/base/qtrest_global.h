#ifndef QTREST_GLOBAL_H
#define QTREST_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(QTREST_LIBRARY)
#  define QTREST_EXPORT Q_DECL_EXPORT
#else
#  define QTREST_EXPORT Q_DECL_IMPORT
#endif

#endif // QTREST_GLOBAL_H
