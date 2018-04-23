#ifndef GUIEVENTS_H
#define GUIEVENTS_H

#include <QEvent>
#include "GuiStructs.h"




//QEvent::Type idleEvent = (QEvent::Type)CUST_IDLE_EVENT; //1235;
QEvent::Type enableEvent = (QEvent::Type)CUST_ENABLE_EVENT;//1236;
QEvent::Type searchEvent = (QEvent::Type)CUST_SEARCH_EVENT;//1237;
QEvent::Type connectingEvent = (QEvent::Type)CUST_CONNECTING_EVENT;//1238;
QEvent::Type connectedEvent = (QEvent::Type)CUST_CONNECTED_EVENT;//1239;
QEvent::Type disConntingEvent = (QEvent::Type)CUST_DISCONNECTING_EVENT;//1240;
QEvent::Type disableEvent = (QEvent::Type)CUST_DISABLE_EVENT;//1241;

//config for Auto/Manual custom event
QEvent::Type guiCfgAutoEvent = (QEvent::Type)CUST_AUTO_EVENT;//1244;
QEvent::Type guiCfgManuEvent = (QEvent::Type)CUST_MANUAL_EVENT;//1245;

//config for log level custom event
QEvent::Type guiLogDebugEvent = (QEvent::Type)CUST_DEBUG_EVENT;//1246;
QEvent::Type guiLogInfoEvent = (QEvent::Type)CUST_INFO_EVENT;//1247;
QEvent::Type guiLogErrorEvent = (QEvent::Type)CUST_ERROR_EVENT;//1248;

//config for enabling server logging custom event
QEvent::Type guiServerLogEnabledEvent = (QEvent::Type)CUST_SER_LOG_ENABLED_EVENT;//1249;
QEvent::Type guiServerLogDisabledEvent = (QEvent::Type)CUST_SER_LOG_DISABLED_EVENT;//1250;

//Custom system event from the device
QEvent::Type guiSysDeviceErrorEvent = (QEvent::Type)CUST_SYS_DEVICE_ERROR_EVENT;//1251;
QEvent::Type guiSysDeviceNotOpenEvent = (QEvent::Type)CUST_SYS_DEVICE_NOT_OPEN_EVENT;//1252;
QEvent::Type guiSysDeviceOpenEvent = (QEvent::Type)CUST_SYS_DEVICE_OPEN_EVENT;//1253;
QEvent::Type guiSysDisconnectedEvent = (QEvent::Type)CUST_SYS_DISCONNECTED_EVENT;//1254;
QEvent::Type guiSysDeviceConnectedEvent = (QEvent::Type)CUST_SYS_DEVICE_CONNECTED_EVENT;//1255;
QEvent::Type guiSysDeviceDisconnectedEvent = (QEvent::Type)CUST_SYS_DEVICE_DISCONNECTED_EVENT;//1256;
QEvent::Type guiCMServerLostEvent = (QEvent::Type)CUST_CM_SERVER_CONN_LOST;//1257;


QEvent::Type guiScanResponseEvent = (QEvent::Type)CUST_SCAN_RESPONSE_EVENT; //1259
QEvent::Type guiWifiPrefNetworksEvent = (QEvent::Type)CUST_WIFI_PREF_EVENT; //1260
QEvent::Type guiWifiStatsRspEvent = (QEvent::Type)CUST_WIFI_STATS_RSP_EVENT; //1261
QEvent::Type guiWifiPasswordEvent = (QEvent::Type)CUST_WIFI_PASSWORD_EVENT; //1262

QEvent::Type guiThreeGStatsEvent = (QEvent::Type)CUST_3G_STATS_EVENT; //1263

QEvent::Type guiShowNMSettingsEvent = (QEvent::Type)CUST_SHOW_NM_SETTINGS_EVT; //1264
QEvent::Type guiAddInitialTechsEvent = (QEvent::Type)CUST_ADD_INITIAL_TECHS_EVT; //1265
QEvent::Type guiNMStateChangeEvent = (QEvent::Type)CUST_HANDLE_NM_STATE_CHANGE_EVT; //1266
QEvent::Type guiNMGetParamEvent = (QEvent::Type)CUST_SHOW_GOT_NM_PARAM; //1268
QEvent::Type guiNMUssdResponse = (QEvent::Type)CUST_USSD_RESPONSE; //1269
QEvent::Type guiNMConnectFail = (QEvent::Type) CUST_NM_CONNECT_FAIL_EVT;//1270
QEvent::Type guiWifiDisableNoti = (QEvent::Type) CUST_WIFI_DISABLED_NOTI_EVT;//1271
QEvent::Type guiRasProfileEvt = (QEvent::Type) CUST_INITIAL_RAS_PROFILE_EVT;//1272
QEvent::Type guiErrorDisplayEvt = (QEvent::Type) CUST_ERROR_DISPLAY_EVT;//1273
//vpn
QEvent::Type guiVpnProfilesList = (QEvent::Type) CUST_VPN_PROFILE_LIST_EVT;//1274
QEvent::Type guiVpnClientList = (QEvent::Type) CUST_VPN_CLIENT_LIST_EVT;//1275
QEvent::Type guiVpnConnect = (QEvent::Type) CUST_VPN_CONNECT_EVT;//1276
QEvent::Type guiVpnDisconnect = (QEvent::Type) CUST_VPN_DISCONNECT_EVT;//1277




