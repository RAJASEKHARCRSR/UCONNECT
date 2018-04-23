/*****************************************************************************
**
** uconn_Event.h
** This header file describes the interfaces for Events in the CM
**
** Author: Sathyanarayanan S
** Date  : 07-JAN-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		07/01/11     SS			Creation
****************************************************************************/

#ifndef UCONN_EVENT_H_
#define UCONN_EVENT_H_

#include <cstring>
#include "UTypes.h"
#include "uconn_OSAL.h"
#include "uconn_Config.h"

#define UCONN_CMD_LTE_EQUIP_CONFIG_GET				1
#define UCONN_CMD_LTE_EQUIP_CONFIG_SET				2
#define UCONN_CMD_LTE_EQUIP_INFO_GET				3
#define UCONN_CMD_LTE_NET_SEARCH					4
#define UCONN_CMD_LTE_NET_ATTACH					5
#define UCONN_CMD_LTE_NET_CFG_GET					6
#define UCONN_CMD_LTE_NET_CFG_SET					7
#define UCONN_CMD_LTE_NET_DETACH					8
#define UCONN_CMD_LTE_NET_SEARCH_STATUS				9
#define UCONN_CMD_LTE_4G_OFF						10
#define UCONN_CMD_LTE_4G_ON							11
#define UCONN_EVT_LTE_REG_STATUS					12
#define UCONN_EVT_PRIMARY_ACT						13
#define UCONN_EVT_SECONDARY_ACT						14
#define UCONN_EVT_PRIMARY_DEACT						15
#define UCONN_EVT_SECONDARY_DEACT					16
#define UCONN_EVT_ROAMING_IND						17
#define UCONN_SAPI_WRAP_GETSTATS                    18
#define UCONN_SAPI_WRAP_GENERIC_RESP                19
#define UCONN_SAPI_WRAP_NETSEARCH_RSP               20
#define UCONN_SAPI_WRAP_GET_RSP                     21
#define UCONN_SAPI_WRAP_SETCFGPARAM					22
#define UCONN_CMD_LTE_EQUIP_CONFIG_MODE             23
#define UCONN_CMD_LTE_4G_ONOFF                      24
#define UCONN_CMD_SERVER_STATE_FSM					25
#define UCONN_EVT_LTE_MODEM_STATISTIC					26
#define UCONN_CMD_LTE_MOD_STATISTIC_REQ					27
#define UCONN_EVT_NET_DETACH						28
#define UCONN_EVT_SYSTEM_EVENT						29
#define UCONN_SAPI_WRAP_GET_STRPARAM_REQ                30
#define UCONN_SAPI_WRAP_GET_STRPARAM_RSP                31

#define UCONN_CMD_LTE_VERSION_INFO_REQ				32
#define UCONN_CMD_LTE_VERSION_INFO_RSP				33
#define UCONN_CMD_NET_STATUS_GET					34
#define UCONN_CMD_NET_SIGQUAL_GET					35
#define UCONN_EVT_READY_FOR_ATTACH					36

#define UCONN_EVT_AT_CMD_REQUEST					37
#define UCONN_EVT_AT_CMD_RESPONSE					38

#define UCONN_EVT_LTE_MODEM_NAS_STATISTIC			39
#define UCONN_EVT_LTE_MODEM_RRC_STATISTIC			40
#define UCONN_EVT_LTE_MODEM_TIMERS_STATISTIC		41
#define UCONN_EVT_LTE_MODEM_L1A_STATISTIC			42
#define UCONN_EVT_LTE_MODEM_RSRP_MEASUREMENT		43
#define UCONN_EVT_LTE_MODEM_RSRQ_MEASUREMENT		44
#define UCONN_EVT_LTE_MODEM_SINR_MEASUREMENT		45
#define UCONN_SAPI_WRAP_GET_SIGSTATS				46
#define UCONN_SAPI_WRAP_GET_SIGSTATS_RSP			47
#define UCONN_CMD_LTE_MEAS_STATISTIC_REQ			48
#define UCONN_EVT_LTE_MEAS_STATISTIC_RSP			49
#define UCONN_CMD_LTE_BRCM_STATISTIC_REQ			50
#define UCONN_EVT_LTE_BRCM_STATISTIC_RSP			51
#define UCONN_CMD_LTE_BRCM_REFRESH_STATS			53

//asper at_command changed from 39 and 40 to avoid conflict
#define UCONN_SAPI_WRAP_GET_FLT_REQ                   59
#define UCONN_SAPI_WRAP_GET_FLT_RSP                   60



#define UCONN_EVT_STATS_RECEIVED		            79
#define UCONN_EVT_IMS_REG_SUCCESS                   80
#define UCONN_EVT_IMS_REG_FAILURE                   81
#define UCONN_EVT_IMS_DEREG                         82
#define UCONN_EVT_REQ_ATTACH                        83
#define UCONN_EVT_REQ_DETACH                        84
#define UCONN_EVT_REQ_POR                           85
#define UCONN_EVT_REQ_IMS_LTD_MODE                  86

#define UCONN_TEST_MODE_TOGGLE                      87
#define UCONN_CMD_UPDATE_STATS						88
#define UCONN_EVT_CONTRDP_RSP						89
#define UCONN_POWERUP_RSP							90
#define UCONN_EVT_PDN_REJECT						91
#define UCONN_EVT_CTX_QUERY_RSP						92

#define UCONN_RSP_OK								94
#define UCONN_DEVICE_STATE_CHANGE                   95
#define UCONN_EVT_RESET                             96
#define UCONN_EVT_GET_RSP                           97
#define UCONN_EVT_UPDATE							98
#define UCONN_EVT_TIMEOUT                           99
#define UCONN_EVT_LOG								500
#define UCONN_EVT_SERVER_CONN_LOST					505
#define UCONN_CMD_SERVER_LOG_ONOFF					510
#define UCONN_CMD_SERVER_LOG_LEVEL					515
#define UCONN_CMD_SERVER_LOG_GETSTATUS				520
#define UCONN_CMD_SERVER_LOG_STATUS_RESP			525

#define UCONN_CMD_WIFI_SCAN_INTERFACE				700
#define UCONN_EVT_WIFI_SCAN_INTERFACE				701
#define UCONN_CMD_WIFI_SCAN_NETWORK					702
#define UCONN_EVT_WIFI_SCAN_NETWORK					703
#define UCONN_CMD_WIFI_CONNECT						704
#define UCONN_EVT_WIFI_CONNECT						705
#define UCONN_CMD_WIFI_DISCONNECT					706
#define UCONN_EVT_WIFI_DISCONNECT					707
#define UCONN_CMD_WIFI_STATS						708
#define UCONN_EVT_WIFI_STATS						709
#define UCONN_EVT_WIFI_REQUEST_PASSWORD				710
#define UCONN_CMD_WIFI_CONNECT_PASSWORD				711
#define UCONN_CMD_WIFI_ENABLED						712
#define UCONN_EVT_WIFI_DISABLED_NOTI				713
/*Independent bearer defenitions*/
#define UCONN_CMD_IND_BEARER_CONNECT				800
#define UCONN_CMD_IND_BEARER_DISCONNECT				801

/* 3G events */
#define UCONN_EVT_NM_RSSI_UPDATE						900
#define UCONN_RSP_NM_RSSI_CINR_UPDATE					901
#define UCONN_EVT_NM_MODEM_STATE_UPDATE					902
#define UCONN_EVT_NM_MODEM_STATE_NETWORK_ACCESS_UPDATE	903
#define UCONN_EVT_NM_NETWORK_UPDATE						904
#define UCONN_EVT_NM_USSD_RSP							905
#define UCONN_EVT_NM_MANUFACTURE_UPDATE					906

