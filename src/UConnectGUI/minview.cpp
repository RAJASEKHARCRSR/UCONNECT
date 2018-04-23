#include "ui_minview.h"
#include "minview.h"

#include "uconnect.h"

extern TechDetails gTechDetails[];

MinimumView::MinimumView(QMainWindow *parent) :
        QMainWindow(parent),ui(new Ui::MinimumView),pUConnect(NULL)
{
    ui->setupUi(this);

}

void MinimumView::init(void *parent)
{

	accessTypeName = "";
    settingInfoState = false;
    clearWifiCombo = false;
    sigLabel1 = false;
    sigLabel2 = false;
    sigLabel3 = false;
    pUConnect = parent;
    createActions();
    createTrayIcon();
    setIcon();
    trayIcon->show();

    setWindowTitle(tr("Connection Manager"));
    setWindowIcon(QIcon(":/icon"));
     ui->network_name->clear();
    ui->onselectWiredPushButton->setDisabled(true);

    ui->lte4gInfoDown->setVisible(false);
    ui->lte4gInfoUp->setVisible(false);
    ui->wifiInfoUp->setVisible(false);
    ui->wifiInfoDown->setVisible(false);


    ui->tx_rate->clear();
    ui->wifiNetworkName->clear();
    ui->rx_rate->clear();


    ui->upload_rate->clear();
    ui->download_rate->clear();

    ui->threeGInfoDown->setVisible(false);
    ui->threeGInfoUp->setVisible(false);


    ui->threeGDownload_rate->clear();
    ui->threeGUpload_rate->clear();


    //hide connect button initially because no tech is selected yet;
    ui->minconnectButton->setVisible(false);
    currentTech = INVALID_TECH;

    QLayout *layout = this->layout();
    layout->addWidget(((Uconnect *)pUConnect)->detView);
    QRect coord;
    coord.setY(height());
    ((Uconnect *)pUConnect)->detView->setGeometry(coord);
    this->setLayout(layout);
    timeLine = NULL;
    frameAnimationTimeLineLte = NULL;
    frameAnimationTimeLineWifi = NULL;
    frameAnimationTimeLine3G = NULL;
    ui->lteStatsFrame->resize(ui->lteStatsFrame->width(),0);
    ui->threegStatsframe->resize(ui->threegStatsframe->width(),0);
    ui->wifiStatsframe->resize(ui->wifiStatsframe->width(),0);


    /*signal strength label initialization */

    sigStrength[0].imageNumb = 0;
    sigStrength[0].imageString = "image: url(:/ltesignal%1);";
    sigStrength[0].label = ui->sigStrengthLte;
    sigStrength[1].imageNumb = 0;
    sigStrength[1].imageString = "image: url(:/3gsignal%1);";
    sigStrength[1].label = ui->sigStrength1;
    sigStrength[2].imageNumb = 0;
    sigStrength[2].imageString = "image: url(:/wifisignal%1);";
    sigStrength[2].label = ui->sigStrength2;

    //ui->minconnectButton->setToolTip("<i>Shows connected when in auto mode and connect when in manual mode</i>");
    ui->versionPushButton->setToolTip("<i>Gives info about Version Manufacturer Model</i>");
    ui->help_pushButton->setToolTip("<i>Help</i>");
    ui->detail_pushButton->setToolTip("<i>Settings</i>");

    ui->lte4gInfoLabel->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0, 0, 0, 100), stop:1 rgba(36, 36, 36, 100));");



}

MinimumView::~MinimumView()
{
    trayIcon->hide();
    if(trayIcon)
    delete trayIcon;
    if(open)
    delete open;
    if(close)
    delete close;
    delete ui;
}

/***********************************************************************
*  This fuction is called for creating System Tray
***********************************************************************/

void MinimumView::createActions()
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
void MinimumView::appQuit()
{
    trayIcon->setVisible(false);
    qApp->quit();
}


void MinimumView::createTrayIcon()
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
void MinimumView::setIcon()
{
    //QDebug() << "setIcon()--Start";
    trayIcon->setIcon(QIcon(":/icon"));
    //QDebug() << "setIcon()--End";
}

void MinimumView::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    //QDebug() << "trayIconClicked()--Start";
    if(reason == QSystemTrayIcon::Trigger)
    this->show();
    //QDebug() << "trayIconClicked()--End";
}
void MinimumView::closeEvent(QCloseEvent *event)
{
    //QDebug() << "closeEvent()--Start";
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
        }
    //QDebug() << "closeEvent()--End";
}


void MinimumView::on_minconnectButton_clicked()
{
    //QDebug() << "on_connectButton_clicked()--Start";

    bool uconnectState = gTechDetails[currentTech].state == NM_CONNECTED_STATE;
    if(uconnectState == false)
    {
        ((Uconnect*)pUConnect)->minConnectButtonHandler(currentTech,false);
    }
    else
    {
        ((Uconnect*)pUConnect)->minConnectButtonHandler(currentTech,true);
        if(gTechDetails[LTE_TECH].state == NM_CONNECTED_STATE)
        {
            ui->lteStatsFrame->hide();
            ui->lte4gInfoDown->hide();
        }else if(gTechDetails[WIFI_TECH].state == NM_CONNECTED_STATE)
        {
            ui->wifiStatsframe->hide();
            ui->wifiInfoDown->hide();
        }else if(gTechDetails[THREE_G_TECH].state == NM_CONNECTED_STATE)
        {
            ui->threegStatsframe->hide();
            ui->threeGInfoDown->hide();
        }
    }

    //QDebug() << "on_connectButton_clicked()--End";
}

void MinimumView::on_help_pushButton_clicked()
{
    QString PDFpath=QDir::currentPath()+"/Release_Notes_CM_LTE.chm";
    QDesktopServices::openUrl(QUrl::fromLocalFile(PDFpath));

}

void MinimumView::on_detail_pushButton_clicked()
{
   //((Uconnect*)pUConnect)->switchView();

    if (settingInfoState == false)
    {
        if(timeLine == NULL){
            timeLine = new QTimeLine(333,this);
        }
       
		if(timeLine->state()==QTimeLine::NotRunning){
        settingInfoState = true;
         //int x = height();
        //int y = ((Uconnect *)pUConnect)->detView->height();
        connect(timeLine,SIGNAL(frameChanged(int)),this,SLOT(animatedDetResize(int)));
        timeLine->setFrameRange(height(),height()+ ((Uconnect *)pUConnect)->detView->height());
        timeLine->start();
		}

    }
    else
    {
        if(timeLine == NULL){
            timeLine = new QTimeLine(333,this);
        }
		if(timeLine->state()==QTimeLine::NotRunning ){
       
        settingInfoState = false;

		 connect(timeLine,SIGNAL(frameChanged(int)),this,SLOT(animatedDetResize(int)));
        timeLine->setFrameRange(height(),height() -((Uconnect *)pUConnect)->detView->height());
        timeLine->start();
		}


    }


}

/*void MinimumView::on_info_pushButton_clicked()
{
      QMessageBox a;
      a.about(this,
      tr("About Version Information"),
      ((Uconnect *)pUConnect)->disp);


}*/

