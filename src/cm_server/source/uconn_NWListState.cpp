/*****************************************************************************
**
** uconn_NWListState.cpp
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
#include "uconn_NWListState.h"

/***********************************************************************
*	UCMNWListState::entry() -
*	NWList State Entry point
***********************************************************************/

void UCMNWListState::entry()
{
	uLogger.log(1, "NWLIST State Entry \n");
}

/***********************************************************************
*	UCMNWListState::handle -
*	NWList State handler
*
*	Parameters
*	evt	- Pointer to the Event type
***********************************************************************/

U_UINT32 UCMNWListState::handle(UEvent *evt)
{
	U_UINT32 iResult = SUCCESS;
	UNwSearchEvent *nwsrchevt = (UNwSearchEvent*)evt;
	UCMStateMachine* ucmsm_p = (UCMStateMachine*)this->getStateMachine();
	U_UINT32 plmns;
	
	uLogger.log(1, "NWLIST State Handle \n");
	/* Retrieve the list of APNs and update nwlist variable */
	if(evt->getCmdId() == UCONN_CMD_LTE_NET_SEARCH_STATUS)
	{
		/* Get the list of networks and update NWListInfo */
		iResult = uconnBcmNetSearchStatus(&nwsrchevt->search_results.PLMN_count,
										nwsrchevt->search_results.plmn_search_entry);
		if(iResult)
		{
			uLogger.errorLog("Error in NetSearch_Status \n");
			gCMInfo.search_results.net_search_status = SRCH_FAILED;
			ucmsm_p->transit(ucmsm_p->idleState);
		}
		else
		{
			/* Copy the results from "search_results" */
			plmns = gCMInfo.search_results.num_plmns 
				= nwsrchevt->search_results.PLMN_count;
			for(U_UINT32 i = 0; i < plmns; i++)
			{
				gCMInfo.search_results.nw_list[i].PLMN_id = 
					nwsrchevt->search_results.plmn_search_entry[i].PLMN_id;
				gCMInfo.search_results.nw_list[i].rat = 
					nwsrchevt->search_results.plmn_search_entry[i].rat;
				gCMInfo.search_results.nw_list[i].list_membership =
					nwsrchevt->search_results.plmn_search_entry[i].list_membership;
				strcpy(gCMInfo.search_results.nw_list[i].free_text, 
					nwsrchevt->search_results.plmn_search_entry[i].free_text);
			}
			/* Go back to IDLE State after getting the list of APNs */
			gCMInfo.search_results.net_search_status = SRCH_COMPLETED;
			ucmsm_p->transit(ucmsm_p->idleState);
		}
	}

	return iResult;
}

/***********************************************************************
*	UCMNWListState::exit() -
*	NWList State Exit point
***********************************************************************/

void UCMNWListState::exit()
{
	uLogger.log(1, "NWLIST State Exit \n");
}