#define UCONN_CMD_TECH_ENABLED						1500	
#define UCONN_CMD_TECH_DISABLED						1501
#define UCONN_CMD_TECH_CONFIG_MODE_CHANGED				1502
#define UCONN_CMD_TECH_PRIORITY_CHANGED				1503
#define UCONN_CMD_TECH_NET_SEARCH					1504
#define UCONN_CMD_CONNECT_REQUEST					1505
#define UCONN_EVT_CONNECT_RESPONSE					1506
#define UCONN_CMD_SET_NM_PARAM						1507

#define UCONN_CMD_BEARER_ENABLED					1600
#define UCONN_CMD_BEARER_DISABLED					1601
#define UCONN_CMD_BEARER_PRIORITY					1602
#define UCONN_CMD_BEARER_MODE						1603
#define UCONN_CMD_BEARER_SEARCH						1604
#define UCONN_CMD_BEARER_SEARCH_RESPONSE			1605
#define UCONN_CMD_BEARER_CONNECT					1606
#define UCONN_CMD_BEARER_DISCONNECT					1607
#define UCONN_CMD_BEARER_GET_PARAM					1608
#define UCONN_CMD_BEARER_SET_PARAM					1609
#define UCONN_CMD_BEARER_STATISTICS					1610
#define UCONN_CMD_BEARER_AT_COMMOND_MODE			1611
#define UCONN_CMD_BEARER_LOG_ENABLED				1612
#define UCONN_CMD_BEARER_LOG_DISABLED				1613
#define UCONN_CMD_BEARER_LOG_LEVEL					1614

#define UCONN_CMD_BEARER_GET_CFG_PARAM_REQ			1615
#define UCONN_CMD_BEARER_SET_CFG_PARAM_REQ			1616

#define UCONN_CMD_BEARER_ARBIT_ON					1617
#define UCONN_CMD_BEARER_ARBIT_OFF					1618
#define UCONN_CMD_BEARER_GET_ENABLED_TECH			1619
#define UCONN_CMD_BEARER_GET_PRIORITY_TECH			1620
#define UCONN_EVT_BEARER_ENABLED_TECH_RSP			1621
#define UCONN_EVT_BEARER_PRIORITY_TECH_RSP			1622

#define UCONN_CMD_BEARER_MEAS_STATISTICS			1623
#define UCONN_CMD_BEARER_PRIORITY_REQ				1624

#define UCONN_CMD_BEARER_GET_NM_STATE_REQ			1625

#define UCONN_EVT_GET_BEARER_PARAM_RSP				1626

/*Related to VPN*/

#define UCONN_CMD_VPN_LIST_CLIENTS			1650
#define UCONN_CMD_VPN_LIST_PROFILES		    1651
#define UCONN_CMD_VPN_START					1652
#define UCONN_CMD_VPN_STOP					1653

#define UCONN_EVT_VPN_LIST_CLIENTS					1654
#define UCONN_EVT_VPN_LIST_PROFILES					1655
#define UCONN_EVT_VPN_START_STATUS					1656
#define UCONN_EVT_VPN_STOP_STATUS					1657





/*command from BM to LTE NM*/
#define UCONN_BM_NM_CONNECT							1800
#define UCONN_BM_NM_DISCONNECT						1801
#define UCONN_BM_NM_ENABLE							1802
#define UCONN_BM_NM_DISABLE							1803
#define UCONN_BM_NM_SEARCH							1804



#define UCONN_EVT_NM_SCAN_REQ						1700 /*scan request from NM to BearerNM/GUI*/
#define UCONN_EVT_NM_SCAN_RESP						1701 /*scan response from NM to BearerNM/GUI*/
#define UCONN_EVT_NM_ENABLE_STATUS					1702
#define UCONN_EVT_NM_CONNECT_STATUS					1703
#define UCONN_EVT_NM_STATE_CHANGE					1704
#define UCONN_EVT_NM_DEVICE_STATE_CHANGE			1705
#define UCONN_EVT_NM_PRIORITY_CHANGE				1706
#define UCONN_EVT_SEND_USSD							1707


#define UCONN_EVT_WIFI_PREFERRED_NETWORKS			1711
#define UCONN_EVT_WIFI_STATS_RSP					1712

//events meant for gui
#define UCONN_EVT_NM_CONNECT_CHANGE					1715
#define UCONN_EVT_NM_SHOW_SETTINGS					1716
#define UCONN_EVT_NM_ERROR_DISPLAY					1717

/* events for 3G */
#define UCONN_EVT_PARAM_UPDATE                      1900
#define UCONN_RSP_NM_NETWORK_SEARCH					1901
#define UCONN_EVT_3G_STATS_RSP						1902
#define UCONN_EVT_NETWORK_NAME_UPDATE				1903
#define UCONN_EVT_USSD								1904
#define UCONN_RSP_NM_VERSION_INFO					1905
#define UCONN_CMD_SET_3G_RAS_PROFILE				1906
#define UCONN_CMD_GET_3G_RAS_PROFILE				1907

#define UCONN_LAST_CMD_ID							2000
/* State Machine timeout variable */
#define UCONN_SM_TIMEOUT							120000	/* 120 seconds */

/* RS/RA polling interval variable */				
#define RSRA_POLLING_INTERVAL						1000
#define RSRA_MAX_TRIES		20

#define MAX_NM_PARAM_LENGTH	100
#define MAX_NETWORK_SEARCH_RESPONSE				2000
#define MAX_NUMBER_OF_NETWORKMANAGERS			3
#define MAX_USSD_LENGTH							1000

#define INVALID_PRIORITY                                100

enum ETechSelect
{
	LTE_TECH = 0,
	THREE_G_TECH = 1, 
	WIFI_TECH = 2,
	ETHER_TECH = 3,
	BLUETOOTH = 4,
	VPN_TECH = 5,
	VPN_TETHERING = 6,
	ALL_TECH = 20,
    INVALID_TECH = 21
};

enum ENMStateForBM
{
	NM_DISABLED_STATE = 0,
	NM_READY_STATE = 1,
	NM_NOT_READY_STATE = 2,
	NM_CONNECTED_STATE = 3,
    NM_DISCONNECTED_STATE = 4
};


typedef enum
{
	AUTO = 0,
	MANUAL = 1
}EBearerMode;

enum Set_NM_Param
{
	WIFI_SSID,
	WIFI_PASSWORD,
	WIFI_PREFERRED_LIST
};

enum Get_NM_Param
{
	NETWORK_NAME = 0,
	ADAPTER_NAME = 1,
    SIGNAL_QUALITY = 2,
    RSSI = 3,
    CINR = 4,
    IP_ADDRESS = 5,
	NETWORK_ACCESS_TYPE = 6,
	VERSION = 7,
	BEARER_MNGR_MODE = 8,
    RAS_PROFILE = 9,
    UCM_VERSION
};

enum NM_Connect_Status {

	NM_CONNECT_SUCCESS=0,
	NM_CONNECT_FAILURE,
	NM_CONNECT_NO_NETWORK_FOUND,
	NM_CONNECT_REQUEST_PASSWORD
};


enum ENM_Error
{
	NM_INFORMATIVE = 0,
	NM_REPORT_ERROR 
};


class UEvent
{
public:
	U_UINT32    mCmdId;         /* identifies the type command - netattach, netsearch..*/
 	U_UINT32	tokenId;		/* the tokenID used to match request with the response */
	U_SINT32	execStatus;		/* the execution status of the command at the server */
	U_UINT32    length;         /* length in bytes of the command */
    U_UINT32 techId;

	UEvent(U_UINT32  cmdId):mCmdId(cmdId) 
	{
		tokenId = 0;
		execStatus = 0;
		length = sizeof(UEvent);
		techId = LTE_TECH;
	}
	
	U_UINT32 getCmdId()
	{
		return mCmdId;
	}
	U_SINT32   getExecStatus()
	{
		return execStatus;
	}
	void setExecStatus(U_SINT32 val)
	{
		execStatus = val;
	}
	U_UINT32   getTokenId()
	{
		return tokenId;
	}
	void setTokenId(U_UINT32 val) 
	{
		tokenId = val;
	}

