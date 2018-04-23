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
#ifndef UCONN_SEARCH_EVENT_H_
#define UCONN_SEARCH_EVENT_H_

#include "uconn_Event.h"
#include "uconn_AT_DAL.h"

class USearchEvent:UEvent
{
private:
	
public:
	USearchEvent(U_UINT32 timer):UEvent(UCONN_CMD_LTE_NET_SEARCH){
				   plmn_timer = timer;
				   setSize(sizeof(*this));
				   }
	USearchEvent():UEvent(UCONN_CMD_LTE_NET_SEARCH_STATUS){
		setSize(sizeof(*this));

	}
	U_UINT32 plmn_timer;
	U_APN_TABLE search_results;	
};

#endif