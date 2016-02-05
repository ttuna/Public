INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

SOURCES += \
    $$PWD/util_json.cpp \
    $$PWD/util_configfile.cpp

HEADERS +=  $$PWD/util_templates.h \
    $$PWD/util_properties.h \
    $$PWD/util_global.h \
    $$PWD/util_connector.h \
    $$PWD/util_json.h \
    $$PWD/util_configfile.h

windows {
    contains(QT, network): {
        SOURCES += $$PWD/util_network.cpp
        HEADERS += $$PWD/util_network.h
    }
}
