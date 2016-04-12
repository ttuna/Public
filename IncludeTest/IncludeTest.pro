QT += core
QT -= gui

CONFIG += c++11

TARGET = IncludeTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    a.cpp \
    b.cpp \
    c.cpp

HEADERS += \
    a.h \
    b.h \
    c.h
