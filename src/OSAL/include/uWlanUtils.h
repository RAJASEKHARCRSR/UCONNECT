/*****************************************************************************
**
** uwlanUtils.h
**	header file to use generic wlan functionalities
**
** Author: 
** Date  : 
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

******************************************************************************/

#ifndef _UWLANUTILS_H_
#define _UWLANUTILS_H_

#include "uconn_OSAL.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <io.h>
#include <windows.h>
#include <wlanapi.h>
#include <objbase.h>
#include <fcntl.h>
#include <Rpc.h>
#include <wtypes.h>
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "Rpcrt4.lib")
#endif /*WIN32*/


#include "uWlanHlpr.h"


/* generic return values */
typedef enum uwlanRetVal_e{
	UWLAN_SUCCESS = 0,
	UWLAN_FAILURE,
	UWLAN_PARAMETER_ERROR,
	UWLAN_MEMORY_FAILURE,
	UWLAN_PROFILE_NOT_FOUND,
	UWLAN_NEED_PASSWORD,
	UWLAN_PEMISSION_DENIED,
	UWLAN_NW_WILL_NOT_CONNECT,
	UWLAN_NW_TYPE_NOT_SUPPORTED
}uwlanRetVal_t;

/* generic states - not to be confused with NM states */
typedef enum uwlanState_e{
	UWLAN_STATE_INITED = 100,
	UWLAN_STATE_DISCONNECTING,
	UWLAN_STATE_DISCONNECTED,
	UWLAN_STATE_CONNECTING,
	UWLAN_STATE_CONNECTED,
	UWLAN_STATE_SCAN_FAILED,
	UWLAN_STATE_LAST_OP_FAILED
}uwlanState_t;

/* generic callback reasons */
typedef enum uwlanCallBkRsn_e{
	UWLAN_CONNECTED = 1000,
	UWLAN_DISCONNECTED,
	UWLAN_OPERATION_FAILED,
	UWLAN_SCAN_COMPLETED,
	UWLAN_DEVICE_PLUGIN,
	UWLAN_DEVICE_PLUGOUT
}uwlanCallBkRsn_t;

#define UWLAN_GET_STATE(ctrlBlk_p, st)\
do {\
	/* take mutex */\
	take_lock(&((ctrlBlk_p)->stateLock));\
	(st) = (ctrlBlk_p)->state;\
	/*give mutex */\
	release_lock(&((ctrlBlk_p)->stateLock));\
}while(0)


#define UWLAN_SET_STATE(ctrlBlk_p, st)\
do {\
	/* take mutex */\
	take_lock(&((ctrlBlk_p)->stateLock));\
	(ctrlBlk_p)->state = (st);\
	/*give mutex */\
	release_lock(&((ctrlBlk_p)->stateLock));\
}while(0)

/* wlan control block. Add all state variables and 
 * stuff that is needed all through the existence of 
 * the wlan connection. The callers need to pass this 
 * struct to all the APIs
 */
typedef struct uWlanCB_s {
#ifdef WIN32
	HANDLE handle;
	/* for windows we need to pass the if struct to get other stuff, so store it here */
	PWLAN_INTERFACE_INFO_LIST ifInfoLst_p;
#endif
	/* mutex state lock */
    
	U_MUTEX stateLock;
	/* mutex sync lock - required to make async calls as sync */
	USemaphore *syncLock_p;
	uwlanState_t state; /* any state change should be atomic */
	/* pointer to callback function of the application using 
	 * uwlan utility */
	void (*callBk_p)(int reason);
	int lastCallBkRsn; /* the last callback reason - save, what the heck */
}uWlanCB_t;

/* Following is a list of interface and network info structs. The respective
 * OS funstions should convert to the following structures before passing it 
 * higher up */
/* Interface Info */
typedef struct uWlanIfInfo_s{
	unsigned char *ifGUID_p;
	unsigned char *ifDesc_p;
}uWlanIfInfo_t;

typedef struct uWlanIfInfoLst_s{
	int numOfIf; /* number of interfaces */
	uWlanIfInfo_t *ifInfo_p;
}uWlanIfInfoLst_t;

typedef struct uWlanNwInfo_s{
#define UWLAN_SSID_LEN 256
#define UWLAN_BSS_LEN  32
#define UWLAN_AUTH_ALGO_LEN 32
#define UWLAN_CIPHER_ALGO_LEN 32
	char ssid[UWLAN_SSID_LEN];
	int isConnectable; /* True if we can connect to this nw */
	int isConnected; /* True if connected to this n/w */
	int isProfileAvlbl; /* True if profile is present */
	unsigned int sigQuality; /* this is a %age */
	char bssType[UWLAN_BSS_LEN]; /* infra and so on */
	char authAlgo[UWLAN_AUTH_ALGO_LEN];
	char cipherAlgo[UWLAN_CIPHER_ALGO_LEN];
}uWlanNwInfo_t;

typedef struct uWlanNwLst_s{
	unsigned int numOfNw; /* number of interfaces */
	uWlanNwInfo_t *nwInfo_p;
}uWlanNwLst_t;

/* just name in the profile for now */
typedef struct uWlanPrInfo_s{
	char *prName_p;
}uWlanPrInfo_t;

typedef struct uWlanStats_s{
	unsigned int TxRate;
	unsigned int RxRate;
}uWlanStats_t;


/* prototypes */
uwlanRetVal_t uwlanInit (uWlanCB_t **CB_p, void (*callBk_p)(int));
void uwlanUnInit (uWlanCB_t *CB_p);
void uwlanFreeIfLst (uWlanIfInfoLst_t *ifInfoLst_p);
uwlanRetVal_t uwlanGetIfLst (uWlanCB_t *CB_p, uWlanIfInfoLst_t **ifInfoLst_p);
uwlanRetVal_t uwlanConnect (uWlanCB_t *CB_p, 
							char *ssid_p, unsigned int ifIdx, 
							bool synchronously);
uwlanRetVal_t uwlanDisconnect (uWlanCB_t *CB_p, unsigned int idx, bool synchronously);
uwlanRetVal_t uwlanGetStats ();
void uwlanFreeNwLst (uWlanNwLst_t *nwLst_p);
uwlanRetVal_t uwlanGetNwList (uWlanCB_t *CB_p, uWlanNwLst_t **nwLst_p, unsigned int ifIdx);
uwlanRetVal_t uwlanScanNw (uWlanCB_t *CB_p, unsigned int ifIdx, 
						   bool synchronously);
int uwlanProfilePresent (uWlanCB_t *CB_p, unsigned int ifIdx, 
						 char *ssid_p);
int uwlanRVConnected (uWlanCB_t *CB_p, char *ssid_p, 
					  unsigned int ifIdx);
uwlanRetVal_t uwlanAddProfile (uWlanCB_t *CB_p, char *ssid_p, char *shareKey_p,
							   unsigned int ifIdx);
uwlanRetVal_t uwlanDelProfile (uWlanCB_t *CB_p, 
								unsigned int ifIdx,
								 char *ssidStr);
#ifdef WIN32
static void WINAPI uwlanCallBk (PWLAN_NOTIFICATION_DATA pData, void* pContext);
#endif
uwlanRetVal_t uwlanGetStats (uWlanCB_t *CB_p, unsigned int ifIdx, 
							 uWlanStats_t **stats_p);
#endif /* _UWLANUTILS_H_ */