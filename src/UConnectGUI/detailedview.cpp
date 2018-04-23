#include "detailedview.h"
#include "ui_detailedview.h"
#include "uconnect.h"
#include <sstream>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include "UCommonFunctions.h"

#define MAX_TIME_TO_SET_PREFERENCE 2000
extern TechDetails gTechDetails[];
extern U_UINT32 gPriorityList[MAX_NUMBER_OF_NETWORKMANAGERS];

//extern U_UINT32            guiLogLevel;
#define GETVARNAME(x) #x
std::vector<string> vStr;
std::vector<string> meansStr;

DetailedView::DetailedView(QWidget *) :

        ui(new Ui::DetailedView),pUConnect(NULL)
{


    ui->setupUi(this);

//#ifdef ALTAIR
//       ui->uConnectTabWidget->removeTab(4);
//#endif

#ifdef BROADCOM
      ui->uConnectTabWidget->removeTab(3);
#else
       ui->uConnectTabWidget->removeTab(4);
#endif

       techPreferTimer = NULL;
       //QObject::connect(techPreferTimer,SIGNAL(timeout()),this,SLOT(setTechPriority()));

    //DetailedViewTab tab3g;
    //tab3g
    //ui->_3gPushButton
     DetailedViewTabButton tabGeneral;
     tabGeneral.button = ui->generalPushButton;
     tabGeneral.tabId = ALL_TECH;
     tabGeneral.selected = true;
     tabGeneral.checkBox = NULL;

     DetailedViewTabButton tab3g;
     tab3g.button = ui->_3gPushButton;
     tab3g.tabId = THREE_G_TECH;
     tab3g.checkBox = ui->_3GCheckBox;

     DetailedViewTabButton tabWifi;
     tabWifi.button = ui->wifiPushButton;
     tabWifi.tabId = WIFI_TECH;
     tabWifi.checkBox = ui->wifiCheckBox;

     DetailedViewTabButton tabLte;
     tabLte.button = ui->ltePushButton;
     tabLte.tabId = LTE_TECH;
     tabWifi.checkBox = ui->lteCheckBox;

     DetailedViewTabButton tabTether;
     tabTether.button = ui->tetheringPushButton;
     tabTether.tabId = VPN_TETHERING;
     tabTether.checkBox = ui->tetheringCheckBox;

     DetailedViewTabButton tabVpn;
     tabVpn.button = ui->vpnPushButton;
     tabVpn.tabId = VPN_TECH;

     this->tabButtons.push_back(tabGeneral);

     this->tabButtons.push_back(tab3g);
     this->tabButtons.push_back(tabWifi);
     this->tabButtons.push_back(tabLte);
     this->tabButtons.push_back(tabTether);
     this->tabButtons.push_back(tabVpn);

     for(unsigned int i=1;i<tabButtons.size();i++)
     {
         tabButtons[i].selected = false;
     }
     this->preferredWifiSsids = "";
     this->wifiNetworkList = "";

     this->gen.Apn = "";
     this->gen.ipAddress = "";
     this->gen.Duration = "";

}

void DetailedView::init(void *parent)
{
    pUConnect = parent;
    timerRun= false;
    timerstate = false;
    bStatWidgetInitCheck = false;
    serverLoggingEnabled = false;
    csvflag = false;
    iswifiEnabled = false;
    isWifiConnected = false;
    isLteConnected = false; //UCM check

    applyPressed = false;
    singleUssdResponse = false;

    ui->connectState->setStyleSheet("image: url(:/onButton);border:none;outline:none;");

    ui->generalPushButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:0.988727, stop:0 rgba(254, 187, 25, 255), stop:1 rgba(235, 112, 18, 255));");
    settinggenState = true;
    ui->uConnectTabWidget->clear();
    ui->uConnectTabWidget->insertTab(0,ui->generalSetting,tr("Settings"));
    ui->uConnectTabWidget->insertTab(1,ui->profiles,tr("Profiles"));
    ui->profiles->setDisabled(true);
    ui->ussd_tab->setDisabled(true);
    setting3gState = false;
    setting4gState = false;
    settingWifiState = false;
    settingTeatherState = false;
    settingVpnState = false;
    //MCSflag = false;

    isGeneralCurrentTab = true;

   //createStatus();
   createActions();
    createTrayIcon();
    setIcon();
    trayIcon->show();
    graph();
    DisplayApnTitle();
    setWindowTitle(tr("Connection Manager"));
    setWindowIcon(QIcon(":/icon"));

    QStringList labels;
    labels << tr("NETWORK") << tr("Duration");
    ui->apntableWidget->setHorizontalHeaderLabels(labels);

   /* ui->connectButton->setStyleSheet("background-image: url(:/connect_det_dis);");
    ui->connectButton->setDisabled(true);*/
    //setMinimumSize(600, 576);
    //resize(600, 576);
    ui->GetModStats->setDisabled(false);
    //fp = fopen("csv_out.csv", "a");
   // Append_CSV_title();


   isScanned = false;
   isNetworkSelected =false;

   ui->techUpPushButton->setVisible(false);
   ui->techDownPushButton->setVisible(false);
   ui->applySettingsButton->setVisible(false);

   ui->wifiPreferredList->clear();
   ui->wifiNetworkCheck->setChecked(true);
   ui->preferredNetworkCheck->setChecked(false);
   ui->tetheringPushButton->setDisabled(true);
   ui->tetheringPushButton->setStyleSheet("background-color: qlineargradient(spread:reflect, x1:1, y1:1, x2:1, y2:0.415, stop:0 rgba(160, 160, 160, 255), stop:1 rgba(221, 221, 221, 255));");
   ui->tetheringCheckBox->setDisabled(true);

   ui->generalSetting->setDisabled(true);
   ui->threeGStatistics->setDisabled(true);

   ui->debugTool->setDisabled(true);
   ui->tab->setDisabled(true);
   ui->wifiSetting->setDisabled(true);
   ui->vpn->setDisabled(true);
   QTableWidgetItem *dllab= new QTableWidgetItem("Download Rate");
   ui->threeGStatTableWidget->setItem(0, 0, dllab);
   QTableWidgetItem *dlval = new QTableWidgetItem("0kbps");
   ui->threeGStatTableWidget->setItem(0, 1, dlval);
   QTableWidgetItem *ullab= new QTableWidgetItem("Upload Rate");
   ui->threeGStatTableWidget->setItem(0, 0, ullab);
   QTableWidgetItem *ulval = new QTableWidgetItem("0kbps");
   ui->threeGStatTableWidget->setItem(1, 1, ulval);
   QTableWidgetItem *durlab= new QTableWidgetItem("Duration");
   ui->threeGStatTableWidget->setItem(2, 0, durlab);
   QTableWidgetItem *durval = new QTableWidgetItem("0::0::0");
   ui->threeGStatTableWidget->setItem(2, 1, durval);
   //function for refresheditcheckbox removed
  // if(gTechDetails[LTE_TECH].state == NM_CONNECTED_STATE)
  // {
		statisticUpdate();
  // }
        ui->applySettingsButton->setDisabled(true);
 // Vpn Button Setting
   if(ui->vpnAutoCheck->isChecked())
   {
       ui->vpnButton->hide();
       ui->vpnButton->setText("VPN Stop");
   }else
   {
       ui->vpnButton->show();
       ui->vpnButton->setText("VPN Start");
   }
}

DetailedView::~DetailedView()
{
    if(stSig)
    delete stSig;
    trayIcon->hide();
    if(trayIcon)
    delete trayIcon;


    delete ui;
    ///fclose(fp);
    //f.close();
}

void DetailedView::DisplayApnTitle()
{

    QStringList labels;
    labels << tr("NETWORK") << tr("Duration");
    ui->apntableWidget->setHorizontalHeaderLabels(labels);

    Searchnetwork.clear();
    Searchnetwork = "NETWORK";
    Searchnetwork.append("                            ");
    Searchnetwork.append("MCC");
    Searchnetwork.append("                            ");
    Searchnetwork.append("MNC");
    Searchnetwork.append("                            ");
    Searchnetwork.append("BSID");
    //ui->searchResults->insertItem(0,Searchnetwork);
    //ui->searchResults->item(0)->setBackgroundColor(QColor(180,180,180));
}

/***********************************************************************
*  -This fuction is called for creating Status Bar.
*  -The images of signal strenght, upload datarate
*       and download datarate with active network.
***********************************************************************/
void DetailedView::createStatus()
{
    //QDebug() << "createStatus()--Start";
    statusBar = new QStatusBar(this);

    stLTE = new QLabel("  LTE  ");
    statusBar->addWidget(stLTE,0);

    stNetValue= new QLabel("");
    statusBar->addWidget(stNetValue,3);

    downLink = new QLabel;
    downLink->setPixmap(QPixmap(tr(":/download-icon")));
    statusBar->addWidget(downLink,0);


    downLinkText = new QLabel;
    downLinkText->setText(tr("0 Mbps"));
    statusBar->addWidget(downLinkText,0);

    upLink = new QLabel;
    upLink->setPixmap(QPixmap(tr(":/upload-icon")));
    statusBar->addWidget(upLink,0);

    upLinkText = new QLabel;
    upLinkText->setText(tr("0 Mbps"));
    statusBar->addWidget(upLinkText,0);

    stSig = new QLabel;
    int nImageToShow = 0;
    if(isLteConnected)
    {
        stSig->setPixmap(QPixmap(tr(":/sig%1").arg(nImageToShow)));
        stSig->setAlignment(Qt::AlignRight);
    }
    statusBar->addWidget(stSig,0);

    //setStatusBar(statusBar);
    setMinimumSize(600,576);
    //QDebug() << "createStatus()--End";
}

void DetailedView::graph()
{
    //QWidget vBox;
    ui->vBox->setWindowTitle("Graph");

    statsGraph  = new CMGraphStats(ui->vBox);
   // statsGraph->setTitle("GRAPH");

    const int margin = 5;
    statsGraph->setContentsMargins(margin, margin, margin, margin);

   // QLabel *label = new QLabel(info, &vBox);

    QVBoxLayout *layout = new QVBoxLayout(ui->vBox);
    layout->addWidget(statsGraph);
    //layout->addWidget(label);

    ui->vBox->resize(288,230);
    ui->vBox->show();
}

void DetailedView::AddWidgetItem(QTreeWidgetItem*& pWidget, QString strName, U_UINT32 )
{
    QTreeWidgetItem *pItem = new QTreeWidgetItem(pWidget);
    pItem->setText(0,strName);
}

void DetailedView::DebugUpdateStatValues(QTreeWidgetItem*& pWidget, U_UINT32 childPos, U_UINT32 subchildPos,bool subChildFlag, U_SINT32 s32Value)
{

    if(subChildFlag == false)
        {
         pWidget->child(childPos)->setText(1,tr("%1").arg(s32Value));
     }
    else{
         pWidget->child(childPos)->child(subchildPos)->setText(1,tr("%1").arg(s32Value));
     }

}
void DetailedView::DebugUpdateStatValues(QTreeWidgetItem*& pWidget, U_UINT32 childPos, U_UINT32 subchildPos,bool subChildFlag, U_UINT32 u32Value)
{

    if(subChildFlag == false)
        {
         pWidget->child(childPos)->setText(1,tr("%1").arg(u32Value));
     }
    else{
         pWidget->child(childPos)->child(subchildPos)->setText(1,tr("%1").arg(u32Value));
     }

}

/***********************************************************************
*  Initialises the GUI with default setting when dissconnected from server.
***********************************************************************/
void DetailedView::DisconnectSetting()
{
    ui->apntableWidget->clear();
    QStringList labels;
    labels << tr("NETWORK") <<tr("Duration");
    ui->apntableWidget->setHorizontalHeaderLabels(labels);

    ui->radioAuto->setDisabled(true);
    ui->radioDebug->setDisabled(true);
    ui->radioInfo->setDisabled(true);
    ui->radioError->setDisabled(true);
    ui->radioManual->setDisabled(true);
    ui->fileLogginOption->setDisabled(true);
    ui->searchButton->setDisabled(true);
    /*ui->connectButton->setStyleSheet("background-image: url(:/connect_det_dis);");
    ui->connectButton->setDisabled(true);*/
   // ui->Refresh_checkBox->setDisabled(true);
    ui->m_editRefreshInterval->setDisabled(true);
    ui->GetModStats->setDisabled(true);
    ui->logFileName->setDisabled(true);
}

void  DetailedView::UI_deviceStateUpdate(U_UINT32 deviceState)
{
    switch(deviceState)
    {
    case NOTI_DEVICE_ERROR:

       // ui->deviceMsg->setText(tr("DEVICE STATUS::Device Error"));

        break;
    case NOTI_DEVICE_NOT_OPEN:
       // ui->deviceMsg->setText(tr("DEVICE STATUS::Device Not Open"));

        break;
    case NOTI_DEVICE_OPEN:
        //ui->deviceMsg->setText(tr("DEVICE STATUS::Device Open"));

        break;
    case NOTI_DISCONNECTED:
       // ui->deviceMsg->setText(tr("DEVICE STATUS::Disconnected"));

        break;
    case NOTI_DEVICE_CONNECTED:
      //  ui->deviceMsg->setText(tr("DEVICE STATUS::Device Connected"));

        //stMsg->clear();
       // stMsg->setText(tr("Active Connection"));
        break;
    case NOTI_DEVICE_DISCONNECTED:
       // ui->deviceMsg->setText(tr("DEVICE STATUS::Device Disconnected"));

        break;
    default:
       // ui->deviceMsg->setText(tr("DEVICE STATUS::Unknown"));
        qCritical() << "deviceState:" << deviceState;
        break;
    }
}

void DetailedView::connectErrortoServer()
{
    ui->apntableWidget->clear();
    QStringList labels;
    labels << tr("NETWORK") << tr("Duration");
    ui->apntableWidget->setHorizontalHeaderLabels(labels);

   /* ui->serverState->clear();
    ui->serverState->setText(tr("CM SERVER STATUS::Not Connected to CM Server"));*/
    //ui->modemState->clear();
    //ui->modemState->insertItem(0,tr("DISCONNECTED"));
   // stNetValue->clear();
    str = "";
    str.clear();
    if(isLteConnected)
    {
        stSig->setPixmap(QPixmap(tr(":/sig%1").arg(0)));
        stSig->setAlignment(Qt::AlignRight);
        stSig->show();
    }
    ui->RSSIProgressBar->setValue(0);
    ui->rssiValueEdit->setText(tr("dBm"));
    ui->RSSIProgressBar->update();

    ui->CINRProgressBar->setValue(0);
    ui->cinrValueEdit->setText(tr("dB"));
    ui->CINRProgressBar->update();
     //item = ui->apnName->takeItem(1);
     //delete item;


}

void DetailedView::modemUpdate()
{
   /* if( (((Uconnect *)pUConnect)->mCMInfo.m_nModemState == 1) || (((Uconnect *)pUConnect)->mCMInfo.m_nModemState == 5) )
    {
        genStr = "CONNECTED";
    }
    else
    {
        genStr = "DISCONNECTED";

    }
   // ui->modemState->clear();
    //genStr = "DISCONNECTED";
    ui->modemState->insertItem(0,genStr);*/
}
void DetailedView::modemStateUpdate()
{
   // modemUpdate();
    ui->radioAuto->setChecked(true);
    ui->radioManual->setDisabled(true);
    ui->searchButton->setDisabled(true);
}

