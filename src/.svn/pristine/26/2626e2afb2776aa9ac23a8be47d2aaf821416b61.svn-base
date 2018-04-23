/*****************************************************************************
**
** uconn_IdleState.cpp
** This file describes the methods for the UConnect Idle State
** Machine
**
** Author: Sathyanarayanan S
** Date  : 04-JAN-11
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		04/01/11     SS			Creation
****************************************************************************/

#include "uconn_StateMachine.h"
#include "uconn_ServerAPI.h"
#include "uconn_IdleState.h"
#include "uconn_IdleEvent.h"
#include "uconn_AT_DAL.h"
//#include "uconn_bcm_DAL.h"
#include "ULog.h"

/***********************************************************************
*	UCMIdleState::entry() -
*	Idle State Entry point
***********************************************************************/

void UCMIdleState::entry()
{
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();

	UServerStateEvent *idleStateEvt_p = new UServerStateEvent(IDLE_STATE);
	//SAPI_eventCB((UEvent*)idleStateEvt_p);
	ucmsm_p->mLTE_NM->postEventToBearerManager((UEvent*)idleStateEvt_p);
	ucmsm_p->mLTE_NM->state = NM_READY_STATE;
	ucmsm_p->mLTE_NM->updateNMStateAndPost(LTE_TECH,ucmsm_p->mLTE_NM->state);
	
	/* Read the Configuration here - to be implemented later */
	LOG(ULOG_ERROR_LEVEL, "IDLE State Entry \n");
	//printf("==============IDLE State Entry================ \n");
	//ucmsm_p->mLTE_NM->mNMInfo.state = IDLE_STATE;
	ucmsm_p->mLTE_NM->mNMInfo.state = IDLE_STATE;

	int ctxID = ucmsm_p->mLTE_NM->mNMInfo.context_id;
	ucmsm_p->context_id = ctxID;


	ucmsm_p->getDal()->uconnConfigNetParams(UConnConfigNetParams::CONTEXT_PARAMS,NULL,ctxID);

}

/***********************************************************************
*	UCMIdleState::handle -
*	Idle State handler
*
*	Parameters
*	evt	- Pointer to the Event type
***********************************************************************/