void MinimumView::sigIconupdateMinviewSetting(int imageNumber,ETechSelect techId)
{

		sigStrength[techId].imageNumb = imageNumber;

        for(int i=0; i<MAX_NUMBER_OF_NETWORKMANAGERS ; i++)
        {
			sigStrength[i].label->clear();
            sigStrength[i].label->hide();
        }
		int j=0;
        for(int i=0; i<MAX_NUMBER_OF_NETWORKMANAGERS ; i++)
        {
			
            if(gTechDetails[i].state == NM_CONNECTED_STATE)
            {
                QLabel* label;
				if(j==0)
				{
                    label = ui->sigStrengthLte;
				}
                else if(j==1)
                {
                    label = ui->sigStrength1;
                }
                else if(j==2)
                {
                    label = ui->sigStrength2;
                }
                sigStrength[i].label = label;
                sigStrength[i].label->setStyleSheet(sigStrength[i].imageString.arg(sigStrength[i].imageNumb));
                sigStrength[i].label->show();
				j++;

            }

        }

}

void MinimumView::mindownlinkSetting(float downlinkValue)
{
    QString genStr;
    float downlink_val = floor((downlinkValue*100.0)+0.5)/100.0;
    genStr = QString::number( downlink_val );
//ui->download_rate->setStyleSheet("color:#065a4;");
    genStr.append(tr(" Mbps"));

    ui->download_rate->setText(genStr);
    ui->download_rate->show();
    //downLink->show();
}

void MinimumView::minuplinkSetting(float uplink_val)
{
     QString genStr;
     uplink_val = floor((uplink_val*100.0)+0.5)/100.0;
     genStr = QString::number( uplink_val);
     genStr.append(tr(" Mbps"));
     ui->upload_rate->setText(genStr);
     ui->upload_rate->show();

}

void MinimumView::setApnName(QString Apnname,QString Duration)
{
    ui->network_name->clear();
    ui->network_name->setText(Apnname);
    ui->apnComboBox->addItem(Apnname);
    ui->connectionTime->setText(Duration);


}

void  MinimumView::minUI_deviceStateUpdate(int deviceState)
{
    switch(deviceState)
    {
    case NOTI_DEVICE_ERROR:

       // ui->deviceStatus->setText(tr("DEVICE STATUS::Device Error"));
        ui->network_name->clear();


        break;
    case NOTI_DEVICE_NOT_OPEN:
       // ui->deviceStatus->setText(tr("DEVICE STATUS::Device Not Open"));
        //ui->network_name->clear();


        break;
    case NOTI_DEVICE_OPEN:
        //ui->deviceStatus->setText(tr("DEVICE STATUS::Device Open"));
       // ui->network_name->clear();

        break;
    case NOTI_DISCONNECTED:
        //ui->deviceStatus->setText(tr("DEVICE STATUS::Disconnected"));
        ui->network_name->clear();

        timeLabel.clear();

        break;
    case NOTI_DEVICE_CONNECTED:
       // ui->deviceStatus->setText(tr("DEVICE STATUS::Device Connected"));

        //stMsg->clear();
       // stMsg->setText(tr("Active Connection"));
        break;
    case NOTI_DEVICE_DISCONNECTED:
        //ui->deviceStatus->setText(tr("DEVICE STATUS::Device Disconnected"));
        ui->network_name->clear();

        break;
    default:
        //ui->deviceStatus->setText(tr("DEVICE STATUS::Unknown"));
        //ui->network_name->clear();

        qCritical() << "deviceState:" << deviceState;
        break;
    }
}

void MinimumView::detStateUpdate()
{
//QDebug() << "StateUpdate()--Start";
//ui->serverState->clear();


switch(((Uconnect *)pUConnect)->mServerFSMState)
    {
        case IDLE_STATE:
        case ENABLED_STATE:
            {
                if(((Uconnect*)pUConnect)->autoMode == true)
                {
					bool state = checkIfAnyTechConnected();
					if (state == true)
					{
						ui->minconnectButton->setStyleSheet("image: url(:/autoconnected);");
						ui->minconnectButton->setDisabled(true);
					}
					else
					{
						ui->minconnectButton->setVisible(false);
					}
                }
                else
                {
                    ui->minconnectButton->setStyleSheet("image: url(:/connect);");
                    ui->minconnectButton->setDisabled(false);

                    ((Uconnect *) pUConnect)->mCMInfo.ipAddress[0] = '\0';
                }


            }
                break;
        case SEARCHING_STATE:
                {
                //QDebug() << "StateUpdate()--SEARCHING_STATE";
                    if(((Uconnect*)pUConnect)->autoMode == true)
                    {

						bool state = checkIfAnyTechConnected();
						if (state == true)
						{
							ui->minconnectButton->setStyleSheet("image: url(:/autoconnected);");
							ui->minconnectButton->setDisabled(true);
						}
						else
						{
							ui->minconnectButton->setVisible(false);
						}
					}
                    else
                    {
                        ui->minconnectButton->setDisabled(false);
                    }
                }
                break;
        case CONNECTING_STATE:
        case DEFINE_PRIMARY_STATE:
        case DEFINE_SECONDARY_STATE:
        case NET_ATTACH_STATE:
                {
                    //QDebug() << "StateUpdate()--CONNECTING_STATE||DEFINE_PRIMARY_STATE||DEFINE_SECONDARY_STATE||NET_ATTACH_STATE||";
                    if(((Uconnect*)pUConnect)->autoMode == true)
                    {
                    
						bool state = checkIfAnyTechConnected();
						if (state == true)
						{
							ui->minconnectButton->setStyleSheet("image: url(:/autoconnected);");
							ui->minconnectButton->setDisabled(true);
						}
						else
						{
							ui->minconnectButton->setVisible(false);
						}
					}else
                    {
                        ui->minconnectButton->setStyleSheet("image: url(:/connect_dis);");
                        ui->minconnectButton->setDisabled(false);
                    }
                }
                break;
        case CONNECTED_STATE:
                {
                //QDebug() << "StateUpdate()--CONNECTED_STATE";
                //ui->minconnectButton->setText(tr("Disconnect"));

                    if(((Uconnect*)pUConnect)->autoMode == true)
                    {
                        
						bool state = checkIfAnyTechConnected();
						if (state == true)
						{
							ui->minconnectButton->setStyleSheet("image: url(:/autoconnected);");
							ui->minconnectButton->setDisabled(true);
						}
						else
						{
							ui->minconnectButton->setVisible(false);
						}
                    }
                    else
                    {
                        ui->minconnectButton->setStyleSheet("image: url(:/disconnect);");
                        ui->minconnectButton->setDisabled(false);
                    }

                }
                break;
        case DISCONNECTING_STATE:
                {
                //QDebug() << "StateUpdate()--DISCONNECTING_STATE";

                    if(((Uconnect*)pUConnect)->autoMode == true)
                    {
                        
						bool state = checkIfAnyTechConnected();
						if (state == true)
						{
							ui->minconnectButton->setStyleSheet("image: url(:/autoconnected);");
							ui->minconnectButton->setDisabled(true);
						}
						else
						{
							ui->minconnectButton->setVisible(false);
						}
                    }
                    else
                    {

                        ui->minconnectButton->setStyleSheet("image: url(:/connect_dis);");
                        ui->minconnectButton->setDisabled(false);
                    }
                }
                break;
                case DISABLED_STATE:
                {
                    //QDebug() << "StateUpdate()--DISABLED_STATE";
                    if(((Uconnect*)pUConnect)->autoMode == true)
                    {
                        
						bool state = checkIfAnyTechConnected();
						if (state == true)
						{
							ui->minconnectButton->setStyleSheet("image: url(:/autoconnected);");
							ui->minconnectButton->setDisabled(true);
						}
						else
						{
							ui->minconnectButton->setVisible(false);
						}
                    }
                    else
                    {
                        ui->minconnectButton->setStyleSheet("image: url(:/connect_dis);");
                        ui->minconnectButton->setDisabled(false);
                        ((Uconnect *) pUConnect)->mCMInfo.ipAddress[0] = '\0';
                    }

                }
                break;

       default:
                //statdata = "";
                break;
    }
    //QDebug() << "StateUpdate()--Start";
}

