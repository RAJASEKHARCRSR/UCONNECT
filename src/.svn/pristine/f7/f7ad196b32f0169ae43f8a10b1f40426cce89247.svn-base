/*****************************************************************************
**
**  uconnect.h
**
**	This file contains the definitions of the enums,
      structures and functions prototype used in Uconnect GUI
**
**	Authors: Suresh Lingadalli & Ramya.B
**	Date  : 15-MAR-2011
**  (c) United Mobile Apps Pvt Ltd - (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
    0.1		15/03/10     RB			Creation
    0.2     29/03/10     SL         addition of statusbar, menu bar,
                                    background colours, system tray.

*****************************************************************************/
#ifndef UCONNECT_H
#define UCONNECT_H

/////////////////////////////////////////////////////////////////////////////
/////////////////////////// INCLUDE FILES ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//#define _WIN32_WINNT 0x0501
//#define _WIN32_WINDOWS 0x0501
//#define WINVER 0x0501
#ifdef WIN32
#include <windows.h>
#include <dbt.h>
#endif
#include <QAbstractEventDispatcher>

#include <QMenu>
#include <QTime>
#include <QEvent>
#include <QDebug>
#include <QLabel>
#include <string>
#include <QTimer>
#include <QTime>
#include <ctime>
#include <time.h>
#include <QString>
#include <QWidget>
#include <fstream>
#include <typeinfo>
#include <iostream>
#include <QThread.h>
#include <QTabWidget>
#include <QStatusBar>
#include <QCloseEvent>
#include <QTreeWidget>
#include <QMessageBox>
#include <QCloseEvent>
#include <QMainWindow>

#include <QSharedMemory>
#include <QDesktopServices>
#include <QProcess>
#include <QUrl>
#include <QDir>

#include <qlayout.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_plot_canvas.h>


#include "UTypes.h"
#include "uconn_OSAL.h"
#include "uconn_Event.h"
#include "uconn_ServerAPI.h"
#include "uconn_SAPIwrapper.h"
#include "CMGraphStats.h"
#include "minview.h"
#include "detailedview.h"

#include "GuiStructs.h"
#include <cmath>





extern U_UINT16    serverPort;

/////////////////////////////////////////////////////////////////////////////
////////////////////////////STANDARD BASE CLASSES ///////////////////////////
/////////////////////////////////////////////////////////////////////////////

QT_BEGIN_NAMESPACE
class QMenu;
class QAction;
QT_END_NAMESPACE

/////////////////////////////////////////////////////////////////////////////
/////////////////////////// ENUMERATION ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
typedef enum{
    DEBUG = 0,
    INFO = 1,
    mERROR = 2,
}log_level;



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////// STRUCTURES //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


struct UIEntry {
    QString     tName;
    U_UINT32    u32Value;
};

typedef struct CMinfo{
    U_UINT32 PLMNidx;
    U_UINT32 m_nState;
    U_UINT32 m_nDataRate;
    U_UINT32 m_nULDataRate;
    U_UINT32 m_nDLDataRate;
    U_UINT32 m_nModemState;
    U_SINT8 m_nRssi[20];
    U_SINT8 m_nCinr[20];
    U_UINT32 m_nDeviceState;
    Modem_stats_s m_nModemStats;
    UAltairStats_t m_nAltairStats;
    L1A_Stats_s m_nL1AStats;
    //Meas_Stats_s m_nRSRPStats;
    //Meas_Stats_s m_nRSRQStats;
    //Meas_Stats_s m_nSINRStats;
    Meas_Stats_s  m_nMeasStats;
    Broadcom_Stats_s m_nBroadStats;
    Altair_Stored_Stats_s m_altairStoredstats;
    Broadcom_Stored_Stats_s m_broadcomStoredstats;
    //U_SINT32 RSRP_Report;
   // U_SINT32 RSRQ_Report;
    //U_SINT32 SINR_Report;
    int txThroughput;
    int rxThroughput;
    U_SINT8 ipAddress[30];
}CMinfo;

struct ConnectionChangeParams {

    ETechSelect techId;
    bool connectionStatus;

};


struct TechDetails {

    ETechSelect techId;
    string name;
    bool enabled;
    bool connected;
    U_UINT32 priority;
    ENMStateForBM state;
	ENMStateForBM oldState;

};