	void setSize(U_UINT32 val)
	{
		length = val;
	}

	U_UINT32 getSize()
	{
		return length;
	}

	U_UINT32 getTechId()
	{
		return techId;
	}
	void setTechId(U_UINT32 techId) 
	{
		this->techId = techId;
	}
};

class UTimeOutEvent :public UEvent
{
public:
	UTimeOutEvent(U_TIMERID val):UEvent(UCONN_EVT_TIMEOUT)
	{
		timerId = val;
		setSize(sizeof(*this));
	}

	U_TIMERID     getTimerId() { return timerId;}
	U_TIMERID	  timerId;
//	T_PLMN_EARCH_RESULT_LIST* search_results;	
};




/* Generic Response */

class UGenResponse : public UEvent{
public:
	UGenResponse( ):UEvent(UCONN_SAPI_WRAP_GENERIC_RESP){
		setSize(sizeof(*this));
	}
};


class ULTERegStatusEvent:UEvent
{
private:
	
public:
	/* Declare all the variables for the events that would be handled
	by the CONNECTING state */
	ULTERegStatusEvent( U_UINT32 val):UEvent(UCONN_EVT_LTE_REG_STATUS)
	{
		//apn_table = new T_APN_TABLE();
		setSize(sizeof(*this));
		lte_reg_status = val;
	}
	U_UINT32 lte_reg_status;
	//T_APN_TABLE apn_table;
	U_UINT32 getRegStatus(){
		return lte_reg_status;
	}

};



class UDeviceStateChange : public UEvent{
public:
	UDeviceStateChange(U_UINT32 val=NOTI_DEVICE_NOT_OPEN ):UEvent(UCONN_DEVICE_STATE_CHANGE) ,deviceState(val)
	  {
		setSize(sizeof(*this));

	}
	U_UINT32 getDeviceState()
	{
		return deviceState;
	}

    void setDeviceState(U_UINT32 val)
	{
	   deviceState= val;
	}
	U_UINT32 deviceState;	
};

/* net search structure */
class  UNetsearch : public UEvent{
public:
	UNetsearch(U_UINT32 val):UEvent(UCONN_CMD_LTE_NET_SEARCH),PLMNtimer(val){
		setSize(sizeof(*this));
	}
	UNetsearch():UEvent(UCONN_CMD_LTE_NET_SEARCH_STATUS){
		setSize(sizeof(*this));
		PLMNtimer = 0;
	}
	U_UINT32 PLMNtimer;				/* timer for search */
	
};


/* Net Search Response */
class UNetSearchRsp :public UEvent{
public:
	UNetSearchRsp():UEvent(UCONN_SAPI_WRAP_NETSEARCH_RSP){
		setSize(sizeof(*this));
	}
	NetSearch_Result_s   nwSearchRsp;
	void setNetSearchResults(NetSearch_Result_s &results){
		nwSearchRsp = results;
	}
};





/* net attach structure */
class  UNetattach : public UEvent{
public:
	UNetattach(U_UINT32 idx):UEvent(UCONN_CMD_LTE_NET_ATTACH),PLMNIdx(idx){
		setSize(sizeof(*this));
	}
	U_UINT32 PLMNIdx;				/* index to the plmn search list */
};

/* net detach structure */
class  UNetdetach : public UEvent{
public:
	UNetdetach():UEvent(UCONN_CMD_LTE_NET_DETACH){
		setSize(sizeof(*this));
	}
};

/* Set Config params structure */
class  USetCfgParam : public UEvent{
public:
	USetCfgParam(U_UINT32 param,U_UINT32 val):UEvent(UCONN_SAPI_WRAP_SETCFGPARAM),
			CfgParam(param),value(val){
				setSize(sizeof(*this));
	}
	U_UINT32 CfgParam;				/* Enum of the Param to be set */
	U_UINT32 value;					/* value of the Param to be set */
};




/* Get params structure */
class  UGetCfgParam : public UEvent{
public:
    UGetCfgParam(U_UINT32 param):UEvent(UCONN_SAPI_WRAP_GETSTATS),
                    cfgParam(param){
                            setSize(sizeof(*this));
    }
    U_UINT32 cfgParam;				/* Enum of the Param to get */
};

/* Get Params Response */
class  UGetCfgRsp: public UEvent{
public:
	UGetCfgRsp(U_UINT32 param, U_UINT32 val):UEvent(UCONN_SAPI_WRAP_GET_RSP),
			cfgVal(val),cfgParam(param){
				setSize(sizeof(*this));
	}
	U_UINT32 cfgParam;				/* Enum of the Param to get */
	U_UINT32 cfgVal;				/* Returned Value */
};

/* Get params structure */
class  UGetSigCfgParam : public UEvent{
public:
    UGetSigCfgParam(U_UINT32 param):UEvent(UCONN_SAPI_WRAP_GET_SIGSTATS),
                    cfgParam(param){
                            setSize(sizeof(*this));
    }
    U_UINT32 cfgParam;				/* Enum of the Param to get */
};

/* Get Params Response */
class  UGetSigCfgRsp: public UEvent{
public:
        UGetSigCfgRsp(U_UINT32 param, U_SINT32 val):UEvent(UCONN_SAPI_WRAP_GET_SIGSTATS_RSP),
                        cfgParam(param),cfgVal(val){
                                setSize(sizeof(*this));
        }
        U_UINT32 cfgParam;				/* Enum of the Param to get */
        U_SINT32 cfgVal;				/* Returned Value */
};

class  UGetfltCfgParam : public UEvent{
public:
    UGetfltCfgParam(U_UINT32 param):UEvent(UCONN_SAPI_WRAP_GET_FLT_REQ),
                    cfgParam(param){
                            setSize(sizeof(*this));
    }
    U_UINT32 cfgParam;				/* Enum of the Param to get */
};

/* Get Params Response */
class  UGetfltCfgRsp: public UEvent{
public:
        UGetfltCfgRsp(U_UINT32 param, float val):UEvent(UCONN_SAPI_WRAP_GET_FLT_RSP),
                        cfgVal(val),cfgParam(param){
                                setSize(sizeof(*this));
        }
        U_UINT32 cfgParam;				/* Enum of the Param to get */
        float cfgVal;				/* Returned Value */
};


class  UGetStrCfgParam : public UEvent{
public:
    UGetStrCfgParam(U_UINT32 param):UEvent(UCONN_SAPI_WRAP_GET_STRPARAM_REQ),
                    cfgParam(param){
                            setSize(sizeof(*this));
    }
    U_UINT32 cfgParam;				/* Enum of the Param to get */
};

class  USecondaryAct : public UEvent{
public:
    USecondaryAct(U_UINT32 param):UEvent(UCONN_EVT_SECONDARY_ACT),
                    ctxId(param){
                            setSize(sizeof(*this));
    }
    U_UINT32 ctxId;				/* Enum of the Param to get */
};


class  UPrimaryDeAct : public UEvent{
public:
	UPrimaryDeAct(U_UINT32 param):UEvent(UCONN_EVT_PRIMARY_ACT),
                    ctxId(param){
                            setSize(sizeof(*this));
    }
    U_UINT32 ctxId;				/* Enum of the Param to get */
};

class  USecondaryDeAct : public UEvent{
public:
	USecondaryDeAct(U_UINT32 param):UEvent(UCONN_EVT_SECONDARY_DEACT),
                    ctxId(param){
                            setSize(sizeof(*this));
    }
    U_UINT32 ctxId;				/* Enum of the Param to get */
};

