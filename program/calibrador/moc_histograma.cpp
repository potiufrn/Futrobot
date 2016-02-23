/****************************************************************************
** Histograma meta object code from reading C++ file 'histograma.h'
**
** Created: Thu May 27 13:24:02 2010
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "histograma.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Histograma::className() const
{
    return "Histograma";
}

QMetaObject *Histograma::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Histograma( "Histograma", &Histograma::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Histograma::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Histograma", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Histograma::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Histograma", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Histograma::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"limiarMinChanged", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"limiarMaxChanged", 1, param_signal_1 };
    static const QMetaData signal_tbl[] = {
	{ "limiarMinChanged(int)", &signal_0, QMetaData::Public },
	{ "limiarMaxChanged(int)", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"Histograma", parentObject,
	0, 0,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Histograma.setMetaObject( metaObj );
    return metaObj;
}

void* Histograma::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Histograma" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL limiarMinChanged
void Histograma::limiarMinChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL limiarMaxChanged
void Histograma::limiarMaxChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

bool Histograma::qt_invoke( int _id, QUObject* _o )
{
    return QWidget::qt_invoke(_id,_o);
}

bool Histograma::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: limiarMinChanged((int)static_QUType_int.get(_o+1)); break;
    case 1: limiarMaxChanged((int)static_QUType_int.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool Histograma::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool Histograma::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
