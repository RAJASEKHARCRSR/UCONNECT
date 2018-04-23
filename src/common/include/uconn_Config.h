/*****************************************************************************
**
** uconn_Config.h
** This file has the data structure for the CM Configuration and also the 
** search network list with its status
**
** Author: Sathyanarayanan S
** Date  : 29-JAN-11
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		29/01/11     SS			Creation
****************************************************************************/
#ifndef UCONN_CONFIG_H
#define UCONN_CONFIG_H

#include "UTypes.h"
#define vidPidCountMax 3 
typedef enum
{
	OFF_4G = 0,
	ON_4G = 1
}Enable4G;

typedef enum Pdp_Type
{
	PDP_IPV4 = 1,
	PDP_IPV6 = 2,
	PDP_IPV4V6 = 3,
	PDP_IP	= 4
}ePdp_Type;

typedef enum Context_status
{
	PRIMARY_ACTIVATED = 0, 
	PRIMARY_DEACTIVATED,
	SECONDARY_ACTIVATED,
	SECONDARY_DEACTIVATED
}eContext_status;

typedef enum System_Event
{
	NOTI_DEVICE_ERROR = 1,
	NOTI_DEVICE_NOT_OPEN,
	NOTI_DEVICE_OPEN,
	NOTI_DISCONNECTED,
	NOTI_DEVICE_CONNECTED,
	NOTI_DEVICE_DISCONNECTED
}eSystem_Event;

typedef enum IMS_Status
{
	IMS_REG_SUCCESS = 1,	
	IMS_REG_FAILURE,
	IMS_DEREG,
	REQ_ATTACH,
	REQ_DETACH,
	REQ_POR,
	REQ_IMS_LTD_MODE
}eIMS_Status;

/* CM Info - response for the netsearch command */

typedef struct PLMN_List 
{
	public:
	PLMN_List()
	{
		memset(this, 0, sizeof(PLMN_List));
	}
	U_UINT32 PLMN_id; //	PLMN identity.
	U_UINT32 rat;	   // Access technology. If NULL implies no access 
				   // technology returned by underlying stack.
	U_UINT32 list_membership; 
	// General Network Information
	char free_text[MAX_FREE_TEXT];

} PLMN_List_s;

typedef struct NetSearch_Result {
	U_UINT32 net_search_status;
	U_UINT32 num_plmns;
	PLMN_List_s nw_list[MAX_NUM_PLMN];			
}NetSearch_Result_s;

enum CSCM_State
{
	ENABLED_STATE = 1000,
	DISABLED_STATE,
	IDLE_STATE,
	SEARCHING_STATE,
	CONNECTING_STATE,
	CONNECTED_STATE,
	DISCONNECTING_STATE,
	DEFINE_PRIMARY_STATE,
	DEFINE_SECONDARY_STATE,
    WAITING_FOR_REGSTATUS_ACT,
    WAITING_FOR_PRIMARY_ACT,
	NET_ATTACH_STATE
};

/* Primary and Secondary contexts */
typedef struct Primary_ctx
{

	U_SINT32 context_id;
	U_UINT32 status;
public:
	Primary_ctx()
	{
		memset(this, 0, sizeof(Primary_ctx));
	}
}Primary_ctx_s;

typedef struct Secondary_ctx
{
	U_SINT32 context_id;
	U_UINT32 status;
public:
	Secondary_ctx()
	{
		memset(this, 0, sizeof(Secondary_ctx));
	}
}Secondary_ctx_s;

/* Config Parameter structure */
typedef struct Config_Param
{
	U_UINT32 Enable_4G;
	U_UINT32 Config_mode;
	U_UINT32 Attach_RAT;
	U_UINT32 Disable_RAT;
	U_UINT32 Log_level;
	U_UINT32 Enable_Log;
public:
	Config_Param()
	{
		memset(this, 0, sizeof(Config_Param));
	}
}Config_Param_s;

typedef struct IMS_param
{
    U_UINT32 Sms_writeUicc;
    U_UINT32 Sms_format;
    U_UINT32 Sms_Over_Ims;
    U_UINT32 T1_timer;
    U_UINT32 T2_timer;
    U_UINT32 TF_timer;
    U_UINT32 Test_Mode;
    U_UINT32 IMS_Reg_Status;
	U_UINT32 IMS_Attach_Only;
public:
	IMS_param()
	{
		memset(this, 0, sizeof(IMS_param));
	}
}IMS_param;