void DetailedView::connectedToServer()
{

   /* ui->serverState->clear();
    ui->serverState->setText(tr("CM SERVER STATUS::Connected to CM Server"));*/
   // ui->modemState->clear();


    if(isLteConnected)
    {
        stSig->setPixmap(QPixmap(tr(":/sig%1").arg(0)));

        stSig->setAlignment(Qt::AlignRight);
        stSig->show();
    }
    ui->RSSIProgressBar->setValue(0);
    ui->rssiValueEdit->setText(tr("dBm"));
    ui->RSSIProgressBar->update();

    ui->CINRProgressBar->setValue(0);
    ui->cinrValueEdit->setText(tr("dB"));
    ui->CINRProgressBar->update();
    ui->apntableWidget->clear();
    QStringList labels;
    labels << tr("NETWORK") <<  tr("Duration");
    ui->apntableWidget->setHorizontalHeaderLabels(labels);

    //QStringList labels1;
    //labels1 << tr("NETWORK") << tr("MCC")<<tr("MNC") << tr("Duration");
   // ui->manualtableWidget->setHorizontalHeaderLabels(labels1);




    ui->radioAuto->setDisabled(false);
    ui->radioDebug->setDisabled(false);
    ui->radioInfo->setDisabled(false);
    ui->radioError->setDisabled(false);
    ui->radioManual->setDisabled(true);
    ui->fileLogginOption->setDisabled(false);
    ui->searchButton->setDisabled(true);
    //ui->Refresh_checkBox->setDisabled(false);
    ui->m_editRefreshInterval->setDisabled(false);
    ui->GetModStats->setDisabled(false);
    ui->logFileName->setDisabled(false);
    modemStateUpdate();
    //stNetValue->clear();
    //item = ui->apnName->takeItem(1);
   // delete item;

   // ((Uconnect*)pUConnect)->getEnabledTechsWithPriority();


}


/***********************************************************************
*  -Its the fuction called in periodic timer to update the current state,
*       this is done by calling the getconfigParam function.
*  -The state is updated on to the screen
***********************************************************************/
void DetailedView::detStateUpdate()
{
//QDebug() << "StateUpdate()--Start";
//ui->serverState->clear();


switch(((Uconnect *)pUConnect)->mServerFSMState)
    {
        case IDLE_STATE:
        case ENABLED_STATE:
            {
                //QDebug() << "StateUpdate()--IDLE_STATE||ENABLED_STATE";
                /*ui->serverState->clear();
                ui->serverState->setText(tr("CM SERVER STATUS::IDLE"));*/
                ui->searchButton->setDisabled(true);
                //stNetValue->clear();
               //ui->searchResults->clear();
                //item  = ui->searchResults->takeItem(1);
                //delete item;
                                //stMsg->clear();
                //stMsg->setText(tr("Active Connection"));
                 str.clear();
                 ui->apntableWidget->clear();
                 QStringList labels;
                 labels << tr("NETWORK") << tr("Duration");
                 ui->apntableWidget->setHorizontalHeaderLabels(labels);

               // ui->apnName->clear();
               // ui->apnName->clearFocus();
                //item  = ui->apnName->takeItem(1);
                //delete item;
                statdata = "IDLE";
                ((Uconnect *) pUConnect)->mCMInfo.ipAddress[0] = '\0';
            }
                break;
        case SEARCHING_STATE:
                {
                //QDebug() << "StateUpdate()--SEARCHING_STATE";
                /*ui->serverState->clear();
                ui->serverState->setText(tr("CM SERVER STATUS::SEARCHING"));*/
                ui->searchButton->setDisabled(true);
                ui->apntableWidget->clear();
                QStringList labels;
                labels << tr("NETWORK") << tr("Duration");
                ui->apntableWidget->setHorizontalHeaderLabels(labels);

                }
                break;
        case CONNECTING_STATE:
        case DEFINE_PRIMARY_STATE:
        case DEFINE_SECONDARY_STATE:
        case NET_ATTACH_STATE:
                {
                    //QDebug() << "StateUpdate()--CONNECTING_STATE||DEFINE_PRIMARY_STATE||DEFINE_SECONDARY_STATE||NET_ATTACH_STATE||";
                      /* ui->serverState->clear();
                    ui->serverState->setText(tr("CM SERVER STATUS::CONNECTING"));*/
                    ui->searchButton->setDisabled(true);

                }
                break;
        case CONNECTED_STATE:
                {
                //QDebug() << "StateUpdate()--CONNECTED_STATE";
               /* ui->serverState->clear();
                ui->serverState->setText(tr("CM SERVER STATUS::CONNECTED"));*/
                ui->searchButton->setDisabled(true);
                                //stMsg->clear();
               // stMsg->setText(tr("Active Connection"));
                statdata = "CONNECTED";
                QStringList labels;
                labels << tr("NETWORK") << tr("Duration");
                ui->apntableWidget->setHorizontalHeaderLabels(labels);

              }


                break;
        case DISCONNECTING_STATE:
                {
                //QDebug() << "StateUpdate()--DISCONNECTING_STATE";
                   /* ui->serverState->clear();
                    ui->serverState->setText(tr("CM SERVER STATUS::DISCONNECTING"));*/
                    ui->searchButton->setDisabled(true);
                    QStringList labels;
                    labels << tr("NETWORK") << tr("Duration");
                    ui->apntableWidget->setHorizontalHeaderLabels(labels);
                    if(isLteConnected)
                    {
                        stSig->setPixmap(QPixmap(tr(":/sig%1").arg(0)));
                        stSig->setAlignment(Qt::AlignRight);
                    }
                }
                break;
                case DISABLED_STATE:
                {
                    //QDebug() << "StateUpdate()--DISABLED_STATE";
                   /* ui->serverState->clear();
                    ui->serverState->setText(tr("CM SERVER STATUS::DISABLED"));*/
                    ui->searchButton->setDisabled(true);
                    QStringList labels;
                    labels << tr("NETWORK") <<  tr("Duration");
                    ui->apntableWidget->setHorizontalHeaderLabels(labels);


                    if(isLteConnected)
                    {
                        stSig->setPixmap(QPixmap(tr(":/sig%1").arg(0)));
                        stSig->setAlignment(Qt::AlignRight);
                    }
                    ((Uconnect *) pUConnect)->mCMInfo.ipAddress[0] = '\0';
                }
                break;

       default:
                //statdata = "";
                break;
    }
    //QDebug() << "StateUpdate()--Start";
}

/*void DetailedView::on_versionButton_clicked()
{
    QMessageBox a;
    ((Uconnect *) pUConnect)->getVersionInformation();
    a.about(this,
    tr("Version Information"),
    ((Uconnect *) pUConnect)->disp);
}*/

void DetailedView::logLevel()
{
    if(ULOG_DEBUG_LEVEL == ((Uconnect *)pUConnect)->mCMCfgInfo.Log_level)
    {
        //QDebug() << "getConfigValuesFromServer()--Log_level:ULOG_DEBUG_LEVEL";
        ui->radioInfo->setChecked(false);
        ui->radioError->setChecked(false);
        ui->radioDebug->setChecked(true);
    }
    else if(ULOG_INFO_LEVEL == ((Uconnect *)pUConnect)->mCMCfgInfo.Log_level)
    {
        //QDebug() << "getConfigValuesFromServer()--Log_level:ULOG_INFO_LEVEL";
        ui->radioDebug->setChecked(false);
        ui->radioError->setChecked(false);
        ui->radioInfo->setChecked(true);
    }
    else if(ULOG_ERROR_LEVEL == ((Uconnect *)pUConnect)->mCMCfgInfo.Log_level)
    {
        //QDebug() << "getConfigValuesFromServer()--Log_level:ULOG_ERROR_LEVEL";
        ui->radioDebug->setChecked(false);
        ui->radioInfo->setChecked(false);
        ui->radioError->setChecked(true);
    }

}

void DetailedView::enableLog()
{
    if(0 == ((Uconnect *)pUConnect)->mCMCfgInfo.Enable_Log)
    {
        //QDebug() << "getConfigValuesFromServer()--Enable_Log:0";
        ui->fileLogginOption->setChecked(false);
    }
    else if(1 == ((Uconnect *)pUConnect)->mCMCfgInfo.Enable_Log)
    {
        //QDebug() << "getConfigValuesFromServer()--Enable_Log:1";
        ui->fileLogginOption->setChecked(true);

            outFile.open(serverLogName.toUtf8(),ios::app);
            if(!outFile.is_open())
            {
                qCritical() << "Please give proper name in the edit box";
                outFile.open("serverLog.txt",ios::app);
            }
        }
        ui->logFileName->setDisabled(true);

}

void DetailedView::initSetting()
{
    //ui->deviceMsg->setText(tr("DEVICE STATUS::Device Not Open"));
    QStringList labels;
    labels << tr("NETWORK") << tr("Duration");
    ui->apntableWidget->setHorizontalHeaderLabels(labels);
    ui->radioDebug->setChecked(true);
    /*ui->serverState->clear();
    ui->serverState->setText(tr("CM SERVER STATUS::Not Connected to Server"));*/
   // ui->modemState->clear();
    //ui->modemState->insertItem(0,tr("DISCONNECTED"));
}

void  DetailedView::startRefreshTimer(int val)
{

    m_tRefreshTimer->start(val);
    timerstate = true;


}

bool  DetailedView::stopRefreshTimer()
{
    if(timerstate == true) {
        m_tRefreshTimer->stop();
        timerstate = false;
        return true;
    }
    return false;
}
void DetailedView::debugTool()
{
    if(gTechDetails[LTE_TECH].state != NM_DISABLED_STATE)
    {
        ((Uconnect*)pUConnect)->sendLteStatsRequest();
    }

    m_tRefreshTimer =  new QTimer(this);
        if(m_tRefreshTimer == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    connect(m_tRefreshTimer, SIGNAL(timeout()), this, SLOT(BuildStatWidget()));

}

std::vector<std::string>& DetailedView::tokeniseInformation(std::string &rspStr, std::vector<std::string>& vStr, char ch)
{
    std::string s = rspStr;
    std::string::size_type prev_pos = 0, pos = 0;
    while( (pos = s.find(ch, pos)) != std::string::npos )
    {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );
            vStr.push_back(substring);
            prev_pos = ++pos;
    }
    std::string substring( s.substr(prev_pos, pos-prev_pos) );
    vStr.push_back(substring);
    return vStr;

}

void DetailedView::BuildStatWidget()
{
    ((Uconnect *)pUConnect)->sendLteStatsRequest();


    //ui->statTableWidget->clear();
    statString.clear();
    vStr.clear();

//#ifdef ALTAIR
#if (defined(ALTAIR)|| !defined(BROADCOM))

    QString csvHeader;
    QString csvValue;
    string statVal(((Uconnect *)pUConnect)->m_CMmodemStats.m_nL1AStats.L1AencodStr);
    ((Uconnect *)pUConnect)->m_CMmodemStats.m_nL1AStats.L1AencodStr[0]='\0';
    processAltairStats(statVal);

    vStr.clear();

    //((Uconnect*)pUConnect)->MeasStatParams();

    std::string encodeStr;
    encodeGenParams(encodeStr);

    string measStatVal(encodeStr);

    measStatVal += ((Uconnect *)pUConnect)->m_CMmodemStats.m_nMeasStats.RSRPencodStr;
    measStatVal += ((Uconnect *)pUConnect)->m_CMmodemStats.m_nMeasStats.RSRQencodStr;
    measStatVal += ((Uconnect *)pUConnect)->m_CMmodemStats.m_nMeasStats.SINRencodStr;

    processAltairMeasStats(measStatVal);

    PlotGraph();


     statVal += measStatVal;

    writeMeasStatsToCsvFile(statVal,csvHeader,csvValue);

    csvValue.replace((QString)"\n",(QString)"");
    csvValue.replace((QString)"\r",(QString)"");

    csvValue += "\n";

    if(vStr.size()>1)
    {
        if(csvflag == false  )
        {

                csvHeader += "\n";
                write_csv(csvHeader,0);
                csvflag= true;
        }
         write_csv(csvValue,1);
    }

    vStr.clear();
    statString.clear();
    statValue.clear();
    csvValue.clear();
    csvHeader.clear();




#endif

#ifdef BROADCOM
    string statVal(((Uconnect *)pUConnect)->m_CMmodemStats.m_nBroadStats.genStats);
    statVal.append(((Uconnect *)pUConnect)->m_CMmodemStats.m_nBroadStats.macStats);
    statVal.append(((Uconnect *)pUConnect)->m_CMmodemStats.m_nBroadStats.phyStats);
    processBroadcomStats(statVal);
#endif

}

void DetailedView::PlotGraph()
{
  stStats siginfo;
  siginfo.Rsrp = ((Uconnect *)pUConnect)->m_CMmodemStats.m_nAltairStats.RSRP_Reported;
  siginfo.Rsrq = ((Uconnect *)pUConnect)->m_CMmodemStats.m_nAltairStats.RSRQ_Reported;
  siginfo.Sinr = ((Uconnect *)pUConnect)->m_CMmodemStats.m_nAltairStats.SINR_Reported;

  if(statsGraph!= NULL){
      statsGraph->updateAndPlot(siginfo);
  }
}


void DetailedView::write_title_csv()
{
    QString path = QDir::currentPath();
    path.append("/table.csv");
    QFile f(path);

     if( f.open(QIODevice::WriteOnly ) )
    {
        QTextStream ts( &f );
        QStringList strList;

        for( int r = 0; r < ui->statTableWidget->rowCount(); ++r ){
                   strList.clear();
                    strList << "\""+ui->statTableWidget->item( r, 0 )->text()+"\"";

                   ts << strList.join( "," )+"\n";
               }
    }
     f.close();
}

void DetailedView::write_csv(QString str, int valORhead)
{

    QString path = QDir::currentPath();
    path.append("/table.csv");
    QFile f(path);

    if(valORhead == 0)
    {
        if(f.exists() == true)
        {
            csvflag = true;
            return;
        }
    }
    if( !f.open(QIODevice::Append) )
    {
        cout<<"file opening error" << f.error();
    }else
    {
        QTextStream stream( &f );
        stream << str;
        f.close();
    }

}



void DetailedView::setApnname(QString apn,QString mcc,QString mnc, QString duration)
{

   // stNetValue->clear();
   // stNetValue->setText(apn);
    ui->apntableWidget->clear();
    //QTableWidget* apntableWidget = new QTableWidget(this);
    QStringList labels;
    labels << tr("NETWORK") <<tr("Duration");
    ui->apntableWidget->setHorizontalHeaderLabels(labels);
    //apntableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    //apntableWidget->verticalHeader()->hide();
    //apntableWidget->setShowGrid(false);

    gen.Apn = apn.toStdString();
    gen.Duration = duration.toStdString();
    QTableWidgetItem *item1 = new QTableWidgetItem(apn);
    ui->apntableWidget->setItem(0, 0, item1);
    QTableWidgetItem *item4 = new QTableWidgetItem(duration);
    ui->apntableWidget->setItem(0, 1, item4);

}

void DetailedView::sigIconupdateDetviewSetting(int imageNumber)
{
    if(isLteConnected)
    {
        stSig->setPixmap(QPixmap(tr(":/sig%1").arg(imageNumber)));
        stSig->setAlignment(Qt::AlignRight);
        stSig->show();
    }

}

