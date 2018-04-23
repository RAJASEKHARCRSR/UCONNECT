#ifndef MINVIEW_H
#define MINVIEW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QAction>
#include <QDebug>
#include <QEvent>
#include <QMessageBox>
#include <QDesktopServices>
#include <QtCore/QTimeLine>
#include "uconn_Event.h"
#include "UCommonFunctions.h"

namespace Ui {
    class MinimumView;
}

struct SigStrength
{
    int imageNumb;
    QString imageString;
    QLabel* label;

};

class MinimumView : public QMainWindow
{
    Q_OBJECT

public:

    SigStrength sigStrength[3];

    bool sigLabel1;
    bool sigLabel2;
    bool sigLabel3;
	QString sigStrLte;
	QString sigStrWifi;
	QString sigStrThreeG;
    explicit MinimumView(QMainWindow *parent = 0);
    ~MinimumView();
    void*           pUConnect;
    bool settingInfoState;
    ETechSelect currentTech;
    bool clearWifiCombo;
	string accessTypeName;
    QSystemTrayIcon *trayIcon;
    QAction         *minimizeAction;
    QAction         *maximizeAction;
    QAction         *restoreAction;
    QAction         *quitAction;
    QMenu *trayIconMenu;
    QAction *open;
    QAction *close;
    QAction *minimize;
    QString timeLabel;
    QLabel          *stNetValue;
    QTimeLine       *timeLine;
    QTimeLine       *frameAnimationTimeLineLte;
    QTimeLine       *frameAnimationTimeLineWifi;
    QTimeLine       *frameAnimationTimeLine3G;
    void clearWifiNetworksDisplay();
    void setApnName(QString Apnname,QString Duration);
    void minuplinkSetting(float uplink_val);
    void mindownlinkSetting(float downlinkValue);
    void sigIconupdateMinviewSetting(int imageNumber,ETechSelect techId);
    void  minUI_deviceStateUpdate(int deviceState);
    //void     trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void     closeEvent(QCloseEvent *event);
    void     createActions();
    void     createTrayIcon();
    void     detStateUpdate();
    void     setIcon();
    void     init(void *parent);
    void DisconnectSetting();
    void custEvents(int event);
    void custConnectedEventSetting();
    void custidleEnableEventSetting();

    void setContextForConnectButton(bool connectionStatus);
     void wifinetworkDisplay(QString networkName);

     void updateConnectionState(ETechSelect techId,bool connectStatus);
     void showDropDownForWifi();
     void showDropDownForLte();
     void showDropDownFor3G();
     void showWifiStats(string stats);
    void show3gStats(string stats);
    void updateMinNMSettings(ETechSelect techId,bool state);
    void setConnectButtonMinviewForAutoMode(bool connectButtonState);
    void hideDisconnectedTechItems(ETechSelect techId);
    void hide3gItems();
    void hideWifiItems();
    void hide4gItems();

    void show3gItems();
    void showWifiItems();
    void show4gItems();
    void showStatusMsg(QString statusMsg);

    void updateOnNMStateChange(ETechSelect techId,ENMStateForBM state);
    void getReadySettings(ETechSelect techId);
    void updateNetworkName(string threeGName);
	void updateAccessType(string accessType);
    void settingServerNotThere();
    void settingServerThere();
    void clearThreeGNetworksDisplay();
    void showWifiNetworkName(string wifiname);
    void showMinConnectStyleSheetManual(ETechSelect techId);
    void showMinConnectStyleSheetAuto(ETechSelect techId);
    void updateWifiMinviewLabels(bool userPressed);
    void updateThreeGMinviewLabels(bool userPressed);
    void updateLTEMinviewLabels(bool userPressed);
    void updateMessageBar();
    void showLTENetworkName(string apnName);
    void displayConnectFailedMin(ETechSelect techId,string param);
	bool checkIfAnyTechConnected();
    void handleWifiDisabledNotification();
	void handleWifiEnabledNotification();
    void displayErrorInfoMsgs(string errorStr,ENM_Error errorType,ETechSelect techId);
    void showVersionOnMinview(QString uCMVersion);
    void vpnDisconnectMsg();
private:
    Ui::MinimumView *ui;
private Q_SLOTS:
    void on_versionPushButton_clicked();
    void on_threeGInfoDown_clicked();
    void on_threeGInfoUp_clicked();
    void on_onselect3gPushButton_clicked();
    void on_lte4gInfoDown_clicked();
    void on_lte4gInfoUp_clicked();
    void on_wifiInfoDown_clicked();
    void on_wifiInfoUp_clicked();
    void on_wifiComboBox_activated(QString );
    void on_onselectWiredPushButton_clicked();
    void on_onselectLtePushButton_clicked();

    void on_onselectwifiPushButton_clicked();
    void on_detail_pushButton_clicked();
   void on_help_pushButton_clicked();
    void on_minconnectButton_clicked();
    //void on_info_pushButton_clicked();
    void trayIconClicked(QSystemTrayIcon::ActivationReason);
    void appQuit();
    void animatedDetResize(int height);
    void animatedLTEResize(int height);
    void animated3GResize(int height);
    void animatedWifiResize(int height);

};


#endif // MINVIEW_H
