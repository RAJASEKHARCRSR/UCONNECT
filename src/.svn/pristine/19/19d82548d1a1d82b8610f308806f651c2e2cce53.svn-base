/*****************************************************************************
**
**  UWlan.cpp
**
**	This file contains the implementation of the Wlan wrapper functions and 
**	functionality for Windows. This will be part of UConnect.
**	
**	Author:
**	Date  : 26-DEC-2011
**  (c) United Mobile Apps Pvt Ltd - (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		26/12/2011    			Creation
*****************************************************************************/

#include "UWlan.h"
#include "uconn_Server.h"
#include "uconn_Config.h"
#include "uconn_Event.h"
#include "WiFi_NM.h"

#include "UCommonFunctions.h"

#include <sstream>

extern UConnServer *gConnServer;

extern WifiNMInfo gWifiNMInfo;

extern UWiFi_NM* gWiFiNMForCallback;


#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")


/***********************************************************************
*	Gets a unique handle for wifi operations
*	
*	Parameters
*	cliVersion	-	Version supported by the OS
*	pNegVersion -	Version negotiates
*	pHandle		-	Pointer to the Handle 
*	Returns uwlan_retVal_t
***********************************************************************/
uwlan_retVal_t uwlan_getHandle (const unsigned int cliVersion,
								unsigned int *pNegVersion,
								HANDLE *pHandle)
{
	if(WlanOpenHandle(cliVersion, NULL /* reserved */,
						(PDWORD)pNegVersion, pHandle) != ERROR_SUCCESS)
	{
		return UWLAN_ERROR;
	}
	return UWLAN_SUCCESS;
}

/***********************************************************************
*	Get the wlan interfaces on the m/c
*	
*	Parameters
*	handle		-	unique handle
*	ppIflist	-	Interface list that is populated
*	Returns uwlan_retVal_t
***********************************************************************/
uwlan_retVal_t uwlan_listInterfaces (HANDLE handle, 
									 PWLAN_INTERFACE_INFO_LIST *ppIfList)
{
	if(WlanEnumInterfaces(handle, NULL, ppIfList) != ERROR_SUCCESS)
	{
		return UWLAN_ERROR;
	}
	return UWLAN_SUCCESS;
}

/***********************************************************************
*	Get all the available networks in the air
*	
*	Parameters
*	handle		-	unique handle
*	pInterface	-	Pointer to the interface info struct
*	ppNetList	-	Pointer to pointer of the network list
*	Returns uwlan_retVal_t
***********************************************************************/
uwlan_retVal_t uwlan_listAvblNetworks (HANDLE handle,
									   PWLAN_INTERFACE_INFO pInterface, 
									   PWLAN_AVAILABLE_NETWORK_LIST *ppNetList)
{
	if(WlanGetAvailableNetworkList(handle, &pInterface->InterfaceGuid, 
		0, NULL, ppNetList) != ERROR_SUCCESS)
	{
		return UWLAN_ERROR;
	}
	return UWLAN_SUCCESS;
}

/***********************************************************************
*	Get all the saved profiles
*	
*	Parameters
*	handle		-	unique handle
*	pInterface	-	Pointer to the interface info struct
*	ppProfileList	-	Pointer to pointer of the profile list
*	Returns uwlan_retVal_t
***********************************************************************/
uwlan_retVal_t uwlan_listAvblProfiles (HANDLE handle,
									   PWLAN_INTERFACE_INFO pInterface,
									   PWLAN_PROFILE_INFO_LIST *ppProfileList)
{
	if(WlanGetProfileList(handle, &pInterface->InterfaceGuid, 
		NULL, ppProfileList) != ERROR_SUCCESS)
		return UWLAN_ERROR;
	return UWLAN_SUCCESS;
}

/***********************************************************************
*	connect to a network
*	
*	Parameters
*	handle		-	unique handle
*	pInterface	-	Pointer to the interface info struct
*	pConParams	-	Pointer to connection params struct
*	Returns uwlan_retVal_t
***********************************************************************/
uwlan_retVal_t uwlan_connect (HANDLE handle,
							PWLAN_INTERFACE_INFO pInterface,
							WLAN_CONNECTION_PARAMETERS *pConParams)
{
	if(WlanConnect(handle, &pInterface->InterfaceGuid, 
		pConParams, NULL) != ERROR_SUCCESS)
		return UWLAN_ERROR;
	return UWLAN_SUCCESS;
}

/***********************************************************************
*	Disconnects from a n/w
*	
*	Parameters
*	handle		-	unique handle
*	pInterface	-	Pointer to the interface info struct
*	Returns uwlan_retVal_t
***********************************************************************/
uwlan_retVal_t uwlan_disconnect (HANDLE handle,
								 PWLAN_INTERFACE_INFO pInterface)
{
	if(WlanDisconnect(handle, &pInterface->InterfaceGuid, NULL) != 
		ERROR_SUCCESS)
		return UWLAN_ERROR;

	return UWLAN_SUCCESS;
}

/***********************************************************************
*	Attach a profile to an interface
*	
*	Parameters
*	handle		-	unique handle
*	pInterface	-	Pointer to the interface info struct
*	proFileName	-	Name of the profile (usually the SSID of the n/w)
*	Returns uwlan_retVal_t
***********************************************************************/
static uwlan_retVal_t uwlan_attachProfile (HANDLE handle, 
										   PWLAN_INTERFACE_INFO pInterface,
										   char *proFileName)
{
	char* pfileStrm;
	DWORD dwReason;
	wchar_t* bstrXml = NULL;

	if(readFile(&pfileStrm, proFileName)
		== -1)
	{
		printf("Error: can't read the expecified file.\n");
		return UWLAN_ERROR;
	}
	bstrXml = stringToWString(pfileStrm);
	free(pfileStrm);

	{
	DWORD ret; 
	WCHAR errReason[256];

	if((ret = WlanSetProfile(handle, &(pInterface->InterfaceGuid), 0,
			bstrXml, NULL, TRUE, NULL, &dwReason)) != ERROR_SUCCESS)
	{
		free(bstrXml);
		printf("Error setting profile %d\n", ret);
		WlanReasonCodeToString(dwReason, 256, errReason, NULL);
		wprintf(L"Error: %s\n", errReason);
		return UWLAN_ERROR;
	}
	}
	free(bstrXml);
	printf("Success setting profile\n");
	return UWLAN_SUCCESS;
}


