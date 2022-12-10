cmake_minimum_required(VERSION 3.24.0)

qt_add_qml_module(treemodel
    URI treemodel
    VERSION 1.0
    SOURCES
        treemodel.cpp treemodel.h treeitem.cpp treeitem.h
)

# This is required on Windows and MSVC
target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Next line is required for Qml Preview on Linux to be able to load the module
target_link_libraries(${PROJECT_NAME} PRIVATE
    treemodel
)

