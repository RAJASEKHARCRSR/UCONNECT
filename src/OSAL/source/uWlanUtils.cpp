/*****************************************************************************
**
** uwlanUtils.cpp
** This fle cntains the generic functions that manage the wlan functionalities
** across all OS platforms. The native wlan functionalities and interfaces
** are abstracted in this file
** Make sure that the OS related code is under the right macros
**
** Author: 
** Date  : 
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

******************************************************************************/
#include "uWlanUtils.h"
#include "UCommonUtils.h"
#include "uconn_OSAL.h"


/***********************************************************************
*	inits the wlan utility. registers with the OS callback and performs
*	any OS specific init, such as getting a handle. the control block is
*	allocated in this function. this control block is opaque to the caller
*	and should be passed to any subsequent api calls 
*	
*	Parameters
*	CB_p		-	address of Ctrl Blk pointer (alloced in this func)
*	callBk_p	-	pointer to the callback function
*	Returns uwlanRetVal_t
***********************************************************************/
uwlanRetVal_t uwlanInit (uWlanCB_t **CB_p, void (*callBk_p)(int))
{
	uwlanRetVal_t retVal = UWLAN_SUCCESS;

	if(!CB_p || !callBk_p)
	{
		retVal = UWLAN_PARAMETER_ERROR;
		goto err0;
	}
	if(!(*CB_p = (uWlanCB_t *)malloc(sizeof(uWlanCB_t))))
	{
		retVal = UWLAN_MEMORY_FAILURE;
		goto err0;
	}
	memset(*CB_p, 0, sizeof(uWlanCB_t));
	/* get the handle wherever i is needed. Some os that dont need a handle
	return away */
	(*CB_p)->callBk_p = callBk_p; /* register our cb. fiure out what to do with it later */
#ifdef WIN32
	{
		unsigned int negVersion;
		if(WlanOpenHandle(2, NULL /* reserved */,
				(PDWORD)(&negVersion), &((*CB_p)->handle)) != ERROR_SUCCESS)
		{
			retVal = UWLAN_FAILURE;
			goto err1;
		}
	}
	/* register the windows callback here */
	if(WlanRegisterNotification((*CB_p)->handle, WLAN_NOTIFICATION_SOURCE_ALL, 
								FALSE, uwlanCallBk, (PVOID)(*CB_p), NULL, NULL) != ERROR_SUCCESS)
	{
		retVal = UWLAN_FAILURE;
		goto err1;
	}
#endif/*WIN32*/
	initialize_lock(&((*CB_p)->stateLock));
	(*CB_p)->syncLock_p = new USemaphore();
	UWLAN_SET_STATE((*CB_p),UWLAN_STATE_INITED);
	goto err0;
err1:
	free(*CB_p);
err0:
	return retVal;
}

/***********************************************************************
*	free any allocated memory and resources. also any OS specific holds
*	
*	Parameters
*	CB_p		-	address of Ctrl Blk pointer (alloced in this func)
*	Returns void
***********************************************************************/
void uwlanUnInit (uWlanCB_t *CB_p)
{
	/* clean up any pointers inside */
#ifdef WIN32
	WlanRegisterNotification(CB_p->handle, WLAN_NOTIFICATION_SOURCE_NONE, 
	FALSE, NULL, NULL, NULL, NULL);
	if(CB_p->ifInfoLst_p)
		WlanFreeMemory(CB_p->ifInfoLst_p);
	if(CB_p->handle)
		WlanCloseHandle(CB_p->handle, NULL);
#endif /*WIN32*/
	delete_lock(&(CB_p->stateLock));
	delete (CB_p->syncLock_p);
	free(CB_p);
}

/***********************************************************************
*	call this when you have to free the memory allocated for 
*	listing down the interface. it has to be called after every
*	successful call to uwlanGetIfLst to avoid a leak
*	
*	Parameters
*	ifInfoLst_p		-	pointer to ifList
*	Returns void
***********************************************************************/
void uwlanFreeIfLst (uWlanIfInfoLst_t *ifInfoLst_p)
{
	int ii;
	
	if(!ifInfoLst_p) return;

	for(ii = 0; ii < ifInfoLst_p->numOfIf; ii++)
	{
		if(ifInfoLst_p->ifInfo_p[ii].ifGUID_p) free(ifInfoLst_p->ifInfo_p[ii].ifGUID_p);
		if(ifInfoLst_p->ifInfo_p[ii].ifDesc_p) free(ifInfoLst_p->ifInfo_p[ii].ifDesc_p);
	}
	free(ifInfoLst_p->ifInfo_p);
	free(ifInfoLst_p);
}