void MinimumView::custConnectedEventSetting()
{
    if(((Uconnect*)pUConnect)->autoMode == true)
    {
        ui->minconnectButton->setStyleSheet("image: url(:/autoconnected);");
        ui->minconnectButton->setDisabled(true);
    }
    else
    {
        ui->minconnectButton->setStyleSheet("image: url(:/disconnect);");
        ui->minconnectButton->setDisabled(false);
    }

}

void MinimumView::custidleEnableEventSetting()
{
    if(((Uconnect*)pUConnect)->autoMode == true)
    {
        ui->minconnectButton->setStyleSheet("image: url(:/autoconnected);");
        ui->minconnectButton->setDisabled(true);
    }
    else
    {
        ui->minconnectButton->setDisabled(false);
        ui->minconnectButton->setStyleSheet("image: url(:/connect);");
        ui->network_name->clear();
    }
}

void MinimumView::custEvents(int event)
{
    switch(event)
    {

    case CUST_SYS_DEVICE_ERROR_EVENT:
        {

            ui->network_name->clear();
            ui->minconnectButton->setStyleSheet("image: url(:/connect);");
          //ui->minconnectButton->setText(tr("Connect"));
        }
        break;
    case CUST_SYS_DISCONNECTED_EVENT:
        {
            ui->minconnectButton->setStyleSheet("image: url(:/connect);");
            // ui->minconnectButton->setText(tr("Connect"));

             ui->network_name->clear();
             //ui->mCMInfo.ipAddress[0] = '\0';
             ((Uconnect *) pUConnect)->mCMInfo.ipAddress[0] = '\0';


        }
        break;
    case CUST_SYS_DEVICE_DISCONNECTED_EVENT:
        {
            ui->minconnectButton->setStyleSheet("image: url(:/connect);");
            //ui->minconnectButton->setText(tr("Connect"));

            ui->network_name->clear();
            ((Uconnect *) pUConnect)->mCMInfo.ipAddress[0] = '\0';


        }
        break;
    case CUST_CM_SERVER_CONN_LOST:
        {

           // ui->minconnectButton->setStyleSheet("image: url(:/connect_dis);");
              //ui->minconnectButton->setText(tr("Connect"));
              ui->minconnectButton->hide();
              ui->network_name->clear();


        }
        break;
    default:
        break;
    }
}

void MinimumView::DisconnectSetting()
{

    if(((Uconnect*)pUConnect)->autoMode == true)
    {
        ui->minconnectButton->setStyleSheet("image: url(:/autoconnected);");
        ui->minconnectButton->setDisabled(true);
    }
    else
    {
        ui->minconnectButton->setStyleSheet("image: url(:/connect_dis);");
        ui->minconnectButton->setDisabled(false);
        ui->network_name->clear();
    }


}
void MinimumView::showMinConnectStyleSheetAuto(ETechSelect techId)
{
    if(gTechDetails[techId].state == NM_CONNECTED_STATE)
    {
        ui->minconnectButton->show();
        ui->minconnectButton->setStyleSheet("image: url(:/autoconnected);");
        ui->minconnectButton->setDisabled(true);
    }else
    {
        ui->minconnectButton->hide();
    }
}

void MinimumView::on_onselectwifiPushButton_clicked()
{
	((Uconnect*)pUConnect)->setSettingsOnMinview(true,WIFI_TECH);
    updateWifiMinviewLabels(true);
}

void MinimumView::showMinConnectStyleSheetManual(ETechSelect techId)
{

	if(gTechDetails[techId].state == NM_CONNECTED_STATE)
    {
        ui->minconnectButton->setStyleSheet("image: url(:/disconnect);");
    }
    else
    {
        ui->minconnectButton->setStyleSheet("image: url(:/connect);");
    }
    ui->minconnectButton->show();
    ui->minconnectButton->setDisabled(false);
}

void MinimumView::on_onselectLtePushButton_clicked()
{
	((Uconnect*)pUConnect)->setSettingsOnMinview(true,LTE_TECH);
    updateLTEMinviewLabels(true);

}

