/****************************************************************************
** Meta object code from reading C++ file 'futdatatable.h'
**
** Created: Sat Aug 14 15:17:12 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../interface/futdatatable.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'futdatatable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FutDataTable[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_FutDataTable[] = {
    "FutDataTable\0"
};

const QMetaObject FutDataTable::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FutDataTable,
      qt_meta_data_FutDataTable, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FutDataTable::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FutDataTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FutDataTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FutDataTable))
        return static_cast<void*>(const_cast< FutDataTable*>(this));
    return QWidget::qt_metacast(_clname);
}

int FutDataTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
