#ifndef DETAILEDVIEW_H
#define DETAILEDVIEW_H

#include <vector>
#include <QDockWidget>

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
#include <QTableWidget>
#include <QTableWidgetItem>
#include <qcheckbox.h>

#include <QSharedMemory>
#include <QDesktopServices>
#include <QProcess>
#include <QUrl>
#include <QDir>
#include <QInputDialog>
#include <QIcon>

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
#include <QListWidgetItem>
#include <QSystemTrayIcon>
#include <QInputDialog>
#include "CMGraphStats.h"
#include "UTypes.h"
#include "uconn_OSAL.h"
#include "uconn_Event.h"
#include "uconn_ServerAPI.h"
#include "uconn_SAPIwrapper.h"

namespace Ui {
    class DetailedView;
}

typedef struct EncodedStats {

        int level :3;
        int valuePresent: 1;
        int rangeFlag: 2;
        int doNotDisplay: 1;
        int reserved: 1;


}EncodedStats_s;



struct DetailedViewTabButton {

    QPushButton* button;
    ETechSelect tabId;
    bool selected;
    QCheckBox* checkBox;
};

typedef struct genInfo{

    std::string Apn;
    std::string Duration;
    std::string ipAddress;

}genInfo_s;

typedef struct StatDisplayInfo {
    string statNameStr;
    string valueStr;
    EncodedStats_s encodedStats;
} StatDisplayInfo_s;

class DetailedView : public QFrame
{
    Q_OBJECT

public:

    QTimer* techPreferTimer;

    bool applyPressed;
    bool lteChecked;
    bool wifiChecked;
    bool _3GChecked;

    bool settinggenState;
    bool setting3gState;
    bool setting4gState;
    bool settingWifiState;
    bool settingTeatherState;
    bool settingVpnState;

    bool singleUssdResponse;
    vector<DetailedViewTabButton> tabButtons;


        genInfo_s gen;
    QSystemTrayIcon *trayIcon;
    QAction         *minimizeAction;
    QAction         *maximizeAction;
    QAction         *restoreAction;
    QAction         *quitAction;
    QMenu *trayIconMenu;
    QAction *open;
    QAction *close;
    QAction *minimize;
    QLabel          *stMsg;
    QLabel          *stSig;
    QLabel          *stThreeG;
    QString         ApnName;
    //bool            mbConnectedToServer;
    FILE            *fileptr;
    void*           pUConnect;
    CMGraphStats    *statsGraph;
    bool            timerRun;
    //bool MCSflag;
    bool csvflag;
    bool            bStatWidgetInitCheck;
    QTimer          *timer;
    QTimer          *m_tRefreshTimer;
        QString Searchnetwork;
    bool serverLoggingEnabled;

    QString          genStr;
    QString          downStr;
    QLabel          *stNetValue;

    QMenu           *fileMenu;
    QMenu           *viewMenu;
    QMenu           *toolsMenu;
    QMenu           *optionsMenu;
    QMenu           *helpMenu;
    QAction         *DebugtoolAct;
    //QLabel          *stMsg;

    QTimer          *StatUpdateTimer;
    //QTimer          *timer;
    QTimer          *periodicUpdateTimer;
    QString          statdata;

    QString          statdataEvent;
    QString          tNetworkList;

     QString          str;
     QString          secStr;
     QString          mins1 ;
     QString          hours1;
    QStatusBar      *statusBar;
    QLabel          *upLinkText;
    QLabel          *downLinkText;

    QLabel          *stLTE;
    QLabel          *stWIFI;
    //QLabel          *stNetName;
    QLabel          *downLink;
    QLabel          *upLink;
    bool            bFirstTimeSearchButtonClicked;
//QTimer          *m_tRefreshTimer;
    int             RSSi;
    int             CINr;
    //FILE *fileptr;
        ///FILE *fp;
       // QFile *f;
        double Ant0;
    double Ant1;
    ofstream        outFile;
    QString         serverLogName;
    QString 		stat;

    bool saveApplyButtonMessage();
    bool iswifiConnectclicked ;
    bool iswifiEnabled ;
    bool isScanned;
    bool isNetworkSelected;
    bool isGeneralCurrentTab;

    void     init(void *parent);
	//void ifconnectedSetting(bool is_connected);
    //std::vector<std::string>& tokeniseInformation(std::string &rspStr, std::vector<std::string>& vStr, char ch);
    void     closeEvent(QCloseEvent *event);
    void     createActions();
    void     createTrayIcon();
    void     setIcon();

