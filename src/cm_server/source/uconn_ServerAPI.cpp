/*****************************************************************************
**	uconn_ServerAPI.cpp
**  This file describes the methods for the UConnect Server to 
**  be called by the UConnect Client
**
** Author: Sathyanarayanan S
** Date  : 28-DEC-2010
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		28/12/10     SS			Creation
****************************************************************************/

#include "uconn_StateMachine.h"
#include "uconn_ServerAPI.h"
#include "uconn_IdleState.h"
#include "uconn_SearchingState.h"
#include "uconn_IdleEvent.h"
#include "uconn_server.h"
#include "uconn_SAPIwrapper.h"
#include "uconn_AT_DAL.h"
#include "UCommonFunctions.h"
#include <cmath>
#include "BearerManager.h"

#if (defined(__linux__)||defined(__MACH__))
#define _strdup strdup
#define _strnicmp strncasecmp
#endif

CM_Info_s gCMInfo;
UConnServer *gConnServer= NULL;

U_MUTEX	timerCBLock;

UBearerManager *pBearerMngr = NULL;
/***********************************************************************
*	SAPI_UC_Init -
*	Initializes the State machine and the device connected
***********************************************************************/
string comPort;

U_SINT32 UCM_Init(U_VOID (*eventCB)(UEvent *))
{
	pBearerMngr = new UBearerManager();
	pBearerMngr->UCM_Init();
	pBearerMngr->eventBearerMngrCBHdlr = eventCB;

	return 0;
}
U_VOID unInitialsieUCM()
{
	if(pBearerMngr != NULL)
	{
		delete pBearerMngr;
		pBearerMngr = NULL;
	}
}

UEvent* UCM_Execute(UEvent *reqFromClient_p)
{
	UEvent *rspCommand = pBearerMngr->handle_ClientCommand(reqFromClient_p);

	return rspCommand;
}

void handleCallBackFromDevices(U_UINT32 cmdId,U_UINT32 techId)
{
	pBearerMngr->handleCallBacksFromDevice(cmdId,techId);
	
}


CM_Config_Info_s* UCM_getConfig(){
	CM_Config_Info_s* pConfig = NULL;
	if(pBearerMngr != NULL){
		pConfig = &(pBearerMngr->parserPtr->gCMInfo);
	}
	return pConfig; 
}

