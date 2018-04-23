/*****************************************************************************
**	BearerManager.h
** This header file describes the class called BearerManager
**
** Author: Suresh Lingadalli
** Date  : 01-MAR-2012
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		01/03/12     SS			Creation
****************************************************************************/

#ifndef UBEARER_MANAGER_H_
#define UBEARER_MANAGER_H_

#include <list>
#include <cstdlib>
#include "NetworkManager.h"
#include "uconn_Config.h"
#include "LTE_NM.h"
#include "ThreeG_NM.h"
#include "WiFi_NM.h"
#include "Ether_NM.h"
#include "ULog.h"
//#include "UVpn.h"
#include "UThreeGSimulator.h"

#include "XMLParser.h"



#if 0
typedef enum ETechSelect
{
	INVALID_TECH = 0,
	LTE_TECH = 1,
	THREE_G_TECH = 2, 
	WIFI_TECH = 3,
	ETHER_TECH = 4,
	BLUETOOTH = 5,
	VPN_TECH = 6,
	VPN_TETHERING = 7,
	ALL_TECH = 20
};
#endif

typedef enum EResponseType
{
	/*1 for U_UINT32,2 for U_FLOAT and 3 for U_STRING*/
	INVALID_TYPE_RESP = 0,
	U_UINT32_TYPE_RESP = 1,
	U_FLOAT_TYPE_RESP = 2, 
	U_STRING_TYPE_RESP = 3
};

#if 0
typedef enum EDeviceType
{
	INVALID_DEVICE_TYPE = 0,
	BROADCOM_DEVICE_TYPE = 1,
	ALTAIR_DEVICE_TYPE = 2, 
	INTEL_DEVICE_TYPE = 3,
};
#endif

typedef enum ELTETechParam
{
	INVALID_PARAM = 0,
	DATA_RATE_PARAM,
	DL_DATA_PARAM, 
	UL_DATA_PARAM,
	CM_STATE_PARAM,
	MODEM_STATE_PARAM,
	SYSTEM_NOTIFICATION_PARAM,
	CFG_CONFIG_MODE_PARAM,
	CFG_ENABLE_4G_PARAM,
	CFG_LOGGING_LEVEL_PARAM,
	CFG_ENABLE_LOG_PARAM,
	SMS_WRITE_UICC_PARAM,
	SMS_OVER_IMS_PARAM,
	SMS_FORMAT_PARAM,
	T1_TIMER_PARAM,
	T2_TIMER_PARAM,
	TF_TIMER_PARAM,
	IMS_REG_STATUS_PARAM,
	TEST_MODE_PARAM,
	IS_EMULATOR_MODE_PARAM,
	DAL_MODE_PARAM,
	EARCFN_PARAM,
	RSSI_VAL_PARAM,
	CINR_VAL_PARAM,
	SIG_STRENGTH_PARAM
};

typedef enum ETHREEGTechParam
{
};

typedef enum EWIFITechParam
{
};

typedef enum EETHERTechParam
{
};

typedef struct BearerConnectStruct
{
	ETechSelect tech;
	U_UINT32 PLMNId;
	U_SINT8 ssid[32];
	U_SINT8 password[30];
public:
	BearerConnectStruct()
	{
		PLMNId = 0;
		ssid[0] = '\0';
		password[0] = '\0';
		tech = INVALID_TECH;
	}
}BearerConnectStruct_s;

typedef struct BearerDisConnectStruct
{
	ETechSelect tech;
public:
	BearerDisConnectStruct()
	{
		tech = INVALID_TECH;
	}
}BearerDisConnectStruct_s;

typedef struct BearerSearchStruct
{
	ETechSelect tech;
	U_UINT32 searchTimeOutPeriod;
public:
	BearerSearchStruct()
	{
		tech = INVALID_TECH;
		searchTimeOutPeriod = 0;
	}
}BearerSearchStruct_s;

typedef struct BearerInfoStruct
{
	ETechSelect tech;
public: 
	BearerInfoStruct()
	{
		tech = INVALID_TECH;
	}
}BearerInfoStruct_s;


typedef struct LogInfoStruct
{
	ETechSelect tech;
	/*0 for get and 1 for set*/
	U_UINT32 getOrSetIndicator;
	/*0 for get and 1 for set*/
	U_UINT32 logOnOrOff;
	/*0 for Info and 1 for Debug and 2 for Error and 3 for Critical and 4 for Invalid*/
	U_UINT32 logLevel;
public:
	LogInfoStruct()
	{
		tech = INVALID_TECH;
		getOrSetIndicator = 0;
		logOnOrOff = 1;
		logLevel = 4;
	}
}LogInfoStruct_s;

