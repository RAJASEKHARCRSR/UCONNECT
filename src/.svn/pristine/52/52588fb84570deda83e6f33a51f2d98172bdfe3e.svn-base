#include "WiFi_NM.h"
#include "ULog.h"
#include "uWlanUtils.h"
#include "uconn_Event.h"




UWiFi_NM* gWiFiNMForCallback;
UWiFi_NM::UWiFi_NM(UEventQueue* bearerManagerQueue):UNetworkManager(bearerManagerQueue)
{
	gWiFiNMForCallback = this;

	ssid = "";
	attemptedSsid = "";
	password = ""; // clear password
	connectedSigQuality = "";
	adapterName = "";
	this->scanTimerId = NULL;
	this->wlanCB = NULL;
	this->userEnabled = true;
	this->userSelectedSsid = "";

	//hardcoded to 0
	this->ifIdx = 0; 


	this->Init(); //initialization of UProcess

	ENMStateForBM newState = NM_DISABLED_STATE;
	checkStateAndPostToBM(newState);

	UWifiInitialize* wifiInitialize = new UWifiInitialize();
	postToWiFiNM_Queue(reinterpret_cast<UEvent*>(wifiInitialize));

	
}
UWiFi_NM::~UWiFi_NM()
{
	this->bearerMngrQueue = NULL;
	this->wifiDisabler();
	stopWifiTimer();
	uwlanUnInit(this->wlanCB);
	

}

U_VOID UWiFi_NM::initNetMngr(void *)
{
	//WiFi_ConfigInfo *wifiGConfigInfo = (WiFi_ConfigInfo*)configInfo;
}

U_VOID UWiFi_NM::disableNetMngr()
{
	UNMDisable* disable = new UNMDisable(WIFI_TECH);
	postToWiFiNM_Queue((UEvent*)disable);

}

U_VOID UWiFi_NM::enableNetMngr()
{
	this->userEnabled = true;
	UNMEnable* enable = new UNMEnable(WIFI_TECH);
	postToWiFiNM_Queue((UEvent*)enable);
	
}

U_VOID UWiFi_NM::searchNetworks()
{
	UNMSearchNetworks* search = new UNMSearchNetworks(WIFI_TECH);
	postToWiFiNM_Queue((UEvent*)search);

}

U_VOID UWiFi_NM::connectToNetwork()
{
	UNMConnect* connect = new UNMConnect(WIFI_TECH);
	postToWiFiNM_Queue((UEvent*)connect);

}

U_VOID UWiFi_NM::disconnectFromNetwork()
{
	UNMDisConnect* disconnect = new UNMDisConnect(WIFI_TECH);
	postToWiFiNM_Queue((UEvent*)disconnect);
}

U_UINT32 UWiFi_NM::setParam(U_UINT32 paramId,string param)
{
	string paramStr = param;
	UNMParam* nmParam = new UNMParam(WIFI_TECH,(Set_NM_Param)paramId,paramStr);
	postToWiFiNM_Queue((UEvent*)nmParam);
	return 0;
}

U_UINT32 UWiFi_NM::getStatisticsNM()
{
	UBearerStatistics* getStats = new UBearerStatistics(WIFI_TECH);
	postToWiFiNM_Queue((UEvent*)getStats);
	return 0;
}

ETechSelect UWiFi_NM::getTechID()
{
	return WIFI_TECH;
}

void UWiFi_NM::updateNMStateAndPost(ENMStateForBM state)
{
	UNMStateEvent* stateEvent = new UNMStateEvent(WIFI_TECH,state);
	postEventToBearerManager(reinterpret_cast<UEvent*>(stateEvent));
}

///////////////////////////////////////////////////////////////////////////////////////

// UProcess msgQ and event handling

///////////////////////////////////////////////////////////////////////////////////////

U_BOOL UWiFi_NM::EventHandler() {

	while(true) {

			UEvent* evt;

			if(GetQ()->MsgQGet((U_UINT8**)&evt) != -1)	
			{
				if(GetQ()->MsgQGetDepth() < 32)
				{
					handleEvent(evt);
				}
				else
				{  
					LOG(ULOG_ERROR_LEVEL,"WifiNM msgq overflow error");
				}

				if(evt!= NULL){
					delete evt;
				}

				
			}

	}

	return true;

}

