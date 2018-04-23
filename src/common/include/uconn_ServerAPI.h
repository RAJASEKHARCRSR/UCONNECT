/*****************************************************************************
**
** uconn_ServerAPI.h
** This file describes the methods for the UConnect Server to 
** be called by the UConnect Client
**
** Author: Sathyanarayanan S
** Date  : 28-DEC-10
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		28/12/10     SS			Creation
	0.2		31/12/10	 SS			APIs modified to return relevant data 
****************************************************************************/

#ifndef UCONN_SERVERAPI_H
#define UCONN_SERVERAPI_H

#include "UTypes.h"
#include "ULog.h"
#include "uconn_SAPIwrapper.h"

extern ULog uLogger;

#define UCONN_CLI_DEFAULT_CONFIG_FILE "./config.txt"
#define UCONN_CONFIG_BUF_SIZE 100

//extern CM_Config_s config;

#define MAX_APN				10
#define MAX_APN_LEN			50
#define MAX_NW_SIZE			50
#define MAX_USBEP_SIZE		50

typedef U_UINT32 CSCMState_t;

struct TABLE_ENTRY_s
{
	U_SINT8 apn[MAX_APN_LEN];
	U_UINT32 apn_type;
    U_UINT32 eps_bearer_id;
    U_UINT32 APN_id;
    U_SINT8 network_interface[MAX_NW_SIZE];
    U_SINT8 usb_endpoint[MAX_USBEP_SIZE];
    U_SINT8 src_addr[MAX_ADDRESS_SIZE];
};

#if 0 /* Defined in uconn_SAPIWrapper.h */
typedef struct PLMN_List {
	U_UINT32 PLMN_id; //	PLMN identity.
	U_UINT32 rat;	   // Access technology. If NULL implies no access 
				   // technology returned by underlying stack.
	U_UINT32 list_membership; 

} PLMN_List_s;

typedef struct CM_Info
{
	CSCMState_t CurrentState; /* Refer to CMStates above */
	U_UINT32 Config_mode;	  /* Refer to Config_mode above */
	U_UINT32 num_plmns;
	PLMN_List_s nw_list[MAX_APN];			
	U_UINT32 net_search_status; /* Refer to nwsrch_status above */
}CM_Info_s;
#endif

extern CM_Info_s gCMInfo;

typedef struct APN_Table
{
	U_UINT32 num_apns;
	TABLE_ENTRY_s APN_List[MAX_APN];
}APN_Table_s;

/***********************************************************************
*	Returns a unique integer to be used as Context ID
***********************************************************************/
static U_UINT32 uconn_GetCtxID ()
{
	static U_UINT32 ctxId = 0;

	((ctxId == 0xffff)? ctxId = 0: ctxId += 1);
	
	return ctxId;
}

/* Initialize Handle, CM State Machine */
U_SINT32 SAPI_UC_Init(U_VOID (*eventCB)(UEvent *));

/* This API would call a DAL API, which would finally call the 
   net_search Common API call */
U_UINT32	SAPI_UC_NetworkSearch (U_UINT32 plmn_timer);

/* This API is used to query the status of the N/W Search call */
NetSearch_Result_s	SAPI_UC_NWSearchStatus ();

/* The net_attach DAL API would be called within this function after
   setting up the Primary and Secondary (if required) PDP contexts */
U_SINT32	SAPI_UC_Connect (U_UINT32 plmn_idx);

/* This API would end up calling the net_detach via the DAL of the CM */
U_SINT32	SAPI_UC_Disconnect();

/* This API would in turn call equip_info_get, net_status_get, net_cfg_get
   DAL APIs to obtain the Network and Registration status */
CM_Info_s	SAPI_UC_ConnectionStatus(U_UINT32 rat);

/* net_sig_qual_get would be the DAL API to be called via this function */
CM_Statistics_s	SAPI_UC_ConnectionStats(U_UINT32 rat);

/* This API calls the net_cfg_set and the equip_cfg_set via the DAL to 
   Configure network related parameters */
U_SINT32	SAPI_UC_SetNetworkParams(U_UINT32 cfg_type,
									 U_PVOID config_data);

/* SAPI to retrieve the APN table in the Modem */
APN_Table_s	SAPI_UC_GetAPNTable();

/* SAPI to set the APN table with entries coming from the client */
U_SINT32	SAPI_UC_SetAPNTable(TABLE_ENTRY_s APN_Table[MAX_APN]);

/* SAPI to get the statistics from the Server */
U_UINT32	SAPI_UC_GetStat(U_UINT32 Param);

/* SAPI to set the Config Parameter with the given value */
void		SAPI_UC_SetParam(U_UINT32 param, U_UINT32 value);


UEvent * SAPI_UC_Execute(UEvent *reqFromClient_p);

U_VOID SAPI_eventCB (UEvent *event_p);
#endif /* UCONN_SERVERAPI_H */
