/*****************************************************************************
**
** uconn_NetworkMgr.h
** This header file describes the interfaces to the Network Manager Base class
**
** Author: Sathyanarayanan S
** Date  : 05-MAR-2012
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		05/03/12     SS			Creation
****************************************************************************/



#ifndef UNETWORK_MANAGER_H_
#define UNETWORK_MANAGER_H_

#include "uconn_Event.h"

class UConnServer;
class UConnATHandler;

typedef struct Common_NM_Info
{
	U_UINT8 dalMode;
	string com_port;
	U_UINT32 device_status;
	U_BOOL isEmulatorMode;
	/* Version information */
	Version_Info_s versionInfo;
	Config_Param_s cfg_params;
	IMS_param  imsParams;
	U_SINT8 hwIdCOMPort[100];
	U_SINT8 rasProfile[100];
	U_SINT8 rasUserName[100];
	U_SINT8 rasPassword[100];
	U_SINT8 rasPhoneNumber[20];
	U_SINT8 APN_name[MAX_APN_SIZE];
public:
	Common_NM_Info()
	{
		dalMode = 0;
		com_port.clear();
		device_status = 0;
		isEmulatorMode = false;
		memset(hwIdCOMPort, 0, 100*sizeof(U_SINT8));
		memset(rasProfile, 0, 100*sizeof(U_SINT8));
		memset(rasUserName, 0, 100*sizeof(U_SINT8));
		memset(rasPassword, 0, 100*sizeof(U_SINT8));
		memset(rasPhoneNumber, 0, 20*sizeof(U_SINT8));
		memset(APN_name, 0, (MAX_APN_SIZE)*sizeof(U_SINT8));
	}
}Common_NM_Info_s;



class UNetworkManager
{
protected:
	UEventQueue* bearerMngrQueue;

public:
	ENetworkEnableStatus enableStatus;
	ENetworkConnectionStatus connectionStatus;
	ENMStateForBM state;
	Common_NM_Info_s mCommonInfo;
	U_UINT32 nmPriority;
	U_UINT32 nmOldPriority;
	UConnATHandler* mATHandler;

public:
	UNetworkManager(UEventQueue* bearerManagerQueue);
	virtual ~UNetworkManager();

public:
	/* UConnServer instance for all NMs */
	UConnServer *mNMServer;
	virtual U_VOID enableNetMngr()=0;
	virtual U_VOID disableNetMngr() = 0;
	virtual U_VOID connectToNetwork() = 0;
	virtual U_VOID searchNetworks() = 0;
	virtual U_VOID disconnectFromNetwork() = 0;
	virtual U_VOID initNetMngr(void *cfgInfo)=0;
	void postEventToBearerManager(UEvent* evt); // api to post event from NM to BM
	virtual U_UINT32 setParam(U_UINT32 paramID,string param)=0;

	virtual U_VOID setPriority(U_UINT32 techID,U_UINT32 priority); 	
	virtual U_UINT32 getPriority();	
	virtual U_UINT32 getOldPriority();
	virtual U_VOID getParam(Get_NM_Param paramID)=0;
	virtual U_UINT32 getStatisticsNM()=0;
	virtual ENMStateForBM getStateNM(); 
	virtual ENetworkConnectionStatus getConnectionStatus();
	virtual ENetworkEnableStatus getEnableStatus();
	virtual ETechSelect getTechID()=0;
	virtual U_VOID deviceEventsCallBack(U_UINT32 cmdID,U_UINT32 techID) = 0;
	virtual U_VOID updateNMStateAndPost(ETechSelect techID,ENMStateForBM state);
	virtual U_UINT32 getMeasStatisticsNM(){return 0;};
	virtual U_VOID handleEvent(UEvent* evt){delete evt;};

	UConnServer* getNMServer()
	{
		return mNMServer;
	}
};

#endif //UNETWORK_MANAGER_H_