U_UINT32 UCMIdleState::handle(UEvent* evt)
{
	/* In the IDLE state, we could get a Connect or a NW Search command */
	U_UINT32 iResult = UCM_SUCCESS;
	//UIdleEvent *idleEvt = (UIdleEvent*)evt;
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	LOG(ULOG_ERROR_LEVEL, "IDLE State Handle %d \n",evt->getCmdId());
	//printf("IDLE State Handle %d \n",evt->getCmdId());
	/* DAL for Network search or net_connect to be called here */
	switch(evt->getCmdId())
	{
#if 0
	case UCONN_CMD_LTE_NET_SEARCH:
		LOG(ULOG_INFO_LEVEL, "IDLE Handle: NET SEARCH called \n");
		/* Call DAL with idleEvt->plmn_timer */
		iResult = ucmsm_p->getDal()->uconnNetSearch();
		//iResult = uconnATNetSearch(idleEvt->plmn_timer); asper at_command
		ucmsm_p->mLTE_NM->mNMInfo.search_results.net_search_status = SRCH_INPROGRESS;//SRCH_COMPLETED;
		ucmsm_p->searchingState->set_timer(idleEvt->plmn_timer);
		ucmsm_p->transit(ucmsm_p->searchingState);
		break;
#endif
    case UCONN_EVT_IMS_REG_SUCCESS:
        {
            /* For the Auto mode, net_attach ignores the PLMNID, RAT */
			U_UINT32 result = UCM_SUCCESS;
		    //U_UINT32 ctxID = uconn_GetCtxID();
			U_UINT32 ctxID = ucmsm_p->context_id;
		    U_PLMN plmn = new char[20];
            strcpy((U_SINT8*)plmn, "0");
            // plmn.plmnFormat = E_PLMN_ID_NUMERIC;

			U_PS_CTX_PRIMARY_PARAMS pri_params;
			pri_params.u32Bitfields = 0x17;
			pri_params.pdp_type = E_PDP_TYPE_IPV4;
			strcpy((U_SINT8*)pri_params.apn, ucmsm_p->mLTE_NM->mNMInfo.VzAPN);
			strcpy((U_SINT8*)pri_params.pdp_addr, "192.168.0.1");
			pri_params.data_comp = E_DATA_COMP_V42;
			pri_params.hdr_comp = E_HDR_COMP_ROHC;

		
			if(ucmsm_p->bPowerUpDone == false)
			{
				/* Mobile Configuration API */

					result = ucmsm_p->getDal()->uconnNetAttach(CONNECT_MANUAL,NULL,ctxID);
				    ucmsm_p->bPowerUpDone = true;

			}
			else /* Net attach for Default EPS bearer activation */
			{
				result = ucmsm_p->getDal()->uconnNetAttach(CONNECT_DEFAULT,NULL,ctxID);
			}
			if(plmn != NULL)
			{
				delete[] plmn;
				plmn = NULL;
			}
            
			if(result)
            {
			    LOG(ULOG_ERROR_LEVEL, "AUTO mode: Net Attach failed \n");
                /* May have to be in IDLE, till another event is received */
            }
            else
            {
                 ucmsm_p->transit(ucmsm_p->connectingState);
            }
        }
        break;

	case UCONN_CMD_LTE_NET_ATTACH:
	case UCONN_EVT_REQ_ATTACH:
	{
		if(ucmsm_p->isThrottleTimerRunning == true)
		{
			/* If the throttle Timer is running don't process the Request */
			break;
		}
        if(ucmsm_p->mLTE_NM->mCommonInfo.imsParams.Test_Mode == TEST_MODE_ENABLE)
        {
            /* For the Auto mode, net_attach ignores the PLMNID, RAT */
			U_UINT32 result = UCM_SUCCESS;
		    //U_UINT32 ctxID = uconn_GetCtxID();
			U_UINT32 ctxID = ucmsm_p->context_id;
		    U_PLMN plmn = new char[20];
            strcpy((U_SINT8*)plmn, "0");
           // plmn.plmnFormat = E_PLMN_ID_NUMERIC;

			if(ucmsm_p->bPowerUpDone == false)
			{
				/* Mobile Configuration API */
				result = ucmsm_p->getDal()->uconnNetAttach(CONNECT_MANUAL,NULL,ctxID);
				ucmsm_p->bPowerUpDone = true;
				
			}
			else /* Net attach for Default EPS bearer activation */
			{
				result = ucmsm_p->getDal()->uconnNetAttach(CONNECT_DEFAULT,NULL,ctxID); 
				//result = ucmsm_p->getDal()->uconnNetAttach(CONNECT_DEFAULT,NULL
					//result = uconnATNetAttach_DefaultEPS(ucmsm_p->mLTE_NM->mCommonInfo.cfg_params.Config_mode,
											//	plmn, E_RAT_EUTRAN, ctxID); // asper at_command
			}
			if(plmn != NULL)
			{
				delete[] plmn;
				plmn = NULL;
			}
            if(result)
            {
			    LOG(ULOG_ERROR_LEVEL, "AUTO mode: Net Attach failed \n");
                /* May have to be in IDLE, till another event is received */
            }
            else
            {
                 ucmsm_p->transit(ucmsm_p->connectingState);
            }
        }
        else        /* If Test mode is disabled, IMS regn has to be done */
        {
			U_UINT32 result = UCM_SUCCESS;
		    U_UINT32 ctxID = ucmsm_p->context_id;
		    U_PLMN plmn = new char[20];
            strcpy((U_SINT8*)plmn, "0");
           

            /* Call the IMS Registration Related AT commands */
            

			iResult=ucmsm_p->getDal()->uconnConfigure(IMS_CONFIG);

            iResult = ucmsm_p->getDal()->uconnConfigure(IMS_RESPONSE);

			if(ucmsm_p->bPowerUpDone == false)
			{
				/* Mobile Configuration API */
				result = ucmsm_p->getDal()->uconnNetAttach(CONNECT_MANUAL,NULL,ctxID);
				ucmsm_p->bPowerUpDone = true;
			}
			else /* Net attach for Default EPS bearer activation */
			{
				result = ucmsm_p->getDal()->uconnNetAttach(CONNECT_DEFAULT,NULL,ctxID);
				//result = uconnATNetAttach_DefaultEPS(ucmsm_p->mLTE_NM->mCommonInfo.cfg_params.Config_mode,
										//	plmn, E_RAT_EUTRAN, ctxID); //asper at_command
			}

			if(plmn != NULL)
			{
				delete[] plmn;
				plmn = NULL;
			}
            
			if(result)
            {
			    LOG(ULOG_ERROR_LEVEL, "AUTO mode: Net Attach failed \n");
                /* May have to be in IDLE, till another event is received */
            }
            else
            {
                 ucmsm_p->transit(ucmsm_p->connectingState);
            }
        }
    }
    break;
#if 0//asper at_command
	case UCONN_EVT_LTE_REG_STATUS:
		{
		/* Handle events from the modem */
		ULTERegStatusEvent *regEvt = (ULTERegStatusEvent *) evt;
		ucmsm_p->mLTE_NM->mNMInfo.lte_reg_status = regEvt->getRegStatus();	/* Get the LTE Status from event */
		}
		break;
#endif /* if 0 */
    case UCONN_EVT_IMS_REG_FAILURE:
        {
            LOG(ULOG_ERROR_LEVEL, "IMS Registration Failed \n");
            ucmsm_p->transit(ucmsm_p->idleState);
        }
        break;
	case UCONN_EVT_READY_FOR_ATTACH:
		{
			LOG(ULOG_ERROR_LEVEL, "Received UCONN_EVT_READY_FOR_ATTACH Event in idle state \n");
			//printf("Received UCONN_EVT_READY_FOR_ATTACH\n");
		}
		break;
	default:
		return UCMEnabledState::handle(evt);
		break;
	}

	return iResult;
}

/***********************************************************************
*	UCMIdleState::exit() -
*	Idle State Exit point
***********************************************************************/

void UCMIdleState::exit()
{
	LOG(ULOG_ERROR_LEVEL, "IDLE State Exit \n");
	//printf("==============IDLE State Exit================ \n");
	/* Exit this state when a Connect command is received and move to the 
	CONNECTING state */
}
