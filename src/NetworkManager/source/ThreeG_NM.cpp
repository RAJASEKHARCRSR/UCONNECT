/*****************************************************************************
**
** ThreeG_NM.cpp
** Implements the function calls needed for 3G devices.
**
** Author: Mukta Sharma
** Date  : 20-APR-2012
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		20/04/12     MSH		Creation
****************************************************************************/
#include "ThreeG_NM.h"
#include "Three_g_dal.h"
#include "uconn_ATHandler.h"
#include "UCommonUtils.h"
#include "uconn_AT_DAL.h"
#include "uconn_server.h"

/* Init the Static vars of the Ras class */
#ifdef WIN32
HRASCONN URasConnection::URasConnHandle = NULL;

#endif 
bool URasConnection::URasIsConnected = false;
bool URasConnection::URasIsDialing = false;
U_DWORD URasConnection::RasError = RAS_SUCCESS;


UThreeG_NM::UThreeG_NM(UEventQueue* bearerManagerQueue):UNetworkManager(bearerManagerQueue)
{
	this->mCommonInfo.isEmulatorMode = 1;
	this->mCommonInfo.dalMode = 1;
	this->mCommonInfo.device_status = 0;

	dalCallbackfn = InitCallbackWithId;
	
	this->state = NM_DISABLED_STATE;

	/* Initialize the ATHandler instance */
	mATHandler = new UConnATHandler();

	this->mNMServer = new UConnServer();
	this->ThreeGNMInfo.reg_status = 0;
	this->ThreeGNMInfo.EnableCommandRecd = false;
	this->ThreeGNMInfo.stats.cinr = 0;
	this->ThreeGNMInfo.stats.rssi = 0;
	this->ThreeGNMInfo.stats.rasstats.dl_data = 0;
	this->ThreeGNMInfo.stats.rasstats.ul_data = 0;
	this->ThreeGNMInfo.stats.rasstats.duration = 0;
	
	mNMServer->getSM()->UCMStartUp(this);
	mNMServer->initialize();
	mNMServer->getSM()->getState()->entry();
	mNMServer->getSM()->mLTE_NM = NULL;

	if(this->state != NM_DISABLED_STATE)
	{
		UNMStateEvent* StateChange = new UNMStateEvent(THREE_G_TECH,this->state);
		postEventToBearerManager(reinterpret_cast<UEvent*>(StateChange));
	}

}

UThreeG_NM::~UThreeG_NM()
{
	/* All instances created using 'new' in constructor shud have a correspoding 
	 * 'delete' in the destructor here. 
	 */

	this->bearerMngrQueue = NULL;

	delete mNMServer;
	mNMServer = NULL;

	delete mATHandler;
	mATHandler = NULL;

}


/***********************************************************************
*	enableNetMngr -
*	function called from the BM to enable the ThreeG_NM
*
*	Parameters - 
*	None
***********************************************************************/
U_VOID UThreeG_NM::enableNetMngr()
{
    #ifdef WIN32
	if(this->mCommonInfo.dalMode == 1)
	{
		mNMServer->getSM()->dal 
			= reinterpret_cast<uconnDal*>(new UThreeGDal(this));
	}

	this->mCommonInfo.cfg_params.Enable_4G = 1;
	this->mCommonInfo.imsParams.Test_Mode = TEST_MODE_ENABLE;

/* post a message to the SM to enter into enable state. */
	UNMEnable *evt = new UNMEnable(THREE_G_TECH);
	postToSM(evt);
#endif
/* rest of the handling of enabling is done as part of function call from the SM.*/
}


/***********************************************************************
*	disableNetMngr -
*	function called from the BM to disable the ThreeG_NM 
*
*	Parameters - 
*	None
***********************************************************************/
U_VOID UThreeG_NM::disableNetMngr()
{
    #ifdef WIN32
	/* Post an event to state machine intimating 3G is disabled. */
	UNMDisable *evt = new UNMDisable(THREE_G_TECH);
	postToSM(evt);
#endif
	/* rest of the handling of disabling is done as part of function call from the SM*/
}


/***********************************************************************
*	connectToNetwork -
*	function called from the BM to connect to the ThreeG_NM
*
*	Parameters - 
*	None
***********************************************************************/
U_VOID UThreeG_NM::connectToNetwork()
{
    #ifdef WIN32
	/* Post an event to state machine intimating 3G connection shud b established. 
	 * rest of the handling of connecting is done as part of function call from the SM*/
	UNMConnect *evt = new UNMConnect(THREE_G_TECH);
	postToSM(evt);
#endif

}


/***********************************************************************
*	searchNetworks -
*	function called from the BM to Search for the 3G networks that are 
*   available. 
*
*	Parameters - 
*	None
***********************************************************************/
U_VOID UThreeG_NM::searchNetworks()
{
	/* Post an event to state machine intimating 3G networks shud b searched. 
	 * rest of the handling of searching is done as part of function call from the SM*/
    #ifdef WIN32
	UNMSearchNetworks *evt = new UNMSearchNetworks(THREE_G_TECH);
	postToSM(evt);

	NetworkSearchOn = true;
#endif
}


/***********************************************************************
*	disconnectFromNetwork -
*	function called from the BM to disconnect the ThreeG_NM
*
*	Parameters - 
*	None
***********************************************************************/
U_VOID UThreeG_NM::disconnectFromNetwork()
{
	/* Post an event to state machine intimating 3G shud b disconnected. 
	 * rest of the handling of disconnection is done as part of function call from the SM*/
    #ifdef WIN32
	UNMDisconnect *evt = new UNMDisconnect(THREE_G_TECH);
	postToSM(evt);
#endif
}


/***********************************************************************
*	initNetMngr -
*	function called from the BM to init the ThreeG_NM
*   The parameters passed are read from the config file and used to initialize
*   the corresponding variables in 3G NM. 
*
*	Parameters - 
*	configInfo - Pointer to the params read from config file. 
***********************************************************************/
U_VOID UThreeG_NM::initNetMngr(void *configInfo)
{
	ThreeG_Config_Info_s *threeGConfigInfo = (ThreeG_Config_Info_s*)configInfo;
	
	
	this->ThreeGNMInfo.VidPidCount = threeGConfigInfo->ThreeG_vidpidCount;
	
	for(unsigned int i=0;i<this->ThreeGNMInfo.VidPidCount;i++)
	{
		strcpy(this->ThreeGNMInfo.VidPidList[i],(char*)(threeGConfigInfo->ThreeG_VidPidName[i].c_str()));
	}


	strcpy(this->mCommonInfo.rasProfile, 
		threeGConfigInfo->rasProfileName.c_str());
	strcpy(this->mCommonInfo.rasUserName, 
		threeGConfigInfo->rasUsrName.c_str());
	strcpy(this->mCommonInfo.rasPassword, 
		threeGConfigInfo->rasPasswd.c_str());
	strcpy(this->mCommonInfo.rasPhoneNumber, 
		threeGConfigInfo->rasPhoneNo.c_str());
	strcpy(this->mCommonInfo.APN_name, 
		threeGConfigInfo->apnName.c_str());

	this->ThreeGNMInfo.UssdDecodingReqd = threeGConfigInfo->UssdDecodingReqd;

}


/***********************************************************************
*	setParam -
*	function called from the BM to set the parametrs in ThreeG_NM
*
*	Parameters - 
*	paramID - Parameter that needs to be set
*   param   - Value the ParamID shud be set to. 
***********************************************************************/
U_UINT32 UThreeG_NM::setParam(U_UINT32 paramID,string param)
{
	return 0;
}



/***********************************************************************
*	getStatisticsNM -
*	function called from the BM to get stats for the ThreeG_NM
*	Currently this function is not used in 3G, cauz the stats timer 
*   is managed in 3G NM and on expiry of timer the stats are sent to 
*   BM / GUI. 
*	Parameters - 
*	None
***********************************************************************/
U_UINT32 UThreeG_NM::getStatisticsNM()
{
	return 1;
}



/***********************************************************************
*	getTechID -
*	function called from the BM to get the TechId of ThreeG_NM
*
*	Parameters - 
*	None
***********************************************************************/
ETechSelect UThreeG_NM::getTechID()
{
	return THREE_G_TECH;
}


/***********************************************************************
*	postToSM -
*	function to post the event to 3G State Machine. 
*
*	Parameters - 
*	evt - pointer to the event that is being posted to the SM.
***********************************************************************/
U_VOID UThreeG_NM::postToSM(UEvent *evt)
{
	this->mNMServer->MsgPost((U_UINT8*)evt);
}


/***********************************************************************
*	deviceEventsCallBack -
*	function called when a 3G device is plugged in or removed.
*
*	Parameters - 
*	cmdID - parameter to discriminate if the event is device removal 
*           or device plug in.
***********************************************************************/
U_VOID UThreeG_NM::deviceEventsCallBack(U_UINT32 cmdID,U_UINT32 techID)
{
	UDeviceStateChange *evt;
	if ( cmdID == 3 )
	{
		evt = new UDeviceStateChange(NOTI_DEVICE_OPEN);
	}
	else if ( cmdID == 4 )
	{
			evt = new UDeviceStateChange(NOTI_DEVICE_NOT_OPEN);
	}
	postToSM(evt);
}



