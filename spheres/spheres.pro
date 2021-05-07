QT += core gui widgets

TARGET = Spheres
TEMPLATE = app
#DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

INCLUDEPATH += renderer/
include(renderer/3D-Renderer.pri)

SOURCES += main.cpp

