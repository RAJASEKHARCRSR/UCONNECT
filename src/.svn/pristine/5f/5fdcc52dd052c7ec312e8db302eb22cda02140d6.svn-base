/* this file has the APIs that are used by the UConnect to connect to VPNs
 * the APIs are meant for use on Windows. 
 * TBD
 * - Port to MAC
 * - Add support for third party VPNs such as Cisco
 */

#if 1
/* suppress c_safe warnings for the time being */


#ifndef UNICODE
#define UNICODE
#endif
#include <uconn_OSAL.h>
#include <stdio.h>
#include "UCommonUtils.h"
#ifdef WIN32
#include "ras.h"
#include "raserror.h"
#pragma comment(lib, "rasapi32.lib")
#endif
//UVpn* UVpn::uVpn = NULL;



/* prototypes */
wchar_t* stringToWString (const char* src);
int unsignedShortStringLength (const wchar_t* src);
void wCharToUCharString (const wchar_t* src, unsigned char* dest);

/* Primary DS, it has all the information related to Dial MS 
 * VPN profiles in a Linked List
 */
typedef struct rasLstNode_s {
#ifdef WIN32
	RASENTRY rasEntry;
	unsigned char *profileName;
	HRASCONN rasHandle;
	struct rasLstNode_s *nextP;
#endif
} rasLstNode_t;

/* return type enum */
typedef enum ucVPNret_e {
	UC_VPN_SUCCESS = 0,
	UC_VPN_FAILURE,
}ucVPNret_t;

typedef struct RasLstNodeData {

	rasLstNode_s* firstNode;
	rasLstNode_s* selectedNode;

}RasLstNodeData_s;



ucVPNret_t uvpn_isConnected(rasLstNode_t *rasLstP);

RasLstNodeData getRasFromProfileName(std::string vpnProfName);


#define FREE_RAS_LIST(rasLstP)\
	do {\
	rasLstNode_t *tmp; \
	while(rasLstP)\
	{\
		tmp = rasLstP->nextP; \
		free(rasLstP->profileName);\
		free(rasLstP);\
		rasLstP = tmp;\
	}\
}while(0)

					/* util functions */
/***********************************************************************
*	Add an entry to the Ras List
*	
*	Parameters
*	rasLstPP	-	Pointer to Pointer Head of the ras Profile list
*	rasNodeP	-	Pointer to rasProfile node
*	Returns void
***********************************************************************/
static void addToRasEntryLst (rasLstNode_t **rasLstPP,rasLstNode_t *rasNodeP)
{
#ifdef WIN32
	rasLstNode_t *tmp;

	if(!*rasLstPP) *rasLstPP = rasNodeP;
	else
	{
		tmp = *rasLstPP;
		while(tmp->nextP)
			tmp = (rasLstNode_t*)tmp->nextP;
		tmp->nextP = rasNodeP;
	}
#endif
}

/***********************************************************************
*	Print available MS VPN profiles
*	
*	Parameters
*	rasLstP		-	Pointer rasProfile node
*	Returns void
***********************************************************************/
static void printVPNProfileNames (rasLstNode_t *rasLstP)
{

#ifdef WIN32
	while(rasLstP)
	{
		printf("NAME: %s\n",rasLstP->profileName);
		rasLstP = rasLstP->nextP;
	}
#endif
}