/***********************************************************************
*	list down the interface available on a system. the list is got by
*	making os specific calls, and then populated into a generic structure.
*	in some cases such as windows, a pointer to the retrieved list is stored
*	in the control block as it is needed to get the list of networks
*	
*	Parameters
*	CB_p		-	address of Ctrl Blk pointer 
*	ifInfoLst_p -	pointer to the if list (alloced in this func)
*	Returns uwlanRetVal_t
***********************************************************************/
uwlanRetVal_t uwlanGetIfLst (uWlanCB_t *CB_p, uWlanIfInfoLst_t **ifInfoLst_p)
{
	uwlanRetVal_t retVal = UWLAN_SUCCESS;

	if(!CB_p)
	{
		retVal = UWLAN_PARAMETER_ERROR;
		goto err0;
	}
	if(!(*ifInfoLst_p = (uWlanIfInfoLst_t*)malloc(sizeof(uWlanIfInfoLst_t))))
	{
		retVal = UWLAN_MEMORY_FAILURE;
		goto err0;
	}
	memset(*ifInfoLst_p, 0, sizeof(uWlanIfInfoLst_t));
	/* Get interfaces for a specific OS */
#ifdef WIN32
	{
		PWLAN_INTERFACE_INFO_LIST pIfList;
		wchar_t* strGuid = NULL;
		int ii;

		/* if this function has already been called, then fo nothing */
		if(CB_p->ifInfoLst_p)
		{
			WlanFreeMemory(CB_p->ifInfoLst_p);
		}
		if(WlanEnumInterfaces(CB_p->handle, NULL, &pIfList) != ERROR_SUCCESS)
		{
			retVal = UWLAN_FAILURE;
			goto err1;
		}
		/* Copy to our structure */
		(*ifInfoLst_p)->numOfIf = pIfList->dwNumberOfItems;
		if(!((*ifInfoLst_p)->numOfIf))
		{
			retVal = UWLAN_FAILURE;
			goto err1;
		}
		(*ifInfoLst_p)->ifInfo_p = 
			(uWlanIfInfo_t*)malloc(sizeof(uWlanIfInfo_t)* (*ifInfoLst_p)->numOfIf);
		if(!(*ifInfoLst_p)->ifInfo_p)
		{
			retVal = UWLAN_MEMORY_FAILURE;
			goto err1;
		}
		for(ii = 0; ii < (*ifInfoLst_p)->numOfIf; ii++)
		{
			UuidToStringW(&pIfList->InterfaceInfo[ii].InterfaceGuid, (RPC_WSTR*)&strGuid);
			if(!((*ifInfoLst_p)->ifInfo_p[ii].ifGUID_p = 
				(unsigned char*)malloc(unsignedShortStringLength(strGuid)+1)))
			{
				RpcStringFreeW((RPC_WSTR*)&strGuid);
				retVal = UWLAN_MEMORY_FAILURE;
				goto err1;
			}
			wCharToUCharString(strGuid, (*ifInfoLst_p)->ifInfo_p[ii].ifGUID_p);
			RpcStringFreeW((RPC_WSTR*)&strGuid);
			if(!((*ifInfoLst_p)->ifInfo_p[ii].ifDesc_p = 
				(unsigned char*)malloc(unsignedShortStringLength
				(pIfList->InterfaceInfo[ii].strInterfaceDescription)+1)))
			{
				retVal = UWLAN_MEMORY_FAILURE;
				goto err1;
			}
			wCharToUCharString(pIfList->InterfaceInfo[ii].strInterfaceDescription, 
				(*ifInfoLst_p)->ifInfo_p[ii].ifDesc_p);
		}
		/* For windows we need to pass the interface struct to get nw lists
		so store it in the CB. it wont matter for the other oss */
		CB_p->ifInfoLst_p = pIfList;
	}
	goto err0;
#endif /*WIN32*/
err1:
	uwlanFreeIfLst(*ifInfoLst_p);
err0:
	return retVal;
}

