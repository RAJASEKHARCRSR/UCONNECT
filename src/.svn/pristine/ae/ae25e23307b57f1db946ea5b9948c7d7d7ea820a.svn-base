/*****************************************************************************
**
** uconn_LTE_NetworkMgr.cpp
** This source file describes the interfaces to the LTE Network Manager
**
** Author: Sathyanarayanan S
** Date  : 06-MAR-2012
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		06/03/12     SS			Creation
****************************************************************************/
#include "LTE_NM.h"
#include "uconn_ATHandler.h"
#include "uconn_AT_DAL.h"
#include "uconn_Broadcom_DAL.h"
#include "uconn_Altair_DAL.h"
#include "UHuaweiDal.h"
#include "uconn_server.h"


ULTE_NM::ULTE_NM(UEventQueue* bearerManagerQueue):UNetworkManager(bearerManagerQueue)
{
	/* NO references to gCMInfo */
	this->mCommonInfo.cfg_params.Enable_4G = false;	

}


ULTE_NM::~ULTE_NM()
{

	this->bearerMngrQueue = NULL;

	delete mNMServer;
	mNMServer = NULL;

	delete mATHandler;
	mATHandler = NULL;
}


U_VOID ULTE_NM::enableNetMngr()
{
	/* Set the DAL mode here */
	if(mCommonInfo.dalMode == 1)
	{
		mNMServer->getSM()->dal 
			= reinterpret_cast<uconnDal*>(new uconnBroadcomDal(this));
	}
	else if(mCommonInfo.dalMode == 2)
	{
		mNMServer->getSM()->dal 
			= reinterpret_cast<uconnDal*>(new uconnAltairDal(this));
	}
	else if(mCommonInfo.dalMode == 3)
	{
		mNMServer->getSM()->dal  = new UHuaweiDal(this);
	}

	/* ONLY FOR THE EMULATOR MODE */

	if(mCommonInfo.isEmulatorMode)
	{
		mCommonInfo.cfg_params.Enable_4G = 1;
		mCommonInfo.com_port = "COM3";
		mCommonInfo.imsParams.Test_Mode = TEST_MODE_ENABLE;
		//dalCallbackfn("", 3, this);
	}
	UNMEnable *evt = new UNMEnable(LTE_TECH);
	postToSM(evt);
}

U_VOID ULTE_NM::disableNetMngr()
{
	UNMDisable *evt = new UNMDisable(LTE_TECH);
	postToSM(evt);
}

U_VOID ULTE_NM::connectToNetwork()
{
	UNetattach *evt = new UNetattach(0);
	postToSM(evt);
}

U_VOID ULTE_NM::searchNetworks()
{

}

U_VOID ULTE_NM::disconnectFromNetwork()
{
	//UNMDisconnect *evt = new UNMDisconnect(LTE_TECH);
	UNetdetach* evt = new UNetdetach();
	postToSM(evt);
	//ucmsm_p->mLTE_NM->updateNMStateAndPost(LTE_TECH,NM_NOT_READY_STATE);
}

U_VOID ULTE_NM::initNetMngr(void *cfgInfo)
{
	
	LTE_Config_Info_s *lteConfigInfo = (LTE_Config_Info_s*)cfgInfo;

	mCommonInfo.isEmulatorMode = lteConfigInfo->EmulatorMode;
	mCommonInfo.dalMode = lteConfigInfo->DalMode;
	mNMInfo.isPowerdown = 1;
	this->mCommonInfo.device_status = 0;
	mNMInfo.pdp_type = lteConfigInfo->PDPType;
	mCommonInfo.imsParams.Test_Mode = lteConfigInfo->TestMode;
	strcpy(mNMInfo.VzAPN, lteConfigInfo->APNName.c_str());
	strcpy(mNMInfo.VzAPNIMS, lteConfigInfo->APNNameIMS.c_str());
	mNMInfo.FreqEARCFN = lteConfigInfo->EARCFN1;

	/* Copy VID/PIDs from config information */
	mNMInfo.VidPidCount = lteConfigInfo->vidpidCount;
	for(U_UINT32 i = 0; i < mNMInfo.VidPidCount; i++)
	{
		strcpy(mNMInfo.VidPidList[i], lteConfigInfo->VidPidName[i].c_str());
	}
	strcpy(mCommonInfo.hwIdCOMPort, lteConfigInfo->comPortHWId.c_str());
	
	dalCallbackfn = InitCallbackWithId;

	//Initialize the ATHandler instance 

	mATHandler = new UConnATHandler();

	mNMServer = new UConnServer();
	mNMServer->getSM()->UCMStartUp(this);
	mNMServer->initialize();
	mNMServer->getSM()->getState()->entry();

}

U_UINT32 ULTE_NM::setParam(U_UINT32 paramID,string param)
{
	return 0;
}

void ULTE_NM::getParam(Get_NM_Param paramID)
{
	string param = "";
	switch(paramID)
	{
	case NETWORK_NAME:
		{
			param = this->mNMInfo.apn_name;
			
		}
		break;
	case IP_ADDRESS:
		{
			param = this->mNMInfo.ip_address;

		}
		break;
	case VERSION:
	{
			
		param = this->mCommonInfo.versionInfo.u8VersionString;

	}
	break;
	default :
		break;
	}

	UBearerGetParamRsp* paramRsp = new UBearerGetParamRsp(LTE_TECH,paramID,param);
	postEventToBearerManager(reinterpret_cast<UEvent*>(paramRsp));

}

U_UINT32 ULTE_NM::getStatisticsNM()
{
	UUpdateStatsEvent* updateStatsEvt = new UUpdateStatsEvent();
	this->mNMServer->MsgPost((U_UINT8*)updateStatsEvt);	

	return UCM_SUCCESS;
}

ETechSelect ULTE_NM::getTechID()
{
	return LTE_TECH;
}

U_VOID ULTE_NM::postToSM(UEvent *evt)
{
	this->mNMServer->MsgPost((U_UINT8*)evt);
}

U_VOID ULTE_NM::deviceEventsCallBack(U_UINT32 cmdID,U_UINT32 techID)
{
	UDeviceStateChange *evt = NULL;
	if ( cmdID == 3 )
	{
		evt = new UDeviceStateChange(NOTI_DEVICE_OPEN);
	}
	else if ( cmdID == 4 )
	{
		evt = new UDeviceStateChange(NOTI_DEVICE_NOT_OPEN);
	}

	if(evt != NULL)
	{
		postToSM(evt);
	}
}

U_UINT32 ULTE_NM::getMeasStatisticsNM()
{
	UUpdateStatsEvent* updateStatsEvt = new UUpdateStatsEvent();
	this->mNMServer->MsgPost((U_UINT8*)updateStatsEvt);


	//UModemStatisticEvent *st = new UModemStatisticEvent(mNMInfo.m_nMeasStats);
	//this->postEventToBearerManager((UEvent*)st);
	return 1;
}