class  URoamingInd : public UEvent{
public:
    URoamingInd(U_UINT32 param):UEvent(UCONN_EVT_ROAMING_IND),
                    roamStatus(param){
                            setSize(sizeof(*this));
    }
    U_UINT32 roamStatus;				/* Enum of the Param to get */
};
/* Get String Params Response */
class  UGetStrCfgRsp: public UEvent{
public:
    UGetStrCfgRsp(U_UINT32 param, U_STRING val):UEvent(UCONN_SAPI_WRAP_GET_STRPARAM_RSP)
            ,cfgParam(param){
                setcfgValue(val);
                setSize(sizeof(*this));
    }
    U_UINT32 cfgParam;				/* Enum of the Param to get */
    U_SINT8 cfgString[250];
	void setcfgValue(string val){
                strncpy(cfgString, val.c_str(), 250);
	}
};

class  UServerDisconnect: public UEvent{
public:
	UServerDisconnect():UEvent(UCONN_EVT_SERVER_CONN_LOST)
	{
	}
};

class  ULogOnOffEvent: public UEvent{
public:
	ULogOnOffEvent(U_UINT32 logOnOff):UEvent(UCONN_CMD_SERVER_LOG_ONOFF),
			cfglogOnOff(logOnOff){
				setSize(sizeof(*this));
	}
	U_UINT32 cfglogOnOff;			/* Set Loggin on or off */
};

class  ULogLevelEvent: public UEvent{
public:
	ULogLevelEvent(U_UINT32 level):UEvent(UCONN_CMD_SERVER_LOG_LEVEL),
			cfglogLevel(level){
				setSize(sizeof(*this));
	}
	U_UINT32 cfglogLevel;			/* Set Loggin on or off */
};

class ULogEvent :public UEvent{
public:
	ULogEvent():UEvent(UCONN_EVT_LOG){
		setSize(sizeof(*this));
	}
	U_SINT8 logString[MAX_LOG_LENGTH_AT_CLIENT];
	void setlogString(string log){
                strncpy(logString, log.c_str(), MAX_LOG_LENGTH_AT_CLIENT);
	}
};

class UGetLogStatusEvent :public UEvent{
public:
	UGetLogStatusEvent():UEvent(UCONN_CMD_SERVER_LOG_GETSTATUS){
		setSize(sizeof(*this));
	}
};

class UGetLogStatusRespEvent :public UEvent{
public:
	UGetLogStatusRespEvent():UEvent(UCONN_CMD_SERVER_LOG_STATUS_RESP){
		setSize(sizeof(*this));
	}
	U_UINT32 cfglogLevel;
	U_UINT32 cfglogOnOff;
};

class  UConfigModeEvent: public UEvent{
public:
        UConfigModeEvent(U_UINT32 mode):UEvent(UCONN_CMD_LTE_EQUIP_CONFIG_MODE),
                        cfgmode(mode){
                                setSize(sizeof(*this));
        }
        U_UINT32 cfgmode;			/* Set config mode to 0 or 1*/
};

class  UConfigEnable4GEvent: public UEvent{
public:
        UConfigEnable4GEvent(U_UINT32 cfgen4g):UEvent(UCONN_CMD_LTE_4G_ONOFF),
                        cfgenable4g(cfgen4g){
                                setSize(sizeof(*this));
        }
        U_UINT32 cfgenable4g;			/* Set config mode to 0 or 1*/
};


class  UServerStateEvent: public UEvent{
public:
        UServerStateEvent(U_UINT32 currState):UEvent(UCONN_CMD_SERVER_STATE_FSM),
                        currentState(currState){
                                setSize(sizeof(*this));
        }
        U_UINT32 currentState;			/* Set current state*/
};

#if 0
class  UModemStatisticEvent: public UEvent{
public:
        UModemStatisticEvent(Modem_stats_s modStatistic):UEvent(UCONN_EVT_LTE_MODEM_STATISTIC),
                        modemStatistic(modStatistic){
                                setSize(sizeof(*this));
        }
        Modem_stats_s modemStatistic;			/* Set modem statistics*/
};
#endif
class  UModemStatisticEvent: public UEvent{
public:
        UModemStatisticEvent(L1A_Stats_s modStatistic):UEvent(UCONN_EVT_LTE_MODEM_STATISTIC),
                        modemStatistic(modStatistic){
                                setSize(sizeof(*this));
        }
        L1A_Stats_s modemStatistic;			/* Set modem statistics*/
};

class  UMEASStatisticEvent: public UEvent{
public:
        UMEASStatisticEvent(Meas_Stats_s measStatistic):UEvent(UCONN_EVT_LTE_MEAS_STATISTIC_RSP),
                        measStat(measStatistic){
                                setSize(sizeof(*this));
        }
        Meas_Stats_s measStat;			/* Set modem statistics*/
};

class  UBrcmStatisticEvent: public UEvent{
public:
	UBrcmStatisticEvent(Broadcom_Stats_s broadcomStatistic):UEvent(UCONN_EVT_LTE_BRCM_STATISTIC_RSP),
                        broadcomStat(broadcomStatistic){
                                setSize(sizeof(*this));
        }
        Broadcom_Stats_s broadcomStat;			/* Set modem statistics*/
};


class  UModemStatisticRequest: public UEvent{
public:
		UModemStatisticRequest():UEvent(UCONN_CMD_LTE_MOD_STATISTIC_REQ){
                                setSize(sizeof(*this));
        }
};

class  UMeasStatisticRequest: public UEvent{
public:
        UMeasStatisticRequest():UEvent(UCONN_CMD_LTE_MEAS_STATISTIC_REQ){
                                setSize(sizeof(*this));
        }
};

class  UBrcmStatisticRequest: public UEvent{
public:
        UBrcmStatisticRequest():UEvent(UCONN_CMD_LTE_BRCM_STATISTIC_REQ){
                                setSize(sizeof(*this));
        }
};



class  UNetdetachEvent : public UEvent{
public:
	UNetdetachEvent():UEvent(UCONN_EVT_NET_DETACH){
		setSize(sizeof(*this));
	}
};

/* Event to signal System Notifications like - Device OPEN, 
   ERROR, DISCONNECTED, NOT_OPEN etc */
class  USystemEvent: public UEvent{
public:
        USystemEvent(U_UINT32 sysEvent):UEvent(UCONN_EVT_SYSTEM_EVENT),
                        notifyEvent(sysEvent){
                                setSize(sizeof(*this));
        }
        U_UINT32 notifyEvent;			/* Set current Notification */
};

/* Event to request LTE Stack Version Info and the corresponding response */
class  UVersionInfoRequest: public UEvent{
public:
	UVersionInfoRequest():UEvent(UCONN_CMD_LTE_VERSION_INFO_REQ){
                                setSize(sizeof(*this));
        }
};
class  UVersionInfoResponse: public UEvent{
public:
	UVersionInfoResponse(Version_Info_s iVersionInfo):
	  UEvent(UCONN_CMD_LTE_VERSION_INFO_RSP),
                        verInfo(iVersionInfo){
                                setSize(sizeof(*this));
        }
	  Version_Info_s verInfo;			/* Set Version Info */
};

/* Event to indicate the UE is ready for a net attach */
class  UReadyforAttach: public UEvent{
public:
	UReadyforAttach(U_UINT32 ready):UEvent(UCONN_EVT_READY_FOR_ATTACH),
                netattachReady(ready){
				setSize(sizeof(*this));
        }
        U_UINT32 netattachReady;			/* Set current Notification */
};

////////////////////IMS Start////////////////////////////////////////
/* Event for Notify successful IMS registration */
class  UImsRegSucess: public UEvent{
public:
	UImsRegSucess(U_UINT32 regSucess):UEvent(UCONN_EVT_IMS_REG_SUCCESS),
                regSucess(regSucess){
				setSize(sizeof(*this));
        }
        U_UINT32 regSucess;		/* Set current Notification */
};

