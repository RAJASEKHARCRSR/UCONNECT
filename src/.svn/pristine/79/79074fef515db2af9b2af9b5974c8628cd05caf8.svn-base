/*****************************************************************************
**
** uconn_DisconnectingState.cpp
** This file describes the methods for the UConnect Disconnecting State
** Machine
**
** Author: Sathyanarayanan S
** Date  : 04-JAN-11
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		04/01/11     SS			Creation
****************************************************************************/

#include "uconn_StateMachine.h"
#include "uconn_DisconnectingState.h"
#include "uconn_ServerAPI.h"

/***********************************************************************
*	UCMDisconnectingState::entry() -
*	Disconnecting State Entry point
***********************************************************************/

void UCMDisconnectingState::entry()
{
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	UServerStateEvent *disconnectingStateEvt_p = new UServerStateEvent(DISCONNECTING_STATE);
	ucmsm_p->mLTE_NM->postEventToBearerManager((UEvent*)disconnectingStateEvt_p);

	LOG(ULOG_ERROR_LEVEL, "DISCONNECTING State Entry \n");
	ucmsm_p->mLTE_NM->mNMInfo.state = DISCONNECTING_STATE;
	
	/* Initialize the State time out Timer for the Disconnecting State */
	if(ucmsm_p->stateTimer == NULL)
	{
		ucmsm_p->stateTimer = USetTimer(UCONN_SM_TIMEOUT, 0,(U_CALLBACK)TimerCallBack);
	}

}

/***********************************************************************
*	UCMDisconnectingState::handle -
*	Disconnecting State handler
*
*	Parameters
*	evt	- Pointer to the Event type
***********************************************************************/

U_UINT32 UCMDisconnectingState::handle(UEvent *evt)
{
	U_UINT8 iResult = UCM_SUCCESS;
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	LOG(ULOG_ERROR_LEVEL, "DISCONNECTING Handle  %d\n",evt->getCmdId());

	switch(evt->getCmdId())
	{
	case UCONN_EVT_PRIMARY_DEACT:
		{
			/* Wait for Primary deactivation */
			LOG(ULOG_INFO_LEVEL, "Primary Deactivated\n");
		}
		break;
	case UCONN_EVT_SECONDARY_DEACT:
		{
			/* Wait for Secondary deactivation */
			LOG(ULOG_INFO_LEVEL, "Secondary Deactivated\n");
		}
		break;
	case UCONN_EVT_LTE_REG_STATUS:
		{	
		ULTERegStatusEvent *regEvt = (ULTERegStatusEvent *) evt;

		ucmsm_p->mLTE_NM->mNMInfo.lte_reg_status = regEvt->getRegStatus();
			if((ucmsm_p->mLTE_NM->mNMInfo.lte_reg_status != 1))// && 
			//	(ucmsm_p->mLTE_NM->mNMInfo.primary_ctx.status == PRIMARY_DEACTIVATED))
			{
				/* Call DAL for IP IF Down */
	
				iResult = ucmsm_p->getDal()->uconnSetStatus(IP_IF_STATUS_DOWN,NULL,ucmsm_p->context_id);
				 //asper at_command
				if(iResult == UCM_SUCCESS)
				{
					/* Bring down the host's interface */
					setIfStatus(ucmsm_p->intface, "down");
					ucmsm_p->transit(ucmsm_p->idleState);
				}
			}
		}
		break;
	case UCONN_EVT_NET_DETACH:
		{
			ucmsm_p->mLTE_NM->state = NM_NOT_READY_STATE;
			ucmsm_p->mLTE_NM->updateNMStateAndPost(LTE_TECH,ucmsm_p->mLTE_NM->state);

			LOG(ULOG_DEBUG_LEVEL, 
				"NET DETACH Confirmed by LTE Stack...\n");
			/* Call Prop_IP_IF_Status and the Set the status DOWN */
			ucmsm_p->getDal()->uconnSetStatus(IP_IF_STATUS_DOWN,NULL,ucmsm_p->context_id);
			//asper at_command
			ucmsm_p->getDal()->uconnReset(ucmsm_p->apn_table);
			
			if(ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV4 || ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IP)
			{
                
#ifdef WIN32                
				deleteIPaddress(ucmsm_p->intface, ucmsm_p->mLTE_NM->mNMInfo.ip_address, false);
#endif                
			}
			
			if(ucmsm_p->mLTE_NM->mNMInfo.isPowerdown)
			{
				ucmsm_p->reset();	/* Necessary to clear the history */
				ucmsm_p->transit(ucmsm_p->enabledState);
			}
			else
			{
				ucmsm_p->transit(ucmsm_p->idleState);
			}
		}
		break;
	default:
		return UCMEnabledState::handle(evt);
		break;
	}
	
	return iResult;
}

/***********************************************************************
*	UCMDisconnectingState::exit() -
*	Disconnecting State Exit point
***********************************************************************/

void UCMDisconnectingState::exit()
{
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	LOG(ULOG_ERROR_LEVEL, "DISCONNECTING State Exit \n");

	/* Reset CM Info when moving from Disconnected to any other state */
	ucmsm_p->resetCMInfo();
	if(ucmsm_p->stateTimer)
	{
		/* Kill the State Timeout Timer before going to the next State */
		UKillTimer(ucmsm_p->stateTimer);
		ucmsm_p->stateTimer = NULL;
	}
}