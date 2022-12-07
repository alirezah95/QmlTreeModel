cmake_minimum_required(VERSION 3.24.0)

project(TreeModel VERSION 1.0 LANGUAGES CXX)

qt_add_qml_module(TreeModel
    URI treemodel
    VERSION 1.0
    SOURCES
        treemodel.cpp treemodel.h treeitem.cpp treeitem.h
)