/* Event for Notify failure to establish IMS registration*/
class  UImsRegFailure: public UEvent{
public:
	UImsRegFailure(U_UINT32 regFail):UEvent(UCONN_EVT_IMS_REG_FAILURE),
                regFailure(regFail){
				setSize(sizeof(*this));
        }
        U_UINT32 regFailure;			/* Set current Notification */
};

/* Event for Notify IMS deregistration*/
class  UImsDeReg: public UEvent{
public:
	UImsDeReg(U_UINT32 deReg):UEvent(UCONN_EVT_IMS_DEREG),
                imsDeReg(deReg){
				setSize(sizeof(*this));
        }
        U_UINT32 imsDeReg;			/* Set current Notification */
};

/* Event for Request PDN connectivity to IMS PDN*/
class  UImsReqAttachPDN: public UEvent{
public:
	UImsReqAttachPDN(U_UINT32 attachPdn):UEvent(UCONN_EVT_REQ_ATTACH),
                imsAttachPdn(attachPdn){
				setSize(sizeof(*this));
        }
        U_UINT32 imsAttachPdn;			/* Set current Notification */
};

/* Event for Request disconnection from IMS PDN*/
class  UImsReqDetachPDN: public UEvent{
public:
	UImsReqDetachPDN(U_UINT32 detachPdn):UEvent(UCONN_EVT_REQ_DETACH),
                imsDetachPdn(detachPdn){
				setSize(sizeof(*this));
        }
        U_UINT32 imsDetachPdn;			/* Set current Notification */
};

/* Event for Notify need for power cycle or ISIM change*/
class  UImsReqPowerCycle: public UEvent{
public:
	UImsReqPowerCycle(U_UINT32 imsPowerUpCycle):UEvent(UCONN_EVT_REQ_POR),
                powerUpCycle(imsPowerUpCycle){
				setSize(sizeof(*this));
        }
        U_UINT32 powerUpCycle;			/* Set current Notification */
};

/* Event for Request IMS limited mode, in this mode the CM should not send any class 1 or classless SMS to the device*/
class  UImsRegLtdMode: public UEvent{
public:
	UImsRegLtdMode(U_UINT32 ltdMode):UEvent(UCONN_EVT_REQ_IMS_LTD_MODE),
                imsLtdMode(ltdMode){
				setSize(sizeof(*this));
        }
        U_UINT32 imsLtdMode;			/* Set current Notification */
};

/* Test Mode--Manage the Test mode options as mentioned below
*	If test mode is Enabled – Connect only to internet PDN
*	If test mode is disabled – Connect to IMS pdn.
*/
class  UTestMode: public UEvent{
public:
	UTestMode(U_UINT32 tMode):UEvent(UCONN_TEST_MODE_TOGGLE),
                testMode(tMode){
				setSize(sizeof(*this));
        }
        U_UINT32 testMode;			/* Set current Notification */
};
////////////////////IMS End//////////////////////////////////////////

/* Event to ask the SM to request for new statistics */
class  UUpdateStatsEvent : public UEvent{
public:
	UUpdateStatsEvent():UEvent(UCONN_CMD_UPDATE_STATS){
		setSize(sizeof(*this));
	}
};

/* Event for OK response from the Modem */
class  UOKResponse: public UEvent{
public:
	UOKResponse():UEvent(UCONN_RSP_OK)
	{
		setSize(sizeof(*this));
    }
};

/* Event to signal completion of AT+CGCONTRDP command */
class  UCGCONTRDPEvent : public UEvent{
public:
	UCGCONTRDPEvent():UEvent(UCONN_EVT_CONTRDP_RSP){
		setSize(sizeof(*this));
	}
};

/* Event for AT command Request */
class  UATCmdRequestEvent: public UEvent{
public:
	UATCmdRequestEvent():UEvent(UCONN_EVT_AT_CMD_REQUEST)
	{
		setSize(sizeof(*this));
    }
	U_SINT8 sATCmdReqStr[MAX_AT_COMMAND_REQUEST];
	void setATReqCmdString(string str)
	{
        strncpy(sATCmdReqStr, str.c_str(), MAX_AT_COMMAND_REQUEST);
	}
};


/* Event for AT command Response */
class  UATCmdResponseEvent: public UEvent{
public:
	UATCmdResponseEvent():UEvent(UCONN_EVT_AT_CMD_RESPONSE)
	{
		setSize(sizeof(*this));
    }
	U_SINT8 sATCmdResStr[MAX_AT_COMMAND_RESPONSE];
	void setATCmdRspString(string str)
	{
        strncpy(sATCmdResStr, str.c_str(), MAX_AT_COMMAND_RESPONSE);
	}
};

/* Event to notify the current Powerup status */
class  UPowerupRspEvent: public UEvent{
public:
	UPowerupRspEvent(U_UINT32 pState):UEvent(UCONN_POWERUP_RSP),
                powerupState(pState){
				setSize(sizeof(*this));
        }
        U_UINT32 powerupState;			/* Set current Notification */
};

/* Event to signal rejection of netattach command */
class  UPDNConnectRejectEvent : public UEvent{
public:
	UPDNConnectRejectEvent():UEvent(UCONN_EVT_PDN_REJECT){
		setSize(sizeof(*this));
	}
};

/* Event to notify the current Ctx ID in use */
class  UCtxQueryRspEvent: public UEvent{
public:
	UCtxQueryRspEvent(U_UINT32 ctxID):UEvent(UCONN_EVT_CTX_QUERY_RSP),
                ctxIDinUse(ctxID){
				setSize(sizeof(*this));
        }
        U_UINT32 ctxIDinUse;			/* Set current Notification */
};


/*----------------- events for Wifi -------------------------------*/

class  UWifiConnectWithPassword : public UEvent{
public:

	
	char password[30];

	UWifiConnectWithPassword(char* password):UEvent(UCONN_CMD_WIFI_CONNECT_PASSWORD){
		setSize(sizeof(*this));
		strncpy(this->password,password,sizeof(this->password));
	}
	
};


class  UWifiDisabledNotification : public UEvent{
public:
	UWifiDisabledNotification():UEvent(UCONN_EVT_WIFI_DISABLED_NOTI){
		setSize(sizeof(*this));
	}
};


class  UTechEnabled : public UEvent{
public:
	UTechEnabled(U_UINT32 techId):UEvent(UCONN_CMD_TECH_ENABLED){
		setSize(sizeof(*this));
		this->techId = techId;
	}
      
};

class  UTechDisabled : public UEvent{
public:
	UTechDisabled(U_UINT32 techId):UEvent(UCONN_CMD_TECH_DISABLED){
		setSize(sizeof(*this));
		this->techId = techId;
	}
      
};

#if 0
class  UTechConfigChanged : public UEvent{
public:
	UTechConfigChanged(U_UINT32 techId,Config_mode mode):UEvent(UCONN_CMD_TECH_CONFIG_MODE_CHANGED){
		setSize(sizeof(*this));
		this->techId = techId;
		this->configMode = mode;
	}

	Config_mode configMode;
      
};


class  UTechPriorityChanged : public UEvent{
public:
        UTechPriorityChanged(U_UINT32 techId,U_UINT32 priority[MAX_NUMBER_OF_NETWORKMANAGERS]):UEvent(UCONN_CMD_TECH_PRIORITY_CHANGED){
		setSize(sizeof(*this));
		this->techId = techId;
                this->priority[MAX_NUMBER_OF_NETWORKMANAGERS] = priority[MAX_NUMBER_OF_NETWORKMANAGERS];
	}

        U_UINT32 priority[MAX_NUMBER_OF_NETWORKMANAGERS];
      
};
#endif
class  UTechNetSearch : public UEvent{
public:
	UTechNetSearch(U_UINT32 techId):UEvent(UCONN_CMD_TECH_NET_SEARCH){
		setSize(sizeof(*this));
		this->techId = techId;
	}
      
};

