#include "UWlan.h"
#include "WiFi_NM.h"

extern WifiNMInfo gWifiNMInfo;

void displayInterfaceInfo (PWLAN_INTERFACE_INFO pInterface)
{
	wchar_t* strGuid = NULL;
	unsigned long dwError;

	if(!pInterface) 
		return;

	dwError = UuidToStringW(&pInterface->InterfaceGuid, (RPC_WSTR*)&strGuid);
	if(dwError!=0) 
		printf("in function UuidToStringW.\n");
	
	printf("\tGUID: ");
	printlnWCharString(strGuid);
	
	printf("\t");
	printlnWCharString(pInterface->strInterfaceDescription);

	printf("\tState: %s\n", GetInterfaceStateString(pInterface->isState));

	RpcStringFreeW((RPC_WSTR*)&strGuid);

}

void displayInterfaces (PWLAN_INTERFACE_INFO_LIST pIfList)
{
	unsigned int i;

	if(!pIfList) 
		return;
	
	printf("There are %lu interfaces in the system.\n", pIfList->dwNumberOfItems);

	/* print out interface information */
	for(i = 0; i < pIfList->dwNumberOfItems; i++)
	{
		printf("Interface %d:\n", i);
		displayInterfaceInfo(&pIfList->InterfaceInfo[i]);
	}
}

void displayNetwork (PWLAN_AVAILABLE_NETWORK pNetwork)
{
	if(!pNetwork) 
		return;

	Wifi_Network_s wifiNetwork;
	
	

	/* SSID */
	//printf("\tSSID: ");
	printlnSsid(pNetwork->dot11Ssid);

	char* ssidString = ssidToString(&(pNetwork->dot11Ssid));

	
	

	strcpy(wifiNetwork.ssid,ssidString);
	//strncpy(wifiNetwork.ssid,ssidString,sizeof(ssidString));
	//wifiNetwork.ssid[sizeof(ssidString)] = '\0';
	delete ssidString;


	


	/* whether security is enabled */
	if(pNetwork->bSecurityEnabled) 
		printf("\tSecurity enabled.\n");
	else 
		printf("\tSecurity not enabled.\n");

	wifiNetwork.securityEnabled = pNetwork->bSecurityEnabled != 0;

	/* number of BSSIDs */
	printf("\tContains %lu BSSIDs.\n", pNetwork->uNumberOfBssids);
	wifiNetwork.noOfBssids = (U_UINT32)pNetwork->uNumberOfBssids;
	

	/* whether have a profile for this SSID */
	if(pNetwork->dwFlags & WLAN_AVAILABLE_NETWORK_HAS_PROFILE)
	{
		
		wifiNetwork.profileAvailable = true;
		printf("\tHas a matching profile: ");
		printlnWCharString(pNetwork->strProfileName);
	}
	else
	{
		wifiNetwork.profileAvailable = false;
	}

	/* whether it is connected */
	if(pNetwork->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED)
	{
		printf("\tCurrently connected.\n");
		wifiNetwork.connected = true;
	}
	else
	{
		wifiNetwork.connected = false;
	}

	/* whether it is connectable */
	if(!pNetwork->bNetworkConnectable)
	{
	
		/* the reason that it is not connectable */
		printf("\tThe network is not connectable \n");
	}
	else 
		printf("\tThe network is connectable\n");

	/* BSS type */
	printf("\tBSS type: %s\n", GetBssTypeString(pNetwork->dot11BssType));
	
	/* Signal quality */
	printf("\tSignal quality: %lu%%\n", pNetwork->wlanSignalQuality);
	wifiNetwork.signalQuality = (U_UINT32)(pNetwork->wlanSignalQuality);

	/* Default auth algorithm */
	printf("\tDefault authentication algorithm: %s\n", GetAuthAlgoString(pNetwork->dot11DefaultAuthAlgorithm));
	strcpy(wifiNetwork.authAlgo,GetAuthAlgoString(pNetwork->dot11DefaultAuthAlgorithm));

	/* Default cipher algorithm */
	printf("\tDefault cipher algorithm: %s\n", GetCipherAlgoString(pNetwork->dot11DefaultCipherAlgorithm));
	strcpy(wifiNetwork.cipherAlgo,GetCipherAlgoString(pNetwork->dot11DefaultCipherAlgorithm));

	gWifiNMInfo.networksAvailable.networkList.push_back(wifiNetwork);
	
}