/***********************************************************************
*	this function frees memory allocated to list networks. should 
*	be called afer every successful call to uwlanGetNwList to avoid
*	a leak
*	
*	Parameters
*	nwLst_p		-	pointer to n/w List
*	Returns void
***********************************************************************/
void uwlanFreeNwLst (uWlanNwLst_t *nwLst_p)
{
	if(nwLst_p->nwInfo_p) free(nwLst_p->nwInfo_p);
	free(nwLst_p);
}

/***********************************************************************
*	get a list of available networks. it gets the list that is stored
*	in the os, and will not start a scan. it makes os specific calls and
*	copies the results to a generic structure
*	
*	Parameters
*	CB_p		-	address of Ctrl Blk pointer 
*	nwLst_p -	pointer to n/w list (alloced in this func)
*	9fIdx	- interface index
*	Returns uwlanRetVal_t
***********************************************************************/
uwlanRetVal_t uwlanGetNwList (uWlanCB_t *CB_p, 
							  uWlanNwLst_t **nwLst_p, unsigned int ifIdx)
{
	uwlanRetVal_t retVal = UWLAN_SUCCESS;

	if(!CB_p || !nwLst_p)
	{
		retVal = UWLAN_PARAMETER_ERROR;
		goto err0;
	}
	if(!(*nwLst_p = (uWlanNwLst_t*)malloc(sizeof(uWlanNwLst_t))))
	{
		retVal = UWLAN_MEMORY_FAILURE;
		goto err0;
	}
	memset(*nwLst_p, 0, sizeof(uWlanNwLst_t));
#ifdef WIN32
	{
		PWLAN_AVAILABLE_NETWORK_LIST netLst_p;
		char *tmp_p;
		unsigned int ii;
		/* check for index */
		if(ifIdx >= CB_p->ifInfoLst_p->dwNumberOfItems)
		{
			retVal = UWLAN_PARAMETER_ERROR;
			goto err1;
		}
		if(WlanGetAvailableNetworkList(CB_p->handle, 
			&(CB_p->ifInfoLst_p->InterfaceInfo[ifIdx].InterfaceGuid), 
			0, NULL, &netLst_p) != ERROR_SUCCESS)
		{
			retVal = UWLAN_FAILURE;
			goto err1;
		}
		/* copy to our structure */
		(*nwLst_p)->numOfNw = netLst_p->dwNumberOfItems;
		(*nwLst_p)->nwInfo_p = 
			(uWlanNwInfo_t*)malloc(sizeof(uWlanNwInfo_t)*(*nwLst_p)->numOfNw);
		if(!(*nwLst_p)->nwInfo_p)
		{
			retVal = UWLAN_MEMORY_FAILURE;
			goto err1;
		}
		for(ii = 0; ii < (*nwLst_p)->numOfNw; ii++)
		{
			tmp_p = ssidToString(&((netLst_p)->Network[ii].dot11Ssid));
			strncpy((*nwLst_p)->nwInfo_p[ii].ssid, (const char *)tmp_p, UWLAN_SSID_LEN - 1);
			free(tmp_p);
			(*nwLst_p)->nwInfo_p[ii].isConnectable = (netLst_p)->Network[ii].bNetworkConnectable;
			(*nwLst_p)->nwInfo_p[ii].isProfileAvlbl = 
				(netLst_p)->Network[ii].dwFlags & WLAN_AVAILABLE_NETWORK_HAS_PROFILE? 1: 0;
			(*nwLst_p)->nwInfo_p[ii].isConnected = 
				((netLst_p)->Network[ii].dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED? 1: 0);
			(*nwLst_p)->nwInfo_p[ii].sigQuality = (netLst_p)->Network[ii].wlanSignalQuality;
			strncpy((*nwLst_p)->nwInfo_p[ii].bssType, 
				GetBssTypeString((netLst_p)->Network[ii].dot11BssType), UWLAN_BSS_LEN);
			strncpy((*nwLst_p)->nwInfo_p[ii].authAlgo, 
				GetAuthAlgoString((netLst_p)->Network[ii].dot11DefaultAuthAlgorithm), 
				UWLAN_AUTH_ALGO_LEN);
			strncpy((*nwLst_p)->nwInfo_p[ii].cipherAlgo, 
				GetCipherAlgoString((netLst_p)->Network[ii].dot11DefaultCipherAlgorithm), 
				UWLAN_CIPHER_ALGO_LEN);
		}
		WlanFreeMemory(netLst_p);
	}
	goto err0;
#endif/*WIN32*/
err1:
	uwlanFreeNwLst(*nwLst_p);
err0:
	return retVal;
}


