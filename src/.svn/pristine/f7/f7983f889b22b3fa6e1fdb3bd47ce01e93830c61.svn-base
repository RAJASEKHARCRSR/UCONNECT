/*****************************************************************************
**
** uconn_ConnectingState.cpp
** This file describes the methods for the UConnect Connecting State
** Machine
**
** Author: Sathyanarayanan S
** Date  : 04-JAN-11
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		04/01/11     SS			Creation
****************************************************************************/

#include "uconn_StateMachine.h"
#include "uconn_ConnectingState.h"
#include "uconn_ServerAPI.h"
#include "uconn_ConnectingEvent.h"


#ifdef WIN32
#include "windows.h"
#include "conio.h"
#include "string.h"
#include "Objbase.h"
#ifndef QTPROJECT
#include "Wbemidl.h"
#endif
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#endif



/***********************************************************************
*	UCMConnectingState::entry() -
*	Connecting State Entry point
***********************************************************************/

void UCMConnectingState::entry()
{
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();

	UServerStateEvent *connectingStateEvt_p = new UServerStateEvent(CONNECTING_STATE);
	ucmsm_p->mLTE_NM->postEventToBearerManager((UEvent*)connectingStateEvt_p);

	LOG(ULOG_INFO_LEVEL, "CONNECTING State Entry \n");
	ucmsm_p->mLTE_NM->mNMInfo.state = CONNECTING_STATE;
	
	/* Initialize the State time out Timer for the Disconnecting State */
	if(ucmsm_p->stateTimer == NULL)
	{
		ucmsm_p->stateTimer = USetTimer(UCONN_SM_TIMEOUT, 0, (U_CALLBACK)TimerCallBack);
	}
	
	/* In the AUTO mode, move immediately to the Primary state */
	ucmsm_p->setState(ucmsm_p->wait_for_primaryAct);//asper at_command 
	ucmsm_p->getState()->entry();
}

/***********************************************************************
*	UCMConnectingState::handle -
*	Connecting State handler
*
*	Parameters
*	evt	- Pointer to the Event type
***********************************************************************/

U_UINT32 UCMConnectingState::handle(UEvent *evt)
{
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	U_UINT32 result = UCM_SUCCESS;
	/*UIdleEvent* idleEvt = (UIdleEvent*)evt;
	U_UINT32 ctxID = uconn_GetCtxID();
	idleEvt->context_id = ctxID;*/

	LOG(ULOG_INFO_LEVEL, "CONNECTING State Handle %d\n",evt->getCmdId());

	switch(evt->getCmdId())
	{
	case UCONN_EVT_LTE_REG_STATUS:
        {
			ULTERegStatusEvent * regEvt = (ULTERegStatusEvent *) evt;
			
			/* Registration accepted, wait for the EVT_PRIMARY_ACT */
			LOG(ULOG_INFO_LEVEL, "EVT_LTE_REG_STATUS received \n");

			ucmsm_p->mLTE_NM->mNMInfo.lte_reg_status = regEvt->getRegStatus();
			
			if(((ucmsm_p->mLTE_NM->mNMInfo.lte_reg_status == 1) ||
			(ucmsm_p->mLTE_NM->mNMInfo.lte_reg_status == 5)) &&  //asper at_command
			(ucmsm_p->isIMSRegnDone == true))
			{
				if(ucmsm_p->stateTimer)
				{
					/* Kill the State Timeout Timer before going to the next State */
					UKillTimer(ucmsm_p->stateTimer);
					ucmsm_p->stateTimer = NULL;
				}
					
				ucmsm_p->transit(ucmsm_p->connectedState);
			}
			else if(ucmsm_p->mLTE_NM->mNMInfo.lte_reg_status == 0)
			{
				ucmsm_p->getDal()->uconnNetAttach(CONNECT_DEFAULT,NULL,ucmsm_p->context_id); 
			}
		}
		break;
	default:
		{
			return UCMEnabledState::handle(evt);
		}
		break;
	}
	return result;
}