class  UConnectRequest : public UEvent{
public:
	UConnectRequest():UEvent(UCONN_CMD_CONNECT_REQUEST){
		setSize(sizeof(*this));
	
	}
      
};


class  UNMParam : public UEvent
{
public:
	UNMParam(U_UINT32 techId,Set_NM_Param paramId,string& param):UEvent(UCONN_CMD_SET_NM_PARAM)
	{
		setSize(sizeof(*this));
		this->techId = techId;
		this->paramId = paramId;
		strncpy(this->param,param.c_str(),sizeof(this->param)-1);
		this->param[sizeof(this->param)-1] = '\0';
	}
	char param[MAX_NM_PARAM_LENGTH];
	Set_NM_Param paramId;
};

class  UWifiScanNetworkEvt : public UEvent
{	
public:	
	UWifiScanNetworkEvt():UEvent(UCONN_EVT_WIFI_SCAN_NETWORK)
	{
		setSize(sizeof(*this));
	}
	U_SINT8 networkList[2000];
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UBearerEnabled : public UEvent
{
public:
      UBearerEnabled(U_UINT32 techId):UEvent(UCONN_CMD_BEARER_ENABLED)
      {
            setSize(sizeof(*this));
			this->techId = techId;
      }
};

class UBearerDisabled : public UEvent
{
public:
      UBearerDisabled(U_UINT32 techId):UEvent(UCONN_CMD_BEARER_DISABLED)
      {
            setSize(sizeof(*this));
			this->techId = techId;
      }
};

class UBearerConnect : public UEvent
{
public:
      UBearerConnect(U_UINT32 techId):UEvent(UCONN_CMD_BEARER_CONNECT)
      {
            setSize(sizeof(*this));
			this->techId = techId;
      }
};

class UBearerDisconnect : public UEvent
{
public:
      UBearerDisconnect(U_UINT32 techId):UEvent(UCONN_CMD_BEARER_DISCONNECT)
      {
            setSize(sizeof(*this));
			this->techId = techId;
      }
};

class  UBearerSearch : public UEvent
{
public:
	UBearerSearch(U_UINT32 techId):UEvent(UCONN_CMD_BEARER_SEARCH)
	{
		setSize(sizeof(*this));
		this->techId = techId;
	}    
};

class  UBearerStrParam : public UEvent
{
public:
	UBearerStrParam(U_UINT32 techId,Set_NM_Param paramId,string& valStr):UEvent(UCONN_CMD_BEARER_SET_PARAM)
	{
		setSize(sizeof(*this));
		this->techId = techId;
		this->paramId = paramId;
		//strncpy(this->valueStr,valStr.c_str(),valStr.length());
		strncpy(this->valueStr,valStr.c_str(),MAX_NM_PARAM_LENGTH-1);
		this->valueStr[MAX_NM_PARAM_LENGTH-1] = '\0';
	}
	char valueStr[MAX_NM_PARAM_LENGTH];
	Set_NM_Param paramId;
};

class  UBearerGetParam : public UEvent
{
public:
	UBearerGetParam(U_UINT32 techId,Get_NM_Param paramId):UEvent(UCONN_CMD_BEARER_GET_PARAM)
	{
		setSize(sizeof(*this));
		this->techId = techId;
		this->paramId = paramId;
	}
	Get_NM_Param paramId;
};

class  UBearerGetParamRsp : public UEvent{
public:
	UBearerGetParamRsp(U_UINT32 techId,Get_NM_Param paramId, string& param):UEvent(UCONN_EVT_GET_BEARER_PARAM_RSP)
	{
		setSize(sizeof(*this));
		this->techId = techId;
		strncpy(this->param,param.c_str(),sizeof(this->param)-1);
		this->param[sizeof(this->param)-1] = '\0';
		this->paramId = paramId;
	}

	UBearerGetParamRsp(U_UINT32 techId,Get_NM_Param paramId, U_SINT8* param):UEvent(UCONN_EVT_GET_BEARER_PARAM_RSP)
	{
	setSize(sizeof(*this));
		this->techId = techId;
		if(strlen(param) > 40){

			this->techId = techId;
		}
		strncpy(this->param,param,sizeof(this->param)-1);
		this->param[sizeof(this->param)-1] = '\0';
		this->paramId = paramId;
	

	}
	char param[500];				/* Enum of NM Param to be set */
	Get_NM_Param paramId;
};

class  UBearerGetCfgParam : public UEvent
{
public:
	UBearerGetCfgParam(U_UINT32 techId,U_UINT32 paramId):UEvent(UCONN_CMD_BEARER_GET_CFG_PARAM_REQ)
	{
		setSize(sizeof(*this));
		this->techId = techId;
		this->paramId = paramId;
	}
	U_UINT32 paramId;
};

class  UBearerGetNMState : public UEvent
{
public:
	UBearerGetNMState(U_UINT32 techId):UEvent(UCONN_CMD_BEARER_GET_NM_STATE_REQ)
	{
		setSize(sizeof(*this));
		this->techId = techId;
	}
};

class  UBearerPriorityChanged : public UEvent
{
public:
        UBearerPriorityChanged(U_UINT32 techId,U_UINT32* priority):UEvent(UCONN_CMD_BEARER_PRIORITY)
	{
		setSize(sizeof(*this));
		this->techId = techId;
                memcpy((void*)(this->techPriority),(void*)(priority),(sizeof(U_UINT32))*MAX_NUMBER_OF_NETWORKMANAGERS);
	}
        U_UINT32 techPriority[MAX_NUMBER_OF_NETWORKMANAGERS];
};

class  UBearerModeChanged : public UEvent
{
public:
	UBearerModeChanged(U_UINT32 techId, EBearerMode mode):UEvent(UCONN_CMD_BEARER_MODE)
	{
		setSize(sizeof(*this));
		this->techId = techId;
		this->bearerMode = mode;
	}
	EBearerMode bearerMode;
};

class UBearerStatistics : public UEvent
{
public:
	UBearerStatistics(U_UINT32 techId):UEvent(UCONN_CMD_BEARER_STATISTICS)
	{
		setSize(sizeof(*this));
		this->techId = techId;
	}
};

class UBearerMeasStatistics : public UEvent
{
public:
	UBearerMeasStatistics(U_UINT32 techId):UEvent(UCONN_CMD_BEARER_MEAS_STATISTICS)
	{
		setSize(sizeof(*this));
		this->techId = techId;
	}
};

class UBearerLogEnabled : public UEvent
{
public:
	UBearerLogEnabled(U_UINT32 techId):UEvent(UCONN_CMD_BEARER_LOG_ENABLED)
	{
		setSize(sizeof(*this));
		this->techId = techId;
	}
};

class UBearerLogDisabled : public UEvent
{
public:
	UBearerLogDisabled(U_UINT32 techId):UEvent(UCONN_CMD_BEARER_LOG_DISABLED)
	{
		setSize(sizeof(*this));
		this->techId = techId;
	}
};

class UBearerLogLevel : public UEvent
{
public:
	UBearerLogLevel(U_UINT32 techId,U_UINT32 logLevel):UEvent(UCONN_CMD_BEARER_LOG_LEVEL)
	{
		setSize(sizeof(*this));
		this->techId = techId;
		this->logLevel = logLevel;
	}
	U_UINT32 logLevel;
};


class  UNMSearchReq : public UEvent
{
public:
	UNMSearchReq(U_UINT32 techId):UEvent(UCONN_EVT_NM_SCAN_REQ)
	{
		setSize(sizeof(*this));
		this->techId = techId;
	}    
};

class  UNMSearchResp : public UEvent
{
public:
	UNMSearchResp(U_UINT32 techId,string searchResp):UEvent(UCONN_EVT_NM_SCAN_RESP)
	{
		setSize(sizeof(*this));
		this->techId = techId;
		strncpy(networkList,searchResp.c_str(),MAX_NETWORK_SEARCH_RESPONSE-1);
		networkList[MAX_NETWORK_SEARCH_RESPONSE-1] = '\0';
	}
	U_SINT8 networkList[MAX_NETWORK_SEARCH_RESPONSE];
};

class  UNMEnableStatus : public UEvent
{
public:
	UNMEnableStatus(U_UINT32 techId,U_UINT32 enableStatus):UEvent(UCONN_EVT_NM_ENABLE_STATUS)
	{
		setSize(sizeof(*this));
		this->techId = techId;
		this->enableStatus = enableStatus;



	}    
	U_UINT32 enableStatus;
	
};

class  UNMConnectStatus : public UEvent
{
public:
	UNMConnectStatus(ETechSelect techId,NM_Connect_Status connectStatus,string param=""):UEvent(UCONN_EVT_NM_CONNECT_STATUS)
	{
		setSize(sizeof(*this));
		this->techId =  techId;
		this->connectStatus = connectStatus;

	    if(param.size())
		{
			strncpy(this->param,param.c_str(),sizeof(this->param)-1);
			this->param[sizeof(this->param)-1] = '\0';
		}
		else
		{
			this->param[0] = '\0';
		}

	}
	NM_Connect_Status connectStatus;
	char param[50];
};

class  UNMStateEvent: public UEvent
{
public:
	UNMStateEvent(ETechSelect techId,ENMStateForBM currState):UEvent(UCONN_EVT_NM_STATE_CHANGE),
					currentState(currState)
	{
		setSize(sizeof(*this));
		this->techId =  techId;
	}
	ENMStateForBM currentState;			/* Set current state*/
	
