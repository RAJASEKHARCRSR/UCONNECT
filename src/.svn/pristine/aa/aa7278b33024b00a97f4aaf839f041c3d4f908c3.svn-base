#ifndef UTHREEG_NM_H_
#define UTHREEG_NM_H_

#include <uconn_OSAL.h>


#ifdef WIN32
#include <ras.h>
#include <raserror.h>
#pragma comment(lib, "rasapi32.lib")
#endif


#include "UBufferParser.h"
#include "NetworkManager.h"



typedef struct Ras_Stats
{
public:
	Ras_Stats()
	{
		memset(this, 0, sizeof(Ras_Stats));
	}
	U_UINT32 ul_data;
	U_UINT32 dl_data;
	U_UINT32 duration;
}Ras_Statistics_s;
	

class URasConnection
{
public:
	URasConnection(void);
	~URasConnection(void);

	/* 	Functions for the RAS connection */
	U_DWORD InitRas(U_CSTR rasName, U_CSTR rasusrname, U_CSTR raspasswd, U_CSTR PhoneNo);
	U_DWORD CreateRasProfile(U_CSTR rasName, U_CSTR rasusrname, U_CSTR raspasswd, U_CSTR PhoneNo);

	U_DWORD DialRas(U_CSTR rasusrname, U_CSTR raspasswd);
	bool HangUpRas();
#ifdef WIN32
	RASCONNSTATE GetRasConnectionStatus(void);
    static VOID WINAPI RasDialEventNotifier (UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError);
    static HRASCONN URasConnHandle;
#endif
    bool GetRasConnectionStatistics(Ras_Stats *rasstats);

	bool GetRasisConnected(void);
	bool GetRasisDialing(void);
	
//	void HandleError (U_UINT32 ErrorCode);
	/* variables to declare if the RAS is connected or is dialling. */
	static bool URasIsConnected;
	static bool URasIsDialing;
	static U_DWORD RasError;

	
private: 
	
	
	/* No of RAS connection enteries. */
//	U_UINT32 RasEntriesCount; 
	
	/* The RAS enteries names. */
	unsigned char *RasEntriesNames;

	Ras_Stats rasstatistics;
};	


typedef struct ThreeG_PLMN_List 
{
public:
	ThreeG_PLMN_List()
	{
		memset(this, 0, sizeof(ThreeG_PLMN_List));
	}
	U_UINT32 PLMN_id;				/*	PLMN identity. */
	U_UINT32 rat;					/* Access technology of the cell. If NULL implies no access technology returned by underlying stack. 
									 * GSM, GSM Compact, UTRAN, GSM w/EGPRS, UTRAN w/HSDPA, UTRAN w/HSUPA, UTRAN w/HSDPA and HSUPA, E-UTRAN */
	U_UINT32 Stat;					/* Availability of the operator - Unknown, Available, Current, Forbidden */
	char free_text[MAX_FREE_TEXT];  /* General Network Information */
} ThreeG_PLMN_List_s;


typedef struct ThreeG_Config_Param
{
public:
	ThreeG_Config_Param()
	{
		memset(this, 0, sizeof(ThreeG_Config_Param));
	}
	U_UINT32 Config_mode; /* Auto or Manual Mode */
	U_UINT32 Attach_RAT;  /* ?? */
	U_UINT32 Log_level;   /* The level of Logging */
	U_UINT32 Enable_Log;  /* Enable or Disable Logging */
}ThreeG_Config_Param_s;


typedef struct ThreeG_NM_Stats
{
	U_SINT32 rssi;
    U_SINT32 cinr;
	Ras_Stats rasstats;
public:
	ThreeG_NM_Stats()
	{
		rssi=0;
		cinr=0;
	}
}ThreeG_NM_Statistics_s;


