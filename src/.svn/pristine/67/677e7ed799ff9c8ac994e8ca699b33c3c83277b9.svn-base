/*****************************************************************************
**
** LTE_NM.h
** This header file describes the interfaces to the LTE Network Manager
**
** Author: Sathyanarayanan S
** Date  : 06-MAR-2012
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		06/03/12     SS			Creation
****************************************************************************/
#ifndef ULTE_NM_H_
#define ULTE_NM_H_

#include "NetworkManager.h"

//class UConnServer;
typedef CM_Info LTE_NM_Info;
class ULTE_NM:public UNetworkManager
{
private:

public:
	LTE_NM_Info mNMInfo;
	ULTE_NM(UEventQueue* msgQueue);
	~ULTE_NM();

public:
	U_VOID enableNetMngr();
	U_VOID disableNetMngr();
	U_VOID connectToNetwork();
	U_VOID searchNetworks();
	U_VOID disconnectFromNetwork();
	U_VOID initNetMngr(void *cfgInfo);
	U_UINT32 setParam(U_UINT32 paramID,string param);
	//U_VOID setPriority(U_UINT32 priority); 	
	//U_UINT32 getPriority();	
	U_VOID getParam(Get_NM_Param paramID);
	U_UINT32 getStatisticsNM();
	ETechSelect getTechID();
	U_VOID (*dalCallbackfn)(U_STRING strResp,
						U_UINT32 cmdId,
						UNetworkManager* p_NM);
	/* AT DAL Callback function */

	U_VOID postToSM(UEvent *evt);
	U_VOID deviceEventsCallBack(U_UINT32 cmdID,U_UINT32 techID);
	U_UINT32 getMeasStatisticsNM();
};

#endif //ULTE_NM_H_