/***********************************************************************
*	Connect to a network. In some OSs the connect is 
*	is synchronous, while in some it is async. This function
*	has options for both Synchronous and ASynchronous calls
*	
*	Parameters
*	CB_p		-	address of Ctrl Blk pointer 
*	ssid_p -	ssid to connect to
*	fIdx	- interface index
*	syncroniusly - if yes, then block till operation completes
*	Returns uwlanRetVal_t
***********************************************************************/
uwlanRetVal_t uwlanConnect (uWlanCB_t *CB_p, 
							char *ssid_p, unsigned int ifIdx, 
							bool synchronously)
{
	uwlanRetVal_t retVal = UWLAN_SUCCESS;

	if(!CB_p || !ssid_p)
	{
		retVal = UWLAN_PARAMETER_ERROR;
		goto err0;
	}

	UWLAN_SET_STATE(CB_p, UWLAN_STATE_CONNECTING);
#ifdef WIN32
	{
		WLAN_CONNECTION_PARAMETERS conParams;
		int callRet;
		wchar_t *wStrProfile_p;
		if(ifIdx >= CB_p->ifInfoLst_p->dwNumberOfItems)
		{
			retVal = UWLAN_PARAMETER_ERROR;
			goto err0;
		}
		conParams.dwFlags = 0; /* always for infrastructure */
		conParams.pDesiredBssidList = NULL; /* we dont care */
		wStrProfile_p = stringToWString(ssid_p);
		conParams.strProfile = wStrProfile_p;
		conParams.pDot11Ssid = NULL;
		conParams.dot11BssType = dot11_BSS_type_infrastructure; /* always infrastructure */
		conParams.wlanConnectionMode = wlan_connection_mode_profile; /* always profile */
		
		/* in windows the connect call is asynchronous. if a synchronous call is needed 
		then take a sync lock here and release in the call back */
		callRet = WlanConnect(CB_p->handle, 
			&(CB_p->ifInfoLst_p->InterfaceInfo[ifIdx].InterfaceGuid), 
			&conParams, NULL);
		/* stringToWString() has a malloc. so free */
		free(wStrProfile_p);
		switch(callRet)
		{
		case ERROR_SUCCESS:
			break;
		case ERROR_INVALID_PARAMETER:
			/* this is reached even when no profile is present
			so check if profile is present, else create a profile
			and attach and connect */
			/* search for ssid in the list and return profile not found */
			if(!uwlanProfilePresent(CB_p, ifIdx, ssid_p))
			{
				retVal = UWLAN_PROFILE_NOT_FOUND;
				goto err0;
			}
			retVal = UWLAN_PARAMETER_ERROR;
			goto err0;
			break;
		case ERROR_INVALID_HANDLE: /* this is fatal */
			retVal = UWLAN_PARAMETER_ERROR;
			goto err0;
			break;
		case ERROR_ACCESS_DENIED:
			retVal = UWLAN_PEMISSION_DENIED;
			goto err0;
		//for xp
		case ERROR_NOT_FOUND:
			retVal = UWLAN_PROFILE_NOT_FOUND;
			goto err0;
			break;
		default:
			goto err0;
			break;
		}
		/* in windows this is an async function. to make it sync take a lock here */
		if(synchronously)
			CB_p->syncLock_p->lock(500);
	}
#endif /*WIN32*/
err0:
	if(retVal != UWLAN_SUCCESS) UWLAN_SET_STATE(CB_p, UWLAN_STATE_INITED);
	return retVal;
}

