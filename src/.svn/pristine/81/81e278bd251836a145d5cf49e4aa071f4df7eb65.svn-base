#ifndef _UWLAN_H_
#define _UWLAN_H_

#ifndef UNICODE
#define UNICODE
#endif

/* suppress c_safe warnings for the time being */
//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "uconn_OSAL.h"
#include <wlanapi.h>
#include <objbase.h>
#include <fcntl.h>
#include <Rpc.h>
#include <wtypes.h>
#include <vector>
#include "uconn_Event.h"
#include "UCommonUtils.h"


/* error codes */
typedef enum uwlan_retVal_e 
{
	UWLAN_SUCCESS = 0,
	UWLAN_ERROR,
	UWLAN_INVALID_PARAM,
	UWLAN_NW_NOT_CONNECTABLE,
	UWLAN_NW_NOT_SUPPORTED
} uwlan_retVal_t;

typedef struct PreferredSsid 
{
	string ssid;
	bool available;
public: 
	PreferredSsid()
	{
		ssid.clear();
		available = false;
	}
} PreferredSsid_s;





/*** Util functions  ***/
void displayInterfaceInfo (PWLAN_INTERFACE_INFO pInterface);
void displayInterfaces (PWLAN_INTERFACE_INFO_LIST pIfList);
void printlnWCharString (wchar_t* wCharString);
void printWCharString (wchar_t* wCharString);
char *GetInterfaceStateString (int wlanInterfaceState);
char* GetCipherAlgoString (int dot11CipherAlgo);
char* GetAuthAlgoString (int dot11AuthAlgo);
char* GetBssTypeString (int dot11BssType);
char *GetInterfaceStateString (int wlanInterfaceState);
void displayNetworks (PWLAN_AVAILABLE_NETWORK_LIST pList);
void displayNetwork (PWLAN_AVAILABLE_NETWORK pNetwork);
void printlnSsid (DOT11_SSID ssid);
void printSsid (DOT11_SSID ssid);
char* ssidToString(PDOT11_SSID pSsid);
void printProfileInfo(PWLAN_PROFILE_INFO profile);
void printProfileInfoList (PWLAN_PROFILE_INFO_LIST pProfileList);
wchar_t* stringToWString (const char* src);
void write2File (FILE *pFile, char *str);
void writeTag (FILE *pFile, char *str, char *startOrEnd, 
			   int level);
void writeXmlLn (FILE *pFile, char *tag, 
				 char *str, int level);
int readFile (char** pData, char *file);
void displayStats (PWLAN_STATISTICS pStats, 
				   PWLAN_INTERFACE_INFO pIfInfo);

/* Exported APIs. Should we declare it elsewhere*/
uwlan_retVal_t uwlan_getHandle (const unsigned int cliVersion, unsigned int *pNegVersion,
								HANDLE *pHandle);
uwlan_retVal_t uwlan_listInterfaces (HANDLE handle, PWLAN_INTERFACE_INFO_LIST *ppIfList);
uwlan_retVal_t uwlan_listAvblNetworks (HANDLE handle, PWLAN_INTERFACE_INFO pInterface, 
									   PWLAN_AVAILABLE_NETWORK_LIST *ppNetList);
uwlan_retVal_t uwlan_listAvblProfiles (HANDLE handle, PWLAN_INTERFACE_INFO pInterface,
									   PWLAN_PROFILE_INFO_LIST *ppProfileList);
uwlan_retVal_t uwlan_connect (HANDLE handle, PWLAN_INTERFACE_INFO pInterface,
							WLAN_CONNECTION_PARAMETERS *pConParams);
uwlan_retVal_t uwlan_disconnect (HANDLE handle, PWLAN_INTERFACE_INFO pInterface);
//uwlan_retVal_t  uwlan_createProfile (HANDLE handle, char *ssidStr, PWLAN_AVAILABLE_NETWORK pNet,PWLAN_INTERFACE_INFO pIfInfo,char* shareKey);
uwlan_retVal_t  uwlan_createProfile (HANDLE handle,char *ssidStr,char* shareKey,PWLAN_AVAILABLE_NETWORK pNet,PWLAN_INTERFACE_INFO pIfInfo);
uwlan_retVal_t uwlan_delProfile (HANDLE handle, char *ssidStr, 
								 PWLAN_INTERFACE_INFO pInterface);