/***********************************************************************
*	Is a Dial profile already connected?
*	if so, get the connection handle
*	
*	Parameters
*	rasLstP		-	Pointer rasProfile node
*	Returns ucVPNret_t
***********************************************************************/
#ifdef WIN32
static ucVPNret_t uvpn_isConnected (rasLstNode_t *rasLstP)
{
    DWORD dwCb = 0;
    DWORD dwRet = ERROR_SUCCESS;
    DWORD dwConnections = 0;
    LPRASCONN lpRasConn = NULL;
	wchar_t *tmpStrP = NULL;
	ucVPNret_t retVal = UC_VPN_FAILURE;
    
    // Call RasEnumConnections with lpRasConn = NULL. dwCb is returned with the required buffer size and 
    // a return code of ERROR_BUFFER_TOO_SMALL
    dwRet = RasEnumConnections(lpRasConn, &dwCb, &dwConnections);

    if (dwRet == ERROR_BUFFER_TOO_SMALL){
        // Allocate the memory needed for the array of RAS structure(s).
        lpRasConn = (LPRASCONN) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);
        if (lpRasConn == NULL){
            printf("HeapAlloc failed!\n");
            goto done;
        }
        // The first RASCONN structure in the array must contain the RASCONN structure size
        lpRasConn[0].dwSize = sizeof(RASCONN);
        
        // Call RasEnumConnections to enumerate active connections
        dwRet = RasEnumConnections(lpRasConn, &dwCb, &dwConnections);

        // If successful, print the names of the active connections.
        if (ERROR_SUCCESS == dwRet){
            wprintf(L"The following RAS connections are currently active:\n");
            for (DWORD i = 0; i < dwConnections; i++){
                         wprintf(L"%s\n", lpRasConn[i].szEntryName);
						 if(!_wcsicmp((tmpStrP = stringToWString
							 ((const char *)rasLstP->profileName)), lpRasConn[i].szEntryName))
						 {
							/* already Connected */
							 rasLstP->rasHandle = lpRasConn[i].hrasconn;
							 retVal = UC_VPN_SUCCESS;
							 free(tmpStrP);
							 break;
						 }
						 free(tmpStrP);
                  }
        }
        //Deallocate memory for the connection buffer
        HeapFree(GetProcessHeap(), 0, lpRasConn);
        lpRasConn = NULL;
        goto done;
    }

    // There was either a problem with RAS or there are no connections to enumerate    
    if(dwConnections >= 1){
        printf("The operation failed to acquire the buffer size.\n");
    }else{
        printf("There are no active RAS connections.\n");
    }
done:
    return retVal;
}
#endif
							/* API */
/***********************************************************************
*	List available VPN Clients
*	This function needs to be written. Find a way to read installed VPN 
*	clientsfrom registry
*	
*	Parameters
***********************************************************************/
ucVPNret_t uvpn_listClients ()
{
		/* Currntly looking only for Windows VPN Client that is
	pre installed on XP and above */
#ifdef WIN32
	DWORD version = GetVersion();
	DWORD major = (DWORD) (LOBYTE(LOWORD(version)));
	DWORD minor = (DWORD) (HIBYTE(LOWORD(version)));

	printf("Windows version %d.%d\n", major, minor);	
	if(major>=5 && minor>=1)
	{
		printf("Microsoft VPN Client \n");
		UVpn::getVpn()->vpnClientList = "Microsoft VPN Client\n";
	}
	else
	{
		printf("No client is available \n");
		UVpn::getVpn()->vpnClientList = "";
	}

	return UC_VPN_SUCCESS;
#endif
}