/***********************************************************************
*	Create a profile
*	
*	Parameters
*	handle		-	unique handle
*	ssidStr		-	Pointer to the SSID string
*	pNet		-	Pointer to the n/w struc
*	pIfInfo		-	Pointer to the interface struc
*	Returns uwlan_retVal_t
***********************************************************************/

uwlan_retVal_t  uwlan_createProfile (HANDLE handle,
									 char *ssidStr, 
									 char* shareKey,
									 PWLAN_AVAILABLE_NETWORK pNet,
									 PWLAN_INTERFACE_INFO pIfInfo)
{
	/* here we have a network with no profile. read the network
	and create a profile on the fly and attach to the interface.
	needs an xml to be created. the method followed here is very 
	crude, and needs to be changed. REVISIT */
	FILE *pFile;
	//char shareKey[256]; // passed as parameter
	char *encrypt;
	char *authAlg;
	char proFileName[256];
	
	/* is network connectable */
	if(!pNet->bNetworkConnectable)
	{
		printf("Error Network not connectable\n");
		return UWLAN_NW_NOT_CONNECTABLE;
	}
	/* only infrastructure network types supported for now */
	if(pNet->dot11BssType != dot11_BSS_type_infrastructure)
	{
		printf("Error Network Type not infrastructure\n");
		return UWLAN_NW_NOT_SUPPORTED;
	}
#if 0
	/* only wpa2 and wpa psk is supported for now */
	if(pNet->dot11DefaultAuthAlgorithm != 4/*WPA*/
					||
	   pNet->dot11DefaultAuthAlgorithm != 7/*WPA2*/)
	{
		printf("Error Unsopported Auth Algo %d\n", );
		return UWLAN_ERROR;
	}
#endif
	sprintf(proFileName, ".\\%s.xml", ssidStr);
	
	if(!(pFile = fopen(proFileName, "w")))
	{
		printf("Error opening file\n");
		return UWLAN_ERROR;
	}

#if 1
	switch(pNet->dot11DefaultAuthAlgorithm)
	{
		//case 1: authAlg = "Open"; break;
		case 2: authAlg = "Shared"; break;
		//case 3: authAlg = "WPA-Enterprise"; break;
		case 4: authAlg = "WPAPSK"; break;
		case 5: authAlg = "WPANONE"; break;
		//case 6: authAlg = "WPA2-Enterprise"; break;
		case 7: authAlg = "WPA2PSK"; break;
		default: 
			if ((pNet->dot11DefaultAuthAlgorithm & DOT11_AUTH_ALGO_IHV_START) == 0x0) 
					 authAlg = "Vendor-specific algorithm";
	}
#endif
	//writeXmlLn(pFile, "authentication", authAlg, 4);
#if 1
	switch(pNet->dot11DefaultCipherAlgorithm)
	{
		case 0: encrypt = "None"; break;
		case 1: encrypt = "WEP40"; break;
		case 2: encrypt = "TKIP"; break;
		case 4: encrypt = "AES"; break;
		case 5: encrypt = "WEP104"; break;
		//case 256: encrypt = "USE-GROUP"; break;
		case 257: encrypt = "WEP"; break;
		default: 
			if ((pNet->dot11DefaultCipherAlgorithm & DOT11_CIPHER_ALGO_IHV_START) == 0x0) 
				encrypt = "Vendor-specific algorithm";
	}
#endif
	/*
	if(pNet->dot11DefaultCipherAlgorithm != 0)
	{
		printf("\nEnter passpherase: ");
		scanf("%s", shareKey);
	}
	*/
	
	string xmlString = generateWPAXmlString(ssidStr,shareKey,authAlg,encrypt);
	write2File(pFile,(char*)(xmlString.c_str()));
	
	fclose(pFile);
	if(uwlan_attachProfile(handle, pIfInfo, (char*)proFileName) != UWLAN_SUCCESS)
	{
		return UWLAN_ERROR;
	}
	return UWLAN_SUCCESS;
}



/***********************************************************************
*	Checks if a profile is present
*	
*	Parameters
*	ssidStr		-	pointer to the ssid str
*	pProfileList	-	Saved profile list
*	Returns uwlan_retVal_t
***********************************************************************/
uwlan_retVal_t uwlan_isProfilePresent (char *ssidStr, 
									   PWLAN_PROFILE_INFO_LIST pProfileList)
{
	unsigned char tmpStr[256];
	unsigned int i;

	for (i = 0; i < pProfileList->dwNumberOfItems; i++)
	{
		wCharToUCharString((pProfileList->ProfileInfo[i]).strProfileName, 
				tmpStr);
		/* the profile name is the same as SSID str */
		if(!strcmp((const char*)tmpStr, ssidStr))
		{
			/* found */
			return UWLAN_SUCCESS;
		}
	}
	return UWLAN_ERROR;
}


/***********************************************************************
*	Populates the connection parameters in the requires struc
*	
*	Parameters
*	pConParams		-	pointer to connection params struct
*	ssidstr	-	SSID string
*	Returns void
***********************************************************************/
static void populateConParams (WLAN_CONNECTION_PARAMETERS *pConParams, 
							   char *ssidStr)
{
	pConParams->dwFlags = 0; /* always for infrastructure */
	pConParams->pDesiredBssidList = NULL; /* we dont care */
	pConParams->strProfile = stringToWString(ssidStr);
	pConParams->pDot11Ssid = NULL;
	pConParams->dot11BssType = dot11_BSS_type_infrastructure; /* always infrastructure */
	pConParams->wlanConnectionMode = wlan_connection_mode_profile; /* always profile */
}

/***********************************************************************
*	free allocated mem in the conparams struct
*	
*	Parameters
*	pConParams		-	pointer to connection params struct
*	Returns void
***********************************************************************/
static void clearConParams (WLAN_CONNECTION_PARAMETERS *pConParams)
{
	free((void*)pConParams->strProfile);
}

/***********************************************************************
*	delete a saved profile
*	
*	Parameters
*	handle		-	handle
*	ssidStr		-	SSID string
*	pInterface	-	pointer to the interface info
*	Returns uwlan_retVal_t
***********************************************************************/
uwlan_retVal_t uwlan_delProfile (HANDLE handle, 
								 char *ssidStr, 
								 PWLAN_INTERFACE_INFO pInterface)
{
	wchar_t *pTmpStr;
	uwlan_retVal_t retVal;

	pTmpStr = stringToWString((const char*)ssidStr);
	if(WlanDeleteProfile(handle, &pInterface->InterfaceGuid, pTmpStr, NULL) !=
			ERROR_SUCCESS)
	{
		retVal = UWLAN_ERROR;
		goto done;
	}
	retVal = UWLAN_SUCCESS;

done:
	free(pTmpStr);
	return retVal;
}


