/*****************************************************************************
**
** uconn_AT_DAL.h
** Implements Device Abstraction Layer for AT Commands
**
** Author: Sathyanarayanan S
** Date  : 20-JUL-2011
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		20/07/11     SSN		Creation
****************************************************************************/
#ifndef _UCONN_AT_DAL_H_
#define _UCONN_AT_DAL_H_

/////////////////////////////////////////////////////////////////////////////
/////////////////////////// INCLUDE FILES ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#include "uconn_OSAL.h"

#ifdef WIN32
//#include "UEmulateSerialPort.h"
#endif

#include "uconn_DAL.h"
#include "uconn_Event.h"
#include "UBufferParser.h"
#include "LTE_NM.h"
#include "ThreeG_NM.h"

/////////////////////////////////////////////////////////////////////////////
///////////////////////  CONSTANTS //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


#define		DAL_SUCCESS				(0)
#define		DAL_ERROR				(1)
#define		CR						13
#define		LF						10

//T_PS_DATA_COMP
#define    E_DATA_COMP_V42                        0
#define    E_DATA_COMP_MANUF                    1
#define    E_DATA_COMP_NONE                    2

//T_PS_HDR_COMP
#define    E_HDR_COMP_RFC1144                  0
#define    E_HDR_COMP_RFC2507                    1
#define    E_HDR_COMP_ROHC                        2    //RFC3095
#define    E_HDR_COMP_MANUF                    3
#define    E_HDR_COMP_NONE                        4

//U_PS_PDP_TYPE
#define		E_PDP_TYPE_IPV4						1
#define		E_PDP_TYPE_IPV6						2
#define		E_PDP_TYPE_IPV4V6					3
#define		E_PDP_TYPE_IP						4
#define		E_PDP_TYPE_UNKNOWN					5



//U_PS_CTX_INFO_TYPE
#define    E_INFO_NEG_QOS_2G                  0
#define    E_INFO_NEG_QOS_3G                  1
#define    E_INFO_NEG_QOS_4G                  2
#define    E_INFO_ACTIVATION_STATUS          3
#define    E_INFO_BYTES_RCVED                  4
#define    E_INFO_BYTES_SENT                  5
#define    E_INFO_TOTAL_BYTES_RCVED          6
#define    E_INFO_TOTAL_BYTES_SENT              7
#define    E_INFO_DYN_DNS                      8
#define    E_PDP_ADDRESS_TYPE                  9
#define    E_INFO_PDP_ADDRESS                  10
#define    E_INFO_APN_NAME                      11
#define    E_INFO_PRIMARY_BEARER_ID             12
#define	   E_INFO_SECONDARY_BEARER_ID          13
#define    E_INFO_SUBNET_MASK                  14
#define    E_INFO_GW_ADDRESS                  15
#define	   E_INFO_DYN_P_CSCF                  16
#define    E_INFO_TFT                          17

//U_IP_IF_STATUS
#define E_IP_IF_STATUS_UP                        1
#define E_IP_IF_STATUS_DOWN                      0
#define E_IP_IF_STATUS_FAILURE                   2 

//U_RAT
#define E_RAT_GSM                           0
#define E_RAT_GSM_COMPACT                   1
#define E_RAT_UTRAN                         2
#define E_RAT_EGPRS                         3
#define E_RAT_HSDPA                         4
#define E_RAT_HSUPA                         5
#define E_RAT_UTRAN_HSDPA_HSUPA             6
#define E_RAT_EUTRAN                        7

//U_STAT_REQUEST
#define E_STAT_GENERAL						0
#define E_STAT_MAC							1
#define E_STAT_PHY							2

//U_PS_CTX_CFG_TYPE
#define E_CFG_QOS_2G						1
#define E_CFG_MIN_QOS_2G                    2
#define E_CFG_REQ_QOS_3G                    3
#define E_CFG_MIN_QOS_3G                    4
#define E_CFG_QOS_4G                        5
#define E_CFG_TFT							6
#define E_CFG_PCO_MS                        7

//CGEREP Values
#define CGEREP_ENABLE						1
#define CGEREP_DISABLE						0

//CEREG Values 
#define CEREG_NW_REG_DISABLE				0
#define CEREG_NW_REG_ENABLE					1
#define CEREG_NW_REG_LOC_INFO_ENABLE		2

//MCNFG Values 
#define MCNFG_PDN_CONN_REQUEST				20

//IMSCFG Values
#define IMSCFG_SMS_OVER_IMS                 1
#define IMSCFG_T1_TIMER                     2
#define IMSCFG_T2_TIMER                     3
#define IMSCFG_TF_TIMER                     4
#define IMSCFG_SMS_FORMAT                   5
#define IMSCFG_IMS_REGN                     6
#define IMSCFG_TEST_MODE                    7

