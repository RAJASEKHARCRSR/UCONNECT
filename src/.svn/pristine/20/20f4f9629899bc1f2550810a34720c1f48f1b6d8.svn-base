/*****************************************************************************
**
** uconn_SearchingState.cpp
** This file describes the methods for the UConnect NWList State
** Machine
**
** Author: Sathyanarayanan S
** Date  : 04-JAN-11
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		04/01/11     SS			Creation
*****************************************************************************/

#include "uconn_StateMachine.h"
#include "uconn_SearchingState.h"

/***********************************************************************
*	UCMSearchingState::UCMSearchingState() -
*	Searching State constructor
***********************************************************************/

UCMSearchingState::UCMSearchingState(UStateMachine *tSM):UCMEnabledState(tSM)
{
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	ucmsm_p->searchTimer = NULL;
};

/***********************************************************************
*	UCMSearchingState::entry() -
*	NWList State Entry point
***********************************************************************/

void UCMSearchingState::entry()
{
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();

	UServerStateEvent *searchStateEvt_p = new UServerStateEvent(SEARCHING_STATE);
	ucmsm_p->mLTE_NM->postEventToBearerManager((UEvent*)searchStateEvt_p);

	LOG(ULOG_ERROR_LEVEL, "SEARCHING State Entry \n");
	ucmsm_p->mLTE_NM->mNMInfo.state = SEARCHING_STATE;
	/* Setup the timer process to update CM_Info structure */
	if((ucmsm_p->searchTimer == NULL))
	{
		ucmsm_p->searchTimer = USetTimer(get_timer() * 1000, 0,(U_CALLBACK)TimerCallBack);
	}

	
}

/***********************************************************************
*	UCMSearchingState::handle -
*	NWList State handler
*
*	Parameters
*	evt	- Pointer to the Event type
***********************************************************************/

