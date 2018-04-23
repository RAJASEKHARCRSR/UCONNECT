/********************************************************************************
** Form generated from reading UI file 'detailedview.ui'
**
** Created: Tue Jul 31 10:42:59 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DETAILEDVIEW_H
#define UI_DETAILEDVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QListWidget>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DetailedView
{
public:
    QWidget *centralWidget;
    QTabWidget *uConnectTabWidget;
    QWidget *generalSetting;
    QLabel *connectSettingLabel;
    QListView *connectSettingListview;
    QLabel *techPriorityLabel;
    QListWidget *techPriorityListWidget;
    QLabel *label_3;
    QPushButton *techUpPushButton;
    QPushButton *techDownPushButton;
    QPushButton *connectState;
    QLabel *label_40;
    QPushButton *applySettingsButton;
    QCheckBox *fileLogginOption;
    QLineEdit *logFileName;
    QListView *listView_4;
    QLabel *label_36;
    QWidget *profiles;
    QLabel *label_6;
    QLabel *label_14;
    QLabel *label_16;
    QLabel *label_45;
    QLabel *label_46;
    QLineEdit *rasProfileName;
    QLineEdit *rasPhoneNum;
    QLineEdit *rasUserName;
    QLineEdit *rasPassword;
    QLineEdit *rasApn;
    QPushButton *rasProfSave;
    QWidget *threeGStatistics;
    QLabel *label_41;
    QTableWidget *threeGStatTableWidget;
    QLabel *label_42;
    QListView *listView_8;
    QProgressBar *rssiThreeGProgressBar;
    QProgressBar *cinrThreeGProgressBar;
    QLabel *label_43;
    QLabel *label_44;
    QLabel *threegRssiVal;
    QLabel *threegCinrVal;
    QWidget *debugTool;
    QPushButton *GetModStats;
    QLabel *stats_bg;
    QTableWidget *statTableWidget;
    QLabel *label_38;
    QListView *listView_6;
    QLabel *rssiValueEdit;
    QLabel *cinrValueEdit;
    QProgressBar *RSSIProgressBar;
    QProgressBar *CINRProgressBar;
    QLabel *rssi;
    QLabel *cinr;
    QListView *listView_7;
    QLabel *refresh;
    QComboBox *m_editRefreshInterval;
    QLabel *second;
    QLabel *label_39;
    QWidget *configure;
    QLabel *conf_bg;
    QLabel *label_34;
    QListView *listView_2;
    QRadioButton *radioAuto;
    QRadioButton *radioManual;
    QLabel *label_35;
    QListView *listView_3;
    QRadioButton *radioDebug;
    QRadioButton *radioInfo;
    QRadioButton *radioError;
    QWidget *tab;
    QLabel *gen_bg;
    QTableWidget *measTableWidget;
    QWidget *vBox;
    QWidget *wifiSetting;
    QListView *connectSettingListview_2;
    QLabel *wifiAdapterSettingLabel;
    QCheckBox *wifiNetworkCheck;
    QCheckBox *preferredNetworkCheck;
    QComboBox *wifiAdapterComboBox;
    QLabel *wifiNetworkListLabel;
    QListWidget *wifiPreferredList;
    QLabel *label_33;
    QPushButton *wifiUpPushButton;
    QPushButton *wifiDownPushButton;
    QWidget *WIFI;
    QLabel *wifi_bg;
    QLabel *wifiNetworks;
    QTableWidget *wifinetworkList;
    QPushButton *wifiScan;
    QLineEdit *PasswordEdit;
    QLabel *passwordLabel;
    QLabel *networkSsid;
    QLabel *wifi_label_test;
    QWidget *vpn;
    QLabel *label_7;
    QLabel *label_37;
    QComboBox *vpnClientListCombo;
    QComboBox *vpnProfileListCombo;
    QCheckBox *vpnAutoCheck;
    QListView *listView_5;
    QPushButton *vpnButton;
    QWidget *network;
    QLabel *baseStationLogo;
    QPushButton *searchButton;
    QLabel *lteLabel;
    QLabel *net_bg;
    QLabel *label_11;
    QLabel *label_18;
    QTableWidget *apntableWidget;
    QWidget *tab_2;
    QLabel *label_19;
    QRadioButton *Testmode_Disable;
    QRadioButton *Testmode_Enable;
    QGroupBox *SMSgroupBox;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QRadioButton *smsWriteUiccOn;
    QRadioButton *smsWriteUiccOff;
    QRadioButton *SmsFormattOn;
    QRadioButton *SmsFormattOff;
    QRadioButton *SOI_On;
    QRadioButton *SOI_Off;
    QListWidget *IMSRegStatus;
    QGroupBox *TimergroupBox;
    QLineEdit *T1timerEdit;
    QLineEdit *T2timerEdit;
    QLineEdit *TFtimerEdit;
    QLabel *label_21;
    QLabel *label_22;
    QLabel *label_23;
    QLabel *label_24;
    QLabel *label_25;
    QLabel *label_26;
    QLabel *label_27;
    QToolBox *messageToolBox;
    QWidget *page;
    QLineEdit *lineEdit_5;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *page_2;
    QLabel *ims_bg;
    QWidget *ussd_tab;
    QLineEdit *ussd_request;
    QPushButton *send_ussd;
    QLabel *label_5;
    QListWidget *ussd_response;
    QLabel *NavigationBar;
    QLabel *label_15;
    QPushButton *pushButton_3;
    QListView *listView;
    QPushButton *_3gPushButton;
    QPushButton *wifiPushButton;
    QPushButton *ltePushButton;
    QPushButton *tetheringPushButton;
    QCheckBox *wifiCheckBox;
    QCheckBox *lteCheckBox;
    QCheckBox *tetheringCheckBox;
    QPushButton *vpnPushButton;
    QPushButton *generalPushButton;
    QCheckBox *_3GCheckBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_17;
    QLabel *label_20;
    QLabel *label_28;
    QLabel *label_29;
    QLabel *label_30;
    QLabel *label_31;
    QLabel *label_32;
    QLabel *demark;

    void setupUi(QFrame *DetailedView)
    {
        if (DetailedView->objectName().isEmpty())
            DetailedView->setObjectName(QString::fromUtf8("DetailedView"));
        DetailedView->resize(600, 400);
        DetailedView->setMinimumSize(QSize(600, 400));
        DetailedView->setMaximumSize(QSize(600, 400));
        DetailedView->setLayoutDirection(Qt::LeftToRight);
        DetailedView->setFrameShape(QFrame::StyledPanel);
        DetailedView->setFrameShadow(QFrame::Sunken);
        centralWidget = new QWidget(DetailedView);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setGeometry(QRect(0, 0, 779, 551));
        uConnectTabWidget = new QTabWidget(centralWidget);
        uConnectTabWidget->setObjectName(QString::fromUtf8("uConnectTabWidget"));
        uConnectTabWidget->setGeometry(QRect(200, 10, 321, 431));
        uConnectTabWidget->setStyleSheet(QString::fromUtf8("QTabBar::tab{height: 20px;width: 100px;}"));
        uConnectTabWidget->setTabShape(QTabWidget::Rounded);
        uConnectTabWidget->setIconSize(QSize(25, 27));
        uConnectTabWidget->setTabsClosable(false);
        generalSetting = new QWidget();
        generalSetting->setObjectName(QString::fromUtf8("generalSetting"));
        connectSettingLabel = new QLabel(generalSetting);
        connectSettingLabel->setObjectName(QString::fromUtf8("connectSettingLabel"));
        connectSettingLabel->setGeometry(QRect(9, 24, 260, 31));
        connectSettingLabel->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:1, stop:0 rgba(48, 48, 48, 255), stop:1 rgba(0, 0, 0, 255));\n"
"font: 75 10pt \"Trebuchet MS\";\n"
"color:#f4f4f4;"));
        connectSettingListview = new QListView(generalSetting);
        connectSettingListview->setObjectName(QString::fromUtf8("connectSettingListview"));
        connectSettingListview->setGeometry(QRect(9, 52, 260, 55));
        connectSettingListview->setStyleSheet(QString::fromUtf8("background-color:#ffffff;"));
        techPriorityLabel = new QLabel(generalSetting);
        techPriorityLabel->setObjectName(QString::fromUtf8("techPriorityLabel"));
        techPriorityLabel->setGeometry(QRect(8, 122, 260, 31));
        techPriorityLabel->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:1, stop:0 rgba(48, 48, 48, 255), stop:1 rgba(0, 0, 0, 255));\n"
"font: 75 10pt \"Trebuchet MS\";\n"
"color:#f4f4f4;"));
        techPriorityListWidget = new QListWidget(generalSetting);
        techPriorityListWidget->setObjectName(QString::fromUtf8("techPriorityListWidget"));
        techPriorityListWidget->setGeometry(QRect(8, 152, 260, 111));
        techPriorityListWidget->setStyleSheet(QString::fromUtf8("background-color:#ffffff;"));
        label_3 = new QLabel(generalSetting);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(9, 253, 260, 21));
        label_3->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:0.988727, stop:0 rgba(232, 232, 232, 255), stop:1 rgba(205, 205, 205, 255));"));
        techUpPushButton = new QPushButton(generalSetting);
        techUpPushButton->setObjectName(QString::fromUtf8("techUpPushButton"));
        techUpPushButton->setGeometry(QRect(12, 252, 20, 20));
        techUpPushButton->setStyleSheet(QString::fromUtf8("background-image: url(:/upTech);\n"
"background-color: transparent;"));
        techDownPushButton = new QPushButton(generalSetting);
        techDownPushButton->setObjectName(QString::fromUtf8("techDownPushButton"));
        techDownPushButton->setGeometry(QRect(38, 252, 20, 20));
        techDownPushButton->setFocusPolicy(Qt::NoFocus);
        techDownPushButton->setStyleSheet(QString::fromUtf8("background-image: url(:/downTech);\n"
"background-color: transparent;"));
        connectState = new QPushButton(generalSetting);
        connectState->setObjectName(QString::fromUtf8("connectState"));
        connectState->setGeometry(QRect(196, 65, 70, 25));
        connectState->setStyleSheet(QString::fromUtf8("image: url(:/onButton);border:none;outline:none;\n"
"\n"
"\n"
""));
        connectState->setIconSize(QSize(10, 10));
        label_40 = new QLabel(generalSetting);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        label_40->setGeometry(QRect(22, 68, 170, 20));
        applySettingsButton = new QPushButton(generalSetting);
        applySettingsButton->setObjectName(QString::fromUtf8("applySettingsButton"));
        applySettingsButton->setGeometry(QRect(60, 248, 61, 30));
        applySettingsButton->setStyleSheet(QString::fromUtf8("border:none;outline:none;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ApplyButton"), QSize(), QIcon::Normal, QIcon::Off);
        applySettingsButton->setIcon(icon);
        applySettingsButton->setIconSize(QSize(100, 28));
        fileLogginOption = new QCheckBox(generalSetting);
        fileLogginOption->setObjectName(QString::fromUtf8("fileLogginOption"));
        fileLogginOption->setGeometry(QRect(146, 322, 110, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font.setPointSize(8);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        fileLogginOption->setFont(font);
        fileLogginOption->setStyleSheet(QString::fromUtf8("font: 8pt \"MS Shell Dlg 2\";"));
        logFileName = new QLineEdit(generalSetting);
        logFileName->setObjectName(QString::fromUtf8("logFileName"));
        logFileName->setGeometry(QRect(19, 328, 110, 20));
        listView_4 = new QListView(generalSetting);
        listView_4->setObjectName(QString::fromUtf8("listView_4"));
        listView_4->setGeometry(QRect(9, 311, 260, 51));
        label_36 = new QLabel(generalSetting);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setGeometry(QRect(9, 282, 260, 31));
        label_36->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:1, stop:0 rgba(48, 48, 48, 255), stop:1 rgba(0, 0, 0, 255));\n"
"font: 75 10pt \"Trebuchet MS\";\n"
"color:#f4f4f4;"));
        uConnectTabWidget->addTab(generalSetting, QString());
        connectSettingLabel->raise();
        connectSettingListview->raise();
        techPriorityLabel->raise();
        techPriorityListWidget->raise();
        label_3->raise();
        techUpPushButton->raise();
        techDownPushButton->raise();
        connectState->raise();
        label_40->raise();
        applySettingsButton->raise();
        listView_4->raise();
        label_36->raise();
        fileLogginOption->raise();
        logFileName->raise();
        profiles = new QWidget();
        profiles->setObjectName(QString::fromUtf8("profiles"));
        label_6 = new QLabel(profiles);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 30, 71, 16));
        label_14 = new QLabel(profiles);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(20, 80, 81, 16));
        label_16 = new QLabel(profiles);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(20, 130, 61, 16));
        label_45 = new QLabel(profiles);
        label_45->setObjectName(QString::fromUtf8("label_45"));
        label_45->setGeometry(QRect(20, 180, 61, 16));
        label_46 = new QLabel(profiles);
        label_46->setObjectName(QString::fromUtf8("label_46"));
        label_46->setGeometry(QRect(20, 230, 61, 16));
        rasProfileName = new QLineEdit(profiles);
        rasProfileName->setObjectName(QString::fromUtf8("rasProfileName"));
        rasProfileName->setGeometry(QRect(20, 50, 161, 20));
        rasPhoneNum = new QLineEdit(profiles);
        rasPhoneNum->setObjectName(QString::fromUtf8("rasPhoneNum"));
        rasPhoneNum->setGeometry(QRect(20, 100, 161, 20));
        rasUserName = new QLineEdit(profiles);
        rasUserName->setObjectName(QString::fromUtf8("rasUserName"));
        rasUserName->setGeometry(QRect(20, 150, 161, 20));
        rasPassword = new QLineEdit(profiles);
        rasPassword->setObjectName(QString::fromUtf8("rasPassword"));
        rasPassword->setGeometry(QRect(20, 200, 161, 20));
        rasApn = new QLineEdit(profiles);
        rasApn->setObjectName(QString::fromUtf8("rasApn"));
        rasApn->setGeometry(QRect(20, 250, 161, 20));
        rasProfSave = new QPushButton(profiles);
        rasProfSave->setObjectName(QString::fromUtf8("rasProfSave"));
        rasProfSave->setGeometry(QRect(20, 300, 75, 23));
        uConnectTabWidget->addTab(profiles, QString());
        threeGStatistics = new QWidget();
        threeGStatistics->setObjectName(QString::fromUtf8("threeGStatistics"));
        label_41 = new QLabel(threeGStatistics);
        label_41->setObjectName(QString::fromUtf8("label_41"));
        label_41->setGeometry(QRect(10, 176, 260, 21));
        label_41->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:1, stop:0 rgba(48, 48, 48, 255), stop:1 rgba(0, 0, 0, 255));\n"
"font: 75 10pt \"Trebuchet MS\";\n"
"color:#f4f4f4;"));
        threeGStatTableWidget = new QTableWidget(threeGStatistics);
        if (threeGStatTableWidget->columnCount() < 2)
            threeGStatTableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        threeGStatTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        if (threeGStatTableWidget->rowCount() < 3)
            threeGStatTableWidget->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        threeGStatTableWidget->setItem(0, 0, __qtablewidgetitem1);
        threeGStatTableWidget->setObjectName(QString::fromUtf8("threeGStatTableWidget"));
        threeGStatTableWidget->setGeometry(QRect(10, 197, 260, 141));
        threeGStatTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        threeGStatTableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
        threeGStatTableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
        threeGStatTableWidget->setShowGrid(false);
        threeGStatTableWidget->setRowCount(3);
        threeGStatTableWidget->setColumnCount(2);
        threeGStatTableWidget->horizontalHeader()->setVisible(false);
        threeGStatTableWidget->horizontalHeader()->setDefaultSectionSize(130);
        threeGStatTableWidget->horizontalHeader()->setMinimumSectionSize(100);
        threeGStatTableWidget->verticalHeader()->setVisible(false);
        threeGStatTableWidget->verticalHeader()->setDefaultSectionSize(40);
        threeGStatTableWidget->verticalHeader()->setMinimumSectionSize(12);
        label_42 = new QLabel(threeGStatistics);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        label_42->setGeometry(QRect(9, 31, 260, 21));
        label_42->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:1, stop:0 rgba(48, 48, 48, 255), stop:1 rgba(0, 0, 0, 255));\n"
"font: 75 10pt \"Trebuchet MS\";\n"
"color:#f4f4f4;"));
        listView_8 = new QListView(threeGStatistics);
        listView_8->setObjectName(QString::fromUtf8("listView_8"));
        listView_8->setGeometry(QRect(9, 51, 260, 91));
        rssiThreeGProgressBar = new QProgressBar(threeGStatistics);
        rssiThreeGProgressBar->setObjectName(QString::fromUtf8("rssiThreeGProgressBar"));
        rssiThreeGProgressBar->setGeometry(QRect(119, 61, 118, 23));
        rssiThreeGProgressBar->setValue(0);
        cinrThreeGProgressBar = new QProgressBar(threeGStatistics);
        cinrThreeGProgressBar->setObjectName(QString::fromUtf8("cinrThreeGProgressBar"));
        cinrThreeGProgressBar->setGeometry(QRect(118, 98, 118, 23));
        cinrThreeGProgressBar->setValue(0);
        label_43 = new QLabel(threeGStatistics);
        label_43->setObjectName(QString::fromUtf8("label_43"));
        label_43->setGeometry(QRect(40, 66, 46, 13));
        label_43->setStyleSheet(QString::fromUtf8("font: 8pt \"MS Shell Dlg 2\";"));
        label_44 = new QLabel(threeGStatistics);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setGeometry(QRect(39, 96, 46, 13));
        label_44->setStyleSheet(QString::fromUtf8("font: 8pt \"MS Shell Dlg 2\";"));
        threegRssiVal = new QLabel(threeGStatistics);
        threegRssiVal->setObjectName(QString::fromUtf8("threegRssiVal"));
        threegRssiVal->setGeometry(QRect(230, 67, 46, 20));
        threegCinrVal = new QLabel(threeGStatistics);
        threegCinrVal->setObjectName(QString::fromUtf8("threegCinrVal"));
        threegCinrVal->setGeometry(QRect(230, 101, 46, 16));
        uConnectTabWidget->addTab(threeGStatistics, QString());
        debugTool = new QWidget();
        debugTool->setObjectName(QString::fromUtf8("debugTool"));
        GetModStats = new QPushButton(debugTool);
        GetModStats->setObjectName(QString::fromUtf8("GetModStats"));
        GetModStats->setGeometry(QRect(730, 10, 61, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(9);
        GetModStats->setFont(font1);
        GetModStats->setStyleSheet(QString::fromUtf8("font: 75 10pt \"Lucida Grande\";\n"
"\n"
""));
        stats_bg = new QLabel(debugTool);
        stats_bg->setObjectName(QString::fromUtf8("stats_bg"));
        stats_bg->setGeometry(QRect(-10, 0, 811, 381));
        stats_bg->setStyleSheet(QString::fromUtf8(""));
        statTableWidget = new QTableWidget(debugTool);
        if (statTableWidget->columnCount() < 2)
            statTableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        statTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        statTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem3);
        if (statTableWidget->rowCount() < 91)
            statTableWidget->setRowCount(91);
        statTableWidget->setObjectName(QString::fromUtf8("statTableWidget"));
        statTableWidget->setGeometry(QRect(10, 180, 260, 171));
        statTableWidget->setAutoScroll(false);
        statTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        statTableWidget->setTabKeyNavigation(true);
        statTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        statTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        statTableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
        statTableWidget->setShowGrid(false);
        statTableWidget->setCornerButtonEnabled(true);
        statTableWidget->setRowCount(91);
        statTableWidget->setColumnCount(2);
        statTableWidget->horizontalHeader()->setVisible(false);
        statTableWidget->horizontalHeader()->setDefaultSectionSize(150);
        statTableWidget->verticalHeader()->setVisible(false);
        label_38 = new QLabel(debugTool);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        label_38->setGeometry(QRect(10, 10, 260, 20));
        label_38->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:1, stop:0 rgba(48, 48, 48, 255), stop:1 rgba(0, 0, 0, 255));\n"
"font: 75 10pt \"Trebuchet MS\";\n"
"color:#f4f4f4;"));
        listView_6 = new QListView(debugTool);
        listView_6->setObjectName(QString::fromUtf8("listView_6"));
        listView_6->setGeometry(QRect(10, 25, 260, 64));
        rssiValueEdit = new QLabel(debugTool);
        rssiValueEdit->setObjectName(QString::fromUtf8("rssiValueEdit"));
        rssiValueEdit->setGeometry(QRect(224, 34, 40, 16));
        cinrValueEdit = new QLabel(debugTool);
        cinrValueEdit->setObjectName(QString::fromUtf8("cinrValueEdit"));
        cinrValueEdit->setGeometry(QRect(224, 58, 40, 16));
        RSSIProgressBar = new QProgressBar(debugTool);
        RSSIProgressBar->setObjectName(QString::fromUtf8("RSSIProgressBar"));
        RSSIProgressBar->setGeometry(QRect(80, 31, 171, 20));
        RSSIProgressBar->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        RSSIProgressBar->setValue(0);
        CINRProgressBar = new QProgressBar(debugTool);
        CINRProgressBar->setObjectName(QString::fromUtf8("CINRProgressBar"));
        CINRProgressBar->setGeometry(QRect(80, 57, 170, 20));
        CINRProgressBar->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        CINRProgressBar->setValue(0);
        rssi = new QLabel(debugTool);
        rssi->setObjectName(QString::fromUtf8("rssi"));
        rssi->setGeometry(QRect(40, 30, 30, 20));
        rssi->setStyleSheet(QString::fromUtf8(""));
        rssi->setScaledContents(true);
        cinr = new QLabel(debugTool);
        cinr->setObjectName(QString::fromUtf8("cinr"));
        cinr->setGeometry(QRect(40, 56, 30, 21));
        cinr->setStyleSheet(QString::fromUtf8(""));
        cinr->setScaledContents(true);
        listView_7 = new QListView(debugTool);
        listView_7->setObjectName(QString::fromUtf8("listView_7"));
        listView_7->setGeometry(QRect(10, 120, 260, 41));
        refresh = new QLabel(debugTool);
        refresh->setObjectName(QString::fromUtf8("refresh"));
        refresh->setGeometry(QRect(60, 132, 81, 20));
        refresh->setStyleSheet(QString::fromUtf8(""));
        refresh->setScaledContents(true);
        m_editRefreshInterval = new QComboBox(debugTool);
        m_editRefreshInterval->setObjectName(QString::fromUtf8("m_editRefreshInterval"));
        m_editRefreshInterval->setGeometry(QRect(161, 132, 51, 20));
        second = new QLabel(debugTool);
        second->setObjectName(QString::fromUtf8("second"));
        second->setGeometry(QRect(216, 133, 20, 20));
        second->setStyleSheet(QString::fromUtf8(""));
        label_39 = new QLabel(debugTool);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setGeometry(QRect(10, 104, 260, 18));
        label_39->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:1, stop:0 rgba(48, 48, 48, 255), stop:1 rgba(0, 0, 0, 255));\n"
"font: 75 10pt \"Trebuchet MS\";\n"
"color:#f4f4f4;"));
        uConnectTabWidget->addTab(debugTool, QString());
        configure = new QWidget();
        configure->setObjectName(QString::fromUtf8("configure"));
        conf_bg = new QLabel(configure);
        conf_bg->setObjectName(QString::fromUtf8("conf_bg"));
        conf_bg->setGeometry(QRect(0, 30, 771, 421));
        conf_bg->setStyleSheet(QString::fromUtf8(""));
        label_34 = new QLabel(configure);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(11, 31, 240, 31));
        label_34->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:1, stop:0 rgba(48, 48, 48, 255), stop:1 rgba(0, 0, 0, 255));\n"
"font: 75 10pt \"Trebuchet MS\";\n"
"color:#f4f4f4;"));
        listView_2 = new QListView(configure);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        listView_2->setGeometry(QRect(11, 61, 240, 51));
        radioAuto = new QRadioButton(configure);
        radioAuto->setObjectName(QString::fromUtf8("radioAuto"));
        radioAuto->setGeometry(QRect(39, 77, 82, 17));
        radioAuto->setStyleSheet(QString::fromUtf8("font: 8pt \"MS Shell Dlg 2\";\n"
""));
        radioAuto->setChecked(false);
        radioManual = new QRadioButton(configure);
        radioManual->setObjectName(QString::fromUtf8("radioManual"));
        radioManual->setGeometry(QRect(150, 73, 70, 20));
        radioManual->setStyleSheet(QString::fromUtf8("font: 8pt \"MS Shell Dlg 2\";"));
        radioManual->setCheckable(false);
        label_35 = new QLabel(configure);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setGeometry(QRect(11, 150, 240, 31));
        label_35->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:1, stop:0 rgba(48, 48, 48, 255), stop:1 rgba(0, 0, 0, 255));\n"
"font: 75 10pt \"Trebuchet MS\";\n"
"color:#f4f4f4;"));
        listView_3 = new QListView(configure);
        listView_3->setObjectName(QString::fromUtf8("listView_3"));
        listView_3->setGeometry(QRect(11, 180, 240, 51));
        radioDebug = new QRadioButton(configure);
        radioDebug->setObjectName(QString::fromUtf8("radioDebug"));
        radioDebug->setGeometry(QRect(30, 196, 82, 17));
        radioDebug->setStyleSheet(QString::fromUtf8("font: 8pt \"MS Shell Dlg 2\";"));
        radioDebug->setChecked(true);
        radioInfo = new QRadioButton(configure);
        radioInfo->setObjectName(QString::fromUtf8("radioInfo"));
        radioInfo->setGeometry(QRect(111, 197, 60, 17));
        radioInfo->setStyleSheet(QString::fromUtf8("font: 8pt \"MS Shell Dlg 2\";"));
        radioError = new QRadioButton(configure);
        radioError->setObjectName(QString::fromUtf8("radioError"));
        radioError->setGeometry(QRect(177, 196, 60, 17));
        radioError->setStyleSheet(QString::fromUtf8("font: 8pt \"MS Shell Dlg 2\";"));
        uConnectTabWidget->addTab(configure, QString());
        conf_bg->raise();
        label_34->raise();
        listView_2->raise();
        radioAuto->raise();
        radioManual->raise();
        label_35->raise();
        listView_3->raise();
        radioDebug->raise();
        radioError->raise();
        radioInfo->raise();
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gen_bg = new QLabel(tab);
        gen_bg->setObjectName(QString::fromUtf8("gen_bg"));
        gen_bg->setGeometry(QRect(0, -45, 801, 420));
        gen_bg->setStyleSheet(QString::fromUtf8(""));
        measTableWidget = new QTableWidget(tab);
        if (measTableWidget->columnCount() < 2)
            measTableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        measTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        measTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        if (measTableWidget->rowCount() < 29)
            measTableWidget->setRowCount(29);
        measTableWidget->setObjectName(QString::fromUtf8("measTableWidget"));
        measTableWidget->setGeometry(QRect(10, 10, 260, 100));
        measTableWidget->setMinimumSize(QSize(260, 100));
        measTableWidget->setMaximumSize(QSize(260, 100));
        measTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        measTableWidget->setShowGrid(false);
        measTableWidget->setRowCount(29);
        measTableWidget->setColumnCount(2);
        measTableWidget->horizontalHeader()->setDefaultSectionSize(140);
        measTableWidget->verticalHeader()->setVisible(false);
        vBox = new QWidget(tab);
        vBox->setObjectName(QString::fromUtf8("vBox"));
        vBox->setGeometry(QRect(10, 129, 260, 221));
        vBox->setMinimumSize(QSize(260, 0));
        vBox->setMaximumSize(QSize(260, 16777215));
        vBox->setLayoutDirection(Qt::LeftToRight);
        uConnectTabWidget->addTab(tab, QString());
        wifiSetting = new QWidget();
        wifiSetting->setObjectName(QString::fromUtf8("wifiSetting"));
        connectSettingListview_2 = new QListView(wifiSetting);
        connectSettingListview_2->setObjectName(QString::fromUtf8("connectSettingListview_2"));
        connectSettingListview_2->setGeometry(QRect(9, 45, 260, 101));
        connectSettingListview_2->setStyleSheet(QString::fromUtf8("background-color:#ffffff;"));
        wifiAdapterSettingLabel = new QLabel(wifiSetting);
        wifiAdapterSettingLabel->setObjectName(QString::fromUtf8("wifiAdapterSettingLabel"));
        wifiAdapterSettingLabel->setGeometry(QRect(9, 17, 260, 31));
        wifiAdapterSettingLabel->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:1, stop:0 rgba(48, 48, 48, 255), stop:1 rgba(0, 0, 0, 255));\n"
"font: 75 10pt \"Trebuchet MS\";\n"
"color:#f4f4f4;"));
        wifiNetworkCheck = new QCheckBox(wifiSetting);
        wifiNetworkCheck->setObjectName(QString::fromUtf8("wifiNetworkCheck"));
        wifiNetworkCheck->setGeometry(QRect(20, 118, 131, 20));
        preferredNetworkCheck = new QCheckBox(wifiSetting);
        preferredNetworkCheck->setObjectName(QString::fromUtf8("preferredNetworkCheck"));
        preferredNetworkCheck->setGeometry(QRect(21, 92, 201, 21));
        wifiAdapterComboBox = new QComboBox(wifiSetting);
        wifiAdapterComboBox->setObjectName(QString::fromUtf8("wifiAdapterComboBox"));
        wifiAdapterComboBox->setGeometry(QRect(17, 58, 240, 26));
        wifiNetworkListLabel = new QLabel(wifiSetting);
        wifiNetworkListLabel->setObjectName(QString::fromUtf8("wifiNetworkListLabel"));
        wifiNetworkListLabel->setGeometry(QRect(10, 159, 260, 31));
        wifiNetworkListLabel->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:1, stop:0 rgba(48, 48, 48, 255), stop:1 rgba(0, 0, 0, 255));\n"
"font: 75 10pt \"Trebuchet MS\";\n"
"color:#f4f4f4;"));
        wifiPreferredList = new QListWidget(wifiSetting);
        wifiPreferredList->setObjectName(QString::fromUtf8("wifiPreferredList"));
        wifiPreferredList->setGeometry(QRect(10, 190, 260, 160));
        label_33 = new QLabel(wifiSetting);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(10, 324, 260, 25));
        label_33->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:0.988727, stop:0 rgba(232, 232, 232, 255), stop:1 rgba(205, 205, 205, 255));"));
        wifiUpPushButton = new QPushButton(wifiSetting);
        wifiUpPushButton->setObjectName(QString::fromUtf8("wifiUpPushButton"));
        wifiUpPushButton->setGeometry(QRect(12, 326, 20, 20));
        wifiUpPushButton->setStyleSheet(QString::fromUtf8("background-image: url(:/upTech);\n"
"background-color: transparent;"));
        wifiDownPushButton = new QPushButton(wifiSetting);
        wifiDownPushButton->setObjectName(QString::fromUtf8("wifiDownPushButton"));
        wifiDownPushButton->setGeometry(QRect(35, 326, 20, 20));
        wifiDownPushButton->setStyleSheet(QString::fromUtf8("background-image: url(:/downTech);\n"
"background-color: transparent;"));
        uConnectTabWidget->addTab(wifiSetting, QString());
        WIFI = new QWidget();
        WIFI->setObjectName(QString::fromUtf8("WIFI"));
        wifi_bg = new QLabel(WIFI);
        wifi_bg->setObjectName(QString::fromUtf8("wifi_bg"));
        wifi_bg->setGeometry(QRect(-10, 2, 811, 391));
        wifiNetworks = new QLabel(WIFI);
        wifiNetworks->setObjectName(QString::fromUtf8("wifiNetworks"));
        wifiNetworks->setGeometry(QRect(74, 60, 91, 21));
        wifinetworkList = new QTableWidget(WIFI);
        if (wifinetworkList->columnCount() < 3)
            wifinetworkList->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        wifinetworkList->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        wifinetworkList->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        wifinetworkList->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        if (wifinetworkList->rowCount() < 10)
            wifinetworkList->setRowCount(10);
        wifinetworkList->setObjectName(QString::fromUtf8("wifinetworkList"));
        wifinetworkList->setGeometry(QRect(50, 90, 221, 191));
        wifinetworkList->setMouseTracking(true);
        wifinetworkList->setEditTriggers(QAbstractItemView::CurrentChanged|QAbstractItemView::DoubleClicked);
        wifinetworkList->setSelectionMode(QAbstractItemView::ExtendedSelection);
        wifinetworkList->setSelectionBehavior(QAbstractItemView::SelectRows);
        wifinetworkList->setShowGrid(false);
        wifinetworkList->setCornerButtonEnabled(true);
        wifinetworkList->setRowCount(10);
        wifinetworkList->horizontalHeader()->setDefaultSectionSize(225);
        wifinetworkList->verticalHeader()->setVisible(false);
        wifiScan = new QPushButton(WIFI);
        wifiScan->setObjectName(QString::fromUtf8("wifiScan"));
        wifiScan->setGeometry(QRect(680, 10, 40, 35));
        wifiScan->setStyleSheet(QString::fromUtf8("border-top-left-radius: 8px;\n"
"border-top-right-radius: 8px;\n"
"border-bottom-left-radius: 8px;\n"
"border-bottom-right-radius: 8px;\n"
""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/search-icon"), QSize(), QIcon::Normal, QIcon::Off);
        wifiScan->setIcon(icon1);
        wifiScan->setIconSize(QSize(40, 35));
        PasswordEdit = new QLineEdit(WIFI);
        PasswordEdit->setObjectName(QString::fromUtf8("PasswordEdit"));
        PasswordEdit->setGeometry(QRect(110, 310, 161, 20));
        PasswordEdit->setEchoMode(QLineEdit::Password);
        passwordLabel = new QLabel(WIFI);
        passwordLabel->setObjectName(QString::fromUtf8("passwordLabel"));
        passwordLabel->setGeometry(QRect(0, 310, 121, 20));
        networkSsid = new QLabel(WIFI);
        networkSsid->setObjectName(QString::fromUtf8("networkSsid"));
        networkSsid->setGeometry(QRect(170, 60, 151, 21));
        wifi_label_test = new QLabel(WIFI);
        wifi_label_test->setObjectName(QString::fromUtf8("wifi_label_test"));
        wifi_label_test->setGeometry(QRect(475, 23, 151, 31));
        uConnectTabWidget->addTab(WIFI, QString());
        wifi_bg->raise();
        wifiScan->raise();
        networkSsid->raise();
        wifi_label_test->raise();
        wifiNetworks->raise();
        wifinetworkList->raise();
        PasswordEdit->raise();
        passwordLabel->raise();
        vpn = new QWidget();
        vpn->setObjectName(QString::fromUtf8("vpn"));
        label_7 = new QLabel(vpn);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(0, 0, 341, 391));
        label_7->setStyleSheet(QString::fromUtf8("background-color:#eeeeee;"));
        label_37 = new QLabel(vpn);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setGeometry(QRect(10, 87, 260, 31));
        label_37->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:1, stop:0 rgba(48, 48, 48, 255), stop:1 rgba(0, 0, 0, 255));\n"
"font: 75 10pt \"Trebuchet MS\";\n"
"color:#f4f4f4;"));
        vpnClientListCombo = new QComboBox(vpn);
        vpnClientListCombo->setObjectName(QString::fromUtf8("vpnClientListCombo"));
        vpnClientListCombo->setGeometry(QRect(60, 143, 130, 22));
        vpnProfileListCombo = new QComboBox(vpn);
        vpnProfileListCombo->setObjectName(QString::fromUtf8("vpnProfileListCombo"));
        vpnProfileListCombo->setGeometry(QRect(61, 189, 130, 22));
        vpnAutoCheck = new QCheckBox(vpn);
        vpnAutoCheck->setObjectName(QString::fromUtf8("vpnAutoCheck"));
        vpnAutoCheck->setGeometry(QRect(17, 261, 240, 17));
        vpnAutoCheck->setChecked(true);
        listView_5 = new QListView(vpn);
        listView_5->setObjectName(QString::fromUtf8("listView_5"));
        listView_5->setGeometry(QRect(10, 118, 260, 131));
        vpnButton = new QPushButton(vpn);
        vpnButton->setObjectName(QString::fromUtf8("vpnButton"));
        vpnButton->setGeometry(QRect(186, 304, 75, 23));
        uConnectTabWidget->addTab(vpn, QString());
        label_7->raise();
        label_37->raise();
        vpnAutoCheck->raise();
        listView_5->raise();
        vpnClientListCombo->raise();
        vpnProfileListCombo->raise();
        vpnButton->raise();
        network = new QWidget();
        network->setObjectName(QString::fromUtf8("network"));
        network->setEnabled(true);
        network->setCursor(QCursor(Qt::ArrowCursor));
        network->setFocusPolicy(Qt::TabFocus);
        baseStationLogo = new QLabel(network);
        baseStationLogo->setObjectName(QString::fromUtf8("baseStationLogo"));
        baseStationLogo->setGeometry(QRect(30, 30, 26, 34));
        baseStationLogo->setPixmap(QPixmap(QString::fromUtf8(":/wi-fi-icon")));
        baseStationLogo->setScaledContents(true);
        searchButton = new QPushButton(network);
        searchButton->setObjectName(QString::fromUtf8("searchButton"));
        searchButton->setGeometry(QRect(680, 10, 40, 35));
        searchButton->setStyleSheet(QString::fromUtf8("border-top-left-radius: 8px;\n"
"border-top-right-radius: 8px;\n"
"border-bottom-left-radius: 8px;\n"
"border-bottom-right-radius: 8px;\n"
""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/search-icon"), QSize(), QIcon::Normal, QIcon::On);
        searchButton->setIcon(icon2);
        searchButton->setIconSize(QSize(40, 35));
        lteLabel = new QLabel(network);
        lteLabel->setObjectName(QString::fromUtf8("lteLabel"));
        lteLabel->setGeometry(QRect(110, 35, 171, 31));
        QFont font2;
        font2.setPointSize(10);
        lteLabel->setFont(font2);
        lteLabel->setStyleSheet(QString::fromUtf8(""));
        lteLabel->setScaledContents(true);
        net_bg = new QLabel(network);
        net_bg->setObjectName(QString::fromUtf8("net_bg"));
        net_bg->setGeometry(QRect(0, 0, 801, 441));
        net_bg->setStyleSheet(QString::fromUtf8(""));
        label_11 = new QLabel(network);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(0, 380, 801, 41));
        label_11->setStyleSheet(QString::fromUtf8(""));
        label_18 = new QLabel(network);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(105, 93, 100, 20));
        label_18->setStyleSheet(QString::fromUtf8(""));
        apntableWidget = new QTableWidget(network);
        if (apntableWidget->columnCount() < 2)
            apntableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        apntableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        apntableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem10);
        if (apntableWidget->rowCount() < 1)
            apntableWidget->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        apntableWidget->setVerticalHeaderItem(0, __qtablewidgetitem11);
        apntableWidget->setObjectName(QString::fromUtf8("apntableWidget"));
        apntableWidget->setGeometry(QRect(100, 130, 601, 171));
        apntableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        apntableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        apntableWidget->setShowGrid(false);
        apntableWidget->setRowCount(1);
        apntableWidget->horizontalHeader()->setDefaultSectionSize(300);
        apntableWidget->horizontalHeader()->setMinimumSectionSize(30);
        apntableWidget->verticalHeader()->setVisible(false);
        apntableWidget->verticalHeader()->setCascadingSectionResizes(true);
        apntableWidget->verticalHeader()->setDefaultSectionSize(300);
        apntableWidget->verticalHeader()->setMinimumSectionSize(60);
        apntableWidget->verticalHeader()->setProperty("showSortIndicator", QVariant(true));
        apntableWidget->verticalHeader()->setStretchLastSection(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/network-unselected"), QSize(), QIcon::Normal, QIcon::Off);
        icon3.addFile(QString::fromUtf8(":/network-selected"), QSize(), QIcon::Normal, QIcon::On);
        uConnectTabWidget->addTab(network, icon3, QString());
        net_bg->raise();
        baseStationLogo->raise();
        searchButton->raise();
        lteLabel->raise();
        label_11->raise();
        label_18->raise();
        apntableWidget->raise();
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        label_19 = new QLabel(tab_2);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(32, 13, 60, 13));
        label_19->setStyleSheet(QString::fromUtf8(""));
        Testmode_Disable = new QRadioButton(tab_2);
        Testmode_Disable->setObjectName(QString::fromUtf8("Testmode_Disable"));
        Testmode_Disable->setGeometry(QRect(202, 11, 70, 20));
        Testmode_Disable->setStyleSheet(QString::fromUtf8(""));
        Testmode_Disable->setChecked(false);
        Testmode_Enable = new QRadioButton(tab_2);
        Testmode_Enable->setObjectName(QString::fromUtf8("Testmode_Enable"));
        Testmode_Enable->setGeometry(QRect(112, 12, 60, 17));
        Testmode_Enable->setStyleSheet(QString::fromUtf8(""));
        Testmode_Enable->setChecked(true);
        SMSgroupBox = new QGroupBox(tab_2);
        SMSgroupBox->setObjectName(QString::fromUtf8("SMSgroupBox"));
        SMSgroupBox->setGeometry(QRect(12, 51, 321, 121));
        SMSgroupBox->setStyleSheet(QString::fromUtf8(""));
        SMSgroupBox->setCheckable(false);
        label_8 = new QLabel(SMSgroupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(-10, 22, 131, 20));
        label_8->setStyleSheet(QString::fromUtf8(""));
        label_9 = new QLabel(SMSgroupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(-10, 50, 131, 20));
        label_9->setStyleSheet(QString::fromUtf8(""));
        label_10 = new QLabel(SMSgroupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(-11, 81, 111, 20));
        label_10->setStyleSheet(QString::fromUtf8(""));
        smsWriteUiccOn = new QRadioButton(SMSgroupBox);
        smsWriteUiccOn->setObjectName(QString::fromUtf8("smsWriteUiccOn"));
        smsWriteUiccOn->setGeometry(QRect(130, 22, 60, 17));
        smsWriteUiccOn->setStyleSheet(QString::fromUtf8(""));
        smsWriteUiccOn->setChecked(false);
        smsWriteUiccOff = new QRadioButton(SMSgroupBox);
        smsWriteUiccOff->setObjectName(QString::fromUtf8("smsWriteUiccOff"));
        smsWriteUiccOff->setGeometry(QRect(210, 21, 50, 20));
        smsWriteUiccOff->setStyleSheet(QString::fromUtf8(""));
        smsWriteUiccOff->setChecked(true);
        smsWriteUiccOff->setAutoExclusive(false);
        SmsFormattOn = new QRadioButton(SMSgroupBox);
        SmsFormattOn->setObjectName(QString::fromUtf8("SmsFormattOn"));
        SmsFormattOn->setGeometry(QRect(130, 51, 61, 17));
        SmsFormattOn->setStyleSheet(QString::fromUtf8(""));
        SmsFormattOn->setChecked(true);
        SmsFormattOn->setAutoExclusive(false);
        SmsFormattOff = new QRadioButton(SMSgroupBox);
        SmsFormattOff->setObjectName(QString::fromUtf8("SmsFormattOff"));
        SmsFormattOff->setGeometry(QRect(210, 49, 50, 20));
        SmsFormattOff->setStyleSheet(QString::fromUtf8(""));
        SmsFormattOff->setAutoExclusive(false);
        SOI_On = new QRadioButton(SMSgroupBox);
        SOI_On->setObjectName(QString::fromUtf8("SOI_On"));
        SOI_On->setGeometry(QRect(130, 80, 50, 20));
        SOI_On->setStyleSheet(QString::fromUtf8(""));
        SOI_On->setChecked(true);
        SOI_On->setAutoExclusive(false);
        SOI_Off = new QRadioButton(SMSgroupBox);
        SOI_Off->setObjectName(QString::fromUtf8("SOI_Off"));
        SOI_Off->setGeometry(QRect(210, 79, 50, 20));
        SOI_Off->setStyleSheet(QString::fromUtf8(""));
        SOI_Off->setAutoExclusive(false);
        IMSRegStatus = new QListWidget(tab_2);
        IMSRegStatus->setObjectName(QString::fromUtf8("IMSRegStatus"));
        IMSRegStatus->setGeometry(QRect(523, 15, 231, 21));
        TimergroupBox = new QGroupBox(tab_2);
        TimergroupBox->setObjectName(QString::fromUtf8("TimergroupBox"));
        TimergroupBox->setGeometry(QRect(363, 50, 401, 121));
        TimergroupBox->setStyleSheet(QString::fromUtf8(""));
        T1timerEdit = new QLineEdit(TimergroupBox);
        T1timerEdit->setObjectName(QString::fromUtf8("T1timerEdit"));
        T1timerEdit->setGeometry(QRect(40, 40, 81, 20));
        T2timerEdit = new QLineEdit(TimergroupBox);
        T2timerEdit->setObjectName(QString::fromUtf8("T2timerEdit"));
        T2timerEdit->setGeometry(QRect(160, 80, 81, 20));
        TFtimerEdit = new QLineEdit(TimergroupBox);
        TFtimerEdit->setObjectName(QString::fromUtf8("TFtimerEdit"));
        TFtimerEdit->setGeometry(QRect(280, 40, 81, 20));
        label_21 = new QLabel(TimergroupBox);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(10, 40, 21, 16));
        label_22 = new QLabel(TimergroupBox);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(120, 80, 31, 16));
        label_23 = new QLabel(TimergroupBox);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(250, 40, 21, 16));
        label_24 = new QLabel(TimergroupBox);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(130, 40, 31, 16));
        label_25 = new QLabel(TimergroupBox);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(250, 80, 31, 16));
        label_26 = new QLabel(TimergroupBox);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(370, 40, 31, 16));
        label_27 = new QLabel(tab_2);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(363, 15, 141, 21));
        label_27->setStyleSheet(QString::fromUtf8(""));
        messageToolBox = new QToolBox(tab_2);
        messageToolBox->setObjectName(QString::fromUtf8("messageToolBox"));
        messageToolBox->setGeometry(QRect(20, 199, 731, 171));
        messageToolBox->setStyleSheet(QString::fromUtf8("color:#eb7012;"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 100, 30));
        lineEdit_5 = new QLineEdit(page);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(220, 20, 321, 81));
        lineEdit_5->setStyleSheet(QString::fromUtf8("color:#eb7012;"));
        pushButton = new QPushButton(page);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(110, 10, 75, 31));
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton_2 = new QPushButton(page);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(110, 60, 75, 31));
        pushButton_2->setStyleSheet(QString::fromUtf8(""));
        messageToolBox->addItem(page, QString::fromUtf8("Create SMS"));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setGeometry(QRect(0, 0, 731, 117));
        messageToolBox->addItem(page_2, QString::fromUtf8("INBOX"));
        ims_bg = new QLabel(tab_2);
        ims_bg->setObjectName(QString::fromUtf8("ims_bg"));
        ims_bg->setGeometry(QRect(0, 0, 801, 391));
        ims_bg->setStyleSheet(QString::fromUtf8(""));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/ims-icon"), QSize(), QIcon::Normal, QIcon::Off);
        icon4.addFile(QString::fromUtf8(":/imsi-icon-selected"), QSize(), QIcon::Normal, QIcon::On);
        uConnectTabWidget->addTab(tab_2, icon4, QString());
        ims_bg->raise();
        label_19->raise();
        Testmode_Disable->raise();
        Testmode_Enable->raise();
        SMSgroupBox->raise();
        IMSRegStatus->raise();
        TimergroupBox->raise();
        label_27->raise();
        messageToolBox->raise();
        ussd_tab = new QWidget();
        ussd_tab->setObjectName(QString::fromUtf8("ussd_tab"));
        ussd_request = new QLineEdit(ussd_tab);
        ussd_request->setObjectName(QString::fromUtf8("ussd_request"));
        ussd_request->setGeometry(QRect(20, 271, 141, 20));
        send_ussd = new QPushButton(ussd_tab);
        send_ussd->setObjectName(QString::fromUtf8("send_ussd"));
        send_ussd->setGeometry(QRect(180, 260, 80, 30));
        label_5 = new QLabel(ussd_tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 251, 141, 20));
        label_5->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:1, stop:0 rgba(48, 48, 48, 255), stop:1 rgba(0, 0, 0, 255));\n"
"font: 75 10pt \"Trebuchet MS\";\n"
"color:#f4f4f4;"));
        ussd_response = new QListWidget(ussd_tab);
        ussd_response->setObjectName(QString::fromUtf8("ussd_response"));
        ussd_response->setGeometry(QRect(20, 40, 256, 192));
        uConnectTabWidget->addTab(ussd_tab, QString());
        NavigationBar = new QLabel(centralWidget);
        NavigationBar->setObjectName(QString::fromUtf8("NavigationBar"));
        NavigationBar->setGeometry(QRect(200, 10, 381, 31));
        NavigationBar->setStyleSheet(QString::fromUtf8(""));
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(482, -24, 311, 31));
        label_15->setPixmap(QPixmap(QString::fromUtf8(":/connection-manager")));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(0, 46, 200, 41));
        listView = new QListView(centralWidget);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(0, 10, 200, 410));
        listView->setMaximumSize(QSize(220, 16777215));
        _3gPushButton = new QPushButton(centralWidget);
        _3gPushButton->setObjectName(QString::fromUtf8("_3gPushButton"));
        _3gPushButton->setGeometry(QRect(0, 50, 200, 40));
        _3gPushButton->setStyleSheet(QString::fromUtf8("font: 75 8pt \"MS Shell Dlg 2\";\n"
"background-color: transparent;"));
        wifiPushButton = new QPushButton(centralWidget);
        wifiPushButton->setObjectName(QString::fromUtf8("wifiPushButton"));
        wifiPushButton->setGeometry(QRect(0, 90, 200, 40));
        wifiPushButton->setStyleSheet(QString::fromUtf8("font: 75 8pt \"MS Shell Dlg 2\";\n"
"background-color: transparent;"));
        ltePushButton = new QPushButton(centralWidget);
        ltePushButton->setObjectName(QString::fromUtf8("ltePushButton"));
        ltePushButton->setGeometry(QRect(0, 130, 200, 40));
        ltePushButton->setStyleSheet(QString::fromUtf8("font: 75 8pt \"MS Shell Dlg 2\";\n"
"background-color: transparent;"));
        tetheringPushButton = new QPushButton(centralWidget);
        tetheringPushButton->setObjectName(QString::fromUtf8("tetheringPushButton"));
        tetheringPushButton->setGeometry(QRect(0, 170, 200, 40));
        tetheringPushButton->setStyleSheet(QString::fromUtf8("font: 75 8pt \"MS Shell Dlg 2\";\n"
"background-color: transparent;"));
        wifiCheckBox = new QCheckBox(centralWidget);
        wifiCheckBox->setObjectName(QString::fromUtf8("wifiCheckBox"));
        wifiCheckBox->setGeometry(QRect(160, 100, 16, 17));
        lteCheckBox = new QCheckBox(centralWidget);
        lteCheckBox->setObjectName(QString::fromUtf8("lteCheckBox"));
        lteCheckBox->setGeometry(QRect(160, 140, 16, 17));
        tetheringCheckBox = new QCheckBox(centralWidget);
        tetheringCheckBox->setObjectName(QString::fromUtf8("tetheringCheckBox"));
        tetheringCheckBox->setGeometry(QRect(160, 180, 16, 17));
        vpnPushButton = new QPushButton(centralWidget);
        vpnPushButton->setObjectName(QString::fromUtf8("vpnPushButton"));
        vpnPushButton->setGeometry(QRect(0, 360, 200, 40));
        vpnPushButton->setStyleSheet(QString::fromUtf8("font: 75 8pt \"MS Shell Dlg 2\";\n"
"background-color: transparent;"));
        generalPushButton = new QPushButton(centralWidget);
        generalPushButton->setObjectName(QString::fromUtf8("generalPushButton"));
        generalPushButton->setGeometry(QRect(0, 10, 200, 40));
        generalPushButton->setStyleSheet(QString::fromUtf8("font: 75 8pt \"MS Shell Dlg 2\";\n"
"background-color: transparent;"));
        _3GCheckBox = new QCheckBox(centralWidget);
        _3GCheckBox->setObjectName(QString::fromUtf8("_3GCheckBox"));
        _3GCheckBox->setGeometry(QRect(160, 60, 16, 17));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(12, 62, 41, 16));
        label->setStyleSheet(QString::fromUtf8(""));
        label->setTextFormat(Qt::PlainText);
        label->setPixmap(QPixmap(QString::fromUtf8(":/3gTech")));
        label->setScaledContents(false);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 100, 20, 20));
        label_2->setStyleSheet(QString::fromUtf8("background-image: url(:/WifiTech);"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 370, 20, 20));
        label_4->setStyleSheet(QString::fromUtf8("background-image: url(:/VpnTech);"));
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(16, 140, 30, 21));
        label_12->setStyleSheet(QString::fromUtf8(""));
        label_12->setPixmap(QPixmap(QString::fromUtf8(":/4gLteTech")));
        label_12->setScaledContents(false);
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(20, 180, 32, 20));
        label_13->setStyleSheet(QString::fromUtf8("background-image: url(:/WiredTech);"));
        label_17 = new QLabel(centralWidget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(20, 20, 20, 20));
        label_17->setStyleSheet(QString::fromUtf8("background-image: url(:/SettingTech);"));
        label_20 = new QLabel(centralWidget);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(0, 50, 200, 2));
        label_20->setStyleSheet(QString::fromUtf8("background-color: #eeeeee;"));
        label_28 = new QLabel(centralWidget);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(0, 90, 200, 2));
        label_28->setStyleSheet(QString::fromUtf8("background-color: #eeeeee;"));
        label_29 = new QLabel(centralWidget);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(0, 130, 200, 2));
        label_29->setStyleSheet(QString::fromUtf8("background-color: #eeeeee;"));
        label_30 = new QLabel(centralWidget);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(0, 170, 200, 2));
        label_30->setStyleSheet(QString::fromUtf8("background-color: #eeeeee;"));
        label_31 = new QLabel(centralWidget);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(0, 210, 200, 2));
        label_31->setStyleSheet(QString::fromUtf8("background-color: #eeeeee;"));
        label_32 = new QLabel(centralWidget);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(0, 360, 200, 2));
        label_32->setStyleSheet(QString::fromUtf8("background-color: #eeeeee;"));
        demark = new QLabel(centralWidget);
        demark->setObjectName(QString::fromUtf8("demark"));
        demark->setGeometry(QRect(0, 0, 601, 16));
        demark->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:reflect, x1:1, y1:1, x2:1, y2:0.415, stop:0 rgba(160, 160, 160, 255), stop:1 rgba(221, 221, 221, 255));"));
        demark->raise();
        NavigationBar->raise();
        uConnectTabWidget->raise();
        label_15->raise();
        pushButton_3->raise();
        listView->raise();
        _3gPushButton->raise();
        wifiPushButton->raise();
        ltePushButton->raise();
        tetheringPushButton->raise();
        wifiCheckBox->raise();
        lteCheckBox->raise();
        tetheringCheckBox->raise();
        vpnPushButton->raise();
        generalPushButton->raise();
        _3GCheckBox->raise();
        label->raise();
        label_2->raise();
        label_4->raise();
        label_12->raise();
        label_13->raise();
        label_17->raise();
        label_20->raise();
        label_28->raise();
        label_29->raise();
        label_30->raise();
        label_31->raise();
        label_32->raise();

        retranslateUi(DetailedView);

        uConnectTabWidget->setCurrentIndex(3);
        messageToolBox->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(DetailedView);
    } // setupUi

    void retranslateUi(QFrame *DetailedView)
    {
        DetailedView->setWindowTitle(QApplication::translate("DetailedView", "Connection Manager", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        uConnectTabWidget->setWhatsThis(QApplication::translate("DetailedView", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        connectSettingLabel->setText(QApplication::translate("DetailedView", "   Network Selection Settings", 0, QApplication::UnicodeUTF8));
        techPriorityLabel->setText(QApplication::translate("DetailedView", "   Connectivity Preferences", 0, QApplication::UnicodeUTF8));
        label_3->setText(QString());
        techUpPushButton->setText(QString());
        techDownPushButton->setText(QString());
        connectState->setText(QString());
        label_40->setText(QApplication::translate("DetailedView", "Connect Automatically To Network", 0, QApplication::UnicodeUTF8));
        applySettingsButton->setText(QString());
        fileLogginOption->setText(QApplication::translate("DetailedView", "Enable ServerLog", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("DetailedView", "   Log File Name", 0, QApplication::UnicodeUTF8));
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(generalSetting), QApplication::translate("DetailedView", "    Settings    ", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DetailedView", "Profile Name* :", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("DetailedView", "Phone Number* :", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("DetailedView", "User Name :", 0, QApplication::UnicodeUTF8));
        label_45->setText(QApplication::translate("DetailedView", "Password :", 0, QApplication::UnicodeUTF8));
        label_46->setText(QApplication::translate("DetailedView", "APN :", 0, QApplication::UnicodeUTF8));
        rasProfSave->setText(QApplication::translate("DetailedView", "Save", 0, QApplication::UnicodeUTF8));
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(profiles), QApplication::translate("DetailedView", "Profiles", 0, QApplication::UnicodeUTF8));
        label_41->setText(QApplication::translate("DetailedView", "   Statisitics:", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = threeGStatTableWidget->isSortingEnabled();
        threeGStatTableWidget->setSortingEnabled(false);
        threeGStatTableWidget->setSortingEnabled(__sortingEnabled);

        label_42->setText(QApplication::translate("DetailedView", "   Signal Info:", 0, QApplication::UnicodeUTF8));
        rssiThreeGProgressBar->setFormat(QString());
        cinrThreeGProgressBar->setFormat(QString());
        label_43->setText(QApplication::translate("DetailedView", "RSSI", 0, QApplication::UnicodeUTF8));
        label_44->setText(QApplication::translate("DetailedView", "CINR", 0, QApplication::UnicodeUTF8));
        threegRssiVal->setText(QString());
        threegCinrVal->setText(QString());
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(threeGStatistics), QApplication::translate("DetailedView", "   Statistics", 0, QApplication::UnicodeUTF8));
        GetModStats->setText(QApplication::translate("DetailedView", "GetStats", 0, QApplication::UnicodeUTF8));
        stats_bg->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = statTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("DetailedView", "Parameter", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = statTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("DetailedView", "Values", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("DetailedView", "   Signal Info", 0, QApplication::UnicodeUTF8));
        rssiValueEdit->setText(QString());
        cinrValueEdit->setText(QString());
        RSSIProgressBar->setFormat(QString());
        CINRProgressBar->setFormat(QString());
        rssi->setText(QApplication::translate("DetailedView", " RSSI", 0, QApplication::UnicodeUTF8));
        cinr->setText(QApplication::translate("DetailedView", "CINR", 0, QApplication::UnicodeUTF8));
        refresh->setText(QApplication::translate("DetailedView", "Refresh Interval", 0, QApplication::UnicodeUTF8));
        m_editRefreshInterval->clear();
        m_editRefreshInterval->insertItems(0, QStringList()
         << QApplication::translate("DetailedView", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DetailedView", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DetailedView", "3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DetailedView", "4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DetailedView", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DetailedView", "6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DetailedView", "7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DetailedView", "8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DetailedView", "9", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DetailedView", "10", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DetailedView", "11", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DetailedView", "12", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DetailedView", "13", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DetailedView", "14", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DetailedView", "15", 0, QApplication::UnicodeUTF8)
        );
        second->setText(QApplication::translate("DetailedView", "secs", 0, QApplication::UnicodeUTF8));
        label_39->setText(QApplication::translate("DetailedView", "   Settings", 0, QApplication::UnicodeUTF8));
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(debugTool), QApplication::translate("DetailedView", "    Statistics    ", 0, QApplication::UnicodeUTF8));
        conf_bg->setText(QString());
        label_34->setText(QApplication::translate("DetailedView", "    Config Mode", 0, QApplication::UnicodeUTF8));
        radioAuto->setText(QApplication::translate("DetailedView", "Auto", 0, QApplication::UnicodeUTF8));
        radioManual->setText(QApplication::translate("DetailedView", "Manual", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("DetailedView", "    Log Level", 0, QApplication::UnicodeUTF8));
        radioDebug->setText(QApplication::translate("DetailedView", "Debug", 0, QApplication::UnicodeUTF8));
        radioInfo->setText(QApplication::translate("DetailedView", "Info", 0, QApplication::UnicodeUTF8));
        radioError->setText(QApplication::translate("DetailedView", "Error", 0, QApplication::UnicodeUTF8));
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(configure), QApplication::translate("DetailedView", "    Settings   ", 0, QApplication::UnicodeUTF8));
        gen_bg->setText(QString());
        QTableWidgetItem *___qtablewidgetitem2 = measTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("DetailedView", "Paramters", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = measTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("DetailedView", "Values", 0, QApplication::UnicodeUTF8));
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(tab), QApplication::translate("DetailedView", "Graph", 0, QApplication::UnicodeUTF8));
        wifiAdapterSettingLabel->setText(QApplication::translate("DetailedView", "  Wifi Adapter Settings", 0, QApplication::UnicodeUTF8));
        wifiNetworkCheck->setText(QApplication::translate("DetailedView", "Display WiFi Networks", 0, QApplication::UnicodeUTF8));
        preferredNetworkCheck->setText(QApplication::translate("DetailedView", "Display Only Preferred Networks", 0, QApplication::UnicodeUTF8));
        wifiAdapterComboBox->clear();
        wifiAdapterComboBox->insertItems(0, QStringList()
         << QApplication::translate("DetailedView", "WiFi Adapter", 0, QApplication::UnicodeUTF8)
        );
        wifiNetworkListLabel->setText(QApplication::translate("DetailedView", "  WiFi Networks", 0, QApplication::UnicodeUTF8));
        label_33->setText(QString());
        wifiUpPushButton->setText(QString());
        wifiDownPushButton->setText(QString());
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(wifiSetting), QApplication::translate("DetailedView", "    Settings     ", 0, QApplication::UnicodeUTF8));
        wifi_bg->setText(QString());
        wifiNetworks->setText(QApplication::translate("DetailedView", "Wifi Networks:", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = wifinetworkList->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("DetailedView", "Network Name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = wifinetworkList->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("DetailedView", "Signal Quality(%)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = wifinetworkList->horizontalHeaderItem(2);
        ___qtablewidgetitem6->setText(QApplication::translate("DetailedView", "Connected?", 0, QApplication::UnicodeUTF8));
        wifiScan->setText(QString());
        passwordLabel->setText(QApplication::translate("DetailedView", "Enter the Password:", 0, QApplication::UnicodeUTF8));
        networkSsid->setText(QString());
        wifi_label_test->setText(QString());
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(WIFI), QApplication::translate("DetailedView", "    Wifi Statistics    ", 0, QApplication::UnicodeUTF8));
        label_7->setText(QString());
        label_37->setText(QApplication::translate("DetailedView", "     VPN Settings", 0, QApplication::UnicodeUTF8));
        vpnClientListCombo->clear();
        vpnClientListCombo->insertItems(0, QStringList()
         << QApplication::translate("DetailedView", "VPN Client", 0, QApplication::UnicodeUTF8)
        );
        vpnProfileListCombo->clear();
        vpnProfileListCombo->insertItems(0, QStringList()
         << QApplication::translate("DetailedView", "Profile", 0, QApplication::UnicodeUTF8)
        );
        vpnAutoCheck->setText(QApplication::translate("DetailedView", "      Automatically connect when VPN is chosen", 0, QApplication::UnicodeUTF8));
        vpnButton->setText(QApplication::translate("DetailedView", "Vpn Start", 0, QApplication::UnicodeUTF8));
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(vpn), QApplication::translate("DetailedView", "    Settings   ", 0, QApplication::UnicodeUTF8));
        baseStationLogo->setText(QString());
        searchButton->setText(QString());
        lteLabel->setText(QApplication::translate("DetailedView", "Long Term Evolution (LTE)", 0, QApplication::UnicodeUTF8));
        net_bg->setText(QString());
        label_11->setText(QString());
        label_18->setText(QApplication::translate("DetailedView", "Network Names:", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = apntableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem7->setText(QApplication::translate("DetailedView", "Network", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = apntableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem8->setText(QApplication::translate("DetailedView", "Duration", 0, QApplication::UnicodeUTF8));
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(network), QApplication::translate("DetailedView", "&Network", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("DetailedView", "Test Mode", 0, QApplication::UnicodeUTF8));
        Testmode_Disable->setText(QApplication::translate("DetailedView", "DISABLE", 0, QApplication::UnicodeUTF8));
        Testmode_Enable->setText(QApplication::translate("DetailedView", "ENABLE", 0, QApplication::UnicodeUTF8));
        SMSgroupBox->setTitle(QApplication::translate("DetailedView", "Sms Settings", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("DetailedView", "      Sms Write UICC", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("DetailedView", "      Sms Format", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("DetailedView", "      Sms Over Ims", 0, QApplication::UnicodeUTF8));
        smsWriteUiccOn->setText(QApplication::translate("DetailedView", "   ON", 0, QApplication::UnicodeUTF8));
        smsWriteUiccOff->setText(QApplication::translate("DetailedView", "  OFF", 0, QApplication::UnicodeUTF8));
        SmsFormattOn->setText(QApplication::translate("DetailedView", "3GPP2", 0, QApplication::UnicodeUTF8));
        SmsFormattOff->setText(QApplication::translate("DetailedView", "3GPP", 0, QApplication::UnicodeUTF8));
        SOI_On->setText(QApplication::translate("DetailedView", "   ON", 0, QApplication::UnicodeUTF8));
        SOI_Off->setText(QApplication::translate("DetailedView", "  OFF", 0, QApplication::UnicodeUTF8));
        TimergroupBox->setTitle(QApplication::translate("DetailedView", "Timer Settings", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("DetailedView", "   T1", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("DetailedView", "    T2", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("DetailedView", "   TF", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("DetailedView", "sec", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("DetailedView", "sec", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("DetailedView", "sec", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("DetailedView", "  IMS Registration Status", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("DetailedView", "Send", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("DetailedView", "Cancel", 0, QApplication::UnicodeUTF8));
        messageToolBox->setItemText(messageToolBox->indexOf(page), QApplication::translate("DetailedView", "Create SMS", 0, QApplication::UnicodeUTF8));
        messageToolBox->setItemText(messageToolBox->indexOf(page_2), QApplication::translate("DetailedView", "INBOX", 0, QApplication::UnicodeUTF8));
        ims_bg->setText(QString());
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(tab_2), QApplication::translate("DetailedView", "IMS", 0, QApplication::UnicodeUTF8));
        send_ussd->setText(QApplication::translate("DetailedView", "send", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DetailedView", " Enter Command Code:", 0, QApplication::UnicodeUTF8));
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(ussd_tab), QApplication::translate("DetailedView", "USSD", 0, QApplication::UnicodeUTF8));
        NavigationBar->setText(QString());
        label_15->setText(QString());
        pushButton_3->setText(QApplication::translate("DetailedView", "General", 0, QApplication::UnicodeUTF8));
        _3gPushButton->setText(QApplication::translate("DetailedView", "3G/2G", 0, QApplication::UnicodeUTF8));
        wifiPushButton->setText(QApplication::translate("DetailedView", "WiFi", 0, QApplication::UnicodeUTF8));
        ltePushButton->setText(QApplication::translate("DetailedView", "4G/LTE", 0, QApplication::UnicodeUTF8));
        tetheringPushButton->setText(QApplication::translate("DetailedView", "Tethering", 0, QApplication::UnicodeUTF8));
        wifiCheckBox->setText(QString());
        lteCheckBox->setText(QString());
        tetheringCheckBox->setText(QString());
        vpnPushButton->setText(QApplication::translate("DetailedView", "VPN", 0, QApplication::UnicodeUTF8));
        generalPushButton->setText(QApplication::translate("DetailedView", "General", 0, QApplication::UnicodeUTF8));
        _3GCheckBox->setText(QString());
        label->setText(QString());
        label_2->setText(QString());
        label_4->setText(QString());
        label_12->setText(QString());
        label_13->setText(QString());
        label_17->setText(QString());
        label_20->setText(QString());
        label_28->setText(QString());
        label_29->setText(QString());
        label_30->setText(QString());
        label_31->setText(QString());
        label_32->setText(QString());
        demark->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DetailedView: public Ui_DetailedView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DETAILEDVIEW_H