/***********************************************************************
*	register an interface callback function. to be called whenever a change
*	int the interface occurs
*	
*	Parameters
*	handle		-	handle
*	CB			-	pointer to the callback function
*	Returns uwlan_retVal_t
***********************************************************************/
uwlan_retVal_t uwlan_registerCB (HANDLE handle,
								 WLAN_NOTIFICATION_CALLBACK  CB)
{
	if(WlanRegisterNotification(handle, WLAN_NOTIFICATION_SOURCE_ALL, /* get all the nonsense */
								FALSE, CB, NULL, NULL, NULL) != ERROR_SUCCESS)
				return UWLAN_ERROR;
	return UWLAN_SUCCESS;
}

/***********************************************************************
*	register an interface callback function. to be called whenever a change
*	int the interface occurs
*	
*	Parameters
*	handle		-	handle
*	CB			-	pointer to the callback function
*	Returns uwlan_retVal_t
***********************************************************************/
void WINAPI uwlan_CB (PWLAN_NOTIFICATION_DATA pData, void* pContext)
{
 	if(!pData)
		return;
	switch(pData->NotificationCode)
	{
	case wlan_notification_acm_scan_fail: 
		{
			//keeps coming, don't handle;
		}
		break;
	case wlan_notification_acm_connection_start:
		{
			break;
		}
	case wlan_notification_acm_connection_complete:
		{
			UWifiScanAndStore* scanAndStore = new UWifiScanAndStore();
			gWiFiNMForCallback->MsgPost((U_UINT8*)scanAndStore);

			UBearerStatistics* statsReq = new UBearerStatistics(WIFI_TECH);
			gWiFiNMForCallback->MsgPost((U_UINT8*)statsReq);

			LOG(ULOG_DEBUG_LEVEL,"Wifi connection complete callback received- > exit");
			
			break;
		}
	case wlan_notification_acm_background_scan_enabled:
		break;
	case wlan_notification_acm_background_scan_disabled:
		break;
	case wlan_notification_acm_power_setting_change:
		break;
	case wlan_notification_acm_scan_complete: 
		{
			UWifiScanAndStore* scanAndStore = new UWifiScanAndStore();
			gWiFiNMForCallback->MsgPost((U_UINT8*)scanAndStore);
		}		
		break;
	case wlan_notification_acm_connection_attempt_fail: 
		break;
	case wlan_notification_acm_interface_arrival: 
		{
			Wifi::getWifi()->handleInterfaceStateChange(true);
		}
		break;
	case wlan_notification_acm_interface_removal: 
		{
			Wifi::getWifi()->handleInterfaceStateChange(false);
		}
		break;
	case wlan_notification_acm_profile_change: 
		break;
	case wlan_notification_acm_profile_name_change: 
		break;
	case wlan_notification_acm_network_not_available: 
		break;
	case wlan_notification_acm_network_available: 
		break;
	case wlan_notification_acm_disconnecting:
		break;
	case wlan_notification_acm_disconnected:
		{
			ENMStateForBM newState = NM_NOT_READY_STATE;
			gWiFiNMForCallback->checkStateAndPostToBM(newState);

			UWifiScanAndStore* scanAndStore = new UWifiScanAndStore();
			gWiFiNMForCallback->MsgPost((U_UINT8*)scanAndStore);

			LOG(ULOG_DEBUG_LEVEL,"Wifi Disconnected callback received - > exit");

		}
		break;
	default:
		break;

	}
}

/***********************************************************************
*	unregister an interface callback function
*	
*	Parameters
*	handle		-	handle
*	Returns void
***********************************************************************/
void uwlan_unregisterCB (HANDLE handle)
{
	WlanRegisterNotification(handle, WLAN_NOTIFICATION_SOURCE_NONE, 
		FALSE, NULL, NULL, NULL, NULL);
}

/***********************************************************************
*	get statistics
*	
*	Parameters
*	handle		-	handle
*	pStats		-	Pointer to stats, allocated in func, freed by caller
*	pInterface	-	pointer to the interface info structure
*	Returns uwlan_retVal_t
***********************************************************************/
uwlan_retVal_t uwlan_getStats (HANDLE handle, void **pStats,
							   PWLAN_INTERFACE_INFO pInterface)
{
	unsigned long statsSz = 0;

	if(WlanQueryInterface(handle, &pInterface->InterfaceGuid,
		wlan_intf_opcode_statistics, NULL, &statsSz, pStats, NULL) != ERROR_SUCCESS)
	{
		return UWLAN_ERROR;
	}
	return UWLAN_SUCCESS;
}

/***********************************************************************
*	gets a network from the available n/ws
*	
*	Parameters
*	ssidStr		- ssid
*	ppNet		- Poiter to the pointer of the Network struc
*	pNetList	- Pointer to Network list
*	Returns void
***********************************************************************/
static uwlan_retVal_t uwlan_getNetwork (char *ssidStr, 
										PWLAN_AVAILABLE_NETWORK *ppNet,
										PWLAN_AVAILABLE_NETWORK_LIST pNetList)
{
	unsigned int i;
	char *tmpStr;

	for(i = 0; i < pNetList->dwNumberOfItems; i++)
	{
		tmpStr = ssidToString(&(pNetList->Network[i].dot11Ssid));
		if(!strcmp(tmpStr, ssidStr))
		{
			/* found */
			*ppNet = &pNetList->Network[i];
			
			free(tmpStr);
			return UWLAN_SUCCESS;
		}
		free(tmpStr);
	}
	*ppNet = NULL;
	return UWLAN_ERROR;
}

Wifi::Wifi()
{
	HANDLE handle = NULL;
	PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
	PWLAN_AVAILABLE_NETWORK_LIST pNetList = NULL;
	PWLAN_AVAILABLE_NETWORK pNetwork = NULL;
	PWLAN_STATISTICS pStats = NULL;
	

	isConnected = false;
	knownSsid = "";
	adapterName = "";
	connectedSignalQuality = "";
}

Wifi* Wifi::wifi = NULL;

Wifi* Wifi::getWifi()
{
	if(Wifi::wifi == NULL)
	{
		Wifi::wifi = new Wifi();
		wifi->startStatus = wifi->wifiStartup();
	}
	return Wifi::wifi;
}

void Wifi::removeWifi()
{
	if(Wifi::wifi != NULL)
	{
		delete wifi;
	}
	Wifi::wifi = NULL;


}