void UWiFi_NM::handleEvent(UEvent *evt) 
{

	switch(evt->mCmdId) {
		
		case UCONN_BM_NM_ENABLE:
			{
				wifiEnabler();
			}
			break;
		case UCONN_BM_NM_DISABLE:
			{
				wifiDisabler();
			}
			break;
		case UCONN_BM_NM_CONNECT:
			{
				if(this->state == NM_DISABLED_STATE)
				{
					return;
				}

				string pref = "";
				listWifiNetworks(pref);
				if(pref.size())
				{
					attemptedSsid = pref;
					connectToWiFi(pref);
				}
			}
			break;
		case UCONN_BM_NM_DISCONNECT:
			{
				disconnectFromWiFi();
			}
			break;
		case UCONN_BM_NM_SEARCH:
			{
				searchWifiNetworks();
			}
			break;
		case UCONN_CMD_SET_NM_PARAM:
			{
				UNMParam* nmParam = reinterpret_cast<UNMParam*>(evt);
				setWifiParam(nmParam->paramId,nmParam->param);

			}
			break;

		case UCONN_CMD_BEARER_STATISTICS:
			{
				getWifiStats();	
			}
			break;

		case UCONN_WIFI_NM_INITIALIZE:
			{
				initializeWifi();

			}
			break;
		case UCONN_WIFI_INTERFACE_STATE_CHANGE:
			{
				handleWifiInterfaceStateChange(evt);

			}
			break;
		case UCONN_CMD_BEARER_GET_PARAM:
			{
				wifiParamGetter(evt);
			}
			break;
		case UCONN_WIFI_NM_LIST_NETWORKS:
			{
				string pref = "";
				listWifiNetworks(pref);
			}
		break;
		case UCONN_WIFI_CALLBACK_EVT:
			{
				UWifiCallbackEvt* callbackEvt = reinterpret_cast<UWifiCallbackEvt*>(evt);
				wifiNMCallbackHandler(callbackEvt->code);
			}
			break;
		case UCONN_WIFI_SCAN_TIMEOUT_EVT:
			{
				handleWifiScanTimeout();
			}
			break;
	}


}

U_VOID UWiFi_NM::postToWiFiNM_Queue(UEvent* evt) 
{
	this->GetQ()->MsgQPost((U_UINT8*)evt);
}

U_VOID UWiFi_NM::wifiEnabler()
{
	string pref = "";
	LOG(ULOG_INFO_LEVEL,"WifiNM wifiDisabler -> entry");
	if(this->state != NM_DISABLED_STATE)
	{
		//To check if we need to go to ready state
		listWifiNetworks(pref);
		return;
	}
	uWlanIfInfoLst_t* wlanIfInfoLst;
	if(uwlanGetIfLst(this->wlanCB,&(wlanIfInfoLst)) != UWLAN_SUCCESS)
	{
		//Stay in disabled state as no interfaces present
		return;
	}
	this->adapterName = (char*)wlanIfInfoLst->ifInfo_p[this->ifIdx].ifDesc_p;
	uwlanFreeIfLst(wlanIfInfoLst);
	startWifiTimer();

	ENMStateForBM newState = NM_NOT_READY_STATE;
	checkStateAndPostToBM(newState);
	//We may have to move to ready state.
	listWifiNetworks(pref);
	LOG(ULOG_INFO_LEVEL,"WifiNM wifiDisabler -> exit");
		
}

U_VOID UWiFi_NM::wifiDisabler()
{
	
	LOG(ULOG_INFO_LEVEL,"WifiNM wifiDisabler -> entry");
	this->userEnabled = false;
	stopWifiTimer();
	if(this->state == NM_CONNECTED_STATE)
	{
		uwlanDisconnect(this->wlanCB,this->ifIdx,false);
	}
	ENMStateForBM newState = NM_DISABLED_STATE;
	checkStateAndPostToBM(newState);

	UWifiDisabledNotification* disabled = new UWifiDisabledNotification();
	postEventToBearerManager(reinterpret_cast<UEvent*>(disabled));


	LOG(ULOG_INFO_LEVEL,"WifiNM wifiDisabler -> exit");
	
}