void DetailedView::detdownlinkSetting(float downlinkValue)
{
    genStr.clear();
    float downlink_val = floor((downlinkValue*100.0)+0.5)/100.0;
    genStr = QString::number( downlink_val );
    genStr.append(tr(" Mbps"));
    downLinkText->setText(genStr);
    downLink->show();
}

void DetailedView::uplinkSetting(float uplink_val)
{
     genStr.clear();
     upLinkText->clear();
     uplink_val = floor((uplink_val*100.0)+0.5)/100.0;
     genStr = QString::number( uplink_val);
     genStr.append(tr(" Mbps"));
     upLinkText->setText(genStr);
     upLink->show();

}
void DetailedView::progressbarSettings(float rssiValue, float cinrValue,float strTemp)
{
	(void) strTemp;

    #ifdef BROADCOM
    ui->RSSIProgressBar->setMaximum(120);
    ui->RSSIProgressBar->setMinimum(0);
    QString strRSSi(tr("%1dBm").arg(strTemp));
    ui->RSSIProgressBar->setValue(rssiValue);
    ui->rssiValueEdit->setText(strRSSi);
    ui->RSSIProgressBar->update();

    ui->CINRProgressBar->setMaximum(100);
    ui->CINRProgressBar->setMinimum(0);
    QString strCINR(tr("%1dB").arg(cinrValue));
    ui->CINRProgressBar->setValue(cinrValue);
    ui->cinrValueEdit->setText(strCINR);
    ui->CINRProgressBar->update();
    #endif

    //#ifdef ALTAIR
    #if (defined(ALTAIR)|| !defined(BROADCOM))
    ui->RSSIProgressBar->setMaximum(120);
    ui->RSSIProgressBar->setMinimum(0);
    //QString strRSSi(tr("-%1dBm").arg(120-rssiValue));
    QString strRSSi(tr("%1dBm").arg(rssiValue));
    rssiValue = 120 + rssiValue;
    if(rssiValue < 0)
        rssiValue = 0;
    ui->RSSIProgressBar->setValue(rssiValue);
    ui->rssiValueEdit->setText(strRSSi);
    ui->RSSIProgressBar->update();

    ui->CINRProgressBar->setMaximum(100);
    ui->CINRProgressBar->setMinimum(0);
    QString strCINR(tr("%1dB").arg(cinrValue));
    ui->CINRProgressBar->setValue(cinrValue);
    ui->cinrValueEdit->setText(strCINR);
    ui->CINRProgressBar->update();
    #endif
 }

void DetailedView::on_radioDebug_clicked()
{
    //QDebug() << "on_radioDebug_clicked()--Start--checked:" << checked;
    if(!ui->radioInfo->isChecked() && ui->radioError->isChecked())
    {
        ((Uconnect *)pUConnect)->SetConfigParams(CFG_LOGGING_LEVEL,mERROR);
    }
    //QDebug() << "on_radioDebug_clicked()--End";
}


void DetailedView::on_radioInfo_clicked()
{
    //QDebug() << "on_radioInfo_clicked()--Start--checked:" << checked;
    if(!ui->radioDebug->isChecked() && ui->radioError->isChecked())
    {
        ((Uconnect *)pUConnect)->SetConfigParams(CFG_LOGGING_LEVEL,mERROR);
    }
    //QDebug() << "on_radioInfo_clicked()--End";
}

void DetailedView::on_radioError_clicked()
{
    //QDebug() << "on_radioError_clicked()--Start--checked:" << checked;
    if(!ui->radioDebug->isChecked() && ui->radioInfo->isChecked())
    {
        ((Uconnect *)pUConnect)->SetConfigParams(CFG_LOGGING_LEVEL,mERROR);
    }
    //QDebug() << "on_radioError_clicked()--End";
}


void DetailedView::on_radioAuto_clicked()
{
    //QDebug() << "on_radioAuto_clicked()--Start--checked:" << checked;
    ui->radioManual->setChecked(false);
    ui->radioAuto->setChecked(true);
        ((Uconnect *)pUConnect)->SetConfigParams(CFG_CONFIG_MODE,0);
    ui->radioManual->setDisabled(true);
    ui->radioAuto->setDisabled(true);
    //QDebug() << "on_radioAuto_clicked()--End";
}


void DetailedView::on_radioManual_clicked()
{
    //QDebug() << "on_radioManual_clicked()--Start checked:" << checked;
    ui->radioManual->setChecked(false);
    ui->radioAuto->setChecked(true);
    ui->radioManual->setDisabled(true);
    ui->radioAuto->setDisabled(true);
    //QDebug() << "on_radioManual_clicked()--End";
}
/*void DetailedView::on_helpButton_clicked()
{

    QString PDFpath=QDir::currentPath()+"/Release_Notes_CM_LTE.chm";
    QDesktopServices::openUrl(QUrl::fromLocalFile(PDFpath));
}*/
void DetailedView::custidleEnableEventSetting()
{
    //item  = ui->searchResults->takeItem(1);
    //delete item;
    ui->searchButton->setDisabled(true);

    statdataEvent = tr("CM SERVER STATUS::IDLE");

    /*ui->serverState->clear();
    ui->serverState->setText(statdataEvent);*/


    //stNetValue->clear();
    str.clear();
    ui->apntableWidget->clear();
        ui->apntableWidget->clear();
    //item = ui->apnName->takeItem(1);
   // delete item;


    /*ui->serverState->clear();
    ui->serverState->setText(statdataEvent);*/
    DisplayApnTitle();
}

void DetailedView::custSearchEventSetting()
{
    ui->searchButton->setDisabled(true);

    statdataEvent = tr("CM SERVER STATUS::SEARCHING");
   /* ui->serverState->clear();
    ui->serverState->setText(statdataEvent);*/

}
void DetailedView::custConnectingEventSetting()
{
    ui->searchButton->setDisabled(true);

    statdataEvent = tr("CM SERVER STATUS::CONNECTING");
    /*ui->serverState->clear();
    ui->serverState->setText(statdataEvent);*/

}

void DetailedView::custConnectedEventSetting()
{
    QStringList labels;
    labels << tr("NETWORK") <<  tr("Duration");
    ui->apntableWidget->setHorizontalHeaderLabels(labels);
    ui->searchButton->setDisabled(true);


    statdataEvent = tr("CM SERVER STATUS::CONNECTED");

   // ui->modemState->clear();

   /* ui->serverState->clear();
    ui->serverState->setText(statdataEvent);*/
}

void DetailedView::custDisconnectedEventSetting()
{
    ui->apntableWidget->clear();
    QStringList labels;
    labels << tr("NETWORK") <<  tr("Duration");
    ui->apntableWidget->setHorizontalHeaderLabels(labels);
    ui->searchButton->setDisabled(true);

    statdataEvent = tr("CM SERVER STATUS::DISCONNECTING");
   /* ui->serverState->clear();
    ui->serverState->setText(statdataEvent);*/
    int nImageToShow = 0;
    if(isLteConnected)
    {
        stSig->setPixmap(QPixmap(tr(":/sig%1").arg(nImageToShow)));
        stSig->setAlignment(Qt::AlignRight);
    }
}

void DetailedView::custDisableEventSetting()
{
    ui->apntableWidget->clear();
    QStringList labels;
    labels << tr("NETWORK") <<  tr("Duration");
    ui->apntableWidget->setHorizontalHeaderLabels(labels);

    ui->searchButton->setDisabled(true);


    statdataEvent = tr("CM SERVER STATUS::DISABLED");
   /* ui->serverState->clear();
    ui->serverState->setText(statdataEvent);*/

    //stNetValue->clear();
    str.clear();
        //ui->apntableWidget->clear();
    //ui->apnName->clear();
    //item  = ui->apnName->takeItem(1);
    //delete item;
    if(isLteConnected)
    {
        stSig->setPixmap(QPixmap(tr(":/sig%1").arg(0)));
        stSig->setAlignment(Qt::AlignRight);
        stSig->show();
    }
    ui->RSSIProgressBar->setValue(0);
    ui->rssiValueEdit->setText(tr("dBm"));
    ui->RSSIProgressBar->update();

    ui->CINRProgressBar->setValue(0);
    ui->cinrValueEdit->setText(tr("dB"));
    ui->CINRProgressBar->update();
}

void DetailedView::custEvents(int event)
{
    switch(event)
    {
    case CUST_AUTO_EVENT:
        {   ui->radioAuto->setChecked(true);
            ui->radioManual->setChecked(false);
            ui->searchButton->setDisabled(true);
        }
        break;
     case CUST_MANUAL_EVENT:
        {
            ui->searchButton->setDisabled(true);
        }
        break;
    case CUST_DEBUG_EVENT:
        {
            ui->radioInfo->setChecked(false);
            ui->radioError->setChecked(false);
            ui->radioDebug->setChecked(true);
        }
        break;
    case CUST_INFO_EVENT:
        {
            ui->radioDebug->setChecked(false);
            ui->radioError->setChecked(false);
            ui->radioInfo->setChecked(true);
        }
        break;
    case CUST_ERROR_EVENT:
        {
            ui->radioDebug->setChecked(false);
            ui->radioInfo->setChecked(false);
            ui->radioError->setChecked(true);

        }
        break;
    case CUST_SER_LOG_ENABLED_EVENT:
        {
            ui->fileLogginOption->setChecked(false);
            serverLoggingEnabled = true;
        }
        break;
    case CUST_SER_LOG_DISABLED_EVENT:
        {
            ui->fileLogginOption->setChecked(true);
            serverLoggingEnabled = false;
        }
        break;
    case CUST_SYS_DEVICE_ERROR_EVENT:
        {
          // ui->deviceMsg->setText(tr("DEVICE STATUS::Device Error"));

           //stNetValue->clear();
           str.clear();

             ui->apntableWidget->clear();

          //item  = ui->apnName->takeItem(1);
         // delete item;
           if(isLteConnected)
           {
               stSig->setPixmap(QPixmap(tr(":/sig%1").arg(0)));
               stSig->setAlignment(Qt::AlignRight);
               stSig->show();
           }

           ui->RSSIProgressBar->setValue(0);
           ui->rssiValueEdit->setText(tr("dBm"));
           ui->RSSIProgressBar->update();

           ui->CINRProgressBar->setValue(0);
           ui->cinrValueEdit->setText(tr("dB"));
           ui->CINRProgressBar->update();

        }
        break;
    case CUST_SYS_DEVICE_NOT_OPEN_EVENT:
        {
           // ui->deviceMsg->setText(tr("DEVICE STATUS::Device Not Open"));

            //stNetValue->clear();
            str.clear();
                ui->apntableWidget->clear();
           // ui->apnName->clear();
            //item = ui->apnName->takeItem(1);
           // delete item;
                if(isLteConnected)
                {
                    stSig->setPixmap(QPixmap(tr(":/sig%1").arg(0)));
                    stSig->setAlignment(Qt::AlignRight);
                    stSig->show();
                }

            ui->RSSIProgressBar->setValue(0);
            ui->rssiValueEdit->setText(tr("dBm"));
            ui->RSSIProgressBar->update();

            ui->CINRProgressBar->setValue(0);
            ui->cinrValueEdit->setText(tr("dB"));
            ui->CINRProgressBar->update();
        }
        break;
    case CUST_SYS_DEVICE_OPEN_EVENT:
        {
           // ui->deviceMsg->setText(tr("DEVICE STATUS::Device Open"));

           // stNetValue->clear();
            str.clear();
                ui->apntableWidget->clear();
           // ui->apnName->clear();
            //item = ui->apnName->takeItem(1);
            //delete item;
                if(isLteConnected)
                {
                    stSig->setPixmap(QPixmap(tr(":/sig%1").arg(0)));
                    stSig->setAlignment(Qt::AlignRight);
                    stSig->show();
                }
            ui->RSSIProgressBar->setValue(0);
            ui->rssiValueEdit->setText(tr("dBm"));
            ui->RSSIProgressBar->update();

            ui->CINRProgressBar->setValue(0);
            ui->cinrValueEdit->setText(tr("dB"));
            ui->CINRProgressBar->update();

        }
        break;
    case CUST_SYS_DISCONNECTED_EVENT:
        {
           // ui->deviceMsg->setText(tr("DEVICE STATUS::Disconnected"));
            if(timerRun ==true)
            {
                timer->stop();
                timerRun = false;
            }
           // stNetValue->clear();
            str.clear();
                ui->apntableWidget->clear();
            //ui->apnName->clear();
            //item = ui->apnName->takeItem(1);
           // delete item;
                if(isLteConnected)
                {
                    stSig->setPixmap(QPixmap(tr(":/sig%1").arg(0)));
                    stSig->setAlignment(Qt::AlignRight);
                    stSig->show();
                }

            ui->RSSIProgressBar->setValue(0);
            ui->rssiValueEdit->setText(tr("dBm"));
            ui->RSSIProgressBar->update();

            ui->CINRProgressBar->setValue(0);
            ui->cinrValueEdit->setText(tr("dB"));
            ui->CINRProgressBar->update();
            ((Uconnect *) pUConnect)->mCMInfo.ipAddress[0] = '\0';

        }
        break;
    case CUST_SYS_DEVICE_CONNECTED_EVENT:
        {
          // ui->deviceMsg->setText(tr("DEVICE STATUS::Device Connected"));

          // stNetValue->clear();
           str.clear();
               ui->apntableWidget->clear();
          // ui->apnName->clear();
           //item = ui->apnName->takeItem(1);
          // delete item;
               if(isLteConnected)
               {
                   stSig->setPixmap(QPixmap(tr(":/sig%1").arg(0)));
                   stSig->setAlignment(Qt::AlignRight);
                   stSig->show();
               }
           ui->RSSIProgressBar->setValue(0);
           ui->rssiValueEdit->setText(tr("dBm"));
           ui->RSSIProgressBar->update();

           ui->CINRProgressBar->setValue(0);
           ui->cinrValueEdit->setText(tr("dB"));
           ui->CINRProgressBar->update();

        }
        break;
    case CUST_SYS_DEVICE_DISCONNECTED_EVENT:
        {
            //ui->deviceMsg->setText(tr("DEVICE STATUS::Device Disconnected"));

           // stNetValue->clear();
            str.clear();
                ui->apntableWidget->clear();
            //ui->apnName->clear();
           //item = ui->apnName->takeItem(1);
            //delete item;
            if(isLteConnected)
            {
                stSig->setPixmap(QPixmap(tr(":/sig%1").arg(0)));
                stSig->setAlignment(Qt::AlignRight);
                stSig->show();
            }
            ui->RSSIProgressBar->setValue(0);
            ui->rssiValueEdit->setText(tr("dBm"));
            ui->RSSIProgressBar->update();

            ui->CINRProgressBar->setValue(0);
            ui->cinrValueEdit->setText(tr("dB"));
            ui->CINRProgressBar->update();
            ((Uconnect *) pUConnect)->mCMInfo.ipAddress[0] = '\0';

        }
        break;
    case CUST_CM_SERVER_CONN_LOST:
        {

           // stNetValue->clear();
             str.clear();
                 ui->apntableWidget->clear();

            //ui->apnName->clear();
             //item = ui->apnName->takeItem(1);
            // delete item;
                 if(isLteConnected)
                 {
                     stSig->setPixmap(QPixmap(tr(":/sig%1").arg(0)));
                     stSig->setAlignment(Qt::AlignRight);
                     stSig->show();
                 }
            ui->RSSIProgressBar->setValue(0);
            ui->rssiValueEdit->setText(tr("dBm"));
            ui->RSSIProgressBar->update();
            ui->CINRProgressBar->setValue(0);
            ui->cinrValueEdit->setText(tr("dB"));
            ui->CINRProgressBar->update();
            if(outFile.is_open())
            {
                outFile.close();
                serverLoggingEnabled = true;
            }
            ((Uconnect *) pUConnect)->mCMInfo.ipAddress[0] = '\0';
        }
        break;
    default:
        break;
    }
}
int DetailedView::getRefreshTimer()
{
    //QDebug() << "getRefreshTimer()--Start";
    bool ok;
    QString time = ui->m_editRefreshInterval->currentText();//currentText();
    qCritical() << "time:" << time;
    int timerValue = time.toUInt(&ok, 10);
    qCritical() << "timerValue:" << timerValue;
    //QDebug() << "getRefreshTimer()--End" << timerValue*1000;
    timerValue = timerValue*1000;
    qCritical() << "timerValue*1000:" << timerValue;
    return timerValue;
}
void DetailedView::statisticUpdate()
{

    ((Uconnect*)pUConnect)->isEditRefreshChecked = true;
    //QDebug() << "on_checkBox_clicked()--Start";
    if (bStatWidgetInitCheck == false)
    {
        debugTool();
        bStatWidgetInitCheck = true;
    }
    else
    {
        BuildStatWidget();
        //DebugUpdateStat();

    }
    // If refresh is enabled, run timer
    //if(checked == true)
   // {
        startRefreshTimer(getRefreshTimer());

   /* }
    else
    {
        stopRefreshTimer();

    }*/
    //QDebug() << "on_checkBox_clicked()--End";
}
/*void DetailedView::on_Refresh_checkBox_clicked(bool checked)
{
    ((Uconnect*)pUConnect)->isEditRefreshChecked = true;
    qDebug() << "on_checkBox_clicked()--Start";
    if (bStatWidgetInitCheck == false)
    {
        debugTool();
        bStatWidgetInitCheck = true;
    }
    else
    {
        BuildStatWidget();
        //DebugUpdateStat();

    }
    // If refresh is enabled, run timer
    if(checked == true)
    {
        startRefreshTimer(getRefreshTimer());

    }
    else
    {
        stopRefreshTimer();

    }
    qDebug() << "on_checkBox_clicked()--End";
}
*/
/*void DetailedView::on_m_editRefreshInterval_editingFinished()
{
    qDebug() << "on_m_editRefreshInterval_editingFinished--Start";
    ui->Refresh_checkBox->setChecked(true);
    ui->GetModStats->setDisabled(true);
    //default timer takes 1second

    if(stopRefreshTimer()){
         bStatWidgetInitCheck = true;
    }


    if (bStatWidgetInitCheck == false)
    {
        debugTool();
        bStatWidgetInitCheck = true;
    }
    else
    {
        BuildStatWidget();
    }

    startRefreshTimer(getRefreshTimer());
      qDebug() << "on_m_editRefreshInterval_editingFinished--End";
}
*/