	U_UINT32 getCurrentState()
	{
		return currentState;
	}
};

class  UNMPriorityEvent: public UEvent
{
public:
	UNMPriorityEvent(U_UINT32 techId):UEvent(UCONN_EVT_NM_PRIORITY_CHANGE)//,
					//currentPriority(priority)
	{
		setSize(sizeof(*this));
		this->techId =  techId;
	}
	//U_UINT32 currentPriority;			/* Set current priority*/
	
	//U_UINT32 getPriority()
	//{
	//	return currentPriority;
	//}
};

/*BM to LTE NM*/
class  UNMConnect : public UEvent
{
public:
	UNMConnect(ETechSelect techId):UEvent(UCONN_BM_NM_CONNECT)
	{
		setSize(sizeof(*this));
		this->techId =  techId;
	}
};

class  UNMDisconnect : public UEvent
{
public:
	UNMDisconnect(ETechSelect techId):UEvent(UCONN_BM_NM_DISCONNECT)
	{
		setSize(sizeof(*this));
		this->techId =  techId;
	}
};

class  UNMEnable : public UEvent
{
public:
	UNMEnable(ETechSelect techId):UEvent(UCONN_BM_NM_ENABLE)
	{
		setSize(sizeof(*this));
		this->techId =  techId;
	}
};

class  UNMDisable : public UEvent
{
public:
	UNMDisable(ETechSelect techId):UEvent(UCONN_BM_NM_DISABLE)
	{
		setSize(sizeof(*this));
		this->techId =  techId;
	}
};



class  UNMDisConnect : public UEvent
{
public:
	UNMDisConnect(ETechSelect techId):UEvent(UCONN_BM_NM_DISCONNECT)
	{
		setSize(sizeof(*this));
		this->techId =  techId;
	}
};

class  UNMSearchNetworks : public UEvent
{
public:
	UNMSearchNetworks(ETechSelect techId):UEvent(UCONN_BM_NM_SEARCH)
	{
		setSize(sizeof(*this));
		this->techId =  techId;
	}
};

class UBearerArbitrationON : public UEvent
{
public:
      UBearerArbitrationON():UEvent(UCONN_CMD_BEARER_ARBIT_ON)
      {
            setSize(sizeof(*this));
			this->techId = ALL_TECH;
      }
};

class UBearerArbitrationOFF : public UEvent
{
public:
      UBearerArbitrationOFF():UEvent(UCONN_CMD_BEARER_ARBIT_OFF)
      {
            setSize(sizeof(*this));
			this->techId = ALL_TECH;
      }
};

class  UNMSendUSSD : public UEvent
{
public:
	UNMSendUSSD(string& USSDString) : UEvent(UCONN_EVT_SEND_USSD)
	{
		setSize(sizeof(*this));
		this->techId = THREE_G_TECH;
		strncpy(USSDStr,USSDString.c_str(),(sizeof(this->USSDStr)-1));
		USSDStr[sizeof(this->USSDStr)-1] = '\0';
	}
	char USSDStr[MAX_USSD_LENGTH];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class  UVPNListClients : public UEvent
{
public:
	UVPNListClients(ETechSelect techId=VPN_TECH):UEvent(UCONN_CMD_VPN_LIST_CLIENTS)
	{
		setSize(sizeof(*this));
        this->techId = techId;
	}
};

class UVPNListClientsRsp : public UEvent
{
public:
	UVPNListClientsRsp(string clientList):UEvent(UCONN_EVT_VPN_LIST_CLIENTS)
	{
		setSize(sizeof(*this));
		this->techId = VPN_TECH;
		strncpy(this->clientList,clientList.c_str(),sizeof(this->clientList)-1);
		this->clientList[sizeof(this->clientList)-1] = '\0';
	}
	char clientList[200];

};

class  UVPNListProfiles : public UEvent
{
public:
	UVPNListProfiles(ETechSelect techId=VPN_TECH):UEvent(UCONN_CMD_VPN_LIST_PROFILES)
	{
		setSize(sizeof(*this));
        this->techId = techId;
	}
};

class UVPNListProfilesRsp : public UEvent
{
public:
	UVPNListProfilesRsp(string profileList):UEvent(UCONN_EVT_VPN_LIST_PROFILES)
	{
		setSize(sizeof(*this));
		this->techId = VPN_TECH;
		strncpy(this->profileList,profileList.c_str(),sizeof(this->profileList)-1);
		this->profileList[sizeof(this->profileList)-1] = '\0';
	}
	char profileList[200];
};


class  UVPNStart : public UEvent
{
public:
	UVPNStart(string vpnName):UEvent(UCONN_CMD_VPN_START)
	{
		setSize(sizeof(*this));
		this->techId = VPN_TECH;
		strncpy(this->vpnName,vpnName.c_str(),sizeof(this->vpnName)-1);
		this->vpnName[sizeof(this->vpnName)-1] = '\0';

	}
	char vpnName[50];
};

class  UVPNStartRsp : public UEvent
{
public:
	UVPNStartRsp(U_UINT32 startStatus):UEvent(UCONN_EVT_VPN_START_STATUS)
	{
		setSize(sizeof(*this));
		this->techId = VPN_TECH;
		this->startStatus = startStatus;
	}
	U_UINT32 startStatus;
};

class  UVPNStop : public UEvent
{
public:
	UVPNStop(ETechSelect techId=VPN_TECH):UEvent(UCONN_CMD_VPN_STOP)
	{
		setSize(sizeof(*this));
        this->techId = techId;
	}
};

class  UVPNStopRsp : public UEvent
{
public:
	UVPNStopRsp():UEvent(UCONN_EVT_VPN_STOP_STATUS)
	{
		setSize(sizeof(*this));
		this->techId = VPN_TECH;
	}

};



class  UWifiPreferredNetworksEvt : public UEvent
{	
public:	
	UWifiPreferredNetworksEvt(string encodedPrefSsids):UEvent(UCONN_EVT_WIFI_PREFERRED_NETWORKS)
	{
		this->techId = WIFI_TECH;
		setSize(sizeof(*this));
		strncpy(preferredSsids,encodedPrefSsids.c_str(),sizeof(this->preferredSsids)-1);
		preferredSsids[sizeof(this->preferredSsids)-1] = '\0';
	}
	U_SINT8 preferredSsids[100];
};

class  UGetEnabledTechList : public UEvent
{
public:
	UGetEnabledTechList():UEvent(UCONN_CMD_BEARER_GET_ENABLED_TECH)
	{
		setSize(sizeof(*this));
		this->techId = ALL_TECH;
	}

};

class  UGetPriorityTechList : public UEvent
{
public:
	UGetPriorityTechList():UEvent(UCONN_CMD_BEARER_GET_PRIORITY_TECH)
	{
		setSize(sizeof(*this));
		this->techId = ALL_TECH;
	}

};

class  UGetEnabledTechListRsp : public UEvent
{
public:
	UGetEnabledTechListRsp(U_UINT32* enabledTech):UEvent(UCONN_EVT_BEARER_ENABLED_TECH_RSP)
	{
		setSize(sizeof(*this));
		this->techId = ALL_TECH;
		for(int i=0;i<MAX_NUMBER_OF_NETWORKMANAGERS;i++)
		{
			if(enabledTech[i] == 1)
			{
				this->enabledTechs[i] = 1;
			}
			else
			{
				this->enabledTechs[i] = 0;
			}
		}
		checkId=10;


	}
	U_UINT32 enabledTechs[MAX_NUMBER_OF_NETWORKMANAGERS];
	U_UINT32 checkId;
};

class  UGetPriorityTechListRsp : public UEvent
{
public:
	UGetPriorityTechListRsp(U_UINT32* priorityTech):UEvent(UCONN_EVT_BEARER_PRIORITY_TECH_RSP)
	{
		setSize(sizeof(*this));
		this->techId = ALL_TECH;
		memcpy((void*)priorityList,(const void*)priorityTech,sizeof(priorityList));
	}
	U_UINT32 priorityList[MAX_NUMBER_OF_NETWORKMANAGERS];
};

//Wifi Stats Event

class UWifiStatsRsp : public UEvent
{
public:
	UWifiStatsRsp(string& encodeStr):UEvent(UCONN_EVT_WIFI_STATS_RSP)
	{
		setSize(sizeof(*this));
		this->techId = VPN_TECH;
		strncpy(this->wifiStats,encodeStr.c_str(),sizeof(wifiStats)-1);
		this->wifiStats[sizeof(wifiStats)-1] = '\0';
	}
	char wifiStats[200];
};


/* Events defined for 3G */

class  UNMParamUpdate : public UEvent{
public:
	UNMParamUpdate(U_UINT32 param,U_UINT32 val, U_UINT32 val2):UEvent(UCONN_EVT_PARAM_UPDATE),
			NM_Param(param),value(val),value2(val2)
	{
		setSize(sizeof(*this));
	}
	U_UINT32 NM_Param;				/* Enum of NM Param to be set */
	U_UINT32 value;					/* value of NM Param */
	U_UINT32 value2;
};

class  UNMNetworkNameUpdate : public UEvent{
public:
	UNMNetworkNameUpdate(string& param):UEvent(UCONN_EVT_NETWORK_NAME_UPDATE)
	{
		setSize(sizeof(*this));
		strncpy(this->NM_NetworkName,param.c_str(),sizeof(NM_NetworkName)-1);
		NM_NetworkName[sizeof(NM_NetworkName)-1] = '\0';
	}
	char NM_NetworkName[50];				// Enum of NM Param to be set 
};

class  UNMVersionInfo : public UEvent{
public:
	UNMVersionInfo(string encodeStr):UEvent(UCONN_RSP_NM_VERSION_INFO)
	{
		setSize(sizeof(*this));
		this->techId = THREE_G_TECH;
		strncpy(this->VersionInfo,encodeStr.c_str(),sizeof(VersionInfo)-1);
		this->VersionInfo[sizeof(VersionInfo)-1] = '\0';
	}
	char VersionInfo[500];
};


class  UNMSearchNetworksRsp : public UEvent{
public:
	UNMSearchNetworksRsp(string encodeStr):UEvent(UCONN_RSP_NM_NETWORK_SEARCH)
	{
		setSize(sizeof(*this));
		this->techId = THREE_G_TECH;
		strncpy(this->threeGNetworkName,encodeStr.c_str(),sizeof(threeGNetworkName)-1);
		this->threeGNetworkName[sizeof(threeGNetworkName)-1] = '\0';
	}
	char threeGNetworkName[50];
};


class  UNMUSSDRsp : public UEvent{
public:
	UNMUSSDRsp(string encodeStr):UEvent(UCONN_EVT_USSD)
	{
		setSize(sizeof(*this));
		this->techId = THREE_G_TECH;
		strncpy(this->USSDStr,encodeStr.c_str(),(sizeof(USSDStr)-1));
		this->USSDStr[(sizeof(USSDStr)-1)] = '\0';
	}
	char USSDStr[MAX_USSD_LENGTH];
};

class  UNMUSSDToGUI : public UEvent{
public:
	UNMUSSDToGUI(string param):UEvent(UCONN_EVT_NM_USSD_RSP)
	{
		setSize(sizeof(*this));
		strncpy(this->NM_USSDString,param.c_str(),(sizeof(NM_USSDString)-1));
		NM_USSDString[(sizeof(NM_USSDString)-1)] = '\0';
	}
	char NM_USSDString[MAX_USSD_LENGTH];				
};

class  UNMThreeGStats: public UEvent{
public:
	UNMThreeGStats(string& encodeStr):UEvent(UCONN_EVT_3G_STATS_RSP)
	{
		setSize(sizeof(*this));
		this->techId = THREE_G_TECH;
		strncpy(this->threeGStats,encodeStr.c_str(),sizeof(threeGStats)-1);
		this->threeGStats[sizeof(threeGStats)-1] = '\0';
	}
	char threeGStats[200];
};

class  UNM3GSetProfile: public UEvent{
public:
	UNM3GSetProfile(Ras_Details_s* rasDetails):UEvent(UCONN_CMD_SET_3G_RAS_PROFILE)
	{
		setSize(sizeof(*this));
		this->techId = THREE_G_TECH;
		this->rasDetails = *rasDetails;
	}
	Ras_Details_s rasDetails;
};

class  UNM3GGetProfile: public UEvent{
public:
    UNM3GGetProfile():UEvent(UCONN_CMD_GET_3G_RAS_PROFILE)
    {
        setSize(sizeof(*this));
        this->techId = THREE_G_TECH;
    }
};


/* Event sent from NM to GUI to show the different errors */
class  UNMErrorDisplayEvt : public UEvent{
public:
	UNMErrorDisplayEvt(ETechSelect techId ,string& param, ENM_Error ErrType, U_UINT32 errorCode):UEvent(UCONN_EVT_NM_ERROR_DISPLAY)
	{
		setSize(sizeof(*this));
		this->techId = techId;
		strncpy(this->NM_ErrorDisplay, param.c_str(),sizeof(NM_ErrorDisplay)-1);
		NM_ErrorDisplay[sizeof(NM_ErrorDisplay)-1] = '\0';
		ErrorType = ErrType; 
		this->errorCode = errorCode;
	}
	char NM_ErrorDisplay[300];				// The error to be displayed to the user.  
	ENM_Error ErrorType;						// Type of Error, if only informative or user intervention reqd.
	U_UINT32 errorCode;
};

/* Event for sending a signal to decide whether the GUI should show the
   settings tab for that particular NM */
class  UNMShowSettings : public UEvent{
public:
	UNMShowSettings(U_BOOL setting, U_UINT32 tech):UEvent(UCONN_EVT_NM_SHOW_SETTINGS),
			showSettings(setting)
	{
		this->techId = tech;
		setSize(sizeof(*this));
	}
	U_BOOL showSettings;				/* 0 - Don't show, 1 - Show settings */
};






#endif /*UCONN_EVENT_H_*/
