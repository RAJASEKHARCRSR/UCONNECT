/*****************************************************************************
**
**  uconnect.cpp
**
**  This file contains the definitions of the enums,
      structures and functions used in Uconnect GUI
**
**  Authors: Suresh Lingadalli & Ramya.B
**  Date  : 15-MAR-2011
**  (c) United Mobile Apps Pvt Ltd - (2010 - 2015)

    VER       DATE     AUTHOR      DESCRIPTION
    0.1     15/03/10     RB         Creation
    0.2     29/03/10     SL         addition of statusbar, menu bar,
                                    background colours, system tray,
                                    server as lib.

*****************************************************************************/
#include "uconnect.h"
#include "ui_uconnect.h"
#include "GuiEvents.h"


#define GETVARNAME(x) #x

using namespace std;
time_t tStart_time;

extern Uconnect *gUConnect;

//gTechDetails in order of techId.
TechDetails gTechDetails[] =
{{LTE_TECH,"4G/LTE",false,false,INVALID_PRIORITY,NM_DISABLED_STATE,NM_DISABLED_STATE},
 {THREE_G_TECH,"3G/2G",false,false,INVALID_PRIORITY,NM_DISABLED_STATE,NM_DISABLED_STATE},
 {WIFI_TECH,"WiFi",false,false,INVALID_PRIORITY,NM_DISABLED_STATE,NM_DISABLED_STATE},
 //invalid always last. Like in cli array
 {INVALID_TECH,"INVALID",false,false,INVALID_PRIORITY,NM_DISABLED_STATE,NM_DISABLED_STATE}
};

U_UINT32 gPriorityList[MAX_NUMBER_OF_NETWORKMANAGERS];


U_UINT32            guiLogLevel;//1 for debug, 2 for critical
ofstream            logfile;
//server FSM custom states event

#ifdef WIN32
GUID GUID_DEVINTERFACE_USB_DEVICE = {0x4d36e978,0xe325,0x11ce,0xbf,0xc1,0x08,0x00,0x2b,0xe1,0x03,0x18};
#endif
/***********************************************************************
*   This function takes the message type and the character string to be printed
*   to the log file in four different types debug, critical ,warning and fatal.
*
*   Parameter
*   type - Its the message type .
*   msg  - Its the pointer  to the character string to be printed to log file.
***********************************************************************/
void GUIClientLogHandler(QtMsgType type, const char *msg)
{    switch (type)
        {
        case QtDebugMsg:
                if(1 == guiLogLevel)
                logfile << QTime::currentTime().toString().toAscii().data() << " Debug: " << msg << "\n";
                break;
        case QtCriticalMsg:
                if(2 == guiLogLevel)
                logfile << QTime::currentTime().toString().toAscii().data() << " Critical: " << msg << "\n";
                break;
        case QtWarningMsg:
                if(2 == guiLogLevel)
                logfile << QTime::currentTime().toString().toAscii().data() << " Warning: " << msg << "\n";
                break;
        case QtFatalMsg:
                if(2 == guiLogLevel)
                {
                logfile << QTime::currentTime().toString().toAscii().data() <<  " Fatal: " << msg << "\n";
                abort();
                }
                break;
        }
logfile.flush();
}
/////////////////////////////////////////////////////////////////////////////
//////////THE PORT AND IP ADDRESS IS USED FOR SOCKET COMMUNICATION///////////
/////////////////////////////////////////////////////////////////////////////
//#define DEFAULT_SERVER_PORT  55555
#define DEFAULT_SERVER_IP    "127.0.0.1"

U_VOID uconnGuiEvtHandler (UEvent* evt, U_SINT8 *data_p, U_VOID *arg_p);

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////// CONSTRUCTOR ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Uconnect::Uconnect()
{
    minView = new MinimumView();
    detView = new DetailedView(minView);
    minView->init(this);
    detView->init(this);


    currentView = true;
    connectState  = false;
    mbConnectedToServer = false;
	verWifiUpdateState = false;
	verThreeGUpdateState = false;
	verLteUpdateState =false;

//vpn bool to know whether Vpn is connected or not
    vpnMsgBarStatus = false;
    vpnConnectStatus = false;
    logfile.open("cmguilog.txt", ios::app);

    #ifndef QT_NO_DEBUG_OUTPUT
    qInstallMsgHandler(GUIClientLogHandler);
    #endif
    //QDebug() << "Uconnect::Uconnect()--start";
    bStatWidgetInit = false;
    detView->bStatWidgetInitCheck = false;
    autoMode=true;

    isEditRefreshChecked = false;
    timerRun= false;
    mCMInfo.m_nState = IDLE_STATE;
    mservIpAddr_p = strdup(DEFAULT_SERVER_IP);
    mservPort = serverPort;//DEFAULT_SERVER_PORT;


    serverLogName = tr("serverLog.txt");
    bFirstTimeSearchButtonClicked = false;

    detView->setLogFileName();

    //QDebug() << "Thread ID at Uconnect() constructor:" << QThread::currentThreadId();
    //QDebug() << "Uconnect()--end";


    guiLogLevel = 1;
    memset( &mCMInfo, 0, sizeof( CMinfo ) );
    memset( &mCMInfoForEvent, 0, sizeof( CMinfo ) );
    memset( &m_CMmodemStats, 0, sizeof( CMinfo ) );
	doRegisterForDevNotification();

	initialVersionSetting();
}

/////////////////////////////////////////////////////////////////////////////
//////////////////////////////// DESTRUCTOR ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Uconnect::~Uconnect()
{
    //QDebug() << "~Uconnect()--Start";
    free (mservIpAddr_p);
   // if(stMsg)
    //delete stMsg;
    if(stLTE)
    delete stLTE;

    //if(stNetValue)
   // delete stNetValue;
    if(downLink)
    delete downLink;
    if(upLink)
    delete upLink;
    if(statusBar)
    delete statusBar;
    if(periodicUpdateTimer)
    delete periodicUpdateTimer;
    //if(trayIcon)
    //delete trayIcon;




    //if(outFile)
    outFile.close();

    //if(fileptr)
       //fclose(fp);

   // logfile.close();

    //QDebug() << "~Uconnect()--End";
}





/***********************************************************************
*  This function takes the configuration parameter to be obtained from
*  the server and returns the value on success.
*
*  Parameter
*  param - Its the config param whos value to be obtained.
***********************************************************************/
U_UINT32 Uconnect::GetConfigParam(U_UINT32 param)
{
    //QDebug() << "GetConfigParam()--Start" << param;
    UGetCfgParam *reqEvnt = new UGetCfgParam(param);
	if(reqEvnt == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);

    if(rspEvnt->getCmdId() != UCONN_SAPI_WRAP_GET_RSP)
    {
        qCritical() << "Error::GetConfigParam() Failed with param:" << param;
        //stMsg->clear();
        //stMsg->setText(tr("Getparam Failed"));
         m_nstat =0;
    }
    else{
    UGetCfgRsp *getRspEvt = (UGetCfgRsp*)rspEvnt;
    if(getRspEvt->cfgParam == param)
    {
        //QDebug() << "Success::GetConfigParam() with param:" << param << "and value" << getRspEvt->cfgVal;
        m_nstat = getRspEvt->cfgVal;
    }
     }
    delete reqEvnt;
    reqEvnt = 0;
    delete rspEvnt;
    rspEvnt = 0;
    //QDebug() << "GetConfigParam()--End";
    return m_nstat;
}

U_DOUBLE Uconnect::GetFltConfigParam(U_UINT32 param)
{
    //QDebug() << "GetConfigParam()--Start" << param;
    UGetfltCfgParam *reqEvnt = new UGetfltCfgParam(param);
        if(reqEvnt == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);

    if(rspEvnt->getCmdId() != UCONN_SAPI_WRAP_GET_FLT_RSP)
    {
        qCritical() << "Error::GetConfigParam() Failed with param:" << param;
        //stMsg->clear();
        //stMsg->setText(tr("Getparam Failed"));
         m_nfltstat =0;
    }
    else{
    UGetfltCfgRsp *getRspEvt = (UGetfltCfgRsp*)rspEvnt;
    if(getRspEvt->cfgParam == param)
    {
        //QDebug() << "Success::GetConfigParam() with param:" << param << "and value" << getRspEvt->cfgVal;
        m_nfltstat = getRspEvt->cfgVal;
    }
     }
    delete reqEvnt;
    reqEvnt = 0;
    delete rspEvnt;
    rspEvnt = 0;
    //QDebug() << "GetConfigParam()--End";
    return m_nfltstat;
}


U_SINT32 Uconnect::GetSigConfigParam(U_UINT32 param)
{
    //QDebug() << "GetConfigParam()--Start" << param;
    UGetSigCfgParam *reqEvnt = new UGetSigCfgParam(param);
        if(reqEvnt == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);

    if(rspEvnt->getCmdId() != UCONN_SAPI_WRAP_GET_SIGSTATS_RSP)
    {
        qCritical() << "Error::GetConfigParam() Failed with param:" << param;
        //stMsg->clear();
        //stMsg->setText(tr("Getparam Failed"));
         m_nSigstat =0;
    }
    else{
    UGetSigCfgRsp *getRspEvt = (UGetSigCfgRsp*)rspEvnt;
    if(getRspEvt->cfgParam == param)
    {
        //QDebug() << "Success::GetConfigParam() with param:" << param << "and value" << getRspEvt->cfgVal;
        m_nSigstat = getRspEvt->cfgVal;
    }
     }
    delete reqEvnt;
    reqEvnt = 0;
    delete rspEvnt;
    rspEvnt = 0;
    //QDebug() << "GetConfigParam()--End";
    return m_nSigstat;
}

U_STRING Uconnect::GetStrConfigParam(U_UINT32 param)
{

    //QDebug() << "GetStrConfigParam()--Start";
    UGetStrCfgParam *reqEvnt = new UGetStrCfgParam(param);
    UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);

    if(rspEvnt->getCmdId() != UCONN_SAPI_WRAP_GET_STRPARAM_RSP)
    {
        qCritical() << "Error::GetStrConfigParam() Failed with param:" << param;
       // stMsg->clear();
        //stMsg->setText(tr("Getparam Failed"));
        m_nStrstat = "";
    }
    else{
    UGetStrCfgRsp *getRspEvt = (UGetStrCfgRsp*)rspEvnt;
    U_STRING temp(getRspEvt->cfgString);
    m_nStrstat = temp.c_str();
    }


    delete reqEvnt;
    reqEvnt = 0;
    delete rspEvnt;
    rspEvnt = 0;
    //QDebug() << "GetStrConfigParam()--End";
    return m_nStrstat;

}

/***********************************************************************
*  This function takes the configuration parameter and value to be set
*  and returns the generic response if success.
*
*  Parameter
*  param - Its the config param whos value to be set.
*  value - Its the value to be set to the given config param.
***********************************************************************/
void Uconnect::SetConfigParams(U_UINT32 param, U_UINT32 value)
{
    //QDebug() << "SetConfigParams()--Start";
    USetCfgParam *reqEvnt = new USetCfgParam(param,value);
	if(reqEvnt == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);

     if(rspEvnt->getCmdId() != UCONN_SAPI_WRAP_GENERIC_RESP)
    {
         qCritical() << "Error::SetConfigParams() Failed with param:" << param << " and value" << value;
         //stMsg->clear();
        // stMsg->setText(tr("Setparam Failed"));
    }
    //QDebug() << "Success::SetConfigParams() with param:" << param << " and value" << value;

    delete reqEvnt;
    delete rspEvnt;
    //QDebug() << "SetConfigParams()--End";
 }

/***********************************************************************
*  This function gets the modem statistics and set
*  the CM info structure and returns the modemstat event on success.
***********************************************************************/
void Uconnect::ModemStatParams()
{

//#if ALTAIR
#if (defined(ALTAIR)|| !defined(BROADCOM))
    //QDebug() << "ModemStatParams()--Start";
    UModemStatisticRequest *reqEvnt = new UModemStatisticRequest();
	if(reqEvnt == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);

    /*if(rspEvnt->getCmdId() != UCONN_EVT_LTE_MODEM_STATISTIC)
    {
        qCritical() << "Error::ModemStatParams() Failed";
       // stMsg->clear();
       // stMsg->setText(tr("Modem Stats Failed"));
    }
    else*/{
    UModemStatisticEvent *ModemRspEvt = (UModemStatisticEvent*)rspEvnt;
    //m_CMmodemStats.m_nModemStats = ModemRspEvt->modemStatistic;
    //strcpy(m_CMmodemStats.,ModemRspEvt->modemStatistic.L1AencodStr);
    strcpy(m_CMmodemStats.m_nL1AStats.L1AencodStr,ModemRspEvt->modemStatistic.L1AencodStr);
  //  m_CMmodemStats.m_nL1AStats = ModemRspEvt->modemStatistic;
//    delete reqEvnt;
//    reqEvnt = 0;
//    delete rspEvnt;
//    rspEvnt = 0;
    }

    delete reqEvnt;
    reqEvnt = 0;
    delete rspEvnt;
    rspEvnt = 0;
    //QDebug() << "ModemStatParams()--End";
#endif

#ifdef BROADCOM

    UBrcmStatisticRequest *reqEvnt = new UBrcmStatisticRequest();
    if(reqEvnt == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);

    if(rspEvnt->getCmdId() != UCONN_EVT_LTE_BRCM_STATISTIC_RSP)
    {
        qCritical() << "Error::ModemStatParams() Failed";
       // stMsg->clear();
       // stMsg->setText(tr("Modem Stats Failed"));
    }
    else
    {
        UBrcmStatisticEvent *ModemRspEvt = (UBrcmStatisticEvent*)rspEvnt;
        memset((void*)&m_CMmodemStats.m_nBroadStats,0,sizeof(Broadcom_Stats_s));

        strcpy(m_CMmodemStats.m_nBroadStats.genStats,ModemRspEvt->broadcomStat.genStats);
        strcpy(m_CMmodemStats.m_nBroadStats.macStats,ModemRspEvt->broadcomStat.macStats);
        strcpy(m_CMmodemStats.m_nBroadStats.phyStats,ModemRspEvt->broadcomStat.phyStats);
    }
    delete reqEvnt;
    reqEvnt = 0;
    delete rspEvnt;
    rspEvnt = 0;


#endif


}