Wifi::~Wifi()
{
		if(pIfList)
		WlanFreeMemory(pIfList);
		if(pProfileList)
		WlanFreeMemory(pProfileList);
		if(pNetList)
		WlanFreeMemory(pNetList);
		uwlan_unregisterCB(handle);
		WlanCloseHandle(handle, NULL);
}
WIFI_START_STATUS Wifi::wifiStartup()
{
	handle = NULL;
	const unsigned int cliVersion = 2; /* the highest version currently supported */
	unsigned int negCliVersion;

	/* open a client handle used for all subsequent operations  */
	if(uwlan_getHandle(cliVersion, &negCliVersion, &handle) != UWLAN_SUCCESS)
	{
		printf("Unable to get handle\n");
		return WIFI_START_FAILURE;
	}

	/* register a wlan callback. move this to the init function */
	if(uwlan_registerCB(handle, uwlan_CB) != UWLAN_SUCCESS)
	{
		/* nothing much to do here */
		printf("Error registering CB\n");
		return WIFI_START_FAILURE;
	}
	return WIFI_START_SUCCESS;
}

WIFI_NETWORK_STATUS Wifi::wifiScanInterface()
{
	
	/* get the interfaces that support wlan */
	if(uwlan_listInterfaces(handle, &pIfList) != UWLAN_SUCCESS)
	{
		this->adapterName = "";
		return WIFI_NO_INTERFACE_READY;
	}
	unsigned int i;

	if(!pIfList) 
		return WIFI_NO_INTERFACE_READY;
	
	printf("There are %lu interfaces in the system.\n", pIfList->dwNumberOfItems);

	/* print out interface information */
	bool readyInterfaceFound = false;

	for(i = 0; i < pIfList->dwNumberOfItems; i++)
	{
		U_UINT32 notReady = (U_UINT32)wlan_interface_state_not_ready;
		if(&pIfList->InterfaceInfo[i].isState != 0) //check this
		{
			readyInterfaceFound = true;
			char adapterName[WLAN_MAX_NAME_LENGTH];
			wcstombs(adapterName,(const wchar_t*)&pIfList->InterfaceInfo[i].strInterfaceDescription,WLAN_MAX_NAME_LENGTH);
			Wifi::getWifi()->adapterName = adapterName;

			break;
		}
		
	}
	if(!readyInterfaceFound)
	{
		this->adapterName = "";
		return WIFI_NO_INTERFACE_READY;
	}

	return WIFI_INTERFACE_SUCCESS;

}

WIFI_NETWORK_STATUS Wifi::wifiScanNetwork()
{
	//delete all the previously stored networks
	if(gWifiNMInfo.networksAvailable.networkList.size())
	{
		gWifiNMInfo.networksAvailable.networkList.clear();
	}

	/* get the networks available on the interfaces */
	if(uwlan_listAvblNetworks(handle, &pIfList->InterfaceInfo[0], &pNetList) != UWLAN_SUCCESS)
	{
		printf("Unable to list networks\n");
		return WIFI_NO_NETWORK_FOUND;
	}

	/* display the netowrks and prompt to connect */
	displayNetworks(pNetList);
	return WIFI_NETWORK_SUCCESS;

}

WIFI_NETWORK_STATUS Wifi::listAvailableProfiles()
{
	/* list the saved profiles */
	if(uwlan_listAvblProfiles(handle, &pIfList->InterfaceInfo[0], 
		&pProfileList) != UWLAN_SUCCESS)
	{
		printf("Unable to list profiles\n");
		return WIFI_PROFILE_NOT_AVAILABLE;
	}
	
	/* display the profiles */
	printProfileInfoList(pProfileList);
	return WIFI_PROFILE_AVAILABLE;
	
}

WIFI_NETWORK_STATUS Wifi::wifiConnect(char* ssidStr)
{

	HANDLE handle = NULL;
	unsigned int cliVersion = 2;
	unsigned int negCliVersion;

	if(!strlen(ssidStr))
	{
		strncpy(ssidStr,knownSsid.c_str(),knownSsid.size());
		ssidStr[knownSsid.size()] = '\0';
	}


	if(uwlan_getHandle(cliVersion, &negCliVersion, &handle) != UWLAN_SUCCESS)
	{
		printf("Unable to get handle\n");
		return WIFI_CONNECT_FAILURE;
	}

	if(pProfileList) WlanFreeMemory(pProfileList);
	uwlan_listAvblProfiles(handle, &pIfList->InterfaceInfo[0], 
			&pProfileList);

	if(uwlan_isProfilePresent(ssidStr, pProfileList) == UWLAN_SUCCESS)
	{
	
		populateConParams(&conParams, ssidStr);
		/* connect */
		if(uwlan_connect(handle, &pIfList->InterfaceInfo[0], &conParams)
			!= UWLAN_SUCCESS)
		{
			printf("Error connecting\n");
			clearConParams(&conParams);
			WlanCloseHandle(handle, NULL);

			return WIFI_CONNECT_FAILURE;
		}
		WlanCloseHandle(handle, NULL);

		return WIFI_CONNECT_SUCCESS;

	}
	else
	{
		
		int x = gWifiNMInfo.networksAvailable.networkList.size();
		for(unsigned int i=0;i<gWifiNMInfo.networksAvailable.networkList.size();i++)
		{
			if(!strcmp(gWifiNMInfo.networksAvailable.networkList[i].ssid,ssidStr))
			{
				if(!gWifiNMInfo.networksAvailable.networkList[i].securityEnabled)
				{
					if(uwlan_connect(handle, &pIfList->InterfaceInfo[0], &conParams)
						== UWLAN_SUCCESS)
						return WIFI_CONNECT_SUCCESS;
					else
						return WIFI_CONNECT_FAILURE;
						
				}
				
			}
		}
		
	
		WlanCloseHandle(handle, NULL);
		return WIFI_REQUEST_PASSWORD;
	}
	
}


