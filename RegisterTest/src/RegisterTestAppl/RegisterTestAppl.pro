QT += core
QT -= gui

TARGET = RegisterTestAppl
TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

include(../Utilities/Utilities.pri)

PLATFORM = Win32
PLATFORM_TOOLSET = v120
LINKAGE = dynamic
CONFIG(debug, debug|release) {
    CONFIGURATION = Debug
} else {
    CONFIGURATION = Release
}

DESTDIR = $$PWD/../../bin/$$PLATFORM/$$CONFIGURATION/$$PLATFORM_TOOLSET

DEFINES *= QT_USE_QSTRINGBUILDER
DEFINES *= QT_NO_CAST_FROM_ASCII \
           QT_NO_CAST_TO_ASCII

SOURCES += main.cpp \
    registertestappl.cpp

HEADERS += \
    testpropertyobject.h \
    registertestappl.h