typedef struct BearerStatParamStruct
{
	ETechSelect tech;
	//ELTETechParam param;
	U_UINT32 param;
public:
	BearerStatParamStruct()
	{
		tech = INVALID_TECH;
		param = INVALID_PARAM;
	}

}BearerStatParamStruct_s;

typedef struct BearerStatParamRespStruct
{
	ETechSelect tech;
	EResponseType respType;
	U_UINT32 unValue;
	U_FLOAT  fltValue;
	U_STRING strValue;
public:
	BearerStatParamRespStruct()
	{
		tech = INVALID_TECH;
		respType = INVALID_TYPE_RESP;
		unValue = 0;
		fltValue = 0.0f;
		strValue.clear();
	}
}BearerStatParamRespStruct_s;

#if 0
typedef struct BearerSetStatParamStruct
{
	ETechSelect tech;
	//ELTETechParam param;
	U_UINT32 param;
	U_UINT32 value;
public:
	BearerSetStatParamStruct()
	{
		tech = INVALID_TECH;
		param = 0;
		value = 0;
	}

}BearerSetStatParamStruct_s;
#endif

#if 1
typedef struct BearerStatisticsStruct
{
	ETechSelect tech;
	/*0 Invalid,1 for Broadcom, 2 for Altair*/
	U_UINT32 deviceType;
public:
	BearerStatisticsStruct()
	{
		tech = INVALID_TECH;
		deviceType = 0;
	}

}BearerStatisticsStruct_s;
#endif

typedef struct BearerVersionInfoStruct
{
	ETechSelect tech;
	/*0 Invalid, 1 for Broadcom, 2 for Altair*/
	U_UINT32 deviceType;
public:
	BearerVersionInfoStruct()
	{
		deviceType = 0;
		tech = INVALID_TECH;
	}
}BearerVersionInfoStruct_s;

typedef struct BearerVersionInfoRespStruct
{
	ETechSelect tech;
	Version_Info_s verInfo;
	/*0 Invalid, 1 for Broadcom, 2 for Altair*/
	U_UINT32 deviceType;
public:
	BearerVersionInfoRespStruct()
	{
		deviceType = 0;
		tech = INVALID_TECH;
		verInfo.u8VersionString[0] = '\0';
	}
}BearerVersionInfoRespStruct_s;

typedef struct BearerATCommandStruct
{
	ETechSelect tech;
	U_SINT8 sATCmdReqStr[MAX_AT_COMMAND_REQUEST];
public: 
	BearerATCommandStruct()
	{
		tech = INVALID_TECH;
		memset(this, 0, MAX_AT_COMMAND_REQUEST * sizeof(U_SINT8)); 
	}
}BearerATCommandStruct_s;


class UBearerManager : public UProcess
{
private:

	void initializeUCMLogger();
	void updateParamToConfigFile(string tag,string param);
	void updateBMModeToConfigFile();
	void updateRasDetailsToConfigFile(Ras_Details_s& details);
	void updateValueToConfigFile(string& id,string& value);

public:
	UBearerManager();
	~UBearerManager();

public:
	XMLParser *parserPtr;

	//WiFi_ConfigInfo_s *pwifiNMInfo;

	//list <UNetworkManager *> l;
	//list<UNetworkManager *>::iterator it;
public:
	UNetworkManager **nmListOfObjects;
	UNetworkManager *currentActiveNM;
	UNetworkManager **currentActiveNM1;
	U_UINT32 *techPriorityArray;
	U_BOOL isBearerMngrManualMode;
public:
	/*UCM initialise over here*/
	void UCM_Init();

	/*To handle all events from the GUI*/
	UEvent* handle_ClientCommand(UEvent *reqFromClient_p);

	/*To enable the bearer*/
	U_SINT32 enableBearer(int techID);/*1--LTE, 2--3G, 3--WiFi, 4--Ether*/
	
	/*To disable the bearer*/
	U_SINT32 disableBearer(int techID);/*1--LTE, 2--3G, 3--WiFi, 4--Ether*/
	
	/*To connect to the bearer*/
	//U_SINT32 connectBearer(BearerConnectStruct_s *bearerConnPtr);
	U_SINT32 connectBearer(int techID);
	