void displayNetworks (PWLAN_AVAILABLE_NETWORK_LIST pList)
{
	unsigned int i;

	if(!pList) 
		return;
	
	/* print all visible networks */
	printf("Total %lu networks are visible.\n", pList->dwNumberOfItems);
	
	for (i = 0; i < pList->dwNumberOfItems; i++)
	{
		printf("Network %d:\n", i);
		displayNetwork(&pList->Network[i]);
		printf("\n");
	}
}


void printlnWCharString (wchar_t* wCharString)
{
	printWCharString(wCharString);
	printf("\n");
}

void printWCharString (wchar_t* wCharString)
{
	int i = 0;
	while(wCharString[i]!=0)
	{
		printf("%c", wCharString[i]);
		i++;
	}
}

char *GetInterfaceStateString (int wlanInterfaceState)
{
	char* strRetCode;

	switch(wlanInterfaceState)
	{
		case 0: strRetCode = "not ready"; break;
		case 1: strRetCode = "connected"; break;
		case 2: strRetCode = "ad hoc network formed"; break;
		case 3: strRetCode = "disconnecting"; break;
		case 4: strRetCode = "disconnected"; break;
		case 5: strRetCode = "associating"; break;
		case 6: strRetCode = "discovering"; break;
		case 7: strRetCode = "authenticating"; break;
		default: strRetCode = "invalid interface state"; break;
	}
	return strRetCode;
}

char* GetBssTypeString (int dot11BssType)
{
	char* strRetCode;

	switch(dot11BssType)
	{
		case 1: strRetCode = "Infrastructure"; break;
		case 2: strRetCode = "Ad hoc"; break;
		case 3: strRetCode = "Any"; break;
		default: strRetCode = "Unknown BSS type";
	}
	return strRetCode;
}

char* GetAuthAlgoString (int dot11AuthAlgo)
{
	char* strRetCode = "Unknown algorithm";

	switch(dot11AuthAlgo)
	{
		case 1: strRetCode = "Open"; break;
		case 2: strRetCode = "Shared"; break;
		case 3: strRetCode = "WPA-Enterprise"; break;
		case 4: strRetCode = "WPA-Personal"; break;
		case 5: strRetCode = "WPA-NONE"; break;
		case 6: strRetCode = "WPA2-Enterprise"; break;
		case 7: strRetCode = "WPA2-Personal"; break;
		default: if ((dot11AuthAlgo & DOT11_AUTH_ALGO_IHV_START) == 0x0) strRetCode = "Vendor-specific algorithm";
	}
	return strRetCode;
}

char* GetCipherAlgoString (int dot11CipherAlgo)
{
	char* strRetCode = "Unknown algorithm";

	switch(dot11CipherAlgo)
	{
		case 0: strRetCode = "None"; break;
		case 1: strRetCode = "WEP40"; break;
		case 2: strRetCode = "TKIP"; break;
		case 4: strRetCode = "AES"; break;
		case 5: strRetCode = "WEP104"; break;
		case 256: strRetCode = "USE-GROUP"; break;
		case 257: strRetCode = "WEP"; break;
		default: if ((dot11CipherAlgo & DOT11_CIPHER_ALGO_IHV_START) == 0x0) strRetCode = "Vendor-specific algorithm";
	}
	return strRetCode;
}

void printlnSsid (DOT11_SSID ssid)
{
	printSsid(ssid);
	printf("\n");
}



void printSsid (DOT11_SSID ssid)
{
	char* strSsid = ssidToString(&ssid);
	printf("%s", strSsid);
	free(strSsid);
}

char* ssidToString(PDOT11_SSID pSsid)
{
	char* string = (char*) malloc(pSsid->uSSIDLength+1);
	unsigned int i;
	
	for(i=0; i < pSsid->uSSIDLength; i++) 
		string[i] = pSsid->ucSSID[i];
	string[i] = '\0';
	return string;
}

