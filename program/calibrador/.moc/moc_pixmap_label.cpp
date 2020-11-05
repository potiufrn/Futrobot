/****************************************************************************
** Meta object code from reading C++ file 'pixmap_label.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../pixmap_label.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pixmap_label.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_pixmap_label_t {
    QByteArrayData data[5];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_pixmap_label_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_pixmap_label_t qt_meta_stringdata_pixmap_label = {
    {
QT_MOC_LITERAL(0, 0, 12), // "pixmap_label"
QT_MOC_LITERAL(1, 13, 10), // "mouseMoved"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 12), // "mousePressed"
QT_MOC_LITERAL(4, 38, 13) // "mouseReleased"

    },
    "pixmap_label\0mouseMoved\0\0mousePressed\0"
    "mouseReleased"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_pixmap_label[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       3,    1,   32,    2, 0x06 /* Public */,
       4,    1,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPoint,    2,
    QMetaType::Void, QMetaType::QPoint,    2,
    QMetaType::Void, QMetaType::QPoint,    2,

       0        // eod
};

void pixmap_label::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<pixmap_label *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mouseMoved((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 1: _t->mousePressed((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 2: _t->mouseReleased((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (pixmap_label::*)(QPoint );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&pixmap_label::mouseMoved)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (pixmap_label::*)(QPoint );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&pixmap_label::mousePressed)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (pixmap_label::*)(QPoint );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&pixmap_label::mouseReleased)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject pixmap_label::staticMetaObject = { {
    &QLabel::staticMetaObject,
    qt_meta_stringdata_pixmap_label.data,
    qt_meta_data_pixmap_label,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *pixmap_label::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *pixmap_label::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_pixmap_label.stringdata0))
        return static_cast<void*>(this);
    return QLabel::qt_metacast(_clname);
}

int pixmap_label::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void pixmap_label::mouseMoved(QPoint _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void pixmap_label::mousePressed(QPoint _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void pixmap_label::mouseReleased(QPoint _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
