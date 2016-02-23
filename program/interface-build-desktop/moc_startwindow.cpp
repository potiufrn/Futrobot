/****************************************************************************
** Meta object code from reading C++ file 'startwindow.h'
**
** Created: Sat Aug 14 15:17:11 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../interface/startwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'startwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StartWindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      31,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_StartWindow[] = {
    "StartWindow\0\0optionsselected()\0"
    "optionschoosen()\0"
};

const QMetaObject StartWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_StartWindow,
      qt_meta_data_StartWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StartWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StartWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StartWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StartWindow))
        return static_cast<void*>(const_cast< StartWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int StartWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: optionsselected(); break;
        case 1: optionschoosen(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