void MinimumView::on_onselectWiredPushButton_clicked()
{
    ui->minconnectButton->hide(); //because we haven't implemented connect yet

    if(currentTech != ETHER_TECH )
    {
        currentTech = ETHER_TECH;
        // ui->onSelectedwiredlabel->setStyleSheet("image: url(:/onWiredUnselected);");
        ui->onselectWiredPushButton->setStyleSheet("image: url(:/onWiredUnselected);background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:0.988727, stop:0 rgba(254, 187, 25, 255), stop:1 rgba(235, 112, 18, 255));");
    }
    else
    {
        currentTech = INVALID_TECH;
       // ui->onSelectedwiredlabel->setStyleSheet("image: url(:/onWiredSelected);");
        ui->onselectWiredPushButton->setStyleSheet("image: url(:/onWiredSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
    }
}

void MinimumView::setContextForConnectButton(bool connectionStatus)
{
    if(((Uconnect*)pUConnect)->autoMode == true)
    {
        if(connectionStatus)
        {
            ui->minconnectButton->setStyleSheet("image: url(:/autoconnected);");
            //ui->minconnectButton->setToolTip("");
            ui->minconnectButton->setToolTip("<i>CM configured in AUTO mode</i>");
            ui->minconnectButton->setDisabled(true);
        }
        else
        {
            ui->minconnectButton->hide();
        }
    }

    else
    {
        if(currentTech == INVALID_TECH)
        {
            return;
        }

		for(int i=0 ; i<MAX_NUMBER_OF_NETWORKMANAGERS ; i++)
		{
				//if(gTechDetails[i].state == NM_CONNECTED_STATE)
				//{
					//currentTech
					//ui->minconnectButton->setToolTip("");
					ui->minconnectButton->setToolTip("<i>CM configured in MANUAL mode</i>");
					showMinConnectStyleSheetManual(currentTech);
				//}
		}

       /* if(connectionStatus)
        {
            ui->minconnectButton->setStyleSheet("image: url(:/disconnect);");
        }
        else
        {
            ui->minconnectButton->setStyleSheet("image: url(:/connect);");
        }*/
    }

}

void MinimumView::wifinetworkDisplay(QString networkName)
 {
	if(gTechDetails[WIFI_TECH].state != NM_CONNECTED_STATE)
    {
        ui->wifiComboBox->addItem(networkName);
    }
 }

void MinimumView::on_wifiComboBox_activated(QString )
{
    //select network.

}

void MinimumView::clearWifiNetworksDisplay()
{
    ui->wifiComboBox->clear();
    ui->wifiComboBox->addItem("Wifi Networks");
}

void MinimumView::clearThreeGNetworksDisplay()
{
     ui->threeGComboBox->clear();
      ui->threeGComboBox->addItem("3G Network");
}


void MinimumView::updateConnectionState(ETechSelect techId, bool connectStatus)
{

    switch(techId)
    {
    case WIFI_TECH:
        {
            QString message = "Wifi Disconnected";
            if(connectStatus)
            {
                message = "Wifi Connected";
            }
            trayIcon->showMessage("TECH:WIFI", message, QSystemTrayIcon::Information, 151000);
            break;
        }
    }
    if(techId == currentTech)
    {
        setContextForConnectButton(connectStatus);
    }

}

void MinimumView::showDropDownForWifi()
{
	if(gTechDetails[WIFI_TECH].state == NM_CONNECTED_STATE)
    {
        ui->wifiInfoUp->setVisible(true);
       // ui->wifiInfoUp->raise();
        //ui->wifiInfoUp->activateWindow();
    }

}


void MinimumView::on_wifiInfoUp_clicked()
{

    if(frameAnimationTimeLineWifi == NULL){
            frameAnimationTimeLineWifi = new QTimeLine(333,this);
        }
        connect(frameAnimationTimeLineWifi,SIGNAL(frameChanged(int)),this,SLOT(animatedWifiResize(int)));
        frameAnimationTimeLineWifi->setFrameRange(0,100);
        frameAnimationTimeLineWifi->start();

        ui->wifiInfoUp->setVisible(false);
        ui->wifiInfoDown->setVisible(true);
		ui->wifiInfoLabel->setVisible(true);
		//ui->wifiInfoLabel->raise();
		ui->wifiInfoDown->raise();
		ui->wifiStatsframe->show();
		ui->wifiStatsframe->raise();
}

void MinimumView::on_wifiInfoDown_clicked()
{

    if(frameAnimationTimeLineWifi == NULL){
        frameAnimationTimeLineWifi = new QTimeLine(333,this);
    }
    connect(frameAnimationTimeLineWifi,SIGNAL(frameChanged(int)),this,SLOT(animatedWifiResize(int)));
    frameAnimationTimeLineWifi->setFrameRange(100,0);
    frameAnimationTimeLineWifi->start();

    ui->wifiInfoDown->setVisible(false);
    ui->wifiInfoUp->setVisible(true);
    ui->onselectwifiPushButton->show();
    ui->wifiInfoUp->show();
}


void MinimumView::animatedLTEResize(int height){

    ui->lteStatsFrame->resize(ui->lteStatsFrame->width(),height);
    ui->threegStatsframe->resize(ui->threegStatsframe->width(),0);
    ui->wifiStatsframe->resize(ui->wifiStatsframe->width(),0);
}

void MinimumView::animatedWifiResize(int height){

    ui->wifiStatsframe->resize(ui->wifiStatsframe->width(),height);
    ui->lteStatsFrame->resize(ui->lteStatsFrame->width(),0);
    ui->threegStatsframe->resize(ui->threegStatsframe->width(),0);

}

void MinimumView::animated3GResize(int height){

    ui->threegStatsframe->resize(ui->threegStatsframe->width(),height);
    ui->lteStatsFrame->resize(ui->lteStatsFrame->width(),0);
    ui->wifiStatsframe->resize(ui->wifiStatsframe->width(),0);
}

void MinimumView::on_lte4gInfoUp_clicked()
{

    ui->lte4gInfoLabel->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(125, 125, 125, 100), stop:1 rgba(125, 125, 125, 100));");
    if(frameAnimationTimeLineLte == NULL){
                frameAnimationTimeLineLte = new QTimeLine(333,this);
            }
            connect(frameAnimationTimeLineLte,SIGNAL(frameChanged(int)),this,SLOT(animatedLTEResize(int)));
            frameAnimationTimeLineLte->setFrameRange(0,100);
            frameAnimationTimeLineLte->start();


        ui->lte4gInfoUp->setVisible(false);
        ui->lte4gInfoDown->setVisible(true);
		ui->lteStatsFrame->raise();
		ui->lteStatsFrame->show();
        ui->lte4gInfoDown->show();
		//ui->lte4gInfoLabel->show();
}

void MinimumView::showDropDownForLte()
{
	if(gTechDetails[LTE_TECH].state == NM_CONNECTED_STATE)
    {
        //ui->lte4gInfoUp->raise();
       // ui->lte4gInfoUp->activateWindow();
        ui->lte4gInfoUp->setVisible(true);
    }

}

void MinimumView::on_lte4gInfoDown_clicked()
{

    if(frameAnimationTimeLineLte == NULL){
        frameAnimationTimeLineLte = new QTimeLine(333,this);
    }
    connect(frameAnimationTimeLineLte,SIGNAL(frameChanged(int)),this,SLOT(animatedLTEResize(int)));
    frameAnimationTimeLineLte->setFrameRange(100,0);
    frameAnimationTimeLineLte->start();

    ui->lte4gInfoDown->setVisible(false);
    ui->lte4gInfoUp->setVisible(true);
    ui->lte4gInfoUp->show();
    ui->onselectLtePushButton->show();
}

void MinimumView::showWifiStats(string stats)
{
    vector<string> vStr;
    tokeniseInformation(stats,vStr,'\n');

    for(unsigned int i=0;i<vStr.size();i++)
    {
        vector<string> paramVector;
        tokeniseInformation(vStr[i],paramVector,';');
        if(paramVector.size()<2)
        {
            continue;
        }
        QString value = QString::fromStdString(paramVector[1]);
        if(!strcmp(paramVector[0].c_str(),"TxRate"))
        {
            ui->tx_rate->setText(value);

        }
        else if(!strcmp(paramVector[0].c_str(),"RxRate"))
        {
            ui->rx_rate->setText(value);

        }
    }

}

void MinimumView::on_onselect3gPushButton_clicked()
{
	((Uconnect*)pUConnect)->setSettingsOnMinview(true,THREE_G_TECH);
    updateThreeGMinviewLabels(true);

}


void MinimumView::show3gStats(string stats)
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



        if(strstr(param.c_str(),"DL_Rate"))
        {

            float downlink_val = floor((atof(value.c_str())*100.0)+0.5)/100.0;
            QString downStr = QString::number( downlink_val );
            downStr.append("Kbps");
            ui->threeGDownload_rate->setText(downStr);
        }
        else if(strstr(param.c_str(),"UL_Rate"))
        {
            float uplink_val = floor((atof(value.c_str())*100.0)+0.5)/100.0;
            QString upStr = QString::number( uplink_val );
            upStr.append("Kbps");
            ui->threeGUpload_rate->setText(upStr);
        }
        else if(strstr(param.c_str(),"Duration"))
        {
        }

		}
}
void MinimumView::showDropDownFor3G()
{
	if(gTechDetails[THREE_G_TECH].state == NM_CONNECTED_STATE)
    {
        //ui->wifiInfoUp->show();
        ui->threeGInfoUp->setVisible(true);
        //ui->threeGInfoUp->raise();
        //ui->threeGInfoUp->activateWindow();
    }

}