/***********************************************************************
*	EnableThreeGNM -
*	function called from the 3G SM to enable the ThreeG_NM
*
*	Parameters - 
*	None
***********************************************************************/
bool UThreeG_NM::EnableThreeGNM()
{
	/* Initialize the COM port, Open the COM port and Send some AT commands like - 
	 * AT+CFUN=1
	 * Register to the network AT+CGREG, AT+CREG
	 * Initialize the RAS profile. 
	 */

	//ComPortFinder_3GDevices(this->mCommonInfo.com_port);
	bool comPortFound = false;
	for(unsigned int i=0;i<this->ThreeGNMInfo.VidPidCount;i++)
	{
		string vidPid = this->ThreeGNMInfo.VidPidList[i];
		if(ComPortFinder(this->mCommonInfo.com_port,(char*)vidPid.c_str()))
		{
			comPortFound = true;
			break;
		}

	}

	this->mCommonInfo.dalMode = 2;
	this->mCommonInfo.isEmulatorMode = false;
	this->mCommonInfo.device_status = 0;

	DAL_STATUS InitStatus = mNMServer->getDal()->uconnInit(this);
	if(InitStatus == DAL_SUCCESS)
	{
		this->mCommonInfo.device_status = 1;

		/* Post event to the GUI to decide if settings need to be shown */
		//UNMShowSettings* threeGSettings = new UNMShowSettings(true, 
		//	this->mNMServer->getSM()->mThreeG_NM->getTechID());
		//this->mNMServer->getSM()->mThreeG_NM->postEventToBearerManager((UEvent*)threeGSettings);
	
		UThreeGDal* dal_3g = (UThreeGDal*)mNMServer->getDal();

		dal_3g->ATCommandEchoOff();
		dal_3g->ATCommandSetFunctionality();
		dal_3g->uconnPowerStatusQuery();
	}
	else 
	{
		this->mCommonInfo.device_status = 0;
		return false;
	}

	U_DWORD ReturnFromInitRas;
	ReturnFromInitRas = ThreeGRasConnection.InitRas(
				mCommonInfo.rasProfile, 
				mCommonInfo.rasUserName, 
				mCommonInfo.rasPassword, 
				mCommonInfo.rasPhoneNumber);
	if( ReturnFromInitRas == RAS_SUCCESS)
	{
		LOG(ULOG_DEBUG_LEVEL,"Ras Init called sucessfuly\n");
	}
	else
	{
		LOG(ULOG_DEBUG_LEVEL,"Init Ras failed.\n");
		HandleRasError(ReturnFromInitRas);
	}
	return true;
};


/***********************************************************************
*	DisableThreeGNM -
*	function called from the 3G SM to disable the ThreeG_NM
*
*	Parameters - 
*	None
***********************************************************************/
bool UThreeG_NM::DisableThreeGNM()
{
	if ( ( ThreeGRasConnection.GetRasisDialing() == true ) || 
		 ( ThreeGRasConnection.GetRasisConnected() == true ) )
	{
		// If Ras is either connected or dialling we need to disconnect it 
		ThreeGRasConnection.HangUpRas();

		// Since no more connection, reset all the connection related data. 

	}
	if ( NetworkSearchOn == true )
	{
		// How to stop the network search?? 
		// Maybe store somewhere that network info to not be sent to the BM
	}

	DAL_STATUS InitStatus = mNMServer->getDal()->uconnATResetDevice();
	if(InitStatus == DAL_SUCCESS)
	{
		this->mCommonInfo.device_status = 0;
	}

	ThreeGNMInfo.stats.cinr = 0;
	ThreeGNMInfo.stats.rssi = 0;
	ThreeGNMInfo.stats.rasstats.dl_data = 0;
	ThreeGNMInfo.stats.rasstats.ul_data = 0;
	ThreeGNMInfo.stats.rasstats.duration = 0;

	return true;
}


/***********************************************************************
*	enableNetMngr -
*	function to inform the BM of state change ThreeG_NM, in case of change 
*   in state of 3G NM, 
*
*	Parameters - 
*	NewState - the new state of 3G NM. 
***********************************************************************/
void UThreeG_NM::ChangeStateAndInformBM(ENMStateForBM NewState)
{
	if (this->state != NewState)
	{
		this->state = NewState;
		UNMStateEvent* StateChange = new UNMStateEvent(THREE_G_TECH,this->state);
		postEventToBearerManager(reinterpret_cast<UEvent*>(StateChange));
	}
}

/***********************************************************************
*	DisconnectThreeGNM -
*	function called from the 3G SM to disconnect the ThreeG_NM
*
*	Parameters - 
*	None
***********************************************************************/
void UThreeG_NM::DisconnectThreeGNM()
{

	if ( ( ThreeGRasConnection.GetRasisConnected() == true) ||
		 ( ThreeGRasConnection.GetRasisDialing() == true) )
	{
		ThreeGRasConnection.HangUpRas();
		ThreeGNMInfo.stats.rasstats.dl_data = 0;
		ThreeGNMInfo.stats.rasstats.ul_data = 0;
		ThreeGNMInfo.stats.rasstats.duration = 0;
	}

	if ( this->mNMServer->getSM()->isRASDialTimerRunning ==  true )
	{
		this->mNMServer->getSM()->isRASDialTimerRunning = false;
		UKillTimer(this->mNMServer->getSM()->RASDialTimer);
		this->mNMServer->getSM()->RASDialTimer = NULL;
	}
}

/***********************************************************************
*	EncodeSearchNetworkResponse -
*	function called from the 3G SM to encode the network name string and 
*   send it to BM / GUI. 
*
*	Parameters - 
*	None
***********************************************************************/
string UThreeG_NM::EncodeSearchNetworkResponse()
{
	string encodeStr = "";

	/* Convert values into strings */

	char cNetworkName[50]; 

	sprintf(cNetworkName, "%s", ThreeGNMInfo.network_name);

	encodeStr += "NAME:";
	encodeStr += cNetworkName;
	encodeStr += "\n";

	return encodeStr;

}


/***********************************************************************
*	EncodeStatsString -
*	function called from the 3G SM to encode the 3G statistices and send it 
*   to the BM / GUI.
*
*	Parameters - 
*	None
***********************************************************************/
string UThreeG_NM::EncodeStatsString()
{
	string encodeStr = "";

	/* Convert values into strings */
	char cRssi[20], 
		 cCinr[20], 
		 cDL_data[20], 
		 cUL_data[20], 
		 cDuration[20];
	U_UINT32 hours=0, minutes=0, seconds=0, total_time=0;
	double dwlinkrate=0;
	double uplinkrate=0;

	sprintf(cRssi, "%d", ThreeGNMInfo.stats.rssi* (-1));
	sprintf(cCinr, "%d", ThreeGNMInfo.stats.cinr* (-1));

	if ( ThreeGNMInfo.stats.rasstats.duration != 0 )
	{
		dwlinkrate = ((ThreeGNMInfo.stats.rasstats.dl_data/((U_FLOAT) ThreeGNMInfo.stats.rasstats.duration/1000.0))/1024.0);
		uplinkrate = ((ThreeGNMInfo.stats.rasstats.ul_data/((U_FLOAT) ThreeGNMInfo.stats.rasstats.duration/1000.0))/1024.0);

		total_time = ThreeGNMInfo.stats.rasstats.duration/1000;
		seconds = total_time % 60;
		total_time /= 60;
		minutes = total_time % 60;
		hours = total_time / 60;

	}
	sprintf(cDL_data, "%f", dwlinkrate);
	sprintf(cUL_data, "%f", uplinkrate);
	sprintf(cDuration, "%d::%d::%d", hours, minutes, seconds );


	encodeStr += "RSSI;";
	encodeStr += cRssi;
	encodeStr += "\n";

	encodeStr += "CINR;";
	encodeStr += cCinr;
	encodeStr += "\n";

	encodeStr += "DL_Rate;";
	encodeStr += cDL_data;
	encodeStr += "\n";

	encodeStr += "UL_Rate;";
	encodeStr += cUL_data;
	encodeStr += "\n";

	encodeStr += "Duration;";
	encodeStr += cDuration;
	encodeStr += "\n";

	return encodeStr;
}


/***********************************************************************
*	getParam -
*	function called from the BM to get param from the ThreeG_NM
*
*	Parameters - 
*	paramId - param that the BM / GUI wants. 
***********************************************************************/
U_VOID UThreeG_NM::getParam(Get_NM_Param paramId)
{
	string param = "";
	switch(paramId)
	{
	case NETWORK_ACCESS_TYPE:
		{
			switch(this->ThreeGNMInfo.rat)
			{
			case E_RAT_GSM:
				{
					param += "GSM";
				}
				break;
			case E_RAT_GSM_COMPACT:
				{
					param += "GSM Compact";
				}
				break;
			case E_RAT_UTRAN:
				{
					param += "UTRAN";
				}
				break;
			case E_RAT_EGPRS:
				{
					param += "GSM w/ EGPRS";
				}
				break;
			case E_RAT_HSDPA:
				{
					param += "UTRAN w/ HSDPA";
				}
				break;
			case E_RAT_HSUPA:
				{
					param += "UTRAN w/ HSUPA";
				}
				break;
			case E_RAT_UTRAN_HSDPA_HSUPA:
				{
					param += "UTRAN w/ HSDPA & HSUPA";
				}
				break;
			case E_RAT_EUTRAN:
				{
					param += "E-UTRAN";
				}
				break;
			}
		}
		break;
	case NETWORK_NAME:
		{
			param += ThreeGNMInfo.network_name;
		}
		break;
	case VERSION:
		{
			param += mCommonInfo.versionInfo.u8VersionString;
		}
		break;
	default :
		break;
	}

	UBearerGetParamRsp* paramRsp = new UBearerGetParamRsp(THREE_G_TECH, paramId, param);
	postEventToBearerManager(reinterpret_cast<UEvent*>(paramRsp));
}


