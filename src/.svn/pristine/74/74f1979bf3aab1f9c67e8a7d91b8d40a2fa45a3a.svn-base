/*****************************************************************************
**
** UVpn.h
** This header file describes the interfaces for vpn 
**
** Author: RB
** Date  : 29/6/2012
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		26/6/12     RB		Creation
****************************************************************************/
#ifndef _UVPN_H_
#define _UVPN_H_
#include "uconn_OSAL.h"
#include "uconn_Event.h"

/* enum return codes*/
typedef enum ucVPNret_e {
	UC_VPN_SUCCESS = 0,
	UC_VPN_FAILURE,
}ucVPNret_t;

/* Primary DS, it has all the information related to Dial MS 
 * VPN profiles in a Linked List */
typedef struct rasLstNode_s {
	RASENTRY rasEntry;
	unsigned char *profileName;
	HRASCONN rasHandle;
	struct rasLstNode_s *nextP;
} rasLstNode_t;

/*struct to maintain the profile linked list*/
typedef struct RasLstNodeData_s {
	rasLstNode_s* firstNode;
	rasLstNode_s* selectedNode;
}RasLstNodeData_t;

/* Vpn Utilities*/
wchar_t* stringToWString (const char* src);
int unsignedShortStringLength (const wchar_t* src);
void wCharToUCharString (const wchar_t* src, unsigned char* dest);

/* Socket Functions*/
static int vpnInit();
static ucVPNret_t vpnConnectToServer();
static void sendEvent(UEvent* evt);
static void process_vpn ();
static void VPN_Execute(UEvent *reqFromVpn_p);
ucVPNret_t uvpn_isConnected(rasLstNode_t *rasLstP);

/*VPN Wrapper function*/
static U_UINT32 vpnConnect(std::string vpnProfName);
static ucVPNret_t vpnDisconnect();
static std::string vpnListProfiles();
static std::string vpnListClients();
static RasLstNodeData_t getRasFromProfileName(std::string vpnProfName);
static void initializeVPNLogger();


#endif /*_UVPN_H_*/