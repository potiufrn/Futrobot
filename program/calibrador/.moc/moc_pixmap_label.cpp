/****************************************************************************
** Meta object code from reading C++ file 'pixmap_label.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../pixmap_label.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pixmap_label.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_pixmap_label[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      33,   13,   13,   13, 0x05,
      54,   13,   13,   13, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_pixmap_label[] = {
    "pixmap_label\0\0mouseMoved(QPoint)\0"
    "mousePressed(QPoint)\0mouseReleased(QPoint)\0"
};

void pixmap_label::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        pixmap_label *_t = static_cast<pixmap_label *>(_o);
        switch (_id) {
        case 0: _t->mouseMoved((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 1: _t->mousePressed((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 2: _t->mouseReleased((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData pixmap_label::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject pixmap_label::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_pixmap_label,
      qt_meta_data_pixmap_label, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &pixmap_label::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *pixmap_label::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *pixmap_label::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_pixmap_label))
        return static_cast<void*>(const_cast< pixmap_label*>(this));
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
    }
    return _id;
}

// SIGNAL 0
void pixmap_label::mouseMoved(QPoint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void pixmap_label::mousePressed(QPoint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void pixmap_label::mouseReleased(QPoint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
