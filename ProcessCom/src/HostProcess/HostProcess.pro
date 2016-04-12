QT += core
QT -= gui

TARGET = HostProcess
CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DESTDIR = ../../bin

SOURCES += main.cpp \
    hostprocess.cpp

HEADERS += \
    hostprocess.h
