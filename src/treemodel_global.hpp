#ifndef TREEMODEL_GLOBAL_HPP
#define TREEMODEL_GLOBAL_HPP

#include <QtCompilerDetection>

#ifdef TREEMODEL_LIBRARY
# define TM_EXPORT Q_DECL_EXPORT
# else
# define TM_EXPORT Q_DECL_IMPORT
#endif

#endif // TREEMODEL_GLOBAL_HPP