/***********************************************************************
*	HandleRasTimer -
*	function called from the 3G SM to handle the 3G RAS Timer.
*   In case of RAS connection has not moved into RASCS_Connected state, 
*   then move into NOT_READY, so that BM can initate connection to some
*   other available technology and send the AT command to network to check
*   the network status. If the network status is CGREG=1 or 5, then again a 
*   READY is sent to BM and BM will send one more Connect to re-initiate the 
*   RAS connection. 
*
*	Parameters - 
*	None
***********************************************************************/
U_VOID UThreeG_NM::HandleRasTimer()
{
#ifdef WIN32
	if ( ( ThreeGRasConnection.GetRasisConnected() == true) &&
		 ( ThreeGRasConnection.GetRasConnectionStatus() == RASCS_Connected ))
	{
		ChangeStateAndInformBM(NM_CONNECTED_STATE);
	}
	else 
	{
		ThreeGRasConnection.HangUpRas();
		ChangeStateAndInformBM(NM_NOT_READY_STATE);
		if (ThreeGRasConnection.RasError != RAS_SUCCESS)
		{
			HandleRasError(ThreeGRasConnection.RasError);
		}
		UThreeGDal* dal_3g = (UThreeGDal*)mNMServer->getDal();
		dal_3g->ATCommandRegisterNetworkStatus();
	}
	this->mNMServer->getSM()->isRASDialTimerRunning = false;
	UKillTimer(this->mNMServer->getSM()->RASDialTimer);
	this->mNMServer->getSM()->RASDialTimer = NULL;
#endif
}

/***********************************************************************
*	HandleUSSDTimer -
*	function called from the 3G SM to handle the USSD Timer.
*   In case No Response is received from the network this timer expires
*   and a message is sent to the GUI to enable the sending of next USSD message.
*
*	Parameters - 
*	None
***********************************************************************/
U_VOID UThreeG_NM::HandleUSSDTimer()
{
	char* in_ussd = "No Response of previous USSD string from Network";
	unsigned int ret_len;
	int TmpSize = strlen(in_ussd);

	if ( ThreeGNMInfo.UssdDecodingReqd == false )
	{
		UNMUSSDToGUI* USSDStr = new UNMUSSDToGUI(U_STRING(reinterpret_cast<const char*>(in_ussd)));
		postEventToBearerManager((UEvent*)(USSDStr));
	}
	else
	{
		const unsigned char* decoded_ussd = ussd_decode(in_ussd, &ret_len, (TmpSize+1));
		if ( decoded_ussd != NULL )
		{
			/* Format a message and send it to GUI */
			//ucmsm_p->mThreeG_NM->EncodeUSSDString((unsigned char*)(decoded_ussd));

			UNMUSSDToGUI* USSDStr = new UNMUSSDToGUI(U_STRING(reinterpret_cast<const char*>(decoded_ussd)));
			postEventToBearerManager((UEvent*)(USSDStr));
			free((char*)decoded_ussd);
		}
	}

	this->mNMServer->getSM()->isUSSDTimerRunning = false;
	UKillTimer(this->mNMServer->getSM()->USSDTimer);
	this->mNMServer->getSM()->USSDTimer = NULL;
}

/***********************************************************************
*	Handle3GStatsTimerExpiry -
*	function called from the 3G SM to handle the 3G statistics Timer
*   Expiry. 
*   In case the RASCS_Disconnected is returned from the RAS COnnection
*   Status, and the 3G NM param still hold the value that RAS connection 
*   is ongoing, HangUpRas is called to Hangup the RAS and Update all the 
*   global variables accordingly. This is to take care of the scenaio
*   when RAS connection is lost and 3G NM is not aware of it. 
*   to the BM / GUI.
*
*	Parameters - 
*	None
***********************************************************************/
U_VOID UThreeG_NM::Handle3GStatsTimerExpiry()
{
    #ifdef WIN32
	UThreeGDal* dal_3g = (UThreeGDal*)mNMServer->getDal();
	dal_3g->ATCommandGetSignalStrength();
	if ( ( ThreeGRasConnection.GetRasisConnected() == true) &&
		 ( ThreeGRasConnection.GetRasConnectionStatus() == RASCS_Connected ))
	{
		ChangeStateAndInformBM(NM_CONNECTED_STATE);
		ThreeGRasConnection.GetRasConnectionStatistics(&(ThreeGNMInfo.stats.rasstats));
	}
	else if ( ThreeGRasConnection.GetRasConnectionStatus() == RASCS_Disconnected )
	{
		if ( ( ThreeGRasConnection.GetRasisDialing() == false ) &&
			 ( ThreeGRasConnection.GetRasisConnected() == true ) &&
			 ( ThreeGRasConnection.URasConnHandle != NULL ) )
		{
			ThreeGRasConnection.HangUpRas();
			this->state = NM_NOT_READY_STATE;
			UThreeGDal* dal_3g = (UThreeGDal*)mNMServer->getDal();
			dal_3g->ATCommandRegisterNetworkStatus();
		}
	}
	string encodeStr = this->EncodeStatsString();

	UNMThreeGStats* ThreeG_Stats = new UNMThreeGStats(encodeStr);
	this->postEventToBearerManager((UEvent*)(ThreeG_Stats));
	
	UKillTimer(this->mNMServer->getSM()->ThreeGStatsTimer);
	this->mNMServer->getSM()->ThreeGStatsTimer = NULL;
		
	this->mNMServer->getSM()->ThreeGStatsTimer = USetTimer_NM(5000, 0,
			this->mNMServer, (U_CALLBACK)TimerCallBack_NM);
#endif
    
}

/***********************************************************************
*	Update3GParams -
*	function called from the 3G SM to Update the 3G params depending upon 
*   the Responses received from the 3G Parser. 
*
*	Parameters - 
*	None
***********************************************************************/
U_VOID UThreeG_NM::Update3GParams(U_UINT32 ParamId, U_UINT32 value1, U_UINT32 value2)
{
	switch (ParamId)
	{
		case UCONN_EVT_NM_RSSI_UPDATE:
			{
				ThreeGNMInfo.stats.rssi = value1;
			}	
			break;
		case UCONN_RSP_NM_RSSI_CINR_UPDATE:
			{
				ThreeGNMInfo.stats.rssi = value1 ;
				ThreeGNMInfo.stats.cinr = value2 ;
			}
			break;
		case UCONN_EVT_NM_MODEM_STATE_UPDATE:
			{
				ThreeGNMInfo.reg_status = value1; 
			}
		case UCONN_EVT_NM_MODEM_STATE_NETWORK_ACCESS_UPDATE:
			{
				ThreeGNMInfo.reg_status = value1; 
				ThreeGNMInfo.rat = value2;
			}
			break;
	}

}

/***********************************************************************
*	Update3GNMState -
*	function called from the 3G SM to Update the 3G state, if reqd and inform 
*   to the BM / GUI.
*
*	Parameters - 
*	None
***********************************************************************/
U_VOID UThreeG_NM::Update3GNMState()
{
    #ifdef WIN32
	if ( ( ThreeGRasConnection.GetRasConnectionStatus() == RASCS_Disconnected) )
	{
		if ( ( ThreeGRasConnection.GetRasisConnected() == false ) &&
			 ( ThreeGRasConnection.GetRasisDialing() == false ) )
		{
//			if ( ( ThreeGNMInfo.reg_status == 1 ) ||
//					 ( ThreeGNMInfo.reg_status == 5 ) )
//			{
				ChangeStateAndInformBM(NM_READY_STATE);
//			}
//			else 
//			{
//				ChangeStateAndInformBM(NM_NOT_READY_STATE);
//			}
		}
	}
#endif
}