U_VOID UWiFi_NM::connectToWiFi(string ssid)
{

	LOG(ULOG_INFO_LEVEL,"WifiNM connectToWifi -> entry");
	uwlanRetVal_t retVal = uwlanConnect(this->wlanCB,(char*)ssid.c_str(),this->ifIdx,false);
	if(retVal == UWLAN_PROFILE_NOT_FOUND)
	{
		//try to connect to open network
		uwlanRetVal_t profileVal;
		profileVal = uwlanAddProfile(this->wlanCB,(char*)ssid.c_str(),NULL,this->ifIdx);
		if(profileVal == UWLAN_NEED_PASSWORD)
		{
			UNMConnectStatus* rspCommand = new UNMConnectStatus(WIFI_TECH,NM_CONNECT_REQUEST_PASSWORD,this->attemptedSsid);
			rspCommand->setExecStatus(0);
			postEventToBearerManager(rspCommand);
		}
		else if(profileVal == UWLAN_SUCCESS)
		{
			uwlanConnect(this->wlanCB,(char*)ssid.c_str(),this->ifIdx,false);
		}
		else
		{
			sendUnableToConnectEvent();
		}
	}
	else if(retVal != UWLAN_SUCCESS)
	{
		sendUnableToConnectEvent();
	}
	LOG(ULOG_INFO_LEVEL,"WifiNM connectToWifi -> exit");

	
}

uwlanRetVal_e UWiFi_NM::wifiConnectWithPassword(char* password)
{
	LOG(ULOG_INFO_LEVEL,"WifiNM connectWithPassword -> entry");
	uwlanRetVal_t retVal = uwlanAddProfile(this->wlanCB,(char*)this->attemptedSsid.c_str(),password,this->ifIdx);
	if(retVal != UWLAN_SUCCESS)
	{
		sendUnableToConnectEvent();
	}
	else if(retVal == UWLAN_SUCCESS)
	{
		uwlanConnect(this->wlanCB,(char*)this->attemptedSsid.c_str(),this->ifIdx,false);
	}
	LOG(ULOG_INFO_LEVEL,"WifiNM connectWithPassword -> exit");

	return retVal;

}

 

U_VOID UWiFi_NM::disconnectFromWiFi()
{
	LOG(ULOG_INFO_LEVEL,"WifiNM disconnectFromWifi -> entry");
	//set state to not ready to give opportunity to other NMs to connect
	ENMStateForBM newState = NM_NOT_READY_STATE;
	checkStateAndPostToBM(newState);
	
	
	uwlanDisconnect(this->wlanCB,this->ifIdx,false);
	LOG(ULOG_INFO_LEVEL,"WifiNM disconnectFromWifi -> exit");
	
}



U_VOID UWiFi_NM::searchWifiNetworks()
{
	string pref = "";
	listWifiNetworks(pref);
}

U_VOID  UWiFi_NM::setWifiParam(U_UINT32 paramID,string param)
{
	LOG(ULOG_INFO_LEVEL,"WifiNM setWifiParam -> entry - param: %s",param.c_str());
	switch(paramID)
	{
		case WIFI_SSID:
			{
				if(strcmp(this->ssid.c_str(),param.c_str()))
				{
					this->userSelectedSsid = param;
					this->attemptedSsid = param;
					if(param.size())
					{
						connectToWiFi(param);
					}
				}
			}
			break;
		case WIFI_PREFERRED_LIST:
			{

			}
			break;
		case WIFI_PASSWORD:
			{
				
				
				if(param.size())
				{
					this->password = param;
					wifiConnectWithPassword((char*)this->password.c_str());
				}
				//Same paramId used to reset selectedSsid in case of password cancel
				else
				{
					this->userSelectedSsid = "";
				}
			}
			break;
	}
	LOG(ULOG_INFO_LEVEL,"WifiNdM setWifiParam -> exit");
	

}

U_VOID UWiFi_NM::initializeWifi()
{
	LOG(ULOG_INFO_LEVEL,"WifiNM initializeWifi -> entry");
	
	if(uwlanInit(&(this->wlanCB),wifiNMCallback) != UWLAN_SUCCESS)
	{
		LOG(ULOG_ERROR_LEVEL,"Fatal error. Not able to initialize WifiNM");
		return;
	}
	else
	{
		uWlanIfInfoLst_t* wlanIfInfoLst;
		if(uwlanGetIfLst(this->wlanCB,&(wlanIfInfoLst)) != UWLAN_SUCCESS)
		{
			uwlanUnInit(this->wlanCB);
			LOG(ULOG_ERROR_LEVEL,"Fatal error. Not able to initialize WifiNM");
		}
		else
		{
			this->adapterName = (char*)wlanIfInfoLst->ifInfo_p[this->ifIdx].ifDesc_p;
			uwlanFreeIfLst(wlanIfInfoLst);
			char ssid[UWLAN_SSID_LEN];
			/* loop thru all the available interfaces her !! */
			if(uwlanRVConnected(this->wlanCB, ssid, this->ifIdx))
			{
				/* we are connected to ssid */
				this->ssid = ssid;
				ENMStateForBM newState = NM_CONNECTED_STATE;
				checkStateAndPostToBM(newState);
				startWifiTimer();
				//start scan timer
			}
			
		}
	}
	LOG(ULOG_INFO_LEVEL,"WifiNM initializeWifi -> exit");
}