typedef struct ThreeG_NM_Info
{
	//U_UINT32 priority;		 /* Priority of the 3G NM set by the user. */
	//U_UINT32 device_status;  /* Connected / Open and Not connected / closed. */

	U_UINT32 reg_status;		/* Registration status as per 27.001, filled from CREG(sec7.2) or CGREG(sec 10.1.20) */
	//U_UINT32 roaming_status;	/* Roaming Status as per 27.001, filled from CREG(sec7.2) or CGREG(sec 10.1.20) */ 
	U_UINT32 rat;	/* Access Technology of the serving cell, filled from CREG(sec7.2) or CGREG(sec 10.1.20) */
	
	
	Config_Param_s cfg_params;
	  
	/* Interface Name */
	U_SINT8 interface_name[40];
	U_SINT8 VidPidList[3][100];
	U_UINT32 VidPidCount;

	/* State information */
	CSCM_State state;
			
	/* Timer Event ID */
	U_UINT32 timer_id;

	bool NetworkSearchOn;
	bool EnableCommandRecd; 
	U_BOOL UssdDecodingReqd;

	ThreeG_NM_Statistics_s stats;

	/* Equip_info parameters */
	U_UINT32 IMEI;
	U_UINT8 description[50]; /* Description of the mobile device */
	U_UINT8 manufacturer[20];
	U_UINT8 model[10];
	U_UINT8 revision[10];
	char network_name[50];

public:
	ThreeG_NM_Info()
	{
		reg_status = 0;	 
		rat = 0xFFFF;

		timer_id = 0;

		NetworkSearchOn = false;
		EnableCommandRecd = false; 

		IMEI = 0;
		memset(description, 0, 50*sizeof(U_UINT8));
		memset(manufacturer, 0, 20*sizeof(U_UINT8));
		memset(model, 0, 10*sizeof(U_UINT8));
		memset(revision, 0, 10*sizeof(U_UINT8));
		memset(network_name, 0, 50*sizeof(char));
	}
}ThreeG_NM_Info_s;


class UThreeG_NM : public UNetworkManager
{
public:
	ThreeG_NM_Info ThreeGNMInfo;
	UThreeG_NM(UEventQueue* msgQueue);
	~UThreeG_NM();
	/* functions defined from base class UProcess
	 */
//	U_VOID Reset() { }
//	U_BOOL	EventHandler();

	/* functions defined from base class UNetworkManager
	 */
	U_VOID enableNetMngr();
	U_VOID disableNetMngr();
	
	U_VOID connectToNetwork();
	U_VOID searchNetworks();
	U_VOID disconnectFromNetwork();
	U_VOID initNetMngr(void *configInfo);
	U_UINT32 setParam(U_UINT32 paramID,string param);

	U_VOID getParam(Get_NM_Param paramID);
	U_UINT32 getStatisticsNM();
	ETechSelect getTechID();
	U_VOID postToSM(UEvent *evt);

	U_VOID deviceEventsCallBack(U_UINT32 cmdID,U_UINT32 techID); 
	
	U_VOID (*dalCallbackfn)(U_STRING strResp,
					U_UINT32 cmdId,
					UNetworkManager* p_NM);
	U_VOID handleEvent(UEvent* evt){
		this->postToSM(evt);
	}	
public:

    URasConnection ThreeGRasConnection;
	
	/* Private functions of ThreeGNM class */
    void PostEventToThreeGNM(UEvent* evt);

	bool EnableThreeGNM();
	bool DisableThreeGNM();

	void ChangeStateAndInformBM(ENMStateForBM NewState);
	void DisconnectThreeGNM();
	string EncodeSearchNetworkResponse();
	string EncodeStatsString();
	U_VOID HandleRasTimer();
	U_VOID HandleUSSDTimer();
	U_VOID Handle3GStatsTimerExpiry();
	U_VOID Update3GParams(U_UINT32 ParamId, U_UINT32 value1, U_UINT32 value2);
	U_VOID Update3GNMState();
	void EncodeUSSDString(unsigned char* USSDStr);
	void HandleRasError (U_UINT32 ErrorCode);

	bool NetworkSearchOn ;
};


enum EModemMode
{
	eMODE_2G_ONLY,
	eMODE_2G_PREF,
	eMODE_3G_ONLY,
	eMODE_3G_PREF,
	eMODE_ANY
};

enum EModemBand
{
	eBAND_GSM,
	eBAND_EGSM,
	eBAND_PGSM,
	eBAND_DCS,
	eBAND_PCS,
	eBAND_WCDMA,
	eBAND_ANY
};

#endif //UTHREEG_NM_H_