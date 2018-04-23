/*****************************************************************************
**
** uconnStateMachine.h
** This header file describes the interfaces for the UConnect
** State machine
**
** Author: Sathyanarayanan S
** Date  : 31-DEC-10
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		31/12/10     SS			Creation
****************************************************************************/

#ifndef _UCONN_STATE_MACHINE_H_
#define _UCONN_STATE_MACHINE_H_

#include "uconn_IdleState.h"
#include "uconn_SearchingState.h"
#include "uconn_ConnectingState.h"
#include "uconn_ConnectedState.h"
#include "uconn_DisconnectingState.h"
#include "uconn_EnabledState.h"
#include "uconn_DisabledState.h"
#include "UStateMachine.h"
#include "uconn_AT_DAL.h"
//#include "uconn_bcm_DAL.h"
#include "uconn_ServerAPI.h"

#include "uconn_DAL.h"
#include "ThreeG_NM.h"



void 
#ifdef WIN32
CALLBACK
#endif
TimerCallBack(U_PVOID timerHead, U_BOOL val);

void 
#ifdef WIN32
CALLBACK
#endif
TimerCallBack_NM(U_PVOID timerHead, U_BOOL val);

class UCMStateMachine : public UStateMachine
{
private:
protected:
public:
	//stBcmDALInitParams initParams;

	/* Parent States */
	UCMEnabledState *enabledState;
	UCMDisabledState *disabledState;
	
	/* Children of Enabled State */
	UCMIdleState *idleState;
	UCMSearchingState *searchingState;
	UCMConnectingState *connectingState;
	UCMConnectedState *connectedState;
	UCMDisconnectingState *disconnectingState;

	/* Children of Connecting State */
	//UCMDefPrimaryState *primaryState;
	//UCMDefSecondaryState *secondaryState;
	//UCMNetAttachState *netattachState;
    UCMWaitRegStatusAct *wait_for_regstatusAct;
    UCMWaitPrimaryAct *wait_for_primaryAct;

	/* CM Statistics */
	CM_Statistics_s* stats;

	/* Data for the net_attach command */
	U_UINT32 context_id;
	U_PS_CTX_PRIMARY_PARAMS* pri_params;
	U_PS_CTX_SECONDARY_PARAMS* sec_params;
	U_PS_CTX_CFG* ctx_config;
	U_APN_TABLE* apn_table;
	
	/* EVT_PRIMARY_ACT data */
	U_BOOL ipv6_support;
	U_SINT8 ip_addr[MAX_ADDRESS_SIZE];
	U_SINT8 subnet_mask[MAX_ADDRESS_SIZE];
	U_SINT8 gw_addr[MAX_ADDRESS_SIZE];
	U_SINT8 dyndns_server[MAX_ADDRESS_SIZE];
	U_SINT8 intface[50];

    bool    bSyncedWithDevice;

	/* Timer declarations */
	U_TIMERID updateTimer;
	U_TIMERID searchTimer;
	U_TIMERID stateTimer;
	U_TIMERID deviceStateCheckTimer;

	U_TIMERID RSRATimer;
	U_TIMERID throttleTimer;
	U_TIMERID RASDialTimer;
	U_TIMERID ThreeGStatsTimer;
	U_TIMERID USSDTimer;
	
		/* RS attempts when Connect is successful */
	U_UINT32 RS_attempts;
	U_UINT32 IPv6_Throttle_count;
	U_UINT32 PDN_Reject_Throttle_count;
	U_BOOL isThrottleTimerRunning;
	U_BOOL isRASDialTimerRunning;
	U_BOOL isUSSDTimerRunning;

	
	/* Power Up flag */
	bool bPowerUpDone;
	bool isIMSRegnDone;

	/* Call back function */
	//U_VOID (*NM_CBHandler)(UEvent *);

	/* Network Manager pointer */
	ULTE_NM* mLTE_NM;

	UThreeG_NM* mThreeG_NM;

    UCMStateMachine();
    UCMStateMachine(UStateMachine* pSM);
	~UCMStateMachine();
	void UCMStartUp(UNetworkManager* pNM);
	void setCommandId(U_UINT32 cId);
	U_UINT32 getCommandId();
	void throttle_counter_update();

    void reset();

    void resetCMInfo();

	uconnDal  *dal;
	uconnDal *getDal(){
	return dal;
	}

};

//void InitCallback(U_UINT32 length, P_ST_CALLBACK_INFO callbackbuf);
//void InitCallback(string strResp);

#endif