/***********************************************************************
*  This function gets the modem statistics and set
*  the CM info structure and returns the modemstat event on success.
***********************************************************************/
void Uconnect::MeasStatParams()
{
    //QDebug() << "ModemStatParams()--Start";
    UMeasStatisticRequest *reqEvnt = new UMeasStatisticRequest();
    if(reqEvnt == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);

    if(rspEvnt->getCmdId() != UCONN_EVT_LTE_MEAS_STATISTIC_RSP)
    {
        qCritical() << "Error::ModemStatParams() Failed";
       // stMsg->clear();
       // stMsg->setText(tr("Modem Stats Failed"));
    }
    else{
    UMEASStatisticEvent *ModemRspEvt = (UMEASStatisticEvent*)rspEvnt;
    m_CMmodemStats.m_nMeasStats = ModemRspEvt->measStat;

    }

    delete reqEvnt;
    reqEvnt = 0;
    delete rspEvnt;
    rspEvnt = 0;
    //QDebug() << "ModemStatParams()--End";
}

void  Uconnect::deviceStateUpdate(U_UINT32 deviceState)
{
    switch(deviceState)
    {
    case NOTI_DEVICE_ERROR:
        mDeviceCurrentState = 1;
        break;
    case NOTI_DEVICE_NOT_OPEN:
        mDeviceCurrentState = 2;
        break;
    case NOTI_DEVICE_OPEN:
        mDeviceCurrentState = 3;
        break;
    case NOTI_DISCONNECTED:
        mDeviceCurrentState = 4;
        break;
    case NOTI_DEVICE_CONNECTED:
        mDeviceCurrentState = 5;
        break;
    case NOTI_DEVICE_DISCONNECTED:
        mDeviceCurrentState = 6;
        break;
    default:
       qCritical() << "deviceState:" << deviceState;
        break;
    }
    detView->UI_deviceStateUpdate(mDeviceCurrentState);
    minView->minUI_deviceStateUpdate(mDeviceCurrentState);
}
/***********************************************************************
*  Initialises the GUI with default values. In case of Socket,
*  this function opens a socket and connets to the Uconnect Server
***********************************************************************/
 void  Uconnect::connectToServer()
{
    //QDebug() << "connectToServer()--End";
    if(uconn_SAPIwrapInit(uconnGuiEvtHandler, NULL) == 0)
    {
        //QDebug() << "SAPI wrap is initialised";
        #ifdef UCONN_CLIENT_USE_SOCKETS
            if(uconn_SAPIwrapConnectToSever(mservIpAddr_p, mservPort)!= 0)
            {
                qCritical() << "connectToServer()--Error in connecting to server";


                mCMInfo.m_nDeviceState = GetConfigParam(SYSTEM_NOTIFICATION);
				deviceStateUpdate(mCMInfo.m_nDeviceState);
                memset( &mCMInfo, 0, sizeof( CMinfo ) );
                memset( &mCMInfoForEvent, 0, sizeof( CMinfo ) );
                memset( &m_CMmodemStats, 0, sizeof( CMinfo ) );
                memset( &m_CMmodemStats, 0, sizeof( CMinfo ) );
                detView->DisconnectSetting();
                detView->connectErrortoServer();
                detView->UI_deviceStateUpdate(2);
                minView->minUI_deviceStateUpdate(2);
                mbConnectedToServer = false;
                if(timerRun ==true)
                {
                    timer->stop();
                    timerRun = false;
                }
                detView->settingServerNotThere();
                minView->settingServerNotThere();

            }
            else
        #endif
            {
				getEnabledTechsWithPriority();
                getTechParams(ALL_TECH,UCM_VERSION);
                detView->settingServerThere();
                minView->settingServerThere();
                //QDebug() << "connectToServer()--connected to server";

                memset( &mCMInfo, 0, sizeof( CMinfo ) );
                memset( &mCMInfoForEvent, 0, sizeof( CMinfo ) );
                memset( &m_CMmodemStats, 0, sizeof( CMinfo ) );
                mCMInfo.m_nDeviceState = GetConfigParam(SYSTEM_NOTIFICATION);
                deviceStateUpdate(mCMInfo.m_nDeviceState);
                    mbConnectedToServer = true;

                mServerFSMState = GetConfigParam(CM_STATE);
                StateUpdate();
                detView->detStateUpdate();
                minView->detStateUpdate();


                mCMInfo.m_nModemState= GetConfigParam(MODEM_STATE);



                detView->connectedToServer();
                getVersionInformation();
                getConfigValuesFromServer();
                if(serverLoggingEnabled == true && !outFile.is_open())
                   {
                       outFile.open(serverLogName.toUtf8(),ios::app);
                       //outFile.open("serverLog.txt",ios::app);
                       if(!outFile.is_open())
                       {
                           qCritical() << "Please give proper name in the edit box";
                           outFile.open("serverLog.txt",ios::app);
                       }
                   }

                if(timerRun ==true)
                {
                    timer->stop();
                    timerRun = false;
                }
          }
     }
    //QDebug() << "connectToServer()--End";
}

