/****************************************************************************
** Plotter meta object code from reading C++ file 'plotter.h'
**
** Created: Fri Jul 29 20:06:57 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.3   edited Aug 5 16:40 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "plotter.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Plotter::className() const
{
    return "Plotter";
}

QMetaObject *Plotter::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Plotter( "Plotter", &Plotter::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Plotter::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Plotter", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Plotter::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Plotter", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Plotter::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"zoomIn", 0, 0 };
    static const QUMethod slot_1 = {"zoomOut", 0, 0 };
    static const QUMethod slot_2 = {"save", 0, 0 };
    static const QUMethod slot_3 = {"print", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "zoomIn()", &slot_0, QMetaData::Public },
	{ "zoomOut()", &slot_1, QMetaData::Public },
	{ "save()", &slot_2, QMetaData::Public },
	{ "print()", &slot_3, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"Plotter", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Plotter.setMetaObject( metaObj );
    return metaObj;
}

void* Plotter::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Plotter" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool Plotter::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: zoomIn(); break;
    case 1: zoomOut(); break;
    case 2: save(); break;
    case 3: print(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Plotter::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Plotter::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool Plotter::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