class GuiNMStateChangeEvt : public QEvent {

public:
    NMStateChange stateChange;

    GuiNMStateChangeEvt(NMStateChange stateChange):QEvent(guiNMStateChangeEvent) {

        this->stateChange = stateChange;

    }
};

class GuiNMScanRspEvt : public QEvent {

public:
    NetworkListEncoded networkListEncoded;

    GuiNMScanRspEvt(NetworkListEncoded networkListEncoded):QEvent(guiScanResponseEvent) {

        this->networkListEncoded = networkListEncoded;

    }
};

class GuiWifiPreferredRspEvt : public QEvent {

public:
    string encodedSsids;

    GuiWifiPreferredRspEvt(string& encodedSsids):QEvent(guiWifiPrefNetworksEvent) {

        this->encodedSsids = encodedSsids;

    }
};

class GuiWifiStatsRspEvt : public QEvent {

public:
    string encodedSsids;

    GuiWifiStatsRspEvt(string& encodedSsids):QEvent(guiWifiStatsRspEvent) {

        this->encodedSsids = encodedSsids;

    }
};

class GuiWifiPasswordReqEvt : public QEvent {

public:
    string ssid;

    GuiWifiPasswordReqEvt(string& ssid):QEvent(guiWifiPasswordEvent) {

        this->ssid = ssid;

    }
};

class GuiConnectFail : public QEvent {

public:
    ETechSelect techId;
    string networkName;

    GuiConnectFail(ETechSelect techId,string networkName):QEvent(guiNMConnectFail) {

        this->techId = techId;
        this->networkName = networkName;

    }
};
class Gui3gStatsEvt : public QEvent {

public:
    string stats;

    Gui3gStatsEvt(string& stats):QEvent(guiThreeGStatsEvent) {

        this->stats = stats;

    }
};

class GuiNMGetParamEvt : public QEvent {

    public:
         NMGetParam getParam;

        GuiNMGetParamEvt(NMGetParam getParam):QEvent(guiNMGetParamEvent) {

            this->getParam = getParam;

        }

};

class GuiNMUssdResponseEvt : public QEvent {
public:
       string    ussd_response;
       GuiNMUssdResponseEvt(string val):QEvent(guiNMUssdResponse) {

           this->ussd_response = val;

       }

};

class GuiWifiDisabledEvt : public QEvent {
public:
       GuiWifiDisabledEvt():QEvent(guiWifiDisableNoti) {

       }

};

class GuiRasProfileEvt : public QEvent {
public:
       GuiRasProfileEvt(Ras_Details_s& details):QEvent(guiRasProfileEvt) {
            this->details = details;
       }
       Ras_Details_s details;

};

class GuiErrorDisplayEvt : public QEvent {
public:

    NMErrorInfo errorInfo;
    GuiErrorDisplayEvt(NMErrorInfo errorInfo):QEvent(guiErrorDisplayEvt){
    this->errorInfo = errorInfo;
    }
};

class GuiVpnProfileLstEvt : public QEvent {

public:
    string profLst;

	GuiVpnProfileLstEvt(string& profLst):QEvent(guiVpnProfilesList) {

		this->profLst= profLst;

    }
};

class GuiVpnClientLstEvt : public QEvent {

public:
    string cliLst;

    GuiVpnClientLstEvt(string& cliLst):QEvent(guiVpnClientList) {

        this->cliLst= cliLst;

    }
};
class GuiVpnConnectEvt : public QEvent {

public:
    int connectRsp;

    GuiVpnConnectEvt(int connectRsp):QEvent(guiVpnConnect) {

        this->connectRsp = connectRsp;

    }
};

class GuiVpnDisconnectEvt : public QEvent {

public:

    GuiVpnDisconnectEvt():QEvent(guiVpnDisconnect) {

    }
};

#endif // GUIEVENTS_H
