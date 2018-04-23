/****************************************************************************
** Meta object code from reading C++ file 'minview.h'
**
** Created: Tue Jul 31 10:43:00 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../UConnectGUI/minview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'minview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MinimumView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      44,   12,   12,   12, 0x08,
      72,   12,   12,   12, 0x08,
      98,   12,   12,   12, 0x08,
     132,   12,   12,   12, 0x08,
     159,   12,   12,   12, 0x08,
     184,   12,   12,   12, 0x08,
     210,   12,   12,   12, 0x08,
     234,   12,   12,   12, 0x08,
     269,   12,   12,   12, 0x08,
     306,   12,   12,   12, 0x08,
     341,   12,   12,   12, 0x08,
     377,   12,   12,   12, 0x08,
     408,   12,   12,   12, 0x08,
     437,   12,   12,   12, 0x08,
     467,   12,   12,   12, 0x08,
     518,   12,   12,   12, 0x08,
     535,  528,   12,   12, 0x08,
     558,  528,   12,   12, 0x08,
     581,  528,   12,   12, 0x08,
     603,  528,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MinimumView[] = {
    "MinimumView\0\0on_versionPushButton_clicked()\0"
    "on_threeGInfoDown_clicked()\0"
    "on_threeGInfoUp_clicked()\0"
    "on_onselect3gPushButton_clicked()\0"
    "on_lte4gInfoDown_clicked()\0"
    "on_lte4gInfoUp_clicked()\0"
    "on_wifiInfoDown_clicked()\0"
    "on_wifiInfoUp_clicked()\0"
    "on_wifiComboBox_activated(QString)\0"
    "on_onselectWiredPushButton_clicked()\0"
    "on_onselectLtePushButton_clicked()\0"
    "on_onselectwifiPushButton_clicked()\0"
    "on_detail_pushButton_clicked()\0"
    "on_help_pushButton_clicked()\0"
    "on_minconnectButton_clicked()\0"
    "trayIconClicked(QSystemTrayIcon::ActivationReason)\0"
    "appQuit()\0height\0animatedDetResize(int)\0"
    "animatedLTEResize(int)\0animated3GResize(int)\0"
    "animatedWifiResize(int)\0"
};

void MinimumView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MinimumView *_t = static_cast<MinimumView *>(_o);
        switch (_id) {
        case 0: _t->on_versionPushButton_clicked(); break;
        case 1: _t->on_threeGInfoDown_clicked(); break;
        case 2: _t->on_threeGInfoUp_clicked(); break;
        case 3: _t->on_onselect3gPushButton_clicked(); break;
        case 4: _t->on_lte4gInfoDown_clicked(); break;
        case 5: _t->on_lte4gInfoUp_clicked(); break;
        case 6: _t->on_wifiInfoDown_clicked(); break;
        case 7: _t->on_wifiInfoUp_clicked(); break;
        case 8: _t->on_wifiComboBox_activated((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->on_onselectWiredPushButton_clicked(); break;
        case 10: _t->on_onselectLtePushButton_clicked(); break;
        case 11: _t->on_onselectwifiPushButton_clicked(); break;
        case 12: _t->on_detail_pushButton_clicked(); break;
        case 13: _t->on_help_pushButton_clicked(); break;
        case 14: _t->on_minconnectButton_clicked(); break;
        case 15: _t->trayIconClicked((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 16: _t->appQuit(); break;
        case 17: _t->animatedDetResize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->animatedLTEResize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->animated3GResize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->animatedWifiResize((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MinimumView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MinimumView::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MinimumView,
      qt_meta_data_MinimumView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MinimumView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MinimumView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MinimumView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MinimumView))
        return static_cast<void*>(const_cast< MinimumView*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MinimumView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