/***********************************************************************
*	HandleError -
*	function for handling the errors during RAS connection. 
*
*	Parameters - 
*	ErrorCode - contains the Error code returned from the RasDial function API. 
***********************************************************************/
void UThreeG_NM::HandleRasError (U_UINT32 ErrorCode)
{
	
#ifdef WIN32

	U_UINT32 errorCode = ErrorCode;
	string ErrorStr = "";
	switch(ErrorCode)
	{
		case ERROR_CANNOT_FIND_PHONEBOOK_ENTRY:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_CANNOT_FIND_PHONEBOOK_ENTRY");
				ErrorStr += "The system could not find the phone book entry for this connection. Try Redialling, by doing a disable followed by enable for this tecnology.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_PORT_ALREADY_OPEN:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_PORT_ALREADY_OPEN");
				ErrorStr += "The port is already open. Try Plugging out and plugging in the device again.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR ,errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_BUFFER_TOO_SMALL:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_BUFFER_TOO_SMALL");
				ErrorStr += "Unable to Initialize Profile.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR,errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_PORT_NOT_OPEN:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_PORT_NOT_OPEN");
				ErrorStr += "The port is not open. Try Plugging out and plugging in the device again.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_PORT_DISCONNECTED:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_PORT_DISCONNECTED");
				ErrorStr += "The port is disconnected. Try Plugging out and plugging in the device again.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_REMOTE_DISCONNECTION:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_REMOTE_DISCONNECTION");
				ErrorStr += "The port was disconnected by the remote machine.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_PORT_NOT_AVAILABLE:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_PORT_NOT_AVAILABLE");
				ErrorStr += "The port is already in use or is not configured for Remote Access dialout. Try Plugging out and plugging in the device again.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_RESTRICTED_LOGON_HOURS:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_RESTRICTED_LOGON_HOURS");
				ErrorStr += "The account is not permitted to log on at this time of day.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, ErrorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_ACCT_DISABLED:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_ACCT_DISABLED");
				ErrorStr += "The account is disabled.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_PASSWD_EXPIRED:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_PASSWD_EXPIRED");
				ErrorStr += "The password for this account has expired.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_NO_DIALIN_PERMISSION:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_NO_DIALIN_PERMISSION");
				ErrorStr += "The account does not have permission to dial in";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_FROM_DEVICE:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_FROM_DEVICE");
				ErrorStr += "The modem (or other connecting device) has reported an error. Try Plugging out and plugging in the device again.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_PORT_NOT_CONFIGURED:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_PORT_NOT_CONFIGURED");
				ErrorStr += "The modem (or other connecting device) is not properly configured. Try Plugging out and plugging in the device again.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_DEVICE_NOT_READY:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_DEVICE_NOT_READY");
				ErrorStr += "The modem (or other connecting device) is not functioning. Try Plugging out and plugging in the device.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_NO_CONNECTION:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_NO_CONNECTION");
				ErrorStr += "The connection was terminated. ";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_LINE_BUSY:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_LINE_BUSY");
				ErrorStr += "The phone line is busy. Try Redialling, by doing a disable followed by enable for this tecnology.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_VOICE_ANSWER:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_VOICE_ANSWER");
				ErrorStr += "A person answered instead of a modem (or other connecting device). Try Redialling, by doing a disable followed by enable for this tecnology.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_NO_ANSWER:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_NO_ANSWER");
				ErrorStr += "The remote computer did not respond.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_NO_CARRIER:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_NO_CARRIER");
				ErrorStr += "The system could not detect the carrier.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_NO_DIALTONE:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_NO_DIALTONE");
				ErrorStr += "There was no dial tone. Try after some time.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_AUTHENTICATION_FAILURE:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_AUTHENTICATION_FAILURE");
				ErrorStr += "The connection was denied because the username and/or password you specified is invalid. This could be caused by the following conditions: Your username and/or password was mis-typed. The specified username does not exist on the server. Your password has expired. The administrator has not given you access to connect remotely. The selected authentication protocol is not permitted on the remote server.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_ACCT_EXPIRED:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_ACCT_EXPIRED");
				ErrorStr += "The account has expired.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_CHANGING_PASSWORD:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_CHANGING_PASSWORD");
				ErrorStr += "There was an error changing the password on the domain.  The password might have been too short or might have matched a previously used password.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_PPP_TIMEOUT:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_PPP_TIMEOUT");
				ErrorStr += "The connection was terminated because the remote computer did not respond in a timely manner.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_PPP_NO_PROTOCOLS_CONFIGURED:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_PPP_NO_PROTOCOLS_CONFIGURED");
				ErrorStr += "A connection to the remote computer could not be established.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_PPP_NO_RESPONSE:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_PPP_NO_RESPONSE");
				ErrorStr += "The remote computer did not respond.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_PPP_CP_REJECTED:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_PPP_CP_REJECTED");
				ErrorStr += "A connection to the remote computer could not be completed.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_PPP_LCP_TERMINATED:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_PPP_LCP_TERMINATED");
				ErrorStr += "The PPP link control protocol was terminated.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_PPP_REQUIRED_ADDRESS_REJECTED:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_PPP_REQUIRED_ADDRESS_REJECTED");
				ErrorStr += "The requested address was rejected by the server.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_PPP_NCP_TERMINATED:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_PPP_NCP_TERMINATED");
				ErrorStr += "The remote computer terminated the control protocol.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_CANNOT_USE_LOGON_CREDENTIALS:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_CANNOT_USE_LOGON_CREDENTIALS");
				ErrorStr += "The authentication protocol required by the remote server cannot use the stored password.  Redial, entering the password explicitly.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_BAD_PHONE_NUMBER:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_BAD_PHONE_NUMBER. Try entering the right phone number.");
				ErrorStr += "ERROR_BAD_PHONE_NUMBER";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_DIAL_ALREADY_IN_PROGRESS:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_DIAL_ALREADY_IN_PROGRESS");
				ErrorStr += "This connection is already being dialed. Try Plugging out and plugging in the device.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_CONNECTION_REJECT:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_CONNECTION_REJECT");
				ErrorStr += "The remote computer rejected the connection attempt.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_CONGESTION:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_CONGESTION");
				ErrorStr += "The connection attempt failed because the network is busy.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE,errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_TEMPFAILURE:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_TEMPFAILURE");
				ErrorStr += "The connection attempt failed because of a temporary failure.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_BLOCKED:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_BLOCKED");
				ErrorStr += "The call was blocked by the remote computer. Try Redialling, by doing a disable followed by enable for this tecnology.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_DONOTDISTURB:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_DONOTDISTURB");
				ErrorStr += "The call could not be connected because the remote computer has invoked the Do Not Disturb feature. Try Redialling, by doing a disable followed by enable for this tecnology.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_OUTOFORDER:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_OUTOFORDER");
				ErrorStr += "The connection attempt failed because the modem (or other connecting device) on the remote computer is out of order. Try Redialling, by doing a disable followed by enable for this tecnology.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_UNABLE_TO_AUTHENTICATE_SERVER:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_UNABLE_TO_AUTHENTICATE_SERVER");
				ErrorStr += "It was not possible to verify the identity of the server. Try Redialling, by doing a disable followed by enable for this tecnology.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_CONNECTING_DEVICE_NOT_FOUND:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_CONNECTING_DEVICE_NOT_FOUND");
				ErrorStr += "A connection to the remote computer could not be established because the modem was not found or was busy.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_IDLE_TIMEOUT:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_IDLE_TIMEOUT");
				ErrorStr += " The connection was terminated because of idle timeout.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_LINK_FAILURE:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_LINK_FAILURE");
				ErrorStr += "The modem (or other connecting device) was disconnected due to link failure.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_USER_LOGOFF:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_USER_LOGOFF");
				ErrorStr += "The connection was terminated because user logged off. Try Redialling, by doing a disable followed by enable for this tecnology. ";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_FAST_USER_SWITCH:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_FAST_USER_SWITCH");
				ErrorStr += "The connection was terminated because user switch happened. Try Redialling, by doing a disable followed by enable for this tecnology. ";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_HIBERNATION:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_HIBERNATION");
				ErrorStr += "The connection was terminated because of hibernation.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
		case ERROR_SYSTEM_SUSPENDED:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_SYSTEM_SUSPENDED");
				ErrorStr += "The connection was terminated because the system got suspended.";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_INFORMATIVE, errorCode);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break;
	/*	case ERROR_ACCT_EXPIRED:
			{
				LOG(ULOG_INFO_LEVEL,"ERROR_ACCT_EXPIRED");
				ErrorStr += "abc";
				UNMErrorDisplayEvt* ErrorDisplayToGui = new UNMErrorDisplayEvt(THREE_G_TECH, ErrorStr, NM_REPORT_ERROR);
				postEventToBearerManager((UEvent*)(ErrorDisplayToGui));
			}
			break; */
		default:
			break;
	}
#endif
}


/* functions of Ras class */
/***********************************************************************
*	URasConnection -
*	constructor of URasConnection.
*	All initialization of RAS connection needs to be done here. 
*	Parameters - 
*	None
***********************************************************************/
URasConnection::URasConnection()
{
	RasEntriesNames = NULL;
	
	rasstatistics.dl_data = 0;
	rasstatistics.ul_data = 0;
	rasstatistics.duration = 0;
}


URasConnection::~URasConnection()
{
}