/***********************************************************************
*	List available MS VPN Profiles
*	
*	Parameters
*	rasLstPP - Pointer to Pointer to Ras Profile node (allocated here)
*	Returns ucVPNret_t
***********************************************************************/
ucVPNret_t uvpn_listProfiles (rasLstNode_t **rasLstPP)
{
#ifdef WIN32
    DWORD dwCb = 0;
    DWORD dwRet = ERROR_SUCCESS;
    DWORD dwEntries = 0;
    LPRASENTRYNAME lpRasEntryName = NULL;
	RASENTRY rasEntry;
	DWORD rasEntryInfoSize;
	rasLstNode_t *rasNodeP;
	ucVPNret_t retVal = UC_VPN_FAILURE;
	unsigned char tmpStr[100];
	unsigned int ii = 0;
    
    // Call RasEnumEntries with lpRasEntryName = NULL. dwCb is returned with the required buffer size and 
    // a return code of ERROR_BUFFER_TOO_SMALL
    dwRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &dwCb, &dwEntries);

    if (dwRet == ERROR_BUFFER_TOO_SMALL)
	{
        // Allocate the memory needed for the array of RAS entry names.
        lpRasEntryName = (LPRASENTRYNAME) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);
        if (lpRasEntryName == NULL)
		{
            printf("HeapAlloc failed!\n");
            return UC_VPN_FAILURE;
		}
        // The first RASENTRYNAME structure in the array must contain the structure size
        lpRasEntryName[0].dwSize = sizeof(RASENTRYNAME);
        
        // Call RasEnumEntries to enumerate all RAS entry names
        dwRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &dwCb, &dwEntries);
		if(dwRet == ERROR_SUCCESS)
		{
			ii = 0;
			while(ii < dwEntries)
			{
				memset(&rasEntry, 0, sizeof(rasEntry));
				rasEntry.dwSize = sizeof(rasEntry);
				rasEntryInfoSize = sizeof(rasEntry);

				if(RasGetEntryProperties(NULL, lpRasEntryName[ii].szEntryName, (LPRASENTRY)&rasEntry,
					(LPDWORD )&rasEntryInfoSize, NULL, NULL) != ERROR_SUCCESS)
				{
					printf("Ras properties get failed\n");
					goto err;
				}
				else
				{
					if(!_wcsicmp(rasEntry.szDeviceType, L"VPN"))
					{
						rasNodeP = (rasLstNode_t*)calloc(1, sizeof(rasLstNode_t));
						if(!rasNodeP)
						{
							printf("Heap Alloc Failure\n");
							goto err;
						}
						memcpy(&(rasNodeP->rasEntry), &rasEntry, sizeof(RASENTRY));
						wCharToUCharString(lpRasEntryName[ii].szEntryName, tmpStr);
						rasNodeP->profileName = (unsigned char*)strdup((const char*)tmpStr);
						addToRasEntryLst(rasLstPP, rasNodeP);
						retVal = UC_VPN_SUCCESS;
					}
				}
				ii++;
			}
		}
	}
	
err:
    //Deallocate memory for the connection buffer
    HeapFree(GetProcessHeap(), 0, lpRasEntryName);
    lpRasEntryName = NULL;

	return retVal;
#endif
}

/***********************************************************************
*	Disconnect VPN connection
*	
*	Parameters
*	rasLstP - Pointer to Ras Profile node
*	Returns void
***********************************************************************/
void uvpn_disconnect (rasLstNode_t *rasLstP)
{
#ifdef WIN32
	/* check if this is already connected. If so fetch the handle */
	if(uvpn_isConnected(rasLstP) == UC_VPN_SUCCESS)
		RasHangUp(rasLstP->rasHandle);
#endif
}

/***********************************************************************
*	Handle Connection error - prompt for password here 
*	Populate this function to handle other errors as well
*	
*	Parameters
*	rasLstP - Pointer to Ras Profile node
*	Returns void
***********************************************************************/
static void uvpn_handleErr (rasLstNode_t *rasLstP, int err)
{
	char cmdStr[200];

	switch(err)
	{
	case 691:
#ifdef WIN32
		sprintf(cmdStr, "rasphone -d \"%s\"", rasLstP->profileName);
		system(cmdStr);
#endif
		break;
	default:
		break;
	}
}