U_UINT32 UCMSearchingState::handle(UEvent *evt)
{
	U_UINT32 iResult = UCM_SUCCESS;
	USearchEvent *nwsrchevt = (USearchEvent*)evt;
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	U_UINT32 plmns;
	
	LOG(ULOG_ERROR_LEVEL, "SEARCHING State Handle %d \n",evt->getCmdId());
	ucmsm_p->mLTE_NM->mNMInfo.state = SEARCHING_STATE;

	/* Retrieve the list of APNs and update nwlist variable */
	switch(evt->getCmdId())
	{
	case UCONN_CMD_LTE_NET_SEARCH_STATUS:
		/* Get the list of networks and update NWListInfo */
		if(ucmsm_p->mLTE_NM->mNMInfo.search_results.net_search_status == SRCH_COMPLETED)
		{
			LOG(ULOG_ERROR_LEVEL, "Error in NetSearch_Status \n");
			ucmsm_p->mLTE_NM->mNMInfo.search_results.net_search_status = SRCH_FAILED;
			ucmsm_p->transit(ucmsm_p->idleState);
		}
		else
		{
			/* Copy the results from "search_results" */
			plmns = ucmsm_p->mLTE_NM->mNMInfo.search_results.num_plmns 
				= nwsrchevt->search_results.PLMN_count;
			for(U_UINT32 i = 0; i < plmns; i++)
			{
				// T_PLMN structure has changed
/*
				ucmsm_p->mLTE_NM->mNMInfo.search_results.nw_list[i].PLMN_id = 
					nwsrchevt->search_results.plmn_search_entry[i].PLMN_id;
*/
				/*ucmsm_p->mLTE_NM->mNMInfo.search_results.nw_list[i].rat = 
					nwsrchevt->search_results.plmn_search_entry[i].rat;
				ucmsm_p->mLTE_NM->mNMInfo.search_results.nw_list[i].list_membership =
					nwsrchevt->search_results.plmn_search_entry[i].list_membership;
				strcpy(ucmsm_p->mLTE_NM->mNMInfo.search_results.nw_list[i].free_text, 
					nwsrchevt->search_results.plmn_search_entry[i].free_text);*/
			}
			/* Go back to IDLE State after getting the list of APNs */
			ucmsm_p->mLTE_NM->mNMInfo.search_results.net_search_status = SRCH_COMPLETED;
			ucmsm_p->transit(ucmsm_p->idleState);
		}
		break;

	case UCONN_EVT_TIMEOUT:
	{
		UTimeOutEvent* timeoutevt = (UTimeOutEvent*)evt;
        if(ucmsm_p->updateTimer == timeoutevt->getTimerId())
		{
			U_APN_TABLE search_results;
			search_results.PLMN_count = 0;
			/* Timer event for CM Info structure update */

			iResult = ucmsm_p->getDal()->uconnNetSearchStatus();

			/* For Anite DT testing */
			//iResult = 1;
			if(iResult)
			{
				LOG(ULOG_ERROR_LEVEL, "Error in NetSearch_Status \n");
				ucmsm_p->mLTE_NM->mNMInfo.search_results.net_search_status = SRCH_FAILED;
				ucmsm_p->transit(ucmsm_p->idleState);
				UKillTimer(ucmsm_p->searchTimer);
			    ucmsm_p->searchTimer = NULL;
            }
			else if(search_results.PLMN_count > 0)
			{

				/* Go back to IDLE State after getting the list of APNs */
				ucmsm_p->mLTE_NM->mNMInfo.search_results.net_search_status = SRCH_COMPLETED;
				/* Copy the results from "search_results" */
				plmns = ucmsm_p->mLTE_NM->mNMInfo.search_results.num_plmns 
					= search_results.PLMN_count;
				for(U_UINT32 i = 0; i < plmns; i++)
				{
					/* ucmsm_p->mLTE_NM->mNMInfo.search_results.nw_list[i].PLMN_id = 
						search_results.plmn_search_entry[i].PLMN_id; */
					/*ucmsm_p->mLTE_NM->mNMInfo.search_results.nw_list[i].rat = 
						search_results.plmn_search_entry[i].rat;
					ucmsm_p->mLTE_NM->mNMInfo.search_results.nw_list[i].list_membership =
						search_results.plmn_search_entry[i].list_membership;
					strcpy(ucmsm_p->mLTE_NM->mNMInfo.search_results.nw_list[i].free_text, 
						search_results.plmn_search_entry[i].free_text);*/
				}
				/* Go back to IDLE State after getting the list of APNs */
				ucmsm_p->mLTE_NM->mNMInfo.search_results.net_search_status = SRCH_COMPLETED;
				ucmsm_p->transit(ucmsm_p->idleState);
				UKillTimer(ucmsm_p->searchTimer);
				ucmsm_p->searchTimer = NULL;
		}
		else{
                ucmsm_p->mLTE_NM->mNMInfo.search_results.net_search_status = SRCH_INPROGRESS;
            }
			return UCMEnabledState::handle(evt);
		}
		else  if(ucmsm_p->searchTimer == timeoutevt->getTimerId())
		{
			LOG(ULOG_INFO_LEVEL, "Net Search Timed Out \n");
			ucmsm_p->mLTE_NM->mNMInfo.search_results.net_search_status = SRCH_FAILED;
		    ucmsm_p->transit(ucmsm_p->idleState);
		}
		break;
	}
	
	default:
		return UCMEnabledState::handle(evt);
		break;
	}

	return iResult;
}
/***********************************************************************
*	UCMSearchingState::exit() -
*	NWList State Exit point
***********************************************************************/

void UCMSearchingState::exit()
{
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	LOG(ULOG_ERROR_LEVEL, "SEARCHING State Exit \n");
	if(ucmsm_p->stateTimer)
	{
		/* Kill the State Timeout Timer before going to the next State */
		UKillTimer(ucmsm_p->stateTimer);
		ucmsm_p->stateTimer = NULL;
	}
}