typedef struct CM_Stats
{
	U_SINT32 rssi;
    U_SINT32 cinr;
	U_UINT32 data_rate;
	U_UINT32 ul_data;
	U_UINT32 dl_data;
public:
	CM_Stats()
	{
		memset(this, 0, sizeof(CM_Stats));
	}
}CM_Statistics_s;

/* Modem Statistics structure - as per the Modem */
/* May need change as per the change in DAL */

/// \brief DL TB stats


typedef struct UModem_stats
{
	U_UINT32 uiTotalStatsLength;
	U_UINT32 uiGenericStatsLength;	   	
    //UCMGenericStats_t sGenericStats;
	U_UINT32 uiPHYStatsLength;                
    
	U_UINT32 uiMacstatsLength;
public:
	UModem_stats()
	{
		memset(this, 0, sizeof(UModem_stats));
	}
}Modem_stats_s;


typedef struct UCMVL1_Stats_t
{
    U_UINT32     NumCRnti;
    U_UINT32     NumPRnti;
    U_UINT32     NumRaRnti;
    U_UINT32     NumSiRnti;
    U_UINT32     NumSpsRnti;
    U_UINT32     NumTCRnti;
public:
	UCMVL1_Stats_t()
	{
		memset(this, 0, sizeof(UCMVL1_Stats_t));
	}
}UCMVL1_Stats_t;

typedef struct UCMTimer_Stats_t
{
    U_UINT32     T3402_triggered;
    U_UINT32     T3402_expired;
    U_UINT32     T3411_triggered;
    U_UINT32     T3411_expired;
    U_UINT32     T311_triggered;
    U_UINT32     T311_expired;
    U_UINT32     T300_triggered;
    U_UINT32     T300_expired;
    U_UINT32     T301_triggered;
    U_UINT32     T301_expired;
public:
	UCMTimer_Stats_t()
	{
		memset(this, 0, sizeof(UCMTimer_Stats_t));
	}
}UCMTimer_Stats_t;

typedef struct UMeasurement_t
{
    //U_UINT32 measType;
	U_SINT32 reported;
    U_SINT32 Rx0Tx0;
    U_SINT32 Rx0Tx1;
    U_SINT32 Rx1Tx0;
    U_SINT32 Rx1Tx1;
	public:
	UMeasurement_t()
	{
		memset(this, 0, sizeof(UMeasurement_t));
	}
}UMeasurement_t;

typedef struct 
{
        
    /*UCMUSBStats_t usbStats;
    UCMPDMStats_t PDMStats;
    UCMPDCPStats_t PDCPStats;*/
    //UCMRLCStats_t RLCStats;
    //UCmMACStats_t MACStats;
    //UCML1AStats_t L1AStats;
    UMeasurement_t RSRP_Report;
    UMeasurement_t RSRQ_Report;
    UMeasurement_t SINR_Report;
    //UCMRRC_Stats_t RRCStats;
    //UCMNAS_Stats_t NASStats;
    //UCMVL1_Stats_t VL1Stats;
    //UCMTimer_Stats_t TimerStats;
    U_SINT32 RSRP_Reported;
    U_SINT32 RSRQ_Reported;
    U_SINT32 SINR_Reported;
}UAltairStats_t;




typedef struct L1A_Stats
{
	/* Only a L1A stats*/ 
	U_SINT8 L1AencodStr[3500] ;
public:
	L1A_Stats()
	{
		memset(this, 0, sizeof(L1A_Stats));
	}
}L1A_Stats_s;

typedef struct Meas_Stats
{
public:
	 Meas_Stats(){
		memset(this,0,sizeof(Meas_Stats));
	}
	/* Only a Measurement stats*/ 
	U_SINT8 RSRPencodStr[200];
	U_SINT8 RSRQencodStr[200];
	U_SINT8 SINRencodStr[200];
        U_SINT8 dlEarfcn[40];
		U_SINT8 bandcap[40];
		U_SINT8 bands[20];
		U_SINT8 cellID[20];
		U_SINT8 bandwidth[20];
		U_SINT8 rssiVal[20];
		U_SINT8 cinrVal[20];
		

}Meas_Stats_s;