/***********************************************************************
*	Disconnect from a network. Option to diconnect both
*	synchronously and asynchronously
*	
*	Parameters
*	CB_p		-	address of Ctrl Blk pointer 
*	fIdx	- interface index
*	syncroniusly - if yes, then block till operation completes
*	Returns uwlanRetVal_t
***********************************************************************/
uwlanRetVal_t uwlanDisconnect (uWlanCB_t *CB_p, 
							   unsigned int ifIdx, bool synchronously)
{
	uwlanRetVal_t retVal = UWLAN_SUCCESS;

	if(!CB_p )
	{
		retVal = UWLAN_PARAMETER_ERROR;
		goto err0;
	}
	UWLAN_SET_STATE(CB_p, UWLAN_STATE_DISCONNECTING);
#ifdef WIN32
	{
		if(ifIdx >= CB_p->ifInfoLst_p->dwNumberOfItems)
		{
			retVal = UWLAN_PARAMETER_ERROR;
			goto err0;
		}
		if(WlanDisconnect(CB_p->handle, 
			&(CB_p->ifInfoLst_p->InterfaceInfo[ifIdx].InterfaceGuid), NULL) != 
			ERROR_SUCCESS)
		{
			retVal = UWLAN_FAILURE;
			goto err0;
		}
		/* in windows this is an async function. to make it sync take a lock here */
		if(synchronously)
			CB_p->syncLock_p->lock(500);
	}
#endif /*WIN32*/
err0:
	if(retVal != UWLAN_SUCCESS) UWLAN_SET_STATE(CB_p, UWLAN_STATE_INITED);
	return retVal;
}

/***********************************************************************
*	Initiate a scan of the nw available. in some oss this operation
*	is asynchronous. it may make sens to make it synchronous
*	
*	Parameters
*	CB_p		-	address of Ctrl Blk pointer 
*	ifIdx	- interface index
*	syncroniusly - if yes, then block till operation completes
*	Returns uwlanRetVal_t
***********************************************************************/
uwlanRetVal_t uwlanScanNw (uWlanCB_t *CB_p, unsigned int ifIdx, 
						   bool synchronously)
{
	uwlanRetVal_t retVal = UWLAN_SUCCESS;

	if(!CB_p)
	{
		retVal = UWLAN_PARAMETER_ERROR;
		goto err0;
	}
#ifdef WIN32
	{
		if(ifIdx >= CB_p->ifInfoLst_p->dwNumberOfItems)
		{
			retVal = UWLAN_PARAMETER_ERROR;
			goto err0;
		}
		/* if the sync flag is set, then lock until callback is called
		to symulate the sync */
		if(WlanScan(CB_p->handle, 
			&(CB_p->ifInfoLst_p->InterfaceInfo[ifIdx].InterfaceGuid),
			NULL, NULL, NULL) != ERROR_SUCCESS)
		{
			retVal = UWLAN_FAILURE;
			goto err0;
		}
		/* in windows this is an async function. to make it sync take a lock here */
		if(synchronously)
			CB_p->syncLock_p->lock(500);
	}
#endif/*WIN32*/

err0:
	return retVal;
}

/***********************************************************************
*	Check if a profile for the ssid is present in the system
*	
*	Parameters
*	CB_p		-	address of Ctrl Blk pointer 
*	ifIdx	- interface index
*	ssid_p - ssid strng
*	Returns 1 if yes / 0 if no
***********************************************************************/
int uwlanProfilePresent (uWlanCB_t *CB_p, unsigned int ifIdx, 
						 char *ssid_p)
{
	int retVal = 0;

	/* list profiles */
#ifdef WIN32
	{
		PWLAN_PROFILE_INFO_LIST prlst_p;
		unsigned char tmpStr[256];
		unsigned int ii;

		if(WlanGetProfileList(CB_p->handle, 
			&(CB_p->ifInfoLst_p->InterfaceInfo[ifIdx].InterfaceGuid), 
			NULL, &prlst_p) != ERROR_SUCCESS)
				goto err0;
		for (ii = 0; ii < prlst_p->dwNumberOfItems; ii++)
		{
			wCharToUCharString((prlst_p->ProfileInfo[ii]).strProfileName, 
					tmpStr);
			/* the profile name is the same as SSID str */
			if(!strcmp((const char*)tmpStr, ssid_p))
			{
				/* found */
				retVal = 1;
				goto err0;
			}
		}
	}
#endif/*WIN32*/
err0:
	return retVal;
}

