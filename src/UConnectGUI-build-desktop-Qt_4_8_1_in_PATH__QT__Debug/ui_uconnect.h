/********************************************************************************
** Form generated from reading UI file 'uconnect.ui'
**
** Created: Tue Jul 31 10:42:59 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UCONNECT_H
#define UI_UCONNECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_detailedView
{
public:
    QWidget *centralWidget;
    QTabWidget *uConnectTabWidget;
    QWidget *network;
    QLabel *baseStationLogo;
    QPushButton *searchButton;
    QLabel *lteLabel;
    QListWidget *searchResults;
    QListWidget *apnName;
    QLabel *label_9;
    QLabel *label_11;
    QLabel *label_17;
    QLabel *label_18;
    QWidget *configure;
    QGroupBox *groupBox_2;
    QLabel *label_6;
    QRadioButton *radioAuto;
    QRadioButton *radioManual;
    QGroupBox *groupBox_3;
    QLabel *label_7;
    QRadioButton *radioDebug;
    QRadioButton *radioInfo;
    QRadioButton *radioError;
    QGroupBox *groupBox_4;
    QLineEdit *logFileName;
    QCheckBox *fileLogginOption;
    QLabel *label_8;
    QWidget *debugTool;
    QGroupBox *groupBox_5;
    QProgressBar *RSSIProgressBar;
    QProgressBar *CINRProgressBar;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *rssiValueEdit;
    QLabel *cinrValueEdit;
    QGroupBox *groupBox_6;
    QCheckBox *Refresh_checkBox;
    QLabel *label_3;
    QLineEdit *m_editRefreshInterval;
    QLabel *label_4;
    QPushButton *GetModStats;
    QTreeWidget *TreeStat;
    QGroupBox *groupBox_7;
    QListWidget *modemState;
    QLabel *label_12;
    QLabel *label_10;
    QWidget *tab;
    QTreeWidget *Info_treewidget;
    QWidget *vBox;
    QToolButton *connectButton;
    QLabel *NavigationBar;
    QLabel *label_5;
    QLabel *label_13;
    QLabel *serverState;
    QLabel *deviceMsg;
    QPushButton *helpButton;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *detailedView)
    {
        if (detailedView->objectName().isEmpty())
            detailedView->setObjectName(QString::fromUtf8("detailedView"));
        detailedView->resize(800, 600);
        detailedView->setMaximumSize(QSize(800, 600));
        detailedView->setLayoutDirection(Qt::LeftToRight);
        centralWidget = new QWidget(detailedView);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        uConnectTabWidget = new QTabWidget(centralWidget);
        uConnectTabWidget->setObjectName(QString::fromUtf8("uConnectTabWidget"));
        uConnectTabWidget->setGeometry(QRect(0, 90, 801, 431));
        QPalette palette;
        QBrush brush(QColor(255, 85, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        QBrush brush1(QColor(120, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush1);
        uConnectTabWidget->setPalette(palette);
        QFont font;
        font.setPointSize(10);
        uConnectTabWidget->setFont(font);
        uConnectTabWidget->setStyleSheet(QString::fromUtf8("QTabWidget::pane { /* The tab widget frame */\n"