WIFI_NETWORK_STATUS Wifi::wifiDisconnect()
{
	/* Assumption is that there is only one wifi interface */
	if(uwlan_disconnect(handle, &pIfList->InterfaceInfo[0]) != UWLAN_SUCCESS)
	{
		printf("Error Disconnecting\n");
		return WIFI_DISCONNECT_FAILURE;
	}
	
	return WIFI_DISCONNECT_SUCCESS;

} 
WIFI_NETWORK_STATUS Wifi::wifiDeleteProfile( char * ssidStr)
{
	if(uwlan_isProfilePresent(ssidStr, pProfileList) != UWLAN_SUCCESS)
	{
		printf("Error finding profile %s\n", ssidStr);
		return WIFI_PROFILE_NOT_AVAILABLE;
	}
	else
	{
		if(uwlan_delProfile(handle, ssidStr, &pIfList->InterfaceInfo[0])
			!= UWLAN_SUCCESS)
		{
			printf("Error deleting profile\n");
			return WIFI_PROFILE_NOT_DELETED;
		}
	}
	return WIFI_PROFILE_DELETED;
}
string Wifi::wifiGetStatistics()
{
	DWORD dwResult = 0;

	PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
    DWORD connectInfoSize = sizeof(WLAN_CONNECTION_ATTRIBUTES);
    WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;

	PWLAN_INTERFACE_INFO pIfInfo = &(pIfList->InterfaceInfo[0]);
	
	if (pIfInfo->isState == wlan_interface_state_connected) {

	dwResult = WlanQueryInterface(handle,
                                  &pIfInfo->InterfaceGuid,
                                  wlan_intf_opcode_current_connection,
                                  NULL,
                                  &connectInfoSize,
                                  (PVOID *) &pConnectInfo, 
                                  &opCode);

	}

	if(dwResult != ERROR_SUCCESS || !pConnectInfo )
	{
		return "";
	}
	
	long ulTxRate = pConnectInfo->wlanAssociationAttributes.ulTxRate;
	ulTxRate /= 1000;
	long ulRxRate = pConnectInfo->wlanAssociationAttributes.ulRxRate;
	ulRxRate /= 1000;

	string encodeStr = "";
	char txStr[10];
	char rxStr[10];
	ITOA(ulTxRate,txStr,10);
	ITOA(ulRxRate,rxStr,10);

	encodeStr = "ssid;";
	encodeStr += knownSsid;
	encodeStr += "\n";

	encodeStr += "TxRate;";
	encodeStr += txStr;
	encodeStr += " Mbps";
	encodeStr += "\n";
	
	encodeStr += "RxRate;" ;
	encodeStr += rxStr;
	encodeStr += " Mbps";
	
	return encodeStr;
}

WIFI_NETWORK_STATUS Wifi::wifiConnectProfNotPresent(char* ssidStr, char* password)
{

	HANDLE handle = NULL;
	unsigned int cliVersion = 2;
	unsigned int negCliVersion;
	if(uwlan_getHandle(cliVersion, &negCliVersion, &handle) != UWLAN_SUCCESS)
	{
		printf("Unable to get handle\n");
		return WIFI_CONNECT_FAILURE;
	}

		printf("profile not present\n");
		/* refresh the network list */
		if(pNetList) WlanFreeMemory(pNetList);
		uwlan_listAvblNetworks(handle, &pIfList->InterfaceInfo[0], &pNetList);
		/* is there a network with that ssid */
		if(uwlan_getNetwork(ssidStr, &pNetwork, pNetList) != UWLAN_SUCCESS)
		{
			printf("Error finding network\n");
			WlanCloseHandle(handle, NULL);

			
			return WIFI_NO_NETWORK_FOUND;
		}
		else
		{
			/* get the network info and create a profile */
			if(uwlan_createProfile(handle, ssidStr,password,pNetwork, 
				&pIfList->InterfaceInfo[0]) != UWLAN_SUCCESS)
			{
				printf("Error creating profile\n");
				WlanCloseHandle(handle, NULL);

				return WIFI_PROFILE_NOT_CREATED;
			}
			populateConParams(&conParams, ssidStr);
			//uwlan_createProfile(pNetwork);
			if(uwlan_connect(handle, &pIfList->InterfaceInfo[0], &conParams)
			!= UWLAN_SUCCESS)
			{
				printf("Error connecting\n");
				clearConParams(&conParams);
				
				WlanCloseHandle(handle, NULL);

				return WIFI_CONNECT_FAILURE;
			}
		}
		

		WlanCloseHandle(handle, NULL);


		return WIFI_CONNECT_SUCCESS;
	}