/***********************************************************************
*	InitRas -
*	function to initialize the RAS connection. 
*
*	Parameters - 
*	None
***********************************************************************/
U_DWORD URasConnection::InitRas(U_CSTR rasName, U_CSTR rasusrname, U_CSTR raspasswd, U_CSTR PhoneNo)
{
	/* Find the RAS enteries and initialize corresponding variables */
#ifdef WIN32
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	RASENTRY rasEntry;
	DWORD rasEntryInfoSize;
	DWORD dwCb = 0, i = 0;
	DWORD dwRet = ERROR_SUCCESS;
	DWORD dwEntries = 0;
	LPRASENTRYNAME lpRasEntryName = NULL;
	unsigned char tmpStr[100];
	WCHAR userRASProfile[100] = {0};
	size_t convSize = 0;

	URasIsConnected = false;
	URasIsDialing = false;
	URasConnHandle = NULL;
	RasEntriesNames = NULL;
	RasError = RAS_SUCCESS;

	if ( !(strlen(rasName)))
	{
		return ( CreateRasProfile (rasName, rasusrname, raspasswd, PhoneNo ));
		
	}
	if(osvi.dwMajorVersion > 5) 
	{
		//This code way under Windows Vista and Windows 7

		// Call RasEnumEntries with lpRasEntryName = NULL. 
		// dwCb is returned with the required buffer size & return code of ERROR_BUFFER_TOO_SMALL

		dwRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &dwCb, &dwEntries);

		if (dwRet == ERROR_BUFFER_TOO_SMALL)
		{
			// Allocate the memory needed for the array of RAS entry names.
			lpRasEntryName = (LPRASENTRYNAME) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);
			if (lpRasEntryName == NULL)
			{
				LOG(ULOG_DEBUG_LEVEL,"HeapAlloc failed for RAS entry name.\n");
				/* Error : */
				return dwRet;
			}
			// The first RASENTRYNAME structure in the array must contain the structure size
			lpRasEntryName[0].dwSize = sizeof(RASENTRYNAME);

			// Call RasEnumEntries to enumerate all RAS entry names
			dwRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &dwCb, &dwEntries);

			// If successful, print the RAS entry names 
			if (ERROR_SUCCESS == dwRet)
			{
				mbstowcs_s(&convSize, userRASProfile, 2 * (strlen(rasName) + 1), 
					rasName, strlen(rasName));
				for (i = 0; i < dwEntries; i++)
				{
					if(!_wcsicmp(lpRasEntryName[i].szEntryName, userRASProfile))
					{
						wCharToUCharString(lpRasEntryName[i].szEntryName, tmpStr);
						RasEntriesNames = (unsigned char*)_strdup((const char*)tmpStr);
					}
				}
			}


			//Deallocate memory for the connection buffer
			HeapFree(GetProcessHeap(), 0, lpRasEntryName);
			lpRasEntryName = NULL;

			if ( RasEntriesNames == NULL )
			{
				// If user profile is not available, create one. 
				return (CreateRasProfile(rasName, rasusrname, raspasswd, PhoneNo));
			}
			return RAS_SUCCESS;
		}

		// There was either a problem with RAS or there are no RAS entry names to enumerate    
		if(dwEntries >= 1)
		{
			LOG(ULOG_DEBUG_LEVEL,"The operation failed to acquire the buffer size for RAS entry names.\n");
			/* Error */
		}
		else
		{
			return ( CreateRasProfile (rasName, rasusrname, raspasswd, PhoneNo ));
			LOG(ULOG_DEBUG_LEVEL,"There were no Address book entries found for RAS entry name.\n");
			/* Warning */
		}

		return dwRet;
	}
	else
	{
		//Windows XP 
		dwCb = sizeof(RASENTRYNAME);

		//lpRasEntryName should be allocated and intialized with some size
		lpRasEntryName = (LPRASENTRYNAME) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb); 
		lpRasEntryName[0].dwSize = sizeof(RASENTRYNAME);

		// Call RasEnumEntries with lpRasEntryName = NULL. dwCb is returned with the required buffer size and 
		// a return code of ERROR_BUFFER_TOO_SMALL
		dwRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &dwCb, &dwEntries);

		if (dwRet == ERROR_BUFFER_TOO_SMALL || dwRet == 0)
		{
			//Now we know the right size free what is already allocated
			HeapFree(GetProcessHeap(), 0, lpRasEntryName);
			lpRasEntryName = NULL;

			//Now allocate the correct size
			lpRasEntryName = (LPRASENTRYNAME) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);
			if (lpRasEntryName == NULL)
			{
				LOG(ULOG_DEBUG_LEVEL,"HeapAlloc failed for RAS entry name.\n");
				return dwRet;
			}

			// The first RASENTRYNAME structure in the array must contain the structure size
			lpRasEntryName[0].dwSize = sizeof(RASENTRYNAME);

			// Call RasEnumEntries to enumerate all RAS entry names
			dwRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &dwCb, &dwEntries);

			// If successful, print the RAS entry names 
			if (ERROR_SUCCESS == dwRet)
			{
				for (i = 0; i < dwEntries; i++)
				{
					memset(&rasEntry, 0, sizeof(rasEntry));
					rasEntry.dwSize = sizeof(rasEntry);
					rasEntryInfoSize = sizeof(rasEntry);
					BYTE bDeviceInfo = NULL;
					DWORD dwDeviceInfoSize = sizeof(bDeviceInfo);

					dwRet = RasGetEntryProperties(NULL, NULL, NULL,
						(LPDWORD )&rasEntryInfoSize, &bDeviceInfo, &dwDeviceInfoSize);
					
					rasEntry.dwSize = rasEntryInfoSize;
					dwRet = RasGetEntryProperties(NULL, lpRasEntryName[i].szEntryName, (LPRASENTRY)&rasEntry,
						(LPDWORD )&rasEntryInfoSize, &bDeviceInfo, &dwDeviceInfoSize);

					if ( dwRet != ERROR_SUCCESS)
					{
						// Getting Ras enteries failed, so log an error.
						LOG(ULOG_ERROR_LEVEL,"RasGetEntryProperties Failed with %d\n",
							dwRet);
					}
					else
					{
						mbstowcs_s(&convSize, userRASProfile, 2 * (strlen(rasName) + 1), 
							rasName, strlen(rasName));
						if(!_wcsicmp(lpRasEntryName[i].szEntryName, userRASProfile))
						{
							wCharToUCharString(lpRasEntryName[i].szEntryName, tmpStr);
							RasEntriesNames = (unsigned char*)_strdup((const char*)tmpStr);
						}
					}
				}
			}
			//Deallocate memory for the connection buffer
			HeapFree(GetProcessHeap(), 0, lpRasEntryName);
			lpRasEntryName = NULL;
		
			if ( RasEntriesNames == NULL )
			{
				// If user profile is not available, create one. 
				return( CreateRasProfile(rasName, rasusrname, raspasswd, PhoneNo));
			}

			return RAS_SUCCESS;
		}

		// There was either a problem with RAS or there are RAS entry names to enumerate    
		if(dwEntries >= 1)
		{
			LOG(ULOG_DEBUG_LEVEL,"The operation failed to acquire the buffer size for RAS entry names.\n");
			/* Error */
		}
		else
		{
			return ( CreateRasProfile (rasName, rasusrname, raspasswd, PhoneNo ));
			LOG(ULOG_DEBUG_LEVEL,"There were no Address book entries found for RAS entry name.\n");
			/* Warning */
		}

		return dwRet;
	}
	return RAS_SUCCESS;
#elif (defined(__linux__)|| defined(__MACH__))
    return false;
#endif
}