    void allClicksHandler();
    explicit DetailedView(QWidget *parent = 0);
    ~DetailedView();
    void modemUpdate();
    void detdownlinkSetting(float downlinkValue);
    void DisplayApnTitle();
    void createStatus();
    void PlotGraph();
    void progressbarSettings(float rssiValue, float cinrValue,float strTemp);
    //void write_title_csv(FILE *fp, char* title);
    //void Append_CSV_title();
    //void BuildInfoStatWidget();
    void DisconnectSetting();
    void modemStateUpdate();
    void uplinkSetting(float uplink_val);
    void graph();
    void  UI_deviceStateUpdate(U_UINT32 deviceState);
    void connectErrortoServer();
    void connectedToServer();
    void detStateUpdate();
    void logLevel();
    void enableLog();
    void initSetting();
    void  startRefreshTimer(int val);
    bool  stopRefreshTimer();
        bool timerstate;
    //void on_radioDebug_clicked(bool checked);
    void sigIconupdateDetviewSetting(int imageNumber);

     void setApnname(QString apn,QString mcc,QString mnc, QString duration);
    void debugTool();

    void write_title_csv();
    void write_csv(QString str, int valORhead);

    void write_title_csv(FILE *fp, std::string value);
    void showtimeSetting();
   // void UpdateInfoTree();

    void DebugUpdateStatValues(QTreeWidgetItem*& pWidget, U_UINT32 childPos, U_UINT32 subchildPos,bool subChildFlag, U_SINT32 s32Value);
    void DebugUpdateStatValues(QTreeWidgetItem*& pWidget, U_UINT32 childPos, U_UINT32 subchildPos,bool subChildFlag, U_UINT32 u32Value);
    void AddWidgetItem(QTreeWidgetItem*& pWidget, QString strName, U_UINT32 u32Value);
    void custidleEnableEventSetting();
   void custSearchEventSetting();
   void custConnectingEventSetting();
   void custConnectedEventSetting();
   void custDisconnectedEventSetting();
   void custDisableEventSetting();
   void custEvents(int event);
   void wifiSetConnect( bool connectStatus);
    void wifiSetDisconnect( bool disconnectStatus);
    void wifiPasswordReq(char* ssid);
   void searchcompletelistHandler(int col,QString netName);
   void wifinetworkDisplay(QString networkName,QString signalQuality,QString connected,int i);
   void handleWifiConnectionStatusResponse(NM_Connect_Status connectStatus);
   void displayWifiPreferredNetworks(string ssids);
   void handleTechPreferTimer();
   void setTechPriority();
   void updateTechPreference();
//void BuildInfoStatWidget();

   void makeButtonTransparentExpect(ETechSelect tabId);
   void makeGenSettingsSelected();

   void updateConnectionState(ETechSelect techId,bool connectStatus);

    int getRefreshTimer();

   std::vector<std::string>& tokeniseInformation(std::string &rspStr, std::vector<std::string>& vStr, char ch);
   void makeTabChecked(ETechSelect techId);
   void makeWifiSelected();
   void reqPasswordPop(QString ssid);
   void displayAllWifiNetworks(string wifiNetworkList);
   void show3gStats(string stats);
   void updateDetNMSettings(ETechSelect techId,bool state);
   void addTechsToPriorityList();
   void initializeTabCheckButtons();
   void updateOnNMStateChange(ETechSelect techId,ENMStateForBM state);
   void handleWifiDisabledNotification();

    void  setLogFileName();
    void settingServerNotThere();

    QListWidgetItem* item;
    QTableWidgetItem *item1 ;
    QTableWidgetItem *item2;

    QTableWidgetItem *item3 ;
    QTableWidgetItem *item4;
    QString statString;
    QString statValue;

    QString measStatString;
    QString measValueString;

    bool isWifiConnected;
    bool isLteConnected;
   void WificreateStatus();
   string networkSelected;
void minviewLtePressed();
void minviewWifiPressed();
void setwifiposition();
void minviewThreeGPressed();
void ThreeGcreateStatus();

void updateVisibilityOfPriorityWidgets();
void settingServerThere();

void showWifiAdapterName(string wifiAdapter);
//void updateMessageBar();
void handleWifiPreferredRsp(string encodedSsids);
void updateRssiProgressBar(string rssi);
void updateCinrProgressBar(string cinr);
void statisticUpdate();
void displayUssdRespose(string val);
void setContextForAutoManualButton();
void wifiPushButtonPressed();
void ltePushButtonPressed();
void threeGPushButtonPressed();
void displayRasProfile();
/*Vpn functions*/
void showVpnProfileslist(string profLst);
void showVpnClientlist(string cliLst);
void showVpnConnectSettings(int connectRsp);
void showVpnDisconnectSettings();
void vpnAutoConnect();

private:
    Ui::DetailedView *ui;