typedef struct Broadcom_Stored_Stats
{
	double rssiAnt0;
	double rssiAnt1;
	U_UINT32 u32DLBw;
        U_UINT32 u32ULBw;
	float deviceCinr;
public:
	Broadcom_Stored_Stats()
	{
		memset(this, 0, sizeof(Broadcom_Stored_Stats));
	}
}Broadcom_Stored_Stats_s;


typedef struct Broadcom_Stats
{
	U_SINT8 genStats[500];
	U_SINT8 macStats[1000];
	U_SINT8 phyStats[1500];

	U_UINT32 u32DLThruput;
    U_UINT32 u32ULThruput;
public:
	Broadcom_Stats()
	{
		memset(this, 0, sizeof(Broadcom_Stats));
	}
}Broadcom_Stats_s;


typedef struct Altair_Stored_Stats
{
	U_SINT32 RSSI;
	U_SINT32 CINR;
public:
	Altair_Stored_Stats()
	{
		memset(this, 0, sizeof(Altair_Stored_Stats));
	}
}Altair_Stored_Stats_s;

typedef struct Version_Info
{
	/* Only a Version string is required */ 
	U_SINT8		u8VersionString[MAX_VERSION_STRING_LEN];
public:
	Version_Info()
	{
		memset(this, 0, sizeof(Version_Info));
	}
}Version_Info_s;