string Wifi::wifiScanAndStoreAll()
{
		LOG(ULOG_DEBUG_LEVEL,"Wifi Scan And Store All - > entry");
		
		ENMStateForBM newState = gWiFiNMForCallback->state;


		string encodeStr = "";

		vector<string> knownSsids;
		bool knownNetworkAvailable = false;
		wifiScanInterface();
		
		
		if(wifiScanNetwork() == WIFI_NO_NETWORK_FOUND)
		{
			newState = NM_NOT_READY_STATE;
			gWiFiNMForCallback->checkStateAndPostToBM(newState);
			return encodeStr;
		}
		
		
		bool localIsConnected = false;

		for(unsigned int i=0;i<gWifiNMInfo.networksAvailable.networkList.size();i++)
		{
			if(i == 0)
			{
				ssidOne = gWifiNMInfo.networksAvailable.networkList[i].ssid;
			}
			bool duplicate=false;
			for(unsigned int j=0;j<i && j!=i;j++)
			{
				if(!strcmp(gWifiNMInfo.networksAvailable.networkList[i].ssid,
				          gWifiNMInfo.networksAvailable.networkList[j].ssid))
				{
					duplicate=true;
					break;
				}


			}
			if(duplicate)
				continue;

			encodeStr += "SSID:";
			if(strlen(gWifiNMInfo.networksAvailable.networkList[i].ssid) < 2)
			{
				encodeStr +="Unknown Network";
			}
			else
			{
				encodeStr += gWifiNMInfo.networksAvailable.networkList[i].ssid;
			}
			encodeStr += ";";
			encodeStr += "SigQuality:";
			string sigQual = "";
			
			while(gWifiNMInfo.networksAvailable.networkList[i].signalQuality >0)
			{
				char digit[2];
				digit[0] = (char)(gWifiNMInfo.networksAvailable.networkList[i].signalQuality%10+48);
				digit[1] = '\0';
				sigQual += digit;
				gWifiNMInfo.networksAvailable.networkList[i].signalQuality/=10;
				
			}
			
			
			encodeStr += _strrev((char*)sigQual.c_str());
			encodeStr += ";";
			encodeStr += "securityEnabled:";
			if(gWifiNMInfo.networksAvailable.networkList[i].securityEnabled == true)
			{
				encodeStr += "Yes;";
			}
			else
			{
				encodeStr += "No;";
			}
			encodeStr += "profileAvailable:";
			if(gWifiNMInfo.networksAvailable.networkList[i].profileAvailable == true)
			{
				encodeStr += "Yes;";
				knownNetworkAvailable = true;
				knownSsids.push_back(gWifiNMInfo.networksAvailable.networkList[i].ssid);
			}
			else
			{
				encodeStr += "No;";
			}
			encodeStr += "connected:";
			//cout <<gWifiNMInfo.networksAvailable.networkList[i].noOfBssids<<endl;
			if(gWifiNMInfo.networksAvailable.networkList[i].connected == true)
			{
				localIsConnected = true;	
				encodeStr += "Yes;";
				this->isConnected = true;
				newState = NM_CONNECTED_STATE;
				this->connectedNetworkName = gWifiNMInfo.networksAvailable.networkList[i].ssid;
				U_UINT32 sigQual = gWifiNMInfo.networksAvailable.networkList[i].signalQuality;
				char sigQualChar[10];
				sprintf(sigQualChar,"%d",sigQual);
				this->connectedSignalQuality = sigQual;
			}
			else
			{
				encodeStr += "No;";
			}
			encodeStr += "cipherAlgo:";
			encodeStr += gWifiNMInfo.networksAvailable.networkList[i].cipherAlgo;
			encodeStr += ";";
			encodeStr += "authAlgo:";
			encodeStr += gWifiNMInfo.networksAvailable.networkList[i].authAlgo;
			encodeStr += ";";
			encodeStr += "\n";
		}
		

	if(!localIsConnected)
	{
		this->isConnected = false;
		newState = NM_READY_STATE;
	}

	else
	{
		newState = NM_CONNECTED_STATE;
	}

	
		//postEvent to bearer manager only if there's a state change.
		//also post event to gui if disconnected

	   gWiFiNMForCallback->checkStateAndPostToBM(newState);

		if(gWiFiNMForCallback->state == NM_CONNECTED_STATE)
		{
			UBearerStatistics* statsReq = new UBearerStatistics(WIFI_TECH);
			gWiFiNMForCallback->MsgPost((U_UINT8*)statsReq);	
		}

		
	


	if(!knownNetworkAvailable) {
		this->knownSsid = "";
	}

	//Assume previous preferred ssids aren't available until checked below

	for(unsigned int i=0;i<preferredSsids.size();i++)
	{
		preferredSsids[i].available = false;
	}
	

	for(unsigned int i=0;i<knownSsids.size();i++)
	{
		bool stored = false;
		for(unsigned int j=0;j<preferredSsids.size();j++)
		{
			if(!strcmp(knownSsids[i].c_str(),preferredSsids[j].ssid.c_str()))
			{
				stored = true;
				preferredSsids[j].available = true;
				break;
			}
		}
		if(!stored)
		{
			PreferredSsid_s preferred;
			preferred.ssid = knownSsids[i];
			preferred.available = true;
			preferredSsids.push_back(preferred);
		}

	}
	vector<PreferredSsid_s> updatedPreferredSsids;

	for(unsigned int i=0;i<preferredSsids.size();i++)
	{
		if(preferredSsids[i].available)
		{
			updatedPreferredSsids.push_back(preferredSsids[i]);
		}
	}
	preferredSsids = updatedPreferredSsids;

	if(preferredSsids.size())
	{
		this->knownSsid = preferredSsids[0].ssid; //most preferred ssid for connection
	}


	string encodedPreferredSsids = "";
	for(unsigned int i=0;i<preferredSsids.size();i++)
	{
		encodedPreferredSsids += preferredSsids[i].ssid;
		encodedPreferredSsids += "\n";
	}
	UWifiPreferredNetworksEvt* prefEvt = new UWifiPreferredNetworksEvt(encodedPreferredSsids);
	gWiFiNMForCallback->postEventToBearerManager((UEvent*)prefEvt);

	LOG(ULOG_DEBUG_LEVEL,"Wifi Scan And Store All - > exit");

	return encodeStr;

}

U_VOID Wifi::updatePreferredList(string& preferredList)
{
	vector<string> preferredSsids;
	tokeniseInformation(preferredList,preferredSsids,'\n');
	vector<PreferredSsid_s> updatedPreferred;
	for(unsigned int i=0;i<preferredSsids.size();i++)
	{
		if(preferredSsids[i].size() < 2)
			continue;

		PreferredSsid_s preferredSsid;
		preferredSsid.ssid = preferredSsids[i];
		preferredSsid.available = true;
		updatedPreferred.push_back(preferredSsid);
	}
	this->preferredSsids = updatedPreferred;
	if(this->preferredSsids.size())
	{
		this->knownSsid = this->preferredSsids[0].ssid;
	}

	

}

U_VOID Wifi::handleInterfaceStateChange(bool present)
{

	UWifiInterfaceStateChange* stateChange = new UWifiInterfaceStateChange(present);
	gWiFiNMForCallback->postToWiFiNM_Queue(reinterpret_cast<UEvent*>(stateChange));
	
}


// Windows XML Profile creation functions

string generateWPAXmlString(char* ssid,char* shareKey,char* authAlgo,char* encrypt)
{
	XmlTree* xmlTree = new XmlTree();
	xmlTree->xmlString += "<?xml version=\"1.0\" encoding=\"US-ASCII\"?>\n";
	//XmlNode* root = xmlTree->addXmlNode("WLANProfile","xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\"",NULL,false,NULL);
	XmlNode* root = xmlTree->createRootNode("WLANProfile","xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\"");
	string ssidStr(ssid);
	XmlNode* name = xmlTree->addXmlNode("name","",root,true,ssidStr);
	XmlNode* ssidConfig = xmlTree->addXmlNode("SSIDConfig","",root,false,"");
	XmlNode* ssidNode = xmlTree->addXmlNode("SSID","",ssidConfig,false,"");
	ssidNode->children.push_back(name); // reusing the name node
	xmlTree->addXmlNode("connectionType","",root,true,"ESS");
	xmlTree->addXmlNode("connectionMode","",root,true,"auto");
	xmlTree->addXmlNode("autoSwitch","",root,true,"false");
	XmlNode* msm = xmlTree->addXmlNode("MSM","",root,false,"");
	XmlNode* security = xmlTree->addXmlNode("security","",msm,false,"");
	XmlNode* authEncryption = xmlTree->addXmlNode("authEncryption","",security,false,"");

	//add appropriate authentication node
	string authAlgoStr(authAlgo);
	XmlNode* authentication = xmlTree->addXmlNode("authentication","",authEncryption,true,authAlgo);
	//add encryption node
	string encryptStr(encrypt);
	XmlNode* encryptNode = xmlTree->addXmlNode("encryption","",authEncryption,true,encryptStr);

	
	XmlNode* useOneX = xmlTree->addXmlNode("useOneX","",authEncryption,true,"false");

	XmlNode* sharedKey = xmlTree->addXmlNode("sharedKey","",security,false,"");
	XmlNode* keyType = xmlTree->addXmlNode("keyType","",sharedKey,true,"passPhrase");
	XmlNode* protect = xmlTree->addXmlNode("protected","",sharedKey,true,"false");
	string shareKeyStr(shareKey);
	XmlNode* keyMaterial = xmlTree->addXmlNode("keyMaterial","",sharedKey,true,shareKeyStr);
	xmlTree->genXmlString(root);

	int x = 0;
	return xmlTree->xmlString;

}