/***********************************************************************
*  It calls the function to connect to server and also creates and
*  starts a timer thread which updates the UI with regular intervals
***********************************************************************/
void Uconnect::init()
{
    minView->show();
    detView->trayIcon->hide();
   // detView->show();
    //QDebug() << "init()--Start";
    mDeviceCurrentState = 2;
	persistNoOfWifiNetworks = 0;
    detView->initSetting();
    connectToServer();
    if(true == mbConnectedToServer)
    {
        getConfigValuesFromServer();
    }
	else
    {
        detView->DisconnectSetting();
    }



    periodicUpdateTimer =  new QTimer(this);
	if(periodicUpdateTimer == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    connect(periodicUpdateTimer, SIGNAL(timeout()), this, SLOT(periodicUpdateThread()));
    periodicUpdateTimer->start(3000);
    //if(true == mbConnectedToServer)
    //    GetConfigParam(CM_STATE);
    //QDebug() << "init()--End";
}



/***********************************************************************
* This function is called when the Gui is connected to client throught a timer thread.
* this show the APN name and time elpsed from the time its connected.
***********************************************************************/
void Uconnect::showTime()
 {

    //mCMInfo.m_altairStoredstats
   QString apn = GetStrConfigParam(APN_NAME).c_str();
   apn.remove("\"");
   QString mNC = GetStrConfigParam(MNC).c_str();
   QString mCC = GetStrConfigParam(MCC).c_str();
   U_STRING ipv4Add = GetStrConfigParam(IPv4_ADDR).c_str();

   strncpy(mCMInfo.ipAddress,ipv4Add.c_str(),ipv4Add.size());
   mCMInfo.ipAddress[ipv4Add.size()] = '\0';

    time_t tEnd_time;
    time(&tEnd_time);
    double time =difftime(tEnd_time, tStart_time);
    //QDebug() << time << "seconds";
    QString timestr = QString::number(time);
    int totalSecs = timestr.toInt(&ok,10);

    //int milisec = sec*1000;
    //secs = milisec / 1000;
   // int back_s = secs;

    QString secs = QString::number(totalSecs%60);

    int totalMins = totalSecs/60;
    QString mins = QString::number(totalMins%60);

    int totalHours = totalMins/60;
    QString hrs = QString::number(totalHours);


    QString timeStr = hrs.append(":").append(mins).append(":").append(secs);

    detView->setApnname(apn,mCC,mNC,timeStr);
   minView->setApnName(apn, timeStr);


}



/***********************************************************************
 *  -Its the fuction called in periodic timer to update the current state,
 *       this is done by calling the getconfigParam function.
 *  -The state is updated on to the screen
 ***********************************************************************/
void Uconnect::StateUpdate()
 {
 //QDebug() << "StateUpdate()--Start";
 //ui->serverState->clear();


 switch(mServerFSMState)
     {
         case IDLE_STATE:
        case ENABLED_STATE:
            {
                if(timerRun == true)
                  {
                    timer->stop();
                    timerRun = false;
                  }
            }
            break;
         case SEARCHING_STATE:
                 //QDebug() << "StateUpdate()--SEARCHING_STATE";
                 break;
         case CONNECTING_STATE:
         case DEFINE_PRIMARY_STATE:
         case DEFINE_SECONDARY_STATE:
         case NET_ATTACH_STATE:
                 //QDebug() << "StateUpdate()--CONNECTING_STATE||DEFINE_PRIMARY_STATE||DEFINE_SECONDARY_STATE||NET_ATTACH_STATE||";
                 break;
         case CONNECTED_STATE:
                 {
                 //QDebug() << "StateUpdate()--CONNECTED_STATE";
                     connectState = true;

                                 if(timerRun==false)
                                 {
                                      time(&tStart_time);
                                         timerRun = true;
                                         timer = new QTimer(this);
                                         connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
                                         //connect(timer, SIGNAL(timeout()), this, SLOT(((Uconnect*)pUConnect)->showTime()));
                                         timer->start(1000);
                                 }
                   }


                 break;
         case DISCONNECTING_STATE:
                 //QDebug() << "StateUpdate()--DISCONNECTING_STATE";

                 break;
        case DISABLED_STATE:
                 //QDebug() << "StateUpdate()--DISABLED_STATE";

                 break;

        default:
                 //statdata = "";
                 break;
     }
}


/***********************************************************************
*	-This fuction called in regular interval in a timer thread
*   -Tries to connect for server until it gets connected.
*   -Once connected checks for the state and updates the screen
***********************************************************************/
void Uconnect::periodicUpdateThread()
{
    //QDebug() << "periodicUpdateThread()--Start";
    //QDebug() << "Thread ID at periodicUpdateThread():" << QThread::currentThreadId();
    if(!mbConnectedToServer)
    {
        qCritical() << "periodicUpdateThread()--connectToServer is calling";
        connectToServer();
        qCritical() << "periodicUpdateThread()--connectToServer is called";
    }

    if(mbConnectedToServer)
    {

        //QDebug() << "periodicUpdateThread()--connected to Server";
        //StateUpdate();


        //detView->UpdateInfoTree();
        
        if(gTechDetails[LTE_TECH].state == NM_CONNECTED_STATE)
        {
                #if (defined(ALTAIR)|| !defined(BROADCOM))
                float downlink_val = (m_CMmodemStats.rxThroughput)/1024.0;
                //detView->detdownlinkSetting(downlink_val);
                minView->mindownlinkSetting(downlink_val);
                float uplink_val = ( m_CMmodemStats.txThroughput)/1024.0;
                //detView->uplinkSetting(uplink_val);
                minView->minuplinkSetting(uplink_val);

                #endif
				sigIconUpdate();
          }
        if(bStatWidgetInit == false)

        mServerFSMState = GetConfigParam(CM_STATE);
        StateUpdate();
        detView->detStateUpdate();
        minView->detStateUpdate();

        mCMInfo.m_nDeviceState = GetConfigParam(SYSTEM_NOTIFICATION);
        deviceStateUpdate(mCMInfo.m_nDeviceState);

        mCMInfo.m_nModemState= GetConfigParam(MODEM_STATE);
        detView->modemUpdate();
        if((isEditRefreshChecked ==  true) &&
           ((mDeviceCurrentState == 3) || (mDeviceCurrentState == 5)))

        {
             progressUpdate();
        }

            if(mServerFSMState == IDLE_STATE)
            {
            //QDebug() << "periodicUpdateThread()--IDLE_STATE state";

            }

             else if(mServerFSMState == CONNECTED_STATE && mbConnectedToServer && isEditRefreshChecked ==  true)
            {
                //QDebug() << "periodicUpdateThread()--CONNECTED_STATE state";
                                //downLinkText->clear();
               //upLinkText->clear();

                if((isEditRefreshChecked ==  true) &&
                   ((mDeviceCurrentState == 3) || (mDeviceCurrentState == 5)))
                {
                    sigIconUpdate();
                }
#ifdef BROADCOM
                m_CMmodemStats.m_broadcomStoredstats.u32DLBw = GetConfigParam(DL_DATA);
                float downlink_val = (float)(m_CMmodemStats.m_broadcomStoredstats.u32DLBw)/1024.0;
                detView->detdownlinkSetting(downlink_val);
                minView->mindownlinkSetting(downlink_val);


                m_CMmodemStats.m_broadcomStoredstats.u32ULBw = GetConfigParam(UL_DATA);
               float uplink_val =  (float)(m_CMmodemStats.m_broadcomStoredstats.u32ULBw)/1024.0;
               detView->uplinkSetting(uplink_val);
               minView->minuplinkSetting(uplink_val);
#endif

//#ifdef ALTAIR
#if (defined(ALTAIR)|| !defined(BROADCOM))
               float downlink_val = (m_CMmodemStats.rxThroughput)/1024.0;
               detView->detdownlinkSetting(downlink_val);
               minView->mindownlinkSetting(downlink_val);



              float uplink_val = ( m_CMmodemStats.txThroughput)/1024.0;
              detView->uplinkSetting(uplink_val);
              minView->minuplinkSetting(uplink_val);

#endif

            }

        }
    //QDebug() << "periodicUpdateThread()--End";
}


/***********************************************************************
*  -This fuction called when the connect button is clicked.
*  -It calls the netattach fuction after getting the PLMNidx value.
*  -The same connect button changes to disconnect after connecting which
*      calls the netdetach API which will end the session.
***********************************************************************/
void Uconnect::connectbuttonDetachHandler()
{
    UNetdetach *reqEvnt = new UNetdetach();
            if(reqEvnt == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);
    if(rspEvnt->getCmdId() != UCONN_SAPI_WRAP_GENERIC_RESP)
    {
        qCritical() << "on_connectButton_clicked()--netDetach failed";
       // stMsg->clear();
        //stMsg->setText(tr("Disconnection Failure"));
    }

    delete reqEvnt;
    reqEvnt = 0;
    delete rspEvnt;
    rspEvnt = 0;
}

void Uconnect::connectbuttonAttachHandler()
{
    UNetattach *reqEvnt = new UNetattach(0); // mCMInfo.PLMNidx);
    if(reqEvnt == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);

    if(rspEvnt->getCmdId() != UCONN_SAPI_WRAP_GENERIC_RESP)
    {
        qCritical() << "on_connectButton_clicked()--netattach failed";
       // stMsg->clear();
       // stMsg->setText(tr("Connection Failure"));
    }

    delete reqEvnt;
    reqEvnt = 0;
    delete rspEvnt;
    rspEvnt = 0;
}
/***********************************************************************
*  -This fuction called when the search button is clicked,
*        which calls the netsearch fuction
*  -When the session is through socket than netsearchresponse function
*       is call to display the search results on the Ui.
***********************************************************************/
void Uconnect::searchbuttonHandler()
{
    UNetsearch *reqEvnt = new UNetsearch(10);
        if(reqEvnt == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);

    if(rspEvnt->getCmdId() != UCONN_SAPI_WRAP_GENERIC_RESP)
    {
        qCritical() << "on_searchButton_clicked()--Net Search Failed";
      //  stMsg->clear();
      //  stMsg->setText(tr("Net Search Failed"));
    }
    else
    {
        GetNetworkSearchStatus();
    }
    delete reqEvnt;
    reqEvnt = 0;
    delete rspEvnt;
    rspEvnt = 0;
    #ifndef UCONN_CLIENT_USE_SOCKETS
    GetNetworkSearchStatus();
    #endif
}

/***********************************************************************
*  -This fuction is called periodically through the timer thread
* which calls the netsearchstatus and updates the Ui with new values
***********************************************************************/
void Uconnect::GetNetworkSearchStatus()
{
    //QDebug() << "GetNetworkSearchStatus()--Start";
    QString plmn_srch_res[10];
    UNetsearch *reqEvnt1 = new UNetsearch();
	if(reqEvnt1 == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt1 = uconn_SAPIExecute (reqEvnt1);
    NetSearch_Result_s *searchStatus_p =
            &((UNetSearchRsp *)rspEvnt1)->nwSearchRsp;
    setSearchResults(searchStatus_p);
    //ui->connectButton->setDisabled(false);


    switch(rspEvnt1->getExecStatus())
        {
            case SRCH_COMPLETED:
                    for(U_UINT32 i = 0; i < searchStatus_p->num_plmns; i++)
                    {
                        plmn_srch_res[i] =
                            searchStatus_p->nw_list[i].free_text;
                        tNetworkList.insert(i,"\t");
                        tNetworkList.append(plmn_srch_res[i]);
                        detView->searchcompletelistHandler(i,tNetworkList);
                    }
                    tNetworkList.clear();
                    break;
            case SRCH_INPROGRESS:
                    //QDebug() << "Searching Networks...";
                    break;
            case SRCH_NOTDONE:
                    //QDebug() << "Net Search Not Done";
                    break;
            case SRCH_FAILED:
                    //QDebug() << "Net Search Failed";
                    break;
        }

    delete searchStatus_p;
    searchStatus_p = 0;
    delete reqEvnt1;
    reqEvnt1 = 0;
    delete rspEvnt1;
    rspEvnt1 = 0;
    //QDebug() << "GetNetworkSearchStatus()--End";
}

/***********************************************************************
*  -This fuction is called when unsolicitated event occur from the server.
***********************************************************************/
U_VOID uconnGuiEvtHandler (UEvent* evt, U_SINT8 *, U_VOID *)
{
    //QDebug() << "uconnGuiEvtHandler()--Start-- Thread ID for Registered Event Handler:" << QThread::currentThreadId();
    //QDebug() << "evt:" << evt << "and &evt:" << &evt;
    //QDebug() << "Event command id recvd: mCmdId: " <<evt->mCmdId;
    switch(evt->getCmdId())
    {


        case UCONN_CMD_SERVER_LOG_LEVEL:
            //QDebug() << "Event received is UCONN_CMD_SERVER_LOG_LEVEL";
            gUConnect->uconnGuiLogLevelEventHandler(evt);
            break;
        case UCONN_CMD_LTE_EQUIP_CONFIG_MODE:
            //QDebug() << "Event received is UCONN_CMD_LTE_EQUIP_CONFIG_MODE";
            gUConnect->uconnGuiConfigModeEventHandler(evt);
            break;
        case UCONN_CMD_LTE_4G_ONOFF:
            {
            //QDebug() << "Event received is UCONN_CMD_LTE_4G_ONOFF";
            //gUConnect->uconnGuiConfig4GEventHandler(evt);
            }
            break;
        case UCONN_CMD_SERVER_LOG_ONOFF:
            //QDebug() << "Event received is UCONN_CMD_SERVER_LOG_ONOFF";
            gUConnect->uconnGuiServerLoggingEventHandler(evt);
            break;
        case UCONN_EVT_LOG:
            {
                //QDebug() << "Event received is UCONN_EVT_LOG";
                gUConnect->uconnGuiLogEventHandler(evt);
            }
            break;
    case UCONN_EVT_NM_SCAN_RESP:
            {

            //QDebug() <<"Event received is UCONN_EVT_NM_SCAN_RSP";
            gUConnect->uconnScanResponseHandler(evt);
            gUConnect->detView->isNetworkSelected =false;
            }
            break;
    case UCONN_EVT_WIFI_PREFERRED_NETWORKS:
            {
                gUConnect->wifiPrefHandler(evt);

            }
            break;
    case UCONN_EVT_LTE_MODEM_STATISTIC:
            {
                gUConnect->uconnStatsMessageHandler(evt);
            }
            break;
    case UCONN_EVT_LTE_MEAS_STATISTIC_RSP:
            {
                gUConnect->uconnMeasStatsHandler(evt);
            }
            break;
    case UCONN_EVT_WIFI_STATS_RSP:
            {
                gUConnect->wifiStatsRspHandler(evt);

            }
            break;
    case UCONN_EVT_NM_CONNECT_STATUS:
           {
               gUConnect->handleConnectStatusEvt(evt);
           }
            break;

    case UCONN_EVT_3G_STATS_RSP:
           {
               gUConnect->threeGStatsHandler(evt);

           }
           break;
    case UCONN_EVT_NM_SHOW_SETTINGS:
           {
               gUConnect->handleShowNMSettingsEvt(evt);

           }
           break;

    case UCONN_EVT_BEARER_PRIORITY_TECH_RSP:
           {
               gUConnect->updateTechPriority(evt);

           }
           break;
    case UCONN_EVT_BEARER_ENABLED_TECH_RSP:
           {
               gUConnect->handleEnabledTechListEvt(evt);
           }
           break;

    case UCONN_EVT_NM_STATE_CHANGE:
           {
               gUConnect->handleNMStateChangeEvt(evt);
           }
           break;

    case UCONN_EVT_GET_BEARER_PARAM_RSP:
           {
               gUConnect->handleGetParamRspHandler(evt);

           }
           break;
    case UCONN_EVT_WIFI_DISABLED_NOTI:
        {
               gUConnect->handleWifiDisabledNotification();
        }
        break;
    case UCONN_CMD_SET_3G_RAS_PROFILE:
        {
            UNM3GSetProfile* setProfile = reinterpret_cast<UNM3GSetProfile*>(evt);
            gUConnect->showInitialRasSettings(setProfile->rasDetails);
        }
        break;
		//VPN EVENTS FROM SERVER
	case UCONN_EVT_VPN_LIST_CLIENTS:
		{
			gUConnect->handleVpnCliList(evt);
		}
		break;
	case UCONN_EVT_VPN_LIST_PROFILES:
		{
			gUConnect->handleVpnProfList(evt);
		}
		break;
	case UCONN_EVT_VPN_START_STATUS:
		{
            gUConnect->handleVpnConnect(evt);
		}
		break;
	case UCONN_EVT_VPN_STOP_STATUS:
		{
			gUConnect->handleVpnDisconnect();
		}
		break;


#if UCONN_CLIENT_USE_SOCKETS
        case UCONN_EVT_SERVER_CONN_LOST:
            //QDebug() << "Event received is UCONN_EVT_SERVER_CONN_LOST";
            gUConnect->setServerConnStatus(false);
            break;
#endif /*UCONN_CLIENT_USE_SOCKETS*/

         case UCONN_EVT_NM_USSD_RSP:
          {

            GuiNMUssdResponseEvt *guidUssdEvt = new GuiNMUssdResponseEvt(
                    ((UNMUSSDToGUI *)evt)->NM_USSDString);
             QApplication::postEvent(gUConnect,guidUssdEvt);

            }
             break;
        case UCONN_EVT_NM_ERROR_DISPLAY:
            {
               gUConnect->handleNMErrorDisplay(evt);
            }
            break;
         default:
            //QDebug() << "Unknown event at eventhandler:" << evt->getCmdId();
            break;
    }
    //////QDebug() << "uconnGuiEvtHandler()--exiting event handler" << evt->getCmdId();
}


void Uconnect::sigIconUpdate()
{
    //QDebug() << "sigIconUpdate()--Start";
    int nImageToShow = 0;
    if(mCMInfo.m_nDeviceState == NOTI_DEVICE_DISCONNECTED)
    {
        nImageToShow = 0;
    }
    else
    {

       // U_SINT32 sigValue = GetSigConfigParam(SIG_STRENGTH);
       // sigValue = abs(sigValue);
       // int value  = GetFltConfigParam(RSSI_VAL);
        
    }

     //QDebug() << "sigIconUpdate()--End";
}

void Uconnect::calculateSignalStrength(ETechSelect techId,float sigval)
{
    int nImageToShow = 0;
    if(mCMInfo.m_nDeviceState == NOTI_DEVICE_DISCONNECTED)
    {
        nImageToShow = 0;
    }
    else
    {
		if(techId == LTE_TECH || techId == THREE_G_TECH)
		{
			U_UINT32 sigValue = abs(sigval);
			if(sigValue <= 44)
					nImageToShow = 4;
			else if(sigValue > 44 && sigValue <= 79)
					nImageToShow = 3;
			else if(sigValue > 79 && sigValue <= 114)
					nImageToShow = 2;
			else if(sigValue > 114 && sigValue <= 140)
					nImageToShow = 1;
			else
					nImageToShow = 0;
		}
		else if(techId == WIFI_TECH)
		{
					U_UINT32 sigValue = abs(sigval);
			if(sigValue <= 100)
					nImageToShow = 5;
			else if(sigValue > 100 && sigValue <= 80)
					nImageToShow = 5;
			else if(sigValue > 80 && sigValue <= 60)
					nImageToShow = 4;
			else if(sigValue > 60 && sigValue <= 40)
					nImageToShow = 3;
			else if(sigValue > 40 && sigValue <= 20)
					nImageToShow = 2;
			else if(sigValue > 20 && sigValue <= 10)
					nImageToShow = 1;
			else
					nImageToShow = 0;
	
		}
	}
if(gTechDetails[techId].state == NM_CONNECTED_STATE)
{
	minView->sigIconupdateMinviewSetting(nImageToShow,techId);
}

}


void Uconnect::uconnGuiLogEventHandler(UEvent *evt_p)
{
    //QDebug() << "uconnGuiLogEventHandler()--Start";

    ULogEvent *logEvt_p = reinterpret_cast<ULogEvent *>(evt_p);

    /* write the data to the file */
    bool result = outFile.is_open();
    if(result == false)
    {
       //QDebug() << "The file is not open to log";
    }
    else
    {
        outFile << (const U_SINT8*)logEvt_p->logString;
    }
    //QDebug() << "uconnGuiLogEventHandler()--End";
}


void Uconnect::uconnGuiLogLevelEventHandler(UEvent *evt_p)
{
    //QDebug() << "uconnGuiLogLevelEventHandler()--Start";
    ULogLevelEvent *logEvt_p = reinterpret_cast<ULogLevelEvent *>(evt_p);
    if(ULOG_DEBUG_LEVEL == logEvt_p->cfglogLevel)
    {
        //QDebug() << "ULOG_DEBUG_LEVEL event is received";
        //QEvent *t = new QEvent(guiLogDebugEvent);
        QApplication::postEvent(this, new QEvent(guiLogDebugEvent));
    }
    else if(ULOG_INFO_LEVEL == logEvt_p->cfglogLevel)
    {
        //QDebug() << "ULOG_INFO_LEVEL event is received";
        //QEvent *t = new QEvent(guiLogInfoEvent);
        QApplication::postEvent(this, new QEvent(guiLogInfoEvent));
    }
    else if(ULOG_ERROR_LEVEL == logEvt_p->cfglogLevel)
    {
        //QDebug() << "ULOG_ERROR_LEVEL event is received";
        //QEvent *t = new QEvent(guiLogErrorEvent);
        QApplication::postEvent(this, new QEvent(guiLogErrorEvent));
    }
    //QDebug() << "uconnGuiLogLevelEventHandler()--End";
}

void Uconnect::uconnGuiConfigModeEventHandler(UEvent *evt_p)
{
    //QDebug() << "uconnGuiConfigModeEventHandler()--Start";
    UConfigModeEvent *modeEvt_p = reinterpret_cast<UConfigModeEvent *>(evt_p);
    if(0 == modeEvt_p->cfgmode)
    {
        //QDebug() << "0--configmode event is received";
        //QEvent *t = new QEvent(guiCfgAutoEvent);
        QApplication::postEvent(this, new QEvent(guiCfgAutoEvent));
    }
    else if(1 == modeEvt_p->cfgmode)
    {
        //QDebug() << "1--configmode event is received";
        //QEvent *t = new QEvent(guiCfgManuEvent);
        QApplication::postEvent(this, new QEvent(guiCfgManuEvent));
    }
    //QDebug() << "uconnGuiConfigModeEventHandler()--End";
}


void Uconnect::uconnGuiServerLoggingEventHandler(UEvent *evt_p)
{
    //QDebug() << "uconnGuiServerLoggingEventHandler()--Start";
    ULogOnOffEvent *serverLogEvt_p = reinterpret_cast<ULogOnOffEvent *>(evt_p);
    if(0 == serverLogEvt_p->cfglogOnOff)
    {
        //QDebug() << "0--server log is enabled event is received";
        //QEvent *t = new QEvent(guiServerLogEnabledEvent);
        QApplication::postEvent(this, new QEvent(guiServerLogEnabledEvent));
    }
    else if(1 == serverLogEvt_p->cfglogOnOff)
    {
        //QDebug() << "1--server log is disabled event is received";
        //QEvent *t = new QEvent(guiServerLogDisabledEvent);
        QApplication::postEvent(this, new QEvent(guiServerLogDisabledEvent));
    }
    //QDebug() << "uconnGuiServerLoggingEventHandler()--End";
}

void Uconnect::getConfigValuesFromServer()
{
    //QDebug() << "getConfigValuesFromServer()--Start";
    getVersionInformation();

    mServerFSMState = GetConfigParam(CM_STATE);
    StateUpdate();
    detView->detStateUpdate();
    minView->detStateUpdate();

    mCMInfo.m_nModemState = GetConfigParam(MODEM_STATE);
    if(mCMInfo.m_nModemState == 0)
        memset( &mCMInfo, 0, sizeof( CMinfo ) );
    detView->modemStateUpdate();

    mCMCfgInfo.Log_level = GetConfigParam(CFG_LOGGING_LEVEL);
    detView->logLevel();
    if(ULOG_DEBUG_LEVEL == mCMCfgInfo.Log_level)
    {
        //QDebug() << "getConfigValuesFromServer()--Log_level:ULOG_DEBUG_LEVEL";
        guiLogLevel = 1;
    }
    else if(ULOG_INFO_LEVEL == mCMCfgInfo.Log_level)
    {
        //QDebug() << "getConfigValuesFromServer()--Log_level:ULOG_INFO_LEVEL";
        guiLogLevel = 1;
    }
    else if(ULOG_ERROR_LEVEL == mCMCfgInfo.Log_level)
    {
        //QDebug() << "getConfigValuesFromServer()--Log_level:ULOG_ERROR_LEVEL";
        guiLogLevel = 2;
    }

    mCMCfgInfo.Enable_Log = GetConfigParam(CFG_ENABLE_LOG);
    {
        detView->enableLog();


    }

    //QDebug() << "getConfigValuesFromServer()--End";
}


void Uconnect::setServerConnStatus(bool)
{
    //QDebug() << "setServerConnStatus()--Start";
    QApplication::postEvent(this, new QEvent(guiCMServerLostEvent));
    //QDebug() << "setServerConnStatus()--End";
}

void Uconnect::customEvent(QEvent *custEvt)
{
    //////QDebug() << "customEvent()--Start--Thread ID at customEvent:" << QThread::currentThreadId();
    switch(custEvt->type())
    {
        /*case CUST_IDLE_EVENT:
        case CUST_ENABLE_EVENT:
         {
            //QDebug() << "idleEvent event received in main thread";
            mServerFSMState = IDLE_STATE;
            detView->custidleEnableEventSetting();
            minView->custidleEnableEventSetting();
            if(timerRun==true )
            {
            timer->stop();
            timerRun = false;
            }

         }
         break;
        case CUST_SEARCH_EVENT:
         {
            //QDebug() << "searchEvent event received in main thread";
            mServerFSMState = SEARCHING_STATE;
            detView->custSearchEventSetting();
         }
         break;
        case CUST_CONNECTING_EVENT:
         {
             mServerFSMState = CONNECTING_STATE;
             //QDebug() << "connectingEvent  event received in main thread";
             detView->custConnectingEventSetting();
         }
         break;
        case CUST_CONNECTED_EVENT:
         {
             //QDebug() << "connected event received in main thread";
             mServerFSMState = CONNECTED_STATE;
             detView->custConnectedEventSetting();
             minView->custConnectedEventSetting();
             if(timerRun==false)
             {
                 time(&tStart_time);
                 timerRun = true;
                 timer = new QTimer(this);
                 connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
                 timer->start(1000);
             }

             mCMInfoForEvent.m_nModemState= GetConfigParam(MODEM_STATE);
             detView->modemUpdate();
         }
         break;
        case CUST_DISCONNECTING_EVENT:
         {
             //QDebug() << "disConntingEvent event received in main thread";
             mServerFSMState = DISCONNECTING_STATE;
             detView->custDisconnectedEventSetting();
         }
         break;*/
        case CUST_DISABLE_EVENT:
         {
             //QDebug() << "disableEvent event received in main thread";
             mServerFSMState = DISABLED_STATE;
             memset( &mCMInfo, 0, sizeof( CMinfo ) );
             memset( &mCMInfoForEvent, 0, sizeof( CMinfo ) );
             memset( &m_CMmodemStats, 0, sizeof( CMinfo ) );
             detView->custDisableEventSetting();
             if(timerRun ==true)
             {
                 timer->stop();
                 timerRun = false;
             }

         }
         break;
        case CUST_AUTO_EVENT:
         {
            //QDebug() << "guiCfgAutoEvent::1244";
            detView->custEvents(CUST_AUTO_EVENT);

         }
         break;
        case CUST_MANUAL_EVENT:
         {
            //QDebug() << "guiCfgManuEvent::1245";
            detView->custEvents(CUST_MANUAL_EVENT);
        }
         break;
        case CUST_DEBUG_EVENT:
         {
            //QDebug() << "guiLogDebugEvent::1246";
            detView->custEvents(CUST_DEBUG_EVENT);
            guiLogLevel = 1;
         }
        break;
        case CUST_INFO_EVENT:
        {
            //QDebug() << "guiLogInfoEvent::1247";
            detView->custEvents(CUST_INFO_EVENT);
            guiLogLevel = 1;
        }
        break;
        case CUST_ERROR_EVENT:
        {
            //QDebug() << "guiLogErrorEvent::1248";
            detView->custEvents(CUST_ERROR_EVENT);
            guiLogLevel = 2;
        }
         break;
        case CUST_SER_LOG_ENABLED_EVENT:
         {
            //QDebug() << "guiServerLogEnabledEvent::1249";
            detView->custEvents(CUST_SER_LOG_ENABLED_EVENT);

        }
         break;
        case CUST_SER_LOG_DISABLED_EVENT:
         {
             //QDebug() << "guiServerLogDisabledEvent::1250";
            detView->custEvents(CUST_SER_LOG_DISABLED_EVENT);

         }
         break;
        case CUST_SYS_DEVICE_ERROR_EVENT:
         {
            //QDebug() << "guiSysDeviceErrorEvent::1251";
            detView->custEvents(CUST_SYS_DEVICE_ERROR_EVENT);
            minView->custEvents(CUST_SYS_DEVICE_ERROR_EVENT);
            mDeviceCurrentState = 1;
            memset( &mCMInfo, 0, sizeof( CMinfo ) );
            memset( &mCMInfoForEvent, 0, sizeof( CMinfo ) );
            memset( &m_CMmodemStats, 0, sizeof( CMinfo ) );
            if(timerRun ==true)
            {
                timer->stop();
                timerRun = false;
            }

        }
         break;
        case CUST_SYS_DEVICE_NOT_OPEN_EVENT:
         {
            detView->custEvents(CUST_SYS_DEVICE_NOT_OPEN_EVENT);
            //QDebug() << "guiSysDeviceNotOpenEvent::1252";
            mDeviceCurrentState = 2;
            memset( &mCMInfo, 0, sizeof( CMinfo ) );
            memset( &mCMInfoForEvent, 0, sizeof( CMinfo ) );
            memset( &m_CMmodemStats, 0, sizeof( CMinfo ) );
            if(timerRun ==true)
            {
                timer->stop();
                timerRun = false;
            }
        }
         break;
        case CUST_SYS_DEVICE_OPEN_EVENT:
         {
            //QDebug() << "guiSysDeviceOpenEvent::1253";
            mDeviceCurrentState = 3;
            memset( &mCMInfo, 0, sizeof( CMinfo ) );
            memset( &mCMInfoForEvent, 0, sizeof( CMinfo ) );
            memset( &m_CMmodemStats, 0, sizeof( CMinfo ) );
            detView->custEvents(CUST_SYS_DEVICE_OPEN_EVENT);
            if(timerRun ==true)
            {
                timer->stop();
                timerRun = false;
            }
        }
         break;
        case CUST_SYS_DISCONNECTED_EVENT:
         {
            //QDebug() << "guiSysDisconnectedEvent::1254";
            detView->custEvents(CUST_SYS_DISCONNECTED_EVENT);
            minView->custEvents(CUST_SYS_DISCONNECTED_EVENT);
            mDeviceCurrentState = 4;
            memset( &mCMInfo, 0, sizeof( CMinfo ) );
            memset( &mCMInfoForEvent, 0, sizeof( CMinfo ) );
            memset( &m_CMmodemStats, 0, sizeof( CMinfo ) );
            if(timerRun ==true)
            {
                timer->stop();
                timerRun = false;
            }
        }
         break;
        case CUST_SYS_DEVICE_CONNECTED_EVENT:
         {
            //QDebug() << "guiSysDeviceConnectedEvent::1255";
            detView->custEvents(CUST_SYS_DEVICE_CONNECTED_EVENT);
            mDeviceCurrentState = 5;
            connectState = true;
            memset( &mCMInfo, 0, sizeof( CMinfo ) );
            memset( &mCMInfoForEvent, 0, sizeof( CMinfo ) );
            memset( &m_CMmodemStats, 0, sizeof( CMinfo ) );
            if(timerRun ==true)
            {
                timer->stop();
                timerRun = false;
            }
        }
         break;
        case CUST_SYS_DEVICE_DISCONNECTED_EVENT:
         {
            //QDebug() << "guiSysDeviceDisconnectedEvent::1256";
            detView->custEvents(CUST_SYS_DEVICE_DISCONNECTED_EVENT);
            minView->custEvents(CUST_SYS_DEVICE_DISCONNECTED_EVENT);
            mDeviceCurrentState = 6;
            memset( &mCMInfo, 0, sizeof( CMinfo ) );
            memset( &mCMInfoForEvent, 0, sizeof( CMinfo ) );
            memset( &m_CMmodemStats, 0, sizeof( CMinfo ) );

            if(timerRun ==true)
            {
                timer->stop();
                timerRun = false;
            }
        }
         break;
        case CUST_CM_SERVER_CONN_LOST:
         {
            //QDebug() << "guiCMServerLostEvent::1257";
             mbConnectedToServer = false;
             connectState = false;
            deviceStateUpdate(mCMInfo.m_nDeviceState);
            memset( &mCMInfo, 0, sizeof( CMinfo ) );
            memset( &mCMInfoForEvent, 0, sizeof( CMinfo ) );
            memset( &m_CMmodemStats, 0, sizeof( CMinfo ) );
            detView->DisconnectSetting();
            minView->DisconnectSetting();
            detView->DisplayApnTitle();
            detView->custEvents(CUST_CM_SERVER_CONN_LOST);
            minView->custEvents(CUST_CM_SERVER_CONN_LOST);
            if(timerRun ==true)
            {
                timer->stop();
                timerRun = false;
            }

        }
         break;
    case CUST_SCAN_RESPONSE_EVENT:
         {
             GuiNMScanRspEvt* scanEvt = reinterpret_cast<GuiNMScanRspEvt*>(custEvt);
             switch(scanEvt->networkListEncoded.techId)
             {
             case WIFI_TECH:
                 {
                    gUConnect->handleWifiScanResponse(scanEvt->networkListEncoded);
                 }
                 break;
             }

         }
         break;
    case CUST_WIFI_PREF_EVENT:
         {
             GuiWifiPreferredRspEvt* wifiPreferred = reinterpret_cast<GuiWifiPreferredRspEvt*>(custEvt);
             gUConnect->showPreferredWifiNetworks(wifiPreferred->encodedSsids);

         }
         break;
    case CUST_WIFI_STATS_RSP_EVENT:
         {
             GuiWifiStatsRspEvt* wifiStats = reinterpret_cast<GuiWifiStatsRspEvt*>(custEvt);
             gUConnect->showWifiStats(wifiStats->encodedSsids);

         }
         break;
    case CUST_WIFI_PASSWORD_EVENT:
         {
             GuiWifiPasswordReqEvt* reqPassword = reinterpret_cast<GuiWifiPasswordReqEvt*>(custEvt);
             gUConnect->handleWifiPasswordReq(reqPassword->ssid);
         }
         break;
    case CUST_3G_STATS_EVENT:
         {
             Gui3gStatsEvt* stats = reinterpret_cast<Gui3gStatsEvt*>(custEvt);
             gUConnect->handle3GStatsRsp(stats->stats);

         }
         break;

    case CUST_ADD_INITIAL_TECHS_EVT:
         {
             gUConnect->showInitialTechsInGui();

         }
         break;
    case CUST_HANDLE_NM_STATE_CHANGE_EVT:
         {
             GuiNMStateChangeEvt* evt = reinterpret_cast<GuiNMStateChangeEvt*>(custEvt);
             gUConnect->updateGuiOnNMStateChange(evt->stateChange);

         }
         break;
    case CUST_SHOW_GOT_NM_PARAM:
         {
            GuiNMGetParamEvt* getParamEvt = reinterpret_cast<GuiNMGetParamEvt*>(custEvt);
            gUConnect->updateParamGotInGui(getParamEvt->getParam);

         }
         break;
    case CUST_USSD_RESPONSE:
    {
        GuiNMUssdResponseEvt * ussdEvt = (GuiNMUssdResponseEvt*)custEvt;
        gUConnect->detView->displayUssdRespose(ussdEvt->ussd_response);

    }
    break;
    case CUST_NM_CONNECT_FAIL_EVT:
    {
        GuiConnectFail *connectFailEvt = (GuiConnectFail*)custEvt;
        gUConnect->displayConnectFailureInGui(connectFailEvt->techId,connectFailEvt->networkName);
    }
    break;
    case CUST_WIFI_DISABLED_NOTI_EVT:
    {
        gUConnect->showWifiDisabledNotification();
    }
    break;
    case CUST_INITIAL_RAS_PROFILE_EVT:
    {
        GuiRasProfileEvt* rasEvt = (GuiRasProfileEvt*)custEvt;
        gUConnect->handleInitialRasProf(rasEvt->details);

    }
    break;
    case CUST_ERROR_DISPLAY_EVT:
    {
        GuiErrorDisplayEvt* errorEvt = reinterpret_cast<GuiErrorDisplayEvt*>(custEvt);
        gUConnect->displayErrorMessage(errorEvt->errorInfo);
    }
    break;
	case CUST_VPN_PROFILE_LIST_EVT:
	{
		GuiVpnProfileLstEvt *profListEvt =(GuiVpnProfileLstEvt*)custEvt;
        gUConnect->detView->showVpnProfileslist(profListEvt->profLst);
	}
	break;
	case CUST_VPN_CLIENT_LIST_EVT:
	{
		GuiVpnClientLstEvt *cliListEvt =(GuiVpnClientLstEvt*)custEvt;
        gUConnect->detView->showVpnClientlist(cliListEvt->cliLst);
	}
	break;
    case CUST_VPN_CONNECT_EVT:
    {
        GuiVpnConnectEvt *vpnConnectEvt = (GuiVpnConnectEvt*)custEvt;
        gUConnect->detView->showVpnConnectSettings(vpnConnectEvt->connectRsp);
    }
    break;
    case CUST_VPN_DISCONNECT_EVT:
    {
        gUConnect->detView->showVpnDisconnectSettings();
    }
    break;

   default:

            //QDebug() << "Unknown custEvt";
         break;

     }
//	 delete custEvt;
}

void  Uconnect::sendUSSDReq(string req){
    UNMSendUSSD *evt = new UNMSendUSSD(req);
     this->sendEvent(evt);

}

double Uconnect::signedToFloat(unsigned int u32Value, unsigned int u32SignBit, unsigned int u32IntegerBits, unsigned int u32FractionBits)
{

               unsigned int u32IFBits    = u32IntegerBits + u32FractionBits;
               unsigned int u32TotalBits = (u32SignBit ? 1 : 0) + u32IFBits;

               unsigned int u32SignBitMask = (1 << (u32IFBits));
               unsigned int u32Mask = u32TotalBits == 32 ? 0xFFFFFFFF : (1 << u32TotalBits) - 1;

               double dValue, dScale;

               // Mask out only the required bits
               u32Value &= u32Mask;

               // Signed & unsigned conversions differ
               if (u32SignBit)
                              {
                              // Sign extend to 32 bits
                              if (u32Value & u32SignBitMask)
                                             u32Value |= ~u32Mask;
                              dValue = (double) ((signed int) u32Value);
                              }
               else
                              dValue = (double) u32Value;

               dScale = (double) (1 << u32FractionBits);

               return dValue/dScale;

}


void Uconnect::progressUpdate()
{

#ifdef BROADCOM
    //QDebug() << "progressUpdate()--Start";

   /* RSSi = 0;
    qreal power = 10.0;
    qreal ant0 = qPow(power,(this->Ant0/10.0));
    qreal ant1 = qPow(power,(this->Ant1/10.0));
    ant0+=ant1;
    double doubleRssi = 10.0 *(log10(ant0));
    int dupRSSi = abs(doubleRssi);
    if (dupRSSi >= 120)
    {
        dupRSSi = 0;
    }
    else if (dupRSSi <= 0)
    {
        dupRSSi = 120;
    }
    else
    {
        dupRSSi = 120-dupRSSi;
    }


    CINr = 0;
    CINr= GetConfigParam(CINR_VAL);
    //int backupCINr = CINr;
    u32SignBit = 1;
    u32IntegerBits = 15;
    u32FractionBits = 16;
    double doubleCinr;
    doubleCinr = signedToFloat(CINr,u32SignBit,u32IntegerBits,u32FractionBits);
    CINr = doubleCinr;
    //double caliberatedCinr = doubleCinr;
    if ((CINr >= -6) && (CINr <= 42))
    {
        CINr += 6;
    }
    else if (CINr < -6)
    {
        CINr = 0;
    }
    else if (CINr > 42)
    {
        CINr = 48;
    }

*/
    RSSi = 0.0;
    RSSi  = GetFltConfigParam(RSSI_VAL);
    float temp = floor((RSSi*100.0)+0.5)/100.0;
    //QString strRSSi(tr("%1dBm").arg(temp));
    RSSi = abs(temp);
    if (RSSi >= 120)
       {
           RSSi = 0;
       }
       else if (RSSi <= 0)
       {
           RSSi = 120;
       }
       else
       {
           RSSi = 120-RSSi;
       }

       CINr = 0;
       CINr= GetFltConfigParam(CINR_VAL);
       // QString strCINR(tr("%1dB").arg(CINr));
detView->progressbarSettings(RSSi,CINr,temp);
#endif
//#ifdef ALTAIR
#if (defined(ALTAIR)|| !defined(BROADCOM))
    RSSi =0;
    CINr =0;
    //m_CMmodemStats.m_altairStoredstats.RSSI = GetSigConfigParam(RSSI_VAL);
    //m_CMmodemStats.m_altairStoredstats.RSSI = GetSigConfigParam(CINR_VAL_VAL);
    //RSSi = GetFltConfigParam(RSSI_VAL);
    //CINr = GetFltConfigParam(CINR_VAL);
    RSSi = m_CMmodemStats.m_nAltairStats.RSRP_Reported;
    CINr = m_CMmodemStats.m_nAltairStats.SINR_Reported;
   // RSSi = m_CMmodemStats.m_altairStoredstats.RSSI;
    //CINr = m_CMmodemStats.m_altairStoredstats.CINR;
    float temp = RSSi;
    detView->progressbarSettings(RSSi,CINr,temp);
 #endif
  // detView->progressbarSettings(dupRSSi,CINr);
    //QDebug() << "progressUpdate()--End";
}

void Uconnect::getVersionInformation()
{
   /* UEvent *reqCmd = NULL;
    UEvent *rspCmd = NULL;
    UVersionInfoResponse *verRsp;
    
    /* When there is no Server, reset Version info to zeroes */
    /*if(mDeviceCurrentState != 2)
    {
        reqCmd = new UVersionInfoRequest();
        rspCmd = uconn_SAPIExecute(reqCmd);

        verRsp = (UVersionInfoResponse*)rspCmd;
    }
    else    /* When server is unavailable */
    /*{
        LTE_Stack_Version_Info_s versionInfo_s;
        memset(&versionInfo_s, 0, sizeof(LTE_Stack_Version_Info_s));
        verRsp = new UVersionInfoResponse(versionInfo_s);
    }
   //tmpStr.append(QString(verRsp->verInfo.u8VersionString));
    //disp.append(tmpStr);
	
    //updateVersion(nmGetParam.techId,nmGetParam.param);
    //disp.append(techVersion);
   // tmpStr.clear();
    /*if(mDeviceCurrentState != 2)
    {
        delete reqCmd;
        reqCmd = 0;
        delete rspCmd;
        rspCmd = 0;
	}*/
}

void Uconnect::fileloggingHandle()
{
    ULogOnOffEvent *logCommand = new ULogOnOffEvent(1);
        if(logCommand == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt1 = uconn_SAPIExecute (logCommand);
    if(rspEvnt1->getCmdId() != UCONN_SAPI_WRAP_GENERIC_RESP)
    {
        qCritical() << "Error::LogOnOffEvent() Failed";
        //stMsg->clear();
        //stMsg->setText(tr("Log OnOff Failed"));
    }
}

void Uconnect::switchView(){

    if(minView->isVisible()){
       // minView->hide();
        minView->trayIcon->show();
        detView->show();
       // detView->trayIcon->show();
    }
    else{
        detView->hide();
        detView->trayIcon->hide();
       // minView->show();
        minView->trayIcon->show();

    }
}

#ifdef WIN32
bool Uconnect::winEvent(MSG * msg, long *)
{
    int msgType = msg->message;
    if(msgType == WM_DEVICECHANGE)
    {
        //QDebug() << "winEvent in MgFrame : WM_DEVICECHANGE-->" << msg->wParam;
        switch(msg->wParam)
        {
            case DBT_DEVICEARRIVAL:
            {
               // detView->trayIcon->showMessage("USB Device Message", "Device is Plugged-in", QSystemTrayIcon::Critical, 151000);
               // minView->trayIcon->showMessage("USB Device Message", "Device is Plugged-in", QSystemTrayIcon::Critical, 151000);
                //QDebug() << "---->DBT_DEVICEARRIVAL";
            }
            break;
            case DBT_DEVICEREMOVECOMPLETE:
            {
                //detView->trayIcon->showMessage("USB device Message", "Device is Plugged-out", QSystemTrayIcon::Critical, 151000);
                //minView->trayIcon->showMessage("USB device Message", "Device is Plugged-out", QSystemTrayIcon::Critical, 151000);
                //QDebug() << "---->DBT_DEVICEREMOVECOMPLETE";
            }
            break;
         } // skip the rest
    } // end of if msgType
    return false; // let qt handle the rest
}
#endif

bool Uconnect::doRegisterForDevNotification()
{
#ifdef WIN32
    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
    NotificationFilter.dbcc_size = sizeof(NotificationFilter);
    NotificationFilter.dbcc_devicetype =  DBT_DEVTYP_DEVICEINTERFACE;
    //NotificationFilter.dbcc_classguid = ;
    //NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;
    CopyMemory(&NotificationFilter.dbcc_classguid, &GUID_DEVINTERFACE_USB_DEVICE, sizeof(GUID));

    hDevNotify = RegisterDeviceNotification( this->winId(), &NotificationFilter,DEVICE_NOTIFY_WINDOW_HANDLE);
    if(NULL != hDevNotify)
    {
        //QDebug()<<"Success---------->";
        return true;
    }
    else
    {
        //QDebug() << GetLastError();
        //QDebug()<<"Failed---------->:";
        return false;
    }
#endif
}



void Uconnect::sendEvent(UEvent *evt)
{
    if(evt == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt = uconn_SAPIExecute(evt);
    if(rspEvnt->getCmdId() != UCONN_SAPI_WRAP_GENERIC_RESP)
    {
        qCritical() << "cmd failed";
    }
	else if(rspEvnt->execStatus == UCONN_SAPI_WRAP_FAILURE)
	{
		mbConnectedToServer = false;
	}
    delete evt;
    evt = 0;
    delete rspEvnt;
    rspEvnt = 0;
}

void Uconnect::enableDisableTech(ETechSelect techId, bool enable)
{
    UEvent* evt = NULL;
    if(enable)
    {
        evt = reinterpret_cast<UEvent*> (new UBearerEnabled(techId));

    }
    else
    {
        evt = reinterpret_cast<UEvent*> (new UBearerDisabled(techId));
        if (techId == WIFI_TECH)
        {
            minView->clearWifiNetworksDisplay();
        }
        else if(techId == THREE_G_TECH)
        {
            minView->clearThreeGNetworksDisplay();
        }
    }

    sendEvent(evt);
}

void Uconnect::techConfigModeChanged(ETechSelect techId,EBearerMode configMode)
{
    UBearerModeChanged* evt = new UBearerModeChanged(techId,configMode);
    sendEvent(reinterpret_cast<UEvent*>(evt));
}

void Uconnect::techPriorityChanged(ETechSelect techId,U_UINT32* priority)
{
    UBearerPriorityChanged* evt = new UBearerPriorityChanged(techId,priority);
    sendEvent(reinterpret_cast<UEvent*>(evt));
}

void Uconnect::techNetSearch(ETechSelect techId)
{
    UBearerSearch* evt = new UBearerSearch(techId);
    sendEvent(reinterpret_cast<UEvent*>(evt));
}



void Uconnect::uconnScanResponseHandler(UEvent *evt_p)
{
    //QDebug() << "uconnScanResponseHandler entry ";
    UNMSearchResp* searchResp = reinterpret_cast<UNMSearchResp*> (evt_p);

    if(searchResp->techId == WIFI_TECH)
    {
        NetworkListEncoded networkListEncoded;

        networkListEncoded.networkList = searchResp->networkList;
        networkListEncoded.techId = WIFI_TECH;

        GuiNMScanRspEvt* scanRspEvt = new GuiNMScanRspEvt(networkListEncoded);

        QApplication::postEvent(this,scanRspEvt);

        //QDebug() << "uconnScanResponseHandler exit ";


       // //QDebug() << scanNetworkEvt->networkList;
    }
}

void Uconnect::connectRequestHandler()
{
    UConnectRequest* evt = new UConnectRequest();
    sendEvent(reinterpret_cast<UEvent*>(evt));
}

void Uconnect::connectResponseHandler(UNMConnectStatus* connectResponse)
{
    if(connectResponse->techId == WIFI_TECH)
    {
        detView->handleWifiConnectionStatusResponse(connectResponse->connectStatus);

    }

}

void Uconnect::setNMParam(ETechSelect techId,Set_NM_Param paramId, string param)
{
    UNMParam* evt = new UNMParam(techId,paramId,param);
    sendEvent(reinterpret_cast<UEvent*>(evt));
}


void Uconnect::connectToWifiWithPassword(string password)
{
    UWifiConnectWithPassword* evt = new UWifiConnectWithPassword((char*)password.c_str());
    sendEvent(reinterpret_cast<UEvent*>(evt));
}

void Uconnect::settingForWifiOnDetView()
{
    detView->minviewWifiPressed();
}

void Uconnect::settingForLteOnDetView()
{
    detView->minviewLtePressed();
}

void Uconnect::BearerEnabledHandler(ETechSelect techId,bool enabled)
{
    if(enabled)
    {
        UBearerEnabled* bearerEnabled = new UBearerEnabled(techId);
        sendEvent(reinterpret_cast<UEvent*>(bearerEnabled));
    }
    else
    {
        UBearerDisabled* bearerDisabled = new UBearerDisabled(techId);
        sendEvent(reinterpret_cast<UEvent*>(bearerDisabled));

    }

}

void Uconnect::WifiConnectHandler(QString ssid)
{
    UBearerConnect *reqEvnt = new UBearerConnect(WIFI_TECH);
                if(reqEvnt == 0)
        {
            qCritical() << "Error::Memory Allocation Failure";
        }
        UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);
        if(rspEvnt->getCmdId() != UCONN_SAPI_WRAP_GENERIC_RESP)
        {
            qCritical() << "on_connectButton_clicked()--netDetach failed";
           // stMsg->clear();
            //stMsg->setText(tr("Disconnection Failure"));
        }

        delete reqEvnt;
        reqEvnt = 0;
        delete rspEvnt;
        rspEvnt = 0;


}

void Uconnect::WifiPasswordGot(string password,string ssid)
{
    setBearerParameter(WIFI_TECH,WIFI_PASSWORD,password);
}

void Uconnect::WifiPasswordCancelled()
{
	string emptyPassword = "";
	setBearerParameter(WIFI_TECH,WIFI_PASSWORD,emptyPassword);
}

void Uconnect::WifiScanHandler()
{
    UBearerSearch *reqEvnt = new UBearerSearch(WIFI_TECH);
            if(reqEvnt == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);
    if(rspEvnt->getCmdId() != UCONN_SAPI_WRAP_GENERIC_RESP)
    {
        qCritical() << "on_connectButton_clicked()--netDetach failed";
       // stMsg->clear();
        //stMsg->setText(tr("Disconnection Failure"));
    }

    delete reqEvnt;
    reqEvnt = 0;
    delete rspEvnt;
    rspEvnt = 0;
}

/*void Uconnect::onwificlickedShowTab()
{
    detView->setwifiposition();
    switchView();

}*/


void Uconnect::WifiDisConnectHandler()
{
    UBearerDisconnect *reqEvnt = new UBearerDisconnect(WIFI_TECH);
            if(reqEvnt == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);
    if(rspEvnt->getCmdId() != UCONN_SAPI_WRAP_GENERIC_RESP)
    {
        qCritical() << "on_connectButton_clicked()--netDetach failed";
       // stMsg->clear();
        //stMsg->setText(tr("Disconnection Failure"));
    }

    delete reqEvnt;
    reqEvnt = 0;
    delete rspEvnt;
    rspEvnt = 0;

}

void Uconnect::setBearerParameter(U_UINT32 techId,Set_NM_Param paramId,string param)
{
    UBearerStrParam* reqEvnt = new UBearerStrParam(techId,paramId,param);

    if(reqEvnt == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);
    if(rspEvnt->getCmdId() != UCONN_SAPI_WRAP_GENERIC_RESP)
    {
        qCritical() << "on_connectButton_clicked()--netDetach failed";
       // stMsg->clear();
        //stMsg->setText(tr("Disconnection Failure"));
    }

    delete reqEvnt;
    reqEvnt = 0;
    delete rspEvnt;
    rspEvnt = 0;

}

/*
void Uconnect::techPriorityChangedHandler(U_UINT32 techId,U_UINT32* priorityList)
{
    UBearerPriorityChanged *reqEvnt = new UBearerPriorityChanged(techId,this->priorityList);
    for(int i=0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
    {
        reqEvnt->techPriority[i] = priorityList[i];
    }

            if(reqEvnt == 0)
    {
        qCritical() << "Error::Memory Allocation Failure";
    }
    UEvent *rspEvnt = uconn_SAPIExecute(reqEvnt);
    if(rspEvnt->getCmdId() != UCONN_SAPI_WRAP_GENERIC_RESP)
    {
        qCritical() << "techPriorityChangedHandler failed";
    }

    delete reqEvnt;
    reqEvnt = 0;
    delete rspEvnt;
    rspEvnt = 0;

}
*/

void Uconnect::minConnectButtonHandler(ETechSelect techId,bool connectionStatus)
{
    if(connectionStatus == false)
    {
       UBearerConnect* bearerConnect = new UBearerConnect(techId);
       sendEvent(reinterpret_cast<UEvent*>(bearerConnect));

    }
    else
    {
        UBearerDisconnect* bearerDisconnect = new UBearerDisconnect(techId);
        sendEvent(reinterpret_cast<UEvent*>(bearerDisconnect));

    }

}

void Uconnect::updateConnectionChangeInGui() {

    //QDebug() << "updateConnectionChangeInGui entry ";


    if(connectionChangeParams.techId > WIFI_TECH || connectionChangeParams.techId < LTE_TECH) {
        //QDebug() << " Invalid Tech Id received" << connectionChangeParams.techId;
        return;
    }
    //QDebug() << "special entry minView connectionState";
    

    //QDebug() << "special exit minView connectionState";
    minView->updateConnectionState(connectionChangeParams.techId,connectionChangeParams.connectionStatus);
    detView->updateConnectionState(connectionChangeParams.techId,connectionChangeParams.connectionStatus);
    //QDebug() << "updateConnectionChangeInGui exit ";

}

void Uconnect::handleWifiScanResponse(NetworkListEncoded& wifiNetworkList)
{
    //QDebug() << "Handle WifiScanResponse entry";
    //clear combo box
    if(gTechDetails[WIFI_TECH].state != NM_CONNECTED_STATE)
    {
        minView->clearWifiNetworksDisplay();
    }



    bool scanConnected = false;
    QString networkName;
    QString signalQuality;
    QString connected;
    std::vector<string> vStr;
    detView->tokeniseInformation(wifiNetworkList.networkList,vStr,'\n');

    if (persistNoOfWifiNetworks == 0 && vStr.size()> 0)
    {
        QString statusMsg = "Wireless Networks Avaliable" ;
        minView->showStatusMsg(statusMsg);
        persistNoOfWifiNetworks = vStr.size();
    }
    for(unsigned int i=0;i<vStr.size();i++)
    {

        std::vector<string> networkVector;
        detView->tokeniseInformation(vStr[i],networkVector,';');
        if(networkVector.size() < 2)
        {
            break;
        }
        for(unsigned int j=0;j<networkVector.size();j++)
        {
            std::vector<string> param;

            detView->tokeniseInformation(networkVector[j],param,':');
            if(param.size() < 2)
            {
                continue;
            }
            string name = param[0];
            string value = param[1];
            if(strstr(name.c_str(),"SSID"))
            {
                networkName = value.c_str();

            }
            else if(strstr (name.c_str(),"SigQuality"))
            {
                 signalQuality = value.c_str();


            }
            else if(strstr (name.c_str(),"connected"))
            {
                connected = value.c_str();
                if(strstr (value.c_str(),"Yes"))
                {
                    //detView->ifconnectedSetting(true);
                    scanConnected = true;
                    detView->isWifiConnected = true;
                    detView->makeWifiSelected();
                    //detView->makeTabChecked(WIFI_TECH);
                    calculateSignalStrength(WIFI_TECH,signalQuality.toInt());



                }

            }

        }
        if(!scanConnected)
        {
           //detView->ifconnectedSetting(false);
           detView->isWifiConnected = false;
        }
        else
        {
            if(minView->currentTech == WIFI_TECH) {

                minView->setContextForConnectButton(scanConnected);
            }

        }




        //detView->wifinetworkDisplay(networkName,signalQuality,connected,i);
        minView->wifinetworkDisplay(networkName);
    }

    detView->displayAllWifiNetworks(wifiNetworkList.networkList);
   //QDebug() << "Handle WifiScanResponse exit";

}


void Uconnect::wifiPrefHandler(UEvent *evt)
{
   UWifiPreferredNetworksEvt* preferred = reinterpret_cast<UWifiPreferredNetworksEvt*>(evt);

   string encodedSsids = preferred->preferredSsids;

   GuiWifiPreferredRspEvt* wifiPreferredRspEvt = new GuiWifiPreferredRspEvt(encodedSsids);

   QApplication::postEvent(this,wifiPreferredRspEvt);
}

void Uconnect::showPreferredWifiNetworks(string encodedSsids)
{
    detView->handleWifiPreferredRsp(encodedSsids);
}



void Uconnect::toggleArbitrationMode(bool autoMode)
{
    if(!autoMode)
    {
        UBearerArbitrationOFF* arbitOff = new UBearerArbitrationOFF();
        sendEvent(reinterpret_cast<UEvent*>(arbitOff));
    }
    else
    {
        UBearerArbitrationON* arbitOn = new UBearerArbitrationON();
        sendEvent(reinterpret_cast<UEvent*>(arbitOn));
    }
}


void Uconnect::getInitTechConfiguration()
{

     getPriorityTechList();
     getEnabledTechList();
     getEachTechState();
	getInitialRasProfile();



}

void Uconnect::getEnabledTechList()
{
    UGetEnabledTechList* evt = new UGetEnabledTechList();
    sendEvent(reinterpret_cast<UEvent*>(evt));
}

void Uconnect::getPriorityTechList()
{
    UGetPriorityTechList* evt = new UGetPriorityTechList();
    sendEvent(reinterpret_cast<UEvent*>(evt));
}


void Uconnect::uconnStatsMessageHandler(UEvent *evt_p)
{
    UModemStatisticEvent *ModemRspEvt = (UModemStatisticEvent*)evt_p;
    strcpy(m_CMmodemStats.m_nL1AStats.L1AencodStr,ModemRspEvt->modemStatistic.L1AencodStr);
}

void Uconnect::wifiStatsRspHandler(UEvent *evt_p)
{
    UWifiStatsRsp* statsRsp = reinterpret_cast<UWifiStatsRsp*>(evt_p);
    string encodedStats = statsRsp->wifiStats;
    GuiWifiStatsRspEvt* wifiStats = new GuiWifiStatsRspEvt(encodedStats);
    QApplication::postEvent(this,wifiStats);
}

void Uconnect::handleConnectStatusEvt(UEvent *evt)
{
    UNMConnectStatus* connectStatus = reinterpret_cast<UNMConnectStatus*>(evt);
    string ssid = "";

    switch(connectStatus->techId)
    {
    case WIFI_TECH:
        {
            if(connectStatus->connectStatus == NM_CONNECT_REQUEST_PASSWORD)
            {
                if(strlen(connectStatus->param))
                {
                    ssid = connectStatus->param;

                }
                else
                {
                    ssid = "Unknown Network";
                }
                GuiWifiPasswordReqEvt* wifiPasswordReqEvt = new GuiWifiPasswordReqEvt(ssid);

                QApplication::postEvent(this,wifiPasswordReqEvt);
            }
            else if(connectStatus->connectStatus == NM_CONNECT_FAILURE)
            {
                GuiConnectFail* connectFail = new GuiConnectFail(WIFI_TECH,connectStatus->param);
                QApplication::postEvent(this,connectFail);
            }


        }
        break;
    }

}

void Uconnect::showWifiStats(string encodeStats)
{
    minView->showWifiStats(encodeStats);

}

void Uconnect::handleWifiPasswordReq(string ssid)
{
    detView->reqPasswordPop(QString::fromStdString(ssid));
}

void Uconnect::threeGStatsHandler(UEvent *evt)
{
    UNMThreeGStats* threeGStats = reinterpret_cast<UNMThreeGStats*>(evt);
    string stats = threeGStats->threeGStats;
    Gui3gStatsEvt* guiThreeGStats = new Gui3gStatsEvt(stats);

    QApplication::postEvent(this,guiThreeGStats);

}

void Uconnect::handleShowNMSettingsEvt(UEvent* evt)
{
    UNMShowSettings* nmSettings = reinterpret_cast<UNMShowSettings*>(evt);
    this->nmSettings.techId = (ETechSelect)nmSettings->techId;
    this->nmSettings.show = nmSettings->showSettings;

    QApplication::postEvent(this, new QEvent(guiShowNMSettingsEvent));

}

void Uconnect::handle3GStatsRsp(string stats)
{
    detView->show3gStats(stats);
    minView->show3gStats(stats);

}

void Uconnect::settingForThreeGOnDetView()
{
    detView->minviewThreeGPressed();
}

void Uconnect::updateNMSettings()
{
   // detView->updateDetNMSettings(this->nmSettings.techId,this->nmSettings.show);
   // minView->updateMinNMSettings(this->nmSettings.techId,this->nmSettings.show);

}

void Uconnect::getEnabledTechsWithPriority()
{
    //always request priority list before enable list

    UGetPriorityTechList* techList = new UGetPriorityTechList();
    sendEvent(reinterpret_cast<UEvent*>(techList));

    UGetEnabledTechList* enabledList = new UGetEnabledTechList();
    sendEvent(reinterpret_cast<UEvent*>(enabledList));

    getEachTechState();

	getTechParams(ALL_TECH,BEARER_MNGR_MODE);

    getInitialRasProfile();


}

void Uconnect::updateEnabledTechList(U_UINT32* enabledTechs)
{
    //QDebug() << " updateEnabledTechList --> entry";

    for(int i=0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
    {
        if(enabledTechs[i] == 1)
        {
            gTechDetails[i].enabled = true;
        }

    }


    /*
    vector<string> enabledVector;
    detView->tokeniseInformation(enabledListStr,enabledVector,';');

    if(enabledVector.size() > MAX_NUMBER_OF_NETWORKMANAGERS + 1)
    {
        //QDebug() << "ERROR: enabled list exceeds max number of network managers";
        return;
    }

    for(int i=0;i<enabledVector.size();i++)
    {
        if(!enabledVector[i].size())
        {
            continue;
        }

        U_UINT32 techId = atoi(enabledVector[i].c_str());
        if(techId < MAX_NUMBER_OF_NETWORKMANAGERS)
        {
            gTechDetails[i].enabled = true;

        }
        else
        {
            //QDebug() << "ERROR: techId exceeds max number of network managers";

        }


    }
    //QDebug() << " updateEnabledTechList --> exit";
    */

}

void Uconnect::updateTechPriority(UEvent *evt)
{
    UGetPriorityTechListRsp* techList = reinterpret_cast<UGetPriorityTechListRsp*>(evt);
    memcpy((void*)(gPriorityList),(const void*)(techList->priorityList),sizeof(techList->priorityList));
    //QDebug() << "before savePriority";
    savePriorityToTechDetails();
    //QDebug() << "after savePriority";
}

void Uconnect::handleEnabledTechListEvt(UEvent *evt)
{
    UGetEnabledTechListRsp* enabledList = reinterpret_cast<UGetEnabledTechListRsp*>(evt);

    updateEnabledTechList(enabledList->enabledTechs);

    QApplication::postEvent(this, new QEvent(guiAddInitialTechsEvent));

}

void Uconnect::handleNMErrorDisplay(UEvent* evt)
{
    UNMErrorDisplayEvt* errorEvt = reinterpret_cast<UNMErrorDisplayEvt*>(evt);
    NMErrorInfo errorInfo;
    errorInfo.errorStr = errorEvt->NM_ErrorDisplay;
    errorInfo.errorType = errorEvt->ErrorType;
    errorInfo.techId = (ETechSelect)errorEvt->techId;
	errorInfo.errorCode = errorEvt->errorCode;

    GuiErrorDisplayEvt* errorMsgEvt = new GuiErrorDisplayEvt(errorInfo);
    QApplication::postEvent(this,errorMsgEvt);
}

void Uconnect::handleNMStateChangeEvt(UEvent* evt)
{
    UNMStateEvent* stateEvent = reinterpret_cast<UNMStateEvent*>(evt);
	gTechDetails[stateEvent->techId].oldState = gTechDetails[stateEvent->techId].state;
    gTechDetails[stateEvent->techId].state = stateEvent->currentState;


    NMStateChange stateChange;
    stateChange.techId  = (ETechSelect)stateEvent->techId;
    stateChange.state = stateEvent->currentState;

    GuiNMStateChangeEvt* stateChangeEvt = new GuiNMStateChangeEvt(stateChange);



    QApplication::postEvent(this,stateChangeEvt);




}

void Uconnect::showInitialTechsInGui()
{
    detView->addTechsToPriorityList();

}

void Uconnect::setConnectButtonForAutoMode(bool connectButtonState)
{
    minView->setConnectButtonMinviewForAutoMode(connectButtonState);
}

void Uconnect::savePriorityToTechDetails()
{
    int j=0;
    for(int i=0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
    {
        gTechDetails[gPriorityList[i]].priority = j;
        j++;
    }

}

void Uconnect::getEachTechState()
{
    for(int i=0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
    {
        UBearerGetNMState* getNMState = new UBearerGetNMState(gTechDetails[i].techId);
        sendEvent(reinterpret_cast<UEvent*>(getNMState));
    }

}

void Uconnect::updateGuiOnNMStateChange(NMStateChange stateChange)
{
    minView->updateOnNMStateChange(stateChange.techId,stateChange.state);
    detView->updateOnNMStateChange(stateChange.techId,stateChange.state);
    getTechParamsBasedOnState(stateChange.techId);

}



void Uconnect::getTechParams(ETechSelect techId,Get_NM_Param paramId)
{
    UBearerGetParam* getParam = new UBearerGetParam(techId,paramId);
    sendEvent(reinterpret_cast<UEvent*>(getParam));
}

void Uconnect::getTechParamsBasedOnState(ETechSelect techId)
{
    switch(techId)
    {
    case WIFI_TECH:
        {
            if(gTechDetails[WIFI_TECH].state != NM_DISABLED_STATE)
            { 
                getTechParams(WIFI_TECH,ADAPTER_NAME);
				getTechParams(WIFI_TECH,VERSION);
                searchTechNetworks(WIFI_TECH);

            }
            if(gTechDetails[WIFI_TECH].state == NM_CONNECTED_STATE)
            {
                getTechParams(WIFI_TECH,NETWORK_NAME);
                getTechParams(WIFI_TECH,SIGNAL_QUALITY);
				getTechStats(WIFI_TECH);
            }
        }
        break;
    case THREE_G_TECH:
        {
            if(gTechDetails[techId].state == NM_CONNECTED_STATE)
            {
				getTechParams(THREE_G_TECH,NETWORK_NAME);
				getTechParams(THREE_G_TECH,NETWORK_ACCESS_TYPE);
            }
			if(gTechDetails[techId].state != NM_DISABLED_STATE)
			{
				getTechParams(THREE_G_TECH,VERSION);
			}

        }
        break;
    case LTE_TECH:
        {
			if(gTechDetails[techId].state != NM_DISABLED_STATE)
			{
				getTechParams(LTE_TECH,VERSION);
			}
			if(gTechDetails[techId].state == NM_CONNECTED_STATE)
            {
               getTechParams(LTE_TECH,NETWORK_NAME);
               getTechParams(LTE_TECH,IP_ADDRESS);

            }

       }
       break;
       default:
            break;
    }
}

void Uconnect::handleGetParamRspHandler(UEvent* evt)
{


   //return;
    UBearerGetParamRsp* paramRsp = reinterpret_cast<UBearerGetParamRsp*>(evt);
    NMGetParam getParam;
    getParam.techId = (ETechSelect)paramRsp->techId;
    getParam.param = paramRsp->param;
    getParam.paramId = paramRsp->paramId;

//	logfile << "Posting Param :" << paramRsp->paramId << " Param : " << paramRsp->param << "\n";
//	logfile.flush();
//	if(strstr(paramRsp->param,"Redu")){
//		string temp = paramRsp->param;

//	}
    GuiNMGetParamEvt* getParamEvt = new GuiNMGetParamEvt(getParam);

    QApplication::postEvent(this, getParamEvt);



}

void Uconnect::updateParamGotInGui(NMGetParam nmGetParam)
{

	switch(nmGetParam.techId)
    {
		case ALL_TECH:
        {
            switch(nmGetParam.paramId)
            {
                case BEARER_MNGR_MODE:
                {
                    handleBearerMngrModeChange(nmGetParam.param);
                }
                break;
                case UCM_VERSION:
                {
                    this->uCMVersion = nmGetParam.param.c_str();
                    minView->showVersionOnMinview(uCMVersion);
                }
                break;
            }
        }
        break;
        case WIFI_TECH:
        {
            switch(nmGetParam.paramId)
            {
                case NETWORK_NAME:
                    minView->showWifiNetworkName(nmGetParam.param);
                    break;
                case ADAPTER_NAME:
                    detView->showWifiAdapterName(nmGetParam.param);
                    break;
                case SIGNAL_QUALITY:
                    calculateSignalStrength(WIFI_TECH,QString::fromStdString(nmGetParam.param).toInt());
                    break;
                case VERSION:
                    {
						if(nmGetParam.param.size() > 0)
						{
							if(verWifiUpdateState == false)
							{
								wifiVersion.clear();
                                wifiVersion.append("<P><b><i><FONT COLOR='#800000' FONT SIZE = 4>");
								wifiVersion += gTechDetails[WIFI_TECH].name.c_str();
                                wifiVersion.append("</i></b></P></br>");
                                wifiVersion.append("<P>");
								wifiVersion += nmGetParam.param.c_str();
                                wifiVersion.append("</P></br>");
								updateVersion(nmGetParam.techId,nmGetParam.param);
								verWifiUpdateState = true;
							}
						}
                    }
                    break;
				default:
					break;
            }
         }
        break;
    case THREE_G_TECH:
        {
            switch(nmGetParam.paramId)
            {
            case NETWORK_NAME:
                {
                    minView->updateNetworkName(nmGetParam.param);
                }
                break;
			case NETWORK_ACCESS_TYPE:
				{
					minView->accessTypeName = nmGetParam.param;
					minView->updateAccessType(nmGetParam.param);
				}
				break;
            case VERSION:
                {
					if(nmGetParam.param.size() > 0)
					{
						if(verThreeGUpdateState == false)
							{
								threeGVersion.clear();
                                threeGVersion.append("<P><b><i><FONT COLOR='#800000' FONT SIZE = 4>");
								threeGVersion += gTechDetails[THREE_G_TECH].name.c_str();
                                threeGVersion += "</i></b></P></br>";
								std::vector<string> vStr;
								string versionParam = nmGetParam.param.c_str();
								string versionString = "";
								detView->tokeniseInformation(versionParam,vStr,'\n');
								for(unsigned int k =0; k< vStr.size();k++)
								{
									if(vStr[k].size()>2)
									{
										versionString += vStr[k].append("<br />");
									}
								}

                                threeGVersion.append("<P>");
                                threeGVersion += versionString.c_str();
                                threeGVersion += ("</P></br>");
        						updateVersion(nmGetParam.techId,nmGetParam.param);
								verThreeGUpdateState = true;
							}
					}
                }
                break;
            }

        }
		break;
    case LTE_TECH:
		{
            switch(nmGetParam.paramId)
            {
				case RSSI:
					{
						
					//int rssi = abs(atoi(nmGetParam.param.c_str()));
						detView->updateRssiProgressBar(nmGetParam.param);
		                
					}
				break;
				case CINR:
					{
						detView->updateCinrProgressBar(nmGetParam.param);
					}
				break;
				case NETWORK_NAME:
					{
						detView->gen.Apn = nmGetParam.param;
						minView->showLTENetworkName(nmGetParam.param);

					}
				break;
				case IP_ADDRESS:
					{
						detView->gen.ipAddress = nmGetParam.param;
					}
				break;
				case VERSION:
					{
						if(nmGetParam.param.size() > 0)
						{
							if(verLteUpdateState == false)
								{
                                    lteVersion.clear();
                                    lteVersion.append("<P><b><i>< FONT COLOR='#800000' FONT SIZE = 4 >");
									lteVersion += gTechDetails[LTE_TECH].name.c_str();
                                    lteVersion.append("</i></b></P></br>");
                                   	std::vector<string> vStr;
									string versionParam = nmGetParam.param.c_str();
									string versionString = "";
									detView->tokeniseInformation(versionParam,vStr,'\n');
									for(unsigned int k =0; k< vStr.size();k++)
									{
										if(vStr[k].size()>2)
										{
											versionString += vStr[k].append("<br />");
										}
									}

                                    //lteVersion += ":\n";
                                    lteVersion.append("<P>");
                                    lteVersion += versionString.c_str();
                                    //lteVersion += "\n";
                                    lteVersion.append("</P></br>");
									updateVersion(nmGetParam.techId,nmGetParam.param);
									verLteUpdateState = true;
								}
						}
					}
				break;
				default:
					break;
				}
        }
		break;
		default:
		break;

    }



}

void Uconnect::sendLteStatsRequest()
{
	//return;
    UModemStatisticRequest* modemStats = new UModemStatisticRequest();
    sendEvent(reinterpret_cast<UEvent*>(modemStats));

    UMeasStatisticRequest* measStats = new UMeasStatisticRequest();
    sendEvent(reinterpret_cast<UEvent*>(measStats));


}


void Uconnect::uconnMeasStatsHandler(UEvent* evt)
{
    UMEASStatisticEvent *ModemRspEvt = (UMEASStatisticEvent*)(evt);
    m_CMmodemStats.m_nMeasStats = ModemRspEvt->measStat;
}

void Uconnect::searchTechNetworks(ETechSelect techId)
{
    UBearerSearch* bearerSearch = new UBearerSearch(techId);
    sendEvent(reinterpret_cast<UEvent*>(bearerSearch));
}

void Uconnect::getTechStats(ETechSelect techId)
{
	UBearerStatistics* stats = new UBearerStatistics(techId);
	sendEvent(reinterpret_cast<UEvent*>(stats));
}

void Uconnect::displayConnectFailureInGui(ETechSelect techId,string ssid)
{
    //detView->displayConnectFailedDet(techId,ssid);
    minView->displayConnectFailedMin(techId,ssid);

}

void Uconnect::handleWifiDisabledNotification()
{
    GuiWifiDisabledEvt* disabled = new GuiWifiDisabledEvt();
    QApplication::postEvent(this,disabled);
}



void Uconnect::showWifiDisabledNotification()
{
    detView->handleWifiDisabledNotification();
    minView->handleWifiDisabledNotification();
}
void Uconnect::showWifiEnabledNotification()
{
	minView->handleWifiEnabledNotification();
}

void Uconnect::updateVersion(ETechSelect techId,string version)
{

	if(techId == LTE_TECH)
	{
		disp.append(lteVersion);
	}
    else if(techId == WIFI_TECH)
    {
		disp.append(wifiVersion);
	}
    else if(techId == THREE_G_TECH)
    {
        disp.append(threeGVersion);
	}


}


void Uconnect:: initialVersionSetting()
{
	disp.clear();
    disp =  " <P><b><i><FONT COLOR='#800000' FONT SIZE = 4 FONT STYLE = bold  > VERSION INFORMATION </FONT></i></b></P></br>";
    QString tmpStr = "<P>Uconnect Version: ";
    tmpStr.append(uCMVersion);
    tmpStr.append("</P></br>");
    disp.append(tmpStr);
}
 void Uconnect::callSetContextOnMinConnectButton(bool mode)
 {
    minView->setContextForConnectButton(mode);
 }


 void Uconnect::handleBearerMngrModeChange(std::string param)
 {
	 if(!strcmp("auto",param.c_str()))
	 {
		 this->autoMode = true;
	 }
	 else
	 {
		 this->autoMode = false;
	 }
	 minView->setContextForConnectButton(this->autoMode);
	 detView->setContextForAutoManualButton();
 }

 void Uconnect::setSettingsOnMinview(bool view,ETechSelect techId)
 {
	minView->currentTech = ALL_TECH;
     switch(techId)
     {
        case WIFI_TECH:
        {
             if(view == false)
             {
				  minView->updateWifiMinviewLabels(true);
             }
             else
             {
				 detView->wifiPushButtonPressed();
             }
        }
        break;
        case LTE_TECH:
        {
             if(view == false)
             {
				  minView->updateLTEMinviewLabels(true);
             }
             else
             {
				   detView->ltePushButtonPressed();
             }
        }
        break;
        case THREE_G_TECH:
        {
              if(view == false)
              {
				 minView->updateThreeGMinviewLabels(true);
              }
              else
              {
                 detView->threeGPushButtonPressed();
              }
        }
         break;
         default:
             break;
 }
}
 void Uconnect::setSettingsOnMinviewForGeneral()
 {
 
	 for(int i =0;i <MAX_NUMBER_OF_NETWORKMANAGERS;i++)
	 {
		 if(gTechDetails[i].state == NM_CONNECTED_STATE)
		 {
			 minView->currentTech = ALL_TECH;
			 switch(i)
			 {
                 case LTE_TECH:
                 {
					minView->updateLTEMinviewLabels(true);
                 }
                 break;
                 case WIFI_TECH:
                 {
					minView->updateWifiMinviewLabels(true);
                 }
                 break;
                 case THREE_G_TECH:
                 {
					 minView->updateThreeGMinviewLabels(true);
                 }
                 break;
                 default:
                     break;
             }

			
		 }
	 }
 
 }

 void Uconnect::setRasProfile(Ras_Details_s* details)
 {
	 this->rasProfDetails = *details;
    UNM3GSetProfile* setNMProfile = new UNM3GSetProfile(details);
     sendEvent(reinterpret_cast<UEvent*>(setNMProfile));
 }

 void Uconnect::getInitialRasProfile()
 {
     UNM3GGetProfile* getRasProfile = new UNM3GGetProfile();
     sendEvent(reinterpret_cast<UEvent*>(getRasProfile));
 }

 void Uconnect::showInitialRasSettings(Ras_Details_s &rasDetails)
 {
     GuiRasProfileEvt* rasEvt = new GuiRasProfileEvt(rasDetails);
     QApplication::postEvent(this,rasEvt);

 }

 void Uconnect::handleInitialRasProf(Ras_Details_s details)
 {
     this->rasProfDetails = details;
	 detView->displayRasProfile();
 }

 void Uconnect::displayErrorMessage(NMErrorInfo errorInfo)
 {
	 if(errorInfo.errorType == NM_INFORMATIVE)
	 {
		 if(errorInfo.errorCode != previousInfoCode)
		 {
			minView->displayErrorInfoMsgs(errorInfo.errorStr,errorInfo.errorType,errorInfo.techId);
			previousInfoCode = errorInfo.errorCode;
		 }
	 }
	else if(errorInfo.errorType == NM_REPORT_ERROR)
	{
		 if(errorInfo.errorCode != previousErrorCode)
		 {
			 minView->displayErrorInfoMsgs(errorInfo.errorStr,errorInfo.errorType,errorInfo.techId);
			 previousErrorCode = errorInfo.errorCode;
		 }
	 }
 }

/* This function is called to get the vpn profiles from vpn*/
void Uconnect::getVpnProfList()
{
    UVPNListProfiles* proList = new UVPNListProfiles();
    sendEvent(reinterpret_cast<UEvent*>(proList));
}

 /* This function is called to get the vpn clients from vpn*/
void Uconnect::getVpnClientList()
{
    UVPNListClients* cliList = new UVPNListClients();
    sendEvent(reinterpret_cast<UEvent*>(cliList));
}

/* This function sends vpn connect to vpn through server*/
void Uconnect::vpnConnect(QString profToCnt)
{
    string profName = profToCnt.toStdString();
    UVPNStart* vpnCnt = new UVPNStart(profName);
    sendEvent(reinterpret_cast<UEvent*>(vpnCnt));
}

/* This function send vpndisconnect to vpn through server*/
void Uconnect::vpnDisconnect()
{
    UVPNStop* vpnDiscnt = new UVPNStop();
    sendEvent(reinterpret_cast<UEvent*>(vpnDiscnt));
}

/* This function is called to get the vpn profiles evt from vpn and posts the cust event to gui thread*/
void Uconnect::handleVpnProfList(UEvent* evt)
{
	 UVPNListProfilesRsp* profRsp = reinterpret_cast<UVPNListProfilesRsp*>(evt);
	 GuiVpnProfileLstEvt* profileLstEvt = new GuiVpnProfileLstEvt(string(profRsp->profileList));
     QApplication::postEvent(this,profileLstEvt);
}

/*Handle function when gui recevies the client list evt from server and reposts the cust event to guithread*/
void Uconnect::handleVpnCliList(UEvent* evt)
{
	UVPNListClientsRsp* cliRsp = reinterpret_cast<UVPNListClientsRsp*>(evt);
	GuiVpnClientLstEvt* clientLstEvt = new GuiVpnClientLstEvt(string(cliRsp->clientList));
    QApplication::postEvent(this,clientLstEvt);
}

/*Handle function when gui recevies the vpnconnect response evt from server and reposts the cust event to guithread*/
void Uconnect::handleVpnConnect(UEvent* evt)
{
    UVPNStartRsp* connectStatus = reinterpret_cast<UVPNStartRsp*>(evt);
    GuiVpnConnectEvt* vpnConnectEvt = new GuiVpnConnectEvt(connectStatus->startStatus);
    QApplication::postEvent(this,vpnConnectEvt);
}

void Uconnect::handleVpnDisconnect()
{
    GuiVpnDisconnectEvt* vpnDisconnectEvt = new GuiVpnDisconnectEvt();
    QApplication::postEvent(this,vpnDisconnectEvt);
}

void Uconnect::callMsgBarMinview()
{
    minView->updateMessageBar();
}