/***********************************************************************
*	UCMConnectingState::exit() -
*	Connecting State Exit point
***********************************************************************/

void UCMConnectingState::exit()
{
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	LOG(ULOG_INFO_LEVEL, "CONNECTING State Exit \n");
	//if(ucmsm_p->stateTimer)
	//{
	//	/* Kill the State Timeout Timer before going to the next State */
	//	UKillTimer(ucmsm_p->stateTimer);
	//	ucmsm_p->stateTimer = NULL;
	//}
}

/*********************CONNECTING state children*********************/

/***********************************************************************

*	UCMDefRegStatusAct::entry() -
*	Define  wating_for_regstatus Entry point
***********************************************************************/
void UCMWaitRegStatusAct::entry()
{
    UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	LOG(ULOG_ERROR_LEVEL, "Waiting for regstatus event entry \n");
	ucmsm_p->mLTE_NM->mNMInfo.state = WAITING_FOR_REGSTATUS_ACT;
}
/***********************************************************************
*	UCMDefRegStatusAct::handle -
*	Define wating_for_regstatus handler
*
*	Parameters
*	evt	- Pointer to the Event type
***********************************************************************/

U_UINT32 UCMWaitRegStatusAct::handle(UEvent *evt)
{
    U_UINT32 result = UCM_SUCCESS;
    UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();

	LOG(ULOG_DEBUG_LEVEL, "wating for regstatus evt handle %d   %d\n",evt->getCmdId(),evt->getSize());
    switch(evt->getCmdId())
	{
	case UCONN_EVT_IMS_REG_SUCCESS:
		LOG(ULOG_INFO_LEVEL, "UCONN_EVT_IMS_REG_SUCCESS received \n");
		ucmsm_p->isIMSRegnDone = true;
		if(ucmsm_p->mLTE_NM->mCommonInfo.imsParams.IMS_Attach_Only == 1)
		{
			//uconnATPsCtxInfoGet(ucmsm_p->context_id, E_INFO_APN_NAME);
			ucmsm_p->transit(ucmsm_p->connectedState);
		}
		else /* Need to proceed with PDN attach now */
		{//asper at_command
			/* Query the Contexts available */

			ucmsm_p->getDal()->uconnGetStatus(UConnGetStatus::AT_QUERY_CONTEXT,NULL);
			
		}
        break;
	case UCONN_EVT_CTX_QUERY_RSP:
		{
			/* Steps to setup a context for the PDN attach and then AT+CGACT */
			U_PS_CTX_PRIMARY_PARAMS pri_params;
			pri_params.u32Bitfields = 0x17;
			pri_params.pdp_type = E_PDP_TYPE_IPV4V6;
			strcpy((U_SINT8*)pri_params.apn, ucmsm_p->mLTE_NM->mNMInfo.VzAPN);
			strcpy((U_SINT8*)pri_params.pdp_addr, "10.10.10.4");
			pri_params.data_comp = E_DATA_COMP_V42;
			pri_params.hdr_comp = E_HDR_COMP_ROHC;

			ucmsm_p->getDal()->uconnConfigNetParams(PRIMARY_CTX,(void*)&pri_params,5);
			
//asper at_command
			/* Activate the Context */
			
			UCtxQueryRspEvent* ctxQueryEvt = (UCtxQueryRspEvent*)evt;

			/* Set the context id for the PDN net attach */
			ucmsm_p->context_id = ctxQueryEvt->ctxIDinUse + 1;

			ucmsm_p->getDal()->uconnConfigNetParams(PRIMARY_CTX,(void*)&pri_params,ucmsm_p->context_id);

			ucmsm_p->getDal()->uconnSetStatus(ATP_CTX_SET,NULL,ucmsm_p->context_id);
			
			
				
			/* Set the PCO parameters for this context */
			
			
			/* Activate the Context */

			ucmsm_p->getDal()->uconnConfigNetParams(CTX_ID,NULL,ucmsm_p->context_id);

			/* Request the Registration Status once again */

			ucmsm_p->getDal()->uconnGetStatus(GET_NET_STATUS,NULL);

			//asper at_command
		}
        break;
    default:
        {
		return UCMConnectingState::handle(evt);
        }
		break;
    }
	return result;
}