    void formatTheAttribute(QString &statString,int level);
    void getDecodedStrings(StatDisplayInfo& statDisplayInfo,string &rowStr);
    void updateAltairStatsToCsvFile(StatDisplayInfo_s& statDisplayInfo,bool bHeaderUpdate,QString &csvHeader,QString &csvValue);
    void updateAltairStatsWidgetInGUI(StatDisplayInfo_s& statDisplayInfo,int row);
    void updateAltairMeasStatsWidgetInGUI(StatDisplayInfo_s& statDisplayInfo,int row);
    void processAltairStats(string &encodedStr);
    void processAltairMeasStats(string &encodedStr);
    void writeMeasStatsToCsvFile(string& statVal,QString& csvHeader,QString& csvValue);

    void processBroadcomStats(string &encodedStr);
    void checkRangeSetBackground(EncodedStats_s encodedStats,QTableWidgetItem& item1,QTableWidgetItem& item2);

    void decodeStatsInfo(string,EncodedStats_s*);

    bool stringToHex(int& t,const string& s,ios_base& (*f)(ios_base&));

    void encodeGenParams(std::string& encodeStr);

    //void handleTechCheckBoxClicked(ETechSelect techId,bool enable);
    //void handleTechConfigModeChanged(ETechSelect techId,QString configModeText);
    //void handleTechPriorityChanged(ETechSelect techId,QString configModeText);

    void handleUpPreferenceClicked(QListWidget*);
    void handleDownPreferenceClicked(QListWidget*);
    void sendWifiPreferenceList();

    void updateTechPriorityList(ETechSelect techId,bool enabled);


    DetailedViewTabButton& getTabByTechId(ETechSelect techId);




    string preferredWifiSsids;
    string wifiNetworkList;


    int measType;

private Q_SLOTS:
         //void DebugUpdateStat();
    
        //void on_wifiPasswordEdit_editingFinished();


    void on_applySettingsButton_clicked();
    void on_lteCheckBox_clicked(bool checked);
    void on_wifiCheckBox_clicked(bool checked);
    void on_preferredNetworkCheck_clicked(bool checked);
    void on_wifiNetworkCheck_clicked(bool checked);
    void on_wifiPreferredList_itemDoubleClicked(QListWidgetItem* item);
    void on_wifiDownPushButton_clicked();
    void on_wifiUpPushButton_clicked();
    void on_connectState_clicked();
    void on_techDownPushButton_clicked();
    void on_techUpPushButton_clicked();
    void on_vpnPushButton_clicked();
    void on_tetheringPushButton_clicked();
    void on_ltePushButton_clicked();
    void on_wifiPushButton_clicked();
    void on__3gPushButton_clicked();
    void on_generalPushButton_clicked();
    void on_tetheringCheckBox_clicked();

    //void on_wifiCheckBox_clicked();
    void on__3GCheckBox_clicked();
   // void on_wifinetworkList_cellDoubleClicked(int row, int column);

    //void on_apntableWidget_clicked(QModelIndex index);
    //void on_wifi_scanButton_clicked();
    //void on__3g_searchButton_clicked();
    //void on_Lte_searchButton_clicked();
    //void on_wifi_priorityCombo_currentIndexChanged(QString );
    //void on__3g_priorityCombo_currentIndexChanged(QString );
    //void on_Lte_priorityCombo_currentIndexChanged(QString ); //check UCM
    void on_wificomboBox_currentIndexChanged(QString );
    void on__3gcomboBox_currentIndexChanged(QString );
    void on_ltecomboBox_currentIndexChanged(QString );
    void on_wifi_checkBox_clicked();
    void on__3g_checkBox_clicked();
    void on_Lte_checkBox_clicked();
	
	//copied from abs_dal release

    void on_PasswordEdit_returnPressed();
    void on_wifiConnect_clicked();
    void on_wifiScan_clicked(bool checked);
	//end
	
    void BuildStatWidget();
    //void on_helpButton_2_clicked();
    void on_radioManual_clicked();
    void on_radioAuto_clicked();
    void on_radioError_clicked();
    void on_radioInfo_clicked();
    void on_radioDebug_clicked();
   // void on_helpButton_clicked();
    //void on_versionButton_clicked();
    void on_searchButton_clicked();
    //void on_Refresh_checkBox_clicked(bool checked);
    void on_m_editRefreshInterval_currentIndexChanged(int index);
    void on_GetModStats_clicked();
    void on_fileLogginOption_clicked(bool checked);
    void trayIconClicked(QSystemTrayIcon::ActivationReason);
    void appQuit();


    void on_send_ussd_clicked();

    void on_rasProfSave_clicked();
	void on_vpnAutoCheck_clicked();
    void on_vpnButton_clicked();
};

#endif // DETAILEDVIEW_H