typedef struct CM_Info
{
	/* Refer to Config_mode above */
	U_UINT32 lte_reg_status;	
	U_UINT32 roaming_status;
	U_UINT32 system_notification;
	U_UINT32 ipConfigUP;

	Config_Param_s cfg_params;

	/* Interface Name */
	U_SINT8 interface_name[40];
	U_SINT8 VidPidList[3][100];
	U_UINT32 VidPidCount;
	U_SINT8 Adapter_desc[60];
	U_SINT8 Adapter_name[50];	
	U_SINT8 Adapter_index[10];
	U_SINT8 HW_Addr[40];
	U_UINT32 APNId;

	/* State information */
	CSCM_State state;
	U_UINT32 Modem_state;	/* 0 - Disconnected, 1 - Connected */

	/* RAT Information */
	U_UINT32 rat;

	/* Timer Event ID */
	U_UINT32 timer_id;

	NetSearch_Result_s search_results; /* To be returned on netsearch_status() */
	/* Add the Statistics parameters */

	void* allStats;

	CM_Statistics_s stats;
	Modem_stats_s modemStats;
    IMS_param  imsParams;
	/* Equip_info parameters */
	U_UINT32 batt_chrg;
	U_UINT32 IMEI;
	U_UINT32 power_mode;
	U_UINT8 revision[10];

	U_UINT32 pdp_type;
	/* Net_attach result parameters */
	U_SINT8 ip_address[MAX_ADDRESS_SIZE];
	U_SINT8 subnet_mask[MAX_ADDRESS_SIZE];
	U_SINT8 gw_address[MAX_GW_ADDRESS_SIZE];
	U_SINT8 dyn_dns[MAX_DNS_PRIMARY_ADDRESS_SIZE];
	U_SINT8 pri_dns[MAX_ADDRESS_SIZE];
	U_SINT8 sec_dns[MAX_ADDRESS_SIZE];
	U_SINT8 p_cscf_prim_addr[MAX_ADDRESS_SIZE];
	U_SINT8 p_cscf_sec_addr[MAX_ADDRESS_SIZE];
	U_SINT8 apn_name[MAX_APN_SIZE];

	/*Related to ipv6 format*/
	U_SINT8 ipv6_address[MAX_ADDRESS_SIZE];
	U_SINT8 subnetv6_mask[MAX_ADDRESS_SIZE];
	U_SINT8 gwv6_address[MAX_ADDRESS_SIZE];
	U_SINT8 dyn_dnsv6[MAX_DNS_PRIMARY_ADDRESS_SIZE];
	U_SINT8 priv6_dns[MAX_ADDRESS_SIZE];
	U_SINT8 secv6_dns[MAX_ADDRESS_SIZE];
	U_SINT8 p_cscfv6_prim_addr[MAX_ADDRESS_SIZE];
	U_SINT8 p_cscfv6_sec_addr[MAX_ADDRESS_SIZE];


	/* Context Information - needs to go into the Config file */
	Primary_ctx_s primary_ctx;
	Secondary_ctx_s secondary_ctx;

	/* Timer ID definitions */
	//U_TIMERID srchTimer; // //asper at_command
	//U_TIMERID updTimer;

	/* Flag to signal the Callback registration */
	U_UINT32 bCallback_Reg;
	U_SINT8 VzAPN[MAX_APN_SIZE];
	U_SINT8 VzAPNIMS[MAX_APN_SIZE];
	U_UINT32 APN_Inactivity_timer;

	/*boolean for stat request response received*/
	U_BOOL bUpdateStatsResponsePending;

	/*Altair Specific Stats*/
	UAltairStats_t m_nAltairStats;

	/*L1A Stats*/
	L1A_Stats_s m_nL1AStats;

	/*All Meas Stats*/
	Meas_Stats_s m_nMeasStats;

	/*All BRoadcom Stats*/
	Broadcom_Stats_s m_nBroadcomStats;

	Broadcom_Stored_Stats_s m_nBroadcomStoredStats;

	Altair_Stored_Stats_s m_nAltairStoredStats;
	U_BOOL isEmulatorMode;

	U_UINT8 dalMode;
	

	/*used at CLI for AT command mode*/
	U_UINT32 ATCmdMode;

	U_SINT32 FreqEARCFN;
	U_SINT32 Signal_Strength;
	
		/* Flag to determine if PowerDown is required on issuing a Detach */
	U_UINT32 isPowerdown;

	U_UINT32 context_id;

	U_UINT32 attemptToOpenSerialPort;
public: 
	CM_Info()
	{
	lte_reg_status=0;	
	roaming_status=0;
	system_notification=0;
	ipConfigUP=0;

	memset(interface_name, 40*sizeof(U_SINT8), 0);

	memset(interface_name, 40*sizeof(U_SINT8), 0);
	for (int row=0; row<3; row++) 
	{
		for (int col=0; col<100; col++) 
		{
			VidPidList[row][col] = 0;
		}
    }
	memset(interface_name, 40*sizeof(U_SINT8), 0);

	VidPidCount=0;

	memset(Adapter_desc, 60*sizeof(U_SINT8), 0);
	memset(Adapter_name, 50*sizeof(U_SINT8), 0);
	memset(Adapter_index, 10*sizeof(U_SINT8), 0);
	memset(HW_Addr, 40*sizeof(U_SINT8), 0);
	APNId=0;

	Modem_state = 0;

	rat = 0;

	timer_id = 0;

	allStats = NULL;

	batt_chrg = 0;
	IMEI = 0;

	power_mode = 0;
	memset(revision, 10*sizeof(U_UINT8), 0);

	pdp_type = 0;
	memset(ip_address, MAX_ADDRESS_SIZE*sizeof(U_SINT8), 0);
	memset(subnet_mask, MAX_ADDRESS_SIZE*sizeof(U_SINT8), 0);
	memset(gw_address, MAX_GW_ADDRESS_SIZE*sizeof(U_SINT8), 0);
	memset(dyn_dns, MAX_DNS_PRIMARY_ADDRESS_SIZE*sizeof(U_SINT8), 0);
	memset(pri_dns, MAX_ADDRESS_SIZE*sizeof(U_SINT8), 0);
	memset(sec_dns, MAX_ADDRESS_SIZE*sizeof(U_SINT8), 0);
	memset(p_cscf_prim_addr, MAX_ADDRESS_SIZE*sizeof(U_SINT8), 0);
	memset(p_cscf_sec_addr, MAX_ADDRESS_SIZE*sizeof(U_SINT8), 0);

	memset(apn_name, MAX_APN_SIZE*sizeof(U_SINT8), 0);

	memset(ipv6_address, MAX_ADDRESS_SIZE*sizeof(U_SINT8), 0);
	memset(subnetv6_mask, MAX_ADDRESS_SIZE*sizeof(U_SINT8), 0);
	memset(gwv6_address, MAX_ADDRESS_SIZE*sizeof(U_SINT8), 0);

	memset(dyn_dnsv6, MAX_DNS_PRIMARY_ADDRESS_SIZE*sizeof(U_SINT8), 0);
	memset(priv6_dns, MAX_ADDRESS_SIZE*sizeof(U_SINT8), 0);
	memset(secv6_dns, MAX_ADDRESS_SIZE*sizeof(U_SINT8), 0);
	memset(p_cscfv6_prim_addr, MAX_ADDRESS_SIZE*sizeof(U_SINT8), 0);
	memset(p_cscfv6_sec_addr, MAX_ADDRESS_SIZE*sizeof(U_SINT8), 0);

	bCallback_Reg  = 0;
	memset(VzAPN, MAX_APN_SIZE*sizeof(U_SINT8), 0);
	memset(VzAPNIMS, MAX_APN_SIZE*sizeof(U_SINT8), 0);
	APN_Inactivity_timer=0;

	bUpdateStatsResponsePending=false;
	isEmulatorMode= false;
	dalMode=0;
	ATCmdMode=0;
	FreqEARCFN=0;
	Signal_Strength=0;
	isPowerdown=0;
	context_id=0;
	attemptToOpenSerialPort=0;
	}
}CM_Info_s;