string generateWPATLSXmlString(char* ssid,char* shareKey,char* authAlgo,char* encrypt)
{
	XmlTree* xmlTree = new XmlTree();
	xmlTree->xmlString += "<?xml version=\"1.0\" encoding=\"US-ASCII\"?>\n";
	
	XmlNode* root = xmlTree->createRootNode("WLANProfile","xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\"");
	string ssidStr(ssid);
	XmlNode* name = xmlTree->addXmlNode("name","",root,true,ssidStr);
	XmlNode* ssidConfig = xmlTree->addXmlNode("SSIDConfig","",root,false,"");
	XmlNode* ssidNode = xmlTree->addXmlNode("SSID","",ssidConfig,false,"");
	ssidNode->children.push_back(name); // reusing the name node
	xmlTree->addXmlNode("connectionType","",root,true,"ESS");
	xmlTree->addXmlNode("connectionMode","",root,true,"auto");
	xmlTree->addXmlNode("autoSwitch","",root,true,"false");
	XmlNode* msm = xmlTree->addXmlNode("MSM","",root,false,"");
	XmlNode* security = xmlTree->addXmlNode("security","",msm,false,"");
	XmlNode* authEncryption = xmlTree->addXmlNode("authEncryption","",security,false,"");

	//add appropriate authentication node
	string authAlgoStr(authAlgo);
	XmlNode* authentication = xmlTree->addXmlNode("authentication","",authEncryption,true,"WPA2");
	//add encryption node
	string encryptStr(encrypt);
	XmlNode* encryptNode = xmlTree->addXmlNode("encryption","",authEncryption,true,encryptStr);

	
	XmlNode* useOneX = xmlTree->addXmlNode("useOneX","",authEncryption,true,"true");

	XmlNode* oneX = xmlTree->addXmlNode("OneX","xmlns=\"http://www.microsoft.com/networking/OneX/v1\"",security,false,"");
	XmlNode* eapConfig = xmlTree->addXmlNode("EAPConfig","",oneX,false,"");

	std::stringstream ss1;
	ss1 << "xmlns=\"http://www.microsoft.com/provisioning/EapHostConfig\"\n\t\t" 
	   << "xmlns=\"http://www.microsoft.com/provisioning/EapHostConfig\"\n\t\t"
	   << "xmlns:baseEap=\"http://www.microsoft.com/provisioning/BaseEapMethodConfig\"";

	string eapHostConfigStr = ss1.str();

	XmlNode* eapHostConfig = xmlTree->addXmlNode("EAPConfig",eapHostConfigStr,eapConfig,false,"");

	XmlNode* eapMethod = xmlTree->addXmlNode("EapMethod","",eapHostConfig,false,"");

	XmlNode* eapCommonType = xmlTree->addXmlNode("eapCommon:Type","",eapMethod,true,"13");

	XmlNode* eapCommonAuthorId = xmlTree->addXmlNode("eapCommon:AuthorId","",eapMethod,true,"0");


	std::stringstream ss2;

	ss2 << "xmlns:baseEap=\"http://www.microsoft.com/provisioning/BaseEapConnectionPropertiesV1\"\n\t\t" 
		<< "xmlns:eapTls=\"http://www.microsoft.com/provisioning/EapTlsConnectionPropertiesV1\"";

	string configString = ss2.str();

	XmlNode* configNode = xmlTree->addXmlNode("Config",configString,eapHostConfig,false,"");

	XmlNode* baseEap = xmlTree->addXmlNode("baseEap:Eap","",configNode,false,"");

	XmlNode* baseEapType = xmlTree->addXmlNode("baseEap:Type","",baseEap,true,"13");

	XmlNode* eapTlsType = xmlTree->addXmlNode("eapTls:EapType","",baseEap,false,"");

	XmlNode* eapTlsSource = xmlTree->addXmlNode("eapTls:CredentialsSource","",eapTlsType,true,"\n<eapTls:CertificateStore />");
	
	string eapTlsServerPrompt = "eapTls:DisableUserPromptForServerValidation>";
	string eapTlsServerValidStr = "\n<" + eapTlsServerPrompt + "false"+ "</"+eapTlsServerPrompt + "\n" + " \n<eapTls:ServerNames />\n";
	
	XmlNode* eapTlsServerValid = xmlTree->addXmlNode("eapTls:ServerValidation","",eapTlsType,true,eapTlsServerValidStr);

	XmlNode* eapTlsUserName = xmlTree->addXmlNode("eapTls:DifferentUsername","",eapTlsType,true,"false");

	xmlTree->genXmlString(root);

	int x = 0;
	return xmlTree->xmlString;


}


