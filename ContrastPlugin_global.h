#ifndef CONTRASTPLUGIN_GLOBAL_H
#define CONTRASTPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CONTRASTPLUGIN_LIBRARY)
#define CONTRASTPLUGIN_EXPORT Q_DECL_EXPORT
#else
#define CONTRASTPLUGIN_EXPORT Q_DECL_IMPORT
#endif

#endif // CONTRASTPLUGIN_GLOBAL_H