#if 0
typedef struct CM_Config
{
	FILE* configfile_p;
	U_SINT8* configfilename_p;
	FILE* persistfile_p;
	U_SINT8* persistfilename_p;
public:
CM_Config()
{

}
}CM_Config_s;
#endif //0
//////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum Error_values
{
	UCM_SUCCESS			  = 0,
	ERROR_DEVICE_NOT_OPEN = 1,
	ERROR_TIMER_VALUE = 200,
	SRCH_COMPLETED = 400,
	SRCH_INPROGRESS = 401,
	SRCH_FAILED	= 402,
	SRCH_NOTDONE = 403,
	ERROR_PRIMARY_INIT = 500,
	ERROR_SECONDARY_INIT = 501,
	ERROR_NET_ATTACH = 502,
	ERROR_NET_CFG_SET = 503,
	ERROR_CTX_CFG_SET = 504,
	ERROR_APN_TABLE_SET = 505,
	ERROR_IP_IF_STATUS = 506,
	ERROR_BAD_COMMANDID = 600,
	ERROR_BAD_STATE = 601,
	ERROR_ENABLE_FAILURE = 602
};

typedef enum ENetworkEnableStatus
{
	NM_ENABLED = 0,
	NM_DISABLED = 1,
};

typedef enum ENetworkConnectionStatus
{
	NM_CONNECTED = 0,
	NM_DISCONNECTED = 1
};