string generateWPAPeapXmlString(char* ssid,char* shareKey,char* authAlgo,char* encrypt)
{
	XmlTree* xmlTree = new XmlTree();
	xmlTree->xmlString += "<?xml version=\"1.0\" encoding=\"US-ASCII\"?>\n";
	
	XmlNode* root = xmlTree->createRootNode("WLANProfile","xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\"");
	string ssidStr(ssid);
	XmlNode* name = xmlTree->addXmlNode("name","",root,true,ssidStr);
	XmlNode* ssidConfig = xmlTree->addXmlNode("SSIDConfig","",root,false,"");
	XmlNode* ssidNode = xmlTree->addXmlNode("SSID","",ssidConfig,false,"");
	ssidNode->children.push_back(name); // reusing the name node
	xmlTree->addXmlNode("connectionType","",root,true,"ESS");
	xmlTree->addXmlNode("connectionMode","",root,true,"auto");
	xmlTree->addXmlNode("autoSwitch","",root,true,"false");
	XmlNode* msm = xmlTree->addXmlNode("MSM","",root,false,"");
	XmlNode* security = xmlTree->addXmlNode("security","",msm,false,"");
	XmlNode* authEncryption = xmlTree->addXmlNode("authEncryption","",security,false,"");

	//add appropriate authentication node
	string authAlgoStr(authAlgo);
	XmlNode* authentication = xmlTree->addXmlNode("authentication","",authEncryption,true,"WPA2");
	//add encryption node
	string encryptStr(encrypt);
	XmlNode* encryptNode = xmlTree->addXmlNode("encryption","",authEncryption,true,encryptStr);

	
	XmlNode* useOneX = xmlTree->addXmlNode("useOneX","",authEncryption,true,"true");

	XmlNode* oneX = xmlTree->addXmlNode("OneX","xmlns=\"http://www.microsoft.com/networking/OneX/v1\"",security,false,"");
	XmlNode* eapConfig = xmlTree->addXmlNode("EAPConfig","",oneX,false,"");

	std::stringstream ss1;

	ss1 << "xmlns=\"http://www.microsoft.com/provisioning/EapHostConfig\"\n\t\t"
		<< "xmlns:eapCommon=\"http://www.microsoft.com/provisioning/EapCommon\"\n\t\t"
		<< "xmlns:baseEap=\"http://www.microsoft.com/provisioning/BaseEapMethodConfig\"";

	string eapHostConfigStr = ss1.str();

	XmlNode* eapHostConfig = xmlTree->addXmlNode("EAPConfig",eapHostConfigStr,eapConfig,false,"");

	XmlNode* eapMethod = xmlTree->addXmlNode("EapMethod","",eapHostConfig,false,"");

	XmlNode* eapCommonType = xmlTree->addXmlNode("eapCommon:Type","",eapMethod,true,"25");

	XmlNode* eapCommonAuthorId = xmlTree->addXmlNode("eapCommon:AuthorId","",eapMethod,true,"0");

	std::stringstream ss2;

	ss2 << "xmlns:baseEap=\"http://www.microsoft.com/provisioning/BaseEapConnectionPropertiesV1\"\n\t\t"
		<< "xmlns:msPeap=\"http://www.microsoft.com/provisioning/MsPeapConnectionPropertiesV1\"\n\t\t"
		<< "xmlns:msChapV2=\"http://www.microsoft.com/provisioning/MsChapV2ConnectionPropertiesV1\"";
	
	string configString = ss2.str();

	XmlNode* configNode = xmlTree->addXmlNode("Config",configString,eapHostConfig,false,"");

	XmlNode* baseEap = xmlTree->addXmlNode("baseEap:Eap","",configNode,false,"");

	XmlNode* baseEapType = xmlTree->addXmlNode("baseEap:Type","",baseEap,true,"25");

	XmlNode* msPeap = xmlTree->addXmlNode("msPeap:EapType","",baseEap,false,"");

	string msPeapServerPrompt = "msPeap:DisableUserPromptForServerValidation>";

	string msPeapServerValidStr = "\n<" + msPeapServerPrompt + "false"+ "</" + msPeapServerPrompt + "\n" + " \n<msPeap:TrustedRootCA />\n";

	XmlNode* msPeapServerValid = xmlTree->addXmlNode("msPeap:ServerValidation","",msPeap,true,msPeapServerValidStr);

	XmlNode* msPeapFastReconnect = xmlTree->addXmlNode("msPeap:FastReconnect","",msPeap,true,"true");

	XmlNode* msPeapInnerEap = xmlTree->addXmlNode("msPeap:InnerEapOptional","",msPeap,true,"0");

	XmlNode* baseEap2 = xmlTree->addXmlNode("baseEap:Eap","",msPeap,false,"");

	XmlNode* baseEapType2 = xmlTree->addXmlNode("baseEap:Type","",baseEap2,true,"26");

	XmlNode* msChapV2 = xmlTree->addXmlNode("msChapV2:EapType","",baseEap2,false,"");

	XmlNode* msChapV2Cred = xmlTree->addXmlNode("msChapV2:UseWinLogonCredentials","",msChapV2,true,"false");

	XmlNode* msPeapEnableQuar = xmlTree->addXmlNode("msPeap:EnableQuarantineChecks","",msPeap,true,"false");

	XmlNode* msPeapReqBind = xmlTree->addXmlNode("msPeap:RequireCryptoBinding","",msPeap,true,"false");
	
	

	//add <msPeap:PeapExtensions />




	

	xmlTree->genXmlString(root);

	int x = 0;
	return xmlTree->xmlString;


}



XmlNode::XmlNode(string name,string innerVal,XmlNode* parent,bool valuePresent,string value)
{
	this->name = name;
	this->innerVal = innerVal;
	this->parent = parent;
	this->valuePresent = valuePresent;
	this->value = value;
	if(parent == NULL)
	{
		this->level = 0;
	}

	else
	{
		this->level = this->parent->level + 1;
		this->parent->children.push_back(this);
	}
}

XmlTree::XmlTree()
{
	xmlString = "";
}

XmlTree::~XmlTree()
{
	if(xmlNodes.size())
	{
		for(unsigned int i=0;i<xmlNodes.size();i++)
		{
			delete xmlNodes[i];
		}
		xmlNodes.clear();
	}
}

XmlNode* XmlTree::addXmlNode(string name,string innerVal,XmlNode* parent,bool valuePresent,string value)
{
	XmlNode* xmlNode = new XmlNode(name,innerVal,parent,valuePresent,value);
	this->xmlNodes.push_back(xmlNode);
	return xmlNode;
}
XmlNode* XmlTree::createRootNode(string name,string innerVal)
{
	XmlNode* xmlRootNode = new XmlNode(name,innerVal,NULL,false,"");
	this->xmlNodes.push_back(xmlRootNode);
	return xmlRootNode;
}

void XmlTree::genXmlString(XmlNode* root)
{
	//add indentation
	if(root->valuePresent)
	{
		int level = root->level;
		while(level)
		{
			xmlString += "   ";
			level--;
		}
	}

	if(root->innerVal.length()>1)
	{
		xmlString +="<"+ root->name + " "+ root->innerVal + ">";//add innerValue only if we have an attribute
	}
	else
	{
		xmlString +="<" + root->name + ">";
	}

	if(!root->valuePresent)
	{
		xmlString += '\n';
	
		for(unsigned int i=0;i<root->children.size();i++)
	
	{
			
			int level = root->children[i]->level;
			while(level)
			{
				xmlString += "   ";
				level--;
			}
			
			genXmlString(root->children[i]);
		}
		
		int level = root->level;
		while(level)
		{
			xmlString += "   ";
			level--;
		}
		
		
		
	}
	else
	{
		xmlString += root->value;
	}
	xmlString += "</" + root->name + ">\n";	
}

