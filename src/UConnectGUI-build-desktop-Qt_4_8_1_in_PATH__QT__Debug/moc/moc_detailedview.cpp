/****************************************************************************
** Meta object code from reading C++ file 'detailedview.h'
**
** Created: Tue Jul 31 10:43:00 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../UConnectGUI/detailedview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'detailedview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DetailedView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      44,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      55,   47,   13,   13, 0x08,
      84,   47,   13,   13, 0x08,
     114,   47,   13,   13, 0x08,
     153,   47,   13,   13, 0x08,
     192,  187,   13,   13, 0x08,
     249,   13,   13,   13, 0x08,
     281,   13,   13,   13, 0x08,
     311,   13,   13,   13, 0x08,
     337,   13,   13,   13, 0x08,
     369,   13,   13,   13, 0x08,
     399,   13,   13,   13, 0x08,
     426,   13,   13,   13, 0x08,
     459,   13,   13,   13, 0x08,
     486,   13,   13,   13, 0x08,
     514,   13,   13,   13, 0x08,
     541,   13,   13,   13, 0x08,
     572,   13,   13,   13, 0x08,
     603,   13,   13,   13, 0x08,
     628,   13,   13,   13, 0x08,
     673,   13,   13,   13, 0x08,
     717,   13,   13,   13, 0x08,
     761,   13,   13,   13, 0x08,
     788,   13,   13,   13, 0x08,
     814,   13,   13,   13, 0x08,
     840,   13,   13,   13, 0x08,
     872,   13,   13,   13, 0x08,
     897,   47,   13,   13, 0x08,
     923,   13,   13,   13, 0x08,
     941,   13,   13,   13, 0x08,
     966,   13,   13,   13, 0x08,
     989,   13,   13,   13, 0x08,
    1013,   13,   13,   13, 0x08,
    1036,   13,   13,   13, 0x08,
    1060,   13,   13,   13, 0x08,
    1092, 1086,   13,   13, 0x08,
    1142,   13,   13,   13, 0x08,
    1167,   47,   13,   13, 0x08,
    1201,   13,   13,   13, 0x08,
    1252,   13,   13,   13, 0x08,
    1262,   13,   13,   13, 0x08,
    1285,   13,   13,   13, 0x08,
    1310,   13,   13,   13, 0x08,
    1336,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DetailedView[] = {
    "DetailedView\0\0on_applySettingsButton_clicked()\0"
    "checked\0on_lteCheckBox_clicked(bool)\0"
    "on_wifiCheckBox_clicked(bool)\0"
    "on_preferredNetworkCheck_clicked(bool)\0"
    "on_wifiNetworkCheck_clicked(bool)\0"
    "item\0on_wifiPreferredList_itemDoubleClicked(QListWidgetItem*)\0"
    "on_wifiDownPushButton_clicked()\0"
    "on_wifiUpPushButton_clicked()\0"
    "on_connectState_clicked()\0"
    "on_techDownPushButton_clicked()\0"
    "on_techUpPushButton_clicked()\0"
    "on_vpnPushButton_clicked()\0"
    "on_tetheringPushButton_clicked()\0"
    "on_ltePushButton_clicked()\0"
    "on_wifiPushButton_clicked()\0"
    "on__3gPushButton_clicked()\0"
    "on_generalPushButton_clicked()\0"
    "on_tetheringCheckBox_clicked()\0"
    "on__3GCheckBox_clicked()\0"
    "on_wificomboBox_currentIndexChanged(QString)\0"
    "on__3gcomboBox_currentIndexChanged(QString)\0"
    "on_ltecomboBox_currentIndexChanged(QString)\0"
    "on_wifi_checkBox_clicked()\0"
    "on__3g_checkBox_clicked()\0"
    "on_Lte_checkBox_clicked()\0"
    "on_PasswordEdit_returnPressed()\0"
    "on_wifiConnect_clicked()\0"
    "on_wifiScan_clicked(bool)\0BuildStatWidget()\0"
    "on_radioManual_clicked()\0"
    "on_radioAuto_clicked()\0on_radioError_clicked()\0"
    "on_radioInfo_clicked()\0on_radioDebug_clicked()\0"
    "on_searchButton_clicked()\0index\0"
    "on_m_editRefreshInterval_currentIndexChanged(int)\0"
    "on_GetModStats_clicked()\0"
    "on_fileLogginOption_clicked(bool)\0"
    "trayIconClicked(QSystemTrayIcon::ActivationReason)\0"
    "appQuit()\0on_send_ussd_clicked()\0"
    "on_rasProfSave_clicked()\0"
    "on_vpnAutoCheck_clicked()\0"
    "on_vpnButton_clicked()\0"
};

void DetailedView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DetailedView *_t = static_cast<DetailedView *>(_o);
        switch (_id) {
        case 0: _t->on_applySettingsButton_clicked(); break;
        case 1: _t->on_lteCheckBox_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_wifiCheckBox_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_preferredNetworkCheck_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_wifiNetworkCheck_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_wifiPreferredList_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 6: _t->on_wifiDownPushButton_clicked(); break;
        case 7: _t->on_wifiUpPushButton_clicked(); break;
        case 8: _t->on_connectState_clicked(); break;
        case 9: _t->on_techDownPushButton_clicked(); break;
        case 10: _t->on_techUpPushButton_clicked(); break;
        case 11: _t->on_vpnPushButton_clicked(); break;
        case 12: _t->on_tetheringPushButton_clicked(); break;
        case 13: _t->on_ltePushButton_clicked(); break;
        case 14: _t->on_wifiPushButton_clicked(); break;
        case 15: _t->on__3gPushButton_clicked(); break;
        case 16: _t->on_generalPushButton_clicked(); break;
        case 17: _t->on_tetheringCheckBox_clicked(); break;
        case 18: _t->on__3GCheckBox_clicked(); break;
        case 19: _t->on_wificomboBox_currentIndexChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: _t->on__3gcomboBox_currentIndexChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: _t->on_ltecomboBox_currentIndexChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 22: _t->on_wifi_checkBox_clicked(); break;
        case 23: _t->on__3g_checkBox_clicked(); break;
        case 24: _t->on_Lte_checkBox_clicked(); break;
        case 25: _t->on_PasswordEdit_returnPressed(); break;
        case 26: _t->on_wifiConnect_clicked(); break;
        case 27: _t->on_wifiScan_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: _t->BuildStatWidget(); break;
        case 29: _t->on_radioManual_clicked(); break;
        case 30: _t->on_radioAuto_clicked(); break;
        case 31: _t->on_radioError_clicked(); break;
        case 32: _t->on_radioInfo_clicked(); break;
        case 33: _t->on_radioDebug_clicked(); break;
        case 34: _t->on_searchButton_clicked(); break;
        case 35: _t->on_m_editRefreshInterval_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->on_GetModStats_clicked(); break;
        case 37: _t->on_fileLogginOption_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 38: _t->trayIconClicked((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 39: _t->appQuit(); break;
        case 40: _t->on_send_ussd_clicked(); break;
        case 41: _t->on_rasProfSave_clicked(); break;
        case 42: _t->on_vpnAutoCheck_clicked(); break;
        case 43: _t->on_vpnButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DetailedView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DetailedView::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_DetailedView,
      qt_meta_data_DetailedView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DetailedView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DetailedView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DetailedView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DetailedView))
        return static_cast<void*>(const_cast< DetailedView*>(this));
    return QFrame::qt_metacast(_clname);
}

int DetailedView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 44)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 44;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