typedef enum ERasConnectionErrors
{
	RAS_SUCCESS = 9000,
	INIT_RAS_FAILED,
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct LTE_Config_Info
{
	U_UINT32 PLMNselectmode;
	U_UINT32 Enable4G;
	U_UINT32 AttachRAT;
	U_UINT32 DisableRATs;
	U_UINT32 LogLevel;
	U_UINT32 TestMode;
	U_UINT32 SMSWriteUicc;
	U_UINT32 SMSFormat;
	U_UINT32 SMSOverIMS;
	U_UINT32 T1_Timer;
	U_UINT32 T2_Timer;
	U_UINT32 TF_Timer;
	U_UINT32 LogtoFile;
	string LogFile;
	string VidPidName[vidPidCountMax];
	string comPortHWId;
	U_UINT32 PDPType;
	U_BOOL EmulatorMode;
	U_UINT32 DalMode;
	string APNName;
	U_SINT32 EARCFN1;
	U_UINT32 InactivityTimer;
	U_UINT32 isPowerdown;
	U_UINT32 IMSAttachOnly;
	U_UINT32 vidpidCount;
	string APNNameIMS;
	U_UINT32 APNId;
public:
	LTE_Config_Info()
	{
		PLMNselectmode=0;
		Enable4G=0;
		AttachRAT=0;
		DisableRATs=0;
		LogLevel=0;
		TestMode=0;
		SMSWriteUicc=0;
		SMSFormat=0;
		SMSOverIMS=0;
		T1_Timer=0;
		T2_Timer=0;
		TF_Timer=0;
		LogtoFile=0;
		LogFile.clear();
		VidPidName[0].clear();
		VidPidName[1].clear();
		VidPidName[2].clear();
		comPortHWId.clear();
		PDPType=0;
		EmulatorMode=false;
		DalMode=0;
		APNName.clear();
		EARCFN1=0;
		InactivityTimer=0;
		isPowerdown=0;
		IMSAttachOnly=0;
		vidpidCount=0;
		APNNameIMS.clear();
		APNId=0;
	}
}LTE_Config_Info_s;

typedef struct ThreeG_Config_Info
{
	U_UINT32 ThreeG_vidpidCount;
	string ThreeG_VidPidName[vidPidCountMax];
	string rasProfileName;
	string rasUsrName;
	string rasPasswd;
	string rasPhoneNo;
	string apnName;
	U_BOOL UssdDecodingReqd;

public: 
	ThreeG_Config_Info()
	{
		ThreeG_vidpidCount=0;
		ThreeG_VidPidName[0].clear();
		ThreeG_VidPidName[1].clear();
		ThreeG_VidPidName[2].clear();
		rasProfileName.clear();
		rasUsrName.clear();
		rasPasswd.clear();
		rasPhoneNo.clear();
		apnName.clear();
		UssdDecodingReqd = false;
	}
}ThreeG_Config_Info_s;

typedef struct WiFi_ConfigInfo
{
	U_UINT32 wifiEnabled;
	U_UINT32 wifiPriority;
public:
	WiFi_ConfigInfo()
	{
		wifiEnabled = 0;
		wifiPriority = 0;
	}
}WiFi_ConfigInfo_s;

typedef struct Ether_ConfigInfo
{

public:
	Ether_ConfigInfo()
	{
	}
}Ether_ConfigInfo_s;


typedef struct CM_Config_Info
{
	LTE_Config_Info_s lteConfigInfo;
	ThreeG_Config_Info_s threeGConfigInfo;
	WiFi_ConfigInfo wifiConfigInfo;

	string priorityList;
	string enabledList;
	U_BOOL isLTE_Enabled;
	U_BOOL isThreeG_Enabled;
	U_BOOL isWIFI_Enabled;

	U_BOOL isManualMode;

	U_UINT32 LTE_Tech_Priority;
	U_UINT32 THREEG_Tech_Priority;
	U_UINT32 WIFI_Tech_Priority;

	U_UINT32 insertedDeviceTechID;
public:
	CM_Config_Info()
	{
		priorityList.clear();
		enabledList.clear();
		isLTE_Enabled=false;
		isThreeG_Enabled=false;
		isWIFI_Enabled=false;

		LTE_Tech_Priority=0;
		THREEG_Tech_Priority=0;
		WIFI_Tech_Priority=0;
		isManualMode = false;

		insertedDeviceTechID=0;
	}
}CM_Config_Info_s;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



typedef struct ThreeG_stats
{
	U_SINT32 rssi;
	U_SINT32 cinr;
	U_UINT32 ul_data;
	U_UINT32 dl_data;
	U_UINT8 duration;
public:
	ThreeG_stats()
	{
		memset(this, 0, sizeof(ThreeG_stats));
	}
} ThreeG_stats_s;

typedef struct Ras_Details
{
	U_SINT8 apnName[20];
	U_SINT8 rasProfileName[20];
	U_SINT8 rasUsrName[20];
	U_SINT8 rasPasswd[20];
	U_SINT8 rasPhoneNumber[20];	
public:
	Ras_Details()
	{
		memset((void*)rasProfileName,0,sizeof(U_SINT8)*20);
		memset((void*)rasUsrName,0,sizeof(U_SINT8)*20);
		memset((void*)rasPasswd,0,sizeof(U_SINT8)*20);
		memset((void*)rasPhoneNumber,0,sizeof(U_SINT8)*20);
		memset((void*)apnName,0,sizeof(U_SINT8)*20);
	}

}Ras_Details_s;

#endif