void DetailedView::on_m_editRefreshInterval_currentIndexChanged(int)
{
    //QDebug() << "on_m_editRefreshInterval_textChanged()--Start";
    //ui->Refresh_checkBox->setChecked(true);
   // ui->GetModStats->setDisabled(true);
    //default timer takes 1second
    if(timerstate == true) {
        m_tRefreshTimer->stop();
        timerstate = false;
        bStatWidgetInitCheck = true;
    }
	startRefreshTimer(getRefreshTimer());
    if (bStatWidgetInitCheck == false)
    {
        debugTool();
        bStatWidgetInitCheck = true;
    }
    else
    {
        BuildStatWidget();
    }

   // m_tRefreshTimer->start(getRefreshTimer());
    //QDebug() << "on_m_editRefreshInterval_textChanged()--End";
}

void DetailedView::on_GetModStats_clicked()
{
    //QDebug() << "on_GetModStats_clicked()--Start";

    if (bStatWidgetInitCheck == false)
    {
        debugTool();

        bStatWidgetInitCheck = true;
    }
    else
    {
        BuildStatWidget();

    }

    //QDebug() << "on_GetModStats_clicked()--End";
}

void  DetailedView::setLogFileName(){

         serverLogName = tr("serverLog.txt");
        ui->logFileName->setText(serverLogName);
}


void DetailedView::on_searchButton_clicked()
{
    //QDebug() << "on_searchButton_clicked()--Start";
    ui->searchButton->setDisabled(true);
    ((Uconnect *)pUConnect)->searchbuttonHandler();
    //QDebug() << "on_searchButton_clicked()--End";

}

void DetailedView::on_fileLogginOption_clicked(bool checked)
{
    //QDebug() << "on_fileLogginOption_clicked()--Start";
    if(checked == true)
    {
        serverLoggingEnabled = true;
        QString str;
        str = ui->logFileName->text();
        serverLogName = str;
        ui->logFileName->setDisabled(true);
        outFile.open(serverLogName.toUtf8(),ios::app);
        if(!outFile.is_open())
        {
            qCritical() << "Please give proper name in the edit box";
            outFile.open("serverLog.txt",ios::app);
        }
        ((Uconnect *)pUConnect)->fileloggingHandle();

    }
    else
    {
        serverLoggingEnabled = false;
        serverLogName = tr("serverLog.txt");
        ui->logFileName->setDisabled(false);
        ui->logFileName->setText(tr("serverLog.txt"));
        if(outFile.is_open())
            outFile.close();
    }
    //QDebug() << "on_fileLogginOption_clicked()--End";
}




void DetailedView::searchcompletelistHandler(int ,QString )
{
   /* if(!((Uconnect *)pUConnect)->bFirstTimeSearchButtonClicked)
    {
        ((Uconnect *)pUConnect)->bFirstTimeSearchButtonClicked = true;

    }

    //ui->manualtableWidget->insertrtItem(col,netName);
    if(col==1)//for empty black line display on GUI
    {
        //ui->manualtableWidget->insertItem(col+1,"");
        ui->manualtableWidget->item(col+1)->setBackgroundColor(QColor(18,18,18));
    }
    if(col%2 == 0)
    {
        ui->manualtableWidget->item(col)->setBackgroundColor(QColor(18,18,18));
        ui->manualtableWidget->item(col)->setTextColor(QColor(255,255,255));
    }
    else
    {
        ui->manualtableWidget->item(col)->setBackgroundColor(QColor(60,60,60));
        ui->manualtableWidget->item(col)->setTextColor(QColor(255,255,255));
    }
    tNetworkList.clear();*/
}


void DetailedView::createActions()
{
    //QDebug() << "createActions()--Start";
    minimize = new QAction(tr("Mi&nimize"), this);
    connect(minimize, SIGNAL(triggered()), this, SLOT(hide()));
    open = new QAction(tr("&Restore"), this);
    connect(open, SIGNAL(triggered()), this, SLOT(showNormal()));
    close = new QAction(tr("&Quit"), this);
    connect(close, SIGNAL(triggered()), this, SLOT(appQuit()));
    //QDebug() << "createActions()--End";
}
void DetailedView::appQuit()
{
    trayIcon->setVisible(false);
    qApp->quit();
}

void DetailedView::createTrayIcon()
{
    //QDebug() << "createTrayIcon()--Start";
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimize);
    trayIconMenu->addAction(open);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(close);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
              this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    //QDebug() << "createTrayIcon()--End";
}
void DetailedView::setIcon()
{
    //QDebug() << "setIcon()--Start";
    trayIcon->setIcon(QIcon(":/icon"));
    //QDebug() << "setIcon()--End";
}

void DetailedView::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    //QDebug() << "trayIconClicked()--Start";
    if(reason == QSystemTrayIcon::Trigger)
    this->show();
    //QDebug() << "trayIconClicked()--End";
}
void DetailedView::closeEvent(QCloseEvent *event)
{
    //QDebug() << "closeEvent()--Start";
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
        }
    //QDebug() << "closeEvent()--End";
}

void DetailedView::setwifiposition()
{
   //ui->uConnectTabWidget->setTabPosition(6);
   //ui->uConnectTabWidget->setCurrentWidget();
    //ui->WIFI->activateWindow();


}



void DetailedView::formatTheAttribute(QString &statString,int level)
{
    if(level == 0)
    {
        statString.append("");
    }
    else if(level == 1)
    {
        statString.append("      ");
    }
    else
    {
        statString.append("            ");
    }
}



void DetailedView::getDecodedStrings(StatDisplayInfo& statDisplayInfo,string &rowStr)
{

    statDisplayInfo.encodedStats.doNotDisplay = 0; //assume rowStr in not empty

    if(rowStr.size() == 0 || strlen(rowStr.c_str()) == 0)
    {
        statDisplayInfo.encodedStats.doNotDisplay = 1; // flag rowStr as empty so that it won't be displayed

    }

    statDisplayInfo.statNameStr.clear();
    statDisplayInfo.valueStr.clear();

    std::vector<std::string> rowVector;

    tokeniseInformation(rowStr,rowVector,';');

    if(rowVector.size() >= 2)
    {
        statDisplayInfo.statNameStr = rowVector[1];

        if(strstr(statDisplayInfo.statNameStr.c_str(),"RxThroughput"))
        {
            if(rowVector.size()>=2)
            {
                string temp = rowVector[2];
                ((Uconnect*)pUConnect)->m_CMmodemStats.rxThroughput = atoi(temp.c_str());
            }

        }
        else if (strstr(statDisplayInfo.statNameStr.c_str(),"TxThroughput"))
        {
            if(rowVector.size()>=2)
            {
                string temp = rowVector[2];
                ((Uconnect*)pUConnect)->m_CMmodemStats.txThroughput = atoi(temp.c_str());
            }
        }



        decodeStatsInfo(rowVector[0],&statDisplayInfo.encodedStats);



        if(rowVector.size()>=3)
            statDisplayInfo.valueStr = rowVector[2];


    }



}

void DetailedView::decodeStatsInfo(string statsInfo , EncodedStats_s* encodedStats)
{
    //char* code = new char[statsInfo.size()];

    int code;
    stringToHex(code,statsInfo,hex);

    memcpy((void*)encodedStats,(void*)&code,sizeof(encodedStats));

}

void DetailedView::updateAltairStatsToCsvFile(StatDisplayInfo_s& statDisplayInfo,bool bHeaderUpdate,QString &csvHeader,QString &csvValue)
{
    if(statDisplayInfo.encodedStats.valuePresent == 0)
    {
       return;
    }
    if(strstr(statDisplayInfo.statNameStr.c_str(),"Connection Time") ||
            strstr(statDisplayInfo.statNameStr.c_str(),"APN") ||
            strstr(statDisplayInfo.statNameStr.c_str(),"IP_Address"))
    {
        if(statDisplayInfo.valueStr.size() < 2)
        {
            statDisplayInfo.valueStr = "N/A";
        }

    }

    if(bHeaderUpdate == false)
    {
        csvHeader.append(statDisplayInfo.statNameStr.c_str());
        csvHeader.append(",");
    }

    if(strstr(statDisplayInfo.statNameStr.c_str(),"MCS "))
    {
        replaceAll(statDisplayInfo.valueStr,",",";");
        csvValue.append(statDisplayInfo.valueStr.c_str());
        csvValue.append(",");
        //csvValue += ",";
    }
    else //if(strlen(valueStr.c_str()) >= 1 )
    {
        csvValue.append(statDisplayInfo.valueStr.c_str());
        csvValue.append(",");
        //csvValue += ",";
    }
}

void DetailedView::updateAltairStatsWidgetInGUI(StatDisplayInfo_s& statDisplayInfo,int row)
{


    string tmpStr = "";
    /*parent child format handling*/
    formatTheAttribute(statString,statDisplayInfo.encodedStats.level);

    tmpStr = statDisplayInfo.statNameStr;
    replaceAll(tmpStr,":","");
    statString.append(tmpStr.c_str());

    statValue.append(statDisplayInfo.valueStr.c_str());
    statValue.append("\n");


    item1 = new QTableWidgetItem(statString);
    ui->statTableWidget->setItem(row, 0, item1);
    item2 = new QTableWidgetItem(statValue);
    ui->statTableWidget->setItem(row, 1, item2);

#ifdef BROADCOM

    checkRangeSetBackground(statDisplayInfo.encodedStats,*item1,*item2);

#endif

    statString.clear();
    statValue.clear();
    //delete item1;
    //delete item2;
}

bool DetailedView::stringToHex(int& t,const string& s,ios_base& (*f)(ios_base&))
{
  istringstream iss(s);
  return !(iss >> f >> t).fail();
}

void DetailedView::updateAltairMeasStatsWidgetInGUI(StatDisplayInfo_s& statDisplayInfo,int row)
{

    std::string tmpStr = "";
    measStatString.clear();
    measValueString.clear();
    /*parent child format handling*/
    formatTheAttribute(measStatString,statDisplayInfo.encodedStats.level);

    tmpStr = statDisplayInfo.statNameStr;
    replaceAll(tmpStr,":","");
    measStatString.append(tmpStr.c_str());

    measValueString.append(statDisplayInfo.valueStr.c_str());
    measValueString.append("\n");

    if(strstr(statDisplayInfo.statNameStr.c_str(),"Reported"))
    {
        bool ok = 0;
        QString temp = statDisplayInfo.valueStr.c_str();

        if(1 == measType)
        {
            int rr=temp.toInt(&ok,10);
            ((Uconnect *)pUConnect)->m_CMmodemStats.m_nAltairStats.RSRP_Reported = rr;
        }
        else if(2 == measType)
        {
            int rr=temp.toInt(&ok,10);
            ((Uconnect *)pUConnect)->m_CMmodemStats.m_nAltairStats.RSRQ_Reported = rr;
        }
        else if(3 == measType)
        {
            int rr=temp.toInt(&ok,10);
            ((Uconnect *)pUConnect)->m_CMmodemStats.m_nAltairStats.SINR_Reported = rr;
        }
    }
    else if(strstr(statDisplayInfo.statNameStr.c_str(),"RSRP"))
    {
        measType = 1;
    }
    else if(strstr(statDisplayInfo.statNameStr.c_str(),"RSRQ"))
    {
        measType = 2;
    }
    else if(strstr(statDisplayInfo.statNameStr.c_str(),"SINR"))
    {
        measType = 3;
    }

    item3 = new QTableWidgetItem(measStatString);
    ui->measTableWidget->setItem(row, 0, item3);
    item4 = new QTableWidgetItem(measValueString);
    ui->measTableWidget->setItem(row, 1, item4);

    measStatString.clear();
    measValueString.clear();
}


void DetailedView::processAltairStats(string &statVal)
{

    std::string rowStr = "";
    QStringList labels;
    labels << tr("Parameter") << tr("Value");
    ui->statTableWidget->setHorizontalHeaderLabels(labels);
    vStr.clear();

    tokeniseInformation(statVal,vStr,'\n');
    int size = vStr.size();
    if (size <= 3)
    {
        vStr.clear();
        statString.clear();
        statValue.clear();
        return;
    }

    for(unsigned int i=0; i < vStr.size();i++)
    {
        statString.clear();
        statValue.clear();

        rowStr = vStr[i];
        //if(strstr(rowStr.c_str(),""))
        if(!rowStr.compare(""))
        {

            statString.clear();
            statValue.clear();
            continue;
        }



        StatDisplayInfo_s statDisplayInfo;
        getDecodedStrings(statDisplayInfo,rowStr);

        if(statDisplayInfo.encodedStats.doNotDisplay != 1)
        {
          //updateAltairStatsToCsvFile(statDisplayInfo,csvflag,csvHeader,csvValue);
          updateAltairStatsWidgetInGUI(statDisplayInfo,i);
        }

    }

    vStr.clear();



}

