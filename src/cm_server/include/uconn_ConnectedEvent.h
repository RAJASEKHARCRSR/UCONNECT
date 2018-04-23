/*----------------------------------------------------------------
	uconn_ConnectedEvent.h

	This header file describes the interfaces for the Connected State
	events
	
	Author: Sathyanarayanan S
	Date  : 07-JAN-2011
*/



/*
    Revision History

	  DATE		VER		AUTHOR		DESCRIPTION
	07-JAN-11	0.1		 SS	    Creation - Added this header
----------------------------------------------------------------*/
#ifndef UCONN_CONNECTED_EVENT_H_
#define UCONN_CONNECTED_EVENT_H_

#include "uconn_Event.h"

class UConnectedEvent:UEvent
{
private:
	
public:
	/* Declare all the variables for the events that would be handled
	by the CONNECTED state */
	UConnectedEvent():UEvent(UCONN_CMD_LTE_NET_DETACH){
	setSize(sizeof(*this));
	}
	
};

#endif