//IMSCFG_SMS_OVER_IMS
#define SMS_OVER_IMS_DISABLE                0
#define SMS_OVER_IMS_ENABLE                 1

//IMSCFG_SMS_FORMAT
#define SMS_FORMAT_3GPP2                    0
#define SMS_FORMAT_3GPP                     1

//IMSCFG_IMS_REGN
#define IMS_REGN_DEREGISTER                 0
#define IMS_REGN_REGISTER                   1

//IMSCFG_TEST_MODE
#define TEST_MODE_DISABLE                   0
#define TEST_MODE_ENABLE                    1

//*MIMSRSP values
#define IMSEV_DISABLE                       0
#define IMSEV_ENABLE                        1



/////////////////////////////////////////////////////////////////////////////
/////////////////////////// TYPEDEFS ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

typedef int DAL_STATUS;
typedef unsigned int U_PLMN_SELECT_MODE;
typedef char* U_PLMN;
typedef unsigned int U_RAT;
typedef U_UINT32 U_PS_PDP_TYPE;
typedef U_UINT32 U_PS_CTX_CFG_TYPE;
typedef U_UINT32 U_PS_CTX_INFO_TYPE;
typedef U_UINT32 U_IP_IF_STATUS;

typedef enum U_NET_CFG {
	E_CFG_ACTIVATED_RAT = 0,
	E_CFG_OPER_CLASS = 1,
	E_PDP_TYPE = 2,
	E_NS_TYPE = 3
}U_NET_CFG;

typedef struct
{
    U_UINT32 u32Bitfields;
    U_PS_PDP_TYPE  pdp_type;
    U_SINT8    apn[MAX_APN_SIZE];
    U_SINT8    pdp_addr[MAX_PDP_ADDRESS_SIZE];
    U_UINT32    data_comp;
    U_UINT32    hdr_comp;
}U_PS_CTX_PRIMARY_PARAMS;

typedef struct
{
    U_UINT32 u32Bitfields;
    U_UINT32 qci;
    U_UINT32 dl_gbr;
    U_UINT32 ul_gbr;
    U_UINT32 dl_mbr;
    U_UINT32 ul_mbr;
}U_PS_CTX_QOS_4G;

typedef struct
{
    U_UINT32 u32Bitfields;
    U_UINT32 packet_filter_id;
    U_UINT32 eval_prec_index;
    U_SINT8 source_addr[MAX_ADDRESS_SIZE];
    U_UINT32 protocol_number;
    U_SINT8 dest_port[MAX_PORT_RANGE];
    U_SINT8 source_port[MAX_PORT_RANGE];
    U_UINT32  spi;
    U_SINT8 tos[MAX_TOS_SIZE];
    U_UINT32 flow_label;
    U_UINT32  direction;
    U_UINT32  NW_packet_filter_id;
}U_TFT;

typedef union
{
    U_PS_CTX_QOS_4G ps_ctx_qos_4g;
    U_TFT tft;
}U_PS_CTX_CFG;

typedef struct
{
    U_UINT32 u32Bitfields;
    U_SINT8 apn[MAX_APN_SIZE];
    U_PS_PDP_TYPE apn_type;
    U_UINT32 cid;
    U_UINT32 APN_id;
    U_SINT8 network_interface[MAX_NW_SIZE];
    U_SINT8 usb_endpoint[MAX_USBEP_SIZE];
    U_SINT8 source_addr[MAX_ADDRESS_SIZE];
	U_UINT32 inactivity_timer;
}U_APN_TABLE_ENTRY;

typedef struct
{
    U_UINT32 PLMN_count;
    U_APN_TABLE_ENTRY plmn_search_entry[MAX_NUM_APN];
}U_APN_TABLE;

typedef struct
{
    U_UINT32 u32Bitfields;
    U_UINT32    data_comp;
    U_UINT32    hdr_comp;
}U_PS_CTX_SECONDARY_PARAMS;

typedef struct
{
    U_UINT32 ctx_id;
}U_EVT_PRIMARY_ACT;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////// EXPORTED FUNCTIONS //////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//DAL_STATUS	uconnATInitDevice(P_AT_CALLBACK& fnCallback,
//							  string sComPort);

typedef U_VOID(*P_AT_CALLBACK_WITH_ID) (U_STRING sResp,
										U_UINT32 cmdId,
										UNetworkManager* p_NM);

DAL_STATUS uconnSendATCommandMode(U_STRING cmdStr);

void InitCallbackWithId(string strResp,
						unsigned int cmdId,
						UNetworkManager* p_NM);



