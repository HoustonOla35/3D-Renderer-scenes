QT += core gui widgets

TARGET = Sky
TEMPLATE = app
CONFIG += c++11

INCLUDEPATH += renderer/
include(renderer/3D-Renderer.pri)

SOURCES += main.cpp \
    SkyRenderer.cpp

HEADERS += \
    SkyRenderer.hpp \
    constants.hpp