void MinimumView::on_threeGInfoUp_clicked()
{

    if(frameAnimationTimeLine3G == NULL){
            frameAnimationTimeLine3G = new QTimeLine(333,this);
        }
        connect(frameAnimationTimeLine3G,SIGNAL(frameChanged(int)),this,SLOT(animated3GResize(int)));
        frameAnimationTimeLine3G->setFrameRange(0,100);
        frameAnimationTimeLine3G->start();

        ui->threeGInfoUp->setVisible(false);
        ui->threeGInfoDown->setVisible(true);
       ui->threeInfoLabel->setVisible(true);
        ui->threeGInfoDown->show();
		ui->threegStatsframe->show();
		ui->threegStatsframe->raise();
}

void MinimumView::on_threeGInfoDown_clicked()
{

    if(frameAnimationTimeLine3G == NULL){
            frameAnimationTimeLine3G = new QTimeLine(333,this);
        }
        connect(frameAnimationTimeLine3G,SIGNAL(frameChanged(int)),this,SLOT(animated3GResize(int)));
        frameAnimationTimeLine3G->setFrameRange(100,0);
        frameAnimationTimeLine3G->start();

        ui->threeGInfoDown->setVisible(false);
        ui->threeGInfoUp->setVisible(true);
        ui->onselect3gPushButton->show();
        ui->threeGInfoUp->show();

}

void MinimumView::updateMinNMSettings(ETechSelect techId,bool state)
{

    switch(techId)
    {
    case LTE_TECH:
        {
            if(state)
            {
                trayIcon->showMessage("LTE Device Message", "Device is Plugged-In", QSystemTrayIcon::Critical, 151000);
                ui->onselectLtePushButton->setDisabled(false);
                ui->apnComboBox->setDisabled(false);
            }
            else
            {
                trayIcon->showMessage("LTE Device Message", "Device is Plugged-Out", QSystemTrayIcon::Critical, 151000);
                ui->onselectLtePushButton->setDisabled(true);
                ui->apnComboBox->setDisabled(true);
            }
        }
        break;
    case WIFI_TECH:
        {
            if(state)
            {
                trayIcon->showMessage("WIFI Adapter Message", "Adapter is Available", QSystemTrayIcon::Critical, 151000);
                ui->onselectwifiPushButton->setDisabled(false);
                ui->wifiComboBox->setDisabled(false);
            }
            else
            {
                trayIcon->showMessage("WIFI Adapter Message", "Adapter is not Avaliable", QSystemTrayIcon::Critical, 151000);
                //ui->onselectwifiPushButton->setDisabled(true);
                ui->wifiComboBox->setDisabled(true);

            }
        }
        break;
    case THREE_G_TECH:
        {
            if(state)
            {
                trayIcon->showMessage("3G Device Message", "Device is Plugged-In", QSystemTrayIcon::Critical, 151000);
                ui->onselect3gPushButton->setDisabled(false);
                ui->threeGComboBox->setDisabled(false);

            }
            else
            {
                trayIcon->showMessage("3G Device Message", "Device is Plugged-Out", QSystemTrayIcon::Critical, 151000);
                ui->onselect3gPushButton->setDisabled(true);
                ui->threeGComboBox->setDisabled(true);

            }
        }
        break;
    default:
        break;


    }

}

void MinimumView::setConnectButtonMinviewForAutoMode(bool connectButtonState)//tool tip
{
    if(connectButtonState)
    {
        ui->minconnectButton->setDisabled(true);
        ui->minconnectButton->setToolTip("CM configured in AUTO MODE");
        ui->minconnectButton->setStatusTip(tr("CM configured in AUTO MODE"));
    }
}