/***********************************************************************
*	Connect using a Ras Profile
*	
*	Parameters
*	rasLstP - Pointer to Ras Profile node
*	Returns ucVPNret_t
***********************************************************************/
ucVPNret_t uvpn_connect (rasLstNode_t *rasLstP)
{
#ifdef WIN32
	RASCREDENTIALS rasCreds;
	RASDIALPARAMS rasDialPrms;
	wchar_t *tmpStrP = NULL;
	ucVPNret_t retVal = UC_VPN_FAILURE;
	int ret;

	if(!rasLstP)
		goto done;


	/* get the user name and password to connect */
	memset(&rasCreds, 0, sizeof(RASCREDENTIALS));
	rasCreds.dwSize = sizeof(RASCREDENTIALS);
	rasCreds.dwMask = RASCM_UserName | RASCM_Password;
	if(RasGetCredentials(NULL, 
		(tmpStrP = stringToWString((const char *)rasLstP->profileName)), 
		&rasCreds) != ERROR_SUCCESS)
	{
		free(tmpStrP);
		printf("RasGetCredentials failed\n");
		goto done;
	}
	free(tmpStrP);

	/*
	wprintf(L"Got username: %s\n", rasCreds.szUserName);
	wprintf(L"Got password: %s\n", rasCreds.szPassword);
	*/

	printf("Dialing: %s\n", rasLstP->profileName);

	memset(&rasDialPrms, 0, sizeof(RASDIALPARAMS));
	rasDialPrms.dwSize = sizeof(rasDialPrms);
	wcscpy(rasDialPrms.szEntryName, (tmpStrP = stringToWString((const char *)rasLstP->profileName)));
	free(tmpStrP);
	wcscpy(rasDialPrms.szPhoneNumber, L"");
	wcscpy(rasDialPrms.szCallbackNumber, L"");
	wcscpy(rasDialPrms.szUserName, rasCreds.szUserName);
	wcscpy(rasDialPrms.szPassword, rasCreds.szPassword);
	wcscpy(rasDialPrms.szDomain, rasCreds.szDomain);
	rasDialPrms.dwSubEntry = 0;
	rasDialPrms.dwCallbackId = NULL;
	//rasDialPrms.dwIfIndex = 0; /* For Now - Change it to the index got from UI */
	
	if((ret = RasDial(NULL, NULL, &rasDialPrms, 0, NULL/*&RasDialFunc*/, &(rasLstP->rasHandle))) != ERROR_SUCCESS)
	{
		printf("Error dialing %d\n", ret);
		uvpn_handleErr(rasLstP, ret);
		goto done;
	}

	/* save the handle */
	retVal = UC_VPN_SUCCESS;
done:
	return retVal;
#endif
}


/********** Test Code ************/
#define GET_RAS_ENTRY_FRM_IDX(rasLstP, tmpRasNodeP, idx)\
	do {\
	tmpRasNodeP = rasLstP;\
	if(idx) \
	{\
		while(tmpRasNodeP && --idx)\
			tmpRasNodeP = tmpRasNodeP->nextP;\
	}\
}while(0)
/*
int main ()
{
	int choice, idx;
	rasLstNode_t *rasLstP = NULL;
	rasLstNode_t *tmpRasNodeP = NULL;

	while(1)
	{
		printf("Enter a choice:\n");
		printf("1: List VPN Clients\n");
		printf("2: List VPN Profiles\n");
		printf("3: Connect\n");
		printf("4: Disconnect\n");
		printf("5: Exit\n");
		scanf("%d", &choice);
		switch(choice)
		{
		case 1:
			//List VPN Clients 
			uvpn_listClients();
			break;
		case 2:
			//free any earlier allocations 
			if(rasLstP) 
			{
				FREE_RAS_LIST(rasLstP);
				rasLstP = NULL;
			}
			// List VPN Profiles 
			if(uvpn_listProfiles(&rasLstP) != UC_VPN_SUCCESS)
			{
				printf("Listing Profiles Failed\n");
				continue;
			}
			printVPNProfileNames(rasLstP);
			break;
		case 3:
			//Connect 
			if(!rasLstP) 
			{
				printf("List the available profiles first\n");
				continue;
			}
			printf("Enter an Index (starts from 1)\n");
			scanf("%d", &idx);
			GET_RAS_ENTRY_FRM_IDX(rasLstP, tmpRasNodeP, idx);
			if(!tmpRasNodeP)
			{
				printf("Probably entered a wrong Index\n");
				continue;
			}
			if(uvpn_connect(tmpRasNodeP) != UC_VPN_SUCCESS)
			{
				printf("Connecting to VPN failed\n");
				continue;
			}
			break;
		case 4:
			//Disconnect
			if(!rasLstP) 
			{
				printf("List the available profiles first\n");
				continue;
			}
			printf("Enter an Index (starts from 1)\n");
			scanf("%d", &idx);
			GET_RAS_ENTRY_FRM_IDX(rasLstP, tmpRasNodeP, idx);
			uvpn_disconnect(tmpRasNodeP);
			break;
		case 5:
		default:
			goto done;
		}
	}
done:
	if(rasLstP) FREE_RAS_LIST(rasLstP);
	return 0;
}
*/
UVpn::UVpn()
{
	vpnProfName = "";
	vpnClientList = "";
}