U_VOID UWiFi_NM::handleWifiInterfaceStateChange(UEvent* evt)
{
	LOG(ULOG_INFO_LEVEL,"WifiNM interfaceStateChange -> entry");

	UWifiInterfaceStateChange* interfaceState = reinterpret_cast<UWifiInterfaceStateChange*>(evt);
	//call getiflist loop thru interfaces
	//Check if user has enabled WifiNM and then go to not ready state

	if(interfaceState->present)
	{
		if(userEnabled)
		{
			wifiEnabler();
		}
		else
		{
			wifiDisabler();
		}
	}
	else
	{
		checkStateAndPostToBM(NM_DISABLED_STATE);
	}

	LOG(ULOG_INFO_LEVEL,"WifiNM interfaceStateChange -> exit");	
}

U_VOID UWiFi_NM::checkStateAndPostToBM(ENMStateForBM newState)
{
	if(this->state != newState)
	{

		LOG(ULOG_DEBUG_LEVEL,"Wifi::newState %d oldState %d",newState,this->state);
		this->state = newState;
		updateNMStateAndPost(this->state);
		
	}
	
}

U_VOID UWiFi_NM::getParam(Get_NM_Param paramId)
{
	UBearerGetParam* getParam = new UBearerGetParam(WIFI_TECH,paramId);
	postToWiFiNM_Queue(reinterpret_cast<UEvent*>(getParam));

}

U_VOID UWiFi_NM::wifiParamGetter(UEvent* evt)
{
	LOG(ULOG_INFO_LEVEL,"WifiNM wifiParamGetter -> entry");
	UBearerGetParam* getParam = reinterpret_cast<UBearerGetParam*>(evt);
	string param = "";
	switch(getParam->paramId)
	{
		case NETWORK_NAME:
		{
			if(this->state == NM_CONNECTED_STATE)
			{
				param = this->ssid;
			}
		}
		break;
		case ADAPTER_NAME:
		case VERSION:
		{
			param = this->adapterName;
		}
		break;
		default:
		{
		}
		break;
	}

	LOG(ULOG_INFO_LEVEL,"WifiNM wifiParamGetter -> exit");
	UBearerGetParamRsp* paramRsp = new UBearerGetParamRsp(WIFI_TECH,getParam->paramId,param);
	postEventToBearerManager(reinterpret_cast<UEvent*>(paramRsp));

}

U_VOID UWiFi_NM::handleWifiScanTimeout() {

	LOG(ULOG_INFO_LEVEL,"WifiNM wifiNM handleWifiScanTimeout -> entry");
	if(!userEnabled)
	{
		return;
	}
	//uwlanScanNw(this->wlanCB,this->ifIdx,false);
	string pref = "";
	listWifiNetworks(pref);
	LOG(ULOG_INFO_LEVEL,"WifiNM wifiNM handleWifiScanTimeout -> exit");

}

U_VOID UWiFi_NM::getWifiStats()
{
	LOG(ULOG_INFO_LEVEL,"WifiNM getWifiStats -> entry");
	uWlanStats_s* wlanStats = NULL;
	if(uwlanGetStats(this->wlanCB,this->ifIdx,&wlanStats) 
		!= UWLAN_SUCCESS)
		return;
	
	wlanStats->RxRate /= 1000;
	wlanStats->TxRate /= 1000;
	LOG(ULOG_DEBUG_LEVEL,"WifiNM::RxRate %d TxRate %d",wlanStats->RxRate,wlanStats->TxRate);
	
	if(!wlanStats->RxRate)
	{
		wlanStats->RxRate = 54;
	}
	if(!wlanStats->TxRate)
	{
		wlanStats->TxRate = 54;
	}


	string encodeStr = "";

	char txStr[10];
	char rxStr[10];
	sprintf(txStr,"%d",wlanStats->TxRate);
	sprintf(rxStr,"%d",wlanStats->RxRate);
	free(wlanStats);


	encodeStr += "TxRate;";
	encodeStr += txStr;
	encodeStr += " Mbps";
	encodeStr += "\n";
	
	encodeStr += "RxRate;" ;
	encodeStr += rxStr;
	encodeStr += " Mbps";

	LOG(ULOG_INFO_LEVEL,"WifiNM wifiGetStats -> exit");
	UWifiStatsRsp* statsRsp = new UWifiStatsRsp(encodeStr);
	postEventToBearerManager(reinterpret_cast<UEvent*>(statsRsp));


}