/***********************************************************************
*	check if we are connected. if connected send the ssid connected to 
*	
*	Parameters
*	CB_p		-	address of Ctrl Blk pointer 
*	ifIdx	- interface index
*	ssid_p - ssid strng
*	Returns 1 if yes / 0 if no
***********************************************************************/
int uwlanRVConnected (uWlanCB_t *CB_p, char *ssid_p, 
					  unsigned int ifIdx)
{
	int retVal = 0;
	unsigned int ii;
	uWlanNwLst_t *nwLst_p = NULL;

	if(uwlanGetNwList(CB_p, &nwLst_p, ifIdx) != UWLAN_SUCCESS)
		goto err0;
	for(ii = 0; ii < nwLst_p->numOfNw; ii++)
	{
		if(nwLst_p->nwInfo_p[ii].isConnected)
		{
			strncpy(ssid_p, nwLst_p->nwInfo_p[ii].ssid, UWLAN_SSID_LEN);
			retVal = 1;
			goto err1;
		}
	}
err1:
	uwlanFreeNwLst(nwLst_p);
err0:
	return retVal;
}

/***********************************************************************
*	Add a wlan profile to the system. May need the formation of an 
*	xml profile file in some operating systems
*	
*	Parameters
*	CB_p		-	address of Ctrl Blk pointer 
*	ifIdx	- interface index
*	ssid_p - ssid strng
*	shareKey_p - shared key - will be null for the first call
*	Returns uwlanRetVal_t
***********************************************************************/
uwlanRetVal_t uwlanAddProfile (uWlanCB_t *CB_p, char *ssid_p, char *shareKey_p,
							   unsigned int ifIdx)
{
	uwlanRetVal_t retVal = UWLAN_SUCCESS;
	uWlanNwLst_t *nwLst_p = NULL;
	uWlanNwInfo_t *nw_p = NULL;
	unsigned int ii;

	
	if((retVal = uwlanGetNwList(CB_p, &nwLst_p, ifIdx)) != UWLAN_SUCCESS)
	{
		goto err0;
	}
	/* get the nw struct */
	for(ii = 0; ii < nwLst_p->numOfNw; ii++)
	{
		if(!strcmp(ssid_p, nwLst_p->nwInfo_p[ii].ssid))
		{
			nw_p = &(nwLst_p->nwInfo_p[ii]);
			break;
		}
	}
	if(!nw_p)
	{
		retVal = UWLAN_FAILURE;
		goto err1;
	}
#ifdef WIN32
	{
		/* here we have a network with no profile. read the network
		and create a profile on the fly and attach to the interface.
		needs an xml to be created. the method followed here is very 
		crude, and needs to be changed. REVIST */
		FILE *pFile;
		char proFileName[256];
		char* pfileStrm;
		wchar_t* bstrXml = NULL;
		DWORD dwReason;

		
		/* is network connectable */
		if(!nw_p->isConnectable)
		{
			retVal = UWLAN_NW_WILL_NOT_CONNECT;
			goto err1;
		}
		/* only infrastructure network types supported for now */
		if(strncmp(nw_p->bssType, "Infrastructure", UWLAN_BSS_LEN))
		{
			retVal = UWLAN_NW_TYPE_NOT_SUPPORTED;
			goto err1;
		}
		if((stricmp(nw_p->cipherAlgo, "None")) && shareKey_p == NULL)
		{
			/* requres a passphrase */
			retVal = UWLAN_NEED_PASSWORD;
			goto err1;
		}
		sprintf(proFileName, "%s.xml", ssid_p);
		if(!(pFile = fopen(proFileName, "w")))
		{
			retVal = UWLAN_FAILURE;
			goto err1;
		}
		write2File(pFile, "<?xml version=\"1.0\" encoding=\"US-ASCII\"?>\n");
		write2File(pFile, "<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">\n");
		writeXmlLn(pFile, "name", ssid_p, 1);
		writeTag(pFile, "SSIDConfig", "start", 1);
		writeTag(pFile, "SSID", "start", 2);
		writeXmlLn(pFile, "name", ssid_p, 3);
		writeTag(pFile, "SSID", "end", 2);
		writeTag(pFile, "SSIDConfig", "end", 1);
		writeXmlLn(pFile, "connectionType", "ESS", 1);
		writeXmlLn(pFile, "connectionMode", "auto", 1);
		writeXmlLn(pFile, "autoSwitch", "false", 1);
		writeTag(pFile, "MSM", "start", 1);
		writeTag(pFile, "security", "start", 2);
		writeTag(pFile, "authEncryption", "start", 3);
		writeXmlLn(pFile, "authentication", nw_p->authAlgo, 4);
		writeXmlLn(pFile, "encryption", nw_p->cipherAlgo, 4);
		writeXmlLn(pFile, "useOneX", "false", 4);
		writeTag(pFile, "authEncryption", "end", 3);
		if(!(!stricmp(nw_p->authAlgo, "open") && !stricmp(nw_p->cipherAlgo, "none")))
		{
			writeTag(pFile, "sharedKey", "start", 3);
			if((!stricmp(nw_p->authAlgo, "open") || !stricmp(nw_p->authAlgo, "shared")) &&
				(!stricmp(nw_p->cipherAlgo, "WEP") || !stricmp(nw_p->cipherAlgo, "WEP40") || 
				!stricmp(nw_p->cipherAlgo, "WEP104")))
				writeXmlLn(pFile, "keyType", "networkKey", 4);
			if((!stricmp(nw_p->authAlgo, "WPAPSK") || !stricmp(nw_p->authAlgo, "WPA2PSK")) &&
				(!stricmp(nw_p->cipherAlgo, "TKIP") || !stricmp(nw_p->cipherAlgo, "AES")))
				writeXmlLn(pFile, "keyType", "passPhrase", 4);
			writeXmlLn(pFile, "protected", "false", 4);
			writeXmlLn(pFile, "keyMaterial", shareKey_p, 4);
			writeTag(pFile, "sharedKey", "end", 3);
		}
		writeTag(pFile, "security", "end", 2);
		writeTag(pFile, "MSM", "end", 1);

		write2File(pFile, "</WLANProfile>");

		fclose(pFile);
		if(readFile(&pfileStrm, proFileName)
			== -1)
		{
			retVal = UWLAN_FAILURE;
			goto err1;
		}
		bstrXml = stringToWString(pfileStrm);
		free(pfileStrm);
		if(WlanSetProfile(CB_p->handle, &(CB_p->ifInfoLst_p->InterfaceInfo[ifIdx].InterfaceGuid), 
			0, bstrXml, NULL, TRUE, NULL, &dwReason) != ERROR_SUCCESS)
		{
			uwlanDelProfile(CB_p,ifIdx,ssid_p);
			free(bstrXml);
			retVal = UWLAN_FAILURE;
			goto err1;
		}
		free(bstrXml);
	}
#endif/*WIN32*/
err1:
	uwlanFreeNwLst(nwLst_p);
err0:
	return retVal;
}

/***********************************************************************
*	delete a saved profile
*	
*	Parameters
*	CB_p		-	address of Ctrl Blk pointer 
*	ifIdx	- interface index
*	ssid_p - ssid strng
*	shareKey_p - shared key - will be null for the first call
*	Returns uwlanRetVal_t
***********************************************************************/
uwlanRetVal_t uwlanDelProfile (uWlanCB_t *CB_p, 
								unsigned int ifIdx,
								 char *ssidStr)
{
#ifdef WIN32

	wchar_t *pTmpStr;
	uwlanRetVal_t retVal = UWLAN_SUCCESS;

	pTmpStr = stringToWString((const char*)ssidStr);
	if(WlanDeleteProfile(CB_p->handle, 
			&(CB_p->ifInfoLst_p->InterfaceInfo[ifIdx].InterfaceGuid), pTmpStr, NULL) !=
			ERROR_SUCCESS)
	{
		retVal = UWLAN_FAILURE;
		goto done;
	}
	retVal = UWLAN_SUCCESS;

done:
	free(pTmpStr);
	return retVal;

#endif
}

/***********************************************************************
*	Get statistics
*	
*	Parameters
*	Returns uwlanRetVal_t
***********************************************************************/
uwlanRetVal_t uwlanGetStats (uWlanCB_t *CB_p, unsigned int ifIdx, 
							 uWlanStats_t **stats_p)
{
	uwlanRetVal_t retVal = UWLAN_SUCCESS;

	if(!(*stats_p = (uWlanStats_t*)malloc(sizeof(uWlanStats_t))))
	{
		retVal = UWLAN_MEMORY_FAILURE;
		goto err0;
	}
	memset(*stats_p, 0, sizeof(uWlanStats_t));
#ifdef WIN32
	{
		PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
		DWORD connectInfoSize = sizeof(WLAN_CONNECTION_ATTRIBUTES);
		WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;

		if(ifIdx >= CB_p->ifInfoLst_p->dwNumberOfItems)
		{
			retVal = UWLAN_PARAMETER_ERROR;
			goto err1;
		}
		if(WlanQueryInterface(CB_p->handle, 
			&(CB_p->ifInfoLst_p->InterfaceInfo[ifIdx].InterfaceGuid),
			wlan_intf_opcode_current_connection,
			NULL, &connectInfoSize, (PVOID*)&pConnectInfo, &opCode) != ERROR_SUCCESS)
		{
			retVal = UWLAN_FAILURE;
			goto err1;
		}
		/* copy to our struct */
		(*stats_p)->RxRate = pConnectInfo->wlanAssociationAttributes.ulRxRate;
		(*stats_p)->TxRate = pConnectInfo->wlanAssociationAttributes.ulTxRate;
		
		WlanFreeMemory(pConnectInfo);
		goto err0;
	}
#endif/*WIN32*/
err1:
	free(*stats_p);
err0:
	return retVal;
}

#ifdef WIN32
static void WINAPI uwlanCallBk (PWLAN_NOTIFICATION_DATA pData, void* pContext)
{
	uWlanCB_t *CB_p = (uWlanCB_t*)pContext;

	switch(pData->NotificationCode)
	{
		case wlan_notification_acm_connection_start:
			break;
		case wlan_notification_acm_connection_complete:
			if(IsWinXP())
			{
				CB_p->syncLock_p->release();
				CB_p->callBk_p(UWLAN_CONNECTED);
				UWLAN_SET_STATE(CB_p, UWLAN_STATE_CONNECTED);
			}
			break;


		case wlan_notification_msm_connected:
			CB_p->syncLock_p->release();
			CB_p->callBk_p(UWLAN_CONNECTED);
			UWLAN_SET_STATE(CB_p, UWLAN_STATE_CONNECTED);
			break;

		case wlan_notification_acm_connection_attempt_fail:
			CB_p->syncLock_p->release();
			CB_p->callBk_p(UWLAN_OPERATION_FAILED);
			UWLAN_SET_STATE(CB_p, UWLAN_STATE_LAST_OP_FAILED);
			break;

		case wlan_notification_acm_interface_arrival:
			CB_p->callBk_p(UWLAN_DEVICE_PLUGIN);
			break;

		case wlan_notification_acm_interface_removal:
			CB_p->syncLock_p->release();
			CB_p->callBk_p(UWLAN_DEVICE_PLUGOUT);
			break;

		case wlan_notification_acm_profile_change:
			CB_p->syncLock_p->release();
			break;

		case wlan_notification_acm_disconnecting:
			UWLAN_SET_STATE(CB_p, UWLAN_STATE_DISCONNECTING);
			break;

		case wlan_notification_acm_disconnected:
			CB_p->syncLock_p->release();
			CB_p->callBk_p(UWLAN_DISCONNECTED);
			UWLAN_SET_STATE(CB_p, UWLAN_STATE_DISCONNECTED);
			break;

		case wlan_notification_acm_scan_complete:
			CB_p->syncLock_p->release();
			CB_p->callBk_p(UWLAN_SCAN_COMPLETED);
			break;

		case wlan_notification_acm_scan_fail:
			CB_p->syncLock_p->release();
			CB_p->callBk_p(UWLAN_STATE_SCAN_FAILED);
			break;

		default:
			break;

	}
}
#endif /*WIN32*/