/***********************************************************************
*	CreateRasProfile -
*	function which cretaes a RAS profile if none are existing. 
*
*	Parameters - 
*   RASENTRY   -- The RASENTRY structure describes a phone-book entry. 
*   The RasSetEntryProperties and RasGetEntryProperties functions use this 
*   structure to set and retrieve the properties of a phone-book entry. 
*   typedef struct tagRASENTRY {
*   DWORD      dwSize; Specifies the size, in bytes, of the RASENTRY structure. 
*                      Before calling RasSetEntryProperties or RasGetEntryProperties, 
*                      set dwSize to sizeof(RASENTRY) to identify the version of the structure. 
*   DWORD      dwfOptions; A set of bit flags that specify connection options. You can set one 
*                          or more of the following flags. 
*						   Flag Description - 
*							RASEO_UseCountryAndAreaCodes - If this flag is set, the dwCountryID, 
*								dwCountryCode, and szAreaCode members are used to construct the phone number. 
*								If this flag is not set, these members are ignored. This flag corresponds to 
*								the Use Country and Area Codes check box in the Phone dialog box.
*							RASEO_SpecificIpAddr - If this flag is set, RAS tries to use the IP address 
*								specified by ipaddr as the IP address for the dial-up connection. If this flag 
*								is not set, the value of the ipaddr member is ignored. Setting the 
*								RASEO_SpecificIpAddr flag corresponds to selecting the Specify an IP Address 
*								setting in the TCP/IP settings dialog box. Clearing the RASEO_SpecificIpAddr flag 
*								corresponds to selecting the Server Assigned IP Address setting in the TCP/IP 
*								settings dialog box. Currently, an IP address set in the phone-book entry properties 
*								or retrieved from a server overrides the IP address set in the network control panel. 
*							RASEO_SpecificNameServers - If this flag is set, RAS uses the ipaddrDns, ipaddrDnsAlt, 
*								ipaddrWins, and ipaddrWinsAlt members to specify the name server addresses for the 
*								dial-up connection. If this flag is not set, RAS ignores these members. Setting the 
*								RASEO_SpecificNameServers flag corresponds to selecting the Specify Name Server 
*								Addresses setting in the TCP/IP Settings dialog box. Clearing the RASEO_SpecificNameServers 
*								flag corresponds to selecting the Server Assigned Name Server Addresses setting in the TCP/IP 
*								Settings dialog box. 
*							RASEO_IpHeaderCompression - If this flag is set, RAS negotiates to use IP header compression on 
*								PPP connections. If this flag is not set, IP header compression is not negotiated. This flag 
*								corresponds to the Use IP Header Compression check box in the TCP/IP settings dialog box. 
*								It is generally advisable to set this flag because IP header compression significantly improves 
*								performance. The flag should be cleared only when connecting to a server that does not correctly 
*								negotiate IP header compression. 
*						    RASEO_RemoteDefaultGateway - If this flag is set, the default route for IP packets is through the 
*								dial-up adapter when the connection is active. If this flag is clear, the default route is not 
*								modified. This flag corresponds to the Use Default Gateway on Remote Network check box in the 
*								TCP/IP settings dialog box. 
*							RASEO_DisableLcpExtensions - If this flag is set, RAS disables the PPP LCP extensions defined in RFC 
*								1570. This may be necessary to connect to certain older PPP implementations, but interferes with 
*								features such as server callback. Do not set this flag unless specifically required. 
*							RASEO_TerminalBeforeDial - If this flag is set, RAS displays a terminal window for user input before 
*								dialing the connection. 
*							RASEO_TerminalAfterDial - If this flag is set, RAS displays a terminal window for user input after 
*								dialing the connection. Do not set this flag if a dial-up networking script is to be associated 
*								with the connection, because scripting has its own terminal implementation. 
*							RASEO_ModemLights - This flag is currently ignored. 
*							RASEO_SwCompression - If this flag is set, software compression is negotiated on the link. Setting 
*								this flag causes the PPP driver to attempt to negotiate CCP with the server. This flag should be 
*								set by default, but clearing it can reduce the negotiation period if the server does not support 
*								a compatible compression protocol. 
*							RASEO_RequireEncryptedPw - If this flag is set, only secure password schemes can be used to authenticate 
*								the client with the server. This prevents the PPP driver from using the PAP plain-text authentication 
*								protocol to authenticate the client. The CHAP and SPAP authentication protocols are also supported. 
*								Clear this flag for increased interoperability, and set it for increased security. This flag 
*								corresponds to the Require Encrypted Password check box in the Security dialog box. See also 
*								RASEO_RequireMsEncryptedPw. 
*							RASEO_RequireMsEncryptedPw - If this flag is set, only Microsoft’s secure password schemes can be used 
*								to authenticate the client with the server. This prevents the PPP driver from using the PPP plain-text 
*								authentication protocol, MD5-CHAP, MS-CHAP, or SPAP. The flag should be cleared for maximum interoperability 
*								and should be set for maximum security. This flag takes precedence over RASEO_RequireEncryptedPw. 
*								This flag corresponds to the Require Microsoft Encrypted Password check box in the Security dialog box. 
*								See also RASEO_RequireDataEncryption. 
*							RASEO_RequireDataEncryption - If this flag is set, data encryption must be negotiated successfully or the connection 
*								should be dropped. This flag is ignored unless RASEO_RequireMsEncryptedPw is also set. This flag 
*								corresponds to the Require Data Encryption check box in the Security dialog box. 
*							RASEO_NetworkLogon - If this flag is set, RAS logs on to the network after the point-to-point connection is established. 
*								This flag currently has no effect under Windows NT. 
*							RASEO_UseLogonCredentials - If this flag is set, RAS uses the user name, password, and domain of the currently logged-on 
*								user when dialing this entry. This flag is ignored unless RASEO_RequireMsEncryptedPw is also set. Note that this 
*								setting is ignored by the RasDial function, where specifying empty strings for the szUserName and szPassword members 
*								of the RASDIALPARAMS structure gives the same result. This flag corresponds to the Use Current Username and Password 
*								check box in the Security dialog box. 
*							RASEO_PromoteAlternates - This flag has an effect when alternate phone numbers are defined by the dwAlternateOffset member. 
*								If this flag is set, an alternate phone number that connects successfully becomes the primary phone number, and the 
*								current primary phone number is moved to the alternate list. This flag corresponds to the check box in the Alternate 
*								Numbers dialog box.
*							RASEO_SecureLocalFiles - Windows NT only: If this flag is set, RAS checks for existing remote file system and remote printer 
*							bindings before making a connection with this entry. Typically, you set this flag on phone-book entries for public networks 
*							to remind users to break connections to their private network before connecting to a public network. 
*   Location/phone number.
*   DWORD      dwCountryID;		Specifies the TAPI country identifier. Use the RasGetCountryInfo function to enumerate country identifiers. This member is 
*								ignored unless the dwfOptions member specifies the RASEO_UseCountryAndAreaCodes flag. 
*   DWORD      dwCountryCode;	Specifies the country code portion of the phone number. The country code must correspond to the country identifier specified 
*								by dwCountryID. If dwCountryCode is zero, the country code is based on the country identifier specified by dwCountryID. This 
*								member is ignored unless dwfOptions specifies the RASEO_UseCountryAndAreaCodes flag. 
*   TCHAR      szAreaCode[ RAS_MaxAreaCode + 1 ];	Specifies the area code as a null-terminated string. If the dialing location does not have an area code, 
*													specify an empty string (““). Do not include brackets or other delimiters in the area code string. (For 
*													example, “206” is a valid area code; “(206)” is not. This member is ignored unless the dwfOptions member 
*													specifies the RASEO_UseCountryAndAreaCodes flag. 
*   TCHAR      szLocalPhoneNumber[ RAS_MaxPhoneNumber + 1 ];	Specifies a null-terminated string containing a telephone number. The way RAS uses this string 
*																depends on whether the dwfOptions member specifies the RASEO_UseCountryAndAreaCodes flag. If 
*																the flag is set, RAS combines szLocalPhoneNumber with the country and area codes specified by 
*																the dwCountryID, dwCountryCode , and szAreaCode members. If the flag is not set, RAS uses the 
*																szLocalPhoneNumber string as the entire phone number. 
*   DWORD      dwAlternateOffset;	Specifies the offset, in bytes, from the beginning of the structure to a list of consecutive null-terminated strings. The 
*									last string is terminated by two consecutive null characters. The strings are alternate phone numbers that RAS dials in 
*									the order listed if the primary number (see szLocalPhoneNumber) fails to connect. The alternate phone number strings are 
*									ANSI or Unicode, depending on whether you use the ANSI or Unicode version of the structure. 
*  PPP/Ip Information -
*  RASIPADDR  ipaddr;			Specifies the IP address to be used while this connection is active. This member is ignored unless dwfOptions specifies the 
*								RASEO_SpecificIpAddr flag. 
*  RASIPADDR  ipaddrDns;		Specifies the IP address of the DNS server to be used while this connection is active. This member is ignored unless dwfOptions 
*								specifies the RASEO_SpecificNameServers flag. 
*  RASIPADDR  ipaddrDnsAlt;		Specifies the IP address of a secondary or backup DNS server to be used while this connection is active. This member is ignored 
*								unless dwfOptions specifies the RASEO_SpecificNameServers flag. 
*  RASIPADDR  ipaddrWins;		Specifies the IP address of the WINS server to be used while this connection is active. This member is ignored unless dwfOptions 
*								specifies the RASEO_SpecificNameServers flag. 
*  RASIPADDR  ipaddrWinsAlt;	Specifies the IP address of a secondary WINS server to be used while this connection is active. This member is ignored unless 
*								dwfOptions specifies the RASEO_SpecificNameServers flag. 
*  Framing Information -
*  DWORD      dwFrameSize;		Specifies the network protocol frame size. The value should be either 1006 or 1500. This member is ignored unless dwFramingProtocol 
*								specifies the RASFP_Slip flag. 
*  DWORD      dwfNetProtocols;	Specifies the network protocols to negotiate. This member can be a combination of the following flags. 
*								Flag Description - 
*								RASNP_Netbeui - Negotiate the NetBEUI protocol. 
*								RASNP_Ipx - Negotiate the IPX protocol. 
*								RASNP_Ip - Negotiate the TCP/IP protocol. 
*  DWORD dwFramingProtocol;		Specifies the framing protocol used by the server. PPP is the emerging standard. SLIP is used mainly in 
*								UNIX environments. This member can be one of the following flags. 
								Flag Description - 
								RASFP_Ppp - Point-to-Point Protocol (PPP) 
								RASFP_Slip - Serial Line Internet Protocol (SLIP)
								RASFP_Ras - Microsoft proprietary protocol implemented in Windows NT 3.1 and Windows for Workgroups 3.11 
								To use Compressed SLIP, set the RASFP_Slip flag and set the RASEO_IpHeaderCompression flag in the dwfOptions member. 
*  Scripting Information -
*  TCHAR      szScript[ MAX_PATH ];	Specifies a null-terminated string containing the name of the script file. The filename should be a full path. 
*									Windows NT: To indicate a Windows NT SWITCH.INF script name, set the first character of the name to “[“. 
*  AutoDial Information -
*  TCHAR       szAutodialDll[ MAX_PATH ];	Specifies a null-terminated string containing the full path and filename of the dynamic-link library (DLL) 
*											for the customized AutoDial handler. If szAutodialDll contains an empty string (““), RAS uses the default 
*											dialing user interface and the szAutodialFunc member is ignored. 
*  TCHAR       szAutodialFunc[ MAX_PATH ];	Specifies a null-terminated string containing the exported name of the RASADFunc function for the customized 
*											AutoDial handler. An AutoDial DLL must provide both ANSI and Unicode versions of the RASADFunc handler. However,
*											do not include the “A” or “W” suffix in the name specified by szAutodialFunc. 
*  Device Information -
*  TCHAR      szDeviceType[ RAS_MaxDeviceType + 1 ];	Specifies a null-terminated string indicating the RAS device type referenced by szDeviceName. 
*														This member can be one of the following string constants. 
*														String Description - 
*														RASDT_Modem - A modem accessed through a COM port. 
*														RASDT_Isdn - An ISDN card with corresponding NDISWAN driver installed. 
*														RASDT_X25 - An X.25 card with corresponding NDISWAN driver installed. 
*  TCHAR      szDeviceName[ RAS_MaxDeviceName + 1 ];	Contains a null-terminated string containing the name of a TAPI device to use with this phone-book entry. 
*														To enumerate all available RAS-capable devices, use the RasEnumDevices function. 
*  X.25 Information -
*  TCHAR      szX25PadType[ RAS_MaxPadType + 1 ];		Contains a null-terminated string that identifies the X.25 PAD type. Set this member to ““ unless the entry 
*														should dial using an X.25 PAD. 
*														Windows NT: Under Windows NT, the szX25PadType string maps to a section name in PAD.INF. 
*  TCHAR      szX25Address[ RAS_MaxX25Address + 1 ];	Contains a null-terminated string that identifies the X.25 address to connect to. Set this member to ““ 
*														unless the entry should dial using an X.25 PAD or native X.25 device. 
*  TCHAR      szX25Facilities[ RAS_MaxFacilities + 1 ];	Contains a null-terminated string that specifies the facilities to request from the X.25 host at 
*														connection. This member is ignored if szX25Address is an empty string (““). 
*  TCHAR      szX25UserData[ RAS_MaxUserData + 1 ];		Contains a null-terminated string that specifies additional connection information supplied to the 
*														X.25 host at connection. This member is ignored if szX25Address is an empty string (““). 
*  DWORD      dwChannels;
*
*  Reserved Information -
*  DWORD       dwReserved1;					Reserved; must be zero. 
*  DWORD       dwReserved2;					Reserved; must be zero. 
*  #if (WINVER >= 0x401)
*
*  Multilink Information -
*  DWORD       dwSubEntries;				Specifies the number of multilink subentries associated with this entry. When calling RasSetEntryProperties, 
*											set this member to zero. To add subentries to a phone-book entry, use the RasSetSubEntryProperties function. 
*  DWORD       dwDialMode;					Indicates whether RAS should dial all of this entry’s multilink subentries when the entry is first connected. 
*											This member can be one of the following values. 
*											Value Meaning 
*											RASEDM_DialAll - Dial all subentries initially. 
*											RASEDM_DialAsNeeded - Adjust the number of subentries as bandwidth is needed. RAS uses the dwDialExtraPercent, 
*											dwDialExtraSampleSeconds, dwDialHangUpExtraPercent, and dwHangUpExtraSampleSeconds members to determine when 
*											to dial or disconnect a subentry. 
*  DWORD       dwDialExtraPercent;			Specifies a percent of the total bandwidth available from the currently connected subentries. RAS dials an 
*											additional subentry when the total bandwidth used exceeds dwDialExtraPercent percent of the available 
*											bandwidth for at least dwDialExtraSampleSeconds seconds. 
*											This member is ignored unless the dwDialMode member specifies the RASEDM_DialAsNeeded flag. 
*  DWORD       dwDialExtraSampleSeconds;	Specifies the number of seconds that current bandwidth usage must exceed the threshold specified by 
*											dwDialExtraPercent before RAS dials an additional subentry. 
*											This member is ignored unless the dwDialMode member specifies the RASEDM_DialAsNeeded flag. 
*  DWORD       dwHangUpExtraPercent;		Specifies a percent of the total bandwidth available from the currently connected subentries. 
*											RAS terminates (hangs up) an existing subentry connection when total bandwidth used is less than 
*											dwHangUpExtraPercent percent of the available bandwidth for at least dwHangUpExtraSampleSeconds seconds. 
*											This member is ignored unless the dwDialMode member specifies the RASEDM_DialAsNeeded flag. 
*  DWORD       dwHangUpExtraSampleSeconds;	Specifies the number of seconds that current bandwidth usage must be less than the threshold specified by 
*											dwHangUpExtraPercent before RAS terminates an existing subentry connection. 
*											This member is ignored unless the dwDialMode member specifies the RASEDM_DialAsNeeded flag. 
*  Idle timeout Information -
*  DWORD       dwIdleDisconnectSeconds;		Specifies the number of seconds after which the connection is terminated due to inactivity. Note that unless 
*											the idle timeout is disabled, the entire connection is terminated if the connection is idle for the specified 
*											interval. This member can specify a number of seconds, or one of the following values. 
*											Value Meaning 
*											RASIDS_Disabled - There is no idle timeout for this connection. 
*											RASIDS_UseGlobalValue - Use the user preference value as the default.
*  #endif
*  } RASENTRY;
* 
*	None
***********************************************************************/
U_DWORD URasConnection::CreateRasProfile(U_CSTR rasName, U_CSTR rasusrname, U_CSTR raspasswd, U_CSTR PhoneNo)
{
	::Sleep(1500);
	LPRASENTRY lpRasEntry = NULL;
    DWORD cb = sizeof(RASENTRY);
	DWORD dwRet;
	DWORD dwBufferSize = 1;

	char* strEntryName ;


	// This is important! Find the buffer size (different from sizeof(RASENTRY)).
	DWORD dwEntrySize = 0;
	BYTE bDeviceInfo = NULL;
	DWORD dwDeviceInfoSize = sizeof(bDeviceInfo);
			
	RASENTRY RasEntry = 
	{
		sizeof(RASENTRY)
	};
	memset(&RasEntry, 0, sizeof(RASENTRY ));
	
	dwEntrySize = 0 ;

	dwRet = RasGetEntryProperties(
			NULL, 
			NULL, 
			NULL, 
			&dwEntrySize, 
			&bDeviceInfo, 
			&dwDeviceInfoSize);

	RasEntry.dwSize = dwEntrySize;  
	dwRet = RasGetEntryProperties(
			NULL, 
			NULL, 
			&RasEntry, 
			&dwEntrySize, 
			&bDeviceInfo, 
			&dwDeviceInfoSize);


	if (dwRet != ERROR_SUCCESS)
	{
		return dwRet;
	}

	lpRasEntry = (LPRASENTRY)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwEntrySize);
	if (lpRasEntry == NULL)
	{
		return false;
	}

	lpRasEntry->dwSize = dwEntrySize;
	
	// Options
	lpRasEntry->dwfOptions =
		( RASEO_RemoteDefaultGateway
		  | RASEO_SecureLocalFiles 
		  // | RASEO_TerminalAfterDial 
		  // | RASEO_UseCountryAndAreaCodes 
		  // | RASEO_SpecificIpAddr 
		  // | RASEO_SpecificNameServers 
		  // | RASEO_IpHeaderCompression 
		  // | RASEO_ModemLights 
		  // | RASEO_UseLogonCredentials 
		  // | RASEO_RemoteDefaultGateway 
		  // | RASEO_SwCompression 
		  // | RASEO_PreviewPhoneNumber 
		  // | RASEO_PreviewUserPw 
		  // | RASEO_PreviewDomain 
		  // | RASEO_ModemLights 
		  // | RASEO_TerminalBeforeDial 
		  // | RASEO_ShowDialingProgress 
		  );

	if ( strlen(rasusrname))
	{
		lpRasEntry->dwfOptions |= RASEO_RequireMsEncryptedPw ;
		lpRasEntry->dwfOptions |= RASEO_UseLogonCredentials ;
		
	}
		
	wchar_t* PhNo;
	if (!(strlen(PhoneNo)))
	{
		char PhoneNumber[10] = "*99*#";
		PhNo = stringToWString((const char *)PhoneNumber);

	}
	else
	{
		PhNo = stringToWString((const char *)PhoneNo);
	}

	wcscpy(lpRasEntry->szLocalPhoneNumber, PhNo);
	free(PhNo);

	wcscpy(lpRasEntry->szDeviceType, RASDT_Modem);

	lpRasEntry->dwfNetProtocols = RASNP_Ip;
	lpRasEntry->dwFramingProtocol = RASFP_Ppp;
	lpRasEntry->dwIdleDisconnectSeconds = RASIDS_Disabled;

	//lpRasEntry->dwfOptions = RasEntry.dwfOptions;
	
	//lpRasEntry->dwCountryID = RasEntry.dwCountryID;
	//lpRasEntry->dwCountryCode = RasEntry.dwCountryCode;
	//::strcpy(lpRasEntry->szAreaCode, RasEntry.szAreaCode);
	
	//::memcpy(&lpRasEntry->ipaddr, &RasEntry.ipaddr, sizeof(CIPAddressCtrl));
	//::memcpy(&lpRasEntry->ipaddrDns, &RasEntry.ipaddrDns, sizeof(CIPAddressCtrl));
	//::memcpy(&lpRasEntry->ipaddrDnsAlt, &RasEntry.ipaddrDnsAlt, sizeof(CIPAddressCtrl));
	//::memcpy(&lpRasEntry->ipaddrWins, &RasEntry.ipaddrWins, sizeof(CIPAddressCtrl));
	//::memcpy(&lpRasEntry->ipaddrWinsAlt, &RasEntry.ipaddrWinsAlt, sizeof(CIPAddressCtrl));

	//wcscpy (lpRasEntry->szDeviceName, (stringToWString((const char *)RasEntry.szDeviceName)));
	//strcpy(lpRasEntry->szDeviceName, RasEntry.szDeviceName);

	//dwResult = RasSetEntryProperties(NULL, tchNewEntry, &rasEntry,
	//									sizeof(RASENTRY), NULL, 0);
	
	wchar_t* ProfileName;
	if (!strlen(rasName))
	{
		strEntryName = "UMA";
		ProfileName = (stringToWString((const char *)strEntryName));
	}
	else
	{
		ProfileName = (stringToWString((const char *)rasName));
	}


	
	dwRet = RasSetEntryProperties(NULL, 
		ProfileName,
		lpRasEntry, 
		dwEntrySize, 
		&bDeviceInfo, 
		dwDeviceInfoSize);
	
	
	HeapFree(GetProcessHeap(), 0, (LPVOID)lpRasEntry);

	if(dwRet != ERROR_SUCCESS )
	{
		return dwRet;
	}

	RASDIALPARAMS rdParams;
	ZeroMemory(&rdParams, sizeof(RASDIALPARAMS));
	rdParams.dwSize = sizeof(RASDIALPARAMS);
	//wcscpy(rdParams.szPhoneNumber, NULL);
	
	wcscpy (rdParams.szEntryName, ProfileName);
	
	free(ProfileName);

	if ( strlen(rasusrname) && strlen(raspasswd))
	{
		wchar_t* wcUsername = stringToWString(rasusrname);
		wchar_t* wcPassword = stringToWString(raspasswd);

		wcscpy( rdParams.szUserName, wcUsername);
		wcscpy( rdParams.szPassword, wcPassword);

		free( wcUsername );
		free( wcPassword );
	}


	dwRet = RasSetEntryDialParams(NULL, &rdParams, false);

	if(dwRet == ERROR_SUCCESS )
	{
		unsigned char tmpStr[100];
		wCharToUCharString(rdParams.szEntryName, tmpStr);
		RasEntriesNames = (unsigned char*)_strdup((const char*)tmpStr);
		return RAS_SUCCESS;
	}
	else
	{
		return dwRet;
	}

}

