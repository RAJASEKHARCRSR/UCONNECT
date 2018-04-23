/*----------------------------------------------------------------
	uconn_NwSearchEvent.h

	This header file describes the interfaces for the NWSearch State
	events
	
	Author: Sathyanarayanan S
	Date  : 29-DEC-2010
*/



/*
    Revision History

	  DATE		VER		AUTHOR		DESCRIPTION
	29-DEC-10	0.1		 SS	    Creation - Added this header
----------------------------------------------------------------*/
#ifndef UCONN_NW_SEARCH_EVENT_H_
#define UCONN_NW_SEARCH_EVENT_H_

#include "uconn_Event.h"

class UNwSearchEvent:UEvent
{
private:
	
public:
	UNwSearchEvent(U_UINT32 timer):UEvent(UCONN_CMD_LTE_NET_SEARCH){
				   plmn_timer = timer;
				   }
	UNwSearchEvent():UEvent(UCONN_CMD_LTE_NET_SEARCH_STATUS){

	}
	U_UINT32 plmn_timer;
	T_PLMN_EARCH_RESULT_LIST search_results;	
};

#endif