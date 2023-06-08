cmake_minimum_required(VERSION 3.24)

find_package(QT NAMES Qt6 Qt5)

set(PROJECT_NAME "QmlTreeModel")
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION_TWEAK 0)
if (PROJECT_VERSION_TWEAK EQUAL 0)
    set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
else()
    set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK})
endif()
set(PROJECT_VERSION_TYPE "beta")
set(PROJECT_LICENSE_TYPE "MIT")
set(PROJECT_CREATOR "Alireza Hasani")
set(PROJECT_LANGUAGES "CXX")
set(PROJECT_DESCRIPTION "A simple tree model for QML")
set(PROJECT_HOMEPAGE_URL "https://github.com/alirezah95/QmlTreeModel")


if(QT_VERSION_MAJOR EQUAL 6)
    # For Qt 6.2.x and Qt 6.3.x, major version in qt_add_qml_module() must be
    # greater than zero otherwise module is not registered
    qt_add_qml_module(${PROJECT_NAME}
        URI ${PROJECT_NAME}
        SHARED
        VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        SOURCES
            treemodel.cpp
            treemodel.h
            treeitem.cpp
            treeitem.h
    )
else()

endif()