/***********************************************************************
*	DialRas -
*	function which Does a Dialling of RAS connection and inits the variables
*	accordingly.
*
*	Parameters - 
*	None
***********************************************************************/
U_DWORD URasConnection::DialRas(U_CSTR rasusrname, U_CSTR raspasswd)
{
	/* Call the function RasDial, after initializing the phone number to call etc etc 
     * In case of error, call RasGetErrorString to get the error. 
	 * Set the global variables, RasIsDialing indicating no RAS is Dialling. */ 
#ifdef WIN32
	RASCREDENTIALS rasCredentials;
	RASDIALPARAMS rasDialParams;
	RASDIALEXTENSIONS rasDialExtension;
	memset(&rasDialExtension, 0, sizeof(RASDIALEXTENSIONS));
	DWORD dwRet = ERROR_SUCCESS;

	URasIsDialing = true;
	URasIsConnected = false;
	URasConnHandle = NULL;
	RasError = RAS_SUCCESS;

	
	/* get the user name and password to connect */
	memset(&rasCredentials, 0, sizeof(RASCREDENTIALS));
	rasCredentials.dwSize = sizeof(RASCREDENTIALS);
	rasCredentials.dwMask = RASCM_UserName | RASCM_Password;
	
	if ( RasEntriesNames == NULL )
	{
		URasIsDialing = false;
		
		return dwRet;
	}

	wchar_t* wcRasEntryName = stringToWString((const char *)RasEntriesNames);

	dwRet = RasGetCredentials(NULL, wcRasEntryName, &rasCredentials);
	if( dwRet != ERROR_SUCCESS)
	{
		/* Error. */
		LOG(ULOG_DEBUG_LEVEL,"RasGetCredentials failed .\n");
		URasIsDialing = false;
		return dwRet ; 
	}
	free(wcRasEntryName);

	ZeroMemory(&rasDialParams, sizeof(RASDIALPARAMS));
	rasDialParams.dwSize = sizeof(RASDIALPARAMS);
	rasDialParams.szPhoneNumber[0] = TEXT('\0');
	rasDialParams.szCallbackNumber[0] = TEXT('\0');
	
	wchar_t* wcTempRasEntryName = stringToWString((const char *)RasEntriesNames);
	wcscpy (rasDialParams.szEntryName, wcTempRasEntryName);
	free(wcTempRasEntryName);

	wcscpy (rasDialParams.szUserName, rasCredentials.szUserName);
	wcscpy (rasDialParams.szPassword, rasCredentials.szPassword);
	wcscpy (rasDialParams.szDomain, rasCredentials.szDomain);
	rasDialParams.dwCallbackId = NULL;

	if ( strlen(rasusrname) && strlen(raspasswd))
	{
		wchar_t* wcUsername = stringToWString(rasusrname);
		wchar_t* wcPassword = stringToWString(raspasswd);

		wcscpy( rasDialParams.szUserName, wcUsername);
		wcscpy( rasDialParams.szPassword, wcPassword);

		free( wcUsername );
		free( wcPassword );
	}

	dwRet = RasDial(NULL, NULL, &rasDialParams, 0L, /* NULL */(LPVOID)URasConnection::RasDialEventNotifier, &URasConnHandle);

	if (dwRet == ERROR_SUCCESS)
	{
		LOG(ULOG_INFO_LEVEL,"Success from RasDial:",dwRet);
		return RAS_SUCCESS;
	}
	else
	{
		/*if(dwRet == 691 || dwRet == 629)
		{
			char cmdStr[256];
			sprintf(cmdStr, "rasphone -d %s", RasEntriesNames);
			system(cmdStr);
		} */
		LOG(ULOG_INFO_LEVEL,"Error from RasDial:",dwRet);
		//HandleError ( dwRet );
		URasIsDialing = false;
		return dwRet;
	}
#elif (defined(__linux__)|| defined(__MACH__))
    
#endif
}