	/*To Dis-connect with the bearer*/
	//U_SINT32 disConnectBearer(BearerDisConnectStruct_s *bearerDisConnPtr);
	U_SINT32 disConnectBearer(int techID);
	
	/*scan wifi-- search LTE n/w-- */
	//U_SINT32 searchBearerNetwork(BearerSearchStruct_s *bearerSearchPtr);
	U_SINT32 searchBearerNetwork(int techID);
	
	/*config change say priority change,enable/disable bearer etc*/
	U_SINT32 updateUCMConfigFile(BearerInfoStruct_s *bearerInfoPtr);
	
	/*config change say priority change,enable/disable bearer etc*/
	U_SINT32 updateBearerMngrWithConfigChange(string paramName,string paramValue);
	
	/*enable bearer log */
	U_SINT32 enableBearerLog(U_UINT32 techID);

	/*disable bearer log */
	U_SINT32 disableBearerLog(U_UINT32 techID);

	/*bearer log level setting*/
	U_SINT32 bearerLogLevel(U_UINT32 techID,U_UINT32 logLevel);
	
	/*set loglevel*/
	U_SINT32 setBearerLogLevel(LogInfoStruct_s *logPtr);
	
	/*get stat param*/
	U_SINT32 getStatParam(BearerStatParamStruct_s *bearerStatParamPtr);

	/*To get stat param with value independent of data type*/
	U_SINT32 getStatParamWithValue(BearerStatParamStruct_s *bearerStatParamPtr, BearerStatParamRespStruct_s *bearerStatParamRespPtr);

	/*set the bearer param*/
	//U_SINT32 setStatParam(BearerSetStatParamStruct_s *bearerStatParamPtr);
	U_SINT32 setBearerParam(U_UINT32 techID,U_UINT32 paramID,string valStr);

	/*get the bearer param*/
	U_SINT32 getBearerParam(U_UINT32 techID,Get_NM_Param paramID);

	//U_SINT32 bearerPriorityChanged(U_UINT32 techID,U_UINT32* techPriority);
	U_SINT32 bearerPriorityChanged(UBearerPriorityChanged *bearerPriority);

	U_SINT32 bearerModeChanged(U_UINT32 techID, U_UINT32 bearerMode);

	/*To get bearer statistics*/
	//U_SINT32 getBearerStatistics(BearerStatisticsStruct_s *bearerStatParamPtr);
	U_SINT32 getBearerStatistics(U_UINT32 techID);
	U_SINT32 getBearerMeasStatistics(U_UINT32 techID);

	/*get version information*/
	U_SINT32 getVersionInfo(BearerVersionInfoStruct_s *bearerVersionInfoPtr,BearerVersionInfoRespStruct *bearerVersionInfoRespPtr);
	
	/*To send AT command with pass through Mode*/
	U_SINT32 sendATCommandInPassThroughMode(BearerATCommandStruct_s *bearerATCmdPtr);

	/*To handle callbacks from Devices*/
	U_VOID handleCallBacksFromDevice(U_UINT32 cmdId,U_UINT32 techId);

	/*To give xml config value to the GUI*/
	U_UINT32 getCfgParam(U_UINT32 paramID);

	/* to get the state of the NM */
	U_SINT32 getNMCurrentState(U_UINT32 techID);

	//returns enabled techs
	void getEnabledTechList(U_UINT32* enabledTechs);

	//returns techId array according to their priority
	void getPriorityTechList();

public:
	
	U_VOID (*eventBearerMngrCBHdlr)(UEvent *);
	//virtual void Reset() { }
	U_BOOL	EventHandler();
	
	/*create thread for message handle from verious network managers*/
	void initialize();

	/*To call event callbacks to GUI */
	U_VOID BearerMngrEventCB(UEvent *event_p);

	/*queue message handler in a saparate thread which fetches always from NMs to BM*/
	U_UINT32 queueMessageHandler(UEvent *evt);

	/*handle NM states ment for BM to handle arbitration logic here*/	
	void NMAutoModeStateHandler(int techId,U_UINT32 currentState);
	void NMManualModeStateHandler(int techId,U_UINT32 currentState);
	void handleManualToAutoModeChange();
	void NMPriorityHandler();
	void priorityChange(U_UINT32 techId,U_UINT32 priority);
	void updateNMEnableToConfigFile();
	void updateNMPriorityToConfigFile();
	
};

#endif //UBEARER_MANAGER_H_