void DetailedView::processAltairMeasStats(string &measStatVal)
{
    QString csvHeader;
    QString csvValue;
    measType = 0;

    std::string statNameStr = "";
    std::string valueStr = "";
    std::string levelStr = "";
    std::string isValuePreStr = "";

    measStatString = "";
    measValueString = "";

    std::string rowStr = "";
    QStringList labels1;
    labels1 << tr("Parameter") << tr("Value");
    ui->statTableWidget->setHorizontalHeaderLabels(labels1);
    vStr.clear();

    tokeniseInformation(measStatVal,vStr,'\n');
    int size = vStr.size();
    if (size <= 3)
    {
        vStr.clear();
        statString.clear();
        statValue.clear();
       return;
    }
    for(unsigned int i=0; i < vStr.size();i++)
    {
        measStatString.clear();
        measValueString.clear();

        rowStr = vStr[i];
        //if(strstr(rowStr.c_str(),""))
        if(!rowStr.compare(""))
        {
            statString.clear();
            statValue.clear();
            //continue;
        }

        StatDisplayInfo_s statDisplayInfo;
        getDecodedStrings(statDisplayInfo,rowStr);

        if(statDisplayInfo.encodedStats.doNotDisplay != 1)
        {
            updateAltairMeasStatsWidgetInGUI(statDisplayInfo,i);
        }
    }
    csvValue += "\n";

    /*if(vStr.size()>1)
    {
        if(csvflag == false  )
        {
            csvValue += "\n";
            write_csv(csvHeader);
            csvflag= true;
        }
         write_csv(csvValue);
    }*/

    vStr.clear();
    statString.clear();
    statValue.clear();
}


void DetailedView::processBroadcomStats(string &statVal)
{
    QString csvHeader;
    QString csvValue;

    std::string statNameStr = "";
    std::string valueStr = "";
    std::string levelStr = "";
    std::string isValuePreStr = "";

    std::string rowStr = "";
    QStringList labels;
    labels << tr("Parameter") << tr("Value");
    ui->statTableWidget->setHorizontalHeaderLabels(labels);
    vStr.clear();

    tokeniseInformation(statVal,vStr,'\n');
    int size = vStr.size();
    if (size <= 3)
    {
        vStr.clear();
        statString.clear();
        statValue.clear();
       return;
    }

    for(unsigned int i=0; i < vStr.size();i++)
    {
        statString.clear();
        statValue.clear();


        rowStr = vStr[i];
        //if(strstr(rowStr.c_str(),""))
        if(rowStr.compare("") == 0 || strstr(rowStr.c_str(),"OK") || strlen(rowStr.c_str()) < 3)
        {
            statString.clear();
            statValue.clear();
            continue;

        }

            StatDisplayInfo_s statDisplayInfo;
            getDecodedStrings(statDisplayInfo,rowStr);

            if(statDisplayInfo.encodedStats.doNotDisplay != 1)
            {
                updateAltairStatsWidgetInGUI(statDisplayInfo,i);
            }
    }
    csvValue += "\n";

    vStr.clear();
    statString.clear();
    statValue.clear();
}

void DetailedView::checkRangeSetBackground(EncodedStats_s encodedStats, QTableWidgetItem& item1, QTableWidgetItem& item2)
{

    if(encodedStats.rangeFlag != 0 && encodedStats.valuePresent != 0)
    {
        item1.setBackgroundColor(QColor(255,0,0));
        item2.setBackgroundColor(QColor(255,0,0));
    }

}

void DetailedView::encodeGenParams(std::string &encodeStr)
{
    if(gTechDetails[LTE_TECH].state != NM_CONNECTED_STATE)
    {
        return;
    }
    encodeStr = "18;";
    encodeStr += "APN;";
    encodeStr += gen.Apn;
    encodeStr += "\n18;";
    encodeStr += "Connection Time;";
    encodeStr += gen.Duration;
    encodeStr += '\n';
    gen.Duration.clear();

    encodeStr += "18;IP_Address;";
    encodeStr += gen.ipAddress;
    encodeStr += '\n';


}

void DetailedView::writeMeasStatsToCsvFile(string& statVal,QString& csvHeader,QString& csvValue)
{


    std::string rowStr = "";

    tokeniseInformation(statVal,vStr,'\n');
    int size = vStr.size();
    if (size <= 3)
    {
        vStr.clear();
        statString.clear();
        statValue.clear();
        return;
    }

    for(unsigned int i=0; i < vStr.size();i++)
    {
        statString.clear();
        statValue.clear();

        rowStr = vStr[i];
        //if(strstr(rowStr.c_str(),""))
        if(!rowStr.compare(""))
        {

            statString.clear();
            statValue.clear();
            continue;
        }



        StatDisplayInfo_s statDisplayInfo;
        getDecodedStrings(statDisplayInfo,rowStr);

        if(statDisplayInfo.encodedStats.doNotDisplay != 1)
        {
          updateAltairStatsToCsvFile(statDisplayInfo,csvflag,csvHeader,csvValue);

        }

    }
}




void DetailedView::on_wifiScan_clicked(bool checked)
{
 (void)checked;
    ((Uconnect *)pUConnect)->WifiScanHandler();
   // ui->wifiMsg->setText("WIFI STATUS:Searching for wifi networks");

}


 void DetailedView::wifinetworkDisplay(QString networkName,QString signalQuality,QString connected,int i)
{
    // ui->wifinetworkList->clear();
    //ui->networkList->(0,networkName);
    QTableWidgetItem *item1 = new QTableWidgetItem(networkName);
    ui->wifinetworkList->setItem(i, 0, item1);
    item1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    QTableWidgetItem *item2 = new QTableWidgetItem(signalQuality);
    ui->wifinetworkList->setItem(i, 1, item2);
     item2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    QTableWidgetItem *item3 = new QTableWidgetItem(connected);
    ui->wifinetworkList->setItem(i, 2, item3);
     item3->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

     QListWidgetItem *item = new QListWidgetItem(networkName);
     ui->wifiPreferredList->addItem(item);




}

void DetailedView::on_wifiConnect_clicked()
{
    //QDebug() << "connect clicked event";

    if(isWifiConnected == false)
    {
        //QDebug() <<"inside is wifiConnected false";


       // ui->wifiConnect->setStyleSheet("background-image: url(:/connect_det_dis_wifi;");
        QString ssid = "";
        if(isNetworkSelected == false)
        {
            return;
        }
        QSize size = ui->wifinetworkList->size();

           ssid = ui->wifiPreferredList->currentItem()->text();



        if(ssid.length()>2)
        {
            //((Uconnect *)pUConnect)->WifiConnectHandler(ssid);

         }
        else
        {
            //ui->wifiMsg->setText("No Network is selected");
        }
       // ui->wifiMsg->setText("WIFI STATUS:Connecting");

    }else
    {
        // ui->wifiConnect->setStyleSheet("background-image: url(:/connect_det_dis_wifi);");
       // ((Uconnect *)pUConnect)->WifiDisConnectHandler();
        //ui->wifiMsg->setText("WIFI STATUS:Disconnecting");

    }

}

void DetailedView::wifiSetConnect(bool connectStatus)
{
    if(connectStatus)
    {
        // ui->wifiConnect->setStyleSheet("background-image: url(:/disconnect_det_wifi);");
        // ui->wifiMsg->setText("WIFI STATUS:Connected");
         isWifiConnected = true;

    }
    else
    {

        //QMessageBox::warning(this,tr("Warning"),tr("Wrong password"),QMessageBox::Ok);
        //ui->wifiMsg->setText("WIFI STATUS:Entered wrong Password");
        ui->PasswordEdit->clear();
        ui->networkSsid->clear();
        isWifiConnected = false;

    }
        //ui->wifiConnect->setDisabled(false);

}

void DetailedView::wifiSetDisconnect(bool disconnectStatus)
{
    if(disconnectStatus)
    {
        // ui->wifiConnect->setStyleSheet("background-image: url(:/connect_det_wifi);");
    }
    else
    {

    }
   // ui->PasswordEdit->setVisible(false);
   // ui->passwordLabel->setVisible(false);
    ui->networkSsid->clear();
}

void DetailedView::wifiPasswordReq(char* ssid)
{
    //if(ui->PasswordEdit)
   // ui->passwordLabel->setDisabled(false);
   // ui->PasswordEdit->setDisabled(false);
    //setText("WIFI STATUS:Enter the Password");
    string ss = string(ssid);
   ui->networkSsid->setText(ss.c_str());


  //  ui->passwordLabel->setVisible(true);
   // ui->PasswordEdit->setVisible(true);


 /*  QString passwordparse = ui->PasswordEdit->text();
   ((Uconnect *)pUConnect)->WifiPasswordGot(passwordparse.toStdString(),ssid);*/
}



void DetailedView::on_PasswordEdit_returnPressed()
{

    QString ssidname = ui->networkSsid->text();
    QString passwordparse = ui->PasswordEdit->text();
    ((Uconnect *)pUConnect)->WifiPasswordGot(passwordparse.toStdString(),ssidname.toStdString());
}

//void DetailedView::ifconnectedSetting(bool is_connected)
//{
    //remove this function
//}

/*void DetailedView::on_wifinetworkList_cellDoubleClicked(int row, int column)
{
    allClicksHandler();
}*/

/*void DetailedView::on_wifinetworkList_cellEntered(int row, int column)
{
    allClicksHandler();
}*/

void DetailedView::allClicksHandler()
{

   isNetworkSelected = true;

  //int rowCount = ui->wifinetworkList->rowCount();

    QString ssid = ui->wifiPreferredList->currentItem()->text();

    ui->networkSsid->setText(ssid);

	((Uconnect *)pUConnect)->setBearerParameter(WIFI_TECH,WIFI_SSID,ssid.toStdString());


}





void DetailedView::WificreateStatus()
{
    //QDebug() << "createStatus()--Start";
    statusBar = new QStatusBar(this);

    stWIFI = new QLabel("  WIFI ");
    statusBar->addWidget(stWIFI,0);

    QString ssidname = ui->networkSsid->text();
    stNetValue= new QLabel(ssidname);
    statusBar->addWidget(stNetValue,3);

   // setStatusBar(statusBar);
    setMinimumSize(600,576);
    //QDebug() << "createStatus()--End";
}

/*void DetailedView::on_wifinetworkList_cellClicked(int row, int column)
{
    allClicksHandler();
}*/

void DetailedView::minviewWifiPressed()
{
    /*ui->uConnectTabWidget->clear();
    ui->uConnectTabWidget->insertTab(5,ui->WIFI,QIcon(":/wifi-icon-selected"),tr("Wifi"));*/

    iswifiEnabled= true;
    //ui->wifiMsg->setVisible(true);
    //ui->wifiMsg->setText("WIFI STATUS: Wifi enabled");
    //ui->deviceMsg->setVisible(false);
   // WificreateStatus();
    //statusBar->clearFocus();

}

void DetailedView::minviewLtePressed()
{
    //lteClickedTrue= true;
    /*ui->uConnectTabWidget->clear();
    ui->uConnectTabWidget->insertTab(0,ui->network,QIcon(":/network-selected"),tr("Network"));
    ui->uConnectTabWidget->insertTab(1,ui->configure,QIcon(":/config-icon-selected"),tr("Configure"));
    ui->uConnectTabWidget->insertTab(2,ui->debugTool,QIcon(":/statistics-icon-selected"),tr("Statistic"));
    ui->uConnectTabWidget->insertTab(3,ui->tab,QIcon(":/info-icon-selected"),tr("General"));
    ui->uConnectTabWidget->insertTab(4,ui->tab_2,QIcon(":/imsi-icon-selected"),tr("IMS"));*/


    //createStatus();
    /*ui->serverState->setVisible(true);*/

}



void DetailedView::on_Lte_checkBox_clicked()
{
        lteChecked= !lteChecked;
        //handleTechCheckBoxClicked(LTE_TECH,lteChecked);

}

void DetailedView::on__3g_checkBox_clicked()
{

    _3GChecked = !_3GChecked;
    //handleTechCheckBoxClicked(THREE_G_TECH,_3GChecked);

}

void DetailedView::on_wifi_checkBox_clicked()
{

    wifiChecked= !wifiChecked;
   // handleTechCheckBoxClicked(WIFI_TECH,wifiChecked);

}

void DetailedView::on_ltecomboBox_currentIndexChanged(QString )
{
    //QString configModeText = ui->ltecomboBox->currentText();

    //handleTechConfigModeChanged(LTE_TECH,configModeText);

}

void DetailedView::on__3gcomboBox_currentIndexChanged(QString )
{
    //QString configModeText = ui->_3gcomboBox->currentText();
    //handleTechConfigModeChanged(THREE_G_TECH,configModeText);


}

void DetailedView::on_wificomboBox_currentIndexChanged(QString )
{
    //QString configModeText = ui->wificomboBox->currentText();
    //handleTechConfigModeChanged(THREE_G_TECH,configModeText);

}

void DetailedView::handleWifiConnectionStatusResponse(NM_Connect_Status connectStatus)
{
    switch(connectStatus)
    {
        case NM_CONNECT_SUCCESS:
            isWifiConnected = true;
            break;
        case NM_CONNECT_FAILURE:
            isWifiConnected = false;
            break;
        case NM_CONNECT_NO_NETWORK_FOUND:
            isWifiConnected = false;
            break;
        case NM_CONNECT_REQUEST_PASSWORD:
            //setNMParam(WIFI_TECH,WIFI_PASSWORD,"narayana"); //UCM check
           // detView.enableWifiPasswordEdit(); //check UCM

            break;
    }

}


void DetailedView::on__3GCheckBox_clicked()
{
    ((Uconnect*)pUConnect)->enableDisableTech(THREE_G_TECH,ui->_3GCheckBox->isChecked());
    updateTechPriorityList(THREE_G_TECH,ui->_3GCheckBox->isChecked());
    //((Uconnect *)pUConnect)->minView.setContextForConnectButton();
}
#if 0
void DetailedView::on_wifiCheckBox_clicked()
{
    updateTechPriorityList(WIFI_TECH,ui->wifiCheckBox->isChecked());

     ((Uconnect*)pUConnect)->enableDisableTech(WIFI_TECH,ui->wifiCheckBox->isChecked());
     ui->uConnectTabWidget->setStyleSheet("QTabBar::tab{height: 13px;width: 100px;}");
      //if(!ui->wifiCheckBox->isChecked())
     if(!ui->wifiCheckBox->)
      {
         ui->wifiPreferredList->clear();
      }
}
#endif
/*
void DetailedView::on_lteCheckBox_clicked()
{
    updateTechPriorityList(LTE_TECH,ui->lteCheckBox->isChecked());

    ((Uconnect*)pUConnect)->enableDisableTech(LTE_TECH,ui->lteCheckBox->isChecked());
}
*/

void DetailedView::on_tetheringCheckBox_clicked()
{
    //not handled for now.
    //((Uconnect*)pUConnect)->enableDisableTech(tet,ui->_3GCheckBox->isChecked());
}
void DetailedView::updateConnectionState(ETechSelect techId, bool connectStatus)
{
	(void)techId;
    if(connectStatus)
    {
        //getTabByTechId(techId).checkBox->setChecked(true);
    }

}