class uconnAtDal:public uconnDal{

protected:
	UBufferParser*  bufferParser;
	
DAL_STATUS	uconnATInitDevice(string sComPort, UNetworkManager* pNM);

DAL_STATUS	uconnSetATErrorMode();

DAL_STATUS	uconnGetPLMNInfo();

DAL_STATUS	uconnQueryPINStatus();



DAL_STATUS	uconnATNetAttach(U_PLMN_SELECT_MODE plmn_select_mode, 
							  U_PLMN plmn_id, 
							  U_RAT rat, 
							  int primary_ctx_id);



DAL_STATUS	uconnATNetCfgSet(U_UINT32 ctx_id,
							 U_NET_CFG cfg_type,
							 U_UINT32 cfg_value);



DAL_STATUS uconnATPsCtxDefPrimary(U_UINT32 ctx_id,
								   U_PS_CTX_PRIMARY_PARAMS* pri_params);



DAL_STATUS	uconnATNetDetach();


DAL_STATUS uconnATNetStatusGet(U_UINT32 rat);

DAL_STATUS uconnATNetStatusSet(U_UINT32 nwReg);



DAL_STATUS uconnATNetSigQualGet( );

//DAL_STATUS virtual uconnATStatsUpdate(U_UINT32 statCmd)=0;

DAL_STATUS uconnATPsCtxInfoGet(U_UINT32 ctx_id,
							   U_PS_CTX_INFO_TYPE info_type);

DAL_STATUS uconnATpropIPIFStatus(U_UINT32 ctx_id,
								U_IP_IF_STATUS IF_Status);


DAL_STATUS uconnATNetSearch();

DAL_STATUS uconnATPsCtxDefSecondary(U_UINT32 ctx_id,
									 U_UINT32 pri_ctx_id,
									 U_PS_CTX_SECONDARY_PARAMS* sec_params);

DAL_STATUS uconnATPsCtxActivate(U_UINT32 ctx_id);

DAL_STATUS uconnATPsCtxDeactivate(U_UINT32 ctx_id);

DAL_STATUS uconnATNetSearchStatus();

DAL_STATUS virtual uconnGetVersionInfo();

DAL_STATUS uconnPowerUp();

DAL_STATUS	uconnATEPSRegn(U_UINT32 RegInfo);

DAL_STATUS	virtual uconnATModemConfig(U_UINT32 mConfig,
								U_UINT32 ctxId);

DAL_STATUS	uconnIMSConfig(U_UINT32 cfgType,
						   U_UINT32 cfgValue);

DAL_STATUS	uconnIMSResponse(U_UINT32 rspMode);



DAL_STATUS	uconnATNetAttach_DefaultEPS(U_PLMN_SELECT_MODE plmn_select_mode, 
							  U_PLMN plmn_id, 
							  U_RAT rat, 
							  int primary_ctx_id);

	

public:
    
/*
uconnAtDal(UNetworkManager* p_NM):uconnDal()
{
	m_pNM = p_NM;
	m_pLTE_NM = NULL;
	m_pThreeG_NM = NULL;
	if(p_NM->getTechID() == LTE_TECH)
	{
		m_pLTE_NM = reinterpret_cast<ULTE_NM*>(m_pNM);
	}
	else if(p_NM->getTechID() == THREE_G_TECH)
	{
		m_pThreeG_NM = reinterpret_cast<UThreeG_NM*>(m_pNM);
	}
} 
*/

uconnAtDal(UNetworkManager* p_NM);

~uconnAtDal()
{
	m_pNM = NULL;
	m_pLTE_NM = NULL;
	m_pThreeG_NM = NULL;
}

void    callBackHandler(string strResp,unsigned int cmdId);

void  virtual  processBufferParserSetEvent(string strResp,unsigned int cmdId);

U_BOOL isDevicePresent();

DAL_STATUS uconnInit(UNetworkManager* pNM);

DAL_STATUS uconnATResetDevice();

DAL_STATUS  uconnDeInit();

DAL_STATUS  uconnReset(void*);

DAL_STATUS  virtual uconnConfigure(CONFIG_CODE configCode);

DAL_STATUS  uconnNetSearch();

DAL_STATUS uconnNetSearchStatus();

DAL_STATUS  uconnNetAttach(ATTACH_MODE,void*,int ctxId);

DAL_STATUS virtual uconnConfigNetParams(NETWORK_PARAMS,void*,int ctxId=1);


DAL_STATUS  uconnGetStatus(GET_STATUS,void*);

DAL_STATUS virtual uconnSetStatus(SET_STATUS,void*,int ctxId=1);

DAL_STATUS  uconnNetDetach();

DAL_STATUS  virtual uconnGetStatistics(GET_STATISTICS,void*);

DAL_STATUS uconnPowerStatusQuery();

DAL_STATUS uconnATPowerDown();

DAL_STATUS uconnATQueryContext();

ULTE_NM* m_pLTE_NM;
UThreeG_NM* m_pThreeG_NM;

UNetworkManager* m_pNM;

};



#endif /* _UCONN_AT_DAL_H_ */
