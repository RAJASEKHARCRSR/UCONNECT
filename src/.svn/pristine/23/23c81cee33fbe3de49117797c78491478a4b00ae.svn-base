/*****************************************************************************
**
** uconn_ClientCommon.h
** This header file describes the interfaces for the Server process in the CM
**
** Author: Suresh Lingadalli
** Date  : 07-FEB-2012
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		07/02/12     SL			Creation
****************************************************************************/

#ifndef UCONN_CLIENTCOMMON_H_
#define UCONN_CLIENTCOMMON_H_

#ifdef WIN32
#include <dbt.h>
#include <tchar.h>
#include <Wtsapi32.h>
#endif
#include <algorithm>
#include "uconn_OSAL.h"
#include "uconn_sockComm.h"
#include "uconn_ServerAPI.h"
#include "uconn_StateMachine.h"
#include "uconn_ATHandler.h"
#include "UTypes.h"



#define DEFAULT_SERVER_PORT "55555"
#define DEFAULT_SERVER_IP	"127.0.0.1"
#define UCONN_MAX_CLIENTS_SUPPORTED 3

#define DEFAULT_BUFLEN 4096


typedef struct clientDetails_s {
	U_THREAD threadHandle;
	U_SOCKET clientSock;
    clientDetails_s():clientSock(INVALID_SOCKET){};
} clientDetails_t;

U_VOID uconn_EventCallback (UEvent *event_p);

int InitCMSocketService();
//VOID WriteLog(char* pMsg);
#define MY_LOG(str) \
{\
	WriteLog(str);\
}


//VOID WriteSerLog(char* pMsg,int param);
void initialiseCallBackFunction(P_AT_CALLBACK_WITH_ID callback_fn);
//int getComPortFromDevMngsPortSection(string &onlyVidPidStr, string &comport);
void replaceAll(string& s , string strToReplace , string stringToReplaceWith);
bool getVidPidStringFromUSBDeviceArival(string &devVidPidStr);
bool matchInsertedDeviceInfoWithConfigFile(string &onlyVidPidStr,string &devMiStr);
bool extractDevVidPidMiString(string &vidPidStr,string &vidStr,string &pidStr,string &miStr,string &onlyVidPidStr);
void uniodeStringToCharStringConversion(short *src,char *dest);
#ifdef WIN32
bool UpdateDevice(PDEV_BROADCAST_DEVICEINTERFACE pDevInf, WPARAM wParam);
bool DeviceChangeType(LPARAM lParam,WPARAM wParam);
#endif

void initialiseVpn();
#endif //UCONN_CLIENTCOMMON_H_