U_VOID UWiFi_NM::listWifiNetworks(string& preferredSsid)
{
	//Don't list if connection to user selected ssid in progress
	//Because state will be set to ready and unneccessary connect
	//will be issued by BM.
	if(userEnabled == false || this->userSelectedSsid.size())
	{
		return;
	}
	LOG(ULOG_INFO_LEVEL,"listWifiNetworks -> entry");
	uWlanNwLst_t* wlanNWLst = NULL;
	uwlanRetVal_e retVal = uwlanGetNwList(this->wlanCB,&wlanNWLst,this->ifIdx);
	string encodeStr = "";
	string knownSsids = "";
	preferredSsid = "";
	U_UINT32 maxSigQual = 0;
	ENMStateForBM newState = NM_NOT_READY_STATE;
	if(retVal != UWLAN_SUCCESS)
	{
		checkStateAndPostToBM(newState);
		this->ssid = "";
		return;
	}

	for(unsigned int i=0;i<wlanNWLst->numOfNw;i++)
	{
		bool duplicate = false;
		for(unsigned int j=0;j<i;j++)
		{
			if(i == j)
			{
				continue;
			}
			if(!strcmp(wlanNWLst->nwInfo_p[i].ssid,wlanNWLst->nwInfo_p[j].ssid))
			{
				duplicate = true;
				break;
			}
		}
		if(duplicate)
		{
			continue;
		}
		encodeStr += "SSID:";
		if(strlen(wlanNWLst->nwInfo_p[i].ssid) < 2)
		{
			encodeStr += "Unknown Network";
		}
		else
		{
			encodeStr += wlanNWLst->nwInfo_p[i].ssid;
		}
		encodeStr += ";";
		char sigQual[5];

		encodeStr += "SigQuality:";
		sprintf(sigQual,"%d",wlanNWLst->nwInfo_p[i].sigQuality);
		encodeStr += sigQual;
		encodeStr += "\n";
		if(wlanNWLst->nwInfo_p[i].isConnected  == 1)
		{
			this->connectedSigQuality = sigQual;
			this->ssid = wlanNWLst->nwInfo_p[i].ssid;
			preferredSsid = wlanNWLst->nwInfo_p[i].ssid;
		}
		if(wlanNWLst->nwInfo_p[i].isProfileAvlbl == 1)
		{
			if(wlanNWLst->nwInfo_p[i].sigQuality > maxSigQual)
			{
				preferredSsid = wlanNWLst->nwInfo_p[i].ssid;
				maxSigQual = wlanNWLst->nwInfo_p[i].sigQuality;
			}
			knownSsids += wlanNWLst->nwInfo_p[i].ssid;
			knownSsids += "\n";

		}

	}
	uwlanFreeNwLst(wlanNWLst);

	UNMSearchResp* searchResp = new UNMSearchResp(WIFI_TECH,encodeStr);
	UEvent* evt = reinterpret_cast<UEvent*>(searchResp);
	postEventToBearerManager(evt);

	UWifiPreferredNetworksEvt* prefEvt = new UWifiPreferredNetworksEvt(knownSsids);
	postEventToBearerManager(reinterpret_cast<UEvent*>(prefEvt));



	if(this->state != NM_CONNECTED_STATE)
	{
		newState = preferredSsid.size()>0 ? NM_READY_STATE : NM_NOT_READY_STATE;
		checkStateAndPostToBM(newState);
	}
	
	LOG(ULOG_INFO_LEVEL,"listWifiNetworks -> exit");

}

U_VOID scanWifiNetworksCallback(U_PVOID lpParam, U_BOOL val) 
{
	
	UWifiScanTimeoutEvt* evt = new UWifiScanTimeoutEvt();
	gWiFiNMForCallback->postToWiFiNM_Queue(reinterpret_cast<UEvent*>(evt));
	

}




