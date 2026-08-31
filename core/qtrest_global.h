#ifndef QTREST_GLOBAL_H
#define QTREST_GLOBAL_H

#include <QtCore/qglobal.h>

// Export/import markup for the qtrest shared library.
//
// On Windows, moc-generated meta-object data symbols (e.g. the static
// QMetaObject APIBase::staticMetaObject) are NOT linkable from consumers
// through CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS alone: global data symbols must
// be marked with __declspec(dllexport/dllimport) so the consumer references
// them via the import thunk (__imp_*). Without this, consumers fail to link
// with LNK2019 (unresolved external symbol ...::staticMetaObject).
//
//   QTREST_BUILD_DYNAMIC - defined when building the qtrest shared library
//   QTREST_BUILD_STATIC  - defined when building the qtrest static library
//   (neither)            - consuming the library from another target
#if defined(QTREST_BUILD_STATIC)
#  define QTREST_EXPORT
#elif defined(QTREST_BUILD_DYNAMIC)
#  define QTREST_EXPORT Q_DECL_EXPORT
#else
#  define QTREST_EXPORT Q_DECL_IMPORT
#endif

#endif // QTREST_GLOBAL_H