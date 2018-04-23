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

//extern CM_Config_s config;
extern CM_Info_s gCMInfo;
extern ULog uLogger;

#define UCONN_CLI_DEFAULT_CONFIG_FILE "C:/CMSocketService/config.txt"
#define UCONN_CONFIG_BUF_SIZE 100



#define MAX_APN				10
#define MAX_APN_LEN			50
#define MAX_NW_SIZE			50
#define MAX_USBEP_SIZE		50

typedef U_UINT32 CSCMState_t;


U_SINT32 UCM_Init(U_VOID (*eventCB)(UEvent *));
UEvent * UCM_Execute(UEvent *reqFromClient_p);
U_VOID handleCallBackFromDevices(U_UINT32 cmdId,U_UINT32 techId);
//U_VOID UCM_Info(CM_Config_Info_s &configInfo);
U_VOID UCM_Info(CM_Config_Info_s *configInfo);
CM_Config_Info_s* UCM_getConfig();
U_VOID unInitialsieUCM();


/*need to push this function to LTE_NM*/
/***********************************************************************
*	Returns a unique integer to be used as Context ID
***********************************************************************/
static U_UINT32 uconn_GetCtxID ()
{
	static U_UINT32 ctxId = 0;

	((ctxId == 0xffff)? ctxId = 0: ctxId += 1);
	
	return ctxId;
}


#endif /* UCONN_SERVERAPI_H */