void DetailedView::on_generalPushButton_clicked()
{
	((Uconnect*)pUConnect)->setSettingsOnMinviewForGeneral();
    isGeneralCurrentTab = true;


    if(!getTabByTechId(ALL_TECH).selected)
    {
        makeButtonTransparentExpect(ALL_TECH);
        makeGenSettingsSelected();
        ui->uConnectTabWidget->clear();
        ui->uConnectTabWidget->insertTab(0,ui->generalSetting,tr("Settings"));
        ui->uConnectTabWidget->insertTab(1,ui->profiles,tr("Profiles"));


   }
   else
   {
        //ui->generalPushButton->setStyleSheet("background-color: transparent;");
        //settinggenState = false;
   }
}

void DetailedView::on__3gPushButton_clicked()
{
    
	((Uconnect*)pUConnect)->setSettingsOnMinview(false,THREE_G_TECH);
	threeGPushButtonPressed();
   
}

void DetailedView::threeGPushButtonPressed()
{
    if(!saveApplyButtonMessage())
    {
        return;
    }

    if(!getTabByTechId(THREE_G_TECH).selected)
   {
        makeButtonTransparentExpect(THREE_G_TECH);
        ui->_3gPushButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:0.988727, stop:0 rgba(254, 187, 25, 255), stop:1 rgba(235, 112, 18, 255));");
        getTabByTechId(THREE_G_TECH).selected = true;
        ui->uConnectTabWidget->clear();
        ui->uConnectTabWidget->insertTab(0,ui->threeGStatistics,tr("Statistics"));
         ui->uConnectTabWidget->insertTab(1,ui->ussd_tab,tr("USSD"));
          //ui->uConnectTabWidget->insertTab(2,ui->profiles,tr("Profiles"));
   }
   else
   {
        ui->uConnectTabWidget->clear();
        ui->uConnectTabWidget->insertTab(0,ui->generalSetting,tr("Settings"));
        //ui->uConnectTabWidget->insertTab(1,ui->profiles,tr("Profiles"));
        ui->_3gPushButton->setStyleSheet("background-color: transparent;");
        getTabByTechId(THREE_G_TECH).selected = false;
        makeGenSettingsSelected();
		((Uconnect*)pUConnect)->setSettingsOnMinviewForGeneral();
   }
}

void DetailedView::on_wifiPushButton_clicked()
{
    ((Uconnect*)pUConnect)->setSettingsOnMinview(false,WIFI_TECH);
	wifiPushButtonPressed();

}

void DetailedView::wifiPushButtonPressed()
{

    if(!saveApplyButtonMessage())
    {
        return;
    }

    if(!getTabByTechId(WIFI_TECH).selected)
   {
        makeButtonTransparentExpect(WIFI_TECH);
        ui->wifiPushButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:0.988727, stop:0 rgba(254, 187, 25, 255), stop:1 rgba(235, 112, 18, 255));");
        ui->uConnectTabWidget->clear();
        ui->uConnectTabWidget->insertTab(0,ui->wifiSetting,tr("Wifi Settings"));
        //ui->uConnectTabWidget->insertTab(1,ui->WIFI,tr("Wifi Statistics"));
        getTabByTechId(WIFI_TECH).selected = true;

    }
   else
   {
        ui->wifiPushButton->setStyleSheet("background-color: transparent;");

        ui->uConnectTabWidget->clear();
        ui->uConnectTabWidget->insertTab(0,ui->generalSetting,tr("Settings"));
        //ui->uConnectTabWidget->insertTab(1,ui->profiles,tr("Profiles"));
        getTabByTechId(WIFI_TECH).selected = false;
        makeGenSettingsSelected();
		((Uconnect*)pUConnect)->setSettingsOnMinviewForGeneral();
   }
}


void DetailedView::on_ltePushButton_clicked()
{
   ((Uconnect*)pUConnect)->setSettingsOnMinview(false,LTE_TECH);
    ltePushButtonPressed();
}

void DetailedView::ltePushButtonPressed()
{
    if(!saveApplyButtonMessage())
    {
        return;
    }

    if(!getTabByTechId(LTE_TECH).selected)
   {
        makeButtonTransparentExpect(LTE_TECH);
        ui->ltePushButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:0.988727, stop:0 rgba(254, 187, 25, 255), stop:1 rgba(235, 112, 18, 255));");

        ui->uConnectTabWidget->clear();

        ui->uConnectTabWidget->insertTab(1,ui->debugTool,tr("Statistics"));
        ui->uConnectTabWidget->insertTab(2,ui->tab,tr("Graph"));
        getTabByTechId(LTE_TECH).selected = true;


   }
   else
   {
        ui->ltePushButton->setStyleSheet("background-color: transparent;");
        getTabByTechId(LTE_TECH).selected = false;
        ui->uConnectTabWidget->clear();
        ui->uConnectTabWidget->insertTab(0,ui->generalSetting,tr("Settings"));
		((Uconnect*)pUConnect)->setSettingsOnMinviewForGeneral();
       // ui->uConnectTabWidget->insertTab(1,ui->profiles,tr("Profiles"));
   }
}

void DetailedView::on_tetheringPushButton_clicked()
{
    if(!getTabByTechId(VPN_TETHERING).selected)
   {
        makeButtonTransparentExpect(VPN_TETHERING);
        ui->tetheringPushButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:0.988727, stop:0 rgba(254, 187, 25, 255), stop:1 rgba(235, 112, 18, 255));");
        getTabByTechId(VPN_TETHERING).selected = true;

   }
   else
   {
        ui->tetheringPushButton->setStyleSheet("background-color: transparent;");
        getTabByTechId(VPN_TETHERING).selected = false;
        makeGenSettingsSelected();
   }
}

void DetailedView::on_vpnPushButton_clicked()
{
     if(!getTabByTechId(VPN_TECH).selected)
   {
        makeButtonTransparentExpect(VPN_TECH);
        ui->vpnPushButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:0.988727, stop:0 rgba(254, 187, 25, 255), stop:1 rgba(235, 112, 18, 255));");
        getTabByTechId(VPN_TECH).selected = true;
        ui->uConnectTabWidget->clear();
        ui->uConnectTabWidget->insertTab(0,ui->vpn,tr("Settings"));
   }
   else
   {
        ui->vpnPushButton->setStyleSheet("background-color: transparent;");
        getTabByTechId(VPN_TECH).selected = false;
        ui->uConnectTabWidget->clear();
        ui->uConnectTabWidget->insertTab(0,ui->generalSetting,tr("Settings"));
        //ui->uConnectTabWidget->insertTab(1,ui->profiles,tr("Profiles"));
        makeGenSettingsSelected();
   }
}

void DetailedView::on_techUpPushButton_clicked()
{
    ui->applySettingsButton->setDisabled(false);
    applyPressed = true;
    handleUpPreferenceClicked(ui->techPriorityListWidget);
    //handleTechPreferTimer();

}

void DetailedView::on_techDownPushButton_clicked()
{
    ui->applySettingsButton->setDisabled(false);
    applyPressed = true;
    handleDownPreferenceClicked(ui->techPriorityListWidget);
    //handleTechPreferTimer();
}

void DetailedView::on_connectState_clicked()
{

    if((!((Uconnect*)pUConnect)->autoMode))
    {
        //ui->connectState->setStyleSheet("image: url(:/onButton);background-color:Transparent);");
        //ui->connectState->setStyleSheet("background-image: url(:/onButton);");
        ui->connectState->setStyleSheet("image: url(:/onButton);border:none;outline:none;");
        ((Uconnect*)pUConnect)->autoMode = true;
        ((Uconnect*)pUConnect)->setConnectButtonForAutoMode(true);
        ((Uconnect*)pUConnect)->callSetContextOnMinConnectButton(true);

		//UBearerArbitrationON* evt = new UBearerArbitrationON();
		//((Uconnect*)pUConnect)->sendEvent(reinterpret_cast<UEvent*>(evt));
    }
    else
    {
       // ui->connectState->setStyleSheet("image: url(:/offButton);background-color:Transparent);");
       // ui->connectState->setStyleSheet("background-image: url(:/offButton);");
        ui->connectState->setStyleSheet("image: url(:/offButton);border:none;outline:none;");
        ((Uconnect*)pUConnect)->autoMode = false;
        ((Uconnect*)pUConnect)->setConnectButtonForAutoMode(false);
        ((Uconnect*)pUConnect)->callSetContextOnMinConnectButton(false);
		//UBearerArbitrationOFF* evt = new UBearerArbitrationOFF();
		//((Uconnect*)pUConnect)->sendEvent(reinterpret_cast<UEvent*>(evt));
    }

    ((Uconnect *)pUConnect)->toggleArbitrationMode(((Uconnect*)pUConnect)->autoMode);
}

void DetailedView::displayWifiPreferredNetworks(string ssids)
{
    if(!ui->preferredNetworkCheck->isChecked())
	{
        return;	
	}

	ui->wifiPreferredList->clear();
    
	vector<string> networkSsids;
    tokeniseInformation(ssids,networkSsids,'\n');

    for(unsigned int i=0;i<networkSsids.size();i++)
    {
        string ssidStr = networkSsids.at(i);

        QString qSsid = QString::fromStdString(ssidStr);
        QListWidgetItem* item = new QListWidgetItem(qSsid);
        ui->wifiPreferredList->addItem(item);
    }
}

void DetailedView::handleTechPreferTimer()
{
    if(techPreferTimer == NULL)
    {
        //techPreferTimer = new QTimer(this);
        //techPreferTimer->start(MAX_TIME_TO_SET_PREFERENCE);

        techPreferTimer =  new QTimer(this);
        if(techPreferTimer == 0)
        {
            qCritical() << "Error::Memory Allocation Failure";
        }
        connect(techPreferTimer, SIGNAL(timeout()), this, SLOT(updateTechPreference()));
        techPreferTimer->start(1000);

    }
    else
    {
        //reset timer
        techPreferTimer->stop();
        techPreferTimer->start(MAX_TIME_TO_SET_PREFERENCE);
    }

}

void DetailedView::updateTechPreference()
{
    techPreferTimer->stop();
    delete techPreferTimer;
    techPreferTimer = NULL;




}