struct NMSettings {

    ETechSelect techId;
    bool show;

};











namespace Ui{
    class Uconnect;

}

/////////////////////////////////////////////////////////////////////////////
//////////////////////////// CLASS DECLARATIONS ////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class Uconnect : public QMainWindow
{
    Q_OBJECT

public:


	bool vpnConnectStatus;
    bool vpnMsgBarStatus;
	bool verWifiUpdateState;
	bool verLteUpdateState;
	bool verThreeGUpdateState;
	U_UINT32 previousErrorCode;
	U_UINT32 previousInfoCode;
    CMinfo  mCMInfo;
    CMinfo  mCMInfoForEvent;
    CMinfo  m_CMmodemStats;
    Ras_Details_s rasProfDetails;
    NMErrorInfo errorMsgDisp;
        QString         disp;
        QString     lteVersion;
		QString     wifiVersion;
		QString     threeGVersion;
        QString uCMVersion;
      MinimumView        *minView;
     DetailedView       *detView;
     int persistNoOfWifiNetworks;
    explicit Uconnect();
    ~Uconnect();
    void init();
    void searchbuttonHandler();
    void StateUpdate();
    void connectToServer();
    void sigIconUpdate();
    U_UINT32 GetConfigParam(U_UINT32 param);
    U_DOUBLE GetFltConfigParam(U_UINT32 param);
    U_SINT32 GetSigConfigParam(U_UINT32 param);
    void SetConfigParams(U_UINT32 param, U_UINT32 value);
    U_STRING GetStrConfigParam(U_UINT32 param);
    void GetNetworkSearchStatus();
    void wificonnectSetting(bool connected);
     void wifidisconnectSetting(bool disconnected);
     void wifiPasswordReqHandle(char* ssid);
     void WifiPasswordGot(string password,string ssid);
	 void WifiPasswordCancelled();
    void connectbuttonDetachHandler();
    void WifiDisConnectHandler();
    void connectbuttonAttachHandler();
    void setBearerParameter(U_UINT32 techId,Set_NM_Param paramId,string valStr);
    void toggleArbitrationMode(bool autoMode);
	void sendLteStatsRequest();
    void calculateSignalStrength(ETechSelect techId,float sigval);
	void updateVersion(ETechSelect techId,string version);
    void sendUSSDReq(string req);


    void setSearchResults(NetSearch_Result_s *srch_ptr)
    {
        m_tNetSearchResults = srch_ptr;
    }

    void uconnScanResponseHandler(UEvent *evt_p);
    void uconnGuiLogEventHandler(UEvent *evt_p);
    void uconnGuiLogLevelEventHandler(UEvent *evt_p);
    void uconnGuiConfigModeEventHandler(UEvent *evt_p);
    void uconnGuiServerLoggingEventHandler(UEvent *evt_p);
    void uconnServerFSMStateEventHandler(UEvent *evt_p);
    void uconnSystemEventHandler(UEvent *evt_p);
    void setServerConnStatus(bool connStatus);
    void customEvent(QEvent *event);
    void updateValues();
    void UpdateColumnValues(QTreeWidget*& pWidget, U_UINT8*& pCurrPointer, int& nCurrIndex);
    void AddWidgetItem(QTreeWidgetItem*& pWidget, QString strName, U_UINT32 u32Value);
    void ModemStatParams();
    void MeasStatParams();
    void WifiScanHandler();
    void WifiConnectHandler(QString ssid);
    void BearerEnabledHandler(ETechSelect techId,bool enabled);
    void updateConnectionChangeInGui();
    void handleWifiScanResponse(NetworkListEncoded& wifiNetworkList);
    void wifiPrefHandler(UEvent* evt);
    void showPreferredWifiNetworks(string encodedSsids);
    void wifiStatsRspHandler(UEvent* evt);
    void showWifiStats(string encodedStats);
    void handleConnectStatusEvt(UEvent* evt);
    void handleWifiPasswordReq(string ssid);
    void setNMParam();
    void threeGStatsHandler(UEvent* evt);
    void handle3GStatsRsp(string stats);
    void handleShowNMSettingsEvt(UEvent* evt);
    void updateNMSettings();
    void updateEnabledTechList(U_UINT32* enabledTechs);
    void updateTechPriority(UEvent* evt);
    void handleEnabledTechListEvt(UEvent* evt);
    void addTechsToPriorityList();
    void showInitialTechsInGui();
    void handleNMStateChangeEvt(UEvent* evt);
    void handleThreeGNameDisplay(UEvent* evt);
    void updateGuiOnNMStateChange(NMStateChange stateChange);
    void getTechParams(ETechSelect techId,Get_NM_Param paramId);
    void getTechParamsBasedOnState(ETechSelect techId);
	void getTechStats(ETechSelect techId);
    void handleGetParamRspHandler(UEvent* evt);
    void handleWifiDisabledNotification();
    void showWifiDisabledNotification();
	void showWifiEnabledNotification();

    void handleNMErrorDisplay(UEvent* evt);

    void showInitialRasSettings(Ras_Details_s& rasDetails);
    
    void searchTechNetworks(ETechSelect techId);
    void updateParamGotInGui(NMGetParam getParam);
	void handleBearerMngrModeChange(string param);

    void displayConnectFailureInGui(ETechSelect techId,string ssid);

   //
    //void write_to_csv(FILE *fp, int value);
   //void write_title_csv(FILE *fp, char* title);
    //void Append_CSV_title();
    void createTrayAction();
    void createTray();
    void DebugUpdateStatValues(QTreeWidgetItem*& pWidget, U_UINT32 childPos, U_UINT32 subchildPos,bool subChildFlag, U_UINT32 u32Value);
    void DebugUpdateStatValues(QTreeWidgetItem*& pWidget, U_UINT32 childPos, U_UINT32 subchildPos,bool subChildFlag, U_SINT32 s32Value);
    void progressUpdate();
    void DisconnectSetting();
    void UpdateIMSRegStatus();
    void fileloggingHandle();
    void  startRefreshTimer(int val);
    bool  stopRefreshTimer();
        bool autoMode;

    bool           currentView;
    bool isEditRefreshChecked;

    void switchView();

       // void BuildInfoStatWidget();
        void graph();
       void PlotGraph();
        Config_Param_s mCMCfgInfo;
        U_UINT32 mServerFSMState;

    //QSystemTrayIcon *trayIcon;
    bool            bFirstTimeSearchButtonClicked;
    bool    mbConnectedToServer;
    bool connectState;
#ifdef WIN32
    bool winEvent(MSG * msg, long * result);

                HDEVNOTIFY   hDevNotify;
#endif
                bool doRegisterForDevNotification();
    void sendEvent(UEvent* evt);
    void enableDisableTech(ETechSelect techId,bool enable);
    void techConfigModeChanged(ETechSelect techId,EBearerMode configMode);
    void techPriorityChanged(ETechSelect techId,U_UINT32* priority);
    void techNetSearch(ETechSelect techId);
    void setNMParam(ETechSelect techId,Set_NM_Param param_id,string param);
    void connectToWifiWithPassword(string password);
    //copied from abs-dal release
    void settingForWifiOnDetView();
    void settingForLteOnDetView();
    void settingForThreeGOnDetView();
    //void onwificlickedShowTab();

    void connectResponseHandler(UNMConnectStatus* connectResponse);
    void connectRequestHandler();
    void techPriorityChangedHandler(U_UINT32 techId,U_UINT32 priorityList[]);
    void minConnectButtonHandler(ETechSelect techId,bool connectionStatus);

    void setConnectButtonForAutoMode(bool connectButtonState);


    void getEnabledTechList();
    void getPriorityTechList();
    void uconnStatsMessageHandler(UEvent *evt_p);
    void uconnMeasStatsHandler(UEvent* evt);
    void getVersionInformation();

    void getEnabledTechsWithPriority();
    void getInitialRasProfile();
    void updateGuiNMnetworkName();
    void clearThreeGNetworksDisplay();
	void initialVersionSetting();
    void callSetContextOnMinConnectButton(bool mode);
    void setSettingsOnMinview(bool view,ETechSelect techId);
    void setSettingsOnMinviewForGeneral();
    void setRasProfile(Ras_Details_s* details);
    void handleInitialRasProf(Ras_Details_s details);
    void displayErrorMessage(NMErrorInfo errorInfo);
    //vpn
    void getVpnProfList();
    void getVpnClientList();
    void vpnConnect(QString profToCnt );
    void vpnDisconnect();
	string vpnProfiles;
	void handleVpnProfList(UEvent* evt);
	void handleVpnCliList(UEvent* evt);
    void handleVpnConnect(UEvent* evt);
	void handleVpnDisconnect();
    void callMsgBarMinview();

private:
 U_SINT32        secs;
    U_SINT32        sec;
    U_SINT32        min;
    U_SINT32        hour;
    U_SINT32        back_s;
    U_SINT32        back_m;
    U_SINT32        milisec;

    bool timerRun;
bool ok;
    void createActions();
    void createTrayIcon();
    void setIcon();

    QString ApnName;

    U_STRING apnName;

    NMSettings nmSettings;

    vector <NMStateChange> nmStateChange;

    NMGetParam nmGetParam;







    bool bStatWidgetInit ;

    IMS_param iMSInfo;
    U_UINT32 m_nstat;
    float m_nfltstat;
    U_SINT32 m_nSigstat;
    U_STRING m_nStrstat;

    U_INT16 mservPort;
    U_SINT8 *mservIpAddr_p;

    U_UINT32 mDeviceCurrentState;
    uconnSAPIwrapErr_t  m_tInitStatus;
    NetSearch_Result_s* m_tNetSearchResults;
    U_UINT32 u32SignBit;
    U_UINT32 u32IntegerBits;
    U_UINT32 u32FractionBits;

    QMenu           *fileMenu;
    QMenu           *viewMenu;
    QMenu           *toolsMenu;
    QMenu           *optionsMenu;
    QMenu           *helpMenu;
    QAction         *DebugtoolAct;

    QTimer          *StatUpdateTimer;
    QTimer          *timer;
    QTimer          *periodicUpdateTimer;
    QString          statdata;

    QString          statdataEvent;
    QString          tNetworkList;

     QString          DetailedViewstr;
     QString          secStr;
     QString          mins1 ;
     QString          hours1;
    QStatusBar      *statusBar;
    QLabel          *upLinkText;
    QLabel          *downLinkText;

    QLabel          *stLTE;
    //QLabel          *stNetName;
    QLabel          *downLink;
    QLabel          *upLink;
QTimer          *m_tRefreshTimer;
   float             RSSi;
     float           CINr;

	double Ant0;
    double Ant1;
    ofstream        outFile;
    QString         serverLogName;
    QString 		stat;
    bool serverLoggingEnabled;
    




    //void createMenus();
    //void createAction();
    //void setTrayIcon();
    //void createStatus();
    void getConfigValuesFromServer();
    void deviceStateUpdate(U_UINT32 deviceState);
	int getRefreshTimer();
    void getLinkFrequency(U_UINT32 statValue, bool isDownlinkEarFcn, float &linkFrequency);
    double signedToFloat(unsigned int u32Value, unsigned int u32SignBit, unsigned int u32IntegerBits, unsigned int u32FractionBits);
    void getInitTechConfiguration();
    void savePriorityToTechDetails();
    void getEachTechState();

#if 0
void on_TFtimerEdit_editingFinished();
    void on_T2timerEdit_editingFinished();
    void on_T1timerEdit_editingFinished();
    void on_Testmode_Disable_clicked(bool checked);
    void on_Testmode_Enable_clicked(bool checked);
    //void on_TFtimerEdit_returnPressed();
   // void on_T2timerEdit_returnPressed();
    //void on_T1timerEdit_returnPressed();
    void on_SOI_Off_clicked(bool checked);
    void on_SOI_On_clicked(bool checked);
    void on_SmsFormattOff_clicked(bool checked);
    void on_SmsFormattOn_clicked(bool checked);
    void on_smsWriteUiccOff_clicked(bool checked);
    void on_smsWriteUiccOn_clicked(bool checked);
#endif

private Q_SLOTS:

    //void on_m_editRefreshInterval_textChanged(QString );

    void showTime();
    void periodicUpdateThread();



public:

    ConnectionChangeParams connectionChangeParams;
    NetworkListEncoded wifiNetworkList;

private:



};


#endif // UCONNECT_H
