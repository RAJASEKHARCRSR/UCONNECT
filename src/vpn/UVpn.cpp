/*****************************************************************************
**
** UVpn.cpp
**Implements the APIs that are used by the UConnect to connect to VPNs(windows only)
**
** Author: Vasuki Setlur
** Date  : 
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1				     VS			Creation
	0.2		26/06/12	 RB			Wrapper functions.
****************************************************************************/
#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS

#ifndef UNICODE
#define UNICODE
#endif

#include "UTypes.h"
#include "UVpn.h"
#include "ULog.h"
#include <windows.h>
#include <stdio.h>
#include "ras.h"

#include "raserror.h"
#pragma comment(lib, "rasapi32.lib")


/*defines*/
#define DEFAULT_SERVER_PORT "55555"
#define DEFAULT_SERVER_IP	"127.0.0.1"

/*global variables*/
U_SOCKET SocketHandle;
std::string connectedVpnName;

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
	rasLstNode_t *tmp;

	if(!*rasLstPP) *rasLstPP = rasNodeP;
	else
	{
		tmp = *rasLstPP;
		while(tmp->nextP)
			tmp = (rasLstNode_t*)tmp->nextP;
		tmp->nextP = rasNodeP;
	}
}


/***********************************************************************
*	Is a Dial profile already connected?
*	if so, get the connection handle
*	
*	Parameters
*	rasLstP		-	Pointer rasProfile node
*	Returns ucVPNret_t
***********************************************************************/
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
			LOG(ULOG_INFO_LEVEL,"VPN:HeapAlloc failed!");
            goto done;
        }
        // The first RASCONN structure in the array must contain the RASCONN structure size
        lpRasConn[0].dwSize = sizeof(RASCONN);
        
        // Call RasEnumConnections to enumerate active connections
        dwRet = RasEnumConnections(lpRasConn, &dwCb, &dwConnections);

        // If successful, print the names of the active connections.
        if (ERROR_SUCCESS == dwRet){
            //wprintf(L"The following RAS connections are currently active:\n");
            for (DWORD i = 0; i < dwConnections; i++){
                         //wprintf(L"%s\n", lpRasConn[i].szEntryName);
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
		LOG(ULOG_INFO_LEVEL,"VPN:The operation failed to acquire the buffer size.");
    }else{
        LOG(ULOG_INFO_LEVEL,"VPN:There are no active RAS connections.");
    }
done:
    return retVal;
}

							/* API */
