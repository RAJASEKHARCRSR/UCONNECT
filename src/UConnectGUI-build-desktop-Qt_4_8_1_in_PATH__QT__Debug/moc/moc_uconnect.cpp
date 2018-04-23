/****************************************************************************
** Meta object code from reading C++ file 'uconnect.h'
**
** Created: Tue Jul 31 10:43:00 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../UConnectGUI/uconnect.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uconnect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Uconnect[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      21,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Uconnect[] = {
    "Uconnect\0\0showTime()\0periodicUpdateThread()\0"
};

void Uconnect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Uconnect *_t = static_cast<Uconnect *>(_o);
        switch (_id) {
        case 0: _t->showTime(); break;
        case 1: _t->periodicUpdateThread(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Uconnect::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Uconnect::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Uconnect,
      qt_meta_data_Uconnect, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Uconnect::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Uconnect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Uconnect::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Uconnect))
        return static_cast<void*>(const_cast< Uconnect*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Uconnect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
