/****************************************************************************
** Meta object code from reading C++ file 'hostprocess.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/HostProcess/hostprocess.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hostprocess.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_HostProcess_t {
    QByteArrayData data[9];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HostProcess_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HostProcess_t qt_meta_stringdata_HostProcess = {
    {
QT_MOC_LITERAL(0, 0, 11), // "HostProcess"
QT_MOC_LITERAL(1, 12, 6), // "launch"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 13), // "in_executable"
QT_MOC_LITERAL(4, 34, 7), // "in_args"
QT_MOC_LITERAL(5, 42, 5), // "write"
QT_MOC_LITERAL(6, 48, 7), // "in_data"
QT_MOC_LITERAL(7, 56, 25), // "readProcessStandardOutput"
QT_MOC_LITERAL(8, 82, 17) // "readProcessOutput"

    },
    "HostProcess\0launch\0\0in_executable\0"
    "in_args\0write\0in_data\0readProcessStandardOutput\0"
    "readProcessOutput"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HostProcess[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x0a /* Public */,
       1,    1,   44,    2, 0x2a /* Public | MethodCloned */,
       5,    1,   47,    2, 0x0a /* Public */,
       7,    0,   50,    2, 0x08 /* Private */,
       8,    0,   51,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Bool, QMetaType::QString, QMetaType::QStringList,    3,    4,
    QMetaType::Bool, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HostProcess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HostProcess *_t = static_cast<HostProcess *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { bool _r = _t->launch((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = _t->launch((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: _t->write((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->readProcessStandardOutput(); break;
        case 4: _t->readProcessOutput(); break;
        default: ;
        }
    }
}

const QMetaObject HostProcess::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HostProcess.data,
      qt_meta_data_HostProcess,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *HostProcess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HostProcess::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_HostProcess.stringdata0))
        return static_cast<void*>(const_cast< HostProcess*>(this));
    return QObject::qt_metacast(_clname);
}

int HostProcess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