uwlan_retVal_t uwlan_registerCB (HANDLE handle, WLAN_NOTIFICATION_CALLBACK  CB);
void uwlan_unregisterCB (HANDLE handle);
uwlan_retVal_t uwlan_getStats (HANDLE handle, void **pStats, PWLAN_INTERFACE_INFO pInterface);

string generateWPAXmlString(char* ssid,char* shareKey,char* authAlg,char* encrypt);



typedef enum WIFI_START_STATUS_E 
{
	WIFI_START_SUCCESS,
	WIFI_START_FAILURE
}WIFI_START_STATUS;

typedef enum WIFI_NETWORK_STATUS_E 
{
	WIFI_NO_INTERFACE_READY = 101,
	WIFI_NO_NETWORK_FOUND,
	WIFI_INTERFACE_SUCCESS,
	WIFI_NETWORK_SUCCESS,
	WIFI_PROFILE_AVAILABLE,
	WIFI_PROFILE_NOT_AVAILABLE,
	WIFI_CONNECT_SUCCESS,
	WIFI_CONNECT_FAILURE,
	WIFI_DISCONNECT_SUCCESS,
	WIFI_DISCONNECT_FAILURE,
	WIFI_PROFILE_NOT_CREATED,
	WIFI_PROFILE_CREATED,
	WIFI_PROFILE_NOT_DELETED,
	WIFI_PROFILE_DELETED,
	WIFI_STATS_ERROR,
	WIFI_STATS_RECEIVED,
	WIFI_REQUEST_PASSWORD,
}WIFI_NETWORK_STATUS;


class Wifi
{
private:
	HANDLE handle;
	PWLAN_INTERFACE_INFO_LIST pIfList;
	PWLAN_AVAILABLE_NETWORK_LIST pNetList;
	PWLAN_AVAILABLE_NETWORK pNetwork;
	PWLAN_PROFILE_INFO_LIST pProfileList;
	WLAN_CONNECTION_PARAMETERS conParams;
	PWLAN_STATISTICS pStats;

	string knownSsid;
	vector<PreferredSsid_s> preferredSsids; //sorted by preference
	static Wifi* wifi;
	Wifi();


	
public:

	string ssidOne; //ssid of first network

	bool isConnected;

	string adapterName;

	string connectedNetworkName;

	string connectedSignalQuality;
	
	
	WIFI_START_STATUS wifiStartup();

	WIFI_START_STATUS startStatus;
	


	~Wifi();

	static Wifi* getWifi();
	static void removeWifi(); // calls destructor

	WIFI_NETWORK_STATUS wifiScanInterface();
	WIFI_NETWORK_STATUS wifiScanNetwork ();
	WIFI_NETWORK_STATUS listAvailableProfiles();
	WIFI_NETWORK_STATUS wifiConnect (char* ssid);
	WIFI_NETWORK_STATUS wifiConnectProfNotPresent(char* ssid, char* password);
	WIFI_NETWORK_STATUS wifiDisconnect();
	WIFI_NETWORK_STATUS wifiDeleteProfile(char * ssidStr);
	string wifiGetStatistics();
	
	//scans interface,networks,lists profiles and stores info in gWifiInfo
	//used by both background and manual scans
	string wifiScanAndStoreAll(); 

	U_VOID updatePreferredList(string& preferredList);

	U_VOID handleInterfaceStateChange(bool interfacePresent);
};



class XmlNode 
{
public:
	XmlNode* parent;
	string name;
	string innerVal;
	string value;
	int level;
	bool valuePresent;
	vector<XmlNode*> children;

	XmlNode(string name,string innerVal,XmlNode* parent,bool valuePresent,string value);
};

class XmlTree 
{
public:
	string xmlString;
	vector<XmlNode*> xmlNodes;

	XmlTree();
	~XmlTree();

	XmlNode* addXmlNode(string name,string innerVal,XmlNode* parent,bool valuePresent,string value);
	XmlNode* createRootNode(string name,string innerVal);
	void genXmlString(XmlNode* root);
};



#endif /*_UWLAN_H_*/