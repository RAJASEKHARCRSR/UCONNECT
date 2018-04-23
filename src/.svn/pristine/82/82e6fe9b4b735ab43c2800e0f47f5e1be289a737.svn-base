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

#define UCONN_WIFI_NM_LIST_NETWORKS				    3000
#define UCONN_WIFI_NM_INITIALIZE					3001
#define UCONN_WIFI_INTERFACE_STATE_CHANGE			3002
#define UCONN_WIFI_CALLBACK_EVT						3003
#define UCONN_WIFI_SCAN_TIMEOUT_EVT					3004


#define WIFI_SCAN_TIMER_INTERVAL					5000


#ifndef UWIFI_NM_H_
#define UWIFI_NM_H_
#include "NetworkManager.h"
#include "uconn_OSAL.h"
#include "uWlanUtils.h"
#include <vector>


U_VOID scanWifiNetworksCallback(U_PVOID lpParam, U_BOOL val);


//Callback from WifiUtils
void wifiNMCallback(int code);

class UWiFi_NM : public UNetworkManager,public UProcess 
{
private:
	string ssid;
	string attemptedSsid;
	string password;
	string connectedSigQuality;
	string adapterName;

	U_TIMERID scanTimerId;
	U_UINT32 ifIdx;
	bool userEnabled;
	string userSelectedSsid;

	//UProcess private eventHandler

	void handleEvent(UEvent* evt);

	U_VOID wifiEnabler();
	U_VOID wifiDisabler();
	U_VOID connectToWiFi(string ssid);
	U_VOID disconnectFromWiFi();
	U_VOID searchWifiNetworks();
	U_VOID setWifiParam(U_UINT32 paramID,string param);
	U_VOID initializeWifi();
	U_VOID handleWifiInterfaceStateChange(UEvent* evt);
	U_VOID updateWifiScanTimer(ENMStateForBM newState);
	U_VOID listWifiNetworks(string& pref);
	U_VOID wifiNMCallbackHandler(int code);
	U_VOID getWifiStats();
	U_VOID sendUnableToConnectEvent();
	U_VOID handleWifiDisconnectCB();
	U_VOID handleWifiConnectCB();
	U_VOID postNewConnectedStatus();
	U_VOID startWifiTimer();
	U_VOID stopWifiTimer();

	uwlanRetVal_e wifiConnectWithPassword(char* password);

	uWlanCB_t* wlanCB;


protected:

	

public:
	string networkListStr;
	UWiFi_NM(UEventQueue* msgQueue);
	~UWiFi_NM();

public:
	U_VOID enableNetMngr();
	U_VOID disableNetMngr();
	U_VOID connectToNetwork();
	U_VOID searchNetworks();
	U_VOID disconnectFromNetwork();
	U_VOID initNetMngr();
	U_UINT32 setParam(U_UINT32 paramID,string param);

	U_VOID getParam(Get_NM_Param paramID);
	U_UINT32 getStatisticsNM();
	ETechSelect getTechID();
	U_VOID handleDisconnectCallback();
	U_VOID handleConnectionCompleteCallback();
	U_VOID updateNMStateAndPost(ENMStateForBM state);
	U_VOID initNetMngr(void *configInfo);

	void checkStateAndPostToBM(ENMStateForBM state);
	void wifiParamGetter(UEvent* evt);
	void handleWifiScanTimeout();

	//UProcess implementations
	U_VOID Reset() { }
	U_BOOL EventHandler();
	U_VOID postToWiFiNM_Queue(UEvent* evt);
	U_VOID deviceEventsCallBack(U_UINT32 cmdID,U_UINT32 techId)
	{
	}

	


};

class  UWifiListNetworks : public UEvent
{
public:
	UWifiListNetworks(ETechSelect techId=WIFI_TECH):UEvent(UCONN_WIFI_NM_LIST_NETWORKS)
	{
		setSize(sizeof(*this));
		this->techId =  techId;
	}
};

class UWifiInitialize : public UEvent 
{
public:
	UWifiInitialize():UEvent(UCONN_WIFI_NM_INITIALIZE)
	{
		setSize(sizeof(*this));
		this->techId =  WIFI_TECH;
	}
};

class UWifiInterfaceStateChange : public UEvent 
{
public:
	UWifiInterfaceStateChange(bool present):UEvent(UCONN_WIFI_INTERFACE_STATE_CHANGE)
	{
		setSize(sizeof(*this));
		this->techId =  WIFI_TECH;
		this->present = present;
	}
	bool present;
};



class UWifiCallbackEvt : public UEvent 
{
public:
	UWifiCallbackEvt(int code):UEvent(UCONN_WIFI_CALLBACK_EVT)
	{
		setSize(sizeof(*this));
		this->code = code;
	}
	int code;
};

class UWifiScanTimeoutEvt : public UEvent 
{
public:
	UWifiScanTimeoutEvt():UEvent(UCONN_WIFI_SCAN_TIMEOUT_EVT)
	{
		setSize(sizeof(*this));
		
	}
	
};





#endif //UWIFI_NM_H_