/********************************************************************************
** Form generated from reading UI file 'minview.ui'
**
** Created: Tue Jul 31 10:42:59 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MINVIEW_H
#define UI_MINVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MinimumView
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLabel *label_4;
    QLabel *network_name;
    QLabel *sigStrengthLte;
    QPushButton *minconnectButton;
    QPushButton *onselectWiredPushButton;
    QPushButton *detail_pushButton;
    QLabel *wifiNetworkName;
    QPushButton *help_pushButton;
    QLabel *connectionTime;
    QPushButton *versionPushButton;
    QFrame *lteFrame;
    QPushButton *onselectLtePushButton;
    QPushButton *lte4gInfoUp;
    QComboBox *apnComboBox;
    QFrame *lteStatsFrame;
    QLabel *upLoadLabel;
    QLabel *downloadLabel;
    QPushButton *lte4gInfoDown;
    QLabel *upload_rate;
    QLabel *download_rate;
    QLabel *lte4gInfoLabel;
    QFrame *threeGframe;
    QPushButton *onselect3gPushButton;
    QPushButton *threeGInfoUp;
    QComboBox *threeGComboBox;
    QFrame *threegStatsframe;
    QLabel *threeInfoLabel;
    QLabel *threeGDownloadLabel;
    QLabel *threeGUploadLabel;
    QLabel *threeGUpload_rate;
    QLabel *threeGDownload_rate;
    QPushButton *threeGInfoDown;
    QFrame *wifiFrame;
    QPushButton *onselectwifiPushButton;
    QComboBox *wifiComboBox;
    QPushButton *wifiInfoUp;
    QFrame *wifiStatsframe;
    QLabel *wifiInfoLabel;
    QPushButton *wifiInfoDown;
    QLabel *txLabel;
    QLabel *rxLabel;
    QLabel *tx_rate;
    QLabel *rx_rate;
    QLabel *messageBar;
    QLabel *sigStrength1;
    QLabel *sigStrength2;
    QLabel *versionNumb;
    QLabel *vpnLabel;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MinimumView)
    {
        if (MinimumView->objectName().isEmpty())
            MinimumView->setObjectName(QString::fromUtf8("MinimumView"));
        MinimumView->resize(480, 205);
        MinimumView->setMinimumSize(QSize(480, 200));
        MinimumView->setMaximumSize(QSize(480, 600));
        centralwidget = new QWidget(MinimumView);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 520, 41));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setPixmap(QPixmap(QString::fromUtf8(":/header")));
        label->setScaledContents(true);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 0, 125, 44));
        label_4->setPixmap(QPixmap(QString::fromUtf8(":/logo")));
        label_4->setScaledContents(true);
        network_name = new QLabel(centralwidget);
        network_name->setObjectName(QString::fromUtf8("network_name"));
        network_name->setGeometry(QRect(17, 143, 111, 16));
        network_name->setStyleSheet(QString::fromUtf8("color:#b6b6b6;"));
        network_name->setScaledContents(true);
        sigStrengthLte = new QLabel(centralwidget);
        sigStrengthLte->setObjectName(QString::fromUtf8("sigStrengthLte"));
        sigStrengthLte->setGeometry(QRect(14, 139, 41, 40));
        sigStrengthLte->setStyleSheet(QString::fromUtf8("background-image: url(:/signal0);"));
        sigStrengthLte->setScaledContents(true);
        minconnectButton = new QPushButton(centralwidget);
        minconnectButton->setObjectName(QString::fromUtf8("minconnectButton"));
        minconnectButton->setEnabled(true);
        minconnectButton->setGeometry(QRect(370, 0, 100, 43));
        minconnectButton->setMinimumSize(QSize(80, 30));
        minconnectButton->setMaximumSize(QSize(100, 43));
        minconnectButton->setAutoFillBackground(false);
        minconnectButton->setStyleSheet(QString::fromUtf8("image: url(:/connected);\n"
""));
        onselectWiredPushButton = new QPushButton(centralwidget);
        onselectWiredPushButton->setObjectName(QString::fromUtf8("onselectWiredPushButton"));
        onselectWiredPushButton->setGeometry(QRect(360, 40, 120, 100));
        onselectWiredPushButton->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:reflect, x1:1, y1:1, x2:1, y2:0.415, stop:0 rgba(160, 160, 160, 255), stop:1 rgba(221, 221, 221, 255));\n"
"image: url(:/onWiredUnselected);\n"
""));
        onselectWiredPushButton->setIconSize(QSize(100, 100));
        detail_pushButton = new QPushButton(centralwidget);
        detail_pushButton->setObjectName(QString::fromUtf8("detail_pushButton"));
        detail_pushButton->setGeometry(QRect(360, 150, 30, 30));
        detail_pushButton->setStyleSheet(QString::fromUtf8("border-top-left-radius: 8px;\n"
"border-top-right-radius: 8px;\n"
"border-bottom-left-radius: 8px;\n"
"border-bottom-right-radius: 8px;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/settingIcon"), QSize(), QIcon::Normal, QIcon::Off);
        detail_pushButton->setIcon(icon);
        detail_pushButton->setIconSize(QSize(30, 30));
        wifiNetworkName = new QLabel(centralwidget);
        wifiNetworkName->setObjectName(QString::fromUtf8("wifiNetworkName"));
        wifiNetworkName->setGeometry(QRect(320, 144, 81, 16));
        help_pushButton = new QPushButton(centralwidget);
        help_pushButton->setObjectName(QString::fromUtf8("help_pushButton"));
        help_pushButton->setGeometry(QRect(400, 150, 30, 30));
        QFont font;
        font.setFamily(QString::fromUtf8("Verdana"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        help_pushButton->setFont(font);
        help_pushButton->setStyleSheet(QString::fromUtf8("border-top-left-radius: 8px;\n"
"border-top-right-radius: 8px;\n"
"border-bottom-left-radius: 8px;\n"
"border-bottom-right-radius: 8px;\n"
""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/minHelp"), QSize(), QIcon::Normal, QIcon::Off);
        help_pushButton->setIcon(icon1);
        help_pushButton->setIconSize(QSize(30, 30));
        connectionTime = new QLabel(centralwidget);
        connectionTime->setObjectName(QString::fromUtf8("connectionTime"));
        connectionTime->setGeometry(QRect(240, 160, 16, 16));
        versionPushButton = new QPushButton(centralwidget);
        versionPushButton->setObjectName(QString::fromUtf8("versionPushButton"));
        versionPushButton->setGeometry(QRect(440, 150, 30, 30));
        versionPushButton->setStyleSheet(QString::fromUtf8("border-top-left-radius: 8px;\n"
"border-top-right-radius: 8px;\n"
"border-bottom-left-radius: 8px;\n"
"border-bottom-right-radius: 8px;"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/infoIcon"), QSize(), QIcon::Normal, QIcon::Off);
        versionPushButton->setIcon(icon2);
        versionPushButton->setIconSize(QSize(30, 30));
        lteFrame = new QFrame(centralwidget);
        lteFrame->setObjectName(QString::fromUtf8("lteFrame"));
        lteFrame->setGeometry(QRect(0, 40, 120, 100));
        lteFrame->setFrameShape(QFrame::StyledPanel);
        lteFrame->setFrameShadow(QFrame::Raised);
        onselectLtePushButton = new QPushButton(lteFrame);
        onselectLtePushButton->setObjectName(QString::fromUtf8("onselectLtePushButton"));
        onselectLtePushButton->setGeometry(QRect(0, 0, 120, 100));
        onselectLtePushButton->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));\n"
"image: url(:/onLteSelected);\n"
"\n"
"\n"
""));
        onselectLtePushButton->setIconSize(QSize(100, 100));
        lte4gInfoUp = new QPushButton(lteFrame);
        lte4gInfoUp->setObjectName(QString::fromUtf8("lte4gInfoUp"));
        lte4gInfoUp->setGeometry(QRect(0, 0, 120, 20));
        lte4gInfoUp->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/DownArrow"), QSize(), QIcon::Normal, QIcon::Off);
        lte4gInfoUp->setIcon(icon3);
        lte4gInfoUp->setIconSize(QSize(200, 40));
        apnComboBox = new QComboBox(lteFrame);
        apnComboBox->setObjectName(QString::fromUtf8("apnComboBox"));
        apnComboBox->setGeometry(QRect(10, 70, 100, 16));
        lteStatsFrame = new QFrame(lteFrame);
        lteStatsFrame->setObjectName(QString::fromUtf8("lteStatsFrame"));
        lteStatsFrame->setGeometry(QRect(0, 0, 120, 100));
        lteStatsFrame->setStyleSheet(QString::fromUtf8(""));
        lteStatsFrame->setFrameShape(QFrame::StyledPanel);
        lteStatsFrame->setFrameShadow(QFrame::Raised);
        upLoadLabel = new QLabel(lteStatsFrame);
        upLoadLabel->setObjectName(QString::fromUtf8("upLoadLabel"));
        upLoadLabel->setGeometry(QRect(0, 7, 20, 20));
        upLoadLabel->setStyleSheet(QString::fromUtf8("image: url(:/upload-icon);"));
        downloadLabel = new QLabel(lteStatsFrame);
        downloadLabel->setObjectName(QString::fromUtf8("downloadLabel"));
        downloadLabel->setGeometry(QRect(58, 7, 20, 20));
        downloadLabel->setStyleSheet(QString::fromUtf8("image: url(:/download-icon);"));
        lte4gInfoDown = new QPushButton(lteStatsFrame);
        lte4gInfoDown->setObjectName(QString::fromUtf8("lte4gInfoDown"));
        lte4gInfoDown->setGeometry(QRect(0, 75, 120, 30));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lte4gInfoDown->sizePolicy().hasHeightForWidth());
        lte4gInfoDown->setSizePolicy(sizePolicy1);
        lte4gInfoDown->setMaximumSize(QSize(120, 30));
        lte4gInfoDown->setStyleSheet(QString::fromUtf8("background-color: transparent;\n"
""));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/UpArrow"), QSize(), QIcon::Normal, QIcon::Off);
        lte4gInfoDown->setIcon(icon4);
        lte4gInfoDown->setIconSize(QSize(200, 100));
        upload_rate = new QLabel(lteStatsFrame);
        upload_rate->setObjectName(QString::fromUtf8("upload_rate"));
        upload_rate->setGeometry(QRect(18, 9, 41, 16));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(7);
        font1.setBold(false);
        font1.setWeight(50);
        upload_rate->setFont(font1);
        upload_rate->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        download_rate = new QLabel(lteStatsFrame);
        download_rate->setObjectName(QString::fromUtf8("download_rate"));
        download_rate->setGeometry(QRect(76, 10, 41, 16));
        download_rate->setFont(font1);
        download_rate->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        lte4gInfoLabel = new QLabel(lteStatsFrame);
        lte4gInfoLabel->setObjectName(QString::fromUtf8("lte4gInfoLabel"));
        lte4gInfoLabel->setGeometry(QRect(0, 0, 120, 100));
        lte4gInfoLabel->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0, 0, 0, 100), stop:1 rgba(36, 36, 36, 100));"));
        lte4gInfoLabel->raise();
        downloadLabel->raise();
        download_rate->raise();
        lte4gInfoDown->raise();
        upLoadLabel->raise();
        upload_rate->raise();
        threeGframe = new QFrame(centralwidget);
        threeGframe->setObjectName(QString::fromUtf8("threeGframe"));
        threeGframe->setGeometry(QRect(120, 40, 120, 100));
        threeGframe->setFrameShape(QFrame::StyledPanel);
        threeGframe->setFrameShadow(QFrame::Raised);
        onselect3gPushButton = new QPushButton(threeGframe);
        onselect3gPushButton->setObjectName(QString::fromUtf8("onselect3gPushButton"));
        onselect3gPushButton->setGeometry(QRect(0, 0, 120, 100));
        onselect3gPushButton->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));\n"
"image: url(:/on3gSelected);\n"
"\n"
"\n"
""));
        onselect3gPushButton->setIconSize(QSize(100, 100));
        threeGInfoUp = new QPushButton(threeGframe);
        threeGInfoUp->setObjectName(QString::fromUtf8("threeGInfoUp"));
        threeGInfoUp->setEnabled(true);
        threeGInfoUp->setGeometry(QRect(0, 0, 120, 20));
        threeGInfoUp->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        threeGInfoUp->setIcon(icon3);
        threeGInfoUp->setIconSize(QSize(200, 40));
        threeGComboBox = new QComboBox(threeGframe);
        threeGComboBox->setObjectName(QString::fromUtf8("threeGComboBox"));
        threeGComboBox->setGeometry(QRect(10, 70, 100, 16));
        threegStatsframe = new QFrame(threeGframe);
        threegStatsframe->setObjectName(QString::fromUtf8("threegStatsframe"));
        threegStatsframe->setGeometry(QRect(0, 0, 120, 100));
        threegStatsframe->setFrameShape(QFrame::StyledPanel);
        threegStatsframe->setFrameShadow(QFrame::Raised);
        threeInfoLabel = new QLabel(threegStatsframe);
        threeInfoLabel->setObjectName(QString::fromUtf8("threeInfoLabel"));
        threeInfoLabel->setGeometry(QRect(1, 0, 120, 100));
        threeInfoLabel->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0, 0, 0, 100), stop:1 rgba(36, 36, 36, 100));"));
        threeGDownloadLabel = new QLabel(threegStatsframe);
        threeGDownloadLabel->setObjectName(QString::fromUtf8("threeGDownloadLabel"));
        threeGDownloadLabel->setGeometry(QRect(61, 9, 20, 20));
        threeGDownloadLabel->setStyleSheet(QString::fromUtf8("image: url(:/download-icon);"));
        threeGDownloadLabel->setScaledContents(true);
        threeGUploadLabel = new QLabel(threegStatsframe);
        threeGUploadLabel->setObjectName(QString::fromUtf8("threeGUploadLabel"));
        threeGUploadLabel->setGeometry(QRect(1, 8, 20, 20));
        threeGUploadLabel->setStyleSheet(QString::fromUtf8("image: url(:/upload-icon);"));
        threeGUploadLabel->setScaledContents(true);
        threeGUpload_rate = new QLabel(threegStatsframe);
        threeGUpload_rate->setObjectName(QString::fromUtf8("threeGUpload_rate"));
        threeGUpload_rate->setGeometry(QRect(18, 8, 41, 20));
        threeGUpload_rate->setFont(font1);
        threeGUpload_rate->setStyleSheet(QString::fromUtf8("color: rgb(255, 255,255);"));
        threeGDownload_rate = new QLabel(threegStatsframe);
        threeGDownload_rate->setObjectName(QString::fromUtf8("threeGDownload_rate"));
        threeGDownload_rate->setGeometry(QRect(77, 9, 40, 20));
        threeGDownload_rate->setFont(font1);
        threeGDownload_rate->setStyleSheet(QString::fromUtf8("color: rgb(255,255, 255);"));
        threeGInfoDown = new QPushButton(threegStatsframe);
        threeGInfoDown->setObjectName(QString::fromUtf8("threeGInfoDown"));
        threeGInfoDown->setGeometry(QRect(1, 75, 120, 30));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(threeGInfoDown->sizePolicy().hasHeightForWidth());
        threeGInfoDown->setSizePolicy(sizePolicy2);
        threeGInfoDown->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        threeGInfoDown->setIcon(icon4);
        threeGInfoDown->setIconSize(QSize(200, 100));
        threeInfoLabel->raise();
        threeGInfoDown->raise();
        threeGDownload_rate->raise();
        threeGDownloadLabel->raise();
        threeGUploadLabel->raise();
        threeGUpload_rate->raise();
        wifiFrame = new QFrame(centralwidget);
        wifiFrame->setObjectName(QString::fromUtf8("wifiFrame"));
        wifiFrame->setGeometry(QRect(240, 40, 120, 100));
        wifiFrame->setFrameShape(QFrame::StyledPanel);
        wifiFrame->setFrameShadow(QFrame::Raised);
        onselectwifiPushButton = new QPushButton(wifiFrame);
        onselectwifiPushButton->setObjectName(QString::fromUtf8("onselectwifiPushButton"));
        onselectwifiPushButton->setGeometry(QRect(0, 0, 120, 100));
        onselectwifiPushButton->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));\n"
"image: url(:/onWifiSelected);\n"
"\n"
""));
        onselectwifiPushButton->setIconSize(QSize(100, 100));
        wifiComboBox = new QComboBox(wifiFrame);
        wifiComboBox->setObjectName(QString::fromUtf8("wifiComboBox"));
        wifiComboBox->setGeometry(QRect(10, 70, 100, 16));
        wifiInfoUp = new QPushButton(wifiFrame);
        wifiInfoUp->setObjectName(QString::fromUtf8("wifiInfoUp"));
        wifiInfoUp->setEnabled(true);
        wifiInfoUp->setGeometry(QRect(-1, 0, 120, 20));
        wifiInfoUp->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        wifiInfoUp->setIcon(icon3);
        wifiInfoUp->setIconSize(QSize(200, 40));
        wifiStatsframe = new QFrame(wifiFrame);
        wifiStatsframe->setObjectName(QString::fromUtf8("wifiStatsframe"));
        wifiStatsframe->setGeometry(QRect(0, 0, 120, 100));
        wifiStatsframe->setStyleSheet(QString::fromUtf8(""));
        wifiStatsframe->setFrameShape(QFrame::StyledPanel);
        wifiStatsframe->setFrameShadow(QFrame::Raised);
        wifiInfoLabel = new QLabel(wifiStatsframe);
        wifiInfoLabel->setObjectName(QString::fromUtf8("wifiInfoLabel"));
        wifiInfoLabel->setGeometry(QRect(0, 0, 120, 100));
        wifiInfoLabel->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0, 0, 0, 100), stop:1 rgba(36, 36, 36, 100));"));
        wifiInfoDown = new QPushButton(wifiStatsframe);
        wifiInfoDown->setObjectName(QString::fromUtf8("wifiInfoDown"));
        wifiInfoDown->setGeometry(QRect(0, 75, 120, 30));
        sizePolicy2.setHeightForWidth(wifiInfoDown->sizePolicy().hasHeightForWidth());
        wifiInfoDown->setSizePolicy(sizePolicy2);
        wifiInfoDown->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        wifiInfoDown->setIcon(icon4);
        wifiInfoDown->setIconSize(QSize(200, 100));
        txLabel = new QLabel(wifiStatsframe);
        txLabel->setObjectName(QString::fromUtf8("txLabel"));
        txLabel->setGeometry(QRect(-1, 7, 20, 20));
        txLabel->setStyleSheet(QString::fromUtf8("image: url(:/upload-icon);"));
        txLabel->setScaledContents(true);
        rxLabel = new QLabel(wifiStatsframe);
        rxLabel->setObjectName(QString::fromUtf8("rxLabel"));
        rxLabel->setGeometry(QRect(55, 11, 20, 21));
        rxLabel->setStyleSheet(QString::fromUtf8("image: url(:/download-icon);"));
        rxLabel->setScaledContents(true);
        tx_rate = new QLabel(wifiStatsframe);
        tx_rate->setObjectName(QString::fromUtf8("tx_rate"));
        tx_rate->setGeometry(QRect(15, 10, 41, 22));
        tx_rate->setFont(font1);
        tx_rate->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        rx_rate = new QLabel(wifiStatsframe);
        rx_rate->setObjectName(QString::fromUtf8("rx_rate"));
        rx_rate->setGeometry(QRect(71, 11, 40, 22));
        rx_rate->setFont(font1);
        rx_rate->setStyleSheet(QString::fromUtf8("color: rgb(255,255, 255);"));
        wifiInfoLabel->raise();
        wifiInfoDown->raise();
        rx_rate->raise();
        rxLabel->raise();
        txLabel->raise();
        tx_rate->raise();
        messageBar = new QLabel(centralwidget);
        messageBar->setObjectName(QString::fromUtf8("messageBar"));
        messageBar->setGeometry(QRect(0, 181, 351, 20));
        sigStrength1 = new QLabel(centralwidget);
        sigStrength1->setObjectName(QString::fromUtf8("sigStrength1"));
        sigStrength1->setGeometry(QRect(40, 139, 41, 41));
        sigStrength1->setStyleSheet(QString::fromUtf8("background-image: url(:/signal0);"));
        sigStrength1->setScaledContents(true);
        sigStrength2 = new QLabel(centralwidget);
        sigStrength2->setObjectName(QString::fromUtf8("sigStrength2"));
        sigStrength2->setGeometry(QRect(80, 139, 41, 41));
        sigStrength2->setStyleSheet(QString::fromUtf8("background-image: url(:/signal0);"));
        sigStrength2->setScaledContents(true);
        versionNumb = new QLabel(centralwidget);
        versionNumb->setObjectName(QString::fromUtf8("versionNumb"));
        versionNumb->setGeometry(QRect(145, 24, 41, 16));
        versionNumb->setStyleSheet(QString::fromUtf8("font: 87 9pt \"Arial Black\";\n"
"color:rgb(0, 0, 0)"));
        vpnLabel = new QLabel(centralwidget);
        vpnLabel->setObjectName(QString::fromUtf8("vpnLabel"));
        vpnLabel->setGeometry(QRect(4, 160, 15, 15));
        MinimumView->setCentralWidget(centralwidget);
        label->raise();
        label_4->raise();
        network_name->raise();
        sigStrengthLte->raise();
        onselectWiredPushButton->raise();
        detail_pushButton->raise();
        wifiNetworkName->raise();
        help_pushButton->raise();
        connectionTime->raise();
        versionPushButton->raise();
        lteFrame->raise();
        threeGframe->raise();
        wifiFrame->raise();
        messageBar->raise();
        minconnectButton->raise();
        sigStrength1->raise();
        sigStrength2->raise();
        versionNumb->raise();
        vpnLabel->raise();
        menubar = new QMenuBar(MinimumView);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 480, 21));
        MinimumView->setMenuBar(menubar);

        retranslateUi(MinimumView);

        QMetaObject::connectSlotsByName(MinimumView);
    } // setupUi

    void retranslateUi(QMainWindow *MinimumView)
    {
        MinimumView->setWindowTitle(QApplication::translate("MinimumView", "MainWindow", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_4->setText(QString());
        network_name->setText(QString());
        sigStrengthLte->setText(QString());
#ifndef QT_NO_TOOLTIP
        minconnectButton->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        minconnectButton->setText(QString());
        onselectWiredPushButton->setText(QString());
        detail_pushButton->setText(QString());
        wifiNetworkName->setText(QString());
        help_pushButton->setText(QString());
        connectionTime->setText(QString());
        versionPushButton->setText(QString());
        onselectLtePushButton->setText(QString());
        lte4gInfoUp->setText(QString());
        apnComboBox->clear();
        apnComboBox->insertItems(0, QStringList()
         << QApplication::translate("MinimumView", "4G/LTE Networks", 0, QApplication::UnicodeUTF8)
        );
        upLoadLabel->setText(QString());
        downloadLabel->setText(QString());
        lte4gInfoDown->setText(QString());
        upload_rate->setText(QApplication::translate("MinimumView", " 0Mbps ", 0, QApplication::UnicodeUTF8));
        download_rate->setText(QApplication::translate("MinimumView", " 0Mbps ", 0, QApplication::UnicodeUTF8));
        lte4gInfoLabel->setText(QString());
        onselect3gPushButton->setText(QString());
        threeGInfoUp->setText(QString());
        threeGComboBox->clear();
        threeGComboBox->insertItems(0, QStringList()
         << QApplication::translate("MinimumView", "3G Network", 0, QApplication::UnicodeUTF8)
        );
        threeInfoLabel->setText(QString());
        threeGDownloadLabel->setText(QString());
        threeGUploadLabel->setText(QString());
        threeGUpload_rate->setText(QApplication::translate("MinimumView", " 0 Kbps", 0, QApplication::UnicodeUTF8));
        threeGDownload_rate->setText(QApplication::translate("MinimumView", " 0 Kbps", 0, QApplication::UnicodeUTF8));
        threeGInfoDown->setText(QString());
        onselectwifiPushButton->setText(QString());
        wifiComboBox->clear();
        wifiComboBox->insertItems(0, QStringList()
         << QApplication::translate("MinimumView", "WiFi Networks", 0, QApplication::UnicodeUTF8)
        );
        wifiInfoUp->setText(QString());
        wifiInfoLabel->setText(QString());
        wifiInfoDown->setText(QString());
        txLabel->setText(QString());
        rxLabel->setText(QString());
        tx_rate->setText(QApplication::translate("MinimumView", " 0 Mbps", 0, QApplication::UnicodeUTF8));
        rx_rate->setText(QApplication::translate("MinimumView", " 0 Mbps", 0, QApplication::UnicodeUTF8));
        messageBar->setText(QString());
        sigStrength1->setText(QString());
        sigStrength2->setText(QString());
        versionNumb->setText(QString());
        vpnLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MinimumView: public Ui_MinimumView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MINVIEW_H