UVpn* UVpn::getVpn()
{
	if(!uVpn) {
		uVpn = new UVpn();
	}
	return uVpn;
}

std::string UVpn::listVpnProfiles()
{
#ifdef WIN32
	std::string vpnProfileList = "";
	rasLstNode_t *rasLstP = NULL;
    
	if(uvpn_listProfiles(&rasLstP) != UC_VPN_SUCCESS)
	{
		return "";
	}
	else
	{
		while(rasLstP)
		{
			vpnProfileList += (char*)rasLstP->profileName;
			vpnProfileList += "\n";
			rasLstP = rasLstP->nextP;
		}

	}
	if(rasLstP) {

		FREE_RAS_LIST(rasLstP);
		
	}
	return vpnProfileList;
#endif

}

void UVpn::vpnConnect()
{
#ifdef WIN32
	RasLstNodeData nodeData = getRasFromProfileName(vpnProfName);
	
	if(uvpn_isConnected(nodeData.selectedNode) == UC_VPN_SUCCESS)
	{
		return;
	}
	uvpn_connect(nodeData.selectedNode);
	FREE_RAS_LIST(nodeData.firstNode);
#endif
}

void UVpn::vpnDisconnect()
{
#ifdef WIN32
	RasLstNodeData nodeData = getRasFromProfileName(vpnProfName);
	uvpn_disconnect(nodeData.selectedNode);
	FREE_RAS_LIST(nodeData.firstNode);
#endif
}

RasLstNodeData getRasFromProfileName(std::string vpnProfName)
{
#ifdef WIN32
	rasLstNode_t *rasLstP = NULL;
	RasLstNodeData nodeData;
	nodeData.firstNode = NULL;
	nodeData.selectedNode = NULL;
	if(uvpn_listProfiles(&rasLstP) != UC_VPN_SUCCESS)
	{
		return nodeData;
	}
	rasLstNode_t* selectedRasP = rasLstP;
	rasLstNode_t* tempRasP = rasLstP;
	
	
	while(rasLstP)
	{
		if(strstr(vpnProfName.c_str(),(const char*)rasLstP->profileName))
		{
			selectedRasP = rasLstP;
			break;
		}
		rasLstP = rasLstP->nextP;
	}

	nodeData.firstNode = tempRasP;
	nodeData.selectedNode = selectedRasP;
	
	return nodeData;
#endif
}

void UVpn::setVpnProfName(std::string vpnProfName)
{
	this->vpnProfName = vpnProfName;
}

void UVpn::setParam(unsigned int paramId,std::string param)
{
	switch(paramId)
	{
	case VPN_PROFILE:
			setVpnProfName(param);
		break;
	}
}

std::string UVpn::listVpnClients()
{
	vpnClientList = "";
	uvpn_listClients();
	return vpnClientList;

}

void vpn_init()
{

	connect_to_server("127.0.0.1", "55555");
}

#endif