void printProfileInfo(PWLAN_PROFILE_INFO profile)
{
	if (profile==NULL) return;
	printf("\t");
	printlnWCharString(profile->strProfileName);
}

void printProfileInfoList (PWLAN_PROFILE_INFO_LIST pProfileList)
{
	unsigned int i;
	if (pProfileList==NULL) return;
	
	printf("There are %lu profiles on the interface.\n", pProfileList->dwNumberOfItems);
	
	for (i = 0; i < pProfileList->dwNumberOfItems; i++)
        printProfileInfo(&pProfileList->ProfileInfo[i]);
}





void write2File (FILE *pFile, char *str)
{
	fwrite(str, strlen(str), 1, pFile);
}

void writeTag (FILE *pFile, char *str, char *startOrEnd, 
			   int level)
{
	char *pTmpStr;

	pTmpStr = (char*)malloc(strlen(str) + 7);
	if(!pTmpStr) 
	{
		printf("MALLOC FAILED\n");
		return;
	}
	while(level--) 
		fwrite("    ", 4, 1, pFile);
	if(!strcmp(startOrEnd, "start"))
		sprintf(pTmpStr, "<%s>\n", str);
	else
		sprintf(pTmpStr, "</%s>\n", str);
	fwrite(pTmpStr, strlen(pTmpStr), 1, pFile);
	free(pTmpStr);
}

void writeXmlLn (FILE *pFile, char *tag, 
				 char *str, int level)
{
	char *pTmpStr;

	pTmpStr = (char*)malloc(2*strlen(tag) + strlen(str) + 7);
	if(!pTmpStr) 
	{
		printf("MALLOC FAILED 2\n");
		return;
	}
	while(level--) 
		fwrite("    ", 4, 1, pFile);
	sprintf(pTmpStr, "<%s>%s</%s>\n", tag, str, tag);
	fwrite(pTmpStr, strlen(pTmpStr), 1, pFile);
	free(pTmpStr);
}

int readFile (char** pData, char *file)
{
	#define DATA 1024
	FILE *pTmpFil;
	int i=0, readed;
	int finish = FALSE;

	if((pTmpFil = fopen(file, "r")) == NULL)
	{
		printf("Error reading file\n");
		return -1;
	}

	*pData=NULL;
	
	while (!finish)
	{
		*pData = (char*) realloc (*pData, DATA * (i+1) +1);
		readed = fread(*pData + DATA*i, 1, DATA, pTmpFil);
		if (readed<DATA) finish=TRUE;
		i++;
	}
	(*pData)[DATA*(i-1)+readed]=0;
	fclose(pTmpFil);
	return 0;
}

#define UWLAN_DISPLAY_STAT_VAL(val)\
{\
	if((val) == -1)\
		printf("Invalid Counter\n");\
	else\
		printf("%u\n", (val));\
}

void displayStats (PWLAN_STATISTICS pStats, 
				   PWLAN_INTERFACE_INFO pIfInfo)
{
	wchar_t* strGuid = NULL;

	printf("INTERFACE INFO\n");
	UuidToStringW(&pIfInfo->InterfaceGuid, (RPC_WSTR*)&strGuid);
	printf("Interface GUID\t\t:");
	printlnWCharString(strGuid);
	RpcStringFreeW((RPC_WSTR*)&strGuid);	
	
	printf("Interface Description\t:");
	printlnWCharString(pIfInfo->strInterfaceDescription);

	printf("UNICAST COUNTERS\n");
	printf("Transmitted Frames\t:");
	UWLAN_DISPLAY_STAT_VAL(pStats->MacUcastCounters.ullTransmittedFrameCount);
	printf("Received Frames\t\t:");
	UWLAN_DISPLAY_STAT_VAL(pStats->MacUcastCounters.ullReceivedFrameCount);

	printf("MULTICAST COUNTERS\n");
	printf("Transmitted Frames\t:");
	UWLAN_DISPLAY_STAT_VAL(pStats->MacMcastCounters.ullTransmittedFrameCount);
	printf("Received Frames\t\t:");
	UWLAN_DISPLAY_STAT_VAL(pStats->MacMcastCounters.ullReceivedFrameCount);
}