/***********************************************************************
*	UCMDefRegStatusAct::exit() -
*	Define wating_for_regstatus Exit point
***********************************************************************/

void UCMWaitRegStatusAct::exit()
{
	LOG(ULOG_DEBUG_LEVEL, "wating for regstatus evt Exit \n");
}

/***********************************************************************
*	UCMDefPrimaryState::entry() -
*	Define Primary State Entry point
***********************************************************************/
void UCMWaitPrimaryAct::entry()
{
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	UServerStateEvent *connectingStateEvt_p = new UServerStateEvent(CONNECTING_STATE);
	ucmsm_p->mLTE_NM->postEventToBearerManager((UEvent*)connectingStateEvt_p);
	
	LOG(ULOG_DEBUG_LEVEL, "waiting for primary act \n");
	ucmsm_p->mLTE_NM->mNMInfo.state = WAITING_FOR_PRIMARY_ACT;
}

/***********************************************************************
*	UCMDefPrimaryAct::handle -
*	wait_for_primaryact handler
*
*	Parameters
*	evt	- Pointer to the Event type
***********************************************************************/

U_UINT32 UCMWaitPrimaryAct::handle(UEvent *evt)
{
	U_UINT32 result = UCM_SUCCESS;
//	UIdleEvent* idleEvt = (UIdleEvent*)evt;
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
//	U_UINT32 ctxID = idleEvt->context_id;

	LOG(ULOG_DEBUG_LEVEL, "waiting for primary act Handle %d   %d\n",evt->getCmdId(),evt->getSize());

	switch(evt->getCmdId())
	{
	case UCONN_EVT_PRIMARY_ACT:
		/* Move from this state to net_attach state on this event */
		/* Store the event data into the CM persistence structure */
	{
		UConnectingEvent* connEvt = (UConnectingEvent*)evt;

		/* Update the ucmsm_p->mLTE_NM->mNMInfo structure with the Primary details */
		ucmsm_p->mLTE_NM->mNMInfo.primary_ctx.context_id = connEvt->pri_evt.ctx_id;

		/* Set APN Table from the info received via pri_evt */
		//connEvt->apn_table.u32ApnTableEntryCount = 1;

		ucmsm_p->ipv6_support = false;
		if((ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV6) ||
			(ucmsm_p->mLTE_NM->mNMInfo.pdp_type == E_PDP_TYPE_IPV4V6))
		{
			ucmsm_p->ipv6_support = true;
		}
		LOG(ULOG_INFO_LEVEL, "EVT_PRIMARY_ACT received \n");
#if 0
		result = uconnATpropAPNTableSet(connEvt->apn_table);
		if(result)
		{
			result = ERROR_APN_TABLE_SET;
			return result;
		}

		result = ucmsm_p->getDal()->uconnSetStatus(IP_IF_STATUS_UP,NULL);
		if(result)
		{
			result = ERROR_IP_IF_STATUS;
			return result;
		}
#endif	
			if(ucmsm_p->stateTimer)
			{
			/* Kill the State Timeout Timer before going to the next State */
				UKillTimer(ucmsm_p->stateTimer);
				ucmsm_p->stateTimer = NULL;
			}
//		asper at_command	call cereg?

			ucmsm_p->getDal()->uconnGetStatus(UConnGetStatus::GET_NET_STATUS,NULL);

			ucmsm_p->transit(ucmsm_p->wait_for_regstatusAct);
		}
		break;
	default:
		return UCMConnectingState::handle(evt);
		break;
	}

	return result;
}

/***********************************************************************
*	UCMDefPrimaryAct::exit() -
*	waiting_for_primaryact Exit point
***********************************************************************/

void UCMWaitPrimaryAct::exit()
{
	LOG(ULOG_INFO_LEVEL, "waiting for primary act Exit \n");
}