/***********************************************************************
*	List available VPN Clients
*	This function needs to be written. Find a way to read installed VPN 
*	clientsfrom registry
*	
*	Parameters
***********************************************************************/
ucVPNret_t uvpn_listClients (std::string& vpnClient)
{
		/* Currntly looking only for Windows VPN Client that is
	pre installed on XP and above */
	DWORD version = GetVersion();
	DWORD major = (DWORD) (LOBYTE(LOWORD(version)));
	DWORD minor = (DWORD) (HIBYTE(LOWORD(version)));

	if(/*major>=5 && minor>=1*/1)
		vpnClient = "Microsoft VPN Client";
	else 
		vpnClient = "no client present";

	return UC_VPN_SUCCESS;
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

	DWORD r = GetLastError();

    if (dwRet == ERROR_BUFFER_TOO_SMALL)
	{
        // Allocate the memory needed for the array of RAS entry names.
        lpRasEntryName = (LPRASENTRYNAME) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);
        if (lpRasEntryName == NULL)
		{
			LOG(ULOG_INFO_LEVEL,"VPN:HeapAlloc failed!");
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
					LOG(ULOG_INFO_LEVEL,"VPN:Ras properties get failed!");
					goto err;
				}
				else
				{
					if(!_wcsicmp(rasEntry.szDeviceType, L"VPN"))
					{
						rasNodeP = (rasLstNode_t*)calloc(1, sizeof(rasLstNode_t));
						if(!rasNodeP)
						{
							LOG(ULOG_INFO_LEVEL,"VPN:Heap Alloc Failure");
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
}

/***********************************************************************
*	Disconnect VPN connection
*	
*	Parameters
*	Returns void
***********************************************************************/
void uvpn_disconnect ()
{
	/* check if this is already connected. If so fetch the handle */
	rasLstNode_t *rasLstP = NULL;
    if(uvpn_listProfiles(&rasLstP) == UC_VPN_SUCCESS)
	{
		while(rasLstP)
		{
			if(uvpn_isConnected(rasLstP) == UC_VPN_SUCCESS)
			{
				RasHangUp(rasLstP->rasHandle);
				break;
			}
			rasLstP = rasLstP->nextP;
		}
	}

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
		sprintf(cmdStr, "rasphone -d \"%s\"", rasLstP->profileName);
		LOG(ULOG_ERROR_LEVEL,"rasphone -d \"%s\"", rasLstP->profileName);
		system(cmdStr);
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
int uvpn_connect (rasLstNode_t *rasLstP)
{
	RASCREDENTIALS rasCreds;
	RASDIALPARAMS rasDialPrms;
	wchar_t *tmpStrP = NULL;
	int retVal;

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
		goto done;
	}
	free(tmpStrP);

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
	
	if((retVal = RasDial(NULL, NULL, &rasDialPrms, 0, NULL/*&RasDialFunc*/, &(rasLstP->rasHandle))) != ERROR_SUCCESS)
	{
		LOG(ULOG_INFO_LEVEL,"VPN:Error dialing %d",retVal );
		uvpn_handleErr(rasLstP, retVal);
		goto done;
	}

	/* save the handle */
	retVal = UC_VPN_SUCCESS;
done:
	return retVal;
}

/***********************************************************************
*	main calls the vpnInit
*	
*	Parameters
*	Returns int.
***********************************************************************/
int main ()
{
	vpnInit();
	return 0;
}

/***********************************************************************
*	This calls funtions to connect to server and process the event from the server
*	
*	Parameters
*	Returns int.
***********************************************************************/
static int vpnInit()
{
	initializeVPNLogger();
	FreeConsole();
	if(vpnConnectToServer() == UC_VPN_SUCCESS)
	process_vpn();
	return 0;
}

/***********************************************************************
*	Function connects to the server socket
*	
*	Parameters
*	Returns ucVPNret_t.
***********************************************************************/
ucVPNret_t vpnConnectToServer()
{
	ucVPNret_t retVal = UC_VPN_FAILURE;

	if(init_client() != 0)
		goto err;

	if((SocketHandle = connect_to_server(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT))
		== INVALID_SOCKET)
		goto err;

	LOG(ULOG_INFO_LEVEL,"VPN:connection established");

	retVal = UC_VPN_SUCCESS;

err:
	return retVal;
}

/***********************************************************************
*	Function receives the events from the server.
*	
*	Parameters
*	Returns void.
***********************************************************************/
static void process_vpn ()
{
	LOG(ULOG_INFO_LEVEL,"VPN:process_vpn -> entry");
	U_SINT8 recvbuf[4096];
	U_SINT32 iResult;
	U_SINT32 recvbuflen = 4096;
	U_SINT32 rcvLen = 0;
	U_SINT32 responseLen = 0;

	do 
	{
		iResult = rcv_data(SocketHandle, recvbuf, &rcvLen, recvbuflen);

		if (iResult == 0)
		{
			UEvent *reqFromVpn_p;
			reqFromVpn_p = reinterpret_cast<UEvent*>(recvbuf);
			VPN_Execute(reqFromVpn_p);
		}
		else
		{
			LOG(ULOG_ERROR_LEVEL,"VPN:process exited rcv_data failed");
			disconnect_fromServer(SocketHandle);
		} 
	} while(iResult == 0);
	LOG(ULOG_INFO_LEVEL,"VPN:process_vpn -> exit");
}
							/*Event Handle*/
/***********************************************************************
*	Handle for the event recieved from the server
*	
*	Parameters
*	reqFromVpn_p - pointer to the event received.
*	Returns void.
***********************************************************************/
static void VPN_Execute(UEvent *reqFromVpn_p)
{
	switch(reqFromVpn_p->getCmdId())
	{
	case UCONN_CMD_VPN_LIST_CLIENTS:
		{
			LOG(ULOG_INFO_LEVEL,"VPN:UCONN_CMD_VPN_LIST_CLIENTS received");
			std::string cliList = vpnListClients();
			UVPNListClientsRsp* clientList = new UVPNListClientsRsp (cliList); 
			sendEvent(reinterpret_cast<UEvent*>(clientList));	
		}
		break;
	case UCONN_CMD_VPN_LIST_PROFILES:
		{
			LOG(ULOG_INFO_LEVEL,"VPN:UCONN_CMD_VPN_LIST_PROFILES received");
			std::string proList = vpnListProfiles();
			UVPNListProfilesRsp* clientProfile = new UVPNListProfilesRsp(proList);
			sendEvent(reinterpret_cast<UEvent*>(clientProfile));
		}
		break;
	case UCONN_CMD_VPN_START:
		{
			LOG(ULOG_INFO_LEVEL,"VPN:UCONN_CMD_VPN_START received");
			UVPNStart* vpnStart = reinterpret_cast<UVPNStart*>(reqFromVpn_p);
			U_UINT32 connectRsp = vpnConnect(string(vpnStart->vpnName));
			UVPNStartRsp* connectStatus = new UVPNStartRsp(connectRsp);
			sendEvent(reinterpret_cast<UEvent*>(connectStatus));
		}
		break;
	case UCONN_CMD_VPN_STOP:
		{
			LOG(ULOG_INFO_LEVEL,"VPN:UCONN_CMD_VPN_STOP received");
			ucVPNret_t ret = UC_VPN_FAILURE;
			 ret = vpnDisconnect();
			 if (ret == UC_VPN_SUCCESS)
			 {
				UVPNStopRsp* disconnectStatus = new UVPNStopRsp();
				sendEvent(reinterpret_cast<UEvent*>(disconnectStatus));
			 }
		}
		break;
	default:
		break;
	}
}

/***********************************************************************
*	Function sends the event from vpn to server
*	
*	Parameters
*	evt - pointer to the event to be sent to the server.
*	Returns void.
***********************************************************************/
static void sendEvent(UEvent* evt)
{
	send_data(SocketHandle,(U_SINT8 *)evt,evt->getSize());
	delete evt;
}
						/*Wrapper functions*/
/***********************************************************************
*	Vpn wrapper function for vpnconnect 
*
*	Parameters
*	vpnProfName - vpn profilr name to be connected
*	Returns int.
***********************************************************************/
static U_UINT32 vpnConnect(std::string vpnProfName)
{
	LOG(ULOG_INFO_LEVEL,"VPN:vpnConnect ->entry");
	RasLstNodeData_t nodeData = getRasFromProfileName(vpnProfName);
	if(nodeData.firstNode == NULL)
	{
		return UC_VPN_FAILURE;
	}
	
	if(uvpn_isConnected(nodeData.selectedNode) == UC_VPN_SUCCESS)
	{
		LOG(ULOG_INFO_LEVEL,"VPN:Already connected");
		FREE_RAS_LIST(nodeData.firstNode);
		return 0;
	}
	U_UINT32 ret = uvpn_connect(nodeData.selectedNode);

	if (ret == 0)
	{
		connectedVpnName = vpnProfName;
	}
	else
	{
		LOG(ULOG_INFO_LEVEL,"VPN:Connect failed");
	}
	FREE_RAS_LIST(nodeData.firstNode);
	LOG(ULOG_INFO_LEVEL,"VPN:vpnConnect ->exit");
	return ret;
}

/***********************************************************************
*	Vpn wrapper function for vpndisconnect 
*
*	Parameters
*	Returns int.
***********************************************************************/
static ucVPNret_t vpnDisconnect()
{
	LOG(ULOG_INFO_LEVEL,"VPN:vpnDisconnect ->entry");
	uvpn_disconnect();
	LOG(ULOG_INFO_LEVEL,"VPN:vpnDisconnect ->exit");
	return UC_VPN_SUCCESS;
}

/***********************************************************************
*	Vpn wrapper function to get vpn   profile list
*
*	Parameters
*	Returns string.
***********************************************************************/
static std::string vpnListProfiles()
{
	LOG(ULOG_INFO_LEVEL,"VPN:vpnListProfiles ->entry");
	std::string vpnProfileList = "";
	rasLstNode_t *rasLstP = NULL;
    
	if(uvpn_listProfiles(&rasLstP) != UC_VPN_SUCCESS)
	{
		LOG(ULOG_DEBUG_LEVEL,"VPN:vpnListProfiles failed");
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
	FREE_RAS_LIST(rasLstP);

	LOG(ULOG_INFO_LEVEL,"VPN:vpnListProfiles ->exit");
	return vpnProfileList;
}

/***********************************************************************
*	Vpn wrapper function to get vpn client list
*
*	Parameters
*	Returns string.
***********************************************************************/
static std::string vpnListClients()
{
	LOG(ULOG_INFO_LEVEL,"VPN:vpnListClients ->entry");
	std::string vpnClientList="";
	uvpn_listClients(vpnClientList);
	LOG(ULOG_INFO_LEVEL,"VPN:vpnListClients ->exit");
	return vpnClientList;	
}

/***********************************************************************
*	Function to get the ras profile from the link list for given profile.
*
*	Parameters
*	vpnProfName - profile name to get the profile.
*	Returns RasLstNodeData_t.
***********************************************************************/
static RasLstNodeData_t getRasFromProfileName(std::string vpnProfName)
{
	LOG(ULOG_INFO_LEVEL,"VPN:getRasFromProfileName ->entry");
	rasLstNode_t *rasLstP = NULL;
	RasLstNodeData_t nodeData;
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

	LOG(ULOG_INFO_LEVEL,"VPN:getRasFromProfileName ->exit");
	return nodeData;
}

/***********************************************************************
*	Function to log
*
*	Parameters
*	
***********************************************************************/
static void initializeVPNLogger()
{
	string fileLocation = getExecDirectory("vpn.exe") + "\\vpnLog.txt";
	strcpy(getLogger()->logfilename_p,fileLocation.c_str());
	getLogger()->setFileLogEnable(1);
	getLogger()->setLoglevel(ULOG_DEBUG_LEVEL);
	LOG(ULOG_DEBUG_LEVEL,"Logger initialized!");

}
#endif