void DetailedView::setTechPriority()
{
    U_UINT32 priorityArray[MAX_NUMBER_OF_NETWORKMANAGERS];
     bool taken[MAX_NUMBER_OF_NETWORKMANAGERS];
    for(int k=0;k<MAX_NUMBER_OF_NETWORKMANAGERS;k++)
    {
        priorityArray[k] = INVALID_TECH;
         taken[k] = false;
    }
    U_UINT32 priorityCount = 0;

    U_UINT32 techIdArray[3] = {LTE_TECH,THREE_G_TECH,WIFI_TECH};

    int count = ui->techPriorityListWidget->count();


    for(int i=0;i<count;i++)
    {
        QListWidgetItem* item = ui->techPriorityListWidget->item(i);
        string techName = item->text().toStdString();
        int j=0;
        while(gTechDetails[j].techId != INVALID_TECH)
        {
            if(!strcmp(techName.c_str(),gTechDetails[j].name.c_str()))
            {
                priorityArray[priorityCount] = gTechDetails[j].techId;
                priorityCount++;
                taken[gTechDetails[j].techId] = true;

            }
            j++;
        }

    }
    for(int i=0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
    {
        if(!taken[i])
        {
            priorityArray[priorityCount++] = techIdArray[i];
        }
    }

    for(int i=0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
    {
        gPriorityList[i] = priorityArray[i];
    }


    UBearerPriorityChanged* evt = new UBearerPriorityChanged(ALL_TECH,priorityArray);
    ((Uconnect*)pUConnect)->sendEvent(reinterpret_cast<UEvent*>(evt));


}


void DetailedView::handleUpPreferenceClicked(QListWidget* listWidget)
{
    if(listWidget->count() <= 1)
        return;


    int currentRow = listWidget->currentRow();
    if (currentRow == 0)
        return;

    QListWidgetItem *currentItem = listWidget->takeItem(currentRow);
    listWidget->insertItem(currentRow - 1, currentItem);
    listWidget->setCurrentRow(currentRow - 1);

}

void DetailedView::handleDownPreferenceClicked(QListWidget* listWidget)
{
    if(listWidget->count() <= 1)
        return;

    int currentRow = listWidget->currentRow();
    if (currentRow > listWidget->count()-1)
        return;
    QListWidgetItem * currentItem = listWidget->takeItem(currentRow);
    listWidget->insertItem(currentRow + 1, currentItem);
    listWidget->setCurrentRow(currentRow + 1);

}

void DetailedView::on_wifiUpPushButton_clicked()
{
    handleUpPreferenceClicked(ui->wifiPreferredList);
    sendWifiPreferenceList();
}

void DetailedView::on_wifiDownPushButton_clicked()
{
    handleDownPreferenceClicked(ui->wifiPreferredList);
    sendWifiPreferenceList();
}

void DetailedView::sendWifiPreferenceList()
{
    QString encodeStr = "";
    U_UINT32 count = ui->wifiPreferredList->count();

    for(unsigned int i=0;i<count;i++)
    {
        encodeStr += ui->wifiPreferredList->item(i)->text();
        encodeStr += "\n";
    }
    string encodeStdStr = encodeStr.toStdString();

    UBearerStrParam* strParam = new UBearerStrParam(WIFI_TECH,WIFI_PREFERRED_LIST,
                                     encodeStdStr);


    ((Uconnect*)pUConnect)->sendEvent(reinterpret_cast<UEvent*>(strParam));


}

void DetailedView::updateTechPriorityList(ETechSelect techId,bool enabled)
{

    gTechDetails[techId].enabled = enabled;
    string techName = gTechDetails[techId].name;

    if(enabled)
    {
        addTechsToPriorityList();
    }
    else
    {
        for(int i=0;i<ui->techPriorityListWidget->count();i++)
        {
            QListWidgetItem* widgetItem = ui->techPriorityListWidget->item(i);
            string techStr = widgetItem->text().toStdString();
            if(!strcmp(techStr.c_str(),techName.c_str()))
            {
                delete widgetItem;
                break;
            }

        }
        updateVisibilityOfPriorityWidgets();




    }




}

void DetailedView::makeButtonTransparentExpect(ETechSelect tabId)
{
    for(unsigned int i=0;i<tabButtons.size();i++)
    {
        if(this->tabButtons[i].tabId != tabId)
        {
            tabButtons[i].button->setStyleSheet("background-color: transparent;");
            tabButtons[i].selected = false;
        }
    }

}

void DetailedView::makeGenSettingsSelected()
{
    ui->generalPushButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:0.988727, stop:0 rgba(254, 187, 25, 255), stop:1 rgba(235, 112, 18, 255));");
    getTabByTechId(ALL_TECH).selected = true;

}

DetailedViewTabButton& DetailedView::getTabByTechId(ETechSelect techId)
{
    initializeTabCheckButtons();
    unsigned int i=0;
    for(i=0;i<this->tabButtons.size();i++)
    {
        if(this->tabButtons[i].tabId == techId)
        {
            break;
        }
    }
//	//QDebug() << "Should not be hit getTabByTechId";
    return this->tabButtons[i];
	

}

void DetailedView::makeTabChecked(ETechSelect techId)
{
    if(!getTabByTechId(techId).checkBox->isChecked())
    {
        getTabByTechId(techId).checkBox->setChecked(true);
        updateTechPriorityList(techId,true);
    }

}

void DetailedView::makeWifiSelected()
{
   ui->wifiCheckBox->setChecked(true);
   updateTechPriorityList(WIFI_TECH,true);
}

void DetailedView::reqPasswordPop(QString ssid)
{
    bool ok;
    QString text = QInputDialog::getText(this, ssid,
                                         tr("Password:"), QLineEdit::Password,
                                             "", &ok);
    if (ok && !text.isEmpty()){
        if(text.size()>64)
        {
           // QMessageBox::information(this,"Wrong Password","Password lenght exceeds 20 characters");
            // securityKey.setToolTip("Network Security key invalid \nPassword length exceeds 64 characters");
            QMessageBox::warning(this,"Wrong Password","Network Security key invalid \nPassword lenght exceeds 64 characters");

        }else
        {
         ((Uconnect *)pUConnect)->WifiPasswordGot(text.toStdString(),ssid.toStdString());
        }
    }

	else
	{
		((Uconnect *)pUConnect)->WifiPasswordCancelled();
	}
}


void DetailedView::on_wifiPreferredList_itemDoubleClicked(QListWidgetItem*)
{
    allClicksHandler();
}

void DetailedView::on_wifiNetworkCheck_clicked(bool )
{
    ui->wifiPreferredList->clear();
    QString networkName;
    if(ui->preferredNetworkCheck->isChecked())
    {
        ui->preferredNetworkCheck->setChecked(false);
    }
    displayAllWifiNetworks(this->wifiNetworkList);



}

void DetailedView::on_preferredNetworkCheck_clicked(bool checked)
{

    if(ui->wifiNetworkCheck->isChecked())
    {
        ui->wifiNetworkCheck->setChecked(false);
    }
    if(checked)
    {
        ui->wifiNetworkListLabel->setText(tr("  WiFi Preferred Networks "));
        displayWifiPreferredNetworks(this->preferredWifiSsids);
    }

}

void DetailedView::displayAllWifiNetworks(string wifiNetworkList)
{
    this->wifiNetworkList = wifiNetworkList;

    ui->wifiPreferredList->clear();
    QString networkName;
    if(ui->wifiNetworkCheck->isChecked())
    {
        ui->wifiNetworkListLabel->setText(tr("  WiFi Networks "));
        std::vector<string> vStr;
        tokeniseInformation(this->wifiNetworkList,vStr,'\n');
        for(unsigned int i=0;i<vStr.size();i++)
        {
            std::vector<string> networkVector;
            tokeniseInformation(vStr[i],networkVector,';');
            if(networkVector.size() < 2)
            {
                break;
            }
            for(unsigned int j=0;j<networkVector.size();j++)
            {
                std::vector<string> param;

                tokeniseInformation(networkVector[j],param,':');
                if(param.size() < 2)
                {
                    continue;
                }
                string name = param[0];
                string value = param[1];
                if(strstr(name.c_str(),"SSID"))
                {
                    networkName = value.c_str();
                    QListWidgetItem* item = new QListWidgetItem(networkName);
                    ui->wifiPreferredList->addItem(item);
                }
            }


         }
    }



}

void DetailedView::on_wifiCheckBox_clicked(bool checked)
{

    updateTechPriorityList(WIFI_TECH,checked);

     ((Uconnect*)pUConnect)->enableDisableTech(WIFI_TECH,checked);
     ui->uConnectTabWidget->setStyleSheet("QTabBar::tab{height: 13px;width: 100px;}");
      //if(!ui->wifiCheckBox->isChecked())
     if(checked == false)
      {
         if(ui->wifiPreferredList->count())
         {
            ui->wifiPreferredList->clear();
         }
      }
	 if(checked && gTechDetails[WIFI_TECH].state != NM_DISABLED_STATE)
	 {
		((Uconnect*)pUConnect)->showWifiEnabledNotification();	
	 }
}

void DetailedView::on_lteCheckBox_clicked(bool checked)
{
    updateTechPriorityList(LTE_TECH,checked);

    ((Uconnect*)pUConnect)->enableDisableTech(LTE_TECH,checked);
}

void DetailedView::show3gStats(string stats)
{
    vector<string> vStr;
    tokeniseInformation(stats,vStr,'\n');
    for(unsigned int i=0;i<vStr.size();i++)
    {
        vector<string> rowVector;
        tokeniseInformation(vStr[i],rowVector,';');
        if(rowVector.size() < 2)
        {
            continue;
        }
        string param = rowVector[0];
        string value = rowVector[1];
        //append param and value to widget;

        if(strstr(param.c_str(),"RSSI"))
        {
            ui->cinrThreeGProgressBar->setMinimum(0);
			ui->cinrThreeGProgressBar->setMaximum(100);
            int sig = abs(atoi(value.c_str()));
            ui->cinrThreeGProgressBar->setValue(sig);
            QString rssiTemp = value.c_str();
            rssiTemp.append(tr(" db"));
            ui->threegCinrVal->setText(rssiTemp);


            ((Uconnect*)pUConnect)->calculateSignalStrength(THREE_G_TECH,sig);
        }
        else if(strstr(param.c_str(),"CINR"))
        {
            ui->rssiThreeGProgressBar->setMinimum(0);
            ui->rssiThreeGProgressBar->setMaximum(100);
            ui->rssiThreeGProgressBar->setValue(abs(atoi(value.c_str())));
            QString cinrTemp = value.c_str();
            cinrTemp.append(" dbm");
			ui->threegRssiVal->setText(cinrTemp);
        }
        else if(strstr(param.c_str(),"DL_Rate"))
        {
            float downlink_val = floor((atof(value.c_str())*100.0)+0.5)/100.0;
            downStr = QString::number( downlink_val );
            QTableWidgetItem *dllab= new QTableWidgetItem("Download Rate");
            ui->threeGStatTableWidget->setItem(0, 0, dllab);
            downStr.append("kbps");
            QTableWidgetItem *dlval = new QTableWidgetItem(downStr);
            ui->threeGStatTableWidget->setItem(0, 1, dlval);

        }
        else if(strstr(param.c_str(),"UL_Rate"))
        {
            downStr.clear();
            float uplink_val = floor((atof(value.c_str())*100.0)+0.5)/100.0;
            downStr = QString::number( uplink_val );
            QTableWidgetItem *ullab= new QTableWidgetItem("Upload Rate");
            ui->threeGStatTableWidget->setItem(1, 0, ullab);
            downStr.append("kbps");
            QTableWidgetItem *ulval = new QTableWidgetItem(downStr);
            ui->threeGStatTableWidget->setItem(1, 1, ulval);

        }
        else if(strstr(param.c_str(),"Duration"))
        {
            QTableWidgetItem *durlab= new QTableWidgetItem("Duration");
            ui->threeGStatTableWidget->setItem(2, 0, durlab);

            QTableWidgetItem *durval = new QTableWidgetItem(value.c_str());
            ui->threeGStatTableWidget->setItem(2, 1, durval);
        }




    }


}

void DetailedView::minviewThreeGPressed()
{
    /*ui->uConnectTabWidget->clear();
    ui->uConnectTabWidget->insertTab(5,ui->WIFI,QIcon(":/wifi-icon-selected"),tr("Wifi"));*/

    //iswifiEnabled= true;
    //ui->wifiMsg->setVisible(true);
    //ui->wifiMsg->setText("WIFI STATUS: Wifi enabled");
    //ui->deviceMsg->setVisible(false);
    //ThreeGcreateStatus();
    //statusBar->clearFocus();

}

void DetailedView::ThreeGcreateStatus()
{
    //QDebug() << "createStatus()--Start";
    statusBar = new QStatusBar(this);

    stThreeG = new QLabel("  3G ");
    statusBar->addWidget(stThreeG,0);

    /*QString ssidname = ui->networkSsid->text();
    stNetValue= new QLabel(ssidname);
    statusBar->addWidget(stNetValue,3);*/

    //setStatusBar(statusBar);
    setMinimumSize(600,576);
    //QDebug() << "createStatus()--End";
}


void DetailedView::on_applySettingsButton_clicked()
{
    //only place where setTechPriority should be called according to current design
    setTechPriority();
    applyPressed = false;
    ui->applySettingsButton->setDisabled(true);
}

bool DetailedView::saveApplyButtonMessage()
{
    if(!applyPressed)
    {
        return true;
    }

        QMessageBox msgBox;
         msgBox.setText("Do you want to save the changes?");
        // msgBox.setInformativeText("Do you want to save?");
         msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
         msgBox.setDefaultButton(QMessageBox::Save);
         int ret = msgBox.exec();
         applyPressed = false;
         switch(ret)
         {
            case QMessageBox::Save:
             {
                 setTechPriority();
                 addTechsToPriorityList();
                 isGeneralCurrentTab = false;
                 ui->applySettingsButton->setDisabled(true);
                 return true;
             }
             break;
            case QMessageBox::Discard:
             {
                addTechsToPriorityList();
                isGeneralCurrentTab = false;
                ui->applySettingsButton->setDisabled(false);
                return true;
             }
            case QMessageBox::Cancel:
             {
                applyPressed = true;
                ui->applySettingsButton->setDisabled(false);
                return false;
            }

                break;
            default:
               break;
          }
		 return true;

}

void DetailedView::addTechsToPriorityList()
{
    ui->techPriorityListWidget->clear();

   for(int j=0;j<MAX_NUMBER_OF_NETWORKMANAGERS;j++)
   {
       ETechSelect techId = (ETechSelect)gPriorityList[j];
       if(techId > INVALID_TECH)
       {
           //QDebug() << "Invalid id for techId received ";
           return;
       }
       if(gTechDetails[techId].enabled)
       {
           getTabByTechId(techId).checkBox->setChecked(true);
           QString techName = QString::fromStdString(gTechDetails[techId].name);
           QListWidgetItem* item = new QListWidgetItem(techName);
           ui->techPriorityListWidget->addItem(item);

       }

    }
   updateVisibilityOfPriorityWidgets();



}

void DetailedView::initializeTabCheckButtons()
{
    for(unsigned int i=0;i<this->tabButtons.size();i++)
    {
        switch(tabButtons[i].tabId)
        {
            //case ALL_TECH:
              //  tabButtons[i].checkBox = ui->generalCheckBox;
              //  break;
            case LTE_TECH:
                    tabButtons[i].checkBox = ui->lteCheckBox;
                    break;
            case WIFI_TECH:
                    tabButtons[i].checkBox = ui->wifiCheckBox;
                    break;
            case THREE_G_TECH:
                     tabButtons[i].checkBox = ui->_3GCheckBox;
                     break;
            case VPN_TETHERING:
                     tabButtons[i].checkBox = ui->tetheringCheckBox;
                     break;

        }
    }
}


void DetailedView::updateDetNMSettings(ETechSelect techId,bool state)
{

    switch(techId)
    {
    case LTE_TECH:
        {
            if(state)
            {
                trayIcon->showMessage("LTE Device Message", "Device is Plugged-In", QSystemTrayIcon::Critical, 151000);
            }
            else
            {
                trayIcon->showMessage("LTE Device Message", "Device is Plugged-Out", QSystemTrayIcon::Critical, 151000);
            }
        }
        break;
    case WIFI_TECH:
        {
            if(state)
            {
                trayIcon->showMessage("WIFI Adapter Message", "Adapter is Available", QSystemTrayIcon::Critical, 151000);
            }
            else
            {
                trayIcon->showMessage("WIFI Adapter Message", "Device is not Avaliable", QSystemTrayIcon::Critical, 151000);
            }
        }
        break;
    case THREE_G_TECH:
        {
            if(state)
            {
                trayIcon->showMessage("3G Device Message", "Device is Plugged-In", QSystemTrayIcon::Critical, 151000);
            }
            else
            {
                trayIcon->showMessage("3G Device Message", "Device is Plugged-Out", QSystemTrayIcon::Critical, 151000);
            }
        }
        break;
    default:
        break;


    }

}



void DetailedView::updateVisibilityOfPriorityWidgets()
{
    if(ui->techPriorityListWidget->count() < 2)
    {
        ui->techUpPushButton->setVisible(false);
        ui->techDownPushButton->setVisible(false);
        ui->applySettingsButton->setVisible(false);
    }
    else
    {
        ui->techUpPushButton->setVisible(true);
        ui->techDownPushButton->setVisible(true);
        ui->applySettingsButton->setVisible(true);
    }
}

void DetailedView::updateOnNMStateChange(ETechSelect techId, ENMStateForBM state)
{

    if(state != NM_DISABLED_STATE)
    {
        getTabByTechId(techId).checkBox->setChecked(true);
        gTechDetails[techId].enabled = true;
        addTechsToPriorityList(); //based on gTechDetails

    }
    else
    {
        ui->ussd_request->clear();
        ui->ussd_response->clear();
        ui->rasApn->clear();
        ui->rasPassword->clear();
        ui->rasPhoneNum->clear();
        ui->rasProfileName->clear();
        ui->rasUserName->clear();
    }
    switch(state)
    {
    case NM_DISABLED_STATE:
        {
            ui->generalSetting->setDisabled(false);
           // ui->vpn->setDisabled(true);
            if(techId == LTE_TECH)
            {
                //ui->configure->setDisabled(true);
                ui->debugTool->setDisabled(true);
                ui->tab->setDisabled(true);
                ui->RSSIProgressBar->setValue(0);
                ui->CINRProgressBar->setValue(0);
                ui->statTableWidget->clear();
                ui->measTableWidget->clear();
                ui->rssiValueEdit->clear();
                ui->cinrValueEdit->clear();
                trayIcon->showMessage("LTE Device Message", "Device not found", QSystemTrayIcon::Critical, 151000);
            }
            else if(techId == WIFI_TECH)
            {
                 ui->wifiSetting->setDisabled(true);
                 ui->wifiPreferredList->clear();
                 ui->wifiAdapterComboBox->clear();
                 ui->wifiPreferredList->clear();
                 ui->wifiNetworkCheck->setDisabled(true);
                 ui->preferredNetworkCheck->setDisabled(true);
                 ui->wifiAdapterComboBox->clear();
                 ui->wifiAdapterComboBox->addItem("Network Adapters");
                 ui->wifiAdapterComboBox->setDisabled(true);

                 trayIcon->showMessage("Wifi Adapter Message", "Adapter not found", QSystemTrayIcon::Critical, 151000);
            }
            else if(techId == THREE_G_TECH)
            {
                ui->threeGStatistics->setDisabled(true);
                ui->profiles->setDisabled(true);
                ui->ussd_tab->setDisabled(true);
                ui->rssiThreeGProgressBar->setValue(0);
                ui->cinrThreeGProgressBar->setValue(0);
                ui->threeGStatTableWidget->clear();
                ui->threegCinrVal->clear();
                ui->threegRssiVal->clear();
                displayRasProfile();
                trayIcon->showMessage("3G Device Message", "Device not found", QSystemTrayIcon::Critical, 151000);
            }

           // updateMessageBar();

        }
        break;
    case NM_NOT_READY_STATE:
        {
            ui->generalSetting->setDisabled(false);
            //ui->vpn->setDisabled(true);
            if(techId == LTE_TECH)
            {
                //ui->configure->setDisabled(false);
                ui->debugTool->setDisabled(false);
                ui->tab->setDisabled(false);
                trayIcon->showMessage("LTE Device Message", "Device found", QSystemTrayIcon::Critical, 151000);

            }
            else if(techId == WIFI_TECH)
            {
                 ui->wifiSetting->setDisabled(false);
                 ui->wifiNetworkCheck->setDisabled(false);
                 ui->preferredNetworkCheck->setDisabled(false);
                 ui->wifiAdapterComboBox->setDisabled(false);

                 trayIcon->showMessage("Wifi Adapter Message", "Adapter not found", QSystemTrayIcon::Critical, 151000);
            }
            else if(techId == THREE_G_TECH)
            {
                ui->threeGStatTableWidget->clear();
                ui->threeGStatistics->setDisabled(false);
                ui->profiles->setDisabled(false);
                ui->ussd_tab->setDisabled(false);
                displayRasProfile();
                trayIcon->showMessage("3G Device Message", "Device found", QSystemTrayIcon::Critical, 151000);
            }

           // updateMessageBar();
        }
        break;
    case NM_READY_STATE:
        {
            ui->generalSetting->setDisabled(false);
            //ui->vpn->setDisabled(true);
            if(techId == LTE_TECH)
            {
               // ui->configure->setDisabled(false);
                ui->debugTool->setDisabled(false);
                ui->tab->setDisabled(false);


            }
            else if(techId == WIFI_TECH)
            {
                 ui->wifiSetting->setDisabled(false);
                 ui->wifiNetworkCheck->setDisabled(false);
                 ui->preferredNetworkCheck->setDisabled(false);
                 ui->wifiAdapterComboBox->setDisabled(false);

            }
            else if(techId == THREE_G_TECH)
            {
                ui->threeGStatTableWidget->clear();
                ui->threeGStatistics->setDisabled(false);
                ui->profiles->setDisabled(false);
                ui->ussd_tab->setDisabled(false);
                trayIcon->showMessage("3G Device Message", "Device found", QSystemTrayIcon::Critical, 151000);
            }

           // updateMessageBar();
        }
        break;
    case NM_CONNECTED_STATE:
        {
            ui->generalSetting->setDisabled(false);
            ui->vpn->setDisabled(false);
            ((Uconnect*)pUConnect)->getVpnProfList();
            ((Uconnect*)pUConnect)->getVpnClientList();
            vpnAutoConnect();
            if(techId == LTE_TECH)
            {
                //ui->configure->setDisabled(false);
                ui->debugTool->setDisabled(false);
                ui->tab->setDisabled(false);
                //ui->messageBar->setText("Connected to 4G/LTE");
            }
            else if(techId == WIFI_TECH)
            {
                 ui->wifiSetting->setDisabled(false);
                 //ui->messageBar->setText("Connected to WIFI");
            }
            else if(techId == THREE_G_TECH)
            {
                ui->threeGStatistics->setDisabled(false);
                ui->profiles->setDisabled(false);
                ui->ussd_tab->setDisabled(false);
                displayRasProfile();
               // ui->messageBar->setText("Connected to 3G");
            }
        }
        break;
    default:
        break;
    }
    //handle showing of settings
}

void DetailedView::settingServerNotThere()
{

    ui->generalSetting->setDisabled(true);
    ui->ltePushButton->setDisabled(true);
    ui->wifiPushButton->setDisabled(true);
    ui->_3gPushButton->setDisabled(true);
    ui->vpnPushButton->setDisabled(true);
    ui->lteCheckBox->setDisabled(true);
    ui->wifiCheckBox->setDisabled(true);
    ui->_3GCheckBox->setDisabled(true);
	ui->CINRProgressBar->setValue(0);
	ui->CINRProgressBar->update();
	ui->cinrThreeGProgressBar->setValue(0);
	ui->cinrThreeGProgressBar->update();
	ui->cinrValueEdit->clear();
	ui->threegCinrVal->clear();
	ui->RSSIProgressBar->setValue(0);
	ui->RSSIProgressBar->update();
	ui->rssiThreeGProgressBar->setValue(0);
	ui->rssiThreeGProgressBar->update();
	ui->rssiValueEdit->clear();
	ui->threegRssiVal->clear();
	ui->measTableWidget->clear();
	ui->statTableWidget->clear();
    ui->threeGStatTableWidget->clear();
    ui->profiles->setDisabled(true);
    ui->ussd_tab->setDisabled(true);
    ui->ussd_request->clear();
    ui->ussd_response->clear();
    ui->rasApn->clear();
    ui->rasPassword->clear();
    ui->rasPhoneNum->clear();
    ui->rasProfileName->clear();
    ui->rasUserName->clear();
    ui->vpnClientListCombo->clear();
    ui->vpnClientListCombo->addItem("Client Profile");
    ui->vpnProfileListCombo->clear();
    ui->vpnProfileListCombo->addItem("Profile");
	ui->vpn->setDisabled(true);

}

void DetailedView::settingServerThere()
{
	ui->connectState->setStyleSheet("image: url(:/onButton);border:none;outline:none;");
    ui->generalSetting->setDisabled(false);
    ui->ltePushButton->setDisabled(false);
    ui->wifiPushButton->setDisabled(false);
    ui->_3gPushButton->setDisabled(false);
    ui->vpnPushButton->setDisabled(false);
    ui->lteCheckBox->setDisabled(false);
    ui->wifiCheckBox->setDisabled(false);
    ui->_3GCheckBox->setDisabled(false);
    ui->profiles->setDisabled(false);
    ui->ussd_tab->setDisabled(false);
}

void DetailedView::showWifiAdapterName(string wifiAdapter)
{
    ui->wifiAdapterComboBox->clear();
    ui->wifiAdapterComboBox->addItem(wifiAdapter.c_str());
}
/*
void DetailedView::updateMessageBar()
{
    bool isNotConnectedAnyTech = false;
    for(int i=0;i<3;++i)
    {
        if(gTechDetails[i].state == NM_CONNECTED_STATE)
        {
            isNotConnectedAnyTech = true;
            break;
        }
    }
    if(isNotConnectedAnyTech == false)
    {
           //ui->messageBar->setText("");
    }
}
*/
void DetailedView::handleWifiPreferredRsp(string encodedSsids)
{
    this->preferredWifiSsids = encodedSsids;
    displayWifiPreferredNetworks(encodedSsids);

}

void DetailedView::updateRssiProgressBar(string rssi)
{
    vector<string> rssiVect;
    tokeniseInformation(rssi,rssiVect,'=');

	if(rssiVect.size() > 1){
    string rssiStr = rssiVect.at(1);
    int rssiVal = atoi(rssiStr.c_str());
	((Uconnect*)pUConnect)->calculateSignalStrength(LTE_TECH,rssiVal);

    ui->RSSIProgressBar->setMaximum(120);
    ui->RSSIProgressBar->setMinimum(0);
    QString strRSSI(tr("%1dBm").arg(rssiVal));
    rssiVal = 120 + rssiVal;
    if(rssiVal < 0)
        rssiVal = 0;
    ui->RSSIProgressBar->setValue(rssiVal);
    ui->RSSIProgressBar->update();
    ui->rssiValueEdit->setText(strRSSI);
	}
}

void DetailedView::updateCinrProgressBar(string cinr)
{
    vector<string> cinrVect;
    tokeniseInformation(cinr,cinrVect,'=');

	if(cinrVect.size() > 1){
    string cinrStr = cinrVect.at(1);
    int cinrVal = atoi(cinrStr.c_str());
    ui->CINRProgressBar->setValue(cinrVal);
    ui->CINRProgressBar->update();
     QString strCINR(tr("%1dB").arg(cinrVal));
    ui->cinrValueEdit->setText(strCINR);
	}
}
void DetailedView::on_send_ussd_clicked()
{
    singleUssdResponse = true;
    string req = ui->ussd_request->text().toStdString();
    ui->ussd_request->setText("");
    ((Uconnect *)pUConnect)->sendUSSDReq(req);
    ui->send_ussd->setDisabled(true);

    QString ussdReq =" ";
    ussdReq.append(req.c_str());
    QListWidgetItem *item = new QListWidgetItem(ussdReq);
    ui->ussd_response->addItem(item);
}

void DetailedView::displayUssdRespose(string val)
{
    ui->send_ussd->setDisabled(false);
    QString ussdResp = "\n";
    ussdResp.append(QString::fromStdString(val));
    ussdResp.append("\n");
    if(singleUssdResponse == true)
    {
        QListWidgetItem *item = new QListWidgetItem(ussdResp);
        ui->ussd_response->addItem(item);
        singleUssdResponse = false;
    }

    ui->ussd_request->clear();
}


void DetailedView::handleWifiDisabledNotification()
{
    ui->wifiCheckBox->setChecked(false);

}

void DetailedView::setContextForAutoManualButton()
{
	if(((Uconnect*)pUConnect)->autoMode)
	{
		ui->connectState->setStyleSheet("image: url(:/onButton);border:none;outline:none;");
	}
	else
	{
		ui->connectState->setStyleSheet("image: url(:/offButton);border:none;outline:none;");
	}
}

void DetailedView::on_rasProfSave_clicked()
{
    Ras_Details_s details;
   //copyTextFromLabel(ui->rasProfileName,details.rasProfileName);

   // copyTextFromLabel(ui->rasPhoneNum,details.rasPhoneNumber);
   // copyTextFromLabel(ui->rasUserName,details.rasUsrName);
   // copyTextFromLabel(ui->rasPassword,details.rasPasswd);
   // copyTextFromLabel(ui->rasApn,details.apnName);

    string rasPro = ui->rasProfileName->text().toStdString();
    char* rasCharCpy = (char*)rasPro.c_str();
    char rasChar = *rasCharCpy;
    if(rasChar<97)
    {
        rasChar += 32;
    }
    if(rasPro.length()<20 && rasPro != "" && (rasChar>=97 && rasChar<=122))
    {
        strcpy(details.rasProfileName,rasPro.c_str());
    }
    else
    {

        QMessageBox::warning(this,tr("Warning"),tr("Profile name must start with an Alphabetic character"),QMessageBox::Ok);
        return;
    }
    rasPro.clear();
    rasPro = ui->rasPhoneNum->text().toStdString();
    if(rasPro.length()< 20)
    {
        strcpy(details.rasPhoneNumber,rasPro.c_str());
    }
    else
    {
        strcpy(details.rasPhoneNumber,"*99#");
        ui->rasPhoneNum->setText("*99#");
    }
    rasPro.clear();
    rasPro = ui->rasUserName->text().toStdString();
    strcpy(details.rasUsrName,rasPro.c_str());
    rasPro.clear();
    rasPro = ui->rasPassword->text().toStdString();
    strcpy(details.rasPasswd,rasPro.c_str());
    rasPro.clear();
    rasPro = ui->rasApn->text().toStdString();
    strcpy(details.apnName,rasPro.c_str());

    ((Uconnect*)pUConnect)->setRasProfile(&details);


}

void DetailedView::displayRasProfile()
{
    if(gTechDetails[THREE_G_TECH].state != DISABLED_STATE)
    {
        if(ui->rasProfileName->text().size() == 0)
        {
            ui->rasApn->setText(((Uconnect*)pUConnect)->rasProfDetails.apnName);
            ui->rasPassword->setText(((Uconnect*)pUConnect)->rasProfDetails.rasPasswd);
            ui->rasPhoneNum->setText(((Uconnect*)pUConnect)->rasProfDetails.rasPhoneNumber);
            ui->rasProfileName->setText(((Uconnect*)pUConnect)->rasProfDetails.rasProfileName);
            ui->rasUserName->setText(((Uconnect*)pUConnect)->rasProfDetails.rasUsrName);
        }
    }
}

/* This function displays vpn profiles when received from vpn through server*/
void DetailedView::showVpnProfileslist(string profLst)
{
    QString profileList;
	std::vector<std::string> proListVector;
	ui->vpnProfileListCombo->clear();

    tokeniseInformation(profLst,proListVector,'\n');

	for(U_UINT32 i=0; i<proListVector.size();i++)
	{
		profileList = proListVector[i].c_str();
		if(profileList != "")
		{
			ui->vpnProfileListCombo->addItem(profileList);
		}
	}
    if(ui->vpnAutoCheck->isChecked())
    {
        QString profToCont = ui->vpnProfileListCombo->currentText();
        if(profToCont != "")
        {
            ((Uconnect*)pUConnect)->vpnConnect(profToCont);
        }
    }
}

/* This function displays vpn clients when received from vpn through server*/
void DetailedView::showVpnClientlist(string cliLst)
{
	QString ClientList;
	std::vector<std::string>cliListVector;
    ui->vpnClientListCombo->clear();

    tokeniseInformation(cliLst,cliListVector,'\n');

	if(cliListVector[0].c_str() == "")
	{
        ui->vpnClientListCombo->addItem("No Client Present");
		return;
	}
    for(U_UINT32 i=0; i<cliListVector.size();i++)
	{
        ClientList = cliListVector[i].c_str();
		ui->vpnClientListCombo->addItem(ClientList);
	}
}

/* When the autoconnect check box is checked it performs a vpnconnect*/
void DetailedView::on_vpnAutoCheck_clicked()
{
    vpnAutoConnect();
}

/* This function calls the vpncoonect function with profile name*/
void DetailedView::vpnAutoConnect()
{

    if((ui->vpnAutoCheck->isChecked()))
    {
        ui->vpnButton->hide();
        QString profToCont = ui->vpnProfileListCombo->currentText();
        if(profToCont != "" && profToCont != "Profile")
        {
            ((Uconnect*)pUConnect)->vpnConnect(profToCont);
        }
        //ui->vpnButton->setDisabled(true);
    }
    else
    {
        ui->vpnButton->show();
        ((Uconnect*)pUConnect)->vpnDisconnect();
        //ui->vpnButton->setDisabled(true);
    }
}

/* This function shows Vpn connected settings*/
void DetailedView::showVpnConnectSettings(int connectRsp)
{
	((Uconnect*)pUConnect)->vpnConnectStatus = true;
    ((Uconnect*)pUConnect)->vpnMsgBarStatus = true;
    if(connectRsp == 0)
    {
        ui->vpnButton->setDisabled(false);
        ui->vpnButton->setText("VPN Stop");
        ((Uconnect*)pUConnect)->callMsgBarMinview();
    }
	
}

/* This function shows the disconnected settings*/
void DetailedView::showVpnDisconnectSettings()
{
	((Uconnect*)pUConnect)->vpnConnectStatus = false;
    if(ui->vpnAutoCheck->isChecked())
    {
        ui->vpnButton->setText("VPN Stop");
		((Uconnect*)pUConnect)->callMsgBarMinview();
		((Uconnect*)pUConnect)->vpnMsgBarStatus = false;
	}
    else
    {
        ui->vpnButton->setText("VPN Start");
		((Uconnect*)pUConnect)->callMsgBarMinview();
    }

    

}

void DetailedView::on_vpnButton_clicked()
{
    if(ui->vpnAutoCheck->isChecked() && ui->vpnButton->text() != "VPN Start")
    {
        ((Uconnect*)pUConnect)->vpnDisconnect();
   
        QString profToCont = ui->vpnProfileListCombo->currentText();
        if(profToCont != "")
        {
            ((Uconnect*)pUConnect)->vpnConnect(profToCont);
        }
	}
	else if(!ui->vpnAutoCheck->isChecked() && ui->vpnButton->text() == "VPN Start")
	{
		QString profToCont = ui->vpnProfileListCombo->currentText();
        if(profToCont != "")
        {
            ((Uconnect*)pUConnect)->vpnConnect(profToCont);
        }
	}
	else if(!ui->vpnAutoCheck->isChecked() && ui->vpnButton->text() == "VPN Stop")
	{
		((Uconnect*)pUConnect)->vpnDisconnect();	
	}
}