void MinimumView::updateOnNMStateChange(ETechSelect techId, ENMStateForBM state)
{


    //ui->sigStrengthLte->hide();

   
    if(techId == LTE_TECH && state != NM_CONNECTED_STATE)
    {
        ui->apnComboBox->clear();
        ui->apnComboBox->addItem("4G/LTE Networks");
    }
	if(techId == THREE_G_TECH && state != NM_CONNECTED_STATE)
	{
		ui->threeGComboBox->clear();
        ui->threeGComboBox->addItem("3G Networks");
	}



    bool connected = false;
    for(int i=0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
    {
        if(gTechDetails[techId].state == NM_CONNECTED_STATE)
            connected = true;

    }
	if(connected != true)
	{
		ui->vpnLabel->hide();	
	}

	/*if(gTechDetails[techId].oldState == NM_CONNECTED_STATE && gTechDetails[techId].state != NM_CONNECTED_STATE)
	{
		connectedNMCount++;
	}*/
	if(gTechDetails[techId].oldState != NM_DISABLED_STATE && gTechDetails[techId].state == NM_DISABLED_STATE)
	{
		QString statusMsg = "";
		statusMsg.append(gTechDetails[techId].name.c_str());
		statusMsg.append(" is Disabled");
		showStatusMsg(statusMsg);
	}
	else if (gTechDetails[techId].oldState == NM_DISABLED_STATE && gTechDetails[techId].state != NM_CONNECTED_STATE && gTechDetails[techId].state != NM_DISABLED_STATE)
	{
		QString statusMsg = "";
		statusMsg.append(gTechDetails[techId].name.c_str());
		statusMsg.append(" is Enabled");
		showStatusMsg(statusMsg);	
	}

	if(gTechDetails[techId].oldState == NM_CONNECTED_STATE && gTechDetails[techId].state != NM_CONNECTED_STATE)
	{
		setContextForConnectButton(((Uconnect*)pUConnect)->autoMode);
			
		QString statusMsg = "";
		ui->vpnLabel->hide();
		//connectedNMCount--;
	

		for(int i = 0;i < MAX_NUMBER_OF_NETWORKMANAGERS;i++)
		{

			if(gTechDetails[i].state == NM_CONNECTED_STATE && ((Uconnect*)pUConnect)->autoMode == true)
			{
				statusMsg = "Connected to ";
				statusMsg.append(gTechDetails[i].name.c_str());
				statusMsg.append("\n");
			}
		}
        ui->lteStatsFrame->hide();
        ui->wifiStatsframe->hide();
        ui->threegStatsframe->hide();
		statusMsg.append(" Disconnected from ");
		statusMsg.append(gTechDetails[techId].name.c_str());
		showStatusMsg(statusMsg);

	}
	if(gTechDetails[techId].state == NM_CONNECTED_STATE && (gTechDetails[techId].oldState != NM_CONNECTED_STATE))
	{
		setContextForConnectButton(((Uconnect*)pUConnect)->autoMode);
		QString statusMsg = "Connected to ";
		statusMsg.append(gTechDetails[techId].name.c_str());
		showStatusMsg(statusMsg);
	}

	if(gTechDetails[techId].state != NM_CONNECTED_STATE)
	{
		sigIconupdateMinviewSetting(sigStrength[techId].imageNumb,techId);

	}

    switch (state)
    {
    case NM_DISABLED_STATE:
        {
		
            if(techId == LTE_TECH)
            {
				((Uconnect*)pUConnect)->lteVersion.clear();
				((Uconnect*)pUConnect)->updateVersion(LTE_TECH,"");
				((Uconnect*)pUConnect)->verLteUpdateState = false;
                ui->onselectLtePushButton->setStyleSheet("image: url(:/onLteSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
                ui->onselectLtePushButton->setDisabled(true);
                ui->apnComboBox->setDisabled(true);
                ui->upload_rate->clear();
                ui->download_rate->clear();
                //ui->lte4gInfoLabel->hide();
                ui->lte4gInfoDown->hide();
                ui->lte4gInfoUp->hide();



            }else if(techId == WIFI_TECH)
            {
				((Uconnect*)pUConnect)->wifiVersion.clear();
				((Uconnect*)pUConnect)->updateVersion(WIFI_TECH,"");
				((Uconnect*)pUConnect)->verWifiUpdateState = false;
                ui->onselectwifiPushButton->setStyleSheet("image: url(:/onWifiSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
                ui->onselectwifiPushButton->setDisabled(true);
                ui->wifiComboBox->setDisabled(true);
                clearWifiNetworksDisplay();
                ui->wifiInfoUp->hide();
                ui->wifiInfoDown->hide();
                ui->rx_rate->clear();
                ui->tx_rate->clear();
				ui->wifiInfoLabel->hide();


            }
            else if(techId == THREE_G_TECH)
            {
				((Uconnect*)pUConnect)->threeGVersion.clear();
				((Uconnect*)pUConnect)->updateVersion(THREE_G_TECH,"");
				((Uconnect*)pUConnect)->verThreeGUpdateState = false;
                ui->onselect3gPushButton->setStyleSheet("image: url(:/on3gSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
                ui->onselect3gPushButton->setDisabled(true);
                ui->threeGComboBox->setDisabled(true);
                clearThreeGNetworksDisplay();
                ui->threeGInfoDown->hide();
                ui->threeGInfoUp->hide();
                ui->threeGUpload_rate->clear();
                ui->threeGDownload_rate->clear();
				ui->threeInfoLabel->hide();
            }
        updateMessageBar();
        }
        break;
    case NM_NOT_READY_STATE:
    case NM_READY_STATE:
        {
          // ui->sigStrengthLte->show();
           getReadySettings(techId);
           updateMessageBar();

        }
        break;

    case NM_CONNECTED_STATE:
        {
           hideDisconnectedTechItems(techId);
        }
        break;

    default:
        break;
    }

    //connectionState[techId] = connected;


    //handle showing of connect/disconnect

    if(currentTech == techId)
    {
        setContextForConnectButton(connected);
    }

}

void MinimumView::getReadySettings(ETechSelect techId)
{

    if(techId == LTE_TECH)
    {
        ui->onselectLtePushButton->setStyleSheet("image: url(:/onLteSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
        ui->apnComboBox->setDisabled(false);
        ui->upload_rate->clear();
        ui->download_rate->clear();
        ui->lte4gInfoUp->hide();
		setContextForConnectButton(((Uconnect*)pUConnect)->autoMode);

    }else if(techId == WIFI_TECH)
    {
       ui->onselectwifiPushButton->setStyleSheet("image: url(:/onWifiSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
        ui->onselectwifiPushButton->setDisabled(false);
		ui->wifiComboBox->clear();
		ui->wifiComboBox->addItem("Wifi Networks");
        ui->wifiComboBox->setDisabled(false);
        ui->wifiInfoUp->hide();
        ui->rx_rate->clear();
        ui->tx_rate->clear();
		if(gTechDetails[techId].oldState == NM_CONNECTED_STATE)
		{
			QString statusMsg = "";
			for (int i = 0 ; i<MAX_NUMBER_OF_NETWORKMANAGERS ; i++)
			{
				if(gTechDetails[i].state == NM_CONNECTED_STATE)
				{
					statusMsg = "Connected to ";
					statusMsg.append(gTechDetails[i].name.c_str());
					statusMsg.append("\n");
				}
			}

			statusMsg.append(" Disconnected from ");
			statusMsg.append(gTechDetails[techId].name.c_str());
			showStatusMsg(statusMsg);
		}
		setContextForConnectButton(((Uconnect*)pUConnect)->autoMode);
    }
    else if(techId == THREE_G_TECH)
    {
        //ui->onselect3gPushButton->setStyleSheet("image: url(:/on3gSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
        ui->onselect3gPushButton->setDisabled(false);
        ui->threeGComboBox->setDisabled(false);
        ui->threeGInfoUp->hide();
        ui->threeGDownload_rate->clear();
        ui->threeGUpload_rate->clear();
		setContextForConnectButton(((Uconnect*)pUConnect)->autoMode);
    }
}

void MinimumView::updateNetworkName(string threeGName)
{
    vector<string> vStr;
    tokeniseInformation(threeGName,vStr,'\n');
    for(unsigned int i=0;i<vStr.size();i++)
    {
        vector<string> rowVector;
        tokeniseInformation(vStr[i],rowVector,':');
        if(rowVector.size() < 2)
        {
            continue;
        }
        string param = rowVector[0];
        string value = rowVector[1];
        //append param and value to widget;

        if(strstr(param.c_str(),"NAME"))
        {
            ui->threeGComboBox->clear();
            ui->threeGComboBox->addItem(value.c_str());
        }
    }

}
void MinimumView::settingServerNotThere()
{
	//ui->sigStrengthLte->hide();
	for (int i=0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
	{
		sigStrength[i].label->clear();
        sigStrength[i].label->hide();
	}
	
	ui->messageBar->clear();
	ui->minconnectButton->hide();
    ui->onselectLtePushButton->setDisabled(true);
	ui->onselectLtePushButton->setStyleSheet("image: url(:/onLteSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
    ui->apnComboBox->setDisabled(true);
    ui->onselectwifiPushButton->setDisabled(true);
    ui->onselectwifiPushButton->setStyleSheet("image: url(:/onWifiSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
    ui->wifiComboBox->setDisabled(true);
    ui->onselect3gPushButton->setDisabled(true);
	ui->onselect3gPushButton->setStyleSheet("image: url(:/on3gSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
    ui->threeGComboBox->setDisabled(true);
	ui->apnComboBox->clear();
	ui->apnComboBox->addItem("4G/LTE Networks");
	ui->threeGComboBox->clear();
	ui->threeGComboBox->addItem("3G Networks");
	ui->wifiComboBox->clear();
	ui->wifiComboBox->addItem("Wifi Networks");
    ui->wifiInfoUp->hide();
    ui->lte4gInfoUp->hide();
    ui->threeGInfoUp->hide();
    ui->wifiStatsframe->hide();
    ui->threegStatsframe->hide();
    ui->lteStatsFrame->hide();
	ui->vpnLabel->clear();
}


void MinimumView::settingServerThere()
{
    //ui->sigStrengthLte->show();
	//ui->minconnectButton->show();
    ui->onselectLtePushButton->setDisabled(false);
    ui->apnComboBox->setDisabled(false);
    ui->onselectwifiPushButton->setDisabled(false);
    ui->wifiComboBox->setDisabled(false);
    ui->onselect3gPushButton->setDisabled(false);
    ui->threeGComboBox->setDisabled(false);
    //ui->sigStrengthLte->show();
	for(int i=0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
	{
		if(gTechDetails[i].state == NM_CONNECTED_STATE)
		{
			setContextForConnectButton(((Uconnect*)pUConnect)->autoMode);
		}
	}
}

void MinimumView::showWifiNetworkName(string wifiname)
{
    ui->wifiComboBox->clear();
    ui->wifiComboBox->addItem(wifiname.c_str());
}

void MinimumView::updateWifiMinviewLabels(bool userPressed)
{
    //ui->minconnectButton->show();
    ui->onselectLtePushButton->setStyleSheet("image: url(:/onLteSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
     ui->onselect3gPushButton->setStyleSheet("image: url(:/on3gSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
	ui->lte4gInfoLabel->hide();
	ui->lteStatsFrame->hide();
	ui->threeInfoLabel->hide();
	ui->threegStatsframe->hide();
	ui->wifiStatsframe->hide();


    if(currentTech != WIFI_TECH || !userPressed)
    {
		if(gTechDetails[WIFI_TECH].state == NM_CONNECTED_STATE)
        {

            /*if(((Uconnect*)pUConnect)->autoMode == true)
            {
              showMinConnectStyleSheetAuto(WIFI_TECH);
            }
            else
            {
              showMinConnectStyleSheetManual(WIFI_TECH);
            }*/
				ui->messageBar->setText("Connected to Wifi");
				ui->minconnectButton->show();
				showDropDownForWifi();
        
				//sigIconupdateMinviewSetting(0,WIFI_TECH);

			//hideDisconnectedTechItems(THREE_G_TECH,NM_CONNECTED_STATE);(WIFI_TECH,NM_CONNECTED_STATE);
        }
		currentTech = WIFI_TECH;
        //ui->onSelectedWifilabel->setStyleSheet("image: url(:/onWifiUnselected);");
        ui->onselectwifiPushButton->setStyleSheet("image: url(:/onWifiUnselected);background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:0.988727, stop:0 rgba(254, 187, 25, 255), stop:1 rgba(235, 112, 18, 255));");
        ((Uconnect*)pUConnect)->settingForWifiOnDetView();
		setContextForConnectButton(gTechDetails[WIFI_TECH].state == NM_CONNECTED_STATE);
		

    }
    else
    {

        //ui->sigStrengthLte->clear();
		((Uconnect*)pUConnect)->setSettingsOnMinviewForGeneral();
        ui->minconnectButton->hide();
        currentTech = INVALID_TECH;
        ui->onselectwifiPushButton->setStyleSheet("image: url(:/onWifiSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");

    }
}

void MinimumView::updateThreeGMinviewLabels(bool userPressed)
{

    ui->onselectLtePushButton->setStyleSheet("image: url(:/onLteSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
    ui->onselectwifiPushButton->setStyleSheet("image: url(:/onWifiSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
    ui->lte4gInfoLabel->hide();
    ui->lteStatsFrame->hide();
    ui->wifiInfoLabel->hide();
    ui->wifiStatsframe->hide();
	ui->threegStatsframe->hide();


            if(currentTech != THREE_G_TECH || !userPressed)
			{
                
				if(gTechDetails[THREE_G_TECH].state == NM_CONNECTED_STATE )
                {
								ui->minconnectButton->show();
            /*        if(((Uconnect*)pUConnect)->autoMode == true)
                    {
                        showMinConnectStyleSheetAuto(THREE_G_TECH);
                    }
                    else
                    {
                        showMinConnectStyleSheetManual(THREE_G_TECH);
                    }*/
						//ui->messageBar->setText("Connected to 3G");
						updateAccessType(accessTypeName);
						//sigIconupdateMinviewSetting(4,THREE_G_TECH);

					//hideDisconnectedTechItems(THREE_G_TECH,NM_CONNECTED_STATE);
					showDropDownFor3G();

            }


        currentTech = THREE_G_TECH;
        ui->onselect3gPushButton->setStyleSheet("image: url(:/on3gUnselected);background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:0.988727, stop:0 rgba(254, 187, 25, 255), stop:1 rgba(235, 112, 18, 255));");
        ((Uconnect*)pUConnect)->settingForThreeGOnDetView();
		setContextForConnectButton(gTechDetails[THREE_G_TECH].state == NM_CONNECTED_STATE);
			//((Uconnect*)pUConnect)->setSettingsOnMinviewForGeneral();


    }
    else
    {
		((Uconnect*)pUConnect)->setSettingsOnMinviewForGeneral();
        //ui->sigStrengthLte->clear();
        ui->minconnectButton->hide();
        currentTech = INVALID_TECH;
        ui->onselect3gPushButton->setStyleSheet("image: url(:/on3gSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");


    }
}

void MinimumView::updateLTEMinviewLabels(bool userPressed)
{
    ui->onselectwifiPushButton->setStyleSheet("image: url(:/onWifiSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
    ui->onselect3gPushButton->setStyleSheet("image: url(:/on3gSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");
    ui->wifiInfoLabel->hide();
    ui->wifiStatsframe->hide();
    ui->threegStatsframe->hide();
    ui->threeInfoLabel->hide();
	ui->lteStatsFrame->hide();


    if(currentTech != LTE_TECH || !userPressed)
    {
        //ui->minconnectButton->show();
		if(gTechDetails[LTE_TECH].state == NM_CONNECTED_STATE)
        {

           /* if(((Uconnect*)pUConnect)->autoMode == true)
            {
                showMinConnectStyleSheetAuto(LTE_TECH);
            }
            else
            {
                showMinConnectStyleSheetManual(LTE_TECH);
            }*/
				ui->messageBar->setText("Connected to 4G/LTE");
				showDropDownForLte();
				ui->minconnectButton->show();
				//sigIconupdateMinviewSetting(4,LTE_TECH);
				
			//hideDisconnectedTechItems(LTE_TECH,NM_CONNECTED_STATE);
        }

        currentTech = LTE_TECH;
       ui->onselectLtePushButton->setStyleSheet("image: url(:/onLteUnselected);background-color: qlineargradient(spread:pad, x1:0, y1:0.029, x2:0, y2:0.988727, stop:0 rgba(254, 187, 25, 255), stop:1 rgba(235, 112, 18, 255));");
       ((Uconnect*)pUConnect)->settingForLteOnDetView();
	   setContextForConnectButton(gTechDetails[LTE_TECH].state == NM_CONNECTED_STATE);
       





    }
    else
    {
		((Uconnect*)pUConnect)->setSettingsOnMinviewForGeneral();
        //ui->sigStrengthLte->clear();
        currentTech = INVALID_TECH;
        ui->minconnectButton->hide();
        //ui->onSelectedLtelabel->setStyleSheet("image: url(:/onLteSelected);");
        ui->onselectLtePushButton->setStyleSheet("image: url(:/onLteSelected);background-color: qlineargradient(spread:reflect, x1:0, y1:0.029, x2:0, y2:0.494, stop:0 rgba(8, 9, 7, 255), stop:1 rgba(30, 30, 30, 255));");


    }
}


void MinimumView::hideDisconnectedTechItems(ETechSelect techId)
{
	if(gTechDetails[techId].state == NM_CONNECTED_STATE)
	{
		switch(techId)
		{
		case WIFI_TECH:
			hide3gItems();
			hide4gItems();
			showWifiItems();
			
			break;
		case THREE_G_TECH:
			hide4gItems();
			hideWifiItems();
			show3gItems();
			
			break;
		case LTE_TECH:
			hide3gItems();
			hideWifiItems();
			show4gItems();
			
			break;
		}
		updateMessageBar();
	}
}


void MinimumView::hide3gItems()
{
    ui->threeGInfoDown->hide();
   // ui->threeInfoLabel->hide();
    ui->threeGInfoUp->hide();

}

void MinimumView::hide4gItems()
{

    ui->lte4gInfoUp->hide();
    //ui->lte4gInfoLabel->hide();
    ui->lte4gInfoDown->hide();

}

void MinimumView::hideWifiItems()
{
    ui->wifiInfoUp->hide();
    ui->wifiInfoDown->hide();
}

void MinimumView::showWifiItems()
{
    //showDropDownForWifi();
    ui->onselectwifiPushButton->setDisabled(false);
    ui->wifiComboBox->setDisabled(false);
    updateWifiMinviewLabels(false);
	ui->messageBar->setText("Connected to Wifi");
    //sigIconupdateMinviewSetting(4,WIFI_TECH);
}

void MinimumView::show3gItems()
{
   //showDropDownFor3G();
   ui->onselect3gPushButton->setDisabled(false);
   ui->threeGComboBox->setDisabled(false);
   updateThreeGMinviewLabels(false);
	//ui->messageBar->setText("Connected to 3G");
   updateAccessType(accessTypeName);
   //sigIconupdateMinviewSetting(0,THREE_G_TECH);

}

void MinimumView::show4gItems()
{
    //showDropDownForLte();
    ui->onselectLtePushButton->setDisabled(false);
    ui->apnComboBox->setDisabled(false);
    updateLTEMinviewLabels(false);
	ui->messageBar->setText("Connected to 4G/LTE");
    //sigIconupdateMinviewSetting(0,LTE_TECH);
}

void MinimumView::on_versionPushButton_clicked()
{
	((Uconnect *) pUConnect)->initialVersionSetting();
	 QMessageBox a;
     //((Uconnect *) pUConnect)->disp.clear();
	((Uconnect *) pUConnect)->updateVersion(LTE_TECH,((Uconnect *) pUConnect)->lteVersion.toStdString());
	((Uconnect *) pUConnect)->updateVersion(THREE_G_TECH,((Uconnect *) pUConnect)->threeGVersion.toStdString());
	((Uconnect *) pUConnect)->updateVersion(WIFI_TECH,((Uconnect *) pUConnect)->wifiVersion.toStdString());
    a.about(this,tr("Version Information"),((Uconnect *) pUConnect)->disp);


}

void MinimumView::updateMessageBar()
{
    bool isNotConnectedAnyTech = false;
	for(int i=0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
    {
        if(gTechDetails[i].state == NM_CONNECTED_STATE)
        {
            isNotConnectedAnyTech = true;
			switch(gTechDetails[i].techId)
			{
			case LTE_TECH:
				if(((Uconnect*)pUConnect)->vpnConnectStatus == true)
                {
                    ui->vpnLabel->setStyleSheet("image: url(:/vpnConnected);");
					ui->vpnLabel->show();
                    showStatusMsg("VPN Connected throught 4G/LTE");
				}else
				{
                    vpnDisconnectMsg();
                }
                ui->messageBar->setText("Connected to 4G/LTE ");
                ui->onselectLtePushButton->setToolTip("<i>Connected to 4G/LTE</i>");
				break;
			case WIFI_TECH:
    			if(((Uconnect*)pUConnect)->vpnConnectStatus == true)
				{
                    ui->vpnLabel->setStyleSheet("image: url(:/vpnConnected);");
					ui->vpnLabel->show();
                    showStatusMsg("VPN Connected throught Wifi");
				}
				else
				{
                    vpnDisconnectMsg();
                }
                ui->messageBar->setText("Connected to Wifi");
                ui->onselectwifiPushButton->setToolTip("<i>Connected to Wifi</i>");
				break;
            case THREE_G_TECH:
				updateAccessType(accessTypeName);
                ui->onselect3gPushButton->setToolTip("<i>Connected to 3G/2G</i>");
				break;
			default:
				break;
			}
        }
    }
    if(isNotConnectedAnyTech == false)
    {
           ui->messageBar->setText("");
           ui->onselectLtePushButton->setToolTip("");
           ui->onselectwifiPushButton->setToolTip("");
           ui->onselect3gPushButton->setToolTip("");
           for(int i =0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
           {
               sigStrength[i].label->clear();
           }
    }
}

void MinimumView::vpnDisconnectMsg()
{
    if(((Uconnect*)pUConnect)->vpnMsgBarStatus == true)
    {
        showStatusMsg("VPN Disconnected");
        ui->vpnLabel->hide();
    }
}
void MinimumView::animatedDetResize(int height)
{
    resize(width(),height);
}

void MinimumView::showLTENetworkName(string apnName)
{
	if(gTechDetails[LTE_TECH].state == NM_CONNECTED_STATE)// this holds good for auto mode check this later
	{
		ui->apnComboBox->clear();
		ui->apnComboBox->addItem(apnName.c_str());
	}
}

void MinimumView::displayConnectFailedMin(ETechSelect techId,string ssid)
{
    if(ssid.size()< 0)
    {
        ssid = gTechDetails[techId].name;
    }

    QMessageBox a;
    QString dispInfo = "Unable to connect: ";
     dispInfo.append(ssid.c_str());
    a.about(this,
    tr("Info:"),
    dispInfo);
}

void MinimumView::updateAccessType(string accessType)
{
	if(gTechDetails[THREE_G_TECH].state == NM_CONNECTED_STATE)
	{
		QString accessTypeMsg = "Connected ";
		if(accessType.size()>0)
		{
			accessTypeMsg += "to " ;
			accessTypeMsg += accessType.c_str();
		}
		if(((Uconnect*)pUConnect)->vpnConnectStatus == true)
        {
            ui->vpnLabel->setStyleSheet("image: url(:/vpnConnected);");
            ui->vpnLabel->show();
			showStatusMsg("VPN Connected throught 3G/2G");
        }else
        {
            vpnDisconnectMsg();
        }
		ui->messageBar->setText(accessTypeMsg);
       // ui->onselect3gPushButton->setToolTip(accessTypeMsg);
	
	}
}

bool MinimumView::checkIfAnyTechConnected()
{
    for(int i=0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
    {
        if(gTechDetails[i].state == NM_CONNECTED_STATE)
            return true;

    }
 return false;

}
void MinimumView::showStatusMsg(QString statusMsg)
{
    trayIcon->showMessage("UConnect", statusMsg, QSystemTrayIcon::Information, 500);
}

void MinimumView::handleWifiDisabledNotification()
{
    QString msg = "Wifi is disabled";
    showStatusMsg(msg);
}
void MinimumView::handleWifiEnabledNotification()
{
	QString msg = "Wifi is Enabled";
    showStatusMsg(msg);
}

void MinimumView::displayErrorInfoMsgs(string errorStr,ENM_Error errorType,ETechSelect techId)
{
    QString techStr = gTechDetails[techId].name.c_str();

    if(errorType == NM_INFORMATIVE)
    {
        techStr.append(" Info");
        QMessageBox::warning(this,techStr,errorStr.c_str());
    }
    else if(errorType == NM_REPORT_ERROR)
    {
        techStr.append(" Error");
        QMessageBox::critical(this,techStr,errorStr.c_str());
    }

}

void MinimumView::showVersionOnMinview(QString uCMVersion)
{
    ui->versionNumb->setText(uCMVersion);

}
