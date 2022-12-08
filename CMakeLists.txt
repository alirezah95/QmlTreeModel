cmake_minimum_required(VERSION 3.24.0)

qt_add_qml_module(treemodel
    NO_PLUGIN
    URI treemodel
    VERSION 1.0
    SOURCES
        treemodel.cpp treemodel.h treeitem.cpp treeitem.h
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    treemodel
)

