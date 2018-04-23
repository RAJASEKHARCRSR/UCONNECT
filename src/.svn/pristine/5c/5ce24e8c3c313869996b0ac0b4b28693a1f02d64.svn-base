/*----------------------------------------------------------------
	uconn_ConnectingEvent.h

	This header file describes the interfaces for the Connecting State
	events
	
	Author: Sathyanarayanan S
	Date  : 07-JAN-2011
*/



/*
    Revision History

	  DATE		VER		AUTHOR		DESCRIPTION
	07-JAN-11	0.1		 SS	    Creation - Added this header
----------------------------------------------------------------*/
#ifndef UCONN_CONNECTING_EVENT_H_
#define UCONN_CONNECTING_EVENT_H_

#include "uconn_IdleEvent.h"

class UConnectingEvent:UEvent
{
private:
	
public:
	/* Declare all the variables for the events that would be handled
	by the CONNECTING state */
	UConnectingEvent(U_UINT32 cmdID):UEvent(cmdID)
	{
		//apn_table = new T_APN_TABLE();
		setSize(sizeof(*this));
	}
	U_EVT_PRIMARY_ACT pri_evt;
	U_APN_TABLE  apn_table;

};

#endif