U_VOID UWiFi_NM::wifiNMCallbackHandler(int code)
{
	LOG(ULOG_INFO_LEVEL,"WifiNM wifiNMCallbackHandler -> entry");

	switch(code)
	{
		case UWLAN_CONNECTED:
		{
			handleWifiConnectCB();			
		}
		break;
		case UWLAN_DISCONNECTED:
		{
			handleWifiDisconnectCB();
		}
		break;
		case UWLAN_SCAN_COMPLETED:
		{
			string pref = "";
			listWifiNetworks(pref);
		}
		break;
		case UWLAN_OPERATION_FAILED:
		{
			uwlanState_t wlanState;
			UWLAN_GET_STATE(this->wlanCB,wlanState);
			
			if(wlanState == UWLAN_STATE_CONNECTING)
			{
				uwlanDelProfile(this->wlanCB,this->ifIdx,(char*)userSelectedSsid.c_str());
				sendUnableToConnectEvent();
			}
		}
		break;
		case UWLAN_DEVICE_PLUGIN:
		{
			UWifiInterfaceStateChange* plugin = new UWifiInterfaceStateChange(true);
			postToWiFiNM_Queue(reinterpret_cast<UEvent*>(plugin));
		}
		break;
		case UWLAN_DEVICE_PLUGOUT:
		{
			UWifiInterfaceStateChange* plugout = new UWifiInterfaceStateChange(false);
			postToWiFiNM_Queue(reinterpret_cast<UEvent*>(plugout));
		}
		break;
	}
	LOG(ULOG_INFO_LEVEL,"WifiNM wifiNMCallbackHandler -> exit");

}


void wifiNMCallback(int code)
{
	UWifiCallbackEvt* evt = new UWifiCallbackEvt(code);
	gWiFiNMForCallback->postToWiFiNM_Queue(reinterpret_cast<UEvent*>(evt));
}


void UWiFi_NM::sendUnableToConnectEvent()
{
	
	UNMConnectStatus* connectFail = new UNMConnectStatus(WIFI_TECH,NM_CONNECT_FAILURE,this->attemptedSsid);
	attemptedSsid = "";
	userSelectedSsid = "";
	postEventToBearerManager((UEvent*)(connectFail));
}

void UWiFi_NM::handleWifiConnectCB()
{
	if(userEnabled)
	{
		postNewConnectedStatus();
	}
	else
	{
		//only place where state is directly changed. Special case
		//where user connects thru os
		this->state = NM_CONNECTED_STATE;
		wifiDisabler();
	}

}
void UWiFi_NM::handleWifiDisconnectCB()
{
	if(!userEnabled)
	{
		return;
	}

	//Normal Disconnect case.
	
	if(!userSelectedSsid.size())
	{

		this->ssid = "";
		ENMStateForBM newState = NM_NOT_READY_STATE;
		string pref = "";
		listWifiNetworks(pref);
		if(pref.size())
		{
			newState = NM_READY_STATE;
		}
		checkStateAndPostToBM(newState);
	}
	//Disconnecting due to userSelection of Ssid
	else
	{
		postNewConnectedStatus();
	
	}
	
}

U_VOID UWiFi_NM::postNewConnectedStatus()
{
	char connectedSsid[UWLAN_SSID_LEN];
	if(uwlanRVConnected(this->wlanCB,connectedSsid,this->ifIdx))
	{
		this->ssid = connectedSsid;
		this->userSelectedSsid = "";
		checkStateAndPostToBM(NM_CONNECTED_STATE);
		UBearerGetParamRsp* ssidRsp = new UBearerGetParamRsp(WIFI_TECH,NETWORK_NAME,this->ssid);
		postEventToBearerManager(reinterpret_cast<UEvent*>(ssidRsp));
		
	}
	else
	{
		this->userSelectedSsid = "";
		this->ssid = "";
		string pref = "";
		listWifiNetworks(pref);
	}
}

U_VOID UWiFi_NM::startWifiTimer()
{
	return;
	if(this->scanTimerId == NULL)
	{
		this->scanTimerId = USetTimer(WIFI_SCAN_TIMER_INTERVAL,1,(U_CALLBACK)scanWifiNetworksCallback);
	}
}

U_VOID UWiFi_NM::stopWifiTimer()
{
	if(this->scanTimerId != NULL)
	{
		UKillTimer(this->scanTimerId);
		this->scanTimerId = NULL;
	}
}