"border-top: 2px solid #e5e5e5;\n"
"background-color: #fefefe;\n"
"color:#e0e0dd;\n"
"}\n"
"QTabWidget::tab-bar {\n"
"left: 18px; /* move to the right by 5px */\n"
"background: #1c1c1c;\n"
"background-color: #0065a4;\n"
"}\n"
"/* Style the tab using the tab sub-control. Note that it reads QTabBar _not_ QTabWidget */\n"
"QTabBar::tab {\n"
"background-color: #0065a4;\n"
"color:#e0e0dd;\n"
"border-top-left-radius: 11px;\n"
"border-top-right-radius: 11px;\n"
"min-width: 0px;\n"
"padding: 4px;\n"
"}\n"
"QTabBar::tab:selected{\n"
"color:#0065a4;\n"
"font: bold;\n"
"background:  #e5e5e5;\n"
"\n"
"min-width: 50px;\n"
"}\n"
"\n"
"background-color: #0065a4;\n"
""));
        uConnectTabWidget->setTabShape(QTabWidget::Rounded);
        uConnectTabWidget->setIconSize(QSize(25, 25));
        uConnectTabWidget->setTabsClosable(false);
        network = new QWidget();
        network->setObjectName(QString::fromUtf8("network"));
        network->setEnabled(true);
        network->setCursor(QCursor(Qt::ArrowCursor));
        network->setFocusPolicy(Qt::TabFocus);
        baseStationLogo = new QLabel(network);
        baseStationLogo->setObjectName(QString::fromUtf8("baseStationLogo"));
        baseStationLogo->setGeometry(QRect(30, 10, 41, 31));
        baseStationLogo->setPixmap(QPixmap(QString::fromUtf8(":/images/wi-fi-icon.png")));
        baseStationLogo->setScaledContents(true);
        searchButton = new QPushButton(network);
        searchButton->setObjectName(QString::fromUtf8("searchButton"));
        searchButton->setGeometry(QRect(680, 10, 70, 31));
        searchButton->setStyleSheet(QString::fromUtf8("background-color: #e5e5e5;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/search-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        searchButton->setIcon(icon);
        searchButton->setIconSize(QSize(35, 35));
        lteLabel = new QLabel(network);
        lteLabel->setObjectName(QString::fromUtf8("lteLabel"));
        lteLabel->setGeometry(QRect(110, 10, 171, 31));
        lteLabel->setFont(font);
        lteLabel->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        lteLabel->setScaledContents(true);
        searchResults = new QListWidget(network);
        searchResults->setObjectName(QString::fromUtf8("searchResults"));
        searchResults->setGeometry(QRect(20, 70, 251, 301));
        searchResults->setStyleSheet(QString::fromUtf8("background-color: #ffffff;\n"
"border: #121212;\n"
"colour:#0065a4;\n"
""));
        searchResults->setLayoutMode(QListView::SinglePass);
        apnName = new QListWidget(network);
        apnName->setObjectName(QString::fromUtf8("apnName"));
        apnName->setGeometry(QRect(280, 70, 491, 301));
        apnName->setStyleSheet(QString::fromUtf8("coour:#0065a4;"));
        label_9 = new QLabel(network);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(0, 0, 801, 441));
        label_9->setStyleSheet(QString::fromUtf8("background-color: #e5e5e5;"));
        label_11 = new QLabel(network);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(0, 380, 801, 41));
        label_11->setStyleSheet(QString::fromUtf8("colour:#0065a4;"));
        label_17 = new QLabel(network);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(20, 52, 81, 16));
        label_17->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        label_18 = new QLabel(network);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(279, 50, 71, 20));
        label_18->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/network-icon-unselected.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon1.addFile(QString::fromUtf8(":/images/network-icon-selected.png"), QSize(), QIcon::Normal, QIcon::On);
        uConnectTabWidget->addTab(network, icon1, QString());
        label_9->raise();
        baseStationLogo->raise();
        searchButton->raise();
        lteLabel->raise();
        searchResults->raise();
        apnName->raise();
        label_11->raise();
        label_17->raise();
        label_18->raise();
        configure = new QWidget();
        configure->setObjectName(QString::fromUtf8("configure"));
        groupBox_2 = new QGroupBox(configure);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(50, 20, 591, 81));
        QFont font1;
        font1.setPointSize(11);
        groupBox_2->setFont(font1);
        groupBox_2->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 20, 46, 31));
        label_6->setFont(font);
        label_6->setScaledContents(true);
        radioAuto = new QRadioButton(groupBox_2);
        radioAuto->setObjectName(QString::fromUtf8("radioAuto"));
        radioAuto->setGeometry(QRect(400, 20, 82, 17));
        radioAuto->setChecked(true);
        radioManual = new QRadioButton(groupBox_2);
        radioManual->setObjectName(QString::fromUtf8("radioManual"));
        radioManual->setGeometry(QRect(400, 50, 82, 17));
        radioManual->setCheckable(false);
        groupBox_3 = new QGroupBox(configure);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(50, 129, 591, 101));
        groupBox_3->setFont(font1);
        groupBox_3->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        groupBox_3->setCheckable(false);
        groupBox_3->setChecked(false);
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 20, 51, 31));
        label_7->setFont(font);
        label_7->setScaledContents(true);
        radioDebug = new QRadioButton(groupBox_3);
        radioDebug->setObjectName(QString::fromUtf8("radioDebug"));
        radioDebug->setGeometry(QRect(400, 20, 82, 17));
        radioDebug->setChecked(true);
        radioInfo = new QRadioButton(groupBox_3);
        radioInfo->setObjectName(QString::fromUtf8("radioInfo"));
        radioInfo->setGeometry(QRect(400, 44, 82, 21));
        radioError = new QRadioButton(groupBox_3);
        radioError->setObjectName(QString::fromUtf8("radioError"));
        radioError->setGeometry(QRect(400, 70, 82, 17));
        groupBox_4 = new QGroupBox(configure);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(50, 250, 591, 80));
        groupBox_4->setFont(font1);
        groupBox_4->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        logFileName = new QLineEdit(groupBox_4);
        logFileName->setObjectName(QString::fromUtf8("logFileName"));
        logFileName->setGeometry(QRect(30, 30, 161, 31));
        fileLogginOption = new QCheckBox(groupBox_4);
        fileLogginOption->setObjectName(QString::fromUtf8("fileLogginOption"));
        fileLogginOption->setGeometry(QRect(402, 30, 131, 31));
        fileLogginOption->setFont(font1);
        label_8 = new QLabel(configure);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(0, -7, 801, 391));
        label_8->setStyleSheet(QString::fromUtf8("background-color: #e5e5e5;"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/configure-icon-unselected.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon2.addFile(QString::fromUtf8(":/images/configure-icon-selected.png"), QSize(), QIcon::Normal, QIcon::On);
        uConnectTabWidget->addTab(configure, icon2, QString());
        label_8->raise();
        groupBox_2->raise();
        groupBox_3->raise();
        groupBox_4->raise();
        debugTool = new QWidget();
        debugTool->setObjectName(QString::fromUtf8("debugTool"));
        groupBox_5 = new QGroupBox(debugTool);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 10, 271, 81));
        QPalette palette1;
        QBrush brush2(QColor(0, 101, 164, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        groupBox_5->setPalette(palette1);
        groupBox_5->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        RSSIProgressBar = new QProgressBar(groupBox_5);
        RSSIProgressBar->setObjectName(QString::fromUtf8("RSSIProgressBar"));
        RSSIProgressBar->setGeometry(QRect(70, 20, 135, 20));
        RSSIProgressBar->setStyleSheet(QString::fromUtf8("color: #eb7012;"));
        RSSIProgressBar->setValue(0);
        CINRProgressBar = new QProgressBar(groupBox_5);
        CINRProgressBar->setObjectName(QString::fromUtf8("CINRProgressBar"));
        CINRProgressBar->setGeometry(QRect(70, 50, 131, 20));
        CINRProgressBar->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        CINRProgressBar->setValue(0);
        label = new QLabel(groupBox_5);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(14, 20, 46, 20));
        label->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        label->setScaledContents(true);
        label_2 = new QLabel(groupBox_5);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(15, 50, 46, 21));
        label_2->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        label_2->setScaledContents(true);
        rssiValueEdit = new QLineEdit(groupBox_5);
        rssiValueEdit->setObjectName(QString::fromUtf8("rssiValueEdit"));
        rssiValueEdit->setGeometry(QRect(174, 20, 51, 21));
        rssiValueEdit->setStyleSheet(QString::fromUtf8("color: #0065a4;\n"
"background-color: #e5e5e5;\n"
"border: 0px solid #1c1c1c;"));
        cinrValueEdit = new QLabel(groupBox_5);
        cinrValueEdit->setObjectName(QString::fromUtf8("cinrValueEdit"));
        cinrValueEdit->setGeometry(QRect(174, 49, 46, 20));
        groupBox_6 = new QGroupBox(debugTool);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(10, 100, 271, 81));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        groupBox_6->setPalette(palette2);
        groupBox_6->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        Refresh_checkBox = new QCheckBox(groupBox_6);
        Refresh_checkBox->setObjectName(QString::fromUtf8("Refresh_checkBox"));
        Refresh_checkBox->setGeometry(QRect(17, 15, 151, 21));
        Refresh_checkBox->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        label_3 = new QLabel(groupBox_6);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 41, 91, 21));
        label_3->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        label_3->setScaledContents(true);
        m_editRefreshInterval = new QLineEdit(groupBox_6);
        m_editRefreshInterval->setObjectName(QString::fromUtf8("m_editRefreshInterval"));
        m_editRefreshInterval->setGeometry(QRect(110, 40, 51, 21));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_editRefreshInterval->sizePolicy().hasHeightForWidth());
        m_editRefreshInterval->setSizePolicy(sizePolicy);
        m_editRefreshInterval->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        m_editRefreshInterval->setReadOnly(false);
        label_4 = new QLabel(groupBox_6);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(166, 40, 46, 20));
        label_4->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        GetModStats = new QPushButton(debugTool);
        GetModStats->setObjectName(QString::fromUtf8("GetModStats"));
        GetModStats->setGeometry(QRect(730, 10, 61, 31));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Verdana"));
        font2.setPointSize(8);
        GetModStats->setFont(font2);
        GetModStats->setStyleSheet(QString::fromUtf8("background-color: #0065a4;\n"
"border-top-left-radius: 8px;\n"
"border-top-right-radius: 8px;\n"
"border-bottom-left-radius: 8px;\n"
"border-bottom-right-radius: 8px;\n"
"color:#e5e5e5;"));
        TreeStat = new QTreeWidget(debugTool);
        TreeStat->setObjectName(QString::fromUtf8("TreeStat"));
        TreeStat->setGeometry(QRect(300, 10, 421, 361));
        TreeStat->setAutoFillBackground(false);
        TreeStat->setStyleSheet(QString::fromUtf8("colour:#0065a4;"));
        TreeStat->setAlternatingRowColors(true);
        groupBox_7 = new QGroupBox(debugTool);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setGeometry(QRect(10, 190, 271, 91));
        groupBox_7->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        modemState = new QListWidget(groupBox_7);
        modemState->setObjectName(QString::fromUtf8("modemState"));
        modemState->setGeometry(QRect(90, 30, 151, 27));
        modemState->setStyleSheet(QString::fromUtf8("color: #0065a4;"));
        label_12 = new QLabel(groupBox_7);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 30, 71, 28));
        QFont font3;
        font3.setPointSize(8);
        label_12->setFont(font3);
        label_12->setStyleSheet(QString::fromUtf8("color:#0065a4;"));
        label_12->setScaledContents(true);
        label_10 = new QLabel(debugTool);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(-10, 0, 811, 381));
        label_10->setStyleSheet(QString::fromUtf8("background-color: #e5e5e5;"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/statistics-icon-unselected.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon3.addFile(QString::fromUtf8(":/images/statistics-icon-selected.png"), QSize(), QIcon::Normal, QIcon::On);
        icon3.addFile(QString::fromUtf8(":/images/statistics-icon-unselected.png"), QSize(), QIcon::Active, QIcon::Off);
        icon3.addFile(QString::fromUtf8(":/images/statistics-icon-unselected.png"), QSize(), QIcon::Selected, QIcon::Off);
        uConnectTabWidget->addTab(debugTool, icon3, QString());
        label_10->raise();
        groupBox_5->raise();
        groupBox_6->raise();
        GetModStats->raise();
        TreeStat->raise();
        groupBox_7->raise();
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        Info_treewidget = new QTreeWidget(tab);
        Info_treewidget->setObjectName(QString::fromUtf8("Info_treewidget"));
        Info_treewidget->setGeometry(QRect(30, 21, 301, 341));
        vBox = new QWidget(tab);
        vBox->setObjectName(QString::fromUtf8("vBox"));
        vBox->setGeometry(QRect(370, 20, 411, 341));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/info-icon-unselected.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon4.addFile(QString::fromUtf8(":/images/info-icon-selected.png"), QSize(), QIcon::Normal, QIcon::On);
        icon4.addFile(QString::fromUtf8(":/images/info-icon-unselected.png"), QSize(), QIcon::Disabled, QIcon::Off);
        icon4.addFile(QString::fromUtf8(":/images/info-icon-unselected.png"), QSize(), QIcon::Active, QIcon::Off);
        icon4.addFile(QString::fromUtf8(":/images/info-icon-selected.png"), QSize(), QIcon::Active, QIcon::On);
        icon4.addFile(QString::fromUtf8(":/images/info-icon-unselected.png"), QSize(), QIcon::Selected, QIcon::Off);
        icon4.addFile(QString::fromUtf8(":/images/info-icon-selected.png"), QSize(), QIcon::Selected, QIcon::On);
        uConnectTabWidget->addTab(tab, icon4, QString());
        connectButton = new QToolButton(centralWidget);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setGeometry(QRect(627, 95, 81, 21));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Verdana"));
        font4.setPointSize(10);
        font4.setBold(true);
        font4.setWeight(75);
        font4.setStyleStrategy(QFont::PreferAntialias);
        connectButton->setFont(font4);
        connectButton->setFocusPolicy(Qt::StrongFocus);
        connectButton->setStyleSheet(QString::fromUtf8("border-top-left-radius: 8px;\n"
"border-top-right-radius: 8px;\n"
"border-bottom-left-radius: 8px;\n"
"border-bottom-right-radius: 8px;\n"
"color:#0065a4;\n"
"background-color: #e0e0dd;"));
        NavigationBar = new QLabel(centralWidget);
        NavigationBar->setObjectName(QString::fromUtf8("NavigationBar"));
        NavigationBar->setGeometry(QRect(0, 86, 801, 51));
        NavigationBar->setStyleSheet(QString::fromUtf8("background-color: #0065a4;"));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(0, 0, 801, 91));
        label_5->setStyleSheet(QString::fromUtf8("background-color: #fdfdfd"));
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(0, 510, 801, 31));
        label_13->setStyleSheet(QString::fromUtf8("background-color: #0065a4;"));
        serverState = new QLabel(centralWidget);
        serverState->setObjectName(QString::fromUtf8("serverState"));
        serverState->setGeometry(QRect(500, 510, 271, 31));
        serverState->setStyleSheet(QString::fromUtf8("color:#fdfdfd;"));
        deviceMsg = new QLabel(centralWidget);
        deviceMsg->setObjectName(QString::fromUtf8("deviceMsg"));
        deviceMsg->setGeometry(QRect(20, 510, 241, 31));
        deviceMsg->setStyleSheet(QString::fromUtf8("color:#fdfdfd;"));
        deviceMsg->setScaledContents(true);
        helpButton = new QPushButton(centralWidget);
        helpButton->setObjectName(QString::fromUtf8("helpButton"));
        helpButton->setGeometry(QRect(722, 95, 61, 20));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Verdana"));
        font5.setPointSize(10);
        font5.setBold(true);
        font5.setWeight(75);
        helpButton->setFont(font5);
        helpButton->setStyleSheet(QString::fromUtf8("background-color: #e0e0dd;\n"
"border-top-left-radius: 8px;\n"
"border-top-right-radius: 8px;\n"
"border-bottom-left-radius: 8px;\n"
"border-bottom-right-radius: 8px;\n"
"color:#0065a4;"));
        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(20, 10, 201, 61));
        label_14->setPixmap(QPixmap(QString::fromUtf8(":/images/altair-logo.png")));
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(482, 22, 311, 31));
        label_15->setPixmap(QPixmap(QString::fromUtf8(":/images/connection-manager.png")));
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(718, 53, 81, 21));
        label_16->setStyleSheet(QString::fromUtf8("color:#a9a9a9;\n"
"font: 100 10pt \"Trebuchet MS\";"));
        detailedView->setCentralWidget(centralWidget);
        label_5->raise();
        NavigationBar->raise();
        uConnectTabWidget->raise();
        helpButton->raise();
        label_13->raise();
        serverState->raise();
        deviceMsg->raise();
        label_16->raise();
        connectButton->raise();
        label_15->raise();
        label_14->raise();
        menuBar = new QMenuBar(detailedView);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 21));
        menuBar->setFocusPolicy(Qt::ClickFocus);
        menuBar->setContextMenuPolicy(Qt::CustomContextMenu);
        menuBar->setLayoutDirection(Qt::RightToLeft);
        menuBar->setNativeMenuBar(true);
        detailedView->setMenuBar(menuBar);
        statusBar = new QStatusBar(detailedView);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        detailedView->setStatusBar(statusBar);

        retranslateUi(detailedView);

        uConnectTabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(detailedView);
    } // setupUi

    void retranslateUi(QMainWindow *detailedView)
    {
        detailedView->setWindowTitle(QApplication::translate("detailedView", "Connection Manager", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        uConnectTabWidget->setWhatsThis(QApplication::translate("detailedView", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        baseStationLogo->setText(QString());
        searchButton->setText(QString());
        lteLabel->setText(QApplication::translate("detailedView", "Long Term Evolution (LTE)", 0, QApplication::UnicodeUTF8));
        label_9->setText(QString());
        label_11->setText(QString());
        label_17->setText(QApplication::translate("detailedView", "Manual Serach", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("detailedView", "APN Details", 0, QApplication::UnicodeUTF8));
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(network), QApplication::translate("detailedView", "&Network", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("detailedView", "Config_Mode", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("detailedView", "MODE", 0, QApplication::UnicodeUTF8));
        radioAuto->setText(QApplication::translate("detailedView", "AUTO", 0, QApplication::UnicodeUTF8));
        radioManual->setText(QApplication::translate("detailedView", "MANUAL", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("detailedView", "Log_level", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("detailedView", "LEVEL", 0, QApplication::UnicodeUTF8));
        radioDebug->setText(QApplication::translate("detailedView", "DEBUG", 0, QApplication::UnicodeUTF8));
        radioInfo->setText(QApplication::translate("detailedView", "INFO", 0, QApplication::UnicodeUTF8));
        radioError->setText(QApplication::translate("detailedView", "ERROR", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("detailedView", "LogFileName", 0, QApplication::UnicodeUTF8));
        fileLogginOption->setText(QApplication::translate("detailedView", "EnableServerLog", 0, QApplication::UnicodeUTF8));
        label_8->setText(QString());
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(configure), QApplication::translate("detailedView", "&Configure", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("detailedView", "Signal Info", 0, QApplication::UnicodeUTF8));
        RSSIProgressBar->setFormat(QString());
        CINRProgressBar->setFormat(QString());
        label->setText(QApplication::translate("detailedView", "   RSSI", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("detailedView", "   CINR", 0, QApplication::UnicodeUTF8));
        cinrValueEdit->setText(QString());
        groupBox_6->setTitle(QApplication::translate("detailedView", "Refresh", 0, QApplication::UnicodeUTF8));
        Refresh_checkBox->setText(QApplication::translate("detailedView", "       Enable Refresh", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("detailedView", "Refresh Interval", 0, QApplication::UnicodeUTF8));
        m_editRefreshInterval->setText(QApplication::translate("detailedView", "2", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("detailedView", "seconds", 0, QApplication::UnicodeUTF8));
        GetModStats->setText(QApplication::translate("detailedView", "GetStats", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = TreeStat->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("detailedView", "Value", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("detailedView", "StateName", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QApplication::translate("detailedView", "Modem Info", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("detailedView", "Modem State", 0, QApplication::UnicodeUTF8));
        label_10->setText(QString());
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(debugTool), QApplication::translate("detailedView", "&Statistics", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = Info_treewidget->headerItem();
        ___qtreewidgetitem1->setText(1, QApplication::translate("detailedView", "Values", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(0, QApplication::translate("detailedView", "Parameter", 0, QApplication::UnicodeUTF8));
        uConnectTabWidget->setTabText(uConnectTabWidget->indexOf(tab), QApplication::translate("detailedView", "&General ", 0, QApplication::UnicodeUTF8));
        connectButton->setText(QApplication::translate("detailedView", "Connect", 0, QApplication::UnicodeUTF8));
        NavigationBar->setText(QString());
        label_5->setText(QString());
        label_13->setText(QString());
        serverState->setText(QApplication::translate("detailedView", "TextLabel", 0, QApplication::UnicodeUTF8));
        deviceMsg->setText(QApplication::translate("detailedView", "TextLabel", 0, QApplication::UnicodeUTF8));
        helpButton->setText(QApplication::translate("detailedView", "Help", 0, QApplication::UnicodeUTF8));
        label_14->setText(QString());
        label_15->setText(QString());
        label_16->setText(QApplication::translate("detailedView", "Version 1.5", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class detailedView: public Ui_detailedView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UCONNECT_H
