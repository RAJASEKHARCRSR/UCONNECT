/*****************************************************************************
**
** ucon_SAPIwrapper.h
** This file contains the structure definitions and prototypes used in 
** uconn_SAPIwrapper.cpp
**
** Author: Vasuki Setlur
** Date  : 05-JAN-11
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		05/01/11     VS			Creation
****************************************************************************/
#ifndef _UCONN_SAPIWRAPPER_H
#define _UCONN_SAPIWRAPPER_H

#include <stdio.h>
#include <stdlib.h>
#include "UTypes.h"
#include "uconn_OSAL.h"
#include "uconn_Config.h"
#include "uconn_Event.h"

using namespace std;
typedef enum uconnSAPIwrapErr_e {
	UCONN_SAPI_WRAP_SUCCESS = 0,
	UCONN_SAPI_WRAP_FAILURE,
	UCONN_SAPI_WRAP_PARAMETER_ERROR
} uconnSAPIwrapErr_t;


typedef struct uconnSAPIstrResp{
	uconnSAPIwrapErr_t status;
	// Add a String response here
	U_STRING             strResp;
}uconnSAPIstrResp_s;

uconnSAPIstrResp_s* uconn_SAPIExecute (U_SINT32 cmdId,
									  U_SINT8 *cmdComponents[], 
									  U_SINT32 numberOfArgs);


UEvent* uconn_SAPIExecute (UEvent *cmd);





/* Common Structures and definitions used by Client and Server */
#if 0
typedef enum
{
	SUCCESS			  = 0,
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
	ERROR_BAD_STATE = 601
}Error_values;
#endif


typedef enum
{
	IDLE = 0,
	NWLISTUPDATED = 1,
	CONNECTING = 2,
	CONNECTED = 3,
	DISCONNECTING= 4
}CMStates;

/*
typedef enum
{
	OFF_4G = 0,
	ON_4G = 1
}Enable4G;
*/

#define RO_BIT 0x8000
#define READ_WRITE(val)     ((val) | (RO_BIT))
#define READ_ONLY(val)      ((val) & (~RO_BIT))
#define IsReadOnly(val)     (((val) & (RO_BIT))== 0)
#define MASK_ROBIT(val)     ((val) & (~RO_BIT))




#define DATA_RATE               READ_ONLY(0)
#define UL_DATA                 READ_ONLY(DATA_RATE + 1)
#define DL_DATA                 READ_ONLY(UL_DATA + 1)
#define CM_STATE                READ_ONLY(DL_DATA + 1)
#define RSSI_VAL                READ_ONLY(CM_STATE + 1)
#define CINR_VAL                READ_ONLY(RSSI_VAL + 1)
#define ROAMING_INFO            READ_ONLY(CINR_VAL + 1)
#define IP_ADDR                 READ_ONLY(ROAMING_INFO + 1)
#define SUBNET_MASK             READ_ONLY(IP_ADDR + 1)
#define GW_ADDRESS              READ_ONLY(SUBNET_MASK + 1)
#define ACTIVATION_STATUS       READ_ONLY(GW_ADDRESS + 1)
#define MODEM_STATE		        READ_ONLY(ACTIVATION_STATUS + 1)
#define SYSTEM_NOTIFICATION     READ_ONLY(MODEM_STATE + 1)
#define APN_NAME                READ_ONLY(SYSTEM_NOTIFICATION + 1)
#define IPv4_ADDR               READ_ONLY(APN_NAME + 1)
#define IPv6_ADDR               READ_ONLY(IPv4_ADDR + 1)
#define IPv4_SUBNET_MASK        READ_ONLY(IPv6_ADDR + 1)
#define IPv6_SUBNET_MASK        READ_ONLY(IPv4_SUBNET_MASK + 1)
#define IPv4_GW_ADDRESS         READ_ONLY(IPv6_SUBNET_MASK + 1)
#define IPv6_GW_ADDRESS         READ_ONLY(IPv4_GW_ADDRESS + 1)
#define IPv4_DNS				READ_ONLY(IPv6_GW_ADDRESS + 1)
#define IPv6_DNS				READ_ONLY(IPv4_DNS + 1)
#define MNC                     READ_ONLY(IPv6_DNS + 1)
#define MCC                     READ_ONLY(MNC + 1)

#define CFG_ENABLE_4G           READ_WRITE(MCC + 1)
#define CFG_CONFIG_MODE         READ_WRITE(CFG_ENABLE_4G  + 1)
#define CFG_ATTACH_RAT          READ_WRITE(CFG_CONFIG_MODE + 1)
#define CFG_DISABLE_RAT         READ_WRITE(CFG_ATTACH_RAT  + 1)
#define CFG_LOGGING_LEVEL       READ_WRITE(CFG_DISABLE_RAT + 1)
#define CFG_ENABLE_LOG          READ_WRITE(CFG_LOGGING_LEVEL + 1)
#define CFG_ATCOMMAND_MODE		READ_WRITE(CFG_ENABLE_LOG + 1)
//Emulator

#define IS_EMULATOR_MODE		READ_WRITE(CFG_ATCOMMAND_MODE + 1)

#define DAL_MODE				READ_WRITE(IS_EMULATOR_MODE + 1)

//IMS SETTING
#define SMS_WRITE_UICC          READ_WRITE(DAL_MODE + 1)
#define SMS_OVER_IMS            READ_WRITE(SMS_WRITE_UICC + 1)
#define SMS_FORMAT              READ_WRITE(SMS_OVER_IMS + 1)
#define T1_TIMER                READ_WRITE(SMS_FORMAT + 1)
#define T2_TIMER                READ_WRITE(T1_TIMER + 1)
#define TF_TIMER                READ_WRITE(T2_TIMER + 1)
#define TEST_MODE               READ_WRITE(TF_TIMER + 1)
#define IMS_REG_STATUS          READ_WRITE(TEST_MODE + 1)
#define IMS_REGN_ONLY			READ_WRITE(IMS_REG_STATUS + 1)

//ALTAIR
#define EARCFN					READ_WRITE(IMS_REGN_ONLY + 1)
#define ATR_RSRP				READ_WRITE(EARCFN + 1)
#define SIG_STRENGTH            READ_WRITE(ATR_RSRP + 1)

#define NM_CFG_PRIORITY			READ_WRITE(SIG_STRENGTH + 1)
#define NM_CFG_ENABLED			READ_WRITE(NM_CFG_PRIORITY + 1)

//NOTHING GOES BEYOND THIS Last Param type should be last param in the above
//list + 1
#define LAST_PARAM_TYPE         MASK_ROBIT(CFG_ENABLE_LOG + 1)


#ifdef UCONN_CLIENT_USE_SOCKETS

/* prototupes */
uconnSAPIwrapErr_t uconn_SAPIwrapConnectToSever ( U_SINT8 *serverIp_p, U_UINT16 port);
U_VOID uconn_SAPIwrapDisconnectFrmSever ();
#endif /*UCONN_CLIENT_USE_SOCKETS*/
//uconnSAPIwrapErr_t uconn_SAPIwrapInit (U_VOID (*eventHandler)(U_SINT32, U_SINT8*),
//									   U_VOID (*logHandler)(U_SINT32, U_SINT8*));
uconnSAPIwrapErr_t uconn_SAPIwrapInit (U_VOID (*eventHandler)(UEvent *, U_SINT8*, U_VOID *),
									   U_VOID *evtHdlrParam_t);
U_VOID uconn_SAPIwrapEventCallback (UEvent *event_p);
bool IsGet(U_SINT8 *param);

#endif