/***********************************************************************
*	HangUpRas -
*	function which hangs up the RAS connection and inits the variables
*	accordingly.
*
*	Parameters - 
*	None
***********************************************************************/
bool URasConnection::HangUpRas(void)
{
    #ifdef WIN32
	DWORD dwRet = ERROR_SUCCESS;
	/* Call the RasHangUp function to end the RAS connection. 
	 * Handle error if any. 
	 * Set the global variables, RasIsConnected indicating no RAS connection. */

	if ( URasConnHandle != NULL )
	{
		dwRet = RasHangUp (URasConnHandle); 
		LOG(ULOG_INFO_LEVEL,"Return from RasHangUp:",dwRet);
		if (dwRet == ERROR_SUCCESS)
		{
			URasIsConnected = false;
			URasConnHandle = NULL;
			//Msh :: Ras hangup successful so display an error. 
			return false;
		}
		else 
		{
			URasIsConnected = false;
			URasConnHandle = NULL;
		
			// Msh :: Send an AT Command and in the hanlde of response send state change to BM.
			// Msh :: Send an event to the BM/GUI informing the disconnection.
			return true;
		}
		if ( GetRasisDialing() == true )
		{
			URasIsDialing = false;
		}

	}
	return false;
#endif
}


/***********************************************************************
*	GetRasConnectionStatus -
*	function which returns the RAS connection state. 
*
*	Parameters - 
*	None
***********************************************************************/
#ifdef WIN32
RASCONNSTATE URasConnection::GetRasConnectionStatus(void)
{
	/* The function call RasGetConnectStatus returns the RAS connection status
	 */
	RASCONNSTATUS rasconnstatus;
	RASCONNSTATE rasconnstate;

    rasconnstate = RASCS_Disconnected;
	DWORD dwRet = ERROR_SUCCESS;

	ZeroMemory(&rasconnstatus, sizeof(RASCONNSTATUS));
	rasconnstatus.dwSize = sizeof(RASCONNSTATUS);

	if ( URasConnHandle != NULL )
	{
		dwRet = RasGetConnectStatus(URasConnHandle, &rasconnstatus);
		LOG(ULOG_INFO_LEVEL,"Return from RasGetConnectStatus:",dwRet);
		if(dwRet == ERROR_SUCCESS)
		{
			rasconnstate = rasconnstatus.rasconnstate;
		}
		else if ( dwRet = ERROR_INVALID_HANDLE )
		{
			/* if the return of RasGetConnectStatus is ERROR_INVALID_HANDLE then 
			 * handle the error. */
		}
	}
	else 
	{
		if ( URasIsConnected == false && URasIsDialing == true )
		{
			URasIsDialing = false;
			URasConnHandle = NULL;
		}
	}

	return rasconnstate;
}
#endif
/***********************************************************************
*	GetRasConnectionStatistics -
*	function which returns the RAS connection status.
*
*	Parameters - 
*	None
***********************************************************************/
bool URasConnection::GetRasConnectionStatistics(Ras_Stats *rasstats)
{
	/* The function RasGetConnectionStatistics returns the details of the RAS statistics. 
	 * Details of Bytes Transmitted, Bytes Received and Duration of the connection. 
	 */
#ifdef WIN32
	RAS_STATS temprasstats;
	DWORD dwRet = ERROR_SUCCESS;

	ZeroMemory(&temprasstats, sizeof(RAS_STATS));
	temprasstats.dwSize = sizeof(RAS_STATS);
	
	if (RasGetConnectionStatistics (URasConnection::URasConnHandle, &temprasstats) == ERROR_SUCCESS)
	{
		LOG(ULOG_INFO_LEVEL,"Successfully called RasGetConnectionStatistics");
		rasstats->ul_data = temprasstats.dwBytesXmited;
		rasstats->dl_data = temprasstats.dwBytesRcved;
		rasstats->duration  = temprasstats.dwConnectDuration;

		return true;
	}
	else
	{
		LOG(ULOG_INFO_LEVEL,"Error in RasGetConnectionStatistics");
		// Log error for RasGetConnectionStatistics. 
		return false;
	}
	return true;
#endif

}


/***********************************************************************
*	GetRasisConnected -
*	function returns the value of URasIsConnected
*
*	Parameters - 
*	None
***********************************************************************/
bool URasConnection::GetRasisConnected(void)
{
	return URasIsConnected;
}

/***********************************************************************
*	GetRasisDialing -
*	function returns the value of GetRasisDialing
*
*	Parameters - 
*	None
***********************************************************************/
bool URasConnection::GetRasisDialing(void)
{
	return URasIsDialing;
}


/***********************************************************************
*	RasDialEventNotifier -
*	function which is called as call back when RAS connection is being established.
*
*	Parameters - 
*	None
***********************************************************************/

#ifdef WIN32
VOID WINAPI URasConnection::RasDialEventNotifier(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError)
{
	switch(rasconnstate)
	{
		case RASCS_OpenPort:
			break;
		case RASCS_PortOpened:
			// Send a message of Port Opened. 
			break;
		case RASCS_ConnectDevice:
			break;
		case RASCS_Authenticate:
			// Send a message of Authentication. 
			break;
		case RASCS_AuthNotify:
			break;
		case RASCS_AuthRetry:
			break;
		case RASCS_AuthCallback:
			break;
		case RASCS_AuthChangePassword:
			break;
		case RASCS_AuthProject:
			break;
		case RASCS_AuthLinkSpeed:
			break;
		case RASCS_AuthAck:
			break;
		case RASCS_ReAuthenticate:
			break;
		case RASCS_Authenticated:
			// Send a message of Aunthicate successful. 
			break;
		case RASCS_PrepareForCallback:
			break;
		case RASCS_WaitForModemReset:
			break;
		case RASCS_WaitForCallback:
			break;
		case RASCS_Projected:
			break;
		case RASCS_Interactive:
			break;
		case RASCS_RetryAuthentication:
			break;
		case RASCS_CallbackSetByCaller:
			break;
		case RASCS_PasswordExpired:
			break;
				
		case RASCS_DeviceConnected:
		case RASCS_AllDevicesConnected:
		case RASCS_Connected:
			LOG(ULOG_INFO_LEVEL,"RasDialEventNotifier - RASCS_Connected");
			URasIsConnected = true;
			URasIsDialing = false;
			// Send a message of connected. 
			break;

		case RASCS_Disconnected:
			LOG(ULOG_INFO_LEVEL,"RasDialEventNotifier - RASCS_Disconnected");
			URasIsConnected = false;
			URasIsDialing = false;
			URasConnHandle = NULL;
			// Send a message of disconnected. 
			break;
	}

	if (dwError && URasConnHandle)
	{
		LOG(ULOG_INFO_LEVEL,"Return from RasDialEventNotifier:",dwError);
		RasHangUp(URasConnHandle);
		URasConnection::RasError = dwError;
		URasIsDialing = false;
	}

	// Forward the message to BM or GUI
}
#endif

#if 0
/***********************************************************************
*	HandleError -
*	function for handling the errors during RAS connection. 
*
*	Parameters - 
*	ErrorCode - contains the Error code returned from the RasDial function API. 
***********************************************************************/
void URasConnection::HandleError (U_UINT32 ErrorCode)
{
	char cmdStr[200];

#ifdef WIN32
	switch(ErrorCode)
	{
		case ERROR_AUTHENTICATION_FAILURE:
			{
				LOG(ULOG_INFO_LEVEL,"RasDial error - ERROR_AUTHENTICATION_FAILURE");
				sprintf(cmdStr, "rasphone -d %s", RasEntriesNames);
				system(cmdStr);
			}
			break;
		case ERROR_DIAL_ALREADY_IN_PROGRESS:
			{
				LOG(ULOG_INFO_LEVEL,"RasDial error - ERROR_DIAL_ALREADY_IN_PROGRESS");
				/* Msh :: send message to GUI that this connection is 
				 * already being dialled. */
			}
			break;
		case ERROR_NO_CONNECTION:
			{
				LOG(ULOG_INFO_LEVEL,"RasDial error - ERROR_NO_CONNECTION");
				/* Msh :: Send error to GUI that connection was dropped.
				 */
			}
			break;
		default:
			break;
	}
#endif
}
#endif //0

