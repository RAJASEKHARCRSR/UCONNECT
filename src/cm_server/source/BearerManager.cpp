/*****************************************************************************
**	BearerManager.cpp
**  This file describes the methods for the BearerManager to 
**  be called by the thin layer SAPI
**
** Author: Suresh Lingadalli
** Date  : 01-MAR-2012
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		01/03/12     SS			Creation
****************************************************************************/

#include "BearerManager.h"
#include "UCommonFunctions.h"
#include "uconn_SAPIwrapper.h"

#define UCMVersion "4.2"

UBearerManager::UBearerManager()
{
	eventBearerMngrCBHdlr = NULL;
	parserPtr = new XMLParser();
	parserPtr->parseConfigFile();
	this->initialize();
};

UBearerManager::~UBearerManager()
{
	// Save configuration for next
	updateNMPriorityToConfigFile();
	updateNMEnableToConfigFile();

	/* Disconnect from Current Active NM if Connected */
	if(currentActiveNM)
	{
		currentActiveNM->disableNetMngr();
	}

	if(parserPtr != 0)
	{
		delete parserPtr;
		parserPtr = NULL;
	}
	delete [] nmListOfObjects;
	for(unsigned int i=0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
	{
		//delete nmListOfObjects[i];
		nmListOfObjects[i] = NULL;
	}
	nmListOfObjects = NULL;
	this->eventBearerMngrCBHdlr = NULL;
}


void UBearerManager::UCM_Init()
{
	initializeUCMLogger();
	nmListOfObjects = new UNetworkManager *[MAX_NUMBER_OF_NETWORKMANAGERS];

	//initialize to NULL before creating nm objects
	currentActiveNM = 0;


	
	/*How you create objects of NM*? Like this */

	//create wifi nm first to check if connected externally
	nmListOfObjects[WIFI_TECH] = new UWiFi_NM(this->GetQ());
	
	nmListOfObjects[LTE_TECH] = new ULTE_NM(this->GetQ());
	nmListOfObjects[THREE_G_TECH] = new UThreeG_NM(this->GetQ());
	

	techPriorityArray = new U_UINT32[MAX_NUMBER_OF_NETWORKMANAGERS];
	memset(techPriorityArray,0,MAX_NUMBER_OF_NETWORKMANAGERS*sizeof(U_UINT32));
	
	

	nmListOfObjects[LTE_TECH]->initNetMngr(&parserPtr->gCMInfo.lteConfigInfo);
	nmListOfObjects[THREE_G_TECH]->initNetMngr(&parserPtr->gCMInfo.threeGConfigInfo);
	nmListOfObjects[WIFI_TECH]->initNetMngr(&parserPtr->gCMInfo.wifiConfigInfo);
	
	//pwifiNMInfo = new WiFi_ConfigInfo_s();;
	//l.clear();
	isBearerMngrManualMode = parserPtr->gCMInfo.isManualMode;

	U_UINT32 priorityArray[MAX_NUMBER_OF_NETWORKMANAGERS];
	memset(techPriorityArray,0,MAX_NUMBER_OF_NETWORKMANAGERS*sizeof(U_UINT32));
	vector<string> vStr;
	vStr.clear();
	tokeniseInformation(parserPtr->gCMInfo.priorityList,vStr,';');
	priorityArray[0] = (U_UINT32)atoi(vStr[0].c_str());
	priorityArray[1] = (U_UINT32)atoi(vStr[1].c_str());
	priorityArray[2] = (U_UINT32)atoi(vStr[2].c_str());
	memcpy(techPriorityArray,priorityArray,MAX_NUMBER_OF_NETWORKMANAGERS*sizeof(U_UINT32));

	nmListOfObjects[techPriorityArray[0]]->setPriority(techPriorityArray[0],0);
	nmListOfObjects[techPriorityArray[1]]->setPriority(techPriorityArray[1],1);
	nmListOfObjects[techPriorityArray[2]]->setPriority(techPriorityArray[2],2);

	/* Retrieve the list of Enabled Technologies */
	if(parserPtr->gCMInfo.isLTE_Enabled)
	{
		nmListOfObjects[LTE_TECH]->enableNetMngr();
	}
	else
	{
		nmListOfObjects[LTE_TECH]->disableNetMngr();
	}
	if(parserPtr->gCMInfo.isThreeG_Enabled)
	{
		nmListOfObjects[THREE_G_TECH]->enableNetMngr();
	}
	else
	{
		nmListOfObjects[THREE_G_TECH]->disableNetMngr();
	}
	if(parserPtr->gCMInfo.isWIFI_Enabled)
	{
		nmListOfObjects[WIFI_TECH]->enableNetMngr();
	}	
	else
	{
		nmListOfObjects[WIFI_TECH]->disableNetMngr();
	}
}

U_BOOL UBearerManager::EventHandler()
{
	while(1)
	{
		try {
			//printf("event handler try\n");
			// Wait for Message from Message Queue
			UEvent *evt;
			if(GetQ()->MsgQGet((U_UINT8**)&evt) != -1)	
			{
				//printf("got a message\n");
				// Ideally we should not have msgqdepth increasing, but if it is then 
				// there are too many messages, so lets drop them instead of processing
				if(GetQ()->MsgQGetDepth() < 32)
				{
				// Do any processing on this event if required
				// Post this event to the statemachine
					//mCMSM.handle(evt);
					queueMessageHandler(evt);
				}
				else{
				// ALARM LOG HERE , DROP THE EVENT as we have too many events
					//LOG(ULOG_ERROR_LEVEL, "ALARM : Msg Q overload Dropped Event id: %d size:%d\n",evt->getCmdId(),evt->getSize());
					LOG( ULOG_ERROR_LEVEL,"ALARM : Msg Q overload Dropped Event id and size:",evt->getCmdId(), evt->getSize()) ;
				}

				/*delete evt;
				evt = NULL;*/

			}
		}
		catch(bad_alloc) {
			printf("Out of Memory!\n");
		}
		catch(exception) {
			printf("Unknown Exception received\n");
		}
	}
}



void UBearerManager::initialize()
{	
	// Initialize the Task
	this->Init();
}


UEvent* UBearerManager::handle_ClientCommand(UEvent *reqFromClient_p)
{
	U_UINT32 execStatus = 0;
	//U_UINT32 value;
	U_STRING strVal;
	UEvent *rspCommand = NULL;

	//to prevent crash during uninstall because NMs are already deleted
	if(nmListOfObjects == NULL)
	{
		rspCommand = new UGenResponse();
		rspCommand->setExecStatus(execStatus);
		rspCommand->setTokenId(reqFromClient_p->getTokenId());
		return rspCommand;
	}

	
	LOG(ULOG_INFO_LEVEL, "BM::Handle_ClientCommand() Received event from client,evtCmdID=%d,evtSize=%d",reqFromClient_p->getCmdId(),reqFromClient_p->getSize());

	switch(reqFromClient_p->getCmdId())
	{
		case UCONN_CMD_BEARER_ENABLED:
			{
				UBearerEnabled *techEnabled = reinterpret_cast<UBearerEnabled*> (reqFromClient_p);
				execStatus = enableBearer(techEnabled->techId);
			}
			break;
		case UCONN_CMD_BEARER_DISABLED:
			{
				UBearerDisabled *techDisabled = reinterpret_cast<UBearerDisabled*> (reqFromClient_p);
				execStatus = disableBearer(techDisabled->techId);
			}
			break;
		case UCONN_CMD_BEARER_CONNECT:
			{
				UBearerConnect *techConnect = reinterpret_cast<UBearerConnect*> (reqFromClient_p);
				execStatus = connectBearer(techConnect->techId);
			}
			break;
		case UCONN_CMD_BEARER_DISCONNECT:
			{
				UBearerDisconnect *techConnect = reinterpret_cast<UBearerDisconnect*> (reqFromClient_p);
				execStatus = disConnectBearer(techConnect->techId);
			}
			break;
		case UCONN_CMD_BEARER_ARBIT_ON:
			{
				UBearerArbitrationON *arbitrationON = reinterpret_cast<UBearerArbitrationON*> (reqFromClient_p);
				isBearerMngrManualMode = false;
				UBearerArbitrationON *arbON = new UBearerArbitrationON();
				this->messageQueue->MsgQPost((U_PUINT8)arbON);
			}
			break;
		case UCONN_CMD_BEARER_ARBIT_OFF:
			{
				UBearerArbitrationOFF *arbitrationOFF = reinterpret_cast<UBearerArbitrationOFF*> (reqFromClient_p);
				isBearerMngrManualMode = true;
				UBearerArbitrationOFF *arbOFF = new UBearerArbitrationOFF();
				this->messageQueue->MsgQPost((U_PUINT8)arbOFF);
			}
			break;
		case UCONN_CMD_BEARER_SEARCH:
			{
				UBearerSearch *techSearch = reinterpret_cast<UBearerSearch*> (reqFromClient_p);
				execStatus = searchBearerNetwork(techSearch->techId);
			}
			break;
		case UCONN_CMD_BEARER_GET_CFG_PARAM_REQ:
			{
				UBearerGetCfgParam *gCfgParam = reinterpret_cast<UBearerGetCfgParam*>(reqFromClient_p);
				execStatus = getCfgParam(gCfgParam->paramId);
			}
			break;
		case UCONN_CMD_BEARER_SEARCH_RESPONSE:
			{
			}
			break;
		case UCONN_CMD_BEARER_PRIORITY:
			{
				UBearerPriorityChanged *bearerPriority = reinterpret_cast<UBearerPriorityChanged*>(reqFromClient_p);
				execStatus = bearerPriorityChanged(bearerPriority);
			}
			break;
		case UCONN_CMD_BEARER_STATISTICS:
			{
				UBearerStatistics *bearerStats = reinterpret_cast<UBearerStatistics*>(reqFromClient_p);
				execStatus = getBearerStatistics(bearerStats->techId);
			}
			break;
		case UCONN_CMD_LTE_MEAS_STATISTIC_REQ:
			{
				UBearerMeasStatistics *bearerStats = reinterpret_cast<UBearerMeasStatistics*>(reqFromClient_p);
				execStatus = getBearerMeasStatistics(bearerStats->techId);
			}
			break;
		case UCONN_CMD_LTE_MOD_STATISTIC_REQ:
			{
				UBearerStatistics *bearerStats = reinterpret_cast<UBearerStatistics*>(reqFromClient_p);
				execStatus = getBearerStatistics(bearerStats->techId);
			}
			break;
		case UCONN_CMD_BEARER_SET_PARAM:
			{
				UBearerStrParam *setParam = reinterpret_cast<UBearerStrParam*>(reqFromClient_p);
				execStatus = setBearerParam(setParam->techId,setParam->paramId,setParam->valueStr);
			}
			break;
		case UCONN_CMD_BEARER_GET_PARAM:
			{
				UBearerGetParam *getParam = reinterpret_cast<UBearerGetParam*>(reqFromClient_p);
				execStatus = getBearerParam(getParam->techId,getParam->paramId);
			}
			break;
		case UCONN_CMD_BEARER_MODE:
			{
				UBearerModeChanged *bearerMode = reinterpret_cast<UBearerModeChanged*>(reqFromClient_p);
				execStatus = bearerModeChanged(bearerMode->techId,bearerMode->bearerMode);
			}
			break;
		case UCONN_CMD_BEARER_LOG_ENABLED:
			{
				UBearerLogEnabled *brLogEnabled = reinterpret_cast<UBearerLogEnabled*>(reqFromClient_p);
				execStatus = enableBearerLog(brLogEnabled->techId);
			}
			break;
		case UCONN_CMD_BEARER_LOG_DISABLED:
			{
				UBearerLogDisabled *brLogDisabled = reinterpret_cast<UBearerLogDisabled*>(reqFromClient_p);
				execStatus = disableBearerLog(brLogDisabled->techId);
			}
			break;
		case UCONN_CMD_BEARER_LOG_LEVEL:
			{
				UBearerLogLevel *brLogLevel = reinterpret_cast<UBearerLogLevel*>(reqFromClient_p);
				execStatus = bearerLogLevel(brLogLevel->techId,brLogLevel->logLevel);
			}
			break;
		case UCONN_CMD_BEARER_AT_COMMOND_MODE:
			{
			}
			break;
		case UCONN_CMD_BEARER_GET_ENABLED_TECH:
			{
				U_UINT32 enabledTechs[MAX_NUMBER_OF_NETWORKMANAGERS];
				getEnabledTechList(enabledTechs);
				UGetEnabledTechListRsp* enabledRsp = new UGetEnabledTechListRsp(enabledTechs);
				BearerMngrEventCB(reinterpret_cast<UEvent*>(enabledRsp));
			}
			break;
		case UCONN_CMD_BEARER_GET_PRIORITY_TECH:
			{
				getPriorityTechList();
			}
			break;
		case UCONN_CMD_BEARER_GET_NM_STATE_REQ:
			{
				UBearerGetNMState *NMStateReq = reinterpret_cast<UBearerGetNMState*>(reqFromClient_p);
				execStatus = getNMCurrentState((ETechSelect)(NMStateReq->techId));
			}
			break;
		case UCONN_CMD_SET_3G_RAS_PROFILE:
		{
			UNM3GSetProfile* setProfile = reinterpret_cast<UNM3GSetProfile*>(reqFromClient_p);
			Ras_Details_s& details = setProfile->rasDetails;
			parserPtr->rasDetails = details;
			
			parserPtr->gCMInfo.threeGConfigInfo.apnName = details.apnName;
			parserPtr->gCMInfo.threeGConfigInfo.rasPasswd = details.rasPasswd;
			parserPtr->gCMInfo.threeGConfigInfo.rasPhoneNo = details.rasPhoneNumber;
			parserPtr->gCMInfo.threeGConfigInfo.rasProfileName = details.rasProfileName;
			parserPtr->gCMInfo.threeGConfigInfo.rasUsrName = details.rasUsrName;
			nmListOfObjects[THREE_G_TECH]->disableNetMngr();

			nmListOfObjects[THREE_G_TECH]->initNetMngr(&parserPtr->gCMInfo.threeGConfigInfo);
			nmListOfObjects[THREE_G_TECH]->enableNetMngr();
			updateRasDetailsToConfigFile(details);
		}
		break;
		case UCONN_CMD_GET_3G_RAS_PROFILE:
		{
			UNM3GSetProfile* setProfile = new UNM3GSetProfile(&parserPtr->rasDetails);
			BearerMngrEventCB(reinterpret_cast<UEvent*>(setProfile));
		}
		break;
		case UCONN_EVT_VPN_LIST_CLIENTS:
		{
			UVPNListClientsRsp *cliList = reinterpret_cast<UVPNListClientsRsp*>(reqFromClient_p);
			UVPNListClientsRsp *clientListsRsp = new UVPNListClientsRsp (cliList->clientList); 
			BearerMngrEventCB(reinterpret_cast<UEvent*>(clientListsRsp));
		}
		break;
		case UCONN_CMD_VPN_LIST_CLIENTS:
			{
				UVPNListClients* clientListsReq = new UVPNListClients (); 
				BearerMngrEventCB(reinterpret_cast<UEvent*>(clientListsReq));
			}
			break;
		case UCONN_EVT_VPN_LIST_PROFILES:
			{
				UVPNListProfilesRsp* proList = reinterpret_cast<UVPNListProfilesRsp*>(reqFromClient_p);
				UVPNListProfilesRsp* proListRsp = new UVPNListProfilesRsp(proList->profileList);
				BearerMngrEventCB(reinterpret_cast<UEvent*>(proListRsp));
			}
			break;
		case UCONN_CMD_VPN_LIST_PROFILES:
			{
				UVPNListProfiles* proListReq = new UVPNListProfiles (); 
				BearerMngrEventCB(reinterpret_cast<UEvent*>(proListReq));
			}
			break;
		case UCONN_CMD_VPN_START:
			{
				UVPNStart* vpnStart = reinterpret_cast<UVPNStart*>(reqFromClient_p);
				UVPNStart* vpnStartCmd = new UVPNStart(string(vpnStart->vpnName));
				BearerMngrEventCB(reinterpret_cast<UEvent*>(vpnStartCmd));
			}
			break;
		case UCONN_EVT_VPN_START_STATUS:
			{
				UVPNStartRsp* cntRsp = reinterpret_cast<UVPNStartRsp*>(reqFromClient_p);
				UVPNStartRsp* connectResponse = new UVPNStartRsp(cntRsp->startStatus);
				BearerMngrEventCB(reinterpret_cast<UEvent*>(connectResponse));
			}
			break;
		case UCONN_CMD_VPN_STOP:
			{
				UVPNStop* vpnStop = new UVPNStop();
				BearerMngrEventCB(reinterpret_cast<UEvent*>(vpnStop));
			}
			break;
		case UCONN_EVT_VPN_STOP_STATUS:
			{
				UVPNStopRsp* disconnectResponse = new UVPNStopRsp();
				BearerMngrEventCB(reinterpret_cast<UEvent*>(disconnectResponse));

			}
			break;

		default: 
			{
				if(reqFromClient_p->getTechId() >=  LTE_TECH  && 
					reqFromClient_p->getTechId() <= WIFI_TECH){
				UEvent *pNewEvt = (UEvent *)malloc(reqFromClient_p->getSize()*sizeof(U_SINT8));
				memcpy((void*)pNewEvt,reqFromClient_p,reqFromClient_p->getSize());
				nmListOfObjects[reqFromClient_p->getTechId()]->handleEvent((UEvent*)pNewEvt);
				}

			}
			break;
	}
	rspCommand = new UGenResponse();
	rspCommand->setExecStatus(execStatus);
	rspCommand->setTokenId(reqFromClient_p->getTokenId());
	return rspCommand;
}




U_SINT32 UBearerManager::enableBearer(int techID)
{
	LOG(ULOG_INFO_LEVEL,"EnableBearer is called with techId:%d",techID);
	
	switch(techID)
	{
	case LTE_TECH:
			nmListOfObjects[LTE_TECH]->enableNetMngr();
			parserPtr->gCMInfo.isLTE_Enabled = true;
		break;
	case THREE_G_TECH:
			nmListOfObjects[THREE_G_TECH]->enableNetMngr();
			parserPtr->gCMInfo.isThreeG_Enabled = true;
		break;
	case WIFI_TECH:
			nmListOfObjects[WIFI_TECH]->enableNetMngr();
			parserPtr->gCMInfo.isWIFI_Enabled = true;
		break;
	case ETHER_TECH:
		break;
	default:
		break;
	}
	
	updateNMEnableToConfigFile();

	return UCM_SUCCESS;
}


U_SINT32 UBearerManager::disableBearer(int techID)
{
	LOG(ULOG_INFO_LEVEL,"DisableBearer is called with techId:%d",techID);

	switch(techID)
	{
	case LTE_TECH:
		{
			nmListOfObjects[LTE_TECH]->disableNetMngr();
			parserPtr->gCMInfo.isLTE_Enabled = false;
		}
		break;
	case THREE_G_TECH:
		{
			nmListOfObjects[THREE_G_TECH]->disableNetMngr();
			parserPtr->gCMInfo.isThreeG_Enabled = false;
		}
		break;
	case WIFI_TECH:
		{
			nmListOfObjects[WIFI_TECH]->disableNetMngr();
			parserPtr->gCMInfo.isWIFI_Enabled = false;
		}
		break;
	case ETHER_TECH:
		break;
	default:
		break;
	}

	updateNMEnableToConfigFile();

	return UCM_SUCCESS;
}


U_SINT32 UBearerManager::connectBearer(int techID)
{
	LOG(ULOG_INFO_LEVEL,"BM::ConnectBearer is called with techId:%d",techID);

	UBearerConnect *netConEvt = new UBearerConnect(techID);
	this->messageQueue->MsgQPost((U_PUINT8)netConEvt);

	return UCM_SUCCESS;
}

U_SINT32 UBearerManager::disConnectBearer(int techID)
{
	LOG(ULOG_INFO_LEVEL,"BM::DisConnectBearer is called with techId:%d",techID);

	UBearerDisconnect *netDisConEvt = new UBearerDisconnect(techID);
	this->messageQueue->MsgQPost((U_PUINT8)netDisConEvt);

	return UCM_SUCCESS;
}



U_SINT32 UBearerManager::searchBearerNetwork(int techID)
{
	LOG(ULOG_INFO_LEVEL,"BM::SearchBearerNetwork is called with techId:%d",techID);

	switch(techID)
	{
	case LTE_TECH:
		nmListOfObjects[LTE_TECH]->searchNetworks();
		break;
	case THREE_G_TECH:
		nmListOfObjects[THREE_G_TECH]->searchNetworks();
		break;
	case WIFI_TECH:
		nmListOfObjects[WIFI_TECH]->searchNetworks();
		break;
	case ETHER_TECH:
		break;
	default:
		break;
	}
	return UCM_SUCCESS;
}



U_SINT32 UBearerManager::updateUCMConfigFile(BearerInfoStruct_s *bearerInfoPtr)
{
	/*
	--First update UCMconfig files chanages from the GUI
	--then send the paramname and value to the respective n/w managers
	*/
	string paramName;
	string paramValue;
	updateBearerMngrWithConfigChange(paramName, paramValue);

	int techID = LTE_TECH;
	switch(techID)
	{
	case LTE_TECH:
		/*
		--updateNetworkManager with config change
		--updateLTEConfigChange(paramName,paramValue);
		*/
		break;
	case THREE_G_TECH:
		/*
		--updateNetworkManager with config change
		--updateThreeGConfigChange(paramName,paramValue);
		*/
		break;
	case WIFI_TECH:
		/*
		--updateNetworkManager with config change
		--updateWiFiConfigChange(paramName,paramValue);
		*/
		break;
	case ETHER_TECH:
		/*
		--updateNetworkManager with config change
		--updateEtherConfigChange(paramName,paramValue);
		*/
		break;
	default:
		break;
	}
	return UCM_SUCCESS;
}

U_SINT32 UBearerManager::updateBearerMngrWithConfigChange(string paramName,string paramValue)
{
	/*
	--open file UCMConfigFile in write mode 
	--then match the paramName with paramName in UCMConfig file
	--then update the paramValue of the corrusponding paramName. 
	--Then close the file.
	*/
	return UCM_SUCCESS;
}


U_SINT32 UBearerManager::enableBearerLog(U_UINT32 techID)
{
	switch(techID)
	{
	case LTE_TECH:
		break;
	case THREE_G_TECH:
		break;
	case WIFI_TECH:
		break;
	case ETHER_TECH:
		break;
	default:
		break;
	}
	return UCM_SUCCESS;
}

U_SINT32 UBearerManager::disableBearerLog(U_UINT32 techID)
{
	switch(techID)
	{
	case LTE_TECH:
		break;
	case THREE_G_TECH:
		break;
	case WIFI_TECH:
		break;
	case ETHER_TECH:
		break;
	default:
		break;
	}
	return UCM_SUCCESS;
}

U_SINT32 UBearerManager::bearerLogLevel(U_UINT32 techID, U_UINT32 logLevel)
{
	switch(techID)
	{
	case LTE_TECH:
		break;
	case THREE_G_TECH:
		break;
	case WIFI_TECH:
		break;
	case ETHER_TECH:
		break;
	default:
		break;
	}
	return UCM_SUCCESS;
}

U_SINT32 UBearerManager::setBearerLogLevel(LogInfoStruct_s *logPtr)
{
	int techID = LTE_TECH;
	switch(techID)
	{
	case LTE_TECH:
		break;
	case THREE_G_TECH:
		break;
	case WIFI_TECH:
		break;
	case ETHER_TECH:
		break;
	default:
		break;
	}
	return UCM_SUCCESS;
}

U_SINT32 UBearerManager::getStatParam(BearerStatParamStruct_s *bearerStatParamPtr)
{
	int techID = LTE_TECH;
	switch(techID)
	{
	case LTE_TECH:
		break;
	case THREE_G_TECH:
		break;
	case WIFI_TECH:
		break;
	case ETHER_TECH:
		break;
	default:
		break;
	}
	return UCM_SUCCESS;
}

U_SINT32 UBearerManager::getStatParamWithValue(BearerStatParamStruct_s *bearerStatParamPtr, BearerStatParamRespStruct_s *bearerStatParamRespPtr)
{
	int techID = 1; //check UCM
	switch(techID)
	{
	case LTE_TECH:
#if 0
		bearerStatParamRespPtr->respType = U_FLOAT_TYPE_RESP;
		bearerStatParamRespPtr->fltValue = 15.666f;
		bearerStatParamRespPtr->tech = ETechSelect::LTE_TECH;
#endif
#if 0
		bearerStatParamRespPtr->respType = U_STRING_TYPE_RESP;
		bearerStatParamRespPtr->strValue = "some string value";
		bearerStatParamRespPtr->tech = ETechSelect::LTE_TECH;
#endif
#if 0
		bearerStatParamRespPtr->respType = U_UINT32_TYPE_RESP;
		bearerStatParamRespPtr->fltValue = 2000;
		bearerStatParamRespPtr->tech = ETechSelect::LTE_TECH;
#endif
		break;
	case THREE_G_TECH:
		break;
	case WIFI_TECH:
		break;
	case ETHER_TECH:
		break;
	default:
		break;
	}
	return UCM_SUCCESS;
}

#if 1
//U_SINT32 BearerManager::setStatParam(BearerSetStatParamStruct_s *bearerStatParamPtr)
U_SINT32 UBearerManager::setBearerParam(U_UINT32 techID,U_UINT32 paramID,string valStr)
{
	LOG(ULOG_INFO_LEVEL,"BM::setBearerParam() techId:%d paramId:%d,valStr:%s",techID,paramID,valStr.c_str());
	switch(techID)
	{
	case LTE_TECH:
		nmListOfObjects[LTE_TECH]->setParam(paramID,valStr);
		break;
	case THREE_G_TECH:
		nmListOfObjects[THREE_G_TECH]->setParam(paramID,valStr);
		break;
	case WIFI_TECH:
		nmListOfObjects[WIFI_TECH]->setParam(paramID,valStr);
		break;
	case ETHER_TECH:
		break;
	case VPN_TECH:
	{
		//UVpn::getVpn()->setParam(paramID,valStr);
	}
	break;
	default:
		break;
	}
	return UCM_SUCCESS;
}
#endif

U_SINT32 UBearerManager::getBearerParam(U_UINT32 techID,Get_NM_Param paramID)
{
	LOG(ULOG_INFO_LEVEL,"BM::getBearerParam() techId:%d paramId:%d",techID,paramID);
	switch(techID)
	{
	case LTE_TECH:
		nmListOfObjects[LTE_TECH]->getParam(paramID);
		break;
	case THREE_G_TECH:
		nmListOfObjects[THREE_G_TECH]->getParam(paramID);
		break;
	case WIFI_TECH:
		nmListOfObjects[WIFI_TECH]->getParam(paramID);
		break;
	case ETHER_TECH:
		break;
	case ALL_TECH:
		{
			switch(paramID)
			{
				case BEARER_MNGR_MODE:
				{
					string mode = "auto";
					if(this->isBearerMngrManualMode)
					{
						mode= "manual";
					}
					
					UBearerGetParamRsp* paramRsp = new UBearerGetParamRsp(ALL_TECH,BEARER_MNGR_MODE,mode);
					BearerMngrEventCB(reinterpret_cast<UEvent*>(paramRsp));
					
				}
				break;
			case UCM_VERSION:
				{
					UBearerGetParamRsp* paramRsp = new UBearerGetParamRsp(ALL_TECH,UCM_VERSION,UCMVersion);
					BearerMngrEventCB(reinterpret_cast<UEvent*>(paramRsp));
				
				}
				break;
			default:
				break;
			}
		}
	}
	return UCM_SUCCESS;
}


U_SINT32 UBearerManager::bearerPriorityChanged(UBearerPriorityChanged *bearerPriority)
{
	LOG(ULOG_INFO_LEVEL,"BM::BearerPriorityChanged() ");

	memcpy(techPriorityArray,bearerPriority->techPriority,MAX_NUMBER_OF_NETWORKMANAGERS*sizeof(U_UINT32));
	int i1 = techPriorityArray[0];
	int i2 = techPriorityArray[1];
	int i3 = techPriorityArray[2];

#if 1
	for(int i = 0; i < MAX_NUMBER_OF_NETWORKMANAGERS; i++)
	{
		switch(techPriorityArray[i])
		{
		case LTE_TECH:
			nmListOfObjects[LTE_TECH]->setPriority(LTE_TECH,i);
			break;
		case THREE_G_TECH:
			nmListOfObjects[THREE_G_TECH]->setPriority(THREE_G_TECH,i);
			break;
		case WIFI_TECH:
			nmListOfObjects[WIFI_TECH]->setPriority(WIFI_TECH,i);
			break;
		case ETHER_TECH:
			break;
		default:
			break;
		}
	}
#endif

	UNMPriorityEvent* priorityEvent = new UNMPriorityEvent(ALL_TECH);
	this->messageQueue->MsgQPost((U_PUINT8)priorityEvent);

	updateNMPriorityToConfigFile();

	return UCM_SUCCESS;
}

U_SINT32 UBearerManager::bearerModeChanged(U_UINT32 techID, U_UINT32 bearerMode)
{
#if 0
	switch(techID)
	{
	case LTE_TECH:
		break;
	case THREE_G_TECH:
		break;
	case WIFI_TECH:
		break;
	case ETHER_TECH:
		break;
	default:
		break;
	}
#endif
	return UCM_SUCCESS;
}


U_SINT32 UBearerManager::getBearerStatistics(U_UINT32 techID)
{
	LOG(ULOG_INFO_LEVEL,"BM::getBearerStatistics() techId:%d",techID);
	if(nmListOfObjects == NULL)
	{
		return UCM_SUCCESS;
	}

#if 1
	switch(techID)
	{
	case LTE_TECH:
		nmListOfObjects[LTE_TECH]->getStatisticsNM();
		break;
	case THREE_G_TECH:
		nmListOfObjects[THREE_G_TECH]->getStatisticsNM();
		break;
	case WIFI_TECH:
		nmListOfObjects[WIFI_TECH]->getStatisticsNM();
		break;
	case ETHER_TECH:
		nmListOfObjects[ETHER_TECH]->getStatisticsNM();
		break;
	default:
		break;
	}
#endif
	return UCM_SUCCESS;
}

U_SINT32 UBearerManager::getBearerMeasStatistics(U_UINT32 techID)
{
	LOG(ULOG_INFO_LEVEL,"BM::getBearerMeasStatistics() techId:%d",techID);
#if 1
	switch(techID)
	{
	case LTE_TECH:
		nmListOfObjects[LTE_TECH]->getMeasStatisticsNM();
		break;
	case THREE_G_TECH:
		nmListOfObjects[THREE_G_TECH]->getMeasStatisticsNM();
		break;
	case WIFI_TECH:
		nmListOfObjects[WIFI_TECH]->getMeasStatisticsNM();
		break;
	case ETHER_TECH:
		nmListOfObjects[ETHER_TECH]->getMeasStatisticsNM();
		break;
	default:
		break;
	}
#endif
	return UCM_SUCCESS;
}

U_SINT32 UBearerManager::getNMCurrentState(U_UINT32 techID)
{
	ENMStateForBM ParamValue;
	switch(techID)
	{
	case LTE_TECH:
		{
			ParamValue = nmListOfObjects[LTE_TECH]->getStateNM();
		}
		break;
	case THREE_G_TECH:
		{
			ParamValue = nmListOfObjects[THREE_G_TECH]->getStateNM();
		}
		break;
	case WIFI_TECH:
		{
			ParamValue = nmListOfObjects[WIFI_TECH]->getStateNM();
		}
		break;
	case ETHER_TECH:
		break;
	default:
		break;
	}

	UNMStateEvent* NMStateEvent = new UNMStateEvent((ETechSelect)techID, ParamValue);
	BearerMngrEventCB(reinterpret_cast<UEvent*>(NMStateEvent));	
	

	return UCM_SUCCESS;
}


U_SINT32 UBearerManager::getVersionInfo(BearerVersionInfoStruct_s *bearerVersionInfoPtr,BearerVersionInfoRespStruct *bearerVersionInfoRespPtr)
{
#if 0
	int techID;
	switch(techID)
	{
	case LTE_TECH:
		//bearerVersionInfoRespPtr->deviceType = 1;
		//bearerVersionInfoRespPtr->tech = ETechSelect::LTE_TECH;
		//strcpy(bearerVersionInfoRespPtr->verInfo.u8VersionString,"version information in it");
		break;
	case THREE_G_TECH:
		break;
	case WIFI_TECH:
		break;
	case ETHER_TECH:
		break;
	default:
		break;
	}
#endif
	return UCM_SUCCESS;
}

U_SINT32 UBearerManager::sendATCommandInPassThroughMode(BearerATCommandStruct_s *bearerATCmdPtr)
{
#if 0
	int techID;
	switch(techID)
	{
	case LTE_TECH:
		break;
	case THREE_G_TECH:
		break;
	case WIFI_TECH:
		break;
	case ETHER_TECH:
		break;
	default:
		break;
	}
#endif
	return UCM_SUCCESS;
}


U_VOID UBearerManager::handleCallBacksFromDevice(U_UINT32 cmdId,U_UINT32 techId)
{
	LOG(ULOG_INFO_LEVEL,"BM::handleCallBacksFromDevice() techId:%d cmdID:%d",techId,cmdId);
	switch(techId)
	{
	case LTE_TECH:
		nmListOfObjects[LTE_TECH]->deviceEventsCallBack(cmdId,techId);
		break;
	case THREE_G_TECH:
		nmListOfObjects[THREE_G_TECH]->deviceEventsCallBack(cmdId,techId);
		break;
	case WIFI_TECH:
		break;
	case ETHER_TECH:
		break;
	default:
		break;
	}
}


/***********************************************************************
*	BearerMngr_eventCB -
*	this is the registered callback for event messages. based on whether 
*	sockets are used or not, the appropriate function will be called here
*
*	Parameters
*	event
***********************************************************************/
U_VOID UBearerManager::BearerMngrEventCB(UEvent *event_p)
{
	if(this->eventBearerMngrCBHdlr == NULL)
	{
		return;
	}
	LOG(ULOG_DEBUG_LEVEL,"BM::Posting event to cliient cmdId:%d techId:%d  ",event_p->mCmdId,event_p->techId);
	this->eventBearerMngrCBHdlr(event_p);
	
}


U_UINT32 UBearerManager::queueMessageHandler(UEvent *evt)
{
	U_UINT status = -1;
	U_UINT32 cmdID = evt->getCmdId();
	LOG(ULOG_INFO_LEVEL,"BM::queueMessageHandler() techId:%d cmdID:%d",evt->getTechId(),evt->getCmdId());

	/* Check for Invalid Tech ID */
	if(evt->getTechId() == INVALID_TECH)
	{
		return 0;
	}

	switch(cmdID)
	{
		case UCONN_CMD_BEARER_ARBIT_ON:
			{
				isBearerMngrManualMode = false;
				handleManualToAutoModeChange();
				updateBMModeToConfigFile();
			}
			break;
		case UCONN_CMD_BEARER_ARBIT_OFF:
			{
				isBearerMngrManualMode = true;
				updateBMModeToConfigFile();
			}
			break;
		case UCONN_CMD_BEARER_PRIORITY_REQ:
			{
				BearerMngrEventCB(evt);
			}
			break;
		case UCONN_EVT_LTE_MODEM_STATISTIC:
			{
				BearerMngrEventCB(evt);
			}
			break;
			case UCONN_CMD_BEARER_CONNECT:
			{
				if(isBearerMngrManualMode == true)
				{
					nmListOfObjects[evt->techId]->connectToNetwork();
				}
				else
				{
					for(int i = 0;i<MAX_NUMBER_OF_NETWORKMANAGERS;++i)
					{
						int techIDCurrent = techPriorityArray[i];//WIFI_TECH;//techPriorityArray[i];
						if(nmListOfObjects[techIDCurrent]->getStateNM() == NM_READY_STATE)
						{
							nmListOfObjects[techIDCurrent]->connectToNetwork();
							break;
						}
					}
				}
			}
			break;
		case UCONN_CMD_BEARER_DISCONNECT:
			{
				if(isBearerMngrManualMode == true)
				{
					nmListOfObjects[evt->techId]->disconnectFromNetwork();
				}
				else
				{
					if(currentActiveNM != 0 && currentActiveNM->getStateNM() == NM_CONNECTED_STATE)
					{
						currentActiveNM->disconnectFromNetwork();
					}
					else
					{
						nmListOfObjects[evt->techId]->disconnectFromNetwork();
					}
				}
			}
			break;
		case UCONN_EVT_NM_PRIORITY_CHANGE:
			{
				UNMPriorityEvent *priorityEvt = (UNMPriorityEvent*) evt;
				if(isBearerMngrManualMode == false)
				{
					NMPriorityHandler();

					for(int i = 0;i<MAX_NUMBER_OF_NETWORKMANAGERS;++i)
					{
						int techIDCurrent = techPriorityArray[i];//WIFI_TECH;//techPriorityArray[i];
						if(nmListOfObjects[techIDCurrent]->getStateNM() == NM_READY_STATE)
						{
							nmListOfObjects[techIDCurrent]->connectToNetwork();
							break;
						}
					}
				}
			}
			break;
		case UCONN_EVT_NM_STATE_CHANGE:
			{
				UNMStateEvent *stateEvt = (UNMStateEvent*) evt;
				if(isBearerMngrManualMode == false)
				{
					NMAutoModeStateHandler(stateEvt->techId,stateEvt->getCurrentState());
				}
				else
				{
					//NMManualModeStateHandler(stateEvt->techId,stateEvt->getCurrentState());
					//to be handled
				}
				BearerMngrEventCB(evt);
			}
			break;
		case UCONN_EVT_NM_SCAN_RESP:
			{
				BearerMngrEventCB(evt);
			}
			break;
		case UCONN_EVT_NM_ENABLE_STATUS:
			{
				BearerMngrEventCB(evt);
			}
			break;
		case UCONN_EVT_NM_CONNECT_STATUS:
			{
				BearerMngrEventCB(evt);
			}
			break;
		case UCONN_EVT_NM_DEVICE_STATE_CHANGE:
			{
			}
			break;
		case UCONN_EVT_WIFI_PREFERRED_NETWORKS:
			{
				BearerMngrEventCB(evt);
			}
			break;
		case UCONN_EVT_WIFI_STATS_RSP:
			{
				BearerMngrEventCB(evt);
				
			}
			break;	
		case UCONN_EVT_3G_STATS_RSP:
			{
				BearerMngrEventCB(evt);

			}
			break;
		case UCONN_EVT_NM_SHOW_SETTINGS:
			{
				BearerMngrEventCB(evt);
			}
			break;
		case UCONN_EVT_GET_BEARER_PARAM_RSP:
			{
				BearerMngrEventCB(evt);
			}
			break;
		default:
			{
				BearerMngrEventCB(evt);
			}

			break;
	}
	return status;
}

void UBearerManager::priorityChange(U_UINT32 techId,U_UINT32 priority)
{
	if(currentActiveNM == 0)
	{
		if(nmListOfObjects[techId]->getStateNM() == NM_READY_STATE)
		{
			nmListOfObjects[techId]->connectToNetwork();
		}
	}
	else if( (currentActiveNM != 0) && (currentActiveNM->getTechID() != techId) )
	{
		if( (priority <= currentActiveNM->getPriority() ) && (nmListOfObjects[techId]->getStateNM() == NM_READY_STATE))
		{
			nmListOfObjects[techId]->connectToNetwork();
		}
	}
}

void UBearerManager::NMPriorityHandler()
{
	for(int i = 0; i < MAX_NUMBER_OF_NETWORKMANAGERS; i++)
	{
		switch(techPriorityArray[i])
		{
		case LTE_TECH:
			priorityChange(LTE_TECH,i);
			break;
		case THREE_G_TECH:
			priorityChange(THREE_G_TECH,i);
			break;
		case WIFI_TECH:
			priorityChange(WIFI_TECH,i);
			break;
		default:
			break;
		}
	}
}


void UBearerManager::NMAutoModeStateHandler(int techId,U_UINT32 currentState)
{
	LOG(ULOG_INFO_LEVEL, "BM::NMStateHandler() techID:%d and state:%d\n",techId,currentState);
	switch(currentState)
	{
	case NM_DISABLED_STATE:
		{
			if((currentActiveNM) && (currentActiveNM->getTechID() == techId))
			{
				currentActiveNM = 0;
				if(nmListOfObjects)
				{
					for(int i = 0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
					{
						if(techPriorityArray[i] == techId)
						{
							continue;
						}

						if( (nmListOfObjects[techPriorityArray[i]]->getStateNM() == NM_READY_STATE))
							
						{
							LOG(ULOG_DEBUG_LEVEL, "BM::NMStateHandler() Sending Connect to NM techId:%d  priority:%d\n",techPriorityArray[i],i);
							nmListOfObjects[techPriorityArray[i]]->connectToNetwork();
							break;
						}
					}
				}
			}
		}
		break; 
	case NM_READY_STATE:
		{
#if 1
			
			int p2 = nmListOfObjects[techId]->getPriority();
			int s1 = nmListOfObjects[techId]->getStateNM();
			if( currentActiveNM == 0 )
			{
				
				//find the highest priority NM among the list which is in NM_READY_STATE
				if( nmListOfObjects[techId]->getStateNM() == NM_READY_STATE )
				{
					LOG(ULOG_DEBUG_LEVEL, "BM::NMStateHandler() Sending Connect to NM techId:%d priority:%d \n",techId,nmListOfObjects[techId]->getPriority());
					nmListOfObjects[techId]->connectToNetwork();
				}
			}
			//else if(techId < currentActiveNM->getTechID())
			//else if(nmListOfObjects[techId]->getPriority() < currentActiveNM->getPriority())
			else if((currentActiveNM->getPriority()) >= (nmListOfObjects[techId]->getPriority()) )
			{
				
				if( (nmListOfObjects[techId]->getStateNM() ) == NM_READY_STATE )
				{
					LOG(ULOG_DEBUG_LEVEL, "BM::NMStateHandler() Sending Connect to NM techId:%d priority:%d \n",techId,nmListOfObjects[techId]->getPriority());
					nmListOfObjects[techId]->connectToNetwork();
				}
			}
#endif
		}
		break;
	case NM_NOT_READY_STATE:
		{
			/*if current active techid  == received tech id*/
			/*then get the current active techId*/
			/*then loop from current active techId till end of the nm list which is in NM_READY_STATE*/
			/*then issue "connect" to the network*/
			/*then break the loop*/
			
			if(currentActiveNM == 0)
			{
				for(int i = 0; i<MAX_NUMBER_OF_NETWORKMANAGERS; ++i)
				{
					if(nmListOfObjects)
					{
						int p = techPriorityArray[i];
						if(nmListOfObjects[techPriorityArray[i]]->getStateNM() == NM_READY_STATE)
						{
						LOG(ULOG_DEBUG_LEVEL, "BM::NMStateHandler() Sending Connect to NM techId:%d priority:%d \n",techPriorityArray[i],i);
							nmListOfObjects[techPriorityArray[i]]->connectToNetwork();
							break;
						}
					}
				}
			}
			else if( (currentActiveNM != 0) && (currentActiveNM->getTechID() == techId) )
			{ 
				for(int i = 0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
				{
					if(techPriorityArray[i] == techId)
					{
						continue;
					}
					if(nmListOfObjects[techPriorityArray[i]]->getStateNM() == NM_CONNECTED_STATE )
					{
						break;
					}
					

					if( (nmListOfObjects[techPriorityArray[i]]->getStateNM() == NM_READY_STATE))
						
					{
							LOG(ULOG_DEBUG_LEVEL, "BM::NMStateHandler() Sending Connect to NM techId:%d priority:%d \n",techPriorityArray[i],i);
							nmListOfObjects[techPriorityArray[i]]->connectToNetwork();
						break;
					}
				}
			}
		}
		break;
	case NM_CONNECTED_STATE:
		{
			/*if current active techid  != received tech id*/
			/*then issue "disconnect" on current active techid*/
			/*then assign current active techid  != received tech id*/
			if(currentActiveNM == 0)
			{
				LOG(ULOG_INFO_LEVEL, "BM::Active connected TechID:%d and connected Active state:%d\n",techId,currentState);
				currentActiveNM = nmListOfObjects[techId];
			}
			else if( currentActiveNM != 0 && currentActiveNM->getTechID() != techId)
			{
				if( (currentActiveNM->getStateNM() == NM_CONNECTED_STATE) && 
				    ( (currentActiveNM->getPriority()) > (nmListOfObjects[techId]->getPriority())) )
				{
					LOG(ULOG_DEBUG_LEVEL, "BM::NMStateHandler() Sending Disconnect to NM techId:%d priority:%d \n",currentActiveNM->getTechID(),currentActiveNM->getPriority());
					currentActiveNM->disconnectFromNetwork();
					/*reset the current active nm*/
					currentActiveNM = 0;
					LOG(ULOG_DEBUG_LEVEL, "BM::Active connected TechID:%d and connected Active state:%d\n",techId,currentState);
					//int p = techPriorityArray[techId];
					currentActiveNM = nmListOfObjects[techId];
				}
				else if( (currentActiveNM->getStateNM() == NM_CONNECTED_STATE) && 
				    ( (currentActiveNM->getPriority()) < (nmListOfObjects[techId]->getPriority())) )
				{
					nmListOfObjects[techId]->disconnectFromNetwork();
					LOG(ULOG_DEBUG_LEVEL, "BM::NMStateHandler() Sending Disconnect to NM techId:%d priority:%d \n",techId,nmListOfObjects[techId]->getPriority());
				}
				else
				{
					/*reset the current active nm*/
					currentActiveNM = 0;

					LOG(ULOG_DEBUG_LEVEL, "BM::Active connected TechID:%d and connected Active state:%d\n",techId,currentState);

					//int p = techPriorityArray[techId];
					currentActiveNM = nmListOfObjects[techId];
				}
			}
		}
		break;
	default:
		break;
	}
}

void UBearerManager::NMManualModeStateHandler(int techId,U_UINT32 currentState)
{
	if(nmListOfObjects[techId]->state == NM_READY_STATE)
	{
		nmListOfObjects[techId]->connectToNetwork();
	}
}


void UBearerManager::initializeUCMLogger()
{
	string fileLocation = getExecDirectory("CMsocketservice.exe") + "log.txt";
	strcpy(getLogger()->logfilename_p,fileLocation.c_str());
	getLogger()->setFileLogEnable(1);
	getLogger()->setLoglevel(ULOG_DEBUG_LEVEL);
	//getLogger()->logfilename_p = (char*)fileLocation.c_str(); 
	/*if(int err = fopen_s(&getLogger()->logfile_p, getLogger()->logfilename_p, "w"))
	{
		printf("error in opening file");
	}*/
	LOG(ULOG_DEBUG_LEVEL,"Logger initialized!");

}

U_UINT32 UBearerManager::getCfgParam(U_UINT32 paramID)
{
	switch(paramID)
	{
		case NM_CFG_PRIORITY:
			{
			}
			break;
		case NM_CFG_ENABLED:
			{
			}
			break;
		default:
			return 0;
	}
#if 0
	case DATA_RATE:
		return gCMInfo.stats.data_rate;
	case DL_DATA:
		return gCMInfo.stats.dl_data;
	case UL_DATA:
		return gCMInfo.stats.ul_data;
	case CM_STATE:
		return gCMInfo.state;
	case MODEM_STATE:
		return gCMInfo.Modem_state;
	case SYSTEM_NOTIFICATION:
		return gCMInfo.system_notification;
	case CFG_CONFIG_MODE:
		return gCMInfo.cfg_params.Config_mode;
	case CFG_ENABLE_4G:
		return gCMInfo.cfg_params.Enable_4G;
	case CFG_LOGGING_LEVEL:
		return uLogger.getLogLevel();
	case CFG_ENABLE_LOG:
		return uLogger.getLogFileEnable();
    case SMS_WRITE_UICC:
        return gCMInfo.imsParams.Sms_writeUicc;
    case SMS_OVER_IMS:
        return gCMInfo.imsParams.Sms_Over_Ims;
    case SMS_FORMAT:
        return gCMInfo.imsParams.Sms_format;
    case T1_TIMER:
        return gCMInfo.imsParams.T1_timer;
    case T2_TIMER:
        return gCMInfo.imsParams.T2_timer;
    case TF_TIMER:
        return gCMInfo.imsParams.TF_timer;
    case IMS_REG_STATUS:
        return gCMInfo.imsParams.IMS_Reg_Status;
	case TEST_MODE:
		return gCMInfo.imsParams.Test_Mode;
	case MCC:
		return gCMInfo.mccVal;
	case MNC:
		return gCMInfo.mncVal;
    default:
        return 0;
    }
#endif
	return UCM_SUCCESS;
}


void UBearerManager::getEnabledTechList(U_UINT32* enabledTechs)
{
	LOG(ULOG_DEBUG_LEVEL, "BM::getEnabledTechList()");
	bool isLteEnabled = parserPtr->gCMInfo.isLTE_Enabled;
	bool isThreeGEnabled = parserPtr->gCMInfo.isThreeG_Enabled;
	bool isWifiEnabled = parserPtr->gCMInfo.isWIFI_Enabled;

	if(isLteEnabled == true)
	{
		enabledTechs[LTE_TECH] = 1;
	}
	else
	{
		enabledTechs[LTE_TECH] = 0;
	}
	
	if(isWifiEnabled == true)
	{
		enabledTechs[WIFI_TECH] = 1;
	}
	else
	{
		enabledTechs[WIFI_TECH] = 0;
	}

	if(isThreeGEnabled == true)
	{
		enabledTechs[THREE_G_TECH] = 1;
	}
	else
	{
		enabledTechs[THREE_G_TECH] = 0;
	}

}

void UBearerManager::getPriorityTechList()
{
	LOG(ULOG_DEBUG_LEVEL, "BM::getPriorityTechList()");
	UGetPriorityTechListRsp* priorityList = new UGetPriorityTechListRsp(this->techPriorityArray);
	BearerMngrEventCB(reinterpret_cast<UEvent*>(priorityList));

}

void UBearerManager::updateNMEnableToConfigFile()
{
	LOG(ULOG_DEBUG_LEVEL, "BM::updateNMEnableToConfigFile()");
	string tempEncodeStr;

	tempEncodeStr.clear();
	tempEncodeStr = parserPtr->convertIntToString(LTE_TECH);
	tempEncodeStr += ",";
	if(parserPtr->gCMInfo.isLTE_Enabled == true)
		tempEncodeStr += parserPtr->convertIntToString(1);
	else
		tempEncodeStr += parserPtr->convertIntToString(0);

	tempEncodeStr += ";";

	tempEncodeStr += parserPtr->convertIntToString(THREE_G_TECH);
	tempEncodeStr += ",";
	if(parserPtr->gCMInfo.isThreeG_Enabled == true)
		tempEncodeStr += parserPtr->convertIntToString(1);
	else
		tempEncodeStr += parserPtr->convertIntToString(0);

	tempEncodeStr += ";";

	tempEncodeStr += parserPtr->convertIntToString(WIFI_TECH);
	tempEncodeStr += ",";
	if(parserPtr->gCMInfo.isWIFI_Enabled == true)
		tempEncodeStr += parserPtr->convertIntToString(1);
	else
		tempEncodeStr += parserPtr->convertIntToString(0);

	string id="enable_Tech";
	parserPtr->updateValue(id,tempEncodeStr,false);
}

void UBearerManager::updateNMPriorityToConfigFile()
{
	LOG(ULOG_INFO_LEVEL, "BM::updateNMPriorityToConfigFile()");
	string tempEncodedStr;
	tempEncodedStr.clear();
	tempEncodedStr = parserPtr->convertIntToString(techPriorityArray[0]);
	tempEncodedStr += ";";
	tempEncodedStr += parserPtr->convertIntToString(techPriorityArray[1]);
	tempEncodedStr += ";";
	tempEncodedStr += parserPtr->convertIntToString(techPriorityArray[2]);
	string id="Tech_Priority";
	parserPtr->updateValue(id,tempEncodedStr,false);

}
void UBearerManager::updateParamToConfigFile(string tag,string param)
{
	string tempEncodedStr = "";
	string startTag = tag;
	startTag = "<" + tag + ">";
	string endTag = "</";
	parserPtr->updateStr(parserPtr->configFileContent,startTag,endTag+tag,param);	
	parserPtr->writeToConfigFile();

}

void UBearerManager::handleManualToAutoModeChange()
{
	for(unsigned int i=0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
	{
		U_UINT32 techId = techPriorityArray[i];
		if(nmListOfObjects[techId]->state == NM_CONNECTED_STATE)
		{
			
			currentActiveNM = nmListOfObjects[techId];
			for(unsigned int j=i+1;j<MAX_NUMBER_OF_NETWORKMANAGERS;j++)
			{
				U_UINT32 lowerTechId = techPriorityArray[j];
				if(nmListOfObjects[lowerTechId]->state == NM_CONNECTED_STATE)
				{
					nmListOfObjects[lowerTechId]->disconnectFromNetwork();
				}
			}
			return;
		}
		
	}
	for(unsigned int i=0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
	{
		U_UINT32 techId = techPriorityArray[i];
		if(nmListOfObjects[techId]->state == NM_READY_STATE)
		{
			currentActiveNM = nmListOfObjects[techId];
			nmListOfObjects[techId]->connectToNetwork();
			return;
		}
		
	}
	

}


void UBearerManager::updateBMModeToConfigFile()
{
	string mode = this->isBearerMngrManualMode ? "Manual" : "Auto";
	updateParamToConfigFile("BM_Mode",mode);
}

void UBearerManager::updateRasDetailsToConfigFile(Ras_Details_s& details)
{
	updateValueToConfigFile(string("RASProfile"),string(details.rasProfileName));
	updateValueToConfigFile(string("UserName"),string(details.rasUsrName));
	updateValueToConfigFile(string("PassWord"),string(details.rasPasswd));
	updateValueToConfigFile(string("PhoneNumber"),string(details.rasPhoneNumber));
	updateValueToConfigFile(string("3GAPNName"),string(details.apnName));
}

void UBearerManager::updateValueToConfigFile(string& id,string& value)
{
	parserPtr->